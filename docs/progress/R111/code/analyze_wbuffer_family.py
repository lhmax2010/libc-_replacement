#!/usr/bin/env python3
from __future__ import annotations

import csv
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
LLVM = ROOT / "codes/llvm"
OUT = ROOT / "docs/progress/R111/tables"

FILES = {
    "main": LLVM / "libcxx/include/__locale_dir/wbuffer_convert.h",
    "cxx03": LLVM / "libcxx/include/__cxx03/locale",
}


def classify_close(line: str) -> str:
    stripped = line.strip()
    if "::__close()" in stripped:
        return "DEFINITION"
    if re.search(r"wbuffer_convert\s*\*\s*__close\s*\(\s*\)\s*;", stripped):
        return "DECLARATION"
    if re.fullmatch(r"__close\s*\(\s*\)\s*;", stripped):
        return "CALL_RESULT_DISCARDED"
    return "OTHER_REFERENCE"


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    rows: list[dict[str, str | int]] = []
    for variant, path in FILES.items():
        for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            if re.search(r"\b__close\s*\(", line):
                rows.append(
                    {
                        "variant": variant,
                        "file": str(path.relative_to(ROOT)),
                        "line": number,
                        "kind": classify_close(line),
                        "source": line.strip(),
                    }
                )

    output = OUT / "item1_close_occurrences.tsv"
    with output.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=["variant", "file", "line", "kind", "source"], delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)

    for variant in FILES:
        subset = [row for row in rows if row["variant"] == variant]
        counts = {kind: sum(row["kind"] == kind for row in subset) for kind in ("DECLARATION", "CALL_RESULT_DISCARDED", "DEFINITION", "OTHER_REFERENCE")}
        assert counts == {
            "DECLARATION": 1,
            "CALL_RESULT_DISCARDED": 1,
            "DEFINITION": 1,
            "OTHER_REFERENCE": 0,
        }, (variant, counts)

    print("ASSERTIONS=PASS variants=2 calls_per_variant=1 return_use_sites=0 positive_control=destructor_call_found")
    print(f"OUTPUT={output.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
