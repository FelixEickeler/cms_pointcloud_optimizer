//
// Created by felix on 25.11.2021.
//

#ifndef CMS_CILANTRO_EXTENSION_HELPERS_H
#define CMS_CILANTRO_EXTENSION_HELPERS_H
#include <string>
#include <vector>
#include <algorithm>

std::string to_lower(std::string s);

constexpr unsigned int hash(const char *s, int off = 0) {
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];
}

template<class TSearched, typename TKey>
inline bool contains(TSearched s, TKey key){
    // Key is not present
    if (s.find(key) == s.end())
        return false;
    return true;
}


inline bool contains(std::initializer_list<std::string> s, std::string  key){
    // Key is not present
    std::vector _s(s);
    if(std::none_of(_s.begin(), _s.end(), [key](const auto& _str){
        return _str == key;
    })){return false;}
    return true;
}


#endif //CMS_CILANTRO_EXTENSION_HELPERS_H
