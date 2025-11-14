#!/bin/sh

branch="${1}"
builddir="$(pwd)"
retcode=0

# Log info
echo "Current Directory: $(pwd)"
echo "Current Home: $(echo ~)"
echo "Current User: $(whoami)"

# Build project
cd github
meson setup build
meson compile -C build
if [[ $? -ne 0 ]]; then
    echo "Failed to compile"
    exit 1
fi
meson compile -C build
if [[ $? -ne 0 ]]; then
    retcode=1
fi

# Echo version to GitHub Actions
version=$(meson introspect build --projectinfo | jq -r ".version")
echo "version=${version}" >> "${GITHUB_OUTPUT}"

# Upload artifacts
if [[ "${retcode}" -ne 1 ]]; then
    echo "Build successful!"
else
    echo "Build failed! :("
fi

exit $retcode
