#!/bin/sh
set +e

echo KEY_RUNTIME_BEGIN
for pattern in '/lib/libstdc++.so.6*' '/usr/lib/libstdc++.so.6*' '/lib/libgcc_s.so.1' '/usr/lib/libgcc_s.so.1' '/lib/libc.so.6' '/usr/lib/libc.so.6' '/lib/libpthread.so*' '/usr/lib/libpthread.so*'; do
    for f in $pattern; do
        if test -e "$f" -o -L "$f"; then
            ls -ld "$f" 2>&1
            printf 'REALPATH=%s\n' "$(readlink -f "$f" 2>&1)"
            sha256sum "$f" 2>&1
        fi
    done
done
echo KEY_RUNTIME_END

echo LIBCXX_SEARCH_BEGIN
find /lib /usr/lib /usr/local/lib /opt -xdev \( -name 'libc++.so*' -o -name 'libc++abi.so*' -o -name 'libc++.a' -o -name 'libc++abi.a' \) -print 2>&1
echo "LIBCXX_FIND_RC=$?"
echo LIBCXX_SEARCH_END

echo DISK_BEGIN
df -h 2>&1
echo "DF_RC=$?"
echo DISK_END

echo PROJECT_PROCESS_BEGIN
ps -ef 2>&1
echo "PS_RC=$?"
echo PROJECT_PROCESS_END
exit 0
