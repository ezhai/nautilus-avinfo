# User Guide


## Installation

### Prerequisites

This extension is only compatible with Nautilus 43+.
```
nautilus --version.
```

---

### RPM (Fedora)
Pre-packaged RPM files can be found on the [Releases](https://github.com/ezhai/nautilus-avinfo/releases) page.
```
dnf install <path/to/nautilus-avinfo.rpm>
```

A source RPM file is also provided if your OS and architecture is not supported or you wish to build the package yourself.
```
rpm -ivh <path/to/nautilus-avinfo.src.rpm>
```

*Note: Not all distros have Nautilus 43+ available in the package repositories.*

---

### Manual
Ensure you have `meson` and the appropriate development headers installed. Then build and install the project.
```
meson setup build
meson install -C build
```

To uninstall, run the uninstall script using `meson`.
```
meson compile -C build uninstall-nautilus-avinfo
```

---

### Notes

The following files are installed.
- `$(libdir)/nautilus/extensions-4/nautilus-avinfo.so`.
- `$(localedir)/*/LC_MESSAGES/nautilus-avinfo.mo`.

The location of `libdir` and `localedir` may vary by distro.
| Distro       | `libdir`     | `localedir`         |
| ------------ | ------------ | ------------------- |
| Arch Linux   | `/usr/lib`   | `/usr/share/locale` |
| Fedora (x64) | `/usr/lib64` | `/usr/share/locale` |
