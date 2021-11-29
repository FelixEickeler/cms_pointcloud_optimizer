#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

template <typename ScalarT, ptrdiff_t EigenDim>
struct PointCloudCartesianNormalIntensity: PointCloudBase<PointCloudCartesianNormalIntensity<ScalarT, EigenDim>>{

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef ScalarT Scalar;
    enum Param{ Dimension = EigenDim };
    VectorSet<ScalarT,EigenDim> points;
    VectorSet<ScalarT,EigenDim> normals;
    VectorSet<float, EigenDim> intensity;

    PointCloudCartesianNormalIntensity() = default;
    PointCloudCartesianNormalIntensity(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
    PointCloudCartesianNormalIntensity(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
                                       const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
                                       const ConstVectorSetMatrixMap<float, 1> &_intensity);
    void resize(Eigen::Index cols);
    void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormalIntensity<ScalarT, EigenDim>& cloud);
    void swap_point(Eigen::Index dst, Eigen::Index src );
};


#include "../src/pointcloud_xyz_normals_intensity.ipp"

//// short handles

template <typename TCoordinateType>
using PointCloudXYNI = PointCloudCartesianNormalIntensity<TCoordinateType, 2>;
template <typename TCoordinateType>
using PointCloudXYZNI = PointCloudCartesianNormalIntensity<TCoordinateType, 3>;

}