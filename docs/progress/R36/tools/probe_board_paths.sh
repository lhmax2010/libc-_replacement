#!/bin/bash
set -u

device=192.168.108.26:26101
probe=/home/toolchain/development/libc++_replacement/progress/R36/board/exec_probe_armv7l
stamp=r36_path_probe_20260814
paths=(/root /opt/usr /opt/home /home/owner /var/tmp /opt/var/tmp)

remote() {
    local command=$1
    printf 'SDB_SHELL_COMMAND=%q\n' "$command"
    sdb -s "$device" shell "$command"
    local carrier_rc=$?
    printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$carrier_rc"
    return "$carrier_rc"
}

for base in "${paths[@]}"; do
    dir="$base/$stamp"
    echo "=== PATH_BEGIN $base ==="
    remote "if test -d '$base'; then ls -ldZ '$base' 2>&1; mkdir '$dir'; rc=\$?; else echo CANDIDATE_NOT_FOUND; rc=44; fi; echo __R36_REMOTE_RC=\$rc"
    mkdir_output=$(sdb -s "$device" shell "test -d '$dir'; rc=\$?; echo __R36_REMOTE_RC=\$rc" 2>&1)
    mkdir_carrier=$?
    printf '%s\n' "$mkdir_output"
    printf 'SDB_CARRIER_EXIT_CODE=%d\n' "$mkdir_carrier"
    if printf '%s\n' "$mkdir_output" | grep -q '__R36_REMOTE_RC=0'; then
        printf 'SDB_PUSH_COMMAND=%q %q %q %q %q\n' sdb -s "$device" push "$probe" "$dir/exec_probe_armv7l"
        sdb -s "$device" push "$probe" "$dir/exec_probe_armv7l"
        push_rc=$?
        printf 'SDB_PUSH_EXIT_CODE=%d\n' "$push_rc"
        if [ "$push_rc" -eq 0 ]; then
            remote "chmod 700 '$dir/exec_probe_armv7l'; rc_chmod=\$?; ls -lZ '$dir/exec_probe_armv7l' 2>&1; '$dir/exec_probe_armv7l'; rc_exec=\$?; echo __R36_CHMOD_RC=\$rc_chmod; echo __R36_EXEC_RC=\$rc_exec"
        fi
    else
        echo "PUSH_AND_EXEC=NOT_RUN"
    fi
    remote "rm -f '$dir/exec_probe_armv7l'; rc_file=\$?; rmdir '$dir' 2>/dev/null; rc_dir=\$?; test ! -e '$dir'; rc_verify=\$?; echo __R36_REMOVE_FILE_RC=\$rc_file; echo __R36_RMDIR_RC=\$rc_dir; echo __R36_CLEAN_VERIFY_RC=\$rc_verify"
    echo "=== PATH_END $base ==="
done
