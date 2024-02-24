#!/bin/sh -l

repository=${1}

cd /github/workspace
git clone "${repository}"
cd nautilus-avinfo

meson setup build
meson compile -C build

ls build -l
echo "Compilation complete"
