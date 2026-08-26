#!/usr/bin/env python3
"""Inventory implementation-level defensive and platform-coupling constructs."""

from __future__ import annotations

import argparse
import csv
import pathlib
import re
import sys


SUFFIXES = {"", ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".tcc", ".inc"}
RULES = [
    ("NO_EXCEPTIONS_COMPAT", re.compile(r"\b(?:_GLIBCXX|_LIBCPP)[A-Za-z0-9_]*(?:EXCEPT|THROW|CATCH|TRY)[A-Za-z0-9_]*\b|\b__(?:try|catch|throw_exception_again)\b")),
    ("CANCELLATION_OR_THREAD_EXIT", re.compile(r"\b(?:pthread_(?:cancel|exit|testcancel|setcancelstate|setcanceltype)|_UA_FORCE_UNWIND|__forced_unwind|__foreign_exception)\b")),
    ("ALLOCATION_RECOVERY", re.compile(r"\b(?:bad_alloc|new_handler|nothrow|fallback_malloc|emergency_(?:buffer|pool)|__cxa_(?:allocate|free)_exception)\b", re.I)),
    ("INCONSISTENT_STATE_FALLBACK", re.compile(r"\b(?:broken_promise|setstate|_M_setstate|__set_badbit|__set_failbit|exception_ptr|terminate)\b")),
    ("GLIBC_SPECIFIC_COUPLING", re.compile(r"\b(?:__libc_[A-Za-z0-9_]+|__glibc(?:_[A-Za-z0-9_]+)?|__GLIBC(?:_MINOR)?__|gnu_get_libc_version|malloc_usable_size|secure_getenv|GLIBC_[0-9A-Za-z_.]+)\b|#\s*include\s*[<\"](?:features|gnu/libc-version|malloc)\.h[>\"]")),
    ("LIBSTDCXX_SYMBOL_VERSIONING", re.compile(r"\bGLIBCXX_[0-9A-Za-z_.]+\b")),
]


def scope_for(rel: str) -> str:
    lowered = rel.lower()
    if lowered.startswith(("doc/", "changelog")):
        return "DOCUMENTATION"
    if lowered.startswith("testsuite/"):
        return "TEST"
    if "config/abi/" in lowered:
        return "ABI_METADATA"
    return "CODE"


def scan(label: str, root: pathlib.Path, writer, counts):
    for path in root.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in SUFFIXES:
            continue
        try:
            lines = path.read_text(errors="replace").splitlines()
        except OSError:
            continue
        rel = str(path.relative_to(root))
        scope = scope_for(rel)
        for line_no, line in enumerate(lines, 1):
            for category, pattern in RULES:
                for match in pattern.finditer(line):
                    token = match.group(0).strip()
                    writer.writerow((label, scope, category, token, rel, line_no, line.strip()[:1000]))
                    key = (label, scope, category)
                    counts[key] = counts.get(key, 0) + 1


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--libstdcxx", required=True)
    parser.add_argument("--libcxx", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--summary", required=True)
    args = parser.parse_args()
    counts = {}
    with pathlib.Path(args.output).open("w", newline="") as out:
        writer = csv.writer(out, delimiter="\t", lineterminator="\n")
        writer.writerow(("implementation", "scope", "category", "token", "file", "line", "text"))
        scan("libstdc++", pathlib.Path(args.libstdcxx), writer, counts)
        scan("libc++", pathlib.Path(args.libcxx), writer, counts)
    with pathlib.Path(args.summary).open("w", newline="") as out:
        writer = csv.writer(out, delimiter="\t", lineterminator="\n")
        writer.writerow(("scope", "category", "libstdcxx_hits", "libcxx_hits"))
        for scope, category in sorted({(key[1], key[2]) for key in counts}):
            writer.writerow((scope, category, counts.get(("libstdc++", scope, category), 0), counts.get(("libc++", scope, category), 0)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
