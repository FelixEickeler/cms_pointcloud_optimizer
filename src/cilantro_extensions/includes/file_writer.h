//
// Created by felix on 23.11.2021.
//
// mapping is part of adaptor ,  keep_orignal_names=false PlyFile file;
#ifndef CMS_PCS_OPTIMIZER_FILE_WRITER_H
#define CMS_PCS_OPTIMIZER_FILE_WRITER_H
#include <filesystem>
#include <cilantro/utilities/ply_io.hpp>
#include "datastructures.h"
#include "helpers.h"
#include "ply_adaptor.h"
#include "file_parser.h"
#include "io_name_mapping.h"

class FileWriter {
    private:
        Path output_path;
        Metadata meta;
        std::unique_ptr<IAdaptor> adaptor;
        FileOutputSettings file_output_settings{};

    public:
        FileWriter(const Path& path, const Metadata& _meta, std::string file_type="") : output_path(path), meta(_meta){
            auto suffix = empty(file_type) ? to_lower(path.extension().string()) : file_type;
            if(contains(FileParser::ending2adaptor, suffix)){
                adaptor = std::unique_ptr<IAdaptor>(FileParser::ending2adaptor[suffix]());
            }
            else{
                spdlog::critical("This file type ({}) is not supported! Terminating !",suffix);
            }
            adaptor->set_property_mapping(IONameMapping::_standard_mapping);
        }

        template <class TPointCloudType>
        void write(const TPointCloudType& cloud, const Eigen::RowVectorXf& cost=Eigen::RowVectorXf::Zero(),  const PlyMapping& mapping = {}) {
            spdlog::stopwatch sw;
            // TODO fix inheritance
            if(auto ply_adaptor = dynamic_cast<PlyAdaptor*>(adaptor.get())){
                ply_adaptor->write(output_path, cloud, cost, file_output_settings);
            }
        }

        void output_settings(FileOutputSettings fos){
            file_output_settings = fos;
        }

        inline const PlyMapping& get_file_mapping() const{
            return adaptor->get_property_mapping();
        }

        inline const void set_file_mapping(const PlyMapping& mapping) const{
            return adaptor->set_property_mapping(mapping);
        }

};
#endif //CMS_PCS_OPTIMIZER_FILE_WRITER_H
