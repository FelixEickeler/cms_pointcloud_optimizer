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

    template<class TPointcloud, class TTree>
    CloudStatistics ComputeCloudResolution(const TPointcloud &cloud, const TTree& tree, size_t count=10){
        if constexpr(HasPointProperty<TPointcloud>::value){
            CloudStatistics cr;
            auto begin = cloud.points.colwise();
            for(auto idx=0; idx < cloud.size(); ++idx){
                cilantro::NeighborSet<float> nn = tree->kNNSearch(cloud.points.col(idx), count+1);
                std::for_each(std::next(nn.begin()), nn.end(), [&cr](auto& nn){
                    nn.value = sqrt(nn.value); // transform
                    cr.mean += nn.value; // accumulate
                });

                cr.max = std::max(cr.max, float(sqrt(nn.back().value)));
                cr.min = std::min(cr.min, float(sqrt(nn[1].value)));
            }
            cr.mean /= cloud.size() * count;
            if constexpr(HasIntensityProperty<TPointcloud>::value){
                cr.roh_min = cloud.intensity.minCoeff();
                cr.roh_max = cloud.intensity.maxCoeff();
            }
            return cr;
        }
    }



}


//cilantro::NeighborSet<float> nn = tree.kNNInRadiusSearch(Eigen::Vector3f(0.1, 0.1, 0.4), 2, 1.001);



#endif //CMS_PCS_OPTIMIZER_COMPRESSION_UTILITIES_H
