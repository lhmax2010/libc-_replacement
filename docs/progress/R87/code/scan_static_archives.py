#!/usr/bin/env python3
"""Scan static archives from the R11 package corpus for R87 symbol candidates."""

from __future__ import annotations

import argparse
import csv
import importlib.util
import json
import subprocess
from pathlib import Path


def load_classifier(path: Path):
    spec = importlib.util.spec_from_file_location("r87_elf", path)
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(module)
    return module.classify, module.demangle


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--paths", required=True, type=Path)
    parser.add_argument("--shards", required=True, type=Path)
    parser.add_argument("--classifier", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    parser.add_argument("--batch-size", type=int, default=20)
    args = parser.parse_args()

    classify, demangle = load_classifier(args.classifier)
    paths = [Path(line) for line in args.paths.read_text().splitlines() if line]
    metadata: dict[str, dict] = {}
    rows: list[tuple[str, str, str, str, str, str, str]] = []
    failures: list[tuple[str, str]] = []
    warnings: list[tuple[str, str]] = []
    symbols_scanned = 0
    total_bytes = 0
    for path in paths:
        try:
            total_bytes += path.stat().st_size
        except OSError as error:
            failures.append((str(path), f"STAT:{error}"))

    for start in range(0, len(paths), args.batch_size):
        batch = paths[start:start + args.batch_size]
        try:
            proc = subprocess.run(
                ["nm", "-A", "--no-demangle", "--", *(str(path) for path in batch)],
                text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=180,
            )
        except subprocess.TimeoutExpired:
            failures.append((f"BATCH:{start}", "NM_TIMEOUT_180S"))
            continue
        diagnostic_lines = proc.stderr.strip().splitlines()
        serious = [line for line in diagnostic_lines if "no symbols" not in line]
        target = str(batch[0]) if len(batch) == 1 else f"BATCH:{start}"
        if serious:
            failures.append((target, f"NM_EXIT_{proc.returncode}:" + "\n".join(serious)))
        elif proc.returncode != 0 and diagnostic_lines:
            warnings.append((target, f"NM_EXIT_{proc.returncode}:NO_SYMBOLS_ONLY"))
        elif proc.returncode != 0:
            failures.append((target, f"NM_EXIT_{proc.returncode}:NO_DIAGNOSTIC"))
        for line in proc.stdout.splitlines():
            fields = line.rsplit(maxsplit=1)
            if len(fields) != 2 or not fields[-1].startswith("_Z"):
                continue
            symbols_scanned += 1
            name = fields[-1]
            kinds = classify(name)
            if not kinds:
                continue
            archive_text = line.split(":", 1)[0]
            archive = Path(archive_text)
            parts = archive.parts
            try:
                sha_index = parts.index("extracted") + 2
                sha = parts[sha_index]
            except (ValueError, IndexError):
                sha = "NOT_AVAILABLE"
            if sha not in metadata:
                shard = args.shards / f"{sha}.json"
                try:
                    metadata[sha] = json.loads(shard.read_text())["package"]
                except (OSError, ValueError, KeyError):
                    metadata[sha] = {}
            package = metadata[sha]
            for kind in kinds:
                rows.append((
                    package.get("repo_id", "NOT_AVAILABLE"),
                    package.get("name", "NOT_AVAILABLE"),
                    package.get("arch", "NOT_AVAILABLE"),
                    sha,
                    str(archive), kind, name,
                ))

    names = sorted({row[-1] for row in rows})
    demangled = demangle(names)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("repo", "package", "arch", "rpm_sha256", "archive_path",
                         "classification", "raw_symbol", "demangled_symbol"))
        for row in sorted(rows):
            writer.writerow((*row, demangled.get(row[-1], "NOT_AVAILABLE")))
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("metric", "value"))
        writer.writerow(("archive_paths", len(paths)))
        writer.writerow(("archive_total_bytes", total_bytes))
        writer.writerow(("mangled_symbols_scanned", symbols_scanned))
        writer.writerow(("candidate_rows", len(rows)))
        for kind in (
            "SITE_WITH_NOEXCEPT_TYPE_ENCODING",
            "SITE_WITH_POINTER_NOEXCEPT_ENCODING",
            "CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE",
            "WBUFFER_CONVERT_NOEXCEPT_FUNCTION_TYPE",
            "WBUFFER_CONVERT_BOOL_TEMPLATE_CANDIDATE",
        ):
            writer.writerow((kind, sum(1 for row in rows if row[-2] == kind)))
        writer.writerow(("scan_failures", len(failures)))
        for path, reason in failures:
            writer.writerow((f"FAILURE:{path}", reason))
        writer.writerow(("nonfatal_no_symbols_warnings", len(warnings)))
        for path, reason in warnings:
            writer.writerow((f"WARNING:{path}", reason))

    print(f"archive_paths={len(paths)}")
    print(f"archive_total_bytes={total_bytes}")
    print(f"mangled_symbols_scanned={symbols_scanned}")
    print(f"candidate_rows={len(rows)}")
    print(f"scan_failures={len(failures)}")
    print(f"nonfatal_no_symbols_warnings={len(warnings)}")
    return 0 if not failures else 2


if __name__ == "__main__":
    raise SystemExit(main())
