#!/bin/sh -l

repository=${1}
version=${2}

mkdir -p /github/workspace
pwd

git clone "${repository}"
cd nautilus-avinfo
git checkout main-dummy

if [[ $? -ne 0 ]]; then
    echo "Failed to checkout repository"
    exit 1
fi

meson setup build
meson compile -C build
if [[ $? -ne 0 ]]; then
    echo "Failed to compile"
    exit 1
fi
cp build/nautilus-avinfo.so /github/workspace/

pkgname="nautilus-avinfo-${version}"
mkdir "${pkgname}"
cp build/nautilus-avinfo.so "${pkgname}/"
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"

rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/
cp unix/fedora/nautilus-avinfo.spec ~/rpmbuild/SPECS/
cd ~/rpmbuild

rpmbuild -bb "SPECS/nautilus-avinfo.spec"
cp RPMS/* /github/workspace/


ls -la /github/workspace
