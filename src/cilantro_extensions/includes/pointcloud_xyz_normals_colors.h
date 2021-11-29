#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

template <typename ScalarT, ptrdiff_t EigenDim, typename TColorType, ptrdiff_t ColorDim>
struct PointCloudCartesianNormalsColors: PointCloudBase<PointCloudCartesianNormalsColors<ScalarT, EigenDim,TColorType, ColorDim>>{

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef ScalarT Scalar;
    typedef TColorType ColorType;
    enum Param{ Dimension = EigenDim, ColorDimensions = ColorDim };
    VectorSet<ScalarT,EigenDim> points;
    VectorSet<ScalarT,EigenDim> normals;
    VectorSet<TColorType, ColorDim> colors;

    PointCloudCartesianNormalsColors() = default;
    PointCloudCartesianNormalsColors(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
    PointCloudCartesianNormalsColors(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
                                       const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
                                       const ConstVectorSetMatrixMap<TColorType, ColorDim> &_colors);
    void resize(Eigen::Index cols);
    void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormalsColors<ScalarT, EigenDim, TColorType, ColorDim>& cloud);
    void swap_point(Eigen::Index dst, Eigen::Index src );
};


#include "../src/pointcloud_xyz_normals_colors.ipp"

//// short handles
template <typename TCoordinateType>
using PointCloudXYNRGB = PointCloudCartesianNormalsColors<TCoordinateType, 2, uint8_t, 3>;
template <typename TCoordinateType>
using PointCloudXYZNRGB = PointCloudCartesianNormalsColors<TCoordinateType, 3, uint8_t, 3>;
template <typename TCoordinateType>
using PointCloudXYNRGBA = PointCloudCartesianNormalsColors<TCoordinateType, 2, uint8_t, 4>;
template <typename TCoordinateType>
using PointCloudXYZNRGBA = PointCloudCartesianNormalsColors<TCoordinateType, 3, uint8_t, 4>;

}