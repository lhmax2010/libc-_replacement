#!/usr/bin/env python3
import argparse
import csv
import re
from collections import Counter
from pathlib import Path


DIRECT = re.compile(
    r"\bstd\s*::\s*condition_variable\s*(?:[*&]\s*)?([A-Za-z_]\w*)"
)
USING = re.compile(r"\busing\s+std\s*::\s*condition_variable\s*;")
UNQUALIFIED = re.compile(
    r"\bcondition_variable\s*(?:[*&]\s*)?([A-Za-z_]\w*)"
)
ALIAS = re.compile(
    r"\b(?:using\s+([A-Za-z_]\w*)\s*=\s*std\s*::\s*condition_variable|"
    r"typedef\s+std\s*::\s*condition_variable\s+([A-Za-z_]\w*))"
)


def declared_objects(text):
    names = set(DIRECT.findall(text))
    if USING.search(text):
        names.update(UNQUALIFIED.findall(text))
    aliases = {a or b for a, b in ALIAS.findall(text)}
    for alias in aliases:
        names.update(re.findall(
            rf"\b{re.escape(alias)}\s*(?:[*&]\s*)?([A-Za-z_]\w*)", text
        ))
    return names


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()

    with args.input.open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    cache = {}
    output = []
    for row in rows:
        path = Path(row["path"])
        if path not in cache:
            try:
                text = path.read_text(errors="replace")
            except OSError:
                text = ""
            cache[path] = declared_objects(text)
        if row["condition_object"] in cache[path]:
            kind = "DIRECT_STD_DECLARATION"
        else:
            kind = "UNRESOLVED_CANDIDATE"
        output.append({**row, "attribution": kind})

    fields = list(rows[0]) + ["attribution"] if rows else ["attribution"]
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(output)

    counter = Counter((r["attribution"], r["ownership_assessment"]) for r in output)
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("scope", "attribution", "ownership_assessment", "rows", "files", "source_rpms"))
        for scope, selected in (
            ("PLATFORM", [r for r in output if r["source_rpm"] != "POSITIVE_CONTROL"]),
            ("POSITIVE_CONTROL", [r for r in output if r["source_rpm"] == "POSITIVE_CONTROL"]),
        ):
            keys = sorted({(r["attribution"], r["ownership_assessment"]) for r in selected})
            for attribution, ownership in keys:
                subset = [r for r in selected if r["attribution"] == attribution and r["ownership_assessment"] == ownership]
                writer.writerow((scope, attribution, ownership, len(subset),
                                 len({r["path"] for r in subset}),
                                 len({r["source_rpm"] for r in subset})))
    print(f"ROWS={len(output)}")
    print(f"DIRECT={sum(r['attribution'] == 'DIRECT_STD_DECLARATION' for r in output)}")
    print(f"CONTROL_DIRECT={sum(r['source_rpm'] == 'POSITIVE_CONTROL' and r['attribution'] == 'DIRECT_STD_DECLARATION' for r in output)}")
    return 0 if any(r["source_rpm"] == "POSITIVE_CONTROL" and r["attribution"] == "DIRECT_STD_DECLARATION" for r in output) else 2


if __name__ == "__main__":
    raise SystemExit(main())
