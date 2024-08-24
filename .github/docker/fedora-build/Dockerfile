FROM fedora:40

RUN dnf install -y https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm
RUN dnf install -y git gcc meson nautilus-devel ffmpeg-free-devel gettext rpm-build rpmdevtools mock mock-rpmfusion-free wget jq

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
