#pragma once
#define PCS_TYPE PointCloudCartesianNormals<ScalarT, EigenDim>

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PCS_TYPE::resize(Eigen::Index cols) {
    points.conservativeResize(Eigen::NoChange, cols);
    normals.conservativeResize(Eigen::NoChange, cols);
}

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PCS_TYPE::set_right_cols(Eigen::Index cols,  const PCS_TYPE &cloud) {
    points.rightCols(cols) = cloud.points;
    normals.rightCols(cols) = cloud.normals;
}

template<typename ScalarT, ptrdiff_t EigenDim>
inline void PCS_TYPE::swap_point(Eigen::Index dst, Eigen::Index src) {
    points.col(dst).swap(points.col(src));
    normals.col(dst).swap(normals.col(src));
}

template<typename ScalarT, ptrdiff_t EigenDim>
PCS_TYPE::PointCloudCartesianNormals(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points) : points(_points) {
    normals.resize(Eigen::NoChange, _points.cols());
}

template<typename ScalarT, ptrdiff_t EigenDim>
PCS_TYPE::PointCloudCartesianNormals(
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals) : points(_points), normals(_normals) {}

#undef PCS_TYPE