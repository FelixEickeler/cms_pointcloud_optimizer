//
// Created by felix on 18.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_PLY_ADAPTOR_H
#define CMS_PCS_OPTIMIZER_PLY_ADAPTOR_H
#include <utility>

#include "../pointclouds.h"
#include "cilantro/utilities/ply_io.hpp"
using namespace tinyply;

using PlyMapping = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

class PlyAdaptor {
    protected:
        PlyMapping _m;
        PlyFile& plyfile;
        std::shared_ptr<PlyData> request_element(const std::string& element_name, std::initializer_list<std::string> properties);

    public:
        PlyAdaptor(PlyFile& _plyfile, PlyMapping  mapping);;
        virtual std::unique_ptr<IsPointCloud> request_pointcloud() = 0;
        virtual void request_properties() = 0;
        virtual void parse_pointcloud() = 0;
};


// TODO Make this as template specialization
template <class TPointCloudCartesian>
class PointCloudCartesianAdaptor : public PlyAdaptor{
    private:
        std::shared_ptr<PlyData> vertices;
        TPointCloudCartesian pointcloud;

    public:
        using PointCloudType = TPointCloudCartesian;

        void request_properties() override {
            try { vertices = request_element("vertex", {"x","y","z"}); }
            catch (const std::exception & e) { spdlog::get("console")->critical("tinyply exception: {}",e.what()); }
        }

        void parse_pointcloud() override {
            using TPoint = typename TPointCloudCartesian::Scalar;
            auto points = cilantro::vectorSetFromPLYDataBuffer<TPoint, TPointCloudCartesian::Param::Dimension>(vertices, TPointCloudCartesian::Param::Dimension);
            TPointCloudCartesian _tmp(points);
            pointcloud.append(_tmp);
            spdlog::get("console")->info("Size now {}", pointcloud.size());
        }

        std::unique_ptr<IsPointCloud> request_pointcloud() override{
            return std::make_unique<TPointCloudCartesian>(pointcloud);
        }

        PointCloudCartesianAdaptor(PlyFile& file, PlyMapping& map) : PlyAdaptor(file, map) {}
};


#endif //CMS_PCS_OPTIMIZER_PLY_ADAPTOR_H
