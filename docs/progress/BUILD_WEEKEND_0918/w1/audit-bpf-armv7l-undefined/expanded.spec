HARNESS_BEFORE_UNDEFINE=0|
HARNESS_AFTER_UNDEFINE=0|
HARNESS_AFTER_INCLUDE=0||armv7l-tizen-linux-gnueabi-gcc|armv7l-tizen-linux-gnueabi-g++














Name:           bpftrace
Version:        0.24.2
Release:        1
Summary:        High-level tracing language for Linux eBPF
License:        Apache-2.0

Source0:        bpftrace-0.24.2.tar.gz
Source1001:     bpftrace.manifest
Source1002:     prebuilt-static-bpftrace.tar.gz

ExclusiveArch:  armv7l aarch64

BuildRequires:  gcc-c++
BuildRequires:  bison
BuildRequires:  flex
BuildRequires:  cmake
BuildRequires:  pkgconfig(libelf)
BuildRequires:  zlib-devel
BuildRequires:  llvm-devel
BuildRequires:  llvm-static-devel
BuildRequires:  clang-devel
BuildRequires:  bcc-tools-devel
BuildRequires:  libbpf-devel

BuildRequires:  vim
BuildRequires:  cereal-devel
BuildRequires:  tar
BuildRequires:  libomp









%description
BPFtrace is a high-level tracing language for Linux enhanced Berkeley Packet
Filter (eBPF) available in recent Linux kernels (4.x). BPFtrace uses LLVM as a
backend to compile scripts to BPF-bytecode and makes use of BCC for
interacting with the Linux BPF system, as well as existing Linux tracing
capabilities: kernel dynamic tracing (kprobes), user-level dynamic tracing
(uprobes), and tracepoints. The BPFtrace language is inspired by awk and C,
and predecessor tracers such as DTrace and SystemTap

%package common
Summary: Common files for bpftrace (scripts, man pages, bash-completion)

%description common
This package contains architecture-independent files for bpftrace:
- Example bpftrace scripts (.bt files)
- Man pages
- Bash completion

%package static
Summary: High-level tracing language for Linux eBPF (static binary)

%description static
BPFtrace is a high-level tracing language for Linux enhanced Berkeley Packet
Filter (eBPF) available in recent Linux kernels (4.x). BPFtrace uses LLVM as a
backend to compile scripts to BPF-bytecode and makes use of BCC for
interacting with the Linux BPF system, as well as existing Linux tracing
capabilities: kernel dynamic tracing (kprobes), user-level dynamic tracing
(uprobes), and tracepoints. The BPFtrace language is inspired by awk and C,
and predecessor tracers such as DTrace and SystemTap

This package is the bpftrace static binary.

%prep
%setup -q
cp /tmp/bpf-sources/bpftrace.manifest .
tar xzvf /tmp/bpf-sources/prebuilt-static-bpftrace.tar.gz

%build
export AR=gcc-ar RANLIB=gcc-ranlib;
export CC=armv7l-tizen-linux-gnueabi-gcc CXX=armv7l-tizen-linux-gnueabi-g++;





mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_VERBOSE_MAKEFILE=ON -DBUILD_TESTING=OFF -DSTATIC_LINKING=OFF \
         -DCMAKE_SKIP_RPATH:BOOL=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr
/bin/make -O -j1
cd ..



%install
export AR=gcc-ar RANLIB=gcc-ranlib;
export CC=armv7l-tizen-linux-gnueabi-gcc CXX=armv7l-tizen-linux-gnueabi-g++;

cd build

  /bin/make \
        DESTDIR=/tmp/task/rpm-bpf-armv7l-undefined/BUILDROOT/bpftrace-0.24.2-1.arm \
        INSTALL_ROOT=/tmp/task/rpm-bpf-armv7l-undefined/BUILDROOT/bpftrace-0.24.2-1.arm \
  install  
  rm -f /tmp/task/rpm-bpf-armv7l-undefined/BUILDROOT/bpftrace-0.24.2-1.arm/usr/share/info/dir 
  find /tmp/task/rpm-bpf-armv7l-undefined/BUILDROOT/bpftrace-0.24.2-1.arm -regex ".*\.la$" | xargs rm -f -- 
  find /tmp/task/rpm-bpf-armv7l-undefined/BUILDROOT/bpftrace-0.24.2-1.arm -regex ".*\.a$" | xargs rm -f --
cd ..

# Install bpftrace static binary
install -m 0755 prebuilt-static-bpftrace/arm/usr/bin/bpftrace /tmp/task/rpm-bpf-armv7l-undefined/BUILDROOT/bpftrace-0.24.2-1.arm/usr/bin/bpftrace-static

%post static
cp -rf /usr/bin/bpftrace-static /usr/bin/bpftrace
rm -rf /usr/bin/bpftrace-static

%postun static
rm -rf /usr/bin/bpftrace

%files
%manifest bpftrace.manifest
%license LICENSE
/usr/bin/bpftrace
/usr/bin/bpftrace-aotrt

%files common
%manifest bpftrace.manifest
%license LICENSE
/usr/share/bpftrace/
/usr/share/man/man8/*.gz
/usr/share/bash-completion/completions/bpftrace

%files static
%manifest bpftrace.manifest
%license LICENSE
/usr/bin/bpftrace-static

