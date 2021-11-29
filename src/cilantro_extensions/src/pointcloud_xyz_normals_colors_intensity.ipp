#pragma once
#define PCS_TYPE PointCloudCartesianNormalsColorsIntensity<ScalarT, EigenDim, TColorType, ColorDim>
#define PCS_TYPE_TEMPLATE template<typename ScalarT, ptrdiff_t EigenDim, typename TColorType, ptrdiff_t ColorDim>

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::resize(Eigen::Index cols) {
    points.conservativeResize(Eigen::NoChange, cols);
    normals.conservativeResize(Eigen::NoChange, cols);
    colors.conservativeResize(Eigen::NoChange, cols);
    intensity.conservativeResize(Eigen::NoChange, cols);
}

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::set_right_cols(Eigen::Index cols,  const PCS_TYPE &cloud) {
    points.rightCols(cols) = cloud.points;
    normals.rightCols(cols) = cloud.normals;
    colors.rightCols(cols) = cloud.colors;
    intensity.rightCols(cols) = cloud.intensity;
}

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::swap_point(Eigen::Index dst, Eigen::Index src) {
    points.col(dst).swap(points.col(src));
    normals.col(dst).swap(normals.col(src));
    colors.col(dst).swap(colors.col(src));
    intensity.col(dst).swap(intensity.col(src));
}

PCS_TYPE_TEMPLATE
PCS_TYPE::PointCloudCartesianNormalsColorsIntensity(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points) : points(_points) {
    normals.resize(Eigen::NoChange, _points.cols());
    colors.resize(Eigen::NoChange, _points.cols());
    intensity.resize(Eigen::NoChange, intensity.cols());
}

PCS_TYPE_TEMPLATE
PCS_TYPE::PointCloudCartesianNormalsColorsIntensity(
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
        const ConstVectorSetMatrixMap<TColorType, ColorDim> &_colors,
        const ConstVectorSetMatrixMap<float, 1> &_intensity
        ) : points(_points), normals(_normals), colors(_colors),intensity(_intensity){}

#undef PCS_TYPE
#undef PCS_TYPE_TEMPLATE