//
// Created by felix on 18.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_PLY_ADAPTOR_H
#define CMS_PCS_OPTIMIZER_PLY_ADAPTOR_H
#include <memory>
#include "../pointclouds.h"
#include "cilantro/utilities/ply_io.hpp"
#include "spdlog/spdlog.h"
#include "datastructures.h"
#include "helpers.h"

using namespace tinyply;
template <typename TScalarType> inline Type PlyEigenTypeMap(){return Type::INVALID;};
template <> inline Type PlyEigenTypeMap<int8_t>(){return Type::INT8;};
template <> inline Type PlyEigenTypeMap<int16_t>(){return Type::INT16;};
template <> inline Type PlyEigenTypeMap<int32_t>(){return Type::INT32;};
template <> inline Type PlyEigenTypeMap<uint8_t>(){return Type::UINT8;};
template <> inline Type PlyEigenTypeMap<uint16_t>(){return Type::UINT16;};
template <> inline Type PlyEigenTypeMap<uint32_t>(){return Type::UINT32;};
template <> inline Type PlyEigenTypeMap<float>(){return Type::FLOAT32;};
template <> inline Type PlyEigenTypeMap<double>(){return Type::FLOAT64;};

struct IAdaptor{
    virtual bool init(const Path& input_path) = 0; //{throw;};
    virtual PointCloudRequirements infer_requirements() = 0; //{throw;};
    virtual void set_metadata(const Metadata& meta) = 0;
    virtual void read() =0; //{throw;}
    virtual const PlyMapping& get_property_mapping() const=0;
    virtual void set_property_mapping(const PlyMapping& mapping)=0;

};

class PlyAdaptor : public IAdaptor{
    private:
        std::vector<uint8_t> byte_buffer;
        std::unique_ptr<std::istream> filestream;
        std::unique_ptr<PlyFile> plyfile;

        PlyMapping _m;
        Metadata meta;
        std::shared_ptr<PlyData> vertices;
        std::shared_ptr<PlyData> normals;
        std::shared_ptr<PlyData> colors;
        std::shared_ptr<PlyData> scalarfieds;
        std::shared_ptr<PlyData> classes;


        std::tuple<std::string, std::initializer_list<std::string>> map_properties(const std::string &element_name, std::initializer_list<std::string> properties){
            auto lename = to_lower(element_name);
            std::vector<std::string> props;
            for(const auto& p : properties){
                props.emplace_back(_m[lename][to_lower(p)]);
            }
            return {_m[lename][lename], properties};
        }
        std::shared_ptr<PlyData> request_element(const std::string &element_name, std::initializer_list<std::string> properties) {
            auto [name, _properties] = map_properties(element_name, properties);
            return plyfile->request_properties_from_element(name, _properties);
        }
        void set_metadata(const Metadata& _meta) override;
        template<class TEigenType>
        void propose_element(const std::string &element_name, std::initializer_list<std::string> properties,
                             const TEigenType& data, Type fault_type=Type::INVALID, int fault_value = 0) {

            auto [name, _properties] = map_properties(element_name, properties);
            return plyfile->add_properties_to_element(name, _properties, PlyEigenTypeMap<typename TEigenType::Scalar>(), data.cols(),
                                                      reinterpret_cast<const uint8_t*>(data.data()), fault_type, fault_value);
        }
        PointCloudRequirements determin_requirements();

    public:
        PlyAdaptor(PlyFile& file, PlyMapping& map);
        PlyAdaptor() : plyfile(new PlyFile){}
        PlyAdaptor(PlyMapping& map) : plyfile(new PlyFile), _m(map){};

        bool init(const Path& input_path) override;
        PointCloudRequirements infer_requirements() override;
        void read() override;
        inline const PlyMapping& get_property_mapping() const override{
            return _m;
        }
        inline void set_property_mapping(const PlyMapping& mapping) override{
            _m = mapping;
        }
        template <class TPointCloudType>
        void request_properties()  {
            if constexpr(HasPointProperty<TPointCloudType>::value){
                try { vertices = this->request_element("vertex", {"x","y","z"}); }
                catch (const std::exception & e) { spdlog::get("console")->critical("tinyply exception: {}",e.what()); }
            }
            if constexpr(HasNormalProperty<TPointCloudType>::value){
                try { normals = this->request_element("vertex", {"nx","ny","nz"}); }
                catch (const std::exception & e) { spdlog::get("console")->critical("tinyply exception: {}",e.what()); }
            }
            if constexpr(HasColorProperty<TPointCloudType>::value){
                try { colors = this->request_element("vertex", {"red","green","blue"}); }
                catch (const std::exception & e) { spdlog::get("console")->critical("tinyply exception: {}",e.what()); }
            }
            if constexpr(HasScalarProperty<TPointCloudType>::value){
                try { scalarfieds = this->request_element("vertex", {"scalar_1"}); }
                catch (const std::exception & e) { spdlog::get("console")->critical("tinyply exception: {}",e.what()); }
            }
            if constexpr(HasScalarProperty<TPointCloudType>::value){
                try { classes = this->request_element("vertex", {"class"}); }
                catch (const std::exception & e) { spdlog::get("console")->critical("tinyply exception: {}",e.what()); }
            }
        }

