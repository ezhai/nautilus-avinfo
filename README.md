# Nautilus AVInfo

**IMPORTANT: This project is under development. Things may not work just yet, please wait warmly.**

A Nautilus (GNOME Files) extension to view audio/video information in the column and properties view. This exension is only compatible
with Nautilus 4+.
```
nautilus --version
```

## Installation
The extension is provided as a package for several package managers. If it is not available for your distro's package manager, it will need to be installed manually. 

### RPM (Fedora, CentOS, RHEL, etc..)
A `.rpm` package is available for download on the Releases page. Download the package and install it.
```
(sudo) rpm -i <path/to/nautilus-avinfo.rpm>
```

### Manual
Ensure you have `meson` and the appropriate development headers installed. Then build and install the project.
```
meson setup build
meson install -C build
```

To uninstall, remove the following files.
- `${libdir}/nautilus/extensions-4/nautilus-avinfo.so`
- `${localedir}/${lang}/LC_MESSAGES/nautilus-avinfo.mo`

### Manual (Psychotic)
Download `nautilus-avinfo.so` from the Releases page. Copy it to `${libdir}/nautilus/extensions-4`.


## Development
The following resources may be helpful if you want to work on Nautilus extensions in C.
- https://docs.gtk.org/gobject/
- https://ffmpeg.org//doxygen/trunk/index.html
- https://gnome.pages.gitlab.gnome.org/nautilus/
- https://stackoverflow.com/questions/9675349/how-to-create-nautilus-c-extensions
