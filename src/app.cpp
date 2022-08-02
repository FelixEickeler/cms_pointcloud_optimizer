//
// Created by felix on 15.11.2021.
//
#include "iostream"
#include <tclap/CmdLine.h>
#include <filesystem>
#include <random>
#include "spdlog/spdlog.h"
#include "helpers.h"
#include "configuration_parser.h"
#include "cilantro_extensions/pointclouds.h"
#include "cilantro_extensions/includes/file_parser.h"
#include "cilantro_extensions/includes/file_writer.h"

using namespace TCLAP;
using Path = std::filesystem::path;
int main(int argc, char **argv) {
    try {
        // Define a value argument and add it to the command line.
        CmdLine cmd("This tool can be used to make a lossy but optimized version of a point cloud", ' ', "0.1");
        ValueArg<std::string> inputArg("i", "input_path", "path to a point cloud [*.ply]", true, "homer", "string");
        cmd.add(inputArg);
        ValueArg<std::string> outputArg("o", "output_path", "output path for the compressed point cloud [*.ply]", true, "homer", "string");
        cmd.add(outputArg);
        ValueArg<std::string> configArg("c", "config_path",
                                        "path to a given configuration file. Not existing attributes will be taken from std_config.json [*.json]", false,
                                        "", "string");
        cmd.add(configArg);

        ValueArg<std::string> estimateNormalArg("", "estimate_normals",
                                        "Estimate normals. Possible values: knn=N where N is the NN-Count [default: knn=5]", false,
                                        "", "string");
        cmd.add(estimateNormalArg);

        SwitchArg outputCostArg("", "update_cost", "Writes the cost as scalar filed", false);
        cmd.add(outputCostArg);

        ValueArg<float> ratioArg("r", "compression_ratio",
                                                "Define compression ratio", false,
                                                0.4, "float");
        ValueArg<long> pointsArg("p", "points",  "Define Number of points that should be kept", false,
                                                0, "long");
        cmd.xorAdd(ratioArg, pointsArg);
        cmd.parse(argc, argv);

        // Get the value parsed by each arg.
        auto input_path = expand_user(Path(inputArg.getValue()));
        auto output_path = expand_user(Path(outputArg.getValue()));
        auto config_path = expand_user(Path(configArg.getValue()));

        // Check the input and output_path
        if (not exists(input_path)) {
            spdlog::warn("The input path: {} does not exist!", input_path.string());
            return 1;
        }
        if (not exists(output_path.parent_path())) {
            spdlog::warn("The parent of the output path: {} does not exist!", output_path.string());
            return 1;
        }
        if (not exists(config_path)) {
            spdlog::info("No configuration given, the input will be processed with the default configuration!", config_path.string());
        }

        // parse configuration and select strategy
        NonApproachRelatedConfig narc;
        std::unique_ptr<cms_opti::CompressionStrategy> pointer;
        auto strategy = cms_opti::configurations::parse_configuration(config_path, narc, pointer);
        strategy->compression_result_setting = CompressionResultSetting(ratioArg.getValue(), pointsArg.getValue());

        // flip switches
        // Normals
        auto arg_normals =  estimateNormalArg.getValue();
        if(arg_normals != "") {
            auto pos = arg_normals.find('=');
            cms_opti::EstimateNormalSetting estimate_normals;
            if (pos != std::string::npos) {
                estimate_normals.algorithm = to_lower(arg_normals.substr(0, pos));
                if (contains(cms_opti::EstimateNormalSetting::allowed_algorithms, estimate_normals.algorithm)) {
                    try {
                        estimate_normals.param = std::stoi(arg_normals.substr(pos + 1));
                        strategy->estimate_normals = estimate_normals;
                    }
                    catch (const std::invalid_argument &e) {
                        spdlog::warn("Second argument of estimate_normals is not valid [first=second]");
                        estimate_normals.algorithm = "";
                    }
                } else {
                    spdlog::warn("The first argument of estimate_normals is not supported [first=second]");
                    estimate_normals.algorithm = "";
                }
            } else {
                spdlog::warn("The given estimate_normals parameters are not supported");
                estimate_normals.algorithm = "";
            }
        }

        // EVALUATE
        FileParser fileParser(input_path);
        if(!fileParser.supports_file()){return 0;}
        auto pointcloud_requirements = fileParser.parse_header();

        FileWriter fileWriter(output_path, Metadata());
        FileOutputSettings out_setting;
        out_setting.pcr = pointcloud_requirements;
        out_setting.add_cost = outputCostArg.getValue();
        fileWriter.output_settings(out_setting);

        // TODO UGLY but works for now also define more point clouds also make it so it will work with float and double
        using spt = SupportedPointCloudTypes;
        using cms_opti::compression_wrapper;
        switch(pointcloud_requirements.type()){
            case spt::XYZ: compression_wrapper<cilantro::PointCloudXYZ<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_N: compression_wrapper<cilantro::PointCloudXYZN<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_RGB:
                if(arg_normals == "") {
                    cms_opti::EstimateNormalSetting estimate_normals;
                    estimate_normals.algorithm = "knn";
                    estimate_normals.param = 5;
                    strategy->estimate_normals = estimate_normals;
                }
            case spt::XYZ_N_RGB: compression_wrapper<cilantro::PointCloudXYZNRGB<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_I:
            case spt::XYZ_N_I: compression_wrapper<cilantro::PointCloudXYZNI<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_N_RGB_I: compression_wrapper<cilantro::PointCloudXYZNRGBI<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_N_T: compression_wrapper<cilantro::PointCloudXYZNT<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_RGB_T:
            case spt::XYZ_N_RGB_T: compression_wrapper<cilantro::PointCloudXYZNRGBT<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_N_I_T: compression_wrapper<cilantro::PointCloudXYZNIT<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_N_RGB_I_T: compression_wrapper<cilantro::PointCloudXYZNRGBIT<float>>(fileParser, fileWriter, strategy); break;
            case spt::XYZ_N_RGBA_I_T: compression_wrapper<cilantro::PointCloudXYZNRGBAIT<float>>(fileParser, fileWriter, strategy); break;

            default:
                spdlog::critical("Very wrong dataformat, no Point could be mapped");
        }
        spdlog::info("\t ---   Output  written to {}     ---", output_path.string());

    } catch (ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
}