#!/usr/bin/env python3
"""Enumerate libstdc++ forced-unwind guards and map them to libc++ areas."""

from __future__ import annotations

import csv
import re
import subprocess
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
GCC = ROOT / "codes/gcc/libstdc++-v3"
OUT = ROOT / "progress/R43b/tables"


def mapping(rel: str, line: int) -> tuple[str, str, str]:
    if "condition_variable" in rel:
        return (
            "condition_variable_any unlock guard",
            "codes/llvm/libcxx/include/condition_variable:150",
            "NO_CATCH; destructor is noexcept and directly relocks",
        )
    if rel.endswith("std/bitset"):
        return (
            "bitset stream extraction",
            "codes/llvm/libcxx/include/istream:1389",
            "CATCH_ALL; conditional rethrow via exceptions() mask",
        )
    if "experimental/executor" in rel:
        return (
            "experimental executor extension",
            "NO_DIRECT_LIBCXX_EQUIVALENT",
            "NO_DIRECT_EQUIVALENT_IN_LIBCXX",
        )
    if "future" in rel:
        return (
            "future/executor exception capture",
            "codes/llvm/libcxx/include/future:792,823,853,890,1664,1681,1764,1782,1865",
            "CATCH_ALL; stores current_exception or conditionally rethrows",
        )
    if "ext/vstring" in rel:
        return (
            "GNU vstring extension",
            "NO_DIRECT_LIBCXX_EQUIVALENT",
            "NO_DIRECT_EQUIVALENT_IN_LIBCXX",
        )
    if "basic_string" in rel and line > 800:
        return (
            "string stream extraction/getline",
            "codes/llvm/libcxx/include/istream:1255,1328",
            "CATCH_ALL; conditional rethrow via exceptions() mask",
        )
    if "basic_string" in rel or "cow_string" in rel:
        return (
            "string capacity/mutation operation",
            "codes/llvm/libcxx/include/string:3429,3466",
            "CATCH_ALL; shrink_to_fit path returns without rethrow",
        )
    if "iomanip" in rel:
        return (
            "iomanip stream helper",
            "codes/llvm/libcxx/include/iomanip:258,306,355,404; codes/llvm/libcxx/src/ios.cpp:362",
            "CATCH_ALL; helper rethrows only when exceptions() includes badbit",
        )
    if "ostream" in rel or "ostream_insert" in rel:
        return (
            "ostream formatted/unformatted output",
            "codes/llvm/libcxx/include/__ostream/basic_ostream.h:107,136,271,368,429,469,487,507; codes/llvm/libcxx/include/__ostream/put_character_sequence.h:48; codes/llvm/libcxx/src/ios.cpp:362",
            "CATCH_ALL; helper rethrows only when exceptions() includes badbit",
        )
    if "istream" in rel or "compatibility.cc" in rel:
        return (
            "istream formatted/unformatted input",
            "codes/llvm/libcxx/include/istream:377,468,519,594,644,679,717,765,808,862,889,915,954,982,1010,1042,1066,1091,1116,1149,1255,1328,1389; codes/llvm/libcxx/src/ios.cpp:362",
            "CATCH_ALL; mostly conditional rethrow via exceptions() mask; one path explicitly swallows",
        )
    if "dynamic_bitset" in rel:
        return (
            "TR2 extension",
            "NO_DIRECT_LIBCXX_EQUIVALENT",
            "NO_DIRECT_EQUIVALENT_IN_LIBCXX",
        )
    return ("unclassified", "NOT_AVAILABLE", "UNDETERMINED")


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    rows = []
    pattern = re.compile(r"(?:__catch|catch)\s*\(\s*(?:const\s+)?__cxxabiv1::__forced_unwind\s*&\s*\)")
    candidate_paths = list((GCC / "include").rglob("*")) + list((GCC / "src").rglob("*"))
    for path in sorted(candidate_paths):
        if not path.is_file():
            continue
        rel = path.relative_to(GCC).as_posix()
        if rel.startswith("doc/"):
            continue
        try:
            lines = path.read_text(errors="replace").splitlines()
        except OSError:
            continue
        for idx, line in enumerate(lines, 1):
            if not pattern.search(line):
                continue
            area, counterpart, behavior = mapping(rel, idx)
            context = " ".join(x.strip() for x in lines[max(0, idx - 2): min(len(lines), idx + 4)])
            rows.append({
                "libstdcxx_file": f"codes/gcc/libstdc++-v3/{rel}",
                "line": idx,
                "guard_source": line.strip(),
                "context": context,
                "facility": area,
                "libcxx_counterpart": counterpart,
                "libcxx_observed_form": behavior,
            })

    fields = list(rows[0]) if rows else []
    with (OUT / "libstdcxx_guards_65.tsv").open("w", newline="") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)

    per_file = Counter(row["libstdcxx_file"] for row in rows)
    with (OUT / "libstdcxx_guard_distribution.tsv").open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(["file", "guard_count"])
        for name, count in sorted(per_file.items()):
            w.writerow([name, count])

    hit_proc = subprocess.run(
        ["rg", "--no-heading", "--line-number", "__forced_unwind", str(GCC)],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    all_hits = []
    for raw in hit_proc.stdout.splitlines():
        path_s, line_s, source = raw.split(":", 2)
        rel = Path(path_s).relative_to(ROOT).as_posix()
        stripped = source.strip()
        if "/doc/" in f"/{rel}/":
            category = "DOCUMENTATION"
        elif re.search(r"(?:__catch|catch)\s*\(", source):
            category = "GUARD_CLAUSE"
        elif re.search(r"\b(?:class|struct)\s+__forced_unwind\b", source):
            category = "TYPE_DEFINITION_OR_DECLARATION"
        elif stripped.startswith(("//", "/*", "*")):
            category = "COMMENT"
        else:
            category = "OTHER_CODE_USE"
        all_hits.append([rel, int(line_s), category, stripped])
    with (OUT / "libstdcxx_all_forced_unwind_hits.tsv").open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(["file", "line", "classification", "source"])
        w.writerows(all_hits)

    print(f"CODE_GUARDS={len(rows)}")
    print(f"CODE_FILES={len(per_file)}")
    print("REVIEWER_EXPECTED_GUARDS=65")
    print("REVIEWER_EXPECTED_FILES=16")
    print(f"COUNT_MATCH={'YES' if len(rows) == 65 and len(per_file) == 16 else 'NO'}")
    print(f"ALL_TEXT_HITS={len(all_hits)}")
    print(f"ALL_HITS_RG_EXIT_CODE={hit_proc.returncode}")


if __name__ == "__main__":
    main()
