//
// Created by felix on 16.11.2021.
//

#include "pointcloud_parser.h"

int cms_opti::load_pcs(const Path &input_path) {
    cilantro::PointCloud3f cloud;

    // Read ply file
    spdlog::get("console")->info("Reading file from {}", input_path.string());
    cloud.fromPLYFile(input_path);
    std::cout << cloud.points.cols() << " points read" << std::endl;
    std::cout << cloud.normals.cols() << " normals read" << std::endl;
    std::cout << cloud.colors.cols() << " colors read" << std::endl;
//    std::cout << cloud.intensity.cols() << " colors read" << std::endl;

    // Modify pointcloud
    std::vector<size_t> ind;
    for (size_t i = 0; i < cloud.points.cols(); i += 50) {
        ind.push_back(i);
    }
    cilantro::PointCloud3f pc(cloud, ind);

//        // Write ply file
//        std::cout << "Writing pointcloud to " << fileOutPath << std::endl;
//        if (!pangolin::FileExists(pangolin::PathParent(fileOutPath))) {
//            std::cout << "Parent directory of output file " << fileOutPath << " doesn't exist." << std::endl;
//            return -1;
//        }
//
//        pc.toPLYFile(fileOutPath);
//        std::cout << pc.points.cols() << " points written" << std::endl;
//        std::cout << pc.normals.cols() << " normals written" << std::endl;
    std::cout << pc.colors.cols() << " colors written" << std::endl;

    return 0;
}
