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

version=$(meson introspect build --projectinfo | jq -r ".version")
pkgname="nautilus-avinfo-${version}"

git clone https://github.com/ezhai/nautilus-avinfo.git "${pkgname}/"
cd "${pkgname}/"
git checkout "${branch}"
cd ..
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"

rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/
cat pkg/rpm/nautilus-avinfo.spec.template | VERSION=${version} envsubst > ~/rpmbuild/SPECS/nautilus-avinfo.spec

sudo yum localinstall --nogpgcheck https://download1.rpmfusion.org/free/el/rpmfusion-free-release-7.noarch.rpm

cd ~/rpmbuild
rpmbuild -bs "SPECS/nautilus-avinfo.spec"
mock -r centos-stream-10-aarch64 -a "http://download1.rpmfusion.org/free/el/rpmfusion-free-release-7.aarch64/" "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"
mock -r centos-stream-10-x86_64 -a "http://download1.rpmfusion.org/free/el/rpmfusion-free-release-7.x86_64/" "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"

mkdir -p /github/rpm/
cp $(find ~/rpmbuild/SPECS/ -regex ".*\.spec") /github/rpm
cp $(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm") /github/rpm
cp $(find /var/lib/mock/centos-stream-10-aarch64/result/ -regex ".*\.rpm") /github/rpm
cp $(find /var/lib/mock/centos-stream-10-x86_64/result/ -regex ".*\.rpm") /github/rpm
