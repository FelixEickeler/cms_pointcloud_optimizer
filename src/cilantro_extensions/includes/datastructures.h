//
// Created by felix on 25.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_DATASTRUCTURES_H
#define CMS_PCS_OPTIMIZER_DATASTRUCTURES_H
#include <unordered_map>
#include <filesystem>

using PlyMapping = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;
using Path = std::filesystem::path;

enum SupportedPointCloudTypes {
    XYZ = 1,
    XYZ_N = 3,
    XYZ_RGB = 5,
    XYZ_N_RGB = 7,
    XYZ_N_I = 19,
    XYZ_N_RGB_I = 23,
    XYZ_N_RGB_I_C = 87
};


enum PlyDialect{
    custom,
    tum_ply
};

struct Metadata{
    std::vector<std::string> comments = {};
    bool is_binary = false;
    PlyDialect dialect = PlyDialect::custom;
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
            uint16_t unused: 1;
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
