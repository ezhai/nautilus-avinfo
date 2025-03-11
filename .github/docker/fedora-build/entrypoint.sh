#!/bin/sh

branch="${1}"
builddir="$(pwd)"
retcode=0

# Log info
echo -n "Current Directory: "
echo "$(pwd)"
echo -n "Current Home:      "
echo ~

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
rpmbuilddir=~/rpmbuild

rpmdev-setuptree
mv "${pkgname}.tar.gz" "${rpmbuilddir}/SOURCES/"
cp pkg/rpm/*.spec "${rpmbuilddir}/SPECS/"
cd "${rpmbuilddir}"

# Build Fedora
echo "Building for Fedora 41..."
rpmbuild -bs "SPECS/nautilus-avinfo.local.spec" --define "dist .fc41"
mock -r fedora-41-x86_64 "$(find SRPMS/ -regex ".*\.fc41\.src\.rpm")"
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
fi

exit $retcode
