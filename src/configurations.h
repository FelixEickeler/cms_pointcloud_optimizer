//
// Created by felix on 15.11.2021.
//

#ifndef CMS_PCS_OPTIMIZER_CONFIGURATIONS_H
#define CMS_PCS_OPTIMIZER_CONFIGURATIONS_H


namespace cms_opti::configurations {

    struct NonApproachRelatedConfig {
        std::string comment{};
        std::string version{};

        std::vector <std::string> evaluate_on_property = {"Coordinates", "Color", "Intensity"};
    };

    struct CompressionParameterSet {};

    struct AdaptiveBilateralParameters : CompressionParameterSet {
        float c = 0;
        float c_thr = 0;
        float c_1 = 0;
        float c_2 = 0;
        float c_3 = 0;
        float c_4 = 0;
        float sig_d = 0;
        float sig_n = 0;
        float sig_rgb = 0;
        float sig_i = 0;
        int point_factor = 20;
    };
}






#endif //CMS_PCS_OPTIMIZER_CONFIGURATIONS_H
