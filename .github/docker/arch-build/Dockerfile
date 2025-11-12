FROM archlinux:base-20251019.0.436919

COPY entrypoint.sh /entrypoint.sh

RUN pacman -Syu --noconfirm base-devel git

ENTRYPOINT ["/entrypoint.sh"]
