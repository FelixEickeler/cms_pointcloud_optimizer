#!/bin/bash
cd /compressor && mkdir -p _build
cd _build && Eigen3_DIR=/usr/share/cmake/Modules cmake -DCMAKE_BUILD_TYPE=Release ../mount && make -j$(nproc)

# copy results to output
cp ./bin/cms_pcs_optimizer /compressor/bin/alpine
cp ./bin/default_config.json /compressor/bin/alpine/default_config.json
cp ./modules/cilantro/libcilantro.so /compressor/bin/alpine/libcilantro.so
