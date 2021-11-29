#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

    template <typename ScalarT, ptrdiff_t EigenDim, typename TColorType, ptrdiff_t ColorDim>
    struct PointCloudCartesianNormalsColorsClass: PointCloudBase<PointCloudCartesianNormalsColorsClass<ScalarT, EigenDim,TColorType, ColorDim>>{

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef ScalarT Scalar;
        typedef TColorType ColorType;
        enum Param{ Dimension = EigenDim, ColorDimensions = ColorDim };
        VectorSet<ScalarT,EigenDim> points;
        VectorSet<ScalarT,EigenDim> normals;
        VectorSet<TColorType, ColorDim> colors;
        VectorSet<uint32_t , 1> object_type;

        PointCloudCartesianNormalsColorsClass() = default;
        PointCloudCartesianNormalsColorsClass(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
        PointCloudCartesianNormalsColorsClass(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
                                         const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
                                         const ConstVectorSetMatrixMap<TColorType, ColorDim> &_colors,
                                         const ConstVectorSetMatrixMap<uint32_t, 1> &_object_type);
        void resize(Eigen::Index cols);
        void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormalsColorsClass<ScalarT, EigenDim, TColorType, ColorDim>& cloud);
        void swap_point(Eigen::Index dst, Eigen::Index src );
    };


#include "../src/pointcloud_xyz_normals_colors_class.ipp"

//// short handles
template <typename TCoordinateType>
using PointCloudXYNRGBT = PointCloudCartesianNormalsColorsClass<TCoordinateType, 2, uint8_t, 3>;
template <typename TCoordinateType>
using PointCloudXYZNRGBT = PointCloudCartesianNormalsColorsClass<TCoordinateType, 3, uint8_t, 3>;
template <typename TCoordinateType>
using PointCloudXYNRGBAT = PointCloudCartesianNormalsColorsClass<TCoordinateType, 2, uint8_t, 4>;
template <typename TCoordinateType>
using PointCloudXYZNRGBAT = PointCloudCartesianNormalsColorsClass<TCoordinateType, 3, uint8_t, 4>;

}