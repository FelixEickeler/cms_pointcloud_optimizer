//
// Created by felix on 18.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_POINTCLOUD_MXINS_H
#define CMS_PCS_OPTIMIZER_POINTCLOUD_MXINS_H
template <typename ...>
using void_t  = void;

template <typename T, typename = void>
struct HasPointProperty : std::false_type { };

template <typename T>
struct HasPointProperty<T, void_t<decltype(T::points)>> : std::true_type { };

template <typename T, typename = void>
struct HasNormalProperty : std::false_type { };

template <typename T>
struct HasNormalProperty<T, void_t<decltype(T::normals)>> : std::true_type { };

template <typename T, typename = void>
struct HasColorProperty : std::false_type { };

template <typename T>
struct HasColorProperty<T, void_t<decltype(T::colors)>> : std::true_type { };

template <typename T, typename = void>
struct HasScalarProperty : std::false_type { };

template <typename T>
struct HasScalarProperty<T, void_t<decltype(T::scalars)>> : std::true_type { };

template <typename T, typename = void>
struct HasIntensityProperty : std::false_type { };

template <typename T>
struct HasIntensityProperty<T, void_t<decltype(T::intensity)>> : std::true_type { };

template <typename T, typename = void>
struct HasClassProperty : std::false_type { };

template <typename T>
struct HasClassProperty<T, void_t<decltype(T::type)>> : std::true_type { };

template <typename T, typename = void>
struct HasCostProperty : std::false_type { };

template <typename T>
struct HasCostProperty<T, void_t<decltype(T::cost)>> : std::true_type { };



#endif //CMS_PCS_OPTIMIZER_POINTCLOUD_MXINS_H
