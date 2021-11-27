//
// Created by felix on 15.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_COMPRESSION_STRATEGY_H
#define CMS_PCS_OPTIMIZER_COMPRESSION_STRATEGY_H
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "configurations.h"
#include "cilantro_extensions/includes/pointcloud_mxins.h"
#include "compression_utilities.h"
#include <cilantro/core/normal_estimation.hpp>
#include <cilantro/core/data_containers.hpp>
#include <spdlog/stopwatch.h>
template<class TPointCloud>
using KDTree = cilantro::KDTree<typename TPointCloud::Scalar, TPointCloud::Param::Dimension, cilantro::KDTreeDistanceAdaptors::L2, size_t>;

using namespace cms_opti::configurations;
namespace cms_opti{
    struct EstimateNormalSetting{
        const static inline std::initializer_list<std::string> allowed_algorithms = {"knn"};
        std::string algorithm;
        int param{};

        explicit operator bool() const{
            return !algorithm.empty();
        }
    };

    class CompressionStrategy{
        public:
            EstimateNormalSetting estimate_normals;
            CompressionResultSetting compression_result_setting;
            CloudStatistics cloud_statistics = {};
            AdaptiveBilateralParameters adaptive_bilateral_config = {};

            CompressionStrategy()= default;
            explicit CompressionStrategy(AdaptiveBilateralParameters &_config) : adaptive_bilateral_config(_config){}
            CompressionStrategy(const CompressionStrategy& cs) = delete;
            CompressionStrategy& operator=(const CompressionStrategy&) = delete;
            virtual std::unique_ptr<CompressionStrategy> clone() const = 0;
    };


    class AdaptiveBilateralCompression : public CompressionStrategy{

        public:
            explicit AdaptiveBilateralCompression(AdaptiveBilateralParameters _config) : CompressionStrategy(_config) {}
            AdaptiveBilateralCompression(const AdaptiveBilateralCompression& cs) = delete;
            AdaptiveBilateralCompression& operator=(const AdaptiveBilateralCompression&) = delete;
            virtual std::unique_ptr<CompressionStrategy> clone() const {
                return std::unique_ptr<CompressionStrategy>(new AdaptiveBilateralCompression(this->adaptive_bilateral_config));
            }


