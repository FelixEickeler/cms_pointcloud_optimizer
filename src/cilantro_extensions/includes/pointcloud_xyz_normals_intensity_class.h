#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

    template <typename ScalarT, ptrdiff_t EigenDim>
    struct PointCloudCartesianNormalsIntensityClass: PointCloudBase<PointCloudCartesianNormalsIntensityClass<ScalarT, EigenDim>>{

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef ScalarT Scalar;
        enum Param{ Dimension = EigenDim};
        VectorSet<ScalarT,EigenDim> points;
        VectorSet<ScalarT,EigenDim> normals;
        VectorSet<float , 1> intensity;
        VectorSet<uint32_t , 1> object_type;

        PointCloudCartesianNormalsIntensityClass() = default;
        PointCloudCartesianNormalsIntensityClass(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
        PointCloudCartesianNormalsIntensityClass(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
                                                       const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
                                                       const ConstVectorSetMatrixMap<float, 1> &_intensity,
                                                       const ConstVectorSetMatrixMap<uint32_t, 1> &_object_type);
        void resize(Eigen::Index cols);
        void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormalsIntensityClass<ScalarT, EigenDim>& cloud);
        void swap_point(Eigen::Index dst, Eigen::Index src );
    };

#include "../src/pointcloud_xyz_normals_intensity_class.ipp"

//// short handles
template <typename TCoordinateType>
using PointCloudXYNIT = PointCloudCartesianNormalsIntensityClass<TCoordinateType, 2>;
template <typename TCoordinateType>
using PointCloudXYZNIT = PointCloudCartesianNormalsIntensityClass<TCoordinateType, 3>;
template <typename TCoordinateType>
using PointCloudXYNRIT = PointCloudCartesianNormalsIntensityClass<TCoordinateType, 2>;
template <typename TCoordinateType>
using PointCloudXYZNIT = PointCloudCartesianNormalsIntensityClass<TCoordinateType, 3>;
}