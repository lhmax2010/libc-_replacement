#!/usr/bin/env bash
set -euo pipefail
export PATH=/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
workspace=/home/toolchain/development/libc++_replacement
package=$1
arch=$2
mode=$3
release=$4
case "$package:$arch" in
  bcc-tools:armv7l|bpftrace:armv7l|bpftrace:aarch64) ;;
  *) echo 'cell outside requested scope' >&2; exit 2;;
esac
case "$mode" in libcxx|gcc) ;; *) exit 2;; esac
build_root="$workspace/tmp/GBS-ROOT/BUILD-W2C-${package}-${arch}-${mode}"
if [ -e "$build_root" ]; then
  echo "Build root already exists; explicit resume decision required: $build_root" >&2
  exit 2
fi
extra=()
if [ "$mode" = gcc ]; then extra+=(--define '_toolchain_override gcc'); fi
cd "$workspace/codes/R104/$package"
exec python3 -u "$workspace/progress/BUILD_W2C_0915/govern_resume.py" \
  --unit "build-w2c-${package}-${arch}-${mode}" -- \
  gbs -c "$workspace/progress/BUILD_W2C_0915/gbs_${mode}.conf" \
  build -A "$arch" -B "$build_root" -C --fail-fast --threads 1 \
  --define '_smp_mflags -j1' --define 'jobs 1' --release "$release" \
  --style git --include-all --packaging-dir packaging --spec "$package.spec" \
  "${extra[@]}"
