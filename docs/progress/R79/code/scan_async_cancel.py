#!/usr/bin/env python3
import argparse
import csv
import os
import re
from pathlib import Path


PATTERNS = [
    ("SET_CANCEL_TYPE", re.compile(r"\bpthread_setcanceltype\b")),
    ("ASYNC_CONSTANT", re.compile(r"\bPTHREAD_CANCEL_ASYNCHRONOUS\b")),
    ("DEFERRED_CONSTANT", re.compile(r"\bPTHREAD_CANCEL_DEFERRED\b")),
    ("SET_CANCEL_STATE", re.compile(r"\bpthread_setcancelstate\b")),
    ("PTHREAD_CANCEL_CALL", re.compile(r"\bpthread_cancel\s*\(")),
    ("DYNAMIC_NAME", re.compile(r"[\"']pthread_setcanceltype[\"']")),
]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--extraction-results", type=Path, required=True)
    ap.add_argument("--extracted-root", type=Path, required=True)
    ap.add_argument("--hits", type=Path, required=True)
    ap.add_argument("--packages", type=Path, required=True)
    args = ap.parse_args()

    with args.extraction_results.open(newline="") as stream:
        packages = [
            row["source_rpm"]
            for row in csv.DictReader(stream, delimiter="\t")
            if row["status"] == "EXTRACTED_OK"
        ]
    args.packages.write_text("\n".join(packages) + "\n")

    rows = []
    readable_files = 0
    binary_files = 0
    unreadable_files = 0
    missing_package_dirs = []
    for package in packages:
        package_root = args.extracted_root / package
        if not package_root.is_dir():
            missing_package_dirs.append(package)
            continue
        for directory, subdirs, filenames in os.walk(package_root):
            subdirs[:] = sorted(d for d in subdirs if d != ".git")
            for filename in sorted(filenames):
                path = Path(directory) / filename
                try:
                    data = path.read_bytes()
                except OSError:
                    unreadable_files += 1
                    continue
                if b"\0" in data[:8192]:
                    binary_files += 1
                    continue
                readable_files += 1
                text = data.decode("utf-8", errors="replace")
                for line_number, line in enumerate(text.splitlines(), 1):
                    categories = [name for name, pattern in PATTERNS if pattern.search(line)]
                    if categories:
                        rows.append(
                            (
                                package,
                                path.relative_to(package_root).as_posix(),
                                line_number,
                                ",".join(categories),
                                line.replace("\t", "\\t").replace("\r", "\\r"),
                            )
                        )

    with args.hits.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("package", "path", "line", "categories", "source_text"))
        writer.writerows(rows)

    print(f"package_denominator={len(packages)}")
    print(f"missing_package_dirs={len(missing_package_dirs)}")
    for package in missing_package_dirs:
        print(f"missing_package_dir={package}")
    print(f"readable_text_files={readable_files}")
    print(f"binary_files_skipped={binary_files}")
    print(f"unreadable_files={unreadable_files}")
    print(f"raw_hit_lines={len(rows)}")
    print(f"hit_packages={len({row[0] for row in rows})}")
    return 2 if missing_package_dirs or unreadable_files else 0


if __name__ == "__main__":
    raise SystemExit(main())
