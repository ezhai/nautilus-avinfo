FROM fedora:42

ARG GITHUB_OUTPUT

ENV GITHUB_OUTPUT=${GITHUB_OUTPUT}

RUN dnf install -y git gcc meson gettext python python-jinja2
RUN dnf install -y nautilus-devel ffmpeg-free-devel gnome-desktop4-devel
RUN dnf install -y rpm-build rpmdevtools mock wget jq

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
