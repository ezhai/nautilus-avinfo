#!/bin/sh

branch=${1}

# Get build info
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

version=$(meson introspect build --projectinfo | jq -r ".version")
pkgname="nautilus-avinfo-${version}"

# Set up rpmbuild
git clone https://github.com/ezhai/nautilus-avinfo.git "${pkgname}/"
cd "${pkgname}/"
git checkout "${branch}"
cd ..
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"

rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/
cat pkg/rpm/nautilus-avinfo.spec.template | VERSION=${version} envsubst > ~/rpmbuild/SPECS/nautilus-avinfo.spec

# Build RPM
cd ~/rpmbuild
rpmbuild -bs "SPECS/nautilus-avinfo.spec"
mock -r centos-stream-10-aarch64 --install "https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm"
mock -r centos-stream-10-aarch64 --install "https://mirrors.rpmfusion.org/free/el/rpmfusion-free-release-9.noarch.rpm"
mock -r centos-stream-10-aarch64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"

mock -r centos-stream-10-rpmfusion-x86_64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"

mkdir -p /github/rpm/
cp $(find ~/rpmbuild/SPECS/ -regex ".*\.spec") /github/rpm
cp $(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm") /github/rpm
cp $(find /var/lib/mock/centos-stream-10-aarch64/result/ -regex ".*\.rpm") /github/rpm
cp $(find /var/lib/mock/centos-stream-10-x86_64/result/ -regex ".*\.rpm") /github/rpm
