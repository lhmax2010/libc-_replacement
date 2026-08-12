Name:       lottie-player
%global __provides_exclude_from ^%{_libdir}/libcxx-pkgconfig/.*$
VCS:        platform/core/uifw/lottie-player#b3a74520e861f09e20a90e75d95215ec619e48f0
Summary:    Platform independent standalone library that plays Lottie Animation
Version:    0.2.5
Release:    1
Group:      UI Framework/Services
License:    MIT and BSD and MPL-2.0
URL:        http://www.tizen.org/
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  pkgconfig
BuildRequires:  meson
BuildRequires:  ninja
BuildRequires:  pkgconfig(dlog)
BuildRequires:  clang
BuildRequires:  libc++-devel
BuildRequires:  libc++abi-devel
BuildRequires:  patchelf
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
rlottie is a platform independent standalone c++ library for rendering vector
based animations and art in realtime.

Lottie loads and renders animations and vectors exported in the bodymovin JSON
format. Bodymovin JSON can be created and exported from After Effects with
bodymovin, Sketch with Lottie Sketch Export, and from Haiku.

For the first time, designers can create and ship beautiful animations without
an engineer painstakingly recreating it by hand. Since the animation is backed
by JSON they are extremely small in size but can be large in complexity!


%package devel
Summary:    Development libraries for rlottie
Group:      UI Frameork/Services
Requires:   %{name} = %{version}-%{release}


%description devel
rlottie is a platform independent standalone c++ library for rendering vector
based animations and art in realtime.

Lottie loads and renders animations and vectors exported in the bodymovin JSON
format. Bodymovin JSON can be created and exported from After Effects with
bodymovin, Sketch with Lottie Sketch Export, and from Haiku.

For the first time, designers can create and ship beautiful animations without
an engineer painstakingly recreating it by hand. Since the animation is backed
by JSON they are extremely small in size but can be large in complexity!

%package libcxx
Summary:    libc++ build of rlottie
Group:      UI Framework/Services
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description libcxx
The rlottie shared library built with LLVM libc++, with an isolated SONAME.

%package libcxx-devel
Summary:    Development files for the libc++ rlottie build
Group:      UI Framework/Services
Requires:   %{name}-libcxx = %{version}-%{release}
Requires:   libc++-devel
Provides:   pkgconfig(rlottie-libcxx) = %{version}

%description libcxx-devel
Private headers and pkg-config overlay for the libc++ rlottie build.


%prep
%setup -q


%build

# The upstream development helper named "format" shadows libc++'s <format>
# when Meson emits -I.. .  Keep the default tree untouched and remove that
# non-header helper only from the isolated libc++ build copy.
rm -rf ../lottie-player-0.2.5-libcxx
cp -a . ../lottie-player-0.2.5-libcxx
mv ../lottie-player-0.2.5-libcxx/format \
   ../lottie-player-0.2.5-libcxx/format.command

export DESTDIR=%{buildroot}

export CXXFLAGS+=" -std=gnu++14"

%{?_toolchain:
%ifarch %arm
%if %{toolchain_is clang}
CXXFLAGS+=" -fno-integrated-as"
%endif
%endif
}

meson setup \
                --prefix /usr \
                --libdir %{_libdir} \
                builddir 2>&1
ninja \
      -C builddir \
      -j %(echo "`/usr/bin/getconf _NPROCESSORS_ONLN`")

pushd ../lottie-player-0.2.5-libcxx
CXX=clang++ \
CXXFLAGS="${CXXFLAGS} -std=gnu++14 -stdlib=libc++" \
LDFLAGS="${LDFLAGS} -stdlib=libc++ -Wl,--no-as-needed -lc++abi -Wl,--as-needed" \
meson setup \
                --prefix /usr \
                --libdir %{_libdir} \
                -Dmoduledir=%{_libdir}/rlottie-libcxx \
                builddir-libcxx 2>&1
ninja \
      -C builddir-libcxx \
      -j %(echo "`/usr/bin/getconf _NPROCESSORS_ONLN`")
popd

%install

export DESTDIR=%{buildroot}

ninja -C builddir install

rm -rf libcxx-install
DESTDIR="$PWD/libcxx-install" \
  ninja -C ../lottie-player-0.2.5-libcxx/builddir-libcxx install
install -d %{buildroot}%{_libdir}
install -m 0755 libcxx-install%{_libdir}/librlottie.so.0.2 \
  %{buildroot}%{_libdir}/librlottie-libcxx.so.0.2
patchelf --set-soname librlottie-libcxx.so.0 \
  %{buildroot}%{_libdir}/librlottie-libcxx.so.0.2
ln -s librlottie-libcxx.so.0.2 %{buildroot}%{_libdir}/librlottie-libcxx.so.0
ln -s librlottie-libcxx.so.0 %{buildroot}%{_libdir}/librlottie-libcxx.so
install -d %{buildroot}%{_libdir}/rlottie-libcxx
install -m 0755 libcxx-install%{_libdir}/rlottie-libcxx/librlottie-image-loader.so \
  %{buildroot}%{_libdir}/rlottie-libcxx/librlottie-image-loader.so
install -d %{buildroot}%{_includedir}/lottie-player-libcxx
install -m 0644 libcxx-install%{_includedir}/*.h \
  %{buildroot}%{_includedir}/lottie-player-libcxx/
install -d %{buildroot}%{_libdir}/libcxx-pkgconfig/lottie-player
install -m 0644 libcxx-install%{_libdir}/pkgconfig/rlottie.pc \
  %{buildroot}%{_libdir}/libcxx-pkgconfig/lottie-player/rlottie.pc
sed -i \
  -e 's|-lrlottie|-lrlottie-libcxx|' \
  -e 's|^includedir=.*|includedir=%{_includedir}/lottie-player-libcxx|' \
  %{buildroot}%{_libdir}/libcxx-pkgconfig/lottie-player/rlottie.pc

%files
%defattr(-,root,root,-)
%{_libdir}/librlottie.so.*
%{_libdir}/librlottie-image-loader.so*
%manifest packaging/rlottie.manifest
%license COPYING licenses/COPYING*

%files devel
%defattr(-,root,root,-)
%{_includedir}/*.h
%{_libdir}/librlottie.so
%{_libdir}/librlottie-image-loader.so

%{_libdir}/pkgconfig/rlottie.pc

%files libcxx
%defattr(-,root,root,-)
%manifest packaging/rlottie.manifest
%license COPYING licenses/COPYING*
%{_libdir}/librlottie-libcxx.so.0
%{_libdir}/librlottie-libcxx.so.0.2
%{_libdir}/rlottie-libcxx/librlottie-image-loader.so

%files libcxx-devel
%defattr(-,root,root,-)
%{_includedir}/lottie-player-libcxx/
%{_libdir}/librlottie-libcxx.so
%{_libdir}/libcxx-pkgconfig/lottie-player/rlottie.pc
