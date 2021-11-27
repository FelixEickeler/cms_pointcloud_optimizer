//
// Created by felix on 17.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_POINTCLOUD_BASE_H
#define CMS_PCS_OPTIMIZER_POINTCLOUD_BASE_H
#include <Eigen/Dense>
#include "pointcloud_mxins.h"

namespace cilantro {
    template<class Derived>
    struct PointCloudBase{
        /// These need to be implemented, see existing classes as example
        void resize(Eigen::Index rows, Eigen::Index cols);
        void set_right_cols(Eigen::Index cols, const Derived &cloud);
        void swap_point(Eigen::Index dst, Eigen::Index src);

        /// Base definitions e.g. actual methods
        void clear();
        Derived& append(const Derived &cloud);
        [[nodiscard]] inline size_t size() const;
        [[nodiscard]] inline bool isEmpty() const;

        template<typename IndexT>
        Derived& remove(const std::vector<IndexT> &indices);
    };
    #include "../src/pointcloud_base.ipp"
}



#endif //CMS_PCS_OPTIMIZER_POINTCLOUD_BASE_H
