#!/bin/bash

libdir="$1"
localedir="$2"

if [[ $(id -u) -ne 0 ]]; then
    echo "[ERROR] Uninstalling nautilus-avinfo requires superuser priviledges."
    exit 1
fi

echo "Removing nautilus-avinfo.so from ${localedir}"
find ${localedir} -name "nautilus-avinfo.mo" -print0 | xargs -r0 echo
find ${localedir} -name "nautilus-avinfo.mo" -print0 | xargs -r0 rm

echo "Removing nautilus-avinfo.mo from ${libdir}"
find ${libdir} -name "nautilus-avinfo.so" -print0 | xargs -r0 echo
find ${libdir} -name "nautilus-avinfo.so" -print0 | xargs -r0 rm
