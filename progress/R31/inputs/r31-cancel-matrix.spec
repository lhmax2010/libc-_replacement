Name:           r31-cancel-matrix
Version:        1
Release:        1
Summary:        R31 pthread cancellation diagnostic matrix
License:        MIT
Source0:        %{name}-%{version}.tar.gz

%description
Single-purpose x86_64 pthread cancellation diagnostic matrix.

%prep
%setup -q

%build
for item in \
    bare-catchall:1 \
    forced-guard:2 \
    noexcept-catchall:3 \
    std-thread-catchall:4 \
    bare-no-catch:5; do
    name=${item%%:*}
    kind=${item##*:}
    %{__cxx} %{optflags} ${CXXFLAGS} -O0 -g -fno-omit-frame-pointer \
        -std=c++17 -pthread -DCASE_KIND=${kind} cancel_matrix.cpp \
        -o r31-${name} ${LDFLAGS}
done

%install
for binary in r31-*; do
    install -D -m 0755 "$binary" "%{buildroot}%{_bindir}/$binary"
done

%files
%{_bindir}/r31-*
