FROM alpine:latest
LABEL maintainer="felix.eickeler@tum.de"

RUN apk add --no-cache wget libtbb libgomp libstdc++ libgcc
RUN adduser -D -g 'compressor' compressor
RUN mkdir /compressor

WORKDIR compressor

# For testing local build, move the alpine folder to the docker folder, comment remote, uncomment local

# REMOTE
#RUN wget https://filedn.com/lLjUrf9apNYzbv3LcEwzgcj/pointcloud_optimizer/cms_pcs_optimizer
#RUN wget https://filedn.com/lLjUrf9apNYzbv3LcEwzgcj/pointcloud_optimizer/libcilantro.so
#RUN wget https://filedn.com/lLjUrf9apNYzbv3LcEwzgcj/pointcloud_optimizer/default_config.json

# LOCAL

COPY ./alpine/cms_pcs_optimizer ./cms_pcs_optimizer
COPY ./alpine/default_config.json ./default_config.json
COPY ./alpine/libcilantro.so ./libcilantro.so

RUN mv ./libcilantro.so /usr/lib/libcilantro.so
RUN chmod +x /compressor/cms_pcs_optimizer
ENTRYPOINT ["/compressor/cms_pcs_optimizer"]
