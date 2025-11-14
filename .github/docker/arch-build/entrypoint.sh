#!/bin/sh

branch="${1}"
builddir="$(pwd)"
retcode=0

# Log info
echo "Current Directory: $(pwd)"
echo "Current Home: $(echo ~)"
echo "Current User: $(whoami)"

# Clone project
git clone https://github.com/ezhai/nautilus-avinfo.git
cd nautilus-avinfo
git checkout "${branch}"
if [[ $? -ne 0 ]]; then
    echo "Branch \"${branch}\" does not exist"
    exit 1
fi
pkgname="nautilus-avinfo"

# Create an archive from a clean source
git clone https://github.com/ezhai/nautilus-avinfo.git "${pkgname}/"
cd "${pkgname}/"
git checkout "${branch}"
rm -rf .github docs pkg
cd ..
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/" > /dev/null
rm -rf "${pkgname}/"

# Set up build directory
mv "${pkgname}.tar.gz" "pkg/aur/"
mv pkg/aur/PKGBUILD.local pkg/aur/PKGBUILD
rm pkg/aur/PKGBUILD.jinja

# Build
cd pkg/aur
makepkg -si --noconfirm
if [[ $? -ne 0 ]]; then
    retcode=1
fi

# Upload artifacts
if [[ "${retcode}" -ne 1 ]]; then
    echo "Build successful, uploading artifacts..."
    sudo mkdir -p /github/artifacts/
    sudo cp $(find . -regex ".*\.tar\.zst") /github/artifacts
    sudo cp $(find . -regex ".*\.pkg.tar\.zst") /github/artifacts
else
    echo "Failed to build package, exiting..."
fi

exit $retcode
