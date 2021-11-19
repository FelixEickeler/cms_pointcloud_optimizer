//
// Created by felix on 15.11.2021.
//
#include "iostream"
//#include <cilantro/core/kd_tree.hpp>
#include <tclap/CmdLine.h>
#include <filesystem>
#include "spdlog/spdlog.h"
#include "helpers.h"
#include "configuration_parser.h"
#include "cilantro_extensions/pointclouds.h"

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

        // Define a switch and add it to the command line.
        SwitchArg reverseSwitch("r", "reverse", "Print name backwards", false);
        cmd.add(reverseSwitch);

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
        auto [narc, strategy] = cms_opti::configurations::parse_configuration(config_path);

        PlyParser plyParser(input_path);
        auto header = plyParser.parse_header();
        int pcs_elector = 0;
        if(contains(header, "vertex")){
            auto& vm = header["vertex"];
            if(contains(vm, "x") && contains(vm, "y") && contains(vm, "z")){
                pcs_elector = pcs_elector | (1 << 3);
            }
//            if(contains(vm, "nx") && contains(vm, "ny") && contains(vm, "nz")){
//                pcs_elector = pcs_elector | (1 << 2);
//            }
            if(contains(vm, "red") && contains(vm, "green") && contains(vm, "blue")){
                pcs_elector = pcs_elector | (1 << 1);
            }
            if(contains(vm, "class") || contains(vm, "scalar")){
                pcs_elector = pcs_elector | (1 << 0);
            }
        }

        // TODO UGLY but works for now also define more point clouds also make it so it will work with float and double
        switch(pcs_elector){
            case 0b1000:

                {
                    using adaptor =  PointCloudCartesianAdaptor<cilantro::PointCloudXYZ<float>>;
                    auto point_cloud = plyParser.parse_data<adaptor>();
                    cms_opti::CompressionStrategyExecutor<cilantro::PointCloudXYZ<float>> executor(point_cloud, *strategy);
                    executor.compress();
//                    spdlog::get("console")->info("Only points could be identified1");
                }
                spdlog::get("console")->info("Only points could be identified");
                break;
            case 0b1001:
                spdlog::get("console")->info("Points & Scalar identified");
                break;
            case 0b1010:
            case 0b1011:
            case 0b1100:
            case 0b1101:
            case 0b1111:
                spdlog::get("console")->info("Multiple Parameters identified, Full datastructures will be used internally");
                break;


            default:
                spdlog::get("console")->critical("Very wrong dataformat, no Point could be mapped");


        }


//        auto [metadata, _tmp_cloud] = PlyParser::parse(input_path);

//        auto _xyz = dynamic_cast<cilantro::PointCloudXYZ<float>&>(*_tmp_cloud);


        spdlog::get("console")->info("LALA");
//        ply_parser.parse_ply(input_path);
//        auto input_points = cms_opti::load_pcs(input_path);





    } catch (ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
}