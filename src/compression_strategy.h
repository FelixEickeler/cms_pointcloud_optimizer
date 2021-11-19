//
// Created by felix on 15.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_COMPRESSION_STRATEGY_H
#define CMS_PCS_OPTIMIZER_COMPRESSION_STRATEGY_H
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "configurations.h"

using namespace cms_opti::configurations;
namespace cms_opti{
    class CompressionStrategy{
        public:
            virtual std::string get_name() = 0;
    };


    class AdaptiveBilateralCompression : public CompressionStrategy{
        private:
            AdaptiveBilateralParameters& config;

        public:
            explicit AdaptiveBilateralCompression(AdaptiveBilateralParameters& _config) : config(_config){}
            std::string get_name() override;

            template<class TPointCloud>
            void compress(TPointCloud& cloud){
                spdlog::get("console")->warn("{}", "yes");
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
