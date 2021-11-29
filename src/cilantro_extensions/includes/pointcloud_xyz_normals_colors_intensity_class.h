#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

    template <typename ScalarT, ptrdiff_t EigenDim, typename TColorType, ptrdiff_t ColorDim>
    struct PointCloudCartesianNormalsColorsIntensityClass: PointCloudBase<PointCloudCartesianNormalsColorsIntensityClass<ScalarT, EigenDim,TColorType, ColorDim>>{

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef ScalarT Scalar;
        typedef TColorType ColorType;
        enum Param{ Dimension = EigenDim, ColorDimensions = ColorDim };
        VectorSet<ScalarT,EigenDim> points;
        VectorSet<ScalarT,EigenDim> normals;
        VectorSet<TColorType, ColorDim> colors;
        VectorSet<float , 1> intensity;
        VectorSet<uint32_t , 1> object_type;

        PointCloudCartesianNormalsColorsIntensityClass() = default;
        PointCloudCartesianNormalsColorsIntensityClass(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points);
        PointCloudCartesianNormalsColorsIntensityClass(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
                                              const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
                                              const ConstVectorSetMatrixMap<TColorType, ColorDim> &_colors,
                                              const ConstVectorSetMatrixMap<float, 1> &_intensity,
                                              const ConstVectorSetMatrixMap<uint32_t, 1> &_object_type);
        void resize(Eigen::Index cols);
        void set_right_cols(Eigen::Index cols, const PointCloudCartesianNormalsColorsIntensityClass<ScalarT, EigenDim, TColorType, ColorDim>& cloud);
        void swap_point(Eigen::Index dst, Eigen::Index src );
    };


#include "../src/pointcloud_xyz_normals_colors_intensity_class.ipp"

//// short handles
    template <typename TCoordinateType>
    using PointCloudXYNRGBIT = PointCloudCartesianNormalsColorsIntensityClass<TCoordinateType, 2, uint8_t, 3>;
    template <typename TCoordinateType>
    using PointCloudXYZNRGBIT = PointCloudCartesianNormalsColorsIntensityClass<TCoordinateType, 3, uint8_t, 3>;
    template <typename TCoordinateType>
    using PointCloudXYNRGBAIT = PointCloudCartesianNormalsColorsIntensityClass<TCoordinateType, 2, uint8_t, 4>;
    template <typename TCoordinateType>
    using PointCloudXYZNRGBAIT = PointCloudCartesianNormalsColorsIntensityClass<TCoordinateType, 3, uint8_t, 4>;

}