//
// Created by felix on 15.11.2021.
//

#include "helpers.h"

Path expand_user(Path _path) {
    const char *home = getenv("HOME");
    if (home == nullptr) {
        spdlog::get("console")->warn("HOME variable not set. No user expand possible");
        return _path;
    }
    auto str_path = _path.string();
    if (str_path[0] == '~') return Path(std::string(home) + str_path.substr(1, str_path.size() - 1));
    else return _path;
}

std::shared_ptr<spdlog::logger> console = spdlog::stdout_color_mt("console");