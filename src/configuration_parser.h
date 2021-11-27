//
// Created by felix on 15.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_CONFIGURATION_PARSER_H
#define CMS_PCS_OPTIMIZER_CONFIGURATION_PARSER_H
#include <nlohmann/json.hpp>
#include "configurations.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "spdlog/spdlog.h"
#include "compression_strategy.h"
#include "helpers.h"


using Path = std::filesystem::path;
using json = nlohmann::json;

namespace cms_opti::configurations{
    // Parsing definitions
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AdaptiveBilateralParameters, c, c_thr, c_1, c_2, c_3, c_4, sig_d, sig_n, sig_rgb, sig_i);

    std::unique_ptr<cms_opti::CompressionStrategy> parse_configuration(const Path &configuration_path, NonApproachRelatedConfig& narc, std::unique_ptr<cms_opti::CompressionStrategy>& lolo);
    NonApproachRelatedConfig parse_outer_configuration(const json &outer_json);
    std::unique_ptr<CompressionStrategy> strategy_factory(const std::string& strategy_name, const json& valid_config);
    json json_from_file(const Path &path);
    json json_from_default(const Path &path);

#include "configuration_parser.ipp"
}


#endif //CMS_PCS_OPTIMIZER_CONFIGURATION_PARSER_H
