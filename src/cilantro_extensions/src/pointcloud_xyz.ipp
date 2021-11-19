#pragma once

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PointCloudCartesian<ScalarT, EigenDim>::set_right_cols(Eigen::Index cols, const PointCloudCartesian<ScalarT, EigenDim> &cloud) {
    points.rightCols(cols) = cloud.points;
}

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PointCloudCartesian<ScalarT, EigenDim>::swap_point(Eigen::Index dst, Eigen::Index src) {
    points.col(dst).swap(points.col(src));
}

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PointCloudCartesian<ScalarT, EigenDim>::resize(Eigen::Index cols) {
    points.conservativeResize(Eigen::NoChange, cols);
}

template<typename ScalarT, ptrdiff_t EigenDim>
PointCloudCartesian<ScalarT, EigenDim>::PointCloudCartesian(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points) : points(_points) {}
