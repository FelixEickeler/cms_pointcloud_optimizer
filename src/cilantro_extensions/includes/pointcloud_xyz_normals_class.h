#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

    template <typename ScalarT, ptrdiff_t EigenDim>
    struct PointCloudCartesianNormalsClass: PointCloudBase<PointCloudCartesianNormalsClass<ScalarT, EigenDim>>{

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef ScalarT Scalar;
        enum Param{ Dimension = EigenDim};
        VectorSet<ScalarT,EigenDim> points;
        VectorSet<ScalarT,EigenDim> normals;
        VectorSet<uint32_t , 1> object_type;

        PointCloudCartesianNormalsClass() = default;
        PointCloudCartesianNormalsClass(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
        PointCloudCartesianNormalsClass(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
                                         const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
                                         const ConstVectorSetMatrixMap<uint32_t, 1> &_object_type);
        void resize(Eigen::Index cols);
        void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormalsClass<ScalarT, EigenDim>& cloud);
        void swap_point(Eigen::Index dst, Eigen::Index src );
    };


#include "../src/pointcloud_xyz_normals_class.ipp"

//// short handles
template <typename TCoordinateType>
using PointCloudXYNT = PointCloudCartesianNormalsClass<TCoordinateType, 2>;
template <typename TCoordinateType>
using PointCloudXYZNT = PointCloudCartesianNormalsClass<TCoordinateType, 3>;
template <typename TCoordinateType>
using PointCloudXYNT = PointCloudCartesianNormalsClass<TCoordinateType, 2>;
template <typename TCoordinateType>
using PointCloudXYZNT = PointCloudCartesianNormalsClass<TCoordinateType, 3>;

}