            template<class TPointCloud>
            void compress(TPointCloud& cloud) {
                spdlog::get("console")->debug("CHECK SFINAE Points: {}", HasPointProperty<TPointCloud>::value);
                spdlog::get("console")->debug("CHECK SFINAE Normals: {}", HasNormalProperty<TPointCloud>::value);
                spdlog::get("console")->debug("CHECK SFINAE Colours: {}", HasColorProperty<TPointCloud>::value);
                spdlog::get("console")->debug("CHECK SFINAE Scalar: {}", HasScalarProperty<TPointCloud>::value);

                spdlog::stopwatch sw_full;
                spdlog::stopwatch sw_steps;
                if constexpr(HasNormalProperty<TPointCloud>::value){
                    if (cloud.normals.cols() > 0 && cloud.normals.col(0).norm() > 0) {
                        spdlog::get("console")->warn("Normals found but \"estimate normals\" was ordered. Normals will be re-estimated (e.g. overwritten)!!");
                    }

                    if(this->estimate_normals){
                        sw_steps.reset();
                        spdlog::get("console")->info("Estimating normals with {} = {}!", estimate_normals.algorithm, estimate_normals.param);
                        if(this->estimate_normals.algorithm == "knn"){
                            using xyz_type = typename TPointCloud::Scalar;
                            using covarianz_type = cilantro::Covariance<xyz_type, TPointCloud::Param::Dimension>;
                            cilantro::NormalEstimation<xyz_type, TPointCloud::Param::Dimension, covarianz_type, size_t> ne(cloud.points);
                            ne.setViewPoint(cilantro::Vector<xyz_type, TPointCloud::Param::Dimension>::Zero(TPointCloud::Param::Dimension, 1));
                            ne.estimateNormalsKNN(cloud.normals, estimate_normals.param);
                        }
                        spdlog::get("console")->info("Estimation took {:.3}s", sw_steps);
                    }
                }

                cloud_statistics = cms_opti::ComputeCloudResolution(cloud);
                const auto reduce_to = compression_result_setting.calculate_remaining(cloud.size());
                const auto number_points_to_be_removed = cloud.size() - reduce_to;
                const auto original_size = cloud.size();
//                spdlog::get("console")->info("The cloud points will be reduced from {} to {}", cloud.size(), reduce_to);
                auto header = fmt::format("  Cycle  │  Duration   │    Removed    │    {:>}/{:>}   │  Ratio", 0, reduce_to);
                spdlog::get("console")->info("┌{0:─^{2}}┐\n"
                                             "{0:^{3}}│{1: ^{2}}│\n"
                                             "{0:^{3}}└{0:─^{2}}┘", "", header, 64, 43);


//                spdlog::get("console")->info("Cycle {:>4}  took  {:>3.2f}s and removed {:>10} points\t{:>}/{:>} := {:>.2f}"
                int removed_points = 0;
                const auto param = adaptive_bilateral_config;
                const auto statistics = cloud_statistics;

                // TODO Expose radius parameter + tolerance factor
                const auto knn_radius = 5;
                const auto skip_tolerance_factor = 0.83;

                sw_steps.reset();
                auto pcnt = 0;

                auto tree = std::make_unique<KDTree<TPointCloud>>(cloud.points, 20);
                for(auto left_to_remove=number_points_to_be_removed; left_to_remove > 0; left_to_remove=cloud.size() - reduce_to) {
                    spdlog::stopwatch sw;
                    spdlog::get("console")->trace("Starting compression cycle : {}", pcnt);
                    spdlog::get("console")->trace("Tree rebuild in {}", sw);
                    spdlog::get("console")->trace("Initial Evaluation...", pcnt);

                    // Evaluate initial cloud cost
                    std::vector<IndexCostPair> idx_cost = calculate_cost(cloud, tree, param, statistics, knn_radius);
                    spdlog::get("console")->trace("Compressing ...", pcnt);

                    // tag points -> Note use n-th element if move back to chunks (original implementation)
                    std::sort(idx_cost.begin(), idx_cost.end(), [&](IndexCostPair& ic1, IndexCostPair& ic2){return ic1.cost < ic2.cost;} );
                    auto cycle_removes = 0;
                    std::vector<uint> to_be_removed;
                    uint tolerated_skips = skip_tolerance_factor *left_to_remove;
                    auto remaining_skips = tolerated_skips;

                    for(;cycle_removes < left_to_remove;){
                        auto& ic = idx_cost[cycle_removes];
                        float initial_cost = ic.cost * 1.01;
                        if(!EvaluateSinglePoint(cloud, tree, ic, param, statistics, knn_radius)){
                            spdlog::get("console")->debug("Tree miss:{}",  cycle_removes);
                            break;
                        }
                        if(initial_cost > 0.0001 && initial_cost < ic.cost) {
                            if(remaining_skips-- == 0){
                                spdlog::get("console")->debug("Number of tolerated skips reached");
                                break;
                            }
                            continue;
                        }
                        else{
                            cloud.points.col(ic.index) = remove_qualifier<TPointCloud>();
                            to_be_removed.push_back(ic.index);
                            ++cycle_removes;
                        }
                    }
                    // remove points from cloud
                    cloud.remove(to_be_removed);
                    removed_points += cycle_removes;
                    spdlog::get("console")->info("{:>7}  \t {:>5.2f}s     {:>9} pts\t {:>7}/{:<7}  {:>.2f}", pcnt++,sw,cycle_removes,
                                                 removed_points, number_points_to_be_removed, float(cloud.size()) / original_size);

                    // reset tree
                    tree = std::make_unique<KDTree<TPointCloud>>(cloud.points, 20);
                }
                auto footer = fmt::format("  {:^7}    {:>5.2f}s     {:>9} pts    {:>7}/{:<7}  {:>.2f}", pcnt, sw_steps, removed_points,
                                          cloud.size(), reduce_to, float(cloud.size()) / original_size);
                spdlog::get("console")->info("┌{0:─^{2}}┐\n"
                                             "{0:<{3}}│{1:<{2}}│\n"
                                             "{0:<{3}}└{0:─^{2}}┘", "", footer, 64, 43);




                if constexpr(HasCostProperty<TPointCloud>::value){
                    auto final_index_cost_pairs = calculate_cost(cloud, tree, param, statistics, knn_radius);
                    Eigen::RowVectorXf cost(cloud.size());
                    for(const auto& icp : final_index_cost_pairs){
                        cost = icp.value;
                    }
                    cloud.cost = cost;
                }
            }

            template<class TTree ,class TPointCloud>
            std::vector<IndexCostPair> calculate_cost(const TPointCloud &cloud, const TTree &tree, const AdaptiveBilateralParameters &param,
                                                      const CloudStatistics &statistics, const int knn_radius) {
                std::vector<IndexCostPair> idx_cost(cloud.size());
                std::iota(idx_cost.begin(), idx_cost.end(), 0);
                for(auto& ic : idx_cost){
                    EvaluateSinglePoint(cloud, tree, ic, param, statistics, knn_radius);
                }
                return idx_cost;
            }

