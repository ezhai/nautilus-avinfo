# Nautilus AVInfo

A Nautilus (GNOME Files) extension to view audio/video information in the column and properties view. This extension is
only compatible with Nautilus 43+.
```
nautilus --version
```

This extension makes use of FFmpeg's `libavformat` library, so the information of the properties page is similar to the
output of`ffprobe`.


## Installation

### RPM (Fedora, CentOS, RHEL, etc...)
A `.rpm` package for your OS and architecture may be available for download on the Releases page. Download the appropriate
RPM package and install it.
```
(sudo) dnf install <path/to/nautilus-avinfo.rpm>
```

The SRPMs are also provided if your OS/arch is not supported or you wish to build the package yourself. Note that not all
distros support the latest version of Nautilus, so the extension may not be supported on your distro just yet.


### Manual
Ensure you have `meson` and the appropriate development headers installed. Then build and install the project.
```
meson setup build
meson install -C build
```

## Contributions
If you encounter any issues, have suggestions, or would like to make a contribution (code, translations, etc...), please
open an issue or submit a PR.


## Development
This project uses the Meson build system.
```
meson set build             # setup build directory
meson compile -C build      # compile project
meson install -C build      # install binaries/translation files (requires sudo privileges)

meson compile -C build nautilus-avinfo-pot              # gather strings needed for translations
meson compile -C build nautilus-avinfo-upgrade-pot      # update strings in the translation files
```

## Additional Notes
The following resources may be of interest if you want to work on Nautilus extensions in C.
- https://docs.gtk.org/gobject/
- https://gnome.pages.gitlab.gnome.org/nautilus/
- https://stackoverflow.com/questions/9675349/how-to-create-nautilus-c-extensions
