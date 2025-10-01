# Release

## Version Bump

If a version bump is required, follow the following instructions.

1.  Set the updated version and release number in `meson.build`.
2.  Run `meson compile -C build rpm-spec` to update the RPM specs.
3.  Run `meson compile -C build pkgbuild` to update the PKGBUILD.

::: warning
The `sha256sum` for the `PKGBUILD` cannot be updated since the tarball for the latest version isn't uploaded until the MR making the change is merged in. It is advised to set the hash to `SKIP` and update the hash to the proper value in a subsequent MR.
:::
