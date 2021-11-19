#pragma once
#include <cilantro/core/data_containers.hpp>
#include "../includes/pointcloud_base.h"

namespace cilantro {

template <typename ScalarT, ptrdiff_t EigenDim, typename TScalarFieldType=float, ptrdiff_t TScalarFieldDim=1>
struct PointCloudCartesianScalar: PointCloudBase<PointCloudCartesianScalar <ScalarT, EigenDim, TScalarFieldType, TScalarFieldDim>>, HasScalarField{

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef ScalarT Scalar;
    enum { Dimension = EigenDim };
    VectorSet<ScalarT,EigenDim> points;
    VectorSet<TScalarFieldType, TScalarFieldDim> scalars;

    PointCloudCartesianScalar() = default;
    PointCloudCartesianScalar(const ConstVectorSetMatrixMap<ScalarT,EigenDim> &_points, const ConstVectorSetMatrixMap<TScalarFieldType, TScalarFieldDim> &_scalar);
    void resize(Eigen::Index cols);
    void set_right_cols(Eigen::Index cols, const PointCloudCartesianScalar <ScalarT, EigenDim, TScalarFieldType, TScalarFieldDim>& cloud);
    void swap_point(Eigen::Index dst, Eigen::Index src );
};

#include "../src/pointcloud_xyz_scalar.ipp"

// short handles
template <typename TCoordinateType>
using PointCloudXYI = PointCloudCartesianScalar<TCoordinateType, 2, float, 1>;
template <typename TCoordinateType>
using PointCloudXYC = PointCloudCartesianScalar<TCoordinateType, 2, int, 1>;

template <typename TCoordinateType>
using PointCloudXYZI = PointCloudCartesianScalar<TCoordinateType, 3, float, 1>;
template <typename TCoordinateType>
using PointCloudXYZC = PointCloudCartesianScalar<TCoordinateType, 3, int, 1>;

}