//
// Created by felix on 18.11.2021.
//
#pragma once
#include <algorithm>

template<class Derived>
inline void PointCloudBase<Derived>::resize(std::ptrdiff_t rows, std::ptrdiff_t cols) {
    throw std::runtime_error("The resize has not been implemented, in the derived class !");
}

template<class Derived>
inline void PointCloudBase<Derived>::set_right_cols(std::ptrdiff_t cols, const Derived &cloud) {
    throw std::runtime_error("The resize has not been implemented, in the derived class !");
}

template<class Derived>
inline void PointCloudBase<Derived>::swap_point(std::ptrdiff_t dst, std::ptrdiff_t src) {
    throw std::runtime_error("The resize has not been implemented, in the derived class !");
}

template<class Derived>
inline void PointCloudBase<Derived>::clear() {
    static_cast<Derived *>(this)->resize(0);
}

template<class Derived>
inline Derived &PointCloudBase<Derived>::append(const Derived &cloud) {
    unsigned long original_size = size();
    static_cast<Derived *>(this)->resize(original_size + cloud.size());
    static_cast<Derived *>(this)->set_right_cols(cloud.points.cols(), cloud);
    return *static_cast<Derived*>(this);
}

template<class Derived>
template<typename IndexT>
inline Derived &PointCloudBase<Derived>::remove(const std::vector<IndexT> &indices) {
    if (indices.empty()) return *static_cast<Derived*>(this);
    if (indices.size() >= size()) {
        clear();
        return *static_cast<Derived*>(this);
    }

    auto new_end = static_cast<Derived*>(this)->size() - 1;
    std::vector<IndexT> sorted_indices(indices.size());
    std::partial_sort_copy(indices.begin(), indices.end(), sorted_indices.begin(), sorted_indices.end(), std::greater<IndexT>());
    for (auto &index: sorted_indices) {
        static_cast<Derived *>(this)->swap_point(index, new_end--);
    }
    const unsigned long new_size = new_end + 1;
    static_cast<Derived *>(this)->resize(new_size);
    return *static_cast<Derived *>(this);
}

template<class Derived>
inline unsigned long PointCloudBase<Derived>::size() const { return static_cast<const Derived *>(this)->points.cols(); }

template<class Derived>
inline bool PointCloudBase<Derived>::isEmpty() const { return  static_cast<const Derived *>(this)->points.cols() == 0; }

