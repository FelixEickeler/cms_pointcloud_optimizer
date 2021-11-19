////
//// Created by felix on 16.11.2021.
////
//
//#ifndef CMS_PCS_OPTIMIZER_POINT_CLOUD_UTILITIES_H
//#define CMS_PCS_OPTIMIZER_POINT_CLOUD_UTILITIES_H
//
//PointCloud& removeInvalidPoints() {
//    std::vector<size_t> ind_to_remove;
//    ind_to_remove.reserve(points.cols());
//    for (size_t i = 0; i < points.cols(); i++) {
//        if (!points.col(i).allFinite()) ind_to_remove.emplace_back(i);
//    }
//    return remove(ind_to_remove);
//}
//
//PointCloud& removeInvalidNormals() {
//    if (!hasNormals()) return *this;
//
//    std::vector<size_t> ind_to_remove;
//    ind_to_remove.reserve(normals.cols());
//    for (size_t i = 0; i < normals.cols(); i++) {
//        if (!normals.col(i).allFinite()) ind_to_remove.emplace_back(i);
//    }
//    return remove(ind_to_remove);
//}
//
//PointCloud& removeInvalidColors() {
//    if (!hasColors()) return *this;
//
//    std::vector<size_t> ind_to_remove;
//    ind_to_remove.reserve(colors.cols());
//    for (size_t i = 0; i < colors.cols(); i++) {
//        if (!colors.col(i).allFinite()) ind_to_remove.emplace_back(i);
//    }
//    return remove(ind_to_remove);
//}
//
//PointCloud& removeInvalidData() {
//    const bool has_normals = hasNormals();
//    const bool has_colors = hasColors();
//
//    std::vector<size_t> ind_to_remove;
//    ind_to_remove.reserve(points.cols());
//    for (size_t i = 0; i < points.cols(); i++) {
//        if (!points.col(i).allFinite() || (has_normals && !normals.col(i).allFinite()) || (has_colors && !colors.col(i).allFinite())) {
//            ind_to_remove.emplace_back(i);
//        }
//    }
//    return remove(ind_to_remove);
//}
//
//template <typename GridPointScalarT = ptrdiff_t>
//PointCloud& gridDownsample(ScalarT bin_size, size_t min_points_in_bin = 1, bool parallel = true) {
//    if (hasNormals() && hasColors()) {
//        PointsNormalsColorsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, normals, colors, bin_size, parallel).getDownsampledPointsNormalsColors(points, normals, colors, min_points_in_bin);
//    } else if (hasNormals()) {
//        PointsNormalsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, normals, bin_size, parallel).getDownsampledPointsNormals(points, normals, min_points_in_bin);
//    } else if (hasColors()) {
//        PointsColorsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, colors, bin_size, parallel).getDownsampledPointsColors(points, colors, min_points_in_bin);
//    } else {
//        PointsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, bin_size, parallel).getDownsampledPoints(points, min_points_in_bin);
//    }
//    return *this;
//}
//
//template <typename GridPointScalarT = ptrdiff_t>
//PointCloud gridDownsampled(ScalarT bin_size, size_t min_points_in_bin = 1, bool parallel = true) const {
//    PointCloud res;
//    if (hasNormals() && hasColors()) {
//        PointsNormalsColorsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, normals, colors, bin_size, parallel).getDownsampledPointsNormalsColors(res.points, res.normals, res.colors, min_points_in_bin);
//    } else if (hasNormals()) {
//        PointsNormalsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, normals, bin_size, parallel).getDownsampledPointsNormals(res.points, res.normals, min_points_in_bin);
//    } else if (hasColors()) {
//        PointsColorsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, colors, bin_size, parallel).getDownsampledPointsColors(res.points, res.colors, min_points_in_bin);
//    } else {
//        PointsGridDownsampler<ScalarT,EigenDim,GridPointScalarT>(points, bin_size, parallel).getDownsampledPoints(res.points, min_points_in_bin);
//    }
//    return res;
//}
//
//template <typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t, typename CountT = size_t>
//inline PointCloud& estimateNormalsKNN(CountT k, bool use_current_as_ref = false) {
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setReferenceNormals(normals).estimateNormalsKNN(normals, k);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormalsKNN(normals, k);
//    }
//    return *this;
//}
//
//template <typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t, typename CountT = size_t>
//inline PointCloud& estimateNormalsKNN(const KDTree<ScalarT,EigenDim,KDTreeDistanceAdaptors::L2,IndexT> &kd_tree,
//                                      CountT k, bool use_current_as_ref = false)
//{
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setReferenceNormals(normals).estimateNormalsKNN(normals, k);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormalsKNN(normals, k);
//    }
//    return *this;
//}
//
//template <typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t>
//inline PointCloud& estimateNormalsRadius(ScalarT radius, bool use_current_as_ref = false) {
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setReferenceNormals(normals).estimateNormalsRadius(normals, radius);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormalsRadius(normals, radius);
//    }
//    return *this;
//}
//
//template <typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t>
//inline PointCloud& estimateNormalsRadius(const KDTree<ScalarT,EigenDim,KDTreeDistanceAdaptors::L2,IndexT> &kd_tree,
//                                         ScalarT radius, bool use_current_as_ref = false)
//{
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setReferenceNormals(normals).estimateNormalsRadius(normals, radius);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormalsRadius(normals, radius);
//    }
//    return *this;
//}
//
//template <typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t, typename CountT = size_t>
//inline PointCloud& estimateNormalsKNNInRadius(CountT k, ScalarT radius, bool use_current_as_ref = false) {
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setReferenceNormals(normals).estimateNormalsKNNInRadius(normals, k, radius);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormalsKNNInRadius(normals, k, radius);
//    }
//    return *this;
//}
//
//template <typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t, typename CountT = size_t>
//inline PointCloud& estimateNormalsKNNInRadius(const KDTree<ScalarT,EigenDim,KDTreeDistanceAdaptors::L2,IndexT> &kd_tree,
//                                              CountT k, ScalarT radius, bool use_current_as_ref = false)
//{
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setReferenceNormals(normals).estimateNormalsKNNInRadius(normals, k, radius);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormalsKNNInRadius(normals, k, radius);
//    }
//    return *this;
//}
//
//template <typename NeighborhoodSpecT, typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t>
//inline PointCloud& estimateNormals(const NeighborhoodSpecT &nh, bool use_current_as_ref = false) {
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setReferenceNormals(normals).estimateNormals(normals, nh);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(points).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormals(normals, nh);
//    }
//    return *this;
//}
//
//template <typename NeighborhoodSpecT, typename CovarianceT = Covariance<ScalarT, EigenDim>, typename IndexT = size_t>
//inline PointCloud& estimateNormals(const KDTree<ScalarT,EigenDim,KDTreeDistanceAdaptors::L2,IndexT> &kd_tree,
//                                   const NeighborhoodSpecT &nh, bool use_current_as_ref = false)
//{
//    use_current_as_ref = use_current_as_ref && hasNormals();
//    normals.resize(points.rows(), points.cols());
//    if (use_current_as_ref) {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setReferenceNormals(normals).estimateNormals(normals, nh);
//    } else {
//        NormalEstimation<ScalarT,EigenDim,CovarianceT,IndexT>(kd_tree).setViewPoint(Vector<ScalarT,EigenDim>::Zero(points.rows(),1)).estimateNormals(normals, nh);
//    }
//    return *this;
//}
//
//template <class TransformT>
//inline PointCloud& transform(const TransformT &tform) {
//    if (hasNormals()) {
//        transformPointsNormals(tform, points, normals);
//    } else {
//        transformPoints(tform, points);
//    }
//    return *this;
//}
//
//template <class TransformT>
//inline PointCloud transformed(const TransformT &tform) const {
//    PointCloud cloud;
//    cloud.points.resize(points.rows(), points.cols());
//    if (hasNormals()) {
//        cloud.normals.resize(normals.rows(), normals.cols());
//        transformPointsNormals(tform, points, normals, cloud.points, cloud.normals);
//    } else {
//        transformPoints(tform, points, cloud.points);
//    }
//    if (hasColors()) cloud.colors = colors;
//    return cloud;
//}
//
//template <class DepthConverterT, class = typename std::enable_if<EigenDim == 3 && std::is_same<typename DepthConverterT::MetricDepth,ScalarT>::value>::type>
//inline PointCloud& fromDepthImage(const typename DepthConverterT::RawDepth* depth_data,
//                                  const DepthConverterT &depth_converter,
//                                  size_t image_w, size_t image_h,
//                                  const Eigen::Ref<const Eigen::Matrix<ScalarT,3,3>> &intrinsics,
//                                  bool keep_invalid = false,
//                                  bool compute_normals = false)
//{
//    colors.resize(Eigen::NoChange, 0);
//    if (compute_normals) {
//        depthImageToPointsNormals<DepthConverterT>(depth_data, depth_converter, image_w, image_h, intrinsics, points, normals, keep_invalid);
//    } else {
//        normals.resize(Eigen::NoChange, 0);
//        depthImageToPoints<DepthConverterT>(depth_data, depth_converter, image_w, image_h, intrinsics, points, keep_invalid);
//    }
//    return *this;
//}
//
//template <class DepthConverterT, class = typename std::enable_if<EigenDim == 3 && std::is_same<typename DepthConverterT::MetricDepth,ScalarT>::value>::type>
//inline PointCloud& fromRGBDImages(unsigned char* rgb_data,
//                                  const typename DepthConverterT::RawDepth* depth_data,
//                                  const DepthConverterT &depth_converter,
//                                  size_t image_w, size_t image_h,
//                                  const Eigen::Ref<const Eigen::Matrix<ScalarT,3,3>> &intrinsics,
//                                  bool keep_invalid = false,
//                                  bool compute_normals = false)
//{
//    if (compute_normals) {
//        RGBDImagesToPointsNormalsColors<DepthConverterT>(rgb_data, depth_data, depth_converter, image_w, image_h, intrinsics, points, normals, colors, keep_invalid);
//    } else {
//        normals.resize(Eigen::NoChange, 0);
//        RGBDImagesToPointsColors<DepthConverterT>(rgb_data, depth_data, depth_converter, image_w, image_h, intrinsics, points, colors, keep_invalid);
//    }
//    return *this;
//}
//
//template <ptrdiff_t Dim = EigenDim, class = typename std::enable_if<Dim == 3>::type>
//inline PointCloud& fromPLYFile(const std::string &file_name, bool preload = true) {
//    PLYReader reader(file_name, preload);
//
//    auto point_data = reader.requestData("vertex", {"x", "y", "z"});
//    auto normal_data = reader.requestData("vertex", {"nx", "ny", "nz"});
//    auto color_data = reader.requestData("vertex", {"red", "green", "blue"});
//
//    reader.readData();
//
//    points = vectorSetFromPLYDataBuffer<ScalarT,3>(point_data, 3);
//    normals = vectorSetFromPLYDataBuffer<ScalarT,3>(normal_data, 3);
//    colors = (1.0f/255.0f)*vectorSetFromPLYDataBuffer<float,3>(color_data, 3);
//
//    return *this;
//}
//
//template <typename ScalarOutT = ScalarT, ptrdiff_t Dim = EigenDim, class = typename std::enable_if<Dim == 3>::type>
//inline const PointCloud& toPLYFile(const std::string &file_name, bool binary = true) const {
//    PLYWriter writer(file_name, binary);
//
//    std::shared_ptr<tinyply::PlyData> point_data, normal_data, color_data;
//    VectorSet3f colors_tmp;
//
//    point_data = PLYDataBufferFromVectorSet<ScalarT,3,ScalarOutT>(points);
//    writer.addData("vertex", {"x", "y", "z"}, point_data);
//
//    if (hasNormals()) {
//        normal_data = PLYDataBufferFromVectorSet<ScalarT,3,ScalarOutT>(normals);
//        writer.addData("vertex", {"nx", "ny", "nz"}, normal_data);
//    }
//
//    if (hasColors()) {
//        colors_tmp = 255.0f*colors;
//        color_data = PLYDataBufferFromVectorSet<float,3,uint8_t>(colors_tmp);
//        writer.addData("vertex", {"red", "green", "blue"}, color_data);
//    }
//
//    writer.writeData();
//
//    return *this;
//}
//
//template <typename NewScalarT>
//PointCloud<NewScalarT,EigenDim> cast() const {
//    PointCloud<NewScalarT,EigenDim> res;
//    res.points = points.template cast<NewScalarT>();
//    res.normals = normals.template cast<NewScalarT>();
//    res.colors = colors;
//    return res;
//}
//#endif //CMS_PCS_OPTIMIZER_POINT_CLOUD_UTILITIES_H
