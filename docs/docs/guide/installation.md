# Installation

This extension is only compatible with Nautilus 43+.

```
nautilus --version
```

If a package for your OS is not available, you will need to install the extension using the [manual installation](#manual) instructions.

## RPM (Fedora, Mageia, OpenSUSE)

The extension can be downloaded through the COPR project repo.

```
dnf copr enable grimbus/nautilus-avinfo
dnf install nautilus-avinfo
```

Alternatively, some pre-packaged RPM files are provided on the [Releases](https://github.com/ezhai/nautilus-avinfo/releases) page.

```
dnf install <path/to/nautilus-avinfo.rpm>
```

The source RPM and RPM spec are also provided on the [Releases](https://github.com/ezhai/nautilus-avinfo/releases) page
if you wish to build the package yourself or your OS is not supported.

## Manual

Install the `meson` build system and the appropriate development headers. Then build and install the project.

```
meson setup build
meson install -C build
```

To uninstall the extension, run the uninstall script.

```
meson compile -C build uninstall-nautilus-avinfo
```

## Notes

The following files are installed by the extension.

- `$(libdir)/nautilus/extensions-4/nautilus-avinfo.so`
- `$(localedir)/*/LC_MESSAGES/nautilus-avinfo.mo`

The location of `libdir` and `localedir` may vary. See the table below for examples.

| Distro       | `libdir`     | `localedir`         |
| ------------ | ------------ | ------------------- |
| Arch Linux   | `/usr/lib`   | `/usr/share/locale` |
| Fedora (x64) | `/usr/lib64` | `/usr/share/locale` |
