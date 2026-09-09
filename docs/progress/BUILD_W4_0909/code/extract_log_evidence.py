#!/usr/bin/env python3
"""Extract bounded, credential-free evidence from ignored QuickBuild logs."""

from __future__ import annotations

import hashlib
import json
import re
import sys
from pathlib import Path


PACKAGES = [
    "lapack", "openblas", "onnxruntime", "yaca", "tidl",
    "noise-suppression", "tensorflow2", "openusd", "protobuf",
    "lottie-player", "libcynara-commons", "rive-tizen", "glog",
    "c-mock", "docker-launcher", "hdf5", "hailo-rt",
]

PATTERNS = {
    "lapack": r"gfortran: error: unrecognized command-line option '-stdlib=libc\+\+'",
    "openblas": r"gcc: error: unrecognized command-line option '-stdlib=libc\+\+'",
    "onnxruntime": r"gcc: error: unrecognized command-line option '-stdlib=libc\+\+'",
    "yaca": r"gcc: error: unrecognized command-line option '-stdlib=libc\+\+'",
    "tidl": r"cannot find -lc\+\+",
    "noise-suppression": r"no member named 'copy' in namespace 'std'",
    "tensorflow2": r"undefined reference to `std::__cxx11::basic_string",
    "openusd": r"unknown type name 'size_t'; did you mean 'std::size_t'",
    "protobuf": r"undefined reference to `absl::.*std::__1",
    "lottie-player": r"\.\./format:1:1: error: unknown type name 'git'",
    "libcynara-commons": r"ignoring return value of function declared with 'nodiscard'",
    "rive-tizen": r"undefined reference to `std::__(?:cxx11|throw_)",
    "glog": r"error: use of undeclared identifier 'auto_ptr'",
    "c-mock": r"undefined reference to `testing::.*std::__cxx11",
    "docker-launcher": r"undefined reference to `Json::Value::operator\[\]\(std::__1",
    "hdf5": r"configure: error: Fortran compiler requires either intrinsic functions SIZEOF or STORAGE_SIZE",
    "hailo-rt": r"implicit instantiation of undefined template 'std::char_traits<fmt::internal::char8_type>'",
}


def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: extract_log_evidence.py MANIFEST OUTPUT_DIR", file=sys.stderr)
        return 2
    manifest_path = Path(sys.argv[1])
    output_dir = Path(sys.argv[2])
    output_dir.mkdir(parents=True, exist_ok=True)
    data = json.loads(manifest_path.read_text(encoding="utf-8"))
    records = [
        record for record in data["records"]
        if record.get("package_hint") in PACKAGES
        and record.get("kind") == "package_log"
        and record.get("status") == "failed"
    ]
    records.sort(key=lambda r: (r["package_hint"], r["architecture"], r["path"]))

    inventory = output_dir / "log_inventory.tsv"
    with inventory.open("w", encoding="utf-8") as stream:
        stream.write("package\tarchitecture\tstatus\tfilename\tsize\tsha256\n")
        for record in records:
            path = Path(record["path"])
            stream.write(
                f"{record['package_hint']}\t{record['architecture']}\tfailed\t"
                f"{path.name}\t{path.stat().st_size}\t{sha256(path)}\n"
            )

    evidence = output_dir / "log_excerpts.md"
    with evidence.open("w", encoding="utf-8") as stream:
        stream.write("# QuickBuild 1165447 失败日志关键摘录\n\n")
        stream.write(
            "以下仅摘录每个可得架构的首个直接失败证据；完整日志位于被 "
            "`.gitignore` 排除的 `logs/quickbuild/build_1165447/`，未提交。\n\n"
        )
        for package in PACKAGES:
            stream.write(f"## `{package}`\n\n")
            package_records = [r for r in records if r["package_hint"] == package]
            seen = set()
            for record in package_records:
                arch = record["architecture"]
                if arch in seen:
                    continue
                seen.add(arch)
                path = Path(record["path"])
                lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
                regex = re.compile(PATTERNS[package])
                hit = next(((index, line) for index, line in enumerate(lines, 1) if regex.search(line)), None)
                stream.write(f"- 来源：`{path.name}`；SHA256 `{sha256(path)}`\n")
                if hit is None:
                    stream.write("- 直接证据：`NOT_OBSERVED`（预定模式未命中）\n\n")
                else:
                    line_no, line = hit
                    stream.write(f"- 第 {line_no} 行：\n\n```text\n{line}\n```\n\n")
            if not package_records:
                stream.write("- `NOT_AVAILABLE`：该包日志未下载。\n\n")

    print(f"build_id={data['build_id']}")
    print(f"selected_records={len(records)}")
    print(f"selected_packages={len(set(r['package_hint'] for r in records))}")
    print(f"inventory={inventory}")
    print(f"evidence={evidence}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
