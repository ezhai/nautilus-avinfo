FROM fedora:40

RUN dnf install -y git gcc meson nautilus-devel ffmpeg-free-devel gettext
RUN dnf install -y rpm-build rpmdevtools mock wget jq

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
