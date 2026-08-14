#!/bin/sh
set +e

echo RESIDUE_SCAN_BEGIN
for base in /root /opt/usr /opt/home /home/owner /var/tmp /opt/var/tmp /tmp; do
    echo "BASE_BEGIN=$base"
    ls -ld "$base" 2>&1
    if test -d "$base"; then
        find "$base" -xdev \( -iname '*r36*' -o -iname '*r22*' -o -iname '*s1_*' -o -iname '*libc++abi*' -o -iname '*probe*' \) -print | while IFS= read -r p; do
            if test -n "$p"; then
                stat -c 'TRACE\t%n\t%s\t%y\t%U\t%G\t%A\t%a' "$p" 2>&1
            fi
        done
        echo "FIND_RC=$?"
    else
        echo BASE_NOT_DIRECTORY
    fi
    echo "BASE_END=$base"
done
echo R36_EXACT_PATH_BEGIN
if test -e /var/tmp/r36_s1_20260814; then
    stat -c 'TRACE\t%n\t%s\t%y\t%U\t%G\t%A\t%a' /var/tmp/r36_s1_20260814 2>&1
    find /var/tmp/r36_s1_20260814 -xdev -print | while IFS= read -r p; do stat -c 'TRACE\t%n\t%s\t%y\t%U\t%G\t%A\t%a' "$p" 2>&1; done
else
    echo NOT_FOUND
fi
echo R36_EXACT_PATH_END

echo ENV_CONFIG_TRACE_BEGIN
for f in /etc/environment /etc/profile /etc/bashrc /root/.profile /root/.bashrc /home/owner/.profile /home/owner/.bashrc; do
    if test -r "$f"; then
        echo "SEARCH_FILE=$f"
        grep -nE 'R3[0-9]|r3[0-9]|R22|r22|s1_|libc\+\+abi|LD_LIBRARY_PATH|/var/tmp/r36' "$f" 2>&1
        echo "GREP_RC=$?"
    else
        echo "SEARCH_FILE=$f NOT_AVAILABLE"
    fi
done
if test -d /etc/profile.d; then
    find /etc/profile.d -maxdepth 1 -type f -print 2>&1 | sort
    grep -nE 'R3[0-9]|r3[0-9]|R22|r22|s1_|libc\+\+abi|LD_LIBRARY_PATH|/var/tmp/r36' /etc/profile.d/* 2>&1
    echo "PROFILE_D_GREP_RC=$?"
else
    echo PROFILE_D_NOT_FOUND
fi
echo ENV_CONFIG_TRACE_END

echo LD_CONFIG_BEGIN
for f in /etc/ld.so.conf /etc/ld.so.conf.d/*; do
    if test -e "$f"; then
        ls -ld "$f" 2>&1
        if test -r "$f"; then cat "$f"; else echo NOT_READABLE; fi
    fi
done
if command -v ldconfig >/dev/null 2>&1; then
    ldconfig -p 2>&1
    echo "LDCONFIG_RC=$?"
else
    echo LDCONFIG_NOT_AVAILABLE
fi
echo LD_CONFIG_END
