#!/bin/sh
set +e
echo PACKAGE_OWNERS_BEGIN
for f in /usr/lib/libstdc++.so.6.0.33 /usr/lib/libgcc_s.so.1 /usr/lib/libc.so.6 /usr/lib/libpthread.so.0; do
    echo "FILE=$f"
    rpm -qf "$f" 2>&1
    echo "RPM_QF_RC=$?"
done
echo PACKAGE_OWNERS_END
echo PACKAGE_VERIFY_BEGIN
for p in libstdc++ libgcc glibc; do
    echo "PACKAGE=$p"
    rpm -V "$p" 2>&1
    echo "RPM_V_RC=$?"
done
echo PACKAGE_VERIFY_END
echo LIBCXX_RPM_QUERY_BEGIN
rpm -qa --qf '%{NAME}-%{VERSION}-%{RELEASE}.%{ARCH}\n' 2>&1 | grep -Ei '(^|-)libc(\+\+|xx)(abi)?([.-]|$)'
echo "LIBCXX_RPM_GREP_RC=$?"
echo LIBCXX_RPM_QUERY_END
echo PROJECT_PROCESS_FILTER_BEGIN
ps -ef 2>&1 | grep -Ei '(^|[ /])(r36|r22|s1_|libc\+\+abi|probe)([ /]|$)' | grep -v '[g]rep'
echo "PROJECT_PROCESS_GREP_RC=$?"
echo PROJECT_PROCESS_FILTER_END
