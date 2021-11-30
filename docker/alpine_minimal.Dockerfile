FROM alpine:latest
LABEL maintainer="felix.eickeler@tum.de"

RUN apk add --no-cache wget libtbb libgomp libstdc++ libgcc
RUN adduser -D -g 'compressor' compressor
RUN mkdir /compressor

WORKDIR compressor
RUN wget https://filedn.com/lLjUrf9apNYzbv3LcEwzgcj/pointcloud_optimizer/cms_pcs_optimizer
RUN wget https://filedn.com/lLjUrf9apNYzbv3LcEwzgcj/pointcloud_optimizer/libcilantro.so
RUN wget https://filedn.com/lLjUrf9apNYzbv3LcEwzgcj/pointcloud_optimizer/default_config.json

RUN mv ./libcilantro.so /usr/lib/libcilantro.so
RUN chmod +x /compressor/cms_pcs_optimizer
ENTRYPOINT ["/compressor/cms_pcs_optimizer"]
