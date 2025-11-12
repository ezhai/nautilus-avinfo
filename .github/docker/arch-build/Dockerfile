FROM archlinux:base-20251019.0.436919

COPY entrypoint.sh /entrypoint.sh

RUN pacman -Syu --noconfirm base-devel git
RUN useradd -ms /bin/bash rei
RUN usermod -aG wheel rei
RUN echo "%wheel ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

USER rei

WORKDIR /home/rei
ENTRYPOINT ["/entrypoint.sh"]
