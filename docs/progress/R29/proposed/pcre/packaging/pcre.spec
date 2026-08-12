%define keepstatic 1
%global __provides_exclude_from ^%{_libdir}/libcxx-pkgconfig/.*$
Name:           pcre
VCS:            platform/upstream/pcre#feba6a19627f5d6e74d551ef2fc8b33af0547780
Version:        8.45
Release:        1.13
License:        BSD-3-Clause
Summary:        A library for Perl-compatible regular expressions
Url:            ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/
Group:          System/Libraries
Source:         ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/%{name}-%{version}.tar.bz2
Source2:        baselibs.conf
Source10:       add_pie_compile_option.patch
Source1001: 	pcre.manifest
BuildRequires:  gcc-c++
BuildRequires:  libtool
BuildRequires:  pkg-config
BuildRequires:  clang
BuildRequires:  libc++-devel
BuildRequires:  libc++abi-devel
BuildRequires:  patchelf

%description
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.

%package        devel
Summary:        A library for Perl-compatible regular expressions
Group:          System/Libraries
Requires:       libpcre = %{version}
Requires:       libpcrecpp = %{version}
Requires:       libpcreposix = %{version}
Requires:       libstdc++-devel

%description devel
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.

%package        devel-static
Summary:        A library for Perl-compatible regular expressions
Group:          System/Libraries
Requires:       pcre-devel = %{version}

%description devel-static
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.
This package contains static versions of the PCRE libraries.

%package -n libpcre
Summary:        A library for Perl-compatible regular expressions
Group:          System/Libraries
Provides:       %{name} = %{version}
Obsoletes:      %{name} < %{version}

%description -n libpcre
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.

%package -n libpcreposix
Summary:        A library for Perl-compatible regular expressions
Group:          System/Libraries

%description -n libpcreposix
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.

%package -n libpcrecpp
Summary:        A library for Perl-compatible regular expressions
Group:          System/Libraries

%description -n libpcrecpp
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.

%package -n libpcrecpp-libcxx
Summary:        libc++ build of the PCRE C++ wrapper
Group:          System/Libraries
Requires:       libpcre = %{version}

%description -n libpcrecpp-libcxx
The PCRE C++ wrapper built with LLVM libc++, with an isolated SONAME.

%package        libcxx-devel
Summary:        Development files for the libc++ PCRE C++ wrapper
Group:          System/Libraries
Requires:       libpcrecpp-libcxx = %{version}-%{release}
Requires:       libc++-devel
Provides:       pkgconfig(libpcrecpp-libcxx) = %{version}

%description libcxx-devel
Private headers and pkg-config overlay for the libc++ PCRE C++ wrapper.

%package        libcxx-devel-static
Summary:        Static libc++ PCRE C++ wrapper
Group:          System/Libraries
Requires:       pcre-libcxx-devel = %{version}-%{release}

%description libcxx-devel-static
Static library for the libc++ PCRE C++ wrapper.

%package doc
Summary:        A library for Perl-compatible regular expressions
Group:          System/Libraries
BuildArch:      noarch

%description doc
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.

%package tools
Summary:        A library for Perl-compatible regular expressions
Group:          System/Libraries

%description tools
The PCRE library is a set of functions that implement regular
expression pattern matching using the same syntax and semantics
as Perl 5.

%prep
%setup -q
cp %{SOURCE1001} .
%{__patch} -p1 < %{SOURCE10}

%build
rm -rf ../pcre-8.45-libcxx
cp -a . ../pcre-8.45-libcxx
export CFLAGS="${CFLAGS} -fPIC"

# Available JIT archs see sljit/sljitConfig.h
autoreconf -fiv
%configure \
%ifarch %ix86 x86_64 %arm ppc ppc64 mips
	    --enable-jit \
%endif
	    --enable-static \
	    --with-link-size=2 \
	    --with-match-limit=10000000 \
	    --enable-newline-is-lf \
	    --enable-utf8 \
        --enable-unicode-properties
make %{?_smp_mflags}

pushd ../pcre-8.45-libcxx
autoreconf -fiv
CXX=clang++ \
CXXFLAGS="${CXXFLAGS} -stdlib=libc++" \
LDFLAGS="${LDFLAGS} -stdlib=libc++ -lc++abi" \
./configure \
            --host=%{_host} \
            --build=%{_build} \
            --program-prefix= \
            --disable-dependency-tracking \
            --prefix=%{_prefix} \
            --exec-prefix=%{_exec_prefix} \
            --bindir=%{_bindir} \
            --sbindir=%{_sbindir} \
            --sysconfdir=%{_sysconfdir} \
            --datadir=%{_datadir} \
            --includedir=%{_includedir} \
            --libdir=%{_libdir} \
            --libexecdir=%{_libexecdir} \
            --localstatedir=%{_localstatedir} \
            --sharedstatedir=%{_sharedstatedir} \
            --mandir=%{_mandir} \
            --infodir=%{_infodir} \
