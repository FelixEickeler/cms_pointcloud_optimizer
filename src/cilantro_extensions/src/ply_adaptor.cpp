//
// Created by felix on 24.11.2021.
//
#include "../includes/ply_adaptor.h"

#include <memory>
#include "../includes/io_name_mapping.h"
#include "../../helpers.h"
#include "cilantro/utilities/ply_io.hpp"

using memory_stream = cilantro::internal::MemoryStream;

PointCloudRequirements PlyAdaptor::infer_requirements() {
    plyfile->parse_header(*filestream);
    spdlog::get("console")->trace("PlyType: {} ", plyfile->is_binary_file() ? "binary" : "ascii");
    meta.is_binary = plyfile->is_binary_file();
    meta.comments = plyfile->get_comments();
    if(std::any_of(meta.comments.begin(), meta.comments.end(), [](const auto& comment){return comment == "dialect=tum_ply";})){
        meta.dialect = PlyDialect::tum_ply;
        spdlog::get("console")->info("Ply file adheres to tum best_practices !");
    }

    using namespace IONameMapping;
    for(const auto& e : plyfile->get_elements()){
        auto lename = to_lower(e.name);
        if(_element_mapping_ext2int.find(lename) != _element_mapping_ext2int.end()){
            std::unordered_map<std::string, std::string> pmap = {{_element_mapping_ext2int[lename], e.name}};
            for(const auto& p : e.properties){
                auto lpname = to_lower(p.name);
                if(_property_mapping_ext2int.find(lpname) != _property_mapping_ext2int.end()){
                    pmap[_property_mapping_ext2int[lpname]] = p.name;
                }
                else{
                    spdlog::get("console")->warn("Property {} of element {} is not mapped and therefore ignored", e.name, p.name);
                }
                //TODO add list types e.g.  if (p.isList) std::cout << " (list_type=" << tinyply::PropertyTable[p.listType].str << ")";
            }
            _m[_element_mapping_ext2int[lename]] = pmap;
            spdlog::get("console")->debug("Created mapping for {}", e.name);
        }
        else{
            spdlog::get("console")->warn("Element {} is not supported and therefore ignored", e.name);
        }
    }
    return determin_requirements();
    // determine requirments from header

}

bool PlyAdaptor::init(const Path& input_path) {
    std::error_code ec;
    std::uintmax_t file_size = std::filesystem::file_size(input_path, ec);
    if (ec) {
        spdlog::get("console")->critical("{} : {}", input_path.string(), ec.message());
    }
    spdlog::get("console")->trace("Filesize: {} KB", (file_size >> 10)+1);

    // 1 GB as threshold, tinyply claims faster loading times
    if(file_size < (1<<30)){
        std::ifstream _file(input_path, std::ios::binary);
        _file.seekg(0, std::ios::end);
        size_t sizeBytes = _file.tellg();
        _file.seekg(0, std::ios::beg);
        byte_buffer.resize(sizeBytes);
        _file.read((char*)byte_buffer.data(), sizeBytes);
        filestream = std::make_unique<memory_stream>((char*)byte_buffer.data(), byte_buffer.size());
    } else{
        filestream = std::make_unique<std::ifstream>(input_path, std::ios::binary);
    }
    if (!filestream || filestream->fail()) throw std::runtime_error("file_stream failed to open " + input_path.string());
    return true;
}

void PlyAdaptor::read() {
    plyfile->read(*filestream);
    byte_buffer.clear();
}

PointCloudRequirements PlyAdaptor::determin_requirements() {
    PointCloudRequirements pcr{};
    if(contains(_m, "vertex")) {
        auto &vm = _m["vertex"];
        if (contains(vm, "x") && contains(vm, "y") && contains(vm, "z")) pcr.points_3D = true;
        if (contains(vm, "nx") && contains(vm, "ny") && contains(vm, "nz")) pcr.has_normals = true;
        if (contains(vm, "red") && contains(vm, "green") && contains(vm, "blue") && !contains(vm, "alpha")) pcr.has_colors = true < 0;
        if (contains(vm, "red") && contains(vm, "green") && contains(vm, "blue") && contains(vm, "alpha")) pcr.has_colors = true < 1;
        if (contains(vm, "class") || contains(vm, "scalar")) pcr.has_classes = true;
    }
    return pcr;
}

PlyAdaptor::PlyAdaptor(PlyFile &file, PlyMapping &map) : _m(map){
    plyfile.reset(&file);
}

void PlyAdaptor::set_metadata(const Metadata &_meta) {
    meta = _meta;
}

/*
            property_list.emplace_back("Points");

            property_list.emplace_back("Normals");

            property_list.emplace_back("Colours");

            property_list.emplace_back("ScalarFields");
 */