#!/bin/sh
cd /compressor && mkdir -p _build
cd _build && Eigen3_DIR=/usr/share/cmake/Modules cmake ../mount && make -j$(nproc)
cp ./bin/cms_pcs_optimizer /compressor/bin/alpine
