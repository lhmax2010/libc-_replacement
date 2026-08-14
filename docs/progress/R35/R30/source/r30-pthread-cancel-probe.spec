Name:           r30-pthread-cancel-probe
Version:        1
Release:        1
Summary:        R30 pthread cancellation semantics probe
License:        MIT
Source0:        %{name}-%{version}.tar.gz

%description
Single-purpose R30 pthread cancellation semantics probe.

%prep
%setup -q

%build
%{__cxx} %{optflags} ${CXXFLAGS} -std=c++17 -pthread \
    pthread_cancel_probe.cpp -o pthread_cancel_probe ${LDFLAGS}

%install
install -D -m 0755 pthread_cancel_probe %{buildroot}%{_bindir}/pthread_cancel_probe

%files
%{_bindir}/pthread_cancel_probe
