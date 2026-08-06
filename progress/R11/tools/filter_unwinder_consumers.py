#!/usr/bin/env python3
import argparse
import csv


def classify(soname):
    if soname == "libgcc_s.so.1": return "LIBGCC_S_DIRECT"
    if soname == "libunwind.so.8": return "NONGNU_LIBUNWIND_GENERIC_DIRECT"
    if soname == "libunwind.so.1": return "LLVM_LIBUNWIND_DIRECT"
    if soname.startswith("libunwind") and ".so" in soname: return "OTHER_LIBUNWIND_COMPONENT_DIRECT"
    return ""


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--needed", required=True); ap.add_argument("--output", required=True)
    args = ap.parse_args()
    with open(args.needed, encoding="utf-8") as stream: rows = list(csv.DictReader(stream, delimiter="\t"))
    output = []
    for row in rows:
        category = classify(row["soname"])
        if category: output.append({**row, "edge_class": category})
    fields = [
        "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256",
        "path", "soname", "edge_class",
    ]
    with open(args.output, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(output)
    return 0


if __name__ == "__main__": raise SystemExit(main())
