//
// Created by felix on 16.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_POINTCLOUD_PARSER_H
#define CMS_PCS_OPTIMIZER_POINTCLOUD_PARSER_H
#include "filesystem"
#include "spdlog/spdlog.h"
#include <cilantro/utilities/point_cloud.hpp>
#include "iostream"

using Path = std::filesystem::path;

namespace cms_opti {
    class PointCloudXYZI{

    };


    int load_pcs(const Path& input_path);



};


#endif //CMS_PCS_OPTIMIZER_POINTCLOUD_PARSER_H
