FROM alpine:latest
LABEL maintainer="felix.eickeler@tum.de"

RUN apk add --no-cache eigen-dev libtbb-dev build-base cmake
RUN adduser -D -g 'compressor' compressor
RUN mkdir /compressor

WORKDIR compressor

COPY builder_alpine.sh /usr/local/bin/builder_alpine.sh
RUN chmod +x /usr/local/bin/builder_alpine.sh
ENTRYPOINT ["/usr/local/bin/builder_alpine.sh"]
