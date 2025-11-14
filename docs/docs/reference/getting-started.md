# Development

## Setup

This project uses the Meson build system. To get started with development, install `meson` and the required headers in `meson.build`.

```
meson setup build                                       # setup build directory
meson compile -C build                                  # compile project
meson install -C build                                  # install binaries/translation files (requires sudo privileges)
meson compile -C build nautilus-avinfo-uninstall        # uninstall binaries/translation files (requires sudo privileges)

meson compile -C build nautilus-avinfo-pot              # gather strings needed for translations using xgettext
meson compile -C build nautilus-avinfo-update-po        # regenerate .po files from the .pot file
```

Some other helpful commands.

```
nautilus -q                 # terminate existing nautilus processes
nautilus                    # start nautilus from the CLI
```

## Required Packages

The exact packages needed for development differ based on distro. As an example, the required packages for Arch Linux are the following.

```
pacman -Syu meson nautilus ffmpeg gnome-desktop libnautilus-extension python python-jinja
```

## Resources

The following resources may be of interest if you want to work on Nautilus extensions.

- https://docs.gtk.org/gobject/
- https://gnome.pages.gitlab.gnome.org/nautilus/
- https://stackoverflow.com/questions/9675349/how-to-create-nautilus-c-extensions
