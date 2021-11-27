//
// Created by felix on 18.11.2021.
//

#include "../includes/file_parser.h"

#include <utility>


Metadata FileParser::get_metadata() {
    return meta;
}

PointCloudRequirements FileParser::parse_header() {
    return adaptor->infer_requirements();
}

bool FileParser::supports_file() {
    return adaptor != nullptr;
}



