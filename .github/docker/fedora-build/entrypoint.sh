#!/bin/sh

branch=${1}

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
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"
rm -rf "${pkgname}/"

# Set up RPM build directory
rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/

# Set up RPM specs
cp pkg/rpm/nautilus-avinfo.*.spec ~/rpmbuild/SPECS/
cd ~/rpmbuild

# Build Fedora
rpmbuild -bs "SPECS/nautilus-avinfo.spec" --define "dist .fc40"
mock -r fedora-40-x86_64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.fc40\.src\.rpm")"

# Build CentOS
# mock -r centos-stream+epel-9-aarch64 --install "https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm"
# mock -r centos-stream+epel-9-aarch64 --install "https://mirrors.rpmfusion.org/free/el/rpmfusion-free-release-9.noarch.rpm"
# mock -r centos-stream+epel-9-aarch64 --no-clean "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"

# Build OpenSUSE
rpmbuild -bs "SPECS/nautilus-avinfo.spec" --define "dist .suse.tw"
mock -r opensuse-tumbleweed-x86_64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.suse\.tw\.src\.rpm")"

# Upload artifacts
mkdir -p /github/rpm/
cp $(find ~/rpmbuild/SPECS/ -regex ".*\.spec") /github/rpm
cp $(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm") /github/rpm
cp $(find /var/lib/mock/*/result/ -regex ".*\.rpm") /github/rpm
