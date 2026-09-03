Name:      taglib
Summary:   library for reading and editing the meta-data of several popular audio formats
Version:   1.13.1
Release:   1
Group:     System/Libraries
License:   LGPL-2.1 and BSL1.0
Source0:   %{name}-%{version}.tar.gz
Source1001:   %{name}.manifest
BuildRequires:  cmake
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
library for reading and editing the meta-data of several popular audio formats

%package devel
Summary:  A taglib  library (Development)
Group:    Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
library for reading and editing the meta-data of several popular audio formats

%prep
%setup -q
cp %{SOURCE1001} .

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER} -DVISIBILITY_HIDDEN=On

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%{_libdir}/libtag.so.*
%{_libdir}/libtag_c.so.*
%{_bindir}/taglib-config
%license COPYING.LGPL COPYING.BSL


%files devel
%{_includedir}/taglib/*.h
%{_libdir}/libtag.so
%{_libdir}/libtag_c.so
%{_includedir}/taglib/tlist.tcc
%{_includedir}/taglib/tmap.tcc
%{_libdir}/pkgconfig/taglib.pc
%{_libdir}/pkgconfig/taglib_c.pc
