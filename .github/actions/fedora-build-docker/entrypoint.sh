#!/bin/sh -l

repository=${1}

cd /github/workspace
git clone "${repository}"
cd nautilus-avinfo
if [[ $? -ne 0 ]]; then
    echo "Failed to checkout repository"
    exit 1
fi

meson setup build
meson compile -C build

ls build -l
echo "Compilation complete"
