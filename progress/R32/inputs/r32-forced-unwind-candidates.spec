Name:           r32-forced-unwind-candidates
Version:        1
Release:        1
Summary:        R32 forced unwind candidate behavior experiment
License:        MIT
Source0:        %{name}-%{version}.tar.gz

%description
Single-purpose x86_64 native forced-unwind candidate behavior experiment.

%prep
%setup -q

%build
%{__cxx} %{optflags} ${CXXFLAGS} -O0 -g -fno-omit-frame-pointer \
    -std=c++17 -pthread forced_unwind_candidates.cpp \
    -o r32-forced-unwind-candidates ${LDFLAGS}

%install
install -D -m 0755 r32-forced-unwind-candidates \
    "%{buildroot}%{_bindir}/r32-forced-unwind-candidates"

%files
%{_bindir}/r32-forced-unwind-candidates
