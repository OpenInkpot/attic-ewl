%define _missing_doc_files_terminate_build 0

Summary: Enlightenment Widget Library
Name: ewl
Version: 0.0.4
Release: 0.%(date '+%Y%m%d')
License: BSD
Group: User Interface/X
URL: http://www.enlightenment.org/pages/ewl.html
Source: ftp://ftp.enlightenment.org/enlightenment/%{name}-%{version}.tar.gz
Packager: %{?_packager:%{_packager}}%{!?_packager:Michael Jennings <mej@eterm.org>}
Vendor: %{?_vendorinfo:%{_vendorinfo}}%{!?_vendorinfo:The Enlightenment Project (http://www.enlightenment.org/)}
Distribution: %{?_distribution:%{_distribution}}%{!?_distribution:%{_vendor}}
#BuildSuggests: xorg-x11-devel
BuildRequires: libjpeg-devel XFree86-devel
BuildRequires: embryo-devel eet-devel edb-devel ecore-devel
BuildRequires: evas-devel etox-devel edje-devel
Requires: edb ecore evas etox edje embryo eet
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
EWL is a widget library which uses the E Foundation Libraries (EFL).

%package devel
Summary: EWL headers and development libraries.
Group: Development/Libraries
Requires: %{name} = %{version}
Requires: libjpeg-devel XFree86-devel
Requires: embryo-devel eet-devel edb-devel ecore-devel
Requires: evas-devel etox-devel edje-devel

%description devel
EWL development files

%prep
%setup -q

%build
%{configure} --prefix=%{_prefix}
%{__make} %{?_smp_mflags} %{?mflags}

%install
%{__make} %{?mflags_install} DESTDIR=$RPM_BUILD_ROOT install
test -x `which doxygen` && sh gendoc || :

%post
/sbin/ldconfig || :

%postun
/sbin/ldconfig || :

%clean
test "x$RPM_BUILD_ROOT" != "x/" && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%doc AUTHORS COPYING* NEWS README TODO
%{_libdir}/libewl.so.*
%{_libdir}/libewl.la
%{_bindir}/ewl_edb_ed
%{_datadir}/ewl

%files devel
%defattr(-, root, root)
%doc doc/html
%{_datadir}/aclocal/ewl.m4
%{_libdir}/libewl.so
%{_libdir}/libewl.a
%{_libdir}/pkgconfig/ewl.pc
%{_includedir}/ewl/Ewl.h
%{_includedir}/ewl/ewl_*.h
%{_bindir}/ewl-config
%{_bindir}/ewl_test
%{_bindir}/ewl_embed_test
%{_bindir}/ewl_simple_test

%changelog
