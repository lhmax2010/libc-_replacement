set -eu
export PATH=/bin:/usr/bin:/sbin:/usr/sbin
export CMAKE_BUILD_PARALLEL_LEVEL=1
export QEMU_RESERVED_VA=0x100000000
printf 'BUILD_SHELL_PID=%s\n' "$$"
cd /home/abuild/rpmbuild
set +e
rpmbuild --define '_smp_mflags -j1' --define 'mlgo_build_jobs 1' \
  --define 'jobs 1' --define '_srcdefattr (-,root,root)' \
  --nosignature --target=armv7l-tizen-linux --define '_build_create_debug 1' \
  --noprep --noclean -ba /home/abuild/rpmbuild/SOURCES/llvm.spec
rc=$?
printf 'W2C_INNER_EXIT=%s\n' "$rc"
exit "$rc"
