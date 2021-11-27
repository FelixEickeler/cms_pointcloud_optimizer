//
// Created by felix on 22.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_COMPRESSION_UTILITIES_H
#define CMS_PCS_OPTIMIZER_COMPRESSION_UTILITIES_H
#include "cilantro/core/kd_tree.hpp"
#include "cilantro_extensions/includes/pointcloud_mxins.h"
#include <numeric>
#include <iostream>
#include "spdlog/spdlog.h"

namespace cms_opti{
    struct CloudStatistics{
        float mean = 0;
        float min =  std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::min();
        float roh_min = std::numeric_limits<float>::max();
        float roh_max = std::numeric_limits<float>::min();
    };

    struct IndexCostPair{
        uint index = 0;
        float cost = 0;

        IndexCostPair& operator=(const uint rhs){
            index = rhs;
            return *this;
        }
    };

    template<class TPointcloud>
    CloudStatistics ComputeCloudResolution(const TPointcloud &cloud){
        if constexpr(HasPointProperty<TPointcloud>::value){
            auto count = 5;
            cilantro::KDTree3f<> tree(cloud.points);
            CloudStatistics cr;
            //TODO use parallel version with 128 ?`
            for(auto idx=0; idx < cloud.size(); ++idx){
                cilantro::NeighborSet<float> nn = tree.kNNSearch(cloud.points.col(idx), count+1);
                cr.mean += std::accumulate(nn.begin()+1, nn.end(), 0.0, [](auto acc, const auto& _nn){
                    return acc + _nn.value;
                });
                cr.max = std::max(cr.max, nn.back().value);
                cr.min = std::min(cr.min, nn[1].value);
            }
            cr.mean /= cloud.size() * count;
            spdlog::get("console")->info("Cloudresoltion is {}, mind_dist: {} , max_distance:{}", cr.mean, cr.min, cr.max);
            if constexpr(HasIntensityProperty<TPointcloud>::value){
                cr.roh_min = cloud.intensities.minCoeff();
                cr.roh_max = cloud.intensities.maxCoeff();
            }
            return cr;
        }
    }


}


//cilantro::NeighborSet<float> nn = tree.kNNInRadiusSearch(Eigen::Vector3f(0.1, 0.1, 0.4), 2, 1.001);



#endif //CMS_PCS_OPTIMIZER_COMPRESSION_UTILITIES_H
