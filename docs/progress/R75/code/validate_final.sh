#!/usr/bin/env bash
set -euo pipefail

workspace=/home/toolchain/development/libc++_replacement
root="${workspace}/progress/R75"

test -s "${root}/REPORT.md"
test -s "${root}/SELF_DECISIONS_AND_QUESTIONS.md"
test -s "${root}/REVIEW_MAP.md"
test -s "${root}/raw/x86_debug/libstdcxx.stdout"
test -s "${root}/raw/x86_debug/libcxx.stdout"
test -s "${root}/raw/arm_final/matrix.tsv"

for family in libstdcxx libcxx; do
  count=$(awk '/EXIT_CODE: 134/{n++} END{print n+0}' "${root}"/raw/x86_final/${family}.run*.command)
  test "${count}" -eq 20
  prepared=$(rg -l 'EVENT prepared' "${root}"/raw/x86_final/${family}.run*.stderr | wc -l)
  target=$(rg -l 'EVENT target_allocate' "${root}"/raw/x86_final/${family}.run*.stderr | wc -l)
  test "${prepared}" -eq 20
  test "${target}" -eq 20
done

test "$(awk -F '\t' 'NR>1 {n++} END{print n+0}' "${root}/raw/arm_final/matrix.tsv")" -eq 40
test "$(awk -F '\t' 'NR>1 && $3 != 0 {n++} END{print n+0}' "${root}/raw/arm_final/matrix.tsv")" -eq 0
test "$(awk -F '\t' 'NR>1 && $4 == "NOT_AVAILABLE" {n++} END{print n+0}' "${root}/raw/arm_final/matrix.tsv")" -eq 0
test "$(awk -F '\t' 'NR>1 && $1 == "libstdcxx" && $4 == 139 && $5 == 1 && $6 == 1 {n++} END{print n+0}' "${root}/raw/arm_final/matrix.tsv")" -eq 20
test "$(awk -F '\t' 'NR>1 && $1 == "libcxx" && $4 == 134 && $5 == 1 && $6 == 1 {n++} END{print n+0}' "${root}/raw/arm_final/matrix.tsv")" -eq 20

rg -q '^debug_evidence_check=PASS$' "${root}/raw/005_debug_evidence_check.txt"
rg -q 'R75_PATH_REMOVED' "${root}/raw/arm_final/008_cleanup_verify.log"
rg -q 'libc\+\+\.so\.1 => /home/toolchain/development/libc\+\+_replacement/tmp/R69/build-x86_64/lib/' "${root}/raw/x86_final/libcxx.ldd"
rg -q 'libc\+\+\.so\.1 => /opt/usr/home/owner/share/tmp/r75_20260828/lib/' "${root}/raw/arm_final/006_identity.log"

for directory in "${root}/raw/x86_final" "${root}/raw/arm_final"; do
  test "$(find "${directory}" -maxdepth 1 -type f -name 'libstdcxx.run*.stdout' | wc -l)" -eq 20
  test "$(find "${directory}" -maxdepth 1 -type f -name 'libstdcxx.run*.stderr' | wc -l)" -eq 20
  test "$(find "${directory}" -maxdepth 1 -type f -name 'libcxx.run*.stdout' | wc -l)" -eq 20
  test "$(find "${directory}" -maxdepth 1 -type f -name 'libcxx.run*.stderr' | wc -l)" -eq 20
done

arm_trace="${root}/raw/arm_libstdcxx_terminate_sequence/007_gdb.stdout"
rg -q 'R75_STAGE_1' "${arm_trace}"
rg -q 'R75_STAGE_2' "${arm_trace}"
rg -q 'R75_STAGE_3' "${arm_trace}"
rg -q 'received signal SIGSEGV' "${arm_trace}"
rg -q 'std::terminate.*libstdc\+\+' "${arm_trace}"
rg -q '__gnu_cxx::__verbose_terminate_handler' "${arm_trace}"
rg -q 'basic_string.*shrink_to_fit' "${arm_trace}"
rg -q 'R75_GDB_PATH_REMOVED' "${root}/raw/arm_libstdcxx_terminate_sequence/009_cleanup_verify.log"

printf 'R75_FINAL_VALIDATION=PASS\n'
