//
// Created by felix on 15.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_HELPERS_H
#define CMS_PCS_OPTIMIZER_HELPERS_H

#include <filesystem>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using Path = std::filesystem::path;
Path expand_user(Path _path);

extern std::shared_ptr<spdlog::logger> console;

constexpr unsigned int hash(const char *s, int off = 0) {
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];
}

template<class TSearched, typename TKey>
inline bool contains(TSearched s, TKey key)
{
    // Key is not present
    if (s.find(key) == s.end())
        return false;
    return true;
}


#endif //CMS_PCS_OPTIMIZER_HELPERS_H
