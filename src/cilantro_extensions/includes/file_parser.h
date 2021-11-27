//
// Created by felix on 17.11.2021.
//
#ifndef CMS_PCS_OPTIMIZER_FILE_PARSER_H
#define CMS_PCS_OPTIMIZER_FILE_PARSER_H
#include <filesystem>
#include <memory>
#include <cilantro/utilities/ply_io.hpp>
#include "spdlog/spdlog.h"
#include <unordered_map>

#include "datastructures.h"
#include "helpers.h"
#include "spdlog/stopwatch.h"
#include "ply_adaptor.h"

using namespace tinyply;

class  FileParser{
    private:
        Path input_path;
        Metadata meta;
        std::unique_ptr<IAdaptor> adaptor;

    public:
        static inline std::map<std::string, std::function<IAdaptor*()>> ending2adaptor = {
                {".ply", [](){return new PlyAdaptor();}},
        };

        FileParser(const Path& path) : input_path(path){
            auto suffix = to_lower(path.extension().string());
            if(contains(FileParser::ending2adaptor, suffix)){
                adaptor = std::unique_ptr<IAdaptor>(FileParser::ending2adaptor[suffix]());
            } else{
                spdlog::get("console")->critical("This file type ({}) is not supported! Terminating !", suffix);
            }
            adaptor->init(input_path);
        };
        PointCloudRequirements parse_header();

        template <class TPointCloudType>
        TPointCloudType parse_data() {
            TPointCloudType point_cloud;
            spdlog::stopwatch sw;

            // TODO fix inheritance
            if(auto ply_adaptor = dynamic_cast<PlyAdaptor*>(adaptor.get())){
                ply_adaptor->request_properties<TPointCloudType>();
                ply_adaptor->read();
                point_cloud= ply_adaptor->parse_pointcloud<TPointCloudType>();
            }
            spdlog::info("The file {} was read in {:.1}s, {} points have been parsed.", input_path.filename().string(), sw, point_cloud.size());
            return point_cloud;
        }
        Metadata get_metadata();

        bool supports_file();
};

#endif //CMS_PCS_OPTIMIZER_FILE_PARSER_H
