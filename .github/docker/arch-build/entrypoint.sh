#!/bin/sh

branch="${1}"
builddir="$(pwd)"
retcode=0

# Log info
echo "Current Directory: $(pwd)"
echo "Current Home: $(echo ~)"

# Clone project
git clone https://github.com/ezhai/nautilus-avinfo.git
cd nautilus-avinfo
git checkout "${branch}"
if [[ $? -ne 0 ]]; then
    echo "Branch \"${branch}\" does not exist"
    exit 1
fi

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
