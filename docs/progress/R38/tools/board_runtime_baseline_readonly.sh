#!/bin/sh
set +e

echo BOARD_RUNTIME_BASELINE_BEGIN
echo LIBGCC_CANDIDATES_BEGIN
find /lib /usr/lib /lib32 /usr/lib32 -name 'libgcc_s.so.1' -type f -o -name 'libgcc_s.so.1' -type l 2>&1
echo LIBGCC_CANDIDATES_END

libgcc=
for candidate in /lib/libgcc_s.so.1 /usr/lib/libgcc_s.so.1 /lib32/libgcc_s.so.1 /usr/lib32/libgcc_s.so.1; do
    if test -e "$candidate"; then
        libgcc=$candidate
        break
    fi
done
echo "LIBGCC_PATH=$libgcc"
if test -n "$libgcc"; then
    ls -l "$libgcc"
    sha256sum "$libgcc"
    rpm -qf "$libgcc" 2>&1
    rpm -q --qf 'NAME=%{NAME}\nEPOCH=%{EPOCH}\nVERSION=%{VERSION}\nRELEASE=%{RELEASE}\nARCH=%{ARCH}\nSOURCERPM=%{SOURCERPM}\n' "$(rpm -qf --qf '%{NAME}' "$libgcc" 2>/dev/null)" 2>&1
    readelf --dyn-syms -W "$libgcc" 2>&1 | grep -E '(_Unwind|Unwind_VRS|__gnu_unwind_frame)'
    readelf --version-info -W "$libgcc" 2>&1
fi

echo LIBSTDCXX_CANDIDATES_BEGIN
find /lib /usr/lib /lib32 /usr/lib32 -name 'libstdc++.so*' -type f -o -name 'libstdc++.so*' -type l 2>&1
echo LIBSTDCXX_CANDIDATES_END
for candidate in /lib/libstdc++.so.6 /usr/lib/libstdc++.so.6 /lib32/libstdc++.so.6 /usr/lib32/libstdc++.so.6; do
    if test -e "$candidate"; then
        echo "LIBSTDCXX_PATH=$candidate"
        ls -l "$candidate"
        sha256sum "$candidate"
        rpm -qf "$candidate" 2>&1
        package=$(rpm -qf --qf '%{NAME}' "$candidate" 2>/dev/null)
        rpm -q --qf 'NAME=%{NAME}\nEPOCH=%{EPOCH}\nVERSION=%{VERSION}\nRELEASE=%{RELEASE}\nARCH=%{ARCH}\nSOURCERPM=%{SOURCERPM}\n' "$package" 2>&1
        break
    fi
done
echo BOARD_RUNTIME_BASELINE_END
