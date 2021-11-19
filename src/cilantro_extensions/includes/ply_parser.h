//
// Created by felix on 17.11.2021.
//
#ifndef CMS_PCS_OPTIMIZER_PLY_PARSER_H
#define CMS_PCS_OPTIMIZER_PLY_PARSER_H
#include "../pointclouds.h"
#include <filesystem>
#include <memory>
#include <cilantro/utilities/ply_io.hpp>
#include "spdlog/spdlog.h"
#include "ply_adaptor.h"
#include <unordered_map>
#include "spdlog/stopwatch.h"
#include "ply_adaptor.h"

using namespace tinyply;
using memory_stream = cilantro::internal::MemoryStream;
using Path = std::filesystem::path;
using PlyMapping = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

inline std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),  [](unsigned char c){ return std::tolower(c); });
    return s;
}

enum PlyDialect{
    custom,
    tum_ply
};

struct Metadata{
    std::vector<std::string> comments = {};
    bool is_binary = false;
    PlyDialect dialect = PlyDialect::custom;
};

class  PlyParser{
    static std::unordered_map<std::string, std::string> _element_mapping_ext2int;
    static std::unordered_map<std::string, std::string> _property_mapping_ext2int;
    private:
        std::unique_ptr<std::istream> file_stream;
        std::vector<uint8_t> byte_buffer;
        PlyFile file;

    public:
        Metadata meta;
        PlyMapping mapping = {};
        const Path input_path;

        PlyParser(Path  path);
        const PlyMapping& parse_header();
        Metadata get_metadata();

        template <typename Adaptor>
        typename Adaptor::PointCloudType& parse_data() {
            Adaptor adaptor(file, mapping);
            adaptor.request_properties();
            spdlog::stopwatch sw;
            file.read(*file_stream);
            spdlog::info("Reading Ply in {:.3}s", sw);
            adaptor.parse_pointcloud();
            return static_cast<typename Adaptor::PointCloudType&>(*adaptor.request_pointcloud());
        }
};


// header creates mapping
// buffer into mapped parameters
// every pointcloud can use an adaptor to fill in the mapped parameters (similar to nloh/json)
// profit

#endif //CMS_PCS_OPTIMIZER_PLY_PARSER_H
