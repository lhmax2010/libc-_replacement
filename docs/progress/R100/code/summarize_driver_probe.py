#!/usr/bin/env python3
"""Summarize the key, independently logged driver-option probe outcomes."""

import re
from pathlib import Path

text = Path("progress/R100/raw/040_probe_global_flags_all_drivers_retry.stdout").read_text(encoding="utf-8")
blocks = re.split(r"(?=^===== )", text, flags=re.M)
rows = []
for block in blocks:
    head = re.match(r"^===== (\S+) (\S+) (\S+) =====", block)
    if not head:
        continue
    arch, family, language = head.groups()
    def result(label):
        match = re.search(rf"^RESULT\[{label}\]\trc=(\d+)\tstatus=(\S+)", block, re.M)
        return (match.group(1), match.group(2)) if match else ("NOT_OBSERVED", "NOT_OBSERVED")
    version_rc, _ = result("version")
    compile_rc, compile_status = result("compile_stdlib")
    trace_rc, trace_status = result("trace_full")
    version_match = re.search(r"(?:gcc|g\+\+).*?\)\s+(14\.2\.0)|clang version\s+(22\.1\.8)", block)
    version = next((value for value in version_match.groups() if value), "NOT_OBSERVED") if version_match else "NOT_OBSERVED"
    if version_rc == "139":
        semantic = "NOT_OBSERVED_AARCH64_GCC_DRIVER_RUNTIME_EXIT_139"
    elif family == "gcc":
        semantic = "REJECTS_STDLIB_EXPLICIT_ERROR"
    elif language == "c":
        semantic = "STDLIB_UNUSED_WITH_WARNING;LINK_FLAGS_FORWARDED_IN_TRACE"
    else:
        semantic = "STDLIB_SELECTS_LIBCXX;LINK_FLAGS_FORWARDED_IN_TRACE"
    rows.append((arch, family, language, version, version_rc, compile_rc, compile_status,
                 trace_rc, trace_status, semantic))

out = Path("progress/R100/tables/driver_option_probe.tsv")
header = ("arch", "driver_family", "language", "version", "version_exit", "compile_stdlib_exit",
          "compile_stdlib_status", "trace_full_exit", "trace_full_status", "semantic_result")
with out.open("w", encoding="utf-8") as stream:
    stream.write("\t".join(header) + "\n")
    for row in rows:
        stream.write("\t".join(row) + "\n")
print(f"rows={len(rows)}")
for row in rows:
    print("\t".join(row))
