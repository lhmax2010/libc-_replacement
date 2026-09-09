#!/usr/bin/env python3
"""Summarize trace-only per-option probes and verify expected forwarding text."""

from __future__ import annotations

import re
import sys
from pathlib import Path


OPTIONS = {
    "stdlib": ("-stdlib=libc++", ("-lc++",)),
    "no_as_needed": ("-Wl,--no-as-needed", ("--no-as-needed",)),
    "cxxabi": ("-lc++abi", ("-lc++abi",)),
    "as_needed": ("-Wl,--as-needed", ("--as-needed",)),
    "full": ("all injected options", ("-lc++", "--no-as-needed", "-lc++abi", "--as-needed")),
}


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: summarize_trace_probe.py RAW OUTPUT", file=sys.stderr)
        return 2
    text = Path(sys.argv[1]).read_text(encoding="utf-8", errors="replace")
    rows = []
    for block in re.split(r"(?=^===== )", text, flags=re.M):
        head = re.match(r"^===== (\S+) (\S+) =====", block)
        if not head:
            continue
        arch, family = head.groups()
        version_result = re.search(r"^RESULT\[version\]\trc=(\d+)", block, re.M)
        version_rc = version_result.group(1) if version_result else "NOT_OBSERVED"
        version_hit = re.search(r"(?:gcc|g\+\+).*?\)\s+(14\.2\.0)|clang version\s+(22\.1\.8)", block)
        version = next((v for v in version_hit.groups() if v), "NOT_OBSERVED") if version_hit else "NOT_OBSERVED"
        full_output = re.search(r"^OUTPUT\[full\]_BEGIN\n(.*?)\nOUTPUT\[full\]_END", block, re.M | re.S)
        full_body = full_output.group(1) if full_output else ""
        for label, (option, needles) in OPTIONS.items():
            result = re.search(rf"^RESULT\[{label}\]\trc=(\d+)", block, re.M)
            out = re.search(rf"^OUTPUT\[{label}\]_BEGIN\n(.*?)\nOUTPUT\[{label}\]_END", block, re.M | re.S)
            rc = result.group(1) if result else "NOT_OBSERVED"
            body = out.group(1) if out else ""
            if version_rc == "139":
                semantic = "NOT_OBSERVED_DRIVER_RUNTIME_EXIT_139"
            elif label in ("stdlib", "full") and family == "gcc":
                semantic = "REJECTED_AT_STDLIB_OPTION" if rc == "1" and "unrecognized command-line option" in body else "UNEXPECTED"
            elif label == "stdlib" and family == "clang":
                semantic = "ACCEPTED_AND_SELECTS_LIBCXX" if rc == "0" and "-lc++" in full_body else "UNEXPECTED"
            elif label == "full" and family == "clang":
                semantic = "ACCEPTED_ALL_FORWARDED" if rc == "0" and all(n in body for n in needles) else "UNEXPECTED"
            else:
                semantic = "ACCEPTED_AND_FORWARDED" if rc == "0" and all(n in body for n in needles) else "UNEXPECTED"
            rows.append((arch, family, version, option, rc, semantic))
    out_path = Path(sys.argv[2])
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", encoding="utf-8") as stream:
        stream.write("arch\tdriver_family\tversion\toption\texit_code\tsemantic_result\n")
        for row in rows:
            stream.write("\t".join(row) + "\n")
    print(f"rows={len(rows)}")
    print(f"unexpected={sum(row[-1] == 'UNEXPECTED' for row in rows)}")
    print(f"not_observed={sum(row[-1].startswith('NOT_OBSERVED') for row in rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
