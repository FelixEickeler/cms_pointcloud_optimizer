//
// Created by felix on 17.11.2021.
//

#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "../src/cilantro_extensions/includes/pointcloud_xyz.h"
#include "../src/cilantro_extensions/pointclouds.h"

std::vector<Eigen::Vector3f> cube_testpoints(){
        std::vector<Eigen::Vector3f> points;
        points.emplace_back(0, 0, 0);
        points.emplace_back(1, 0, 0);
        points.emplace_back(0, 1, 0);
        points.emplace_back(0, 0, 1);
        points.emplace_back(0, 1, 1);
        points.emplace_back(1, 0, 1);
        points.emplace_back(1, 1, 0);
        points.emplace_back(1, 1, 1);
        return points;
};

TEST (Datastructure_PointCloudCartesian , Constructor_Vector3f_PointCloudExists) {
    auto tps = cube_testpoints();
    cilantro::PointCloudCartesian<float, 3> xyz(tps);
    for(auto i = 0; i < xyz.points.cols(); ++i){
        EXPECT_EQ(xyz.points.col(i)(0), tps[i](0));
        EXPECT_EQ(xyz.points.col(i)(1), tps[i](1));
        EXPECT_EQ(xyz.points.col(i)(2), tps[i](2));
    }
}

TEST (Datastructure_PointCloudCartesian , Resize_Custom_AllProperitesResized) {
    auto tps = cube_testpoints();
    cilantro::PointCloudCartesian<float, 3> xyz(tps);
    tps.resize(tps.size()-4);
    xyz.resize(xyz.size()-4);
    ASSERT_EQ(xyz.size(), tps.size());
}

TEST (Datastructure_PointCloudCartesian , APPEND_CubePoints_DoubleTrouble) {
    auto tps = cube_testpoints();
    cilantro::PointCloudCartesian<float, 3> xyz(tps);
    cilantro::PointCloudCartesian<float, 3> to_be_appended(tps);
    int begin2 = xyz.size();
    xyz.append(to_be_appended);

    ASSERT_EQ(xyz.size(), tps.size()*2);
    for(auto i = 0; i < begin2; ++i){
        EXPECT_EQ(xyz.points.col(i)(0), tps[i](0));
        EXPECT_EQ(xyz.points.col(i)(1), tps[i](1));
        EXPECT_EQ(xyz.points.col(i)(2), tps[i](2));
        EXPECT_EQ(xyz.points.col(begin2+i)(0), tps[i](0));
        EXPECT_EQ(xyz.points.col(begin2+i)(1), tps[i](1));
        EXPECT_EQ(xyz.points.col(begin2+i)(2), tps[i](2));
    }
}

TEST (Datastructure_PointCloudCartesian , Clear_EmptyPointCloud) {
    auto tps = cube_testpoints();
    cilantro::PointCloudCartesian<float, 3> xyz(tps);
    xyz.clear();
    ASSERT_EQ(xyz.size(), 0);
}

TEST (Datastructure_PointCloudCartesian , Remove_4Indices_Remaining) {
    auto tps = cube_testpoints();
    cilantro::PointCloudCartesian<float, 3> xyz(tps);
    std::vector<int> indices = {0,2,5,7};
    std::vector<Eigen::Vector3f> points;
    for(auto i=0; i < tps.size(); ++i){
        if(std::any_of(indices.cbegin(), indices.cend(), [i](auto idx){return idx == i;})){
            continue;
        }
        points.push_back(tps[i]);
    }

    xyz.remove(indices);
    ASSERT_EQ(xyz.size(), points.size());
    for(auto i = 0; i < xyz.points.cols(); ++i) {
        Eigen::Vector3f current_point = xyz.points.col(i);
        if(std::none_of(points.cbegin(), points.cend(), [current_point](const Eigen::Vector3f& point){return current_point.isApprox(point);})) {
            ASSERT_FALSE(true) << "Remove failed due, check !";
        }
    }
}

TEST (Datastructure_PointCloudXYZC , Constructor_Vector3f_PointCloudExists) {
    auto tps = cube_testpoints();
    std::vector<int> classes = {1,2,3,4,5,6,7,8};
    cilantro::PointCloudXYZC<float> xyz(tps, classes);
    for(auto i = 0; i < xyz.points.cols(); ++i){
        EXPECT_EQ(xyz.points.col(i)(0), tps[i](0));
        EXPECT_EQ(xyz.points.col(i)(1), tps[i](1));
        EXPECT_EQ(xyz.points.col(i)(2), tps[i](2));
    }
}

TEST (Datastructure_PointCloudXYZScalar, Constructor_Vector3f_PointCloudExists) {
    auto tps = cube_testpoints();
    std::vector<float> intensities = {1,2,3,4,5,6,7,8};
    cilantro::PointCloudXYZI<float> xyz(tps, intensities);
    for(auto i = 0; i < xyz.points.cols(); ++i){
        EXPECT_EQ(xyz.points.col(i)(0), tps[i](0));
        EXPECT_EQ(xyz.points.col(i)(1), tps[i](1));
        EXPECT_EQ(xyz.points.col(i)(2), tps[i](2));
    }
}