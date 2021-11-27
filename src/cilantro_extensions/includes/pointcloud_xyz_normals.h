#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

template <typename ScalarT, ptrdiff_t EigenDim>
struct PointCloudCartesianNormals: PointCloudBase<PointCloudCartesianNormals<ScalarT, EigenDim>>{

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef ScalarT Scalar;
    enum Param{ Dimension = EigenDim };
    VectorSet<ScalarT,EigenDim> points;
    VectorSet<ScalarT,EigenDim> normals;

    PointCloudCartesianNormals() = default;
    PointCloudCartesianNormals(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points, const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_normals);
    PointCloudCartesianNormals(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points);
    void resize(Eigen::Index cols);
    void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormals <ScalarT, EigenDim>& cloud);
    void swap_point(Eigen::Index dst, Eigen::Index src );
};


#include "../src/pointcloud_xyz_normals.ipp"

//// short handles

template <typename TCoordinateType>
using PointCloudXYN = PointCloudCartesianNormals<TCoordinateType, 2>;
template <typename TCoordinateType>
using PointCloudXYZN = PointCloudCartesianNormals<TCoordinateType, 3>;

}