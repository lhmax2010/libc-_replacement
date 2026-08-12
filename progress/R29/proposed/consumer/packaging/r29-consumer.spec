Name:           r29-consumer
Version:        1
Release:        1
Summary:        R29 central-injection consumer fixture
License:        MIT
Source0:        %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig
BuildRequires:  pcre-devel
BuildRequires:  pcre-libcxx-devel
BuildRequires:  lottie-player-devel
BuildRequires:  lottie-player-libcxx-devel
BuildRequires:  flatbuffers-devel
BuildRequires:  flatbuffers-libcxx-devel

%description
Unchanged consumer spec used by both R29 central configuration runs.

%prep
%setup -q

%build
%{cmake} -DCMAKE_BUILD_TYPE=Release .
%{__make} %{?_smp_mflags}

%install
%{__make} DESTDIR=%{buildroot} install

%files
%{_bindir}/r29-pcre
%{_bindir}/r29-lottie
%{_bindir}/r29-flatbuffers
