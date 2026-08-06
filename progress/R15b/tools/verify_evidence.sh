#!/usr/bin/env bash
set -euxo pipefail

cd /home/toolchain/development/libc++_replacement
grep -F '2 packets transmitted, 2 received, 0% packet loss' progress/R15b/commands/01_ping.log
grep -F 'EXIT_CODE=255' progress/R15b/commands/32_ssh_explicit_port22.log
grep -F '192.168.108.25:26101' progress/R15b/commands/04_sdb_devices.log
grep -F 'armv7l' progress/R15b/commands/05_identity.log
grep -F 'BUILD_ID=tizen-unified-dev_20260727.074529_tizen-headed-armv7l' progress/R15b/commands/05_identity.log
grep -F 'UNWIND_EXPORT_COUNT=0' progress/R15b/commands/27_nongnu_unwind_export_count.log
grep -F 'DEFINED_UNWIND_EXPORT_COUNT=16' progress/R15b/commands/33_libgcc_unwind_defined_count.log
grep -F 'DEFINED_UNWIND_EXPORT_COUNT=0' progress/R15b/commands/34_libunwind_defined_count.log
grep -F 'ABSENT=/opt/.r15b_probe_20260806' progress/R15b/commands/35_probe_cleanup_verification.log
test "$(grep -c 'LDD_EXIT_CODE=0' progress/R15b/commands/19_representative_ldd_r.log)" -eq 3
test "$(grep -c 'COMMAND_V_EXIT_CODE=1' progress/R15b/commands/29_tool_availability_exit_codes.log)" -eq 11
test "$(sha256sum tmp/R15b/pulled/libgcc_s.so.1 | cut -d' ' -f1)" = 53f1c12ec20a3daa7c63d4e0631e5a749f42ddd53c5d0919cc90213653f690c7
test "$(sha256sum tmp/R15b/pulled/libunwind.so.8.0.1 | cut -d' ' -f1)" = 0e035b1af7627c5ec19b85a7d68620b05e4dc133ac8705d70f74f1ad89c40ba5
printf 'EVIDENCE_ASSERTIONS=PASS\n'
