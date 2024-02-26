# Nautilus AVInfo

**IMPORTANT: This project is under development. Things may not work just yet, please wait warmly.**

A Nautilus (GNOME Files) extension to view audio/video information in the column and properties view. This exension is only compatible
with Nautilus 4+.
```
nautilus --version
```

## Installation

### RPM (Fedora, CentOS, RHEL, etc...)
A `.rpm` package for your OS and architecture may be available for download on the Releases page. Download the RPM package and install it.
```
(sudo) dnf install <path/to/nautilus-avinfo.rpm>
```

The SRPMs are also provided if your OS/arch is not supported or you wish to build the package yourself.

### Manual
Ensure you have `meson` and the appropriate development headers installed. Then build and install the project.
```
meson setup build
meson install -C build
```

## Development
This project uses the Meson build system.
```
meson set build             # setup build directory
meson compile -C build      # compile project
meson install -C build      # install binaries/translation files (requires sudo privileges)

meson compile -C build nautilus-avinfo-pot              # gather strings needed for translations
meson compile -C build nautilus-avinfo-upgrade-pot      # update strings in the translation files
```

The following resources may be helpful if you want to work on Nautilus extensions in C.
- https://docs.gtk.org/gobject/
- https://gnome.pages.gitlab.gnome.org/nautilus/
- https://stackoverflow.com/questions/9675349/how-to-create-nautilus-c-extensions
