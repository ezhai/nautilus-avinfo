# Release

## Version Bump

If a version bump is required, follow the following instructions.

1.  Set the updated version and release number in `meson.build`.
2.  Run `meson compile -C build rpm-spec` to update the RPM specs.
3.  Run `meson compile -C build pkgbuild` to update the PKGBUILD.

::: warning
The `sha256sum` for the `PKGBUILD` cannot be updated in the same MR as the version bump since the updated tarball needs to be uploaded first. Currently, the recommended workaround is to set the hash to `SKIP` and update the hash in the `PKGBUILD` for the release version.
:::
