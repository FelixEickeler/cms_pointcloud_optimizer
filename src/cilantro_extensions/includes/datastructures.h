//
// Created by felix on 25.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_DATASTRUCTURES_H
#define CMS_PCS_OPTIMIZER_DATASTRUCTURES_H
#include <unordered_map>
#include <filesystem>
#include <stdint.h>

using PlyMapping = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;
using Path = std::filesystem::path;



enum PlyDialect{
    custom,
    tum_ply
};

struct Metadata{
    std::vector<std::string> comments = {};
    bool is_binary = false;
    PlyDialect dialect = PlyDialect::custom;
};


enum SupportedPointCloudTypes {
    XY_N = 2,
    XY_RGB = 4,
    XY_RGBA = 8,
    XY_I = 16,
    XY_R = 32,
    XY_T = 64,
    XY_C = 1024,

    XYZ = 1,
    XYZ_N = XYZ + XY_N,
    XYZ_RGB = XYZ + XY_RGB,
    XYZ_RGBA = XYZ + XY_RGBA,
    XYZ_I = XYZ + XY_I,
    XYZ_N_RGB = XYZ_N + XY_RGB,
    XYZ_N_RGBA = XYZ_N + XY_RGBA,
    XYZ_N_I = XYZ_N + XY_I,
    XYZ_N_RGB_I = XYZ_N_RGB + XY_I,
    XYZ_N_RGBA_I = XYZ_N_RGBA + XY_I,

    XYZ_N_T =  XYZ_N + XY_T,
    XYZ_RGB_T = XYZ_RGB + XY_T,
    XYZ_N_RGB_T = XYZ_N_RGB + XY_T,
    XYZ_N_I_T = XYZ_N_I + XY_T,
    XYZ_N_RGB_I_T = XYZ_N_RGB_I + XY_T,
    XYZ_N_RGBA_I_T = XYZ_N_RGBA_I + XY_T,
};

struct PointCloudRequirements {

    union {
        struct {
            uint16_t points_3D : 1;
            uint16_t has_normals: 1;
            uint16_t has_colors: 2;
            uint16_t has_intensity: 2;
            uint16_t has_classes: 1;
            uint16_t has_integer: 3;
            uint16_t has_cost: 1;
            uint16_t has_float: 3;
            uint16_t unused: 2;
        };
        uint16_t binary{};
    };

    [[nodiscard]] SupportedPointCloudTypes type() const;
};

struct FileOutputSettings{
    PointCloudRequirements pcr {};
    bool binary = false;
    ulong chunks = 0;
    bool compressed = false;
    bool add_cost = true;
    [[nodiscard]] bool split_into_chunks() const;
};

inline bool FileOutputSettings::split_into_chunks() const {
    return chunks != 0;
}

inline SupportedPointCloudTypes PointCloudRequirements::type() const {
    return SupportedPointCloudTypes(binary);
}

namespace Eigen {
    using MatrixXu8 = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>;
    using MatrixXu16 = Eigen::Matrix<uint16_t, Eigen::Dynamic, Eigen::Dynamic>;
    using MatrixXu32 = Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>;
}
#endif //CMS_PCS_OPTIMIZER_DATASTRUCTURES_H
