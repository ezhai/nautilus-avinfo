# Nautilus AVInfo

A Nautilus (GNOME Files) extension to view audio/video information in the column and properties view. This extension is
only compatible with Nautilus versions 43+.
```
nautilus --version
```

This extension makes use of FFmpeg's `libavformat` library to provide information about media files.


## Installation

### RPM (Fedora, CentOS, RHEL, etc...)
A `.rpm` package for your OS and architecture may be available for download on the Releases page. Download the appropriate
RPM package and install it.
```
dnf install <path/to/nautilus-avinfo.rpm>
```

The SRPMs are also provided if your OS/arch is not supported or you wish to build the package yourself. Note that not all
distros have Nautilus 43 or higher available, so the extension may not be supported on your distro as of now.


### Manual
Ensure you have `meson` and the appropriate development headers installed. Then build and install the project.
```
meson setup build
meson install -C build
```

To uninstall, remove `nautilus-avinfo.so` from `$(libdir)/nautilus/extensions-4/`. The location of `libdir` varies by distro.
For example, on Arch Linux it is `/usr/lib`. 

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

Some other helpful commands.
```
nautilus -q                 # terminate existing nautilus processes
nautilus                    # start nautilus from the CLI
```

## Additional Notes
The following resources may be of interest if you want to work on Nautilus extensions in C.
- https://docs.gtk.org/gobject/
- https://gnome.pages.gitlab.gnome.org/nautilus/
- https://stackoverflow.com/questions/9675349/how-to-create-nautilus-c-extensions
