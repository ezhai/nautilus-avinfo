FROM archlinux:base

RUN pacman -Syu --noconfirm base-devel git sudo
RUN useradd -ms /bin/bash rei
RUN usermod -aG wheel rei
RUN echo "%wheel ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

USER rei

COPY entrypoint.sh /entrypoint.sh

WORKDIR /home/rei

ENTRYPOINT ["/entrypoint.sh"]
