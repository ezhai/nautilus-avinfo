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

git clone https://github.com/ezhai/nautilus-avinfo.git "${pkgname}/"
cd "${pkgname}/"
git checkout main-dummy
cd ..
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"

rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/
cat unix/rpm/nautilus-avinfo.spec.template | VERSION=${version} envsubst > ~/rpmbuild/SPECS/nautilus-avinfo.spec

cd ~/rpmbuild
rpmbuild -bs "SPECS/nautilus-avinfo.spec"
cd /github/workspace

mkdir /github/workspace/srpm
cp $(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm") /github/workspace/srpm/
