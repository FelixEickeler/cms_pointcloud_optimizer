#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

    template <typename ScalarT, ptrdiff_t EigenDim, typename TColorType, ptrdiff_t ColorDim>
    struct PointCloudCartesianNormalsColorsIntensity: PointCloudBase<PointCloudCartesianNormalsColorsIntensity<ScalarT, EigenDim,TColorType, ColorDim>>{

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef ScalarT Scalar;
        typedef TColorType ColorType;
        enum Param{ Dimension = EigenDim, ColorDimensions = ColorDim };
        VectorSet<ScalarT,EigenDim> points;
        VectorSet<ScalarT,EigenDim> normals;
        VectorSet<TColorType, ColorDim> colors;
        VectorSet<float , 1> intensity;

        PointCloudCartesianNormalsColorsIntensity() = default;
        PointCloudCartesianNormalsColorsIntensity(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
        PointCloudCartesianNormalsColorsIntensity(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
                                              const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
                                              const ConstVectorSetMatrixMap<TColorType, ColorDim> &_colors,
                                              const ConstVectorSetMatrixMap<float, 1> &_intensity);
        void resize(Eigen::Index cols);
        void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormalsColorsIntensity<ScalarT, EigenDim, TColorType, ColorDim>& cloud);
        void swap_point(Eigen::Index dst, Eigen::Index src );
    };


#include "../src/pointcloud_xyz_normals_colors_intensity.ipp"

//// short handles
    template <typename TCoordinateType>
    using PointCloudXYNRGBI = PointCloudCartesianNormalsColorsIntensity<TCoordinateType, 2, uint8_t, 3>;
    template <typename TCoordinateType>
    using PointCloudXYZNRGBI = PointCloudCartesianNormalsColorsIntensity<TCoordinateType, 3, uint8_t, 3>;
    template <typename TCoordinateType>
    using PointCloudXYNRGBAI = PointCloudCartesianNormalsColorsIntensity<TCoordinateType, 2, uint8_t, 4>;
    template <typename TCoordinateType>
    using PointCloudXYZNRGBAI = PointCloudCartesianNormalsColorsIntensity<TCoordinateType, 3, uint8_t, 4>;

}