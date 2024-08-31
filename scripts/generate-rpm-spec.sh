#!/bin/bash

version="$1"
specdir="$2"

ffmpeg_dev_fedora="(ffmpeg-devel or ffmpeg-free-devel)"
ffmpeg_dev_opensuse="ffmpeg-5-libavcodec-devel ffmpeg-5-libavformat-devel ffmpeg-5-libavutil-devel"

cat "${specdir}/nautilus-avinfo.spec.template" | VERSION=${version} FFMPEG_DEV=${ffmpeg_dev_fedora} envsubst > "${specdir}/nautilus-avinfo.fedora.spec"
cat "${specdir}/nautilus-avinfo.spec.template" | VERSION=${version} FFMPEG_DEV=${ffmpeg_dev_opensuse} envsubst > "${specdir}/nautilus-avinfo.opensuse.spec"
