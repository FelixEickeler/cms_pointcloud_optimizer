//
// Created by felix on 15.11.2021.
//
#include "configuration_parser.h"
#include "helpers.h"
#include <memory>

using Path = std::filesystem::path;
using json = nlohmann::json;
using namespace cms_opti;

namespace cms_opti::configurations{

std::unique_ptr<cms_opti::CompressionStrategy> parse_configuration(const Path &configuration_path, NonApproachRelatedConfig& narc, std::unique_ptr<cms_opti::CompressionStrategy>& lolo) {
    auto given_config = json_from_file(configuration_path);
    auto config = json_from_default("./default_config.json");
    config.merge_patch(given_config);
    spdlog::get("console")->info("Configuraiton to_be_used:\n{}", config.dump(4));
    narc = parse_outer_configuration(config);
    return strategy_factory(config["compression_strategy"], config["compression_parameters"]);
}

std::unique_ptr<CompressionStrategy> strategy_factory(const std::string& strategy_name, const json &valid_config) {
    std::unique_ptr<CompressionStrategy> strategy;
    switch (hash(strategy_name.c_str())) {
        case hash("AdaptiveBilateralCompression"): {
            auto parameters = valid_config.get<AdaptiveBilateralParameters>();
            strategy = std::unique_ptr<CompressionStrategy>(new AdaptiveBilateralCompression(parameters));
            break;
        }
        default:
            spdlog::get("console")->warn("No valid compression selected");
    };
    return strategy;
}

NonApproachRelatedConfig parse_outer_configuration(const json &outer_json) {
    NonApproachRelatedConfig config{};
    if (outer_json.contains("comment")) config.comment = outer_json["comment"].get<std::string>();
    if (outer_json.contains("version")) config.version = outer_json["version"].get<std::string>();
    if (outer_json.contains("evaluate_on_property") && outer_json["evaluate_on_property"].is_array())
        config.evaluate_on_property = outer_json["evaluate_on_property"].get<std::vector<std::string>>();
    return config;
}

json json_from_file(const Path &path) {
    json j;
    try {
        if (exists(path)) {
            std::ifstream i(path.c_str());
            j = json::parse(i);
        } else {
            j["comment"] = "Optimization Default Config";
        }
    }
    catch (const std::exception &e) {
        spdlog::get("console")->info("File {} could not be read or is not a valid json", path.stem().string());
        throw std::runtime_error("FileNotFound");
    }
    return j;
}

json json_from_default(const Path &path = Path("./default_config.json")) {
    auto dpath = Path("./default_config.json");
    if (!exists(dpath)) {
        spdlog::get("console")->critical("The default config was not found. Please make sure it exists in the same folder as the binary!");
        throw std::runtime_error("File: default_config.json not found");
    }
    std::ifstream di(dpath);
    return json::parse(di);
}

}