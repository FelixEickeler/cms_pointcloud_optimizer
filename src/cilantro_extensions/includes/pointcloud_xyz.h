#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

template <typename ScalarT, ptrdiff_t EigenDim>
struct PointCloudCartesian : PointCloudBase<PointCloudCartesian<ScalarT, EigenDim>>{

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef ScalarT Scalar;
    enum Param{ Dimension = EigenDim };
    VectorSet<ScalarT,EigenDim> points;

    PointCloudCartesian() = default;
    PointCloudCartesian(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
    void resize(Eigen::Index cols);
    void set_right_cols(Eigen::Index cols, const PointCloudCartesian<ScalarT,EigenDim>& cloud);
    void swap_point(Eigen::Index dst, Eigen::Index src );
};
#include "../src/pointcloud_xyz.ipp"

template <typename TType>
using PointCloudXY = PointCloudCartesian<TType, 2> ;

template <typename TType>
using PointCloudXYZ = PointCloudCartesian<TType, 3> ;
}




//    typedef PointCloudXYZ<float,2> PointCloudXYZ2f;
//    typedef PointCloudXYZ<double,2> PointCloudXYZ2d;
//    typedef PointCloudXYZ<float,3> PointCloudXYZ3f;
//    typedef PointCloudXYZ<double,3> PointCloudXYZ3d;
//    typedef PointCloudXYZ<float,Eigen::Dynamic> PointCloudXYZXf;
//    typedef PointCloudXYZ<double,Eigen::Dynamic> PointCloudXYZXd;

