set -eu
trap 'printf "PREFLIGHT_EXIT=%s\n" "$?"' EXIT
id
printf 'guest_shell_pid=%s\n' "$$"
rpm --version
rpmbuild --help | grep -E 'noprep|noclean|short-circuit'
test -x /bin/armv7l-tizen-linux-gnueabi-clang
test -x /bin/armv7l-tizen-linux-gnueabi-clang++
armv7l-tizen-linux-gnueabi-clang++ --version
cd /home/abuild/rpmbuild/BUILD/llvm-22.1.8/build
printf 'completed_log_lines='; wc -l < .ninja_log
ninja -n -j1 > /tmp/w2c-ninja-dryrun.txt
tail -n 3 /tmp/w2c-ninja-dryrun.txt
