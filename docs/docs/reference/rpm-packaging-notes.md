# RPM Packaging Notes

## Distro Support

- **OpenSUSE ≤ 15.6 is not supported.**
  - The package repository contains an older version of `libavcodec` (4.4.6). It's possible to build for it, but the distro EOLs in April 2026.
- **Mageia ≤ 8, EPEL ≤ 9 and OpenEuler ≤ 22.03 are not supported.**
  - The package repositories are missing `pkgconfig(libnautilus-extension-4)`.
- **CentOS Stream, RHEL, Fedora ELN, Amazon Linux are not supported.**
  - The default package repositories do not contain the following FFmpeg libraries: `pkgconfig(libavcodec)`, `pkgconfig(libavformat)`, and `pkgconfig(libavutil)`. These packages are available in RPMFusion, but Copr does not allow the use of RPMFusion due to licensing conflicts.

For info on package support across distros, see [www.rpmfind.net](https://www.rpmfind.net/).

## Compatibility Notes

- `libnautilus-extension-4` ≥ 43. Older versions of Nautilus use an older, incompatible version of the extension library.
- `ffmpeg` ≥ 6.1.1. The issue is that the `ch_layout` field in the `AVCodecParameters` struct was introduced in `ffmpeg` 6.0 to replace the deprecated `channel_layout` field. It's possible to build the extension to support older versions of `ffmpeg`, but this requirement should be based on user demand (which there is currently none).
