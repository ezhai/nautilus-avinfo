#!/bin/sh

branch="${1}"
builddir="$(pwd)"
retcode=0

# Log info
echo "Current Directory: $(pwd)"
echo "Current Home: $(echo ~)"
echo "Mock Version: $(rpm -q mock)"

# Checkout project
git clone https://github.com/ezhai/nautilus-avinfo.git
cd nautilus-avinfo
git checkout "${branch}"
if [[ $? -ne 0 ]]; then
    echo "Branch \"${branch}\" does not exist"
    exit 1
fi

# Build project
meson setup build
meson compile -C build
if [[ $? -ne 0 ]]; then
    echo "Failed to compile"
    exit 1
fi
meson compile -C build rpm-spec

version=$(meson introspect build --projectinfo | jq -r ".version")
pkgname="nautilus-avinfo-${version}"

# Create an archive from a clean source
git clone https://github.com/ezhai/nautilus-avinfo.git "${pkgname}/"
cd "${pkgname}/"
git checkout "${branch}"
rm -rf .github docs pkg
cd ..
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/" > /dev/null
rm -rf "${pkgname}/"

# Set up RPM build directory
rpmdev-setuptree
rpmbuilddir=~/rpmbuild
mv "${pkgname}.tar.gz" "${rpmbuilddir}/SOURCES/"
cp pkg/rpm/*.spec "${rpmbuilddir}/SPECS/"
cd "${rpmbuilddir}"

# Build Fedora
echo "Building for Fedora 42..."
rpmbuild -bs "SPECS/nautilus-avinfo.local.spec" --define "dist .fc42"
mock -r fedora-42-x86_64 "$(find SRPMS/ -regex ".*\.fc42\.src\.rpm")"
if [[ $? -ne 0 ]]; then
    retcode=1
fi

# Build Mageia
echo "Building for Mageia 9..."
rpmbuild -bs "SPECS/nautilus-avinfo.local.spec" --define "dist .mg9"
mock -r mageia9-x86_64 "$(find SRPMS/ -regex ".*\.mg9\.src\.rpm")"
if [[ $? -ne 0 ]]; then
    retcode=1
fi

# Build OpenSUSE
echo "Building for OpenSUSE..."
rpmbuild -bs "SPECS/nautilus-avinfo.local.spec" --define "dist .suse.tw"
mock -r opensuse-tumbleweed-x86_64 "$(find SRPMS/ -regex ".*\.suse\.tw\.src\.rpm")"
if [[ $? -ne 0 ]]; then
    retcode=1
fi

# Upload artifacts
if [[ "${retcode}" -ne 1 ]]; then
    mkdir -p /github/rpm/
    cp $(find SOURCES/ -regex ".*\.tar\.gz") /github/rpm
    cp $(find SPECS/ -regex ".*\.spec") /github/rpm
    cp $(find RPMS/ -regex ".*\.src\.rpm") /github/rpm
    cp $(find /var/lib/mock/*/result/ -regex ".*\.rpm") /github/rpm
else
    echo "Failed to build RPM, skipping artifact upload..."
    cp -r /var/lib/mock/fedora-42-x86_64/result /github/rpm/fedora-42
    cp -r /var/lib/mock/mageia9-x86_64/result /github/rpm/mageia9
    cp -r /var/lib/mock/opensuse-tumbleweed-x86_64/result /github/rpm/opensuse-tumbleweed
fi


exit $retcode
