Name:		flatbuffers
%global __provides_exclude_from ^%{_libdir}/libcxx-pkgconfig/.*$
VCS:   platform/upstream/flatbuffers#748aa035ca79aef902589656326a14d0b3ce56e6
Summary:	cross platform serialization library
Version:	24.3.25
Release:	1%{?dist}
Group:		Development/Libraries
Packager:	Yongjoo Ahn <yongjoo1.ahn@samsung.com>
License:	Apache-2.0
Source0:	%{name}-%{version}.tar.gz
Source1:	%{name}.manifest

BuildRequires:	cmake
BuildRequires:	sed
BuildRequires:  python3-devel
BuildRequires:  python3-setuptools
BuildRequires:  clang
BuildRequires:  libc++-devel
BuildRequires:  libc++abi-devel
BuildRequires:  patchelf

%description
FlatBuffers is a cross platform serialization library architected for maximum
memory efficiency. It allows you to directly access serialized data without
parsing/unpacking it first, while still having great forwards/backwards
compatibility.

%package devel
Summary:	Development package to use flatbuffers
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}
%description devel
This package provides headers and other miscellaneous files required to use flatbuffers.

%package libcxx
Summary:	libc++ build of flatbuffers
Group:		Development/Libraries
%description libcxx
Flatbuffers shared library built with LLVM libc++, with an isolated SONAME.

%package libcxx-devel
Summary:	Development files for the libc++ flatbuffers build
Group:		Development/Libraries
Requires:	%{name}-libcxx = %{version}-%{release}
Requires:	libc++-devel
Provides:	pkgconfig(flatbuffers-libcxx) = %{version}
%description libcxx-devel
Private headers, static library, pkg-config overlay and CMake config for libc++.

%package python
Summary:	Python subpackage to use flatbuffers
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}
Requires:   python3-devel
%description python
This package provides flatbuffers python API.

%prep
%setup -q
cp %{SOURCE1} .

%build
rm -rf ../flatbuffers-24.3.25-libcxx
cp -a . ../flatbuffers-24.3.25-libcxx
export CFLAGS+=" -fno-lto"
export CXXFLAGS+=" -fno-lto"
pushd python
export VERSION="24.3.25"
%{_bindir}/python3 setup.py build
popd

# flatbuffers build occasionally fails when using -j${BUILD_THREADS} with an error similar to:
# /mnt/source/flatbuffers/flatbuffers-1.6.0/samples/sample_binary.cpp:19:17: error: 'MyGame' has not been declared
# /mnt/source/flatbuffers/flatbuffers-1.6.0/samples/sample_binary.cpp:19:25: error: 'Sample' is not a namespace-name
# ...
# Disabling build tests gets rid of this flakiness and makes the compilation faster.
%{cmake} \
    -DFLATBUFFERS_INSTALL=ON \
    -DFLATBUFFERS_BUILD_SHAREDLIB=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
    -DFLATBUFFERS_BUILD_TESTS=OFF \
    -DCMAKE_INSTALL_INCLUDEDIR=%{_includedir} \
    -DCMAKE_INSTALL_LIBDIR=%{_libdir} \
    -DCMAKE_INSTALL_BINDIR=%{_bindir} \
    -DFB_CMAKE_DIR=%{_libdir}/cmake \
    -DCMAKE_BUILD_TYPE=Release .
%{__make} %{?_smp_mflags}

mkdir ../flatbuffers-24.3.25-libcxx/build-libcxx
pushd ../flatbuffers-24.3.25-libcxx/build-libcxx
CXX=clang++ \
CXXFLAGS="${CXXFLAGS} -fno-lto -stdlib=libc++" \
LDFLAGS="${LDFLAGS} -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed" \
%{cmake} \
    -DFLATBUFFERS_INSTALL=ON \
    -DFLATBUFFERS_BUILD_SHAREDLIB=ON \
    -DFLATBUFFERS_BUILD_FLATC=OFF \
    -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE \
    -DFLATBUFFERS_BUILD_TESTS=OFF \
    -DCMAKE_INSTALL_INCLUDEDIR=%{_includedir} \
    -DCMAKE_INSTALL_LIBDIR=%{_libdir} \
    -DFB_CMAKE_DIR=%{_libdir}/cmake \
    -DCMAKE_BUILD_TYPE=Release ..
%{__make} %{?_smp_mflags}
popd

