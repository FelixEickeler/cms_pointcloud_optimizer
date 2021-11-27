//
// Created by felix on 24.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_IO_NAME_MAPPING_H
#define CMS_PCS_OPTIMIZER_IO_NAME_MAPPING_H

namespace IONameMapping{
    using str2str = std::unordered_map<std::string, std::string>;

    static inline str2str _element_mapping_ext2int ={
            {"vertex","vertex"},
            {"point","vertex"},
            {"points","vertex"},
            {"vertices","vertex"},
    };

    static inline str2str _property_mapping_ext2int = {
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
            {"class","class"},
            {"object","class"},
            {"scalar_1","scalar_1"},
            {"s1","scalar_1"},
    };

    inline static str2str _init_std_mapping(){
        str2str merged;
        for(auto p : _element_mapping_ext2int){
            merged.insert(p);
        }
        for(auto p : _property_mapping_ext2int){
            merged.insert(p);
        }
        return merged;
    }

    static inline PlyMapping _standard_mapping{
            {"vertex" , _init_std_mapping()}
    };
}

#endif //CMS_PCS_OPTIMIZER_IO_NAME_MAPPING_H
