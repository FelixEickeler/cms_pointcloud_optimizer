#pragma once
#define PCS_SCALAR PointCloudCartesianScalar<ScalarT, EigenDim, TScalarFieldType, TScalarFieldDim>

template<typename ScalarT, ptrdiff_t EigenDim, typename TScalarFieldType, ptrdiff_t TScalarFieldDim>
inline void PCS_SCALAR::resize(Eigen::Index cols) {
    points.conservativeResize(Eigen::NoChange, cols);
    scalars.conservativeResize(Eigen::NoChange, cols);
}

template<typename ScalarT, ptrdiff_t EigenDim, typename TScalarFieldType, ptrdiff_t TScalarFieldDim>
inline void PCS_SCALAR::set_right_cols(Eigen::Index cols,  const PCS_SCALAR &cloud) {
    points.rightCols(cols) = cloud.points;
    scalars.rightCols(cols) = cloud.scalars;
}

template<typename ScalarT, ptrdiff_t EigenDim, typename TScalarFieldType, ptrdiff_t TScalarFieldDim>
inline void PCS_SCALAR::swap_point(Eigen::Index dst, Eigen::Index src) {
    points.col(dst).swap(points.col(src));
    scalars.col(dst).swap(scalars.col(src));
}

template<typename ScalarT, ptrdiff_t EigenDim, typename TScalarFieldType, ptrdiff_t TScalarFieldDim>
PCS_SCALAR::PointCloudCartesianScalar(
        const ConstVectorSetMatrixMap<ScalarT, EigenDim> &_points,
        const ConstVectorSetMatrixMap<TScalarFieldType, TScalarFieldDim> &_scalar) : points(_points), scalars(_scalar) {}

#undef PCS_SCALAR