%install
%{__make} DESTDIR=%{?buildroot:%{buildroot}} install
install -D -m 644 packaging/%{name}.pc.in %{buildroot}%{_libdir}/pkgconfig/%{name}.pc
sed -i 's#@version@#%{version}#g' %{buildroot}%{_libdir}/pkgconfig/%{name}.pc
sed -i 's#@libdir@#%{_libdir}#g' %{buildroot}%{_libdir}/pkgconfig/%{name}.pc
sed -i 's#@includedir@#%{_includedir}#g' %{buildroot}%{_libdir}/pkgconfig/%{name}.pc
mkdir -p %{buildroot}/%{python3_sitelib}/flatbuffers
pushd python
export VERSION="24.3.25"
%{_bindir}/python3 setup.py install --prefix=%{_prefix} --root=%{buildroot}
rm -r %{buildroot}/%{python3_sitelib}/*.egg-info
rm -r %{buildroot}/%{python3_sitelib}/flatbuffers/__pycache__
popd

rm -rf libcxx-install
%{__make} -C ../flatbuffers-24.3.25-libcxx/build-libcxx \
  DESTDIR="$PWD/libcxx-install" install
install -d %{buildroot}%{_libdir}
install -m 0755 libcxx-install%{_libdir}/libflatbuffers.so.%{version} \
  %{buildroot}%{_libdir}/libflatbuffers-libcxx.so.%{version}
patchelf --set-soname libflatbuffers-libcxx.so.%{version} \
  %{buildroot}%{_libdir}/libflatbuffers-libcxx.so.%{version}
ln -s libflatbuffers-libcxx.so.%{version} \
  %{buildroot}%{_libdir}/libflatbuffers-libcxx.so
install -m 0644 libcxx-install%{_libdir}/libflatbuffers.a \
  %{buildroot}%{_libdir}/libflatbuffers-libcxx.a
install -d %{buildroot}%{_includedir}/flatbuffers-libcxx
cp -a libcxx-install%{_includedir}/flatbuffers \
  %{buildroot}%{_includedir}/flatbuffers-libcxx/
install -d %{buildroot}%{_libdir}/libcxx-pkgconfig/flatbuffers
install -m 0644 libcxx-install%{_libdir}/pkgconfig/flatbuffers.pc \
  %{buildroot}%{_libdir}/libcxx-pkgconfig/flatbuffers/flatbuffers.pc
sed -i \
  -e 's|-lflatbuffers|-lflatbuffers-libcxx|' \
  -e 's|^includedir=.*|includedir=%{_includedir}/flatbuffers-libcxx|' \
  %{buildroot}%{_libdir}/libcxx-pkgconfig/flatbuffers/flatbuffers.pc
install -d %{buildroot}%{_libdir}/libcxx-prefix/flatbuffers/lib/cmake/flatbuffers
cp -a libcxx-install%{_libdir}/cmake/flatbuffers/. \
  %{buildroot}%{_libdir}/libcxx-prefix/flatbuffers/lib/cmake/flatbuffers/
sed -i \
  -e 's|libflatbuffers\.so|libflatbuffers-libcxx.so|g' \
  -e 's|%{_libdir}/libflatbuffers\.a|%{_libdir}/libflatbuffers-libcxx.a|g' \
  -e 's|%{_includedir}|%{_includedir}/flatbuffers-libcxx|g' \
  -e 's|${_IMPORT_PREFIX}/include|%{_includedir}/flatbuffers-libcxx|g' \
  -e 's|${_IMPORT_PREFIX}/%{_libdir}|%{_libdir}|g' \
  %{buildroot}%{_libdir}/libcxx-prefix/flatbuffers/lib/cmake/flatbuffers/*.cmake

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%post libcxx -p /sbin/ldconfig

%postun libcxx -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%manifest %{name}.manifest
%license LICENSE
%{_libdir}/libflatbuffers.so.*

%files libcxx
%defattr(-,root,root,-)
%manifest %{name}.manifest
%license LICENSE
%{_libdir}/libflatbuffers-libcxx.so.%{version}

%files libcxx-devel
%defattr(-,root,root,-)
%{_includedir}/flatbuffers-libcxx/
%{_libdir}/libflatbuffers-libcxx.a
%{_libdir}/libflatbuffers-libcxx.so
%{_libdir}/libcxx-pkgconfig/flatbuffers/flatbuffers.pc
%{_libdir}/libcxx-prefix/flatbuffers/

%files devel
%defattr(-,root,root,-)
%{_bindir}/flatc
%{_includedir}/flatbuffers
%{_libdir}/libflatbuffers.a
%{_libdir}/cmake/flatbuffers/*
%{_libdir}/libflatbuffers.so
%{_libdir}/pkgconfig/flatbuffers.pc

%files python
%defattr(-,root,root,-)
%manifest %{name}.manifest
%license LICENSE
%{python3_sitelib}/flatbuffers/

%changelog
* Mon Jan 6 2025 Yongjoo Ahn <yongjoo1.ahn@samsung.com>
- Release of 24.3.25+e6463926479bd6b330cbcf673f7e917803fd5831 (sync with TensorFlow Lite v2.18.0)
