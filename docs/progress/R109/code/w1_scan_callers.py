#!/usr/bin/env python3
import argparse
import csv
import re
from pathlib import Path

EXTENSIONS = {".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".ipp", ".tcc", ".inl"}
CALL = re.compile(r"\b([A-Za-z_]\w*)\s*(?:\.|->)\s*(wait|wait_for|wait_until)\s*\(\s*([A-Za-z_]\w*)")


def assess(lines, index, lock):
    before = "\n".join(lines[max(0, index - 35):index + 1])
    declarations = list(re.finditer(
        rf"(?:unique_lock|scoped_lock)\s*<[^>]+>\s+{re.escape(lock)}\s*\(([^;]*)\)", before))
    if not declarations:
        return "NOT_DETERMINABLE", "lock declaration not found in local window"
    args = declarations[-1].group(1)
    suffix = before[declarations[-1].end():]
    locks = list(re.finditer(rf"\b{re.escape(lock)}\s*\.\s*lock\s*\(", suffix))
    unlocks = list(re.finditer(rf"\b{re.escape(lock)}\s*\.\s*unlock\s*\(", suffix))
    if unlocks and (not locks or unlocks[-1].start() > locks[-1].start()):
        return "POSSIBLY_NOT_HELD", "latest local operation is unlock()"
    if locks:
        return "LIKELY_HELD", "local lock() occurs after construction"
    if "defer_lock" in args:
        return "POSSIBLY_NOT_HELD", "constructed with defer_lock and no local lock() found"
    if "adopt_lock" in args:
        return "NOT_DETERMINABLE", "adopt_lock depends on external ownership"
    return "LIKELY_HELD", "unique_lock constructed from mutex without defer_lock"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--facility-hits", required=True, type=Path)
    parser.add_argument("--source-root", required=True, type=Path)
    parser.add_argument("--control-root", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    packages = set()
    with args.facility_hits.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["facility"] == "condition_variable":
                packages.add(row["source_rpm"])
    roots = [args.source_root / package for package in sorted(packages)] + [args.control_root]
    rows = []
    for root in roots:
        if not root.is_dir():
            continue
        package = root.name if root != args.control_root else "POSITIVE_CONTROL"
        for path in root.rglob("*"):
            if not path.is_file() or path.suffix not in EXTENSIONS:
                continue
            try:
                text = path.read_text(errors="replace")
            except OSError:
                continue
            if "condition_variable" not in text or ".wait" not in text:
                continue
            lines = text.splitlines()
            for index, line in enumerate(lines):
                for match in CALL.finditer(line):
                    assessment, basis = assess(lines, index, match.group(3))
                    context = "\n".join(lines[max(0, index - 4):min(len(lines), index + 3)])
                    rows.append((package, str(path), index + 1, match.group(1),
                                 match.group(2), match.group(3), assessment, basis, context))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("source_rpm", "path", "line", "condition_object", "call",
                         "lock_expression", "ownership_assessment", "basis", "context"))
        writer.writerows(rows)
    for value in ("LIKELY_HELD", "POSSIBLY_NOT_HELD", "NOT_DETERMINABLE"):
        print(f"{value}={sum(row[6] == value for row in rows)}")
    print(f"TOTAL={len(rows)}")
    print(f"PACKAGES={len({row[0] for row in rows if row[0] != 'POSITIVE_CONTROL'})}")
    print(f"CONTROL={sum(row[0] == 'POSITIVE_CONTROL' for row in rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
