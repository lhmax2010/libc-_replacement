














Name:           libcxx-runtimes
Version:        22.1.8
Release:        1
ExclusiveArch:  armv7l aarch64 x86_64
Summary:        LLVM libc++ and libc++abi runtimes
Group:          Development/Toolchain
License:        Apache-2.0 WITH LLVM-exception
URL:            https://llvm.org/
Source0:        llvm-22.1.8.tar.gz
Source1:        libcxx-llvm22.map
Source2:        libcxx-runtimes.manifest
Source3:        SOURCE_PROVENANCE

BuildRequires:  clang = 22.1.8
BuildRequires:  cmake
BuildRequires:  ninja
BuildRequires:  python3
BuildRequires:  glibc-devel
BuildRequires:  libgcc

%description
Source package for the LLVM libc++ standard library and libc++abi runtime.
The libraries use libgcc_s as the unwinder. Existing interfaces retain LLVM_22;
condition-variable waits also provide unwind-capable LLVM_22_TIZEN_1 defaults.

%package -n libc++
Summary:        LLVM C++ standard library runtime
Requires:       libc++abi = 22.1.8-1

%description -n libc++
Shared LLVM libc++ runtime, built against libc++abi and libgcc_s.

%package -n libc++-devel
Summary:        LLVM C++ standard library headers and development files
Requires:       libc++ = 22.1.8-1
Requires:       libc++abi-devel = 22.1.8-1

%description -n libc++-devel
Headers, ABI linker script, module metadata, and development files for LLVM
libc++.

%package -n libc++abi
Summary:        LLVM C++ ABI runtime

%description -n libc++abi
Shared LLVM libc++abi runtime using libgcc_s rather than LLVM libunwind.

%package -n libc++abi-devel
Summary:        LLVM C++ ABI development files
Requires:       libc++abi = 22.1.8-1

%description -n libc++abi-devel
Linker name for LLVM libc++abi.  Its public headers are installed in the
common c++/v1 tree shipped by libc++-devel.

%prep
%setup -q -n llvm-22.1.8
cp /home/toolchain/rpmbuild/SOURCES/libcxx-runtimes.manifest .
cp /home/toolchain/rpmbuild/SOURCES/SOURCE_PROVENANCE .

%build

mkdir -p build
cd build

