# Nautilus AVInfo

**IMPORTANT: This project is under development. Things may not work just yet, please wait warmly.**

A Nautilus (GNOME Files) extension to view audio/video information in the column and properties view. This exension is only compatible
with Nautilus 4+.
```
nautilus --version
```

## Installation

### RPM (Fedora, CentOS, RHEL, etc..)
A `.rpm` package for your OS and architecture may be available for download on the Releases page. Download the package and install it.
```
(sudo) dnf install <path/to/nautilus-avinfo.rpm>
```

### Manual
Ensure you have `meson` and the appropriate development headers installed. Then build and install the project.
```
meson setup build
meson install -C build
```

## Development
The following resources may be helpful if you want to work on Nautilus extensions in C.
- https://docs.gtk.org/gobject/
- https://ffmpeg.org//doxygen/trunk/index.html
- https://gnome.pages.gitlab.gnome.org/nautilus/
- https://stackoverflow.com/questions/9675349/how-to-create-nautilus-c-extensions
