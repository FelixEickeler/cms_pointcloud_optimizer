#pragma once
#define PCS_TYPE PointCloudCartesianNormalsClass<ScalarT, EigenDim>
#define PCS_TYPE_TEMPLATE template<typename ScalarT, ptrdiff_t EigenDim>

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::resize(Eigen::Index cols) {
    points.conservativeResize(Eigen::NoChange, cols);
    normals.conservativeResize(Eigen::NoChange, cols);
    object_type.conservativeResize(Eigen::NoChange, cols);
}

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::set_right_cols(Eigen::Index cols,  const PCS_TYPE &cloud) {
    points.rightCols(cols) = cloud.points;
    normals.rightCols(cols) = cloud.normals;
    object_type.rightCols(cols) = cloud.object_type;
}

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::swap_point(Eigen::Index dst, Eigen::Index src) {
    points.col(dst).swap(points.col(src));
    normals.col(dst).swap(normals.col(src));
    object_type.col(dst).swap(object_type.col(src));
}

PCS_TYPE_TEMPLATE
PCS_TYPE::PointCloudCartesianNormalsClass(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points) : points(_points) {
    normals.resize(Eigen::NoChange, _points.cols());
    object_type.resize(Eigen::NoChange, _points.cols());
}

PCS_TYPE_TEMPLATE
PCS_TYPE::PointCloudCartesianNormalsClass(
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
        const ConstVectorSetMatrixMap<uint32_t, 1> &_object_type
        ) : points(_points), normals(_normals), object_type(_object_type){}

#undef PCS_TYPE
#undef PCS_TYPE_TEMPLATE