#!/bin/sh

branch=${1}

git clone https://github.com/ezhai/nautilus-avinfo.git
cd nautilus-avinfo
git checkout "${branch}"
if [[ $? -ne 0 ]]; then
    echo "Branch \"${branch}\" does not exist"
    exit 1
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
git checkout "${branch}"
cd ..
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"

rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/
cat pkg/rpm/nautilus-avinfo.spec.template | VERSION=${version} envsubst > ~/rpmbuild/SPECS/nautilus-avinfo.spec

cd ~/rpmbuild
rpmbuild -bs "SPECS/nautilus-avinfo.spec"
mock -r fedora-39-aarch64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"
mock -r fedora-39-x86_64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"

mkdir -p /github/rpm/
cp $(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm") /github/rpm
cp $(find /var/lib/mock/fedora-39-aarch64/result/ -regex ".*\.rpm") /github/rpm
cp $(find /var/lib/mock/fedora-39-x86_64/result/ -regex ".*\.rpm") /github/rpm
