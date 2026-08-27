#!/usr/bin/env python3
"""Verify that all four R68 test runtimes match their intended source tree."""

from __future__ import annotations

import hashlib
import subprocess
from pathlib import Path


WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
BASELINE = WORKSPACE / "tmp/R68/source-baseline"
PATCHED = WORKSPACE / "tmp/R69/replay"
BASELINE_HEAD = "5ed6c77278dfa7a470667cf1a137723d3c96fe60"
PATCHED_HEAD = "ae3471c26c66489439354bb1e89df6af8b56da3e"
IDENTITY_TSV = WORKSPACE / "docs/progress/R69/code/evidence/build/baseline_and_final_sha256.tsv"

BUILDS = {
    "patched_x86_64": (WORKSPACE / "tmp/R68/build-patched-x86_64", PATCHED, "x86_64", True),
    "baseline_x86_64": (WORKSPACE / "tmp/R68/build-baseline-x86_64", BASELINE, "x86_64", False),
    "patched_armv7l": (WORKSPACE / "tmp/R68/build-patched-armv7l", PATCHED, "armv7l", True),
    "baseline_armv7l": (WORKSPACE / "tmp/R68/build-baseline-armv7l", BASELINE, "armv7l", False),
}

INSTALLED_HEADERS = {
    "libcxx/include/__ostream/basic_ostream.h": "include/c++/v1/__ostream/basic_ostream.h",
    "libcxx/include/future": "include/c++/v1/future",
    "libcxx/include/istream": "include/c++/v1/istream",
    "libcxx/include/string": "include/c++/v1/string",
    "libcxxabi/include/cxxabi.h": "include/c++/v1/cxxabi.h",
}


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def command(*arguments: str, cwd: Path | None = None) -> str:
    return subprocess.check_output(arguments, cwd=cwd, text=True).strip()


def verify_tree(tree: Path, expected_head: str) -> None:
    head = command("git", "rev-parse", "HEAD", cwd=tree)
    status = command("git", "status", "--porcelain", cwd=tree)
    if head != expected_head:
        raise RuntimeError(f"wrong HEAD for {tree}: {head}")
    if status:
        raise RuntimeError(f"dirty source tree {tree}: {status}")
    print(f"SOURCE_TREE\t{tree}\tHEAD={head}\tCLEAN=yes")


def load_expected_hashes() -> dict[str, tuple[str, str]]:
    expected: dict[str, tuple[str, str]] = {}
    lines = IDENTITY_TSV.read_text(encoding="utf-8").splitlines()
    for line in lines[1:]:
        path, baseline_hash, final_hash = line.split("\t")
        expected[path] = (baseline_hash, final_hash)
    return expected


def verify_source_hashes() -> None:
    expected = load_expected_hashes()
    for relative, (baseline_hash, final_hash) in expected.items():
        observed_baseline = sha256(BASELINE / relative)
        observed_final = sha256(PATCHED / relative)
        if observed_baseline != baseline_hash:
            raise RuntimeError(f"baseline source hash mismatch: {relative}")
        if observed_final != final_hash:
            raise RuntimeError(f"patched source hash mismatch: {relative}")
        print(
            f"SOURCE_FILE\t{relative}\tbaseline={observed_baseline}"
            f"\tpatched={observed_final}\tR69_MATCH=yes"
        )


def verify_build(name: str, build: Path, source: Path, arch: str, patched: bool) -> tuple[str, str]:
    cache = (build / "CMakeCache.txt").read_text(encoding="utf-8")
    expected_home = f"CMAKE_HOME_DIRECTORY:INTERNAL={source / 'runtimes'}"
    if expected_home not in cache:
        raise RuntimeError(f"CMake source mismatch for {name}")

    install = build / "libcxx/test-suite-install"
    for source_relative, installed_relative in INSTALLED_HEADERS.items():
        source_file = source / source_relative
        installed_file = install / installed_relative
        if source_file.read_bytes() != installed_file.read_bytes():
            raise RuntimeError(f"installed header mismatch for {name}: {source_relative}")
        print(
            f"HEADER\t{name}\t{source_relative}\tsha256={sha256(installed_file)}"
            "\tSOURCE_MATCH=yes"
        )

    libcxx = install / "lib/libc++.so.1.0"
    libcxxabi = install / "lib/libc++abi.so.1.0"
    file_output = command("file", str(libcxxabi))
    expected_fragment = "x86-64" if arch == "x86_64" else "ARM"
    if expected_fragment not in file_output:
        raise RuntimeError(f"architecture mismatch for {name}: {file_output}")

    symbols = command("readelf", "-Ws", str(libcxxabi))
    marker = "_ZTIN10__cxxabiv115__forced_unwindE"
    resume = "_Unwind_Resume_or_Rethrow"
    if patched and (marker not in symbols or resume not in symbols):
        raise RuntimeError(f"patched libc++abi markers absent for {name}")
    if not patched and (marker in symbols or resume in symbols):
        raise RuntimeError(f"patched libc++abi markers unexpectedly present for {name}")

    libcxx_hash = sha256(libcxx)
    libcxxabi_hash = sha256(libcxxabi)
    print(
        f"ARTIFACT\t{name}\tarch={arch}\tvariant={'patched' if patched else 'baseline'}"
        f"\tlibc++={libcxx_hash}\tlibc++abi={libcxxabi_hash}"
        f"\tCMAKE_SOURCE={source}\tABI_MARKERS={'present' if patched else 'absent'}"
    )
    return libcxx_hash, libcxxabi_hash


def main() -> None:
    verify_tree(PATCHED, PATCHED_HEAD)
    verify_tree(BASELINE, BASELINE_HEAD)
    verify_source_hashes()

    results: dict[str, tuple[str, str]] = {}
    for name, (build, source, arch, patched) in BUILDS.items():
        results[name] = verify_build(name, build, source, arch, patched)

    for arch in ("x86_64", "armv7l"):
        if results[f"patched_{arch}"][0] == results[f"baseline_{arch}"][0]:
            raise RuntimeError(f"libc++ patched/baseline artifacts are identical on {arch}")
        if results[f"patched_{arch}"][1] == results[f"baseline_{arch}"][1]:
            raise RuntimeError(f"libc++abi patched/baseline artifacts are identical on {arch}")
        print(f"VARIANT_DIFFERENCE\t{arch}\tlibc++=different\tlibc++abi=different")

    print("RESULT\tPASS")


if __name__ == "__main__":
    main()
