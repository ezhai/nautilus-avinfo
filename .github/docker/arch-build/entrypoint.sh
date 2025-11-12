#!/bin/sh

branch="${1}"
builddir="$(pwd)"
retcode=0

# Build project
cd pkg/aur
makepkg -si --noconfirm
if [[ $? -ne 0 ]]; then
    retcode=1
fi

# Upload artifacts
if [[ "${retcode}" -ne 1 ]]; then
    echo "Build successful, uploading artifacts..."
else
    echo "Failed to build package, exiting..."
fi

exit $retcode
