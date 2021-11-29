#pragma once
#define PCS_TYPE PointCloudCartesianNormalsIntensityClass<ScalarT, EigenDim>
#define PCS_TYPE_TEMPLATE template<typename ScalarT, ptrdiff_t EigenDim>

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::resize(Eigen::Index cols) {
    points.conservativeResize(Eigen::NoChange, cols);
    normals.conservativeResize(Eigen::NoChange, cols);
    intensity.conservativeResize(Eigen::NoChange, cols);
    object_type.conservativeResize(Eigen::NoChange, cols);
}

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::set_right_cols(Eigen::Index cols,  const PCS_TYPE &cloud) {
    points.rightCols(cols) = cloud.points;
    normals.rightCols(cols) = cloud.normals;
    intensity.rightCols(cols) = cloud.intensity;
    object_type.rightCols(cols) = cloud.object_type;
}

PCS_TYPE_TEMPLATE
inline void PCS_TYPE::swap_point(Eigen::Index dst, Eigen::Index src) {
    points.col(dst).swap(points.col(src));
    normals.col(dst).swap(normals.col(src));
    intensity.col(dst).swap(intensity.col(src));
    object_type.col(dst).swap(object_type.col(src));
}

PCS_TYPE_TEMPLATE
PCS_TYPE::PointCloudCartesianNormalsIntensityClass(const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points) : points(_points) {
    normals.resize(Eigen::NoChange, _points.cols());
    intensity.resize(Eigen::NoChange, intensity.cols());
    object_type.resize(Eigen::NoChange, _points.cols());
}

PCS_TYPE_TEMPLATE
PCS_TYPE::PointCloudCartesianNormalsIntensityClass(
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_normals,
        const ConstVectorSetMatrixMap<float, 1> &_intensity,
        const ConstVectorSetMatrixMap<uint32_t, 1> &_object_type
        ) : points(_points), normals(_normals), intensity(_intensity), object_type(_object_type){}

#undef PCS_TYPE
#undef PCS_TYPE_TEMPLATE