#!/bin/bash

libdir="$1"
localedir="$2"

# Check arguments
if [[ -z "${libdir}" ]]; then
    echo "libdir is undefined"
    exit 1
fi
if [[ -z "${localedir}" ]]; then
    echo "localedir is undefined"
    exit 1
fi

# Install nautilus-avinfo.so
mkdir -p "${libdir}/nautilus/extensions-4/"
cp build/nautilus-avinfo.so "${libdir}/nautilus/extensions-4/"

# Install nautilus-avinfo.mo
for file in $(find build/po -name "nautilus-avinfo.mo" | sed -e 's/build\/po\///'); do
    mkdir -p "${localedir}/${file%/*}"
    cp "build/po/${file}" "${localedir}/${file}"
done
