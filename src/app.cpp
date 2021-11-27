//
// Created by felix on 15.11.2021.
//
#include "iostream"
//#include <cilantro/core/kd_tree.hpp>
#include <tclap/CmdLine.h>
#include "tclap/Arg.h"
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
        // Define the command line object.
        CmdLine cmd("This tool can be used to make a lossy but optimized version of a point cloud", ' ', "0.1");

        // Define a value argument and add it to the command line.
        ValueArg<std::string> inputArg("i", "input_path", "path to a point cloud [*.ply]", true, "homer", "string");
        cmd.add(inputArg);
        ValueArg<std::string> outputArg("o", "output_path", "output path for the compressed point cloud [*.ply]", true, "homer", "string");
        cmd.add(outputArg);
        ValueArg<std::string> configArg("c", "config_path",
                                        "path to a given configuration file. Not existing attributes will be taken from std_config.json [*.json]", false,
                                        "homer", "string");
        cmd.add(configArg);

        ValueArg<std::string> estimateNormalArg("", "estimate_normals",
                                        "Estimate normals. Possible values: knn=N where N is the NN-Count [default: knn=5]", false,
                                        "knn=5", "string");
        cmd.add(estimateNormalArg);

        ValueArg<float> ratioArg("r", "compression_ratio",
                                                "Define compression ratio", false,
                                                0.4, "float");
        ValueArg<long> pointsArg("p", "points",  "Define Number of points that should be kept", false,
                                                0, "long");
        cmd.xorAdd(ratioArg, pointsArg);

        // Parse the args.
        cmd.parse(argc, argv);

        // Get the value parsed by each arg.
        auto input_path = expand_user(Path(inputArg.getValue()));
        auto output_path = expand_user(Path(outputArg.getValue()));
        auto config_path = expand_user(Path(configArg.getValue()));

        // Do what you intend too...
        if (not exists(input_path)) {
            spdlog::get("console")->warn("The input path: {} does not exist!", input_path.string());
            return 1;
        }
        if (not exists(output_path.parent_path())) {
            spdlog::get("console")->warn("The parent of the output path: {} does not exist!", output_path.string());
            return 1;
        }
        if (not exists(config_path)) {
            spdlog::get("console")->info("No configuration given, the input will be processed with the default configuration!", config_path.string());
        }
        NonApproachRelatedConfig narc;
        std::unique_ptr<cms_opti::CompressionStrategy> pointer;
        auto strategy = cms_opti::configurations::parse_configuration(config_path, narc, pointer);
//        auto& ab = dynamic_cast<cms_opti::AdaptiveBilateralCompression&>(*strategy);
        strategy->compression_result_setting = CompressionResultSetting(ratioArg.getValue(), pointsArg.getValue());

        auto arg_normals =  estimateNormalArg.getValue();
        auto pos = arg_normals.find('=');
        cms_opti::EstimateNormalSetting estimate_normals;

        if(pos != std::string::npos){
            estimate_normals.algorithm = to_lower(arg_normals.substr(0, pos));
            if(contains(cms_opti::EstimateNormalSetting::allowed_algorithms, estimate_normals.algorithm)){
                try{
                    estimate_normals.param= std::stoi(arg_normals.substr(pos+1));
                    strategy->estimate_normals = estimate_normals;
                }
                catch (const std::invalid_argument& e) {
                    spdlog::get("console")->warn("Second argument of estimate_normals is not valid [first=second]");
                    estimate_normals.algorithm = "";
                }
            }
            else{
                spdlog::get("console")->warn("The first argument of estimate_normals is not supported [first=second]");
                estimate_normals.algorithm = "";
            }
        } else{
          spdlog::get("console")->warn("The given estimate_normals parameters are not supported");
          estimate_normals.algorithm = "";
        }

        FileParser fileParser(input_path);
        if(!fileParser.supports_file()){return 0;}
        auto pointcloud_requirements = fileParser.parse_header();

//        spdlog::get("console")->info("Properties identified: {}", fmt::format("{}", fmt::join(property_list.begin(), property_list.end(), ", ")));
        FileWriter fileWriter(output_path, Metadata());
        FileOutputSettings out_setting;
        out_setting.pcr = pointcloud_requirements;
        fileWriter.output_settings(out_setting);

        // TODO UGLY but works for now also define more point clouds also make it so it will work with float and double
        using spt = SupportedPointCloudTypes;
        switch(pointcloud_requirements.type()){
            case spt::XYZ:{
                    auto point_cloud = fileParser.parse_data<cilantro::PointCloudXYZ<float>>();
                    cms_opti::CompressionStrategyExecutor<cilantro::PointCloudXYZ<float>> executor(point_cloud, *strategy);
                    executor.compress();
                } break;
            case spt::XYZ_N:{
                    auto point_cloud = fileParser.parse_data<cilantro::PointCloudXYZN<float>>();
                    cms_opti::CompressionStrategyExecutor<cilantro::PointCloudXYZN<float>> executor(point_cloud, *strategy);
                    executor.compress();
                    fileWriter.write<cilantro::PointCloudXYZN<float>>(point_cloud);
//                    successfull_written = plyWriter.
                } break;
            case 0b1001:
                break;
            case 0b1010:
            case 0b1011:
            case 0b1101:
            case 0b1111:
                spdlog::get("console")->info("Multiple Parameters identified, Full datastructures will be used internally");
                break;
            default:
                spdlog::get("console")->critical("Very wrong dataformat, no Point could be mapped");


        }
        spdlog::get("console")->info("\t ---   Output  written to {}     ---", output_path.string());






    } catch (ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
}