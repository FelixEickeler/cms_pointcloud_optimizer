FROM alpine:latest

LABEL maintainer="felix.eickeler@tum.de"
# RUN apk add nginx
RUN apk add --no-cache wget
RUN adduser -D -g 'compressor' compressor
RUN mkdir /compressor

# RUN chown -R www:www /www

# COPY index.html /www

# RUN ["./usr/sbin/nginx"]

# CMD ["/usr/local/bin/init.sh"]
# ENTRYPOINT ["/usr/local/bin/cms_optimizer-entrypoint.sh"]
