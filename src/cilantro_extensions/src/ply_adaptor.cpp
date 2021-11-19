//
// Created by felix on 18.11.2021.
//

#include "../includes/ply_adaptor.h"

std::shared_ptr<PlyData> PlyAdaptor::request_element(const std::string &element_name, std::initializer_list<std::string> properties) {
    auto lename = to_lower(element_name);
    std::vector<std::string> props;
    for(const auto& p : properties){
        props.emplace_back(_m[lename][to_lower(p)]);
    }
    return plyfile.request_properties_from_element(_m[lename][lename], properties);
}

PlyAdaptor::PlyAdaptor(PlyFile& _plyfile, PlyMapping mapping) : plyfile(_plyfile), _m(std::move(mapping)) {}

//PointCloudCartesianAdaptor::PointCloudCartesianAdaptor(const PlyFile& file, const PlyMapping& map) : PlyAdaptor(file, map) {}