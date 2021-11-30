FROM alpine:latest
LABEL maintainer="felix.eickeler@tum.de"

RUN apk add --no-cache eigen-dev libtbb-dev build-base cmake
RUN adduser -D -g 'compressor' compressor
RUN mkdir /compressor

WORKDIR compressor

COPY cms_optimizer-entrypoint.sh /usr/local/bin/cms_optimizer-entrypoint.sh
RUN chmod +x /usr/local/bin/init.sh
ENTRYPOINT ["/usr/local/bin/cms_optimizer-entrypoint.sh"]
