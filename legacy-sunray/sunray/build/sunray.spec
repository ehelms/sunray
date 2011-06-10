Summary: A simulation of solar radiative exposure across a three dimensional form.
Name: sunray
Version: %{version}
Release: 1
License: GPLv3
Group: Applications/Engineering
Source: http://josiah.homelinux.org/pub/%{name}/%{version}/%{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description
A simulation of solar radiative exposure across a three dimensional form.

%prep
%setup -q

%build
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

%changelog
* Fri Jul 13 2007 Josiah Gore <josiahgore@gmail.com>
- Initial Sunray .spec file
