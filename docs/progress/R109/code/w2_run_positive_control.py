#!/usr/bin/env python3
import argparse
import csv
import shutil
from pathlib import Path

from w2_scan_sources import extract_archive, scan_tree


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--archive", required=True, type=Path)
    parser.add_argument("--work", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    shutil.rmtree(args.work, ignore_errors=True)
    result = extract_archive(args.archive, args.work)
    ok, detail, members, regular, skipped, missing = result
    files, hits = scan_tree(args.work, "POSITIVE_CONTROL", "POSITIVE_CONTROL")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        fields = ("source_rpm", "package_name", "path", "line", "source_text")
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(hits)
    print(f"archive_ok={ok}")
    print(f"archive_detail={detail}")
    print(f"members={members} regular={regular} skipped={skipped} missing={missing}")
    print(f"files_scanned={files} hits={len(hits)}")
    shutil.rmtree(args.work, ignore_errors=True)
    return 0 if ok and skipped == 0 and missing == 0 and len(hits) == 1 else 2


if __name__ == "__main__":
    raise SystemExit(main())
