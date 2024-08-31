FROM fedora:40

RUN dnf install -y git gcc meson gettext python-jinja2
RUN dnf install -y nautilus-devel ffmpeg-free-devel
RUN dnf install -y rpm-build rpmdevtools mock wget jq

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
