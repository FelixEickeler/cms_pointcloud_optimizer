FROM alpine:latest
LABEL maintainer="felix.eickeler@tum.de"

RUN apk add --no-cache wget
RUN adduser -D -g 'compressor' compressor
RUN mkdir /compressor

WORKDIR compressor
#RUN wget
ENTRYPOINT ["/usr/local/bin/cms_optimizer-entrypoint.sh"]
