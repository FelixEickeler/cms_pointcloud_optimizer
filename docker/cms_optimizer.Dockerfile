FROM debian:bookworm-slim
# FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive
ENV LANG C.UTF-8
ENV TERM xterm-256color

RUN apt-get clean && apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libeigen3-dev \
    tar \
    python3 gosu sudo openssh-server gdb \
    htop nano python3-distutils libpython3-dev python3-pip libtbb-dev rsync valgrind bash-completion

 #compile cgal
WORKDIR /home/simpl/
RUN pip3 install numpy scikit-learn scipy

# create user, ids are temporary
ARG USER_ID=1000
RUN useradd -m --no-log-init simpl && yes theoldone | passwd simpl
RUN usermod -aG sudo simpl
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
RUN chown -R simpl:simpl /home/simpl

# Add helios-entrypoint, for user-managment (gosu e.t.c)
COPY cms_optimizer-entrypoint.sh /usr/local/bin/cms_optimizer-entrypoint.sh
COPY init.sh /usr/local/bin/init.sh
RUN chmod +x /usr/local/bin/cms_optimizer-entrypoint.sh
RUN chmod +x /usr/local/bin/init.sh
CMD ["/usr/local/bin/init.sh"]
ENTRYPOINT ["/usr/local/bin/cms_optimizer-entrypoint.sh"]
