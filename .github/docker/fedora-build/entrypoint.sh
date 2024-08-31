#!/bin/sh

branch=${1}

# Build project
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
meson compile -C build rpm-spec

version=$(meson introspect build --projectinfo | jq -r ".version")
pkgname="nautilus-avinfo-${version}"

# Set up a clean source
git clone https://github.com/ezhai/nautilus-avinfo.git "${pkgname}/"
cd "${pkgname}/"
git checkout "${branch}"
rm -rf .github docs pkg
cd ..
tar -cvzf "${pkgname}.tar.gz" "${pkgname}/"

# Set up RPM build directory
rpmdev-setuptree
mv "${pkgname}.tar.gz" ~/rpmbuild/SOURCES/

# Set up Fedora SRPM
cp pkg/rpm/nautilus-avinfo.fc.spec ~/rpmbuild/SPECS/nautilus-avinfo.spec
cd ~/rpmbuild
rpmbuild -bs "SPECS/nautilus-avinfo.spec"

# Build Fedora and CentOS RPM
# mock -r centos-stream+epel-9-aarch64 --install "https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm"
# mock -r centos-stream+epel-9-aarch64 --install "https://mirrors.rpmfusion.org/free/el/rpmfusion-free-release-9.noarch.rpm"
# mock -r centos-stream+epel-9-aarch64 --no-clean "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"
mock -r fedora-40-x86_64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"
cd ..

# Set up OpenSUSE SRPM
cp pkg/rpm/nautilus-avinfo.opensuse.spec ~/rpmbuild/SPECS/nautilus-avinfo.spec
cd ~/rpmbuild
rpmbuild -bs "SPECS/nautilus-avinfo.spec"

# Build OpenSUSE RPM
mock -r opensuse-tumbleweed-x86_64 "$(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm")"
cd ..

# Upload artifacts
mkdir -p /github/rpm/
cp $(find ~/rpmbuild/SPECS/ -regex ".*\.spec") /github/rpm
cp $(find ~/rpmbuild/SRPMS/ -regex ".*\.src\.rpm") /github/rpm
cp $(find /var/lib/mock/*/result/ -regex ".*\.rpm") /github/rpm
