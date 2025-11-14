#!/bin/sh

branch="${1}"
version="${2}"
retcode=0

# Log info
echo "Current Directory: $(pwd)"
echo "Current Home: $(echo ~)"
echo "Current User: $(whoami)"
echo "Mock Version: $(rpm -q mock)"

# Clone project
git clone https://github.com/ezhai/nautilus-avinfo.git
cd nautilus-avinfo
git checkout "${branch}"
if [[ $? -ne 0 ]]; then
    echo "Branch \"${branch}\" does not exist"
    exit 1
fi

# Create an archive from a clean source
pkgname="nautilus-avinfo-${version}"
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
mock -r mageia-9-x86_64 "$(find SRPMS/ -regex ".*\.mg9\.src\.rpm")"
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
    echo "Build successful, uploading artifacts..."
    mkdir -p /github/artifacts/
    cp $(find SOURCES/ -regex ".*\.tar\.gz") /github/artifacts
    cp $(find SPECS/ -regex ".*\.spec") /github/artifacts
    cp $(find RPMS/ -regex ".*\.src\.rpm") /github/artifacts
    cp $(find /var/lib/mock/*/result/ -regex ".*\.rpm") /github/artifacts
else
    echo "Failed to build RPM, uploading logs..."
    mkdir -p /github/artifacts/
    cp -r /var/lib/mock/fedora-42-x86_64/result /github/artifacts/fedora-42
    cp -r /var/lib/mock/mageia-9-x86_64/result /github/artifacts/mageia-9
    cp -r /var/lib/mock/opensuse-tumbleweed-x86_64/result /github/artifacts/opensuse-tumbleweed
fi

exit $retcode