cmake -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=aarch64-tizen-linux-gnu-clang \
  -DCMAKE_CXX_COMPILER=aarch64-tizen-linux-gnu-clang++ \
  -DCMAKE_ASM_COMPILER=aarch64-tizen-linux-gnu-clang \
  -DCMAKE_C_COMPILER_TARGET=aarch64-tizen-linux-gnu \
  -DCMAKE_CXX_COMPILER_TARGET=aarch64-tizen-linux-gnu \
  -DCMAKE_ASM_COMPILER_TARGET=aarch64-tizen-linux-gnu \
  -DCMAKE_C_FLAGS="$CFLAGS --rtlib=libgcc" \
  -DCMAKE_CXX_FLAGS="$CXXFLAGS --rtlib=libgcc" \
  -DCMAKE_ASM_FLAGS="$CFLAGS" \
  -DCMAKE_SHARED_LINKER_FLAGS="-Wl,--version-script=/home/toolchain/rpmbuild/SOURCES/libcxx-llvm22.map -Wl,-z,now --rtlib=libgcc" \
  -DCMAKE_EXE_LINKER_FLAGS="--rtlib=libgcc" \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DCMAKE_INSTALL_LIBDIR=lib64 \
  -DLIBCXX_INSTALL_LIBRARY_DIR=lib64 \
  -DLIBCXXABI_INSTALL_LIBRARY_DIR=lib64 \
  -DLLVM_ENABLE_RUNTIMES="libcxxabi;libcxx" \
  -DLLVM_DEFAULT_TARGET_TRIPLE=aarch64-tizen-linux-gnu \
  -DLLVM_INCLUDE_TESTS=OFF \
  -DLIBCXX_ENABLE_SHARED=ON \
  -DLIBCXX_ENABLE_TIZEN_CONDVAR_VERSIONS=ON \
  -DLIBCXX_ENABLE_STATIC=ON \
  -DLIBCXX_INSTALL_SHARED_LIBRARY=ON \
  -DLIBCXX_INSTALL_STATIC_LIBRARY=ON \
  -DLIBCXX_CXX_ABI=libcxxabi \
  -DLIBCXX_ENABLE_STATIC_ABI_LIBRARY=OFF \
  -DLIBCXX_ENABLE_ABI_LINKER_SCRIPT=ON \
  -DLIBCXX_USE_COMPILER_RT=OFF \
  -DLIBCXXABI_ENABLE_SHARED=ON \
  -DLIBCXXABI_ENABLE_STATIC=ON \
  -DLIBCXXABI_INSTALL_SHARED_LIBRARY=ON \
  -DLIBCXXABI_INSTALL_STATIC_LIBRARY=ON \
  -DLIBCXXABI_USE_COMPILER_RT=OFF \
  -DLIBCXXABI_USE_LLVM_UNWINDER=OFF \
  -DLIBCXXABI_ENABLE_STATIC_UNWINDER=OFF \
  -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_SHARED_LIBRARY=OFF \
  -DLIBCXXABI_STATICALLY_LINK_UNWINDER_IN_STATIC_LIBRARY=OFF \
  -DLIBCXXABI_ENABLE_NEW_DELETE_DEFINITIONS=ON \
  -DLIBCXX_INCLUDE_TESTS=OFF \
  -DLIBCXXABI_INCLUDE_TESTS=OFF \
  -DLIBCXX_INCLUDE_BENCHMARKS=OFF \
  ../runtimes

cmake --build . --target cxxabi cxx cxx_experimental -j1

%install
cd build
DESTDIR=/home/toolchain/rpmbuild/BUILDROOT/libcxx-runtimes-22.1.8-1.x86_64 cmake --install .

install -d /home/toolchain/rpmbuild/BUILDROOT/libcxx-runtimes-22.1.8-1.x86_64/usr/share/doc/libcxx-runtimes
install -m 0644 ../SOURCE_PROVENANCE \
  /home/toolchain/rpmbuild/BUILDROOT/libcxx-runtimes-22.1.8-1.x86_64/usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE

%files -n libc++
%manifest libcxx-runtimes.manifest
%defattr(-,root,root,-)
%license libcxx/LICENSE.TXT
/usr/lib64/libc++.so.1*

%files -n libc++-devel
%manifest libcxx-runtimes.manifest
%defattr(-,root,root,-)
/usr/include/c++/v1
/usr/lib64/libc++.so
/usr/lib64/libc++experimental.a
/usr/lib64/libc++.a
/usr/lib64/libc++.modules.json
/usr/share/libc++/v1
/usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE

%files -n libc++abi
%manifest libcxx-runtimes.manifest
%defattr(-,root,root,-)
%license libcxxabi/LICENSE.TXT
/usr/lib64/libc++abi.so.1*

%files -n libc++abi-devel
%manifest libcxx-runtimes.manifest
%defattr(-,root,root,-)
/usr/lib64/libc++abi.so
/usr/lib64/libc++abi.a

%changelog
* Tue Aug 04 2026 Toolchain ABI Team <toolchain@example.invalid> 22.1.8-1
- Migrate to the verbatim platform llvm-22.1.8 source tarball and LLVM_22 symbol node.
- Preserve the adjudicated 21.1.1 multi-architecture packaging decisions.

* Sat Jul 25 2026 Toolchain ABI Team <toolchain@example.invalid> 21.1.1-2
- Build armv7l, aarch64, and x86_64 repository-candidate runtimes.

* Thu Jul 23 2026 Toolchain ABI Team <toolchain@example.invalid> 21.1.1-1
- Initial armv7l PoC packaging with LLVM_21 default symbol versions.