%ifarch %ix86 x86_64 %arm ppc ppc64 mips
            --enable-jit \
%endif
            --enable-static \
            --with-link-size=2 \
            --with-match-limit=10000000 \
            --enable-newline-is-lf \
            --enable-utf8 \
            --enable-unicode-properties
make %{?_smp_mflags}
popd


# %check
# export LANG=POSIX
# make check || exit 0

%install
%make_install
mkdir -p %{buildroot}/%{_defaultdocdir}
mv %{buildroot}/usr/share/doc/pcre %{buildroot}/%{_defaultdocdir}/pcre-doc
rm -f %{buildroot}%{_libdir}/*.la
%{remove_docs}

rm -rf libcxx-install
make -C ../pcre-8.45-libcxx DESTDIR="$PWD/libcxx-install" install
install -d %{buildroot}%{_libdir}
install -m 0755 libcxx-install%{_libdir}/libpcrecpp.so.0.0.2 \
  %{buildroot}%{_libdir}/libpcrecpp-libcxx.so.0.0.2
patchelf --set-soname libpcrecpp-libcxx.so.0 \
  %{buildroot}%{_libdir}/libpcrecpp-libcxx.so.0.0.2
ln -s libpcrecpp-libcxx.so.0.0.2 %{buildroot}%{_libdir}/libpcrecpp-libcxx.so.0
ln -s libpcrecpp-libcxx.so.0 %{buildroot}%{_libdir}/libpcrecpp-libcxx.so
install -m 0644 libcxx-install%{_libdir}/libpcrecpp.a \
  %{buildroot}%{_libdir}/libpcrecpp-libcxx.a
install -d %{buildroot}%{_includedir}/pcre-libcxx
install -m 0644 libcxx-install%{_includedir}/*.h \
  %{buildroot}%{_includedir}/pcre-libcxx/
install -d %{buildroot}%{_libdir}/libcxx-pkgconfig/pcre
install -m 0644 libcxx-install%{_libdir}/pkgconfig/libpcrecpp.pc \
  %{buildroot}%{_libdir}/libcxx-pkgconfig/pcre/libpcrecpp.pc
sed -i \
  -e 's|-lpcrecpp|-lpcrecpp-libcxx|' \
  -e 's|^includedir=.*|includedir=%{_includedir}/pcre-libcxx|' \
  %{buildroot}%{_libdir}/libcxx-pkgconfig/pcre/libpcrecpp.pc

%post -n libpcre -p /sbin/ldconfig

%postun -n libpcre -p /sbin/ldconfig

%post -n libpcrecpp -p /sbin/ldconfig

%postun -n libpcrecpp -p /sbin/ldconfig

%post -n libpcrecpp-libcxx -p /sbin/ldconfig

%postun -n libpcrecpp-libcxx -p /sbin/ldconfig

%post -n libpcreposix -p /sbin/ldconfig

%postun -n libpcreposix -p /sbin/ldconfig

%files -n libpcre
%manifest %{name}.manifest
%license LICENCE
%{_libdir}/libpcre.so.*

%files -n libpcrecpp
%manifest %{name}.manifest
%license LICENCE
%{_libdir}/libpcrecpp.so.*

%files -n libpcrecpp-libcxx
%manifest %{name}.manifest
%license LICENCE
%{_libdir}/libpcrecpp-libcxx.so.0
%{_libdir}/libpcrecpp-libcxx.so.0.0.2

%files -n libpcreposix
%manifest %{name}.manifest
%license LICENCE
%{_libdir}/libpcreposix.so.*

%files tools
%manifest %{name}.manifest
%license LICENCE
%{_bindir}/pcregrep
%{_bindir}/pcretest

%files devel
%license LICENCE
%manifest %{name}.manifest
%{_bindir}/pcre-config
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/libpcre.pc
%{_libdir}/pkgconfig/libpcrecpp.pc
%{_libdir}/pkgconfig/libpcreposix.pc

%files libcxx-devel
%license LICENCE
%manifest %{name}.manifest
%{_includedir}/pcre-libcxx/
%{_libdir}/libpcrecpp-libcxx.so
%{_libdir}/libcxx-pkgconfig/pcre/libpcrecpp.pc

%files devel-static
%license LICENCE
%manifest %{name}.manifest
%{_libdir}/*.a

%files libcxx-devel-static
%license LICENCE
%manifest %{name}.manifest
%{_libdir}/libpcrecpp-libcxx.a
