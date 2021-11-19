//
// Created by felix on 18.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_POINTCLOUD_MXINS_H
#define CMS_PCS_OPTIMIZER_POINTCLOUD_MXINS_H

struct IsPointCloud{
    virtual bool is_pointcloud() = 0;
};

struct HasNormals{
    virtual bool hasNormals() = 0;
};

struct HasColors{
    virtual bool hasColors() = 0;
};

struct HasScalarField{
    virtual bool hasScalarFields() = 0;
};

#endif //CMS_PCS_OPTIMIZER_POINTCLOUD_MXINS_H
