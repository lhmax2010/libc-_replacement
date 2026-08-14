#!/bin/sh
set +e

echo IDENTITY_BEGIN
uname -a
printf 'ARCH='
uname -m
id
for f in /etc/tizen-release /etc/os-release /etc/info.ini /etc/tizen-build.conf /etc/build-id /etc/tizen-build-id; do
    if test -r "$f"; then
        echo "FILE=$f"
        cat "$f"
    else
        echo "FILE=$f NOT_AVAILABLE"
    fi
done
for f in /etc/machine-id /var/lib/dbus/machine-id /proc/sys/kernel/random/boot_id /sys/firmware/devicetree/base/serial-number; do
    if test -r "$f"; then
        echo "FILE=$f"
        tr '\000' '\n' < "$f"
    else
        echo "FILE=$f NOT_AVAILABLE"
    fi
done
echo CPU_IDENTITY_BEGIN
grep -E '^(Hardware|Revision|Serial|Model)[[:space:]]*:' /proc/cpuinfo 2>&1
echo CPU_IDENTITY_END
echo RPM_COUNT_AND_SET_SHA_BEGIN
rpm -qa --qf '%{NAME}-%{VERSION}-%{RELEASE}.%{ARCH}\n' 2>&1 | sort | tee /dev/stdout | sha256sum
echo "RPM_QUERY_RC=$?"
echo RPM_COUNT_AND_SET_SHA_END
echo IDENTITY_END
