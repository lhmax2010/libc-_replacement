#!/usr/bin/env bash
set -euxo pipefail

workspace=/home/toolchain/development/libc++_replacement
cd "$workspace"

diff -q progress/R14b/experiment_sources/L0_l0.cpp tmp/R14b/L0/src/l0.cpp
diff -q progress/R14b/experiment_sources/L1_thrower.cpp tmp/R14b/L1/src/thrower.cpp
diff -q progress/R14b/experiment_sources/L1_main.cpp tmp/R14b/L1/src/main.cpp

grep -F 'RESULT caught=42 outer=1 inner=1 rethrow=1' progress/R14b/L0/runtime/16_run_A_ld_debug.log
grep -F 'RESULT caught=42 outer=1 inner=1 rethrow=1' progress/R14b/L0/runtime/18_run_B_ld_debug.log
grep -F 'RESULT caught=73 destructor_total=3' progress/R14b/L1/runtime/16_run_A_ld_debug.log
grep -F 'RESULT caught=73 destructor_total=3' progress/R14b/L1/runtime/18_run_B_ld_debug.log

python3 - <<'PY'
import csv
from collections import Counter

same = list(csv.DictReader(open('progress/R14b/L2/tables/matrix_same.tsv'), delimiter='\t'))
cross = list(csv.DictReader(open('progress/R14b/L2/tables/matrix_cross.tsv'), delimiter='\t'))
landing = list(csv.DictReader(open('progress/R14b/L2/tables/symbol_landing.tsv'), delimiter='\t'))
assert len(same) == 48
assert all(row['outcome'] == 'NORMAL_EXIT_0' for row in same)
assert all(row['type_check'] == row['cleanup_count_check'] == row['order_check'] == 'PASS' for row in same)
assert len(cross) == 48
b = [row for row in cross if row['scheme'] == 'B']
a = [row for row in cross if row['scheme'] == 'A']
assert Counter(row['outcome'] for row in b) == Counter({'NORMAL_EXIT_0': 16, 'SIGNAL_SIGABRT': 4, 'SIGNAL_SIGSEGV': 4})
assert Counter(row['outcome'] for row in a) == Counter({'NORMAL_EXIT_1': 24})
assert all(row['cleanup_count_check'] == 'PASS' for row in a)
assert all(row['type_check'] == 'FAIL' for row in a)
assert len(landing) == 96
assert all(row['forced_provider_paths'] == 'NOT_OBSERVED' for row in landing)
assert all(row['same_exception_provider_observation'] != 'IMPLEMENTATION_SWITCH_OBSERVED' for row in landing)
assert all(row['ld_debug_file_count'] == '1' for row in same + cross)
print('EVIDENCE_ASSERTIONS=PASS')
PY

grep -F "liblegacy.so [0] to $workspace/tmp/R14b/runtime/L2B/libunwind.so.1 [0]: normal symbol \`_Unwind_Resume' [GCC_3.0]" progress/R14b/L2/raw/matrix_cross/C006_B_dt_needed_new_legacy_na_lazy_legacy_new/ld_debug.*
grep -F "liblegacy.so [0] to $workspace/tmp/R14b/runtime/L2B/libunwind.so.1 [0]: normal symbol \`_Unwind_Resume' [GCC_3.0]" progress/R14b/L2/raw/matrix_cross/C018_B_dlopen_new_legacy_global_lazy_legacy_new/ld_debug.*
