#!/bin/sh

git status > /dev/null 2>&1
if [[ $? -ne 0 ]]; then
    mkdir -p /github/workspace
    cd /github/workspace

    git init
    git remote add origin https://github.com/ezhai/nautilus-avinfo.git
    git fetch --all
    git checkout main-dummy
fi

meson setup build
meson compile -C build
if [[ $? -ne 0 ]]; then
    echo "Failed to compile"
    exit 1
fi

version=$(meson introspect  build --projectinfo | jq -r ".version")
pkgname="nautilus-avinfo-${version}"

mkdir "${pkgname}"
cp build/nautilus-avinfo.so "${pkgname}/"
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"

rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/
cp unix/fedora/nautilus-avinfo.spec ~/rpmbuild/SPECS/

cd ~/rpmbuild/SPECS
rpmbuild -bs "nautilus-avinfo.spec"
rpmbuild --rebuild "$(find . -regex ".*\.src\.rpm")"
cd /github/workspace

mkdir /github/workspace/rpm
cp $(find ~/rpmbuild/RPMS/ -regex ".*\.rpm") /github/workspace/rpm/
