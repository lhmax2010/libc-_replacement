#!/usr/bin/env python3
import argparse
import csv
import re
from pathlib import Path


PATTERN = re.compile(r"xla_runtime_objects|InlinerSizeModel\.o|RegAllocEvictModel\.o")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    rows = []
    for spec in args.root.rglob("*.spec"):
        if not spec.is_file():
            continue
        for number, line in enumerate(spec.read_text(errors="replace").splitlines(), 1):
            if PATTERN.search(line):
                rows.append(("BUILD_RULE", str(spec), number, line.strip()))
    linked = []
    for path in args.root.rglob("*.o"):
        if not path.is_file():
            continue
        text = str(path).replace("\\", "/")
        if "/xla_runtime_objects/" in text or path.name in {
                "InlinerSizeModel.o", "RegAllocEvictModel.o"}:
            linked.append(path)
            rows.append(("LINKED_OBJECT", str(path), "NOT_APPLICABLE", path.name))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("kind", "path", "line", "evidence"))
        writer.writerows(rows)
    print(f"build_rule_rows={sum(row[0] == 'BUILD_RULE' for row in rows)}")
    print(f"linked_object_files={len(linked)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
