//
// Created by felix on 18.11.2021.
//

#include "../includes/ply_parser.h"

#include <utility>

std::unordered_map<std::string, std::string> PlyParser::_element_mapping_ext2int = {
        {"vertex","vertex"},
        {"point","vertex"},
        {"points","vertex"},
        {"vertices","vertex"},
//        {"face","face"},
//        {"objects", "objects"},
//        {"object_ids, object_ids"},
};


std::unordered_map<std::string, std::string> PlyParser::_property_mapping_ext2int = {
        {"x","x"},
        {"y","y"},
        {"z","z"},
        {"x1","x"},
        {"x2","y"},
        {"x3","z"},
        {"nx","nx"},
        {"ny","ny"},
        {"nz","nz"},
        {"n1","nx"},
        {"n2","ny"},
        {"n3","nz"},
        {"red","red"},
        {"green","green"},
        {"blue","blue"},
        {"alpha","alpha"},
//        {"objects","objects"},
//        {"id","objects"},
//        {"vertex_indices","vertex_indices"}
};

Metadata PlyParser::get_metadata() {
    return meta;
}

const PlyMapping &PlyParser::parse_header() {
    file.parse_header(*file_stream);
    spdlog::get("console")->info("PlyType: {} ", file.is_binary_file() ? "binary" : "ascii");
    meta.is_binary = file.is_binary_file();
    meta.comments = file.get_comments();
    if(std::any_of(meta.comments.begin(), meta.comments.end(), [](const auto& comment){return comment == "dialect=tum_ply";})){
        meta.dialect = PlyDialect::tum_ply;
        spdlog::get("console")->info("Ply file adheres to tum best_practices !");
    }
    // TODO validate info parsing
    // for (const auto & c : file.get_info()) std::cout << "\t[ply_header] Info: " << c << std::endl;

    for(const auto& e : file.get_elements()){
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
            mapping[_element_mapping_ext2int[lename]] = pmap;
            spdlog::get("console")->info("Created mapping for {}", e.name);
        }
        else{
            spdlog::get("console")->warn("Element {} is not supported and therefore ignored", e.name);
        }
    }
    return mapping;
}

PlyParser::PlyParser(Path path) : input_path(std::move(path)){
    std::error_code ec;
    std::uintmax_t file_size = std::filesystem::file_size(input_path, ec);
    if (ec) {
        spdlog::get("console")->critical("{} : {}", input_path.string(), ec.message());
    }
    spdlog::get("console")->info("Filesize: {} KB", (file_size >> 10)+1);

    // 1 GB as threshold, tinyply claims faster loading times
    if(file_size > (1<<30)){
        std::ifstream _file(input_path, std::ios::binary);
        _file.seekg(0, std::ios::end);
        size_t sizeBytes = _file.tellg();
        _file.seekg(0, std::ios::beg);
        byte_buffer.resize(sizeBytes);
        _file.read((char*)byte_buffer.data(), sizeBytes);
    } else{
        file_stream = std::make_unique<std::ifstream>(input_path, std::ios::binary);
    }
    if (!file_stream || file_stream->fail()) throw std::runtime_error("file_stream failed to open " + input_path.string());
}