            template<class TPointCloud>
            constexpr inline Eigen::Matrix<typename  TPointCloud::Scalar, TPointCloud::Param::Dimension, 1> remove_qualifier() const {
                return Eigen::Matrix<typename TPointCloud::Scalar, TPointCloud::Param::Dimension, 1>{
                        std::numeric_limits<typename TPointCloud::Scalar>::min(),
                        std::numeric_limits<typename TPointCloud::Scalar>::min(),
                        std::numeric_limits<typename TPointCloud::Scalar>::min()
                };
            }

            template<class TPointCloud, class TTree>
            bool EvaluateSinglePoint(const TPointCloud& cloud, const TTree& tree, IndexCostPair& ic,  const AdaptiveBilateralParameters& param,
                                     const CloudStatistics& statistics,  const int knn_radius = 5){
                constexpr auto divider = 1 + HasNormalProperty<TPointCloud>::value +  HasColorProperty<TPointCloud>::value +  HasIntensityProperty<TPointCloud>::value;
                float tmp_cost = 0;
                auto p = cloud.points.col(ic.index);
                // start evaluation
                cilantro::NeighborSet<float> n_nearest_neighbours = tree->kNNSearch(p, knn_radius);
                auto nn_it = n_nearest_neighbours.begin();
                std::next(nn_it);
                const auto qualitfier = remove_qualifier<TPointCloud>();

                for (auto end=n_nearest_neighbours.end(); nn_it!=end; ++nn_it) {
                    auto q = cloud.points.col(nn_it->index);
                    auto d_pq = nn_it->value;
                    // to many points removed before tree rebuild.
                    if(q == qualitfier){ return false;}

                    if (d_pq > param.c_thr) {
                        double w_d = 0.0;
                        double w_n = 0.0;
                        double w_rgb = 0.0;
                        double w_i = 0.0;
                        // TODO introduce for scalar fields
                        // double w_s = 0.0;

                        // Formula (2) with k=3
                        auto xpq = d_pq / statistics.max;
                        w_d = param.sig_d *(param.c_1 * pow(xpq, 3) - param.c_2 * pow(xpq, 2) + param.c_3 * xpq + param.c_4);

                        // Formula (3)
                        if constexpr(HasNormalProperty<TPointCloud>::value){
                            auto pn = cloud.normals.col(ic.index);
                            auto qn = cloud.normals.col(nn_it->index);
                            auto ntemp = pn.dot(qn) / (pn.norm() * qn.norm());
                            w_n = param.sig_n / M_PI_2 * acos(ntemp);
                        }

                        // Formula (4)
                        if constexpr(HasColorProperty<TPointCloud>::value){
                            auto prgb = cloud.colors.col(ic.index);
                            auto qrgb = cloud.colors.col(nn_it->index);
                            // TODO add normalization for different coulor spaces orignal impementation 765
                            w_rgb = param.sig_rgb / 256 * (prgb - qrgb).norm();
                        }

                        // Formula (7) Atm the evaluation of (5)/(6) should be done (pre)-dataloading => TODO: add internal evaluation of the radar equation
                        if constexpr(HasIntensityProperty<TPointCloud>::value) {
                            auto proh = cloud.colors.col(ic.index);
                            auto qroh = cloud.colors.col(nn_it->index);
                            // Simplified (7) as R^2 * Pr is roh. cos(alpha) = 1, cscan = const (thus will be removed from (7))
                            w_i = param.sig_i * (proh - qroh) / (statistics.roh_max - statistics.roh_min);
                        }
                        tmp_cost += float(w_d + w_n + w_rgb + w_i);
                    }
                }
                ic.cost = tmp_cost / divider;
                return true;
            }
    };

    template <class TPointCloud>
    class CompressionStrategyExecutor{
        private:
            CompressionStrategy* _strategy;
            TPointCloud& _cloud;

        public:
            CompressionStrategyExecutor(TPointCloud& cloud,
                                        CompressionStrategy& compression_strategy) : _cloud(cloud), _strategy(&compression_strategy) {}


            CompressionStrategy& strategy(){
                return *_strategy;
            }

            void compress(){

                auto* abs = dynamic_cast<AdaptiveBilateralCompression*>(_strategy);
                if(abs){
                    abs->compress<TPointCloud>(_cloud);
                    return;
                }
                spdlog::get("console")->warn("NO");
            }
    };

};



#endif //CMS_PCS_OPTIMIZER_COMPRESSION_STRATEGY_H
