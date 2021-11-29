#pragma once
#define PCS_TYPE PointCloudCartesianNormalIntensity<ScalarT, EigenDim>

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PCS_TYPE::resize(Eigen::Index cols) {
    points.conservativeResize(Eigen::NoChange, cols);
    normals.conservativeResize(Eigen::NoChange, cols);
    intensity.conservativeResize(Eigen::NoChange, cols);
}

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PCS_TYPE::set_right_cols(Eigen::Index cols,  const PCS_TYPE &cloud) {
    points.rightCols(cols) = cloud.points;
    normals.rightCols(cols) = cloud.normals;
    intensity.rightCols(cols) = cloud.intensity;
}

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PCS_TYPE::swap_point(Eigen::Index dst, Eigen::Index src) {
    points.col(dst).swap(points.col(src));
    normals.col(dst).swap(normals.col(src));
    intensity.col(dst).swap(intensity.col(src));
}

template<typename ScalarT, ptrdiff_t EigenDim>
PCS_TYPE::PointCloudCartesianNormalIntensity(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points) : points(_points) {
    normals.resize(Eigen::NoChange, _points.cols());
    intensity.resize(Eigen::NoChange, _points.cols());
}

template<typename ScalarT, ptrdiff_t EigenDim>
PCS_TYPE::PointCloudCartesianNormalIntensity(
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
        const ConstVectorSetMatrixMap<float, 1> &_intensity
        ) : points(_points), normals(_normals), intensity(_intensity){}

#undef PCS_TYPE