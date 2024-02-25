Name:           nautilus-avinfo
Version:        0.1
Release:        1%{?dist}
Summary:        Nautilus extension for audio and video info

License:        GPLv3
URL:            https://github.com/ezhai/nautilus-avinfo
Source0:        %{name}-%{version}.tar.gz

Requires:       nautilus ffmpeg

%description
A Nautilus extension for viewing audio and video information.

%prep
%setup -q

%build

%install
mkdir -p %{buildroot}/%{_libdir}/nautilus-extension/extensions-4/
install -m 0644 nautilus-avinfo.so  %{buildroot}/%{_libdir}/nautilus-extension/extensions-4/

%files
%{_libdir}/nautilus-extension/extensions-4/nautilus-avinfo.so

%changelog
* Sat Feb 24 2024 Eric Zhai <ezhai.dev@gmail.com> - 0.1-1
- Intial RPM package
