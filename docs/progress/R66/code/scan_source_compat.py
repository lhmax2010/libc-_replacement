#!/usr/bin/env python3
"""Scan the frozen extracted-source corpus for implementation dependencies."""

from __future__ import annotations

import argparse
import csv
import pathlib
import re
import json
import subprocess
import sys


SOURCE_SUFFIXES = {
    "", ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx",
    ".tcc", ".inc", ".ipp", ".m", ".mm", ".spec", ".cmake", ".mk",
}

PATTERNS = [
    ("LIBSTDCXX_INTERNAL_HEADER", re.compile(r"#\s*include\s*[<\"]((?:bits|ext|backward|debug|parallel)/[^>\"]+)[>\"]")),
    ("GNU_EXTENSION_NAMESPACE", re.compile(r"\b(__gnu_cxx|__gnu_pbds|__gnu_parallel)\b")),
    ("LIBSTDCXX_ABI_NAMESPACE", re.compile(r"\bstd::(__cxx11|__detail|__debug|__parallel)\b")),
    ("LIBSTDCXX_CONFIG_MACRO", re.compile(r"\b(__GLIBCXX__|_GLIBCXX_[A-Za-z0-9_]+)\b")),
    ("CXX_ABI_RUNTIME_API", re.compile(r"(?:#\s*include\s*[<\"](cxxabi\.h)[>\"]|\b(__cxxabiv1|abi)::__[A-Za-z0-9_]+)")),
    ("STD_PRIVATE_MEMBER", re.compile(r"\bstd(?:::[A-Za-z][A-Za-z0-9_]*){1,4}::(__[A-Za-z0-9_]+|_[A-Z][A-Za-z0-9_]*)\b")),
    ("EXPLICIT_LIBSTDCXX_LINK", re.compile(r"(?:^|[\s,;])(-lstdc\+\+|-stdlib\s*=\s*libstdc\+\+)(?:$|[\s,;])")),
]

RG_DISCOVERY = r"(?:#\s*include\s*[<\"](?:bits|ext|backward|debug|parallel)/|__gnu_(?:cxx|pbds|parallel)|std::__(?:cxx11|detail|debug|parallel)|__GLIBCXX__|_GLIBCXX_[A-Za-z0-9_]+|cxxabi\.h|(?:__cxxabiv1|abi)::__[A-Za-z0-9_]+|std(?:::[A-Za-z][A-Za-z0-9_]*){1,4}::(?:__[A-Za-z0-9_]+|_[A-Z][A-Za-z0-9_]*)|-lstdc\+\+|-stdlib\s*=\s*libstdc\+\+)"


def scope_for(relative: str) -> str:
    lowered = relative.lower()
    if any(part in lowered for part in ("/test/", "/tests/", "/testing/", "unittest", "gtest", "benchmark")):
        return "TEST"
    if any(part in lowered for part in ("third_party", "third-party", "external/", "vendor/", "deps/")):
        return "BUNDLED_OR_VENDOR"
    if "libstdc++" in lowered or "/gcc-" in lowered or "/libcxx/" in lowered:
        return "BUNDLED_TOOLCHAIN"
    return "PRODUCTION_LIKE"


def load_ok(path: pathlib.Path):
    result = []
    with path.open(newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["status"] == "EXTRACTED_OK":
                result.append(row["source_rpm"])
    return result


def write_row(stream, values) -> None:
    cleaned = []
    for value in values:
        text = str(value).replace("\\", "\\\\").replace("\t", "\\t").replace("\r", "\\r").replace("\n", "\\n")
        cleaned.append(text)
    stream.write("\t".join(cleaned) + "\n")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", required=True)
    parser.add_argument("--extraction-results", required=True)
    parser.add_argument("--known", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--summary", required=True)
    parser.add_argument("--tokens", required=True)
    parser.add_argument("--peer", required=True)
    args = parser.parse_args()

    corpus = pathlib.Path(args.corpus)
    known = set(pathlib.Path(args.known).read_text().splitlines())
    packages = load_ok(pathlib.Path(args.extraction_results))
    peer_text = "\n".join(
        p.read_text(errors="replace") for p in pathlib.Path(args.peer).rglob("*")
        if p.is_file() and p.suffix.lower() in SOURCE_SUFFIXES
    )
    counts = {}
    token_packages = {}
    token_hits = {}
    with pathlib.Path(args.output).open("w", newline="") as out:
        write_row(out, ("source_rpm", "known45", "scope", "family", "token", "file", "line", "text"))
        ok_packages = set(packages)
        rg_command = [
            "rg", "--json", "--no-messages", "--no-ignore", "--threads", "2",
            "--glob", "*.{c,cc,cpp,cxx,h,hh,hpp,hxx,tcc,inc,ipp,m,mm,spec,cmake,mk,in}",
            "--glob", "CMakeLists.txt", "--glob", "Makefile*", "--glob", "configure*",
            "--glob", "**/include/**", RG_DISCOVERY, str(corpus)
        ]
        process = subprocess.Popen(
            rg_command,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, errors="replace"
        )
        assert process.stdout is not None
        for raw in process.stdout:
            event = json.loads(raw)
            if event.get("type") != "match":
                continue
            data = event["data"]
            path = pathlib.Path(data["path"]["text"])
            relative_to_corpus = path.relative_to(corpus)
            package = relative_to_corpus.parts[0]
            if package not in ok_packages:
                continue
            relative = str(pathlib.Path(*relative_to_corpus.parts[1:]))
            line_no = data["line_number"]
            line = data["lines"]["text"].rstrip("\r\n")
            scope = scope_for("/" + relative)
            for family, pattern in PATTERNS:
                for match in pattern.finditer(line):
                    token = next((g for g in match.groups() if g), match.group(0)).strip()
                    write_row(out, (package, "YES" if package in known else "NO", scope, family, token, relative, line_no, line.strip()[:1000]))
                    key = (family, scope, package)
                    counts[key] = counts.get(key, 0) + 1
                    token_key = (family, token)
                    token_packages.setdefault(token_key, set()).add(package)
                    token_hits[token_key] = token_hits.get(token_key, 0) + 1
        stderr = process.stderr.read() if process.stderr is not None else ""
        rc = process.wait()
        if rc not in (0, 1):
            print(stderr, file=sys.stderr)
            return rc
    with pathlib.Path(args.summary).open("w", newline="") as out:
        write_row(out, ("family", "scope", "package_count", "hit_count", "new_outside_known45_packages"))
        for family, scope in sorted({(k[0], k[1]) for k in counts}):
            selected = {k[2]: v for k, v in counts.items() if k[0] == family and k[1] == scope}
            new = sum(1 for package in selected if package not in known)
            write_row(out, (family, scope, len(selected), sum(selected.values()), new))
    with pathlib.Path(args.tokens).open("w", newline="") as out:
        write_row(out, ("family", "token", "package_count", "hit_count", "exact_token_in_libcxx", "packages"))
        for key in sorted(token_packages):
            family, token = key
            packages_for_token = sorted(token_packages[key])
            write_row(out, (family, token, len(packages_for_token), token_hits[key], "YES" if token in peer_text else "NO", ";".join(packages_for_token)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
