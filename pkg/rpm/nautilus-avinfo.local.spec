%define         debug_package %{nil}

Name:           nautilus-avinfo
Version:        0.1.1
Release:        2%{?dist}
Summary:        Nautilus extension for audio and video info

License:        GPLv3+
URL:            https://github.com/ezhai/nautilus-avinfo
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc meson gettext python python-jinja2
BuildRequires:  pkgconfig(libnautilus-extension-4)
BuildRequires:  pkgconfig(libavcodec) pkgconfig(libavformat) pkgconfig(libavutil)
Requires:       nautilus >= 43 (ffmpeg or ffmpeg-free)

%description
A Nautilus extension for viewing audio and video information.

%prep
%autosetup -n %{name}-%{version}

%build
meson setup build
meson compile -C build

%install
./scripts/install.sh "%{buildroot}/%{_libdir}" "%{buildroot}/%{_datadir}/locale"

%files
%{_libdir}/nautilus/extensions-4/nautilus-avinfo.so
%{_datadir}/locale/*/LC_MESSAGES/nautilus-avinfo.mo

%changelog
* Fri Aug 30 2024 Eric Zhai <ezhai.dev@gmail.com> - 0.1.1-2
- install translation files

* Fri Aug 23 2024 Eric Zhai <ezhai.dev@gmail.com> - 0.1.1-1
- fix segfault when ffmpeg fails to read media file info

* Mon Feb 26 2024 Eric Zhai <ezhai.dev@gmail.com> - 0.1.0-1
- initial RPM package
