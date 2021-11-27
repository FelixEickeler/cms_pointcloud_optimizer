//
// Created by felix on 15.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_HELPERS_H
#define CMS_PCS_OPTIMIZER_HELPERS_H

#include <filesystem>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "cilantro_extensions/includes/helpers.h"

using Path = std::filesystem::path;
Path expand_user(Path _path);
extern std::shared_ptr<spdlog::logger> console;

#endif //CMS_PCS_OPTIMIZER_HELPERS_H