        template <class TPointCloudType>
        TPointCloudType parse_pointcloud()  {
            using TPoint = typename TPointCloudType::Scalar;
            auto points = cilantro::vectorSetFromPLYDataBuffer<TPoint, TPointCloudType::Param::Dimension>(vertices, TPointCloudType::Param::Dimension);
            TPointCloudType _tmp(points);
            if constexpr(HasNormalProperty<TPointCloudType>::value){
               _tmp.normals = cilantro::vectorSetFromPLYDataBuffer<TPoint, TPointCloudType::Param::Dimension>(normals, TPointCloudType::Param::Dimension);
            }
            //TODO Make colours work with colored point clouds
            if constexpr(HasColorProperty<TPointCloudType>::value){
                _tmp.colors = cilantro::vectorSetFromPLYDataBuffer<TPoint, TPointCloudType::Param::Dimension>(colors, TPointCloudType::Param::Dimension);
            }
            //TODO Make colours work with colored point clouds, How to get the type ?
            if constexpr(HasScalarProperty<TPointCloudType>::value){
                spdlog::get("console")->warn("Scalar field have not been implemented yet!");
            }
            return _tmp;
        }
        template <class TPointCloudType>
        bool write(const Path& output_path, const TPointCloudType& cloud, FileOutputSettings file_output_settings){
            plyfile = std::make_unique<PlyFile>();
            if(file_output_settings.pcr.points_3D){
                propose_element("vertex", { "x", "y", "z" }, cloud.points);
            }
            if(file_output_settings.pcr.has_normals){
                if constexpr(HasNormalProperty<TPointCloudType>::value){
                    propose_element("vertex", { "nx", "ny", "nz" }, cloud.normals);
                }
                else{
                    Eigen::MatrixXf _empty = Eigen::MatrixXf::Zero(3, cloud.size());
                    propose_element("vertex", { "nx", "ny", "nz" }, _empty);
                }
            }
            if(file_output_settings.pcr.has_colors){
                if constexpr(HasColorProperty<TPointCloudType>::value){
                    propose_element("vertex", { "red", "green", "blue" }, cloud.colors);
                }
                else{
                    Eigen::MatrixXu8 _empty = Eigen::MatrixXu8::Zero(3, cloud.size());
                    propose_element("vertex", { "red", "green", "blue" }, _empty);
                }
            }
            if(file_output_settings.pcr.has_intensity){
                if constexpr(HasIntensityProperty<TPointCloudType>::value){
                    propose_element("vertex", {"intensity"}, cloud.intensity);
                }
                else{
                    Eigen::MatrixXf _empty = Eigen::MatrixXf::Zero(1, cloud.size());
                    propose_element("vertex", {"intensity"}, _empty);
                }
            }
            if(file_output_settings.pcr.has_classes){
                if constexpr(HasClassProperty<TPointCloudType>::value){
                    propose_element("vertex", {"class"}, cloud.type);
                }
                else{
                    Eigen::MatrixXu32 _empty = Eigen::MatrixXu32::Zero(1, cloud.size());
                    propose_element("vertex", {"class"}, _empty);
                }
            }
            if(file_output_settings.pcr.has_cost){
                if constexpr(HasCostProperty<TPointCloudType>::value){
                    propose_element("vertex", {"cost"}, cloud.cost);
                }
                else{
                    Eigen::MatrixXf _empty = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>::Zero(1, cloud.size());
                    propose_element("vertex", {"cost"}, _empty);
                }
            }
            // TODO ADD Scalar Int & Scalar Float

            // Actually a copy would maybe be better ?
            if(meta.dialect == PlyDialect::tum_ply){
                plyfile->get_comments().emplace_back("dialect=tum_ply");
            }
            for(const auto& comment : meta.comments){
                plyfile->get_comments().emplace_back(comment);
            }

            std::filebuf outbuffer;
            auto mode = file_output_settings.binary ?  std::ios::out |std::ios::binary :std::ios::out;
            outbuffer.open(output_path, mode);
            std::ostream outstream(&outbuffer);
            if (outstream.fail()) throw std::runtime_error("failed to open " + output_path.string());
            plyfile->write(outstream, file_output_settings.binary);
        }
};

//template<class TPointCloudType>

#endif //CMS_PCS_OPTIMIZER_PLY_ADAPTOR_H
