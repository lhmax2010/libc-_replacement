#!/usr/bin/env python3
import csv
import re
from pathlib import Path

ROOT = Path("/home/toolchain/development/libc++_replacement/progress/R98/inputs/buildlogs")
OUTPUT = Path(__file__).resolve().parents[4] / "docs/progress/R112/tables/item1_fno_exceptions_buildlog_sample.tsv"
COMPILER = re.compile(r"(?:clang\+\+|g\+\+|c\+\+)(?:\s|$)")

def collect(flag: str):
    rows = []
    for path in sorted(ROOT.rglob("*.buildlog.txt")):
        with path.open(errors="replace") as stream:
            for number, line in enumerate(stream, 1):
                if number > 10000:
                    break
                if flag in line and COMPILER.search(line) and "checking " not in line:
                    rows.append((path, number, line.rstrip("\n")))
    return rows

def main() -> int:
    negative = collect("-fno-exceptions")
    positive = collect("-fexceptions")
    rows = []
    for path, number, line in negative:
        rel = path.relative_to(ROOT)
        rows.append({"side": rel.parts[0], "arch": rel.parts[1],
                     "package": path.name.removesuffix(".buildlog.txt"),
                     "line": number, "command_prefix": line[:500]})
    with OUTPUT.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    keys = {(row["side"], row["arch"], row["package"]) for row in rows}
    packages = {row["package"] for row in rows}
    print("sample_buildlogs", len(list(ROOT.rglob("*.buildlog.txt"))), "per_file_line_cap", 10000)
    print("fno_compile_command_lines", len(rows), "side_arch_package_cells", len(keys),
          "packages", len(packages), sorted(packages))
    print("positive_control_fexceptions_compile_command_lines", len(positive),
          "packages", len({p.name.removesuffix('.buildlog.txt') for p, _, _ in positive}))
    assert positive
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
