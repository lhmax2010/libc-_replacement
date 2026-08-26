#!/usr/bin/env python3
"""Calibrate generic incompatibility-discovery methods against frozen evidence.

The rules intentionally do not contain identifiers from the four known findings.
They operate on structural shapes: typed catch-and-rethrow guards, compiler/linker
diagnostics, and duplicate RTTI definitions whose dynamic visibility differs.
"""

from __future__ import annotations

import argparse
import collections
import pathlib
import re
import subprocess
import sys


TEXT_SUFFIXES = {
    "", ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".tcc", ".inc"
}


def iter_text(root: pathlib.Path):
    for path in root.rglob("*"):
        if not path.is_file() or path.suffix.lower() not in TEXT_SUFFIXES:
            continue
        try:
            yield path, path.read_text(errors="replace")
        except OSError:
            continue


def normalize_catch_type(raw: str) -> str:
    value = re.sub(r"\b(const|volatile)\b", "", raw)
    value = value.replace("&", " ").replace("*", " ")
    return re.sub(r"\s+", "", value)


def defensive(args: argparse.Namespace) -> int:
    left = pathlib.Path(args.left)
    right = pathlib.Path(args.right)
    right_text = "\n".join(text for _, text in iter_text(right))
    rows = []
    catch_re = re.compile(r"(?:catch|__catch)\s*\(([^)]*)\)")
    for path, text in iter_text(left):
        for match in catch_re.finditer(text):
            raw_type = match.group(1).strip()
            if raw_type == "...":
                continue
            tail = text[match.end() : match.end() + 900]
            if not re.search(r"\bthrow\s*;|__throw_exception_again", tail):
                continue
            typ = normalize_catch_type(raw_type)
            token = typ.rsplit("::", 1)[-1]
            line = text.count("\n", 0, match.start()) + 1
            rows.append((typ, str(path), line, raw_type, token in right_text))
    print("normalized_type\tfile\tline\traw_catch\tpeer_token_present")
    for row in sorted(rows):
        print("\t".join(map(str, row)))
    return 0


def diagnostics(args: argparse.Namespace) -> int:
    private = []
    link = []
    patterns = [
        re.compile(r"(?P<loc>[^:\n]+:\d+(?::\d+)?): error: no (?:type|member) named '(?P<id>[^']+)'"),
        re.compile(r"(?P<loc>[^:\n]+:\d+(?::\d+)?): error: '(?P<owner>[^']+)' .*not a class, namespace, or enumeration"),
    ]
    for name in args.logs:
        path = pathlib.Path(name)
        text = path.read_text(errors="replace")
        lines = text.splitlines()
        for line_no, line in enumerate(lines, 1):
            for pattern in patterns:
                match = pattern.search(line)
                if match:
                    identifier = match.groupdict().get("id") or match.groupdict().get("owner")
                    context = " ".join(lines[line_no : line_no + 2])
                    private_tokens = re.findall(r"::([A-Za-z_][A-Za-z0-9_]*)", context)
                    if private_tokens:
                        identifier = private_tokens[-1]
                    private.append((str(path), line_no, identifier, line.strip()))
            if "undefined reference to" in line:
                symbol = line.split("undefined reference to", 1)[1].strip(" `'")
                namespaces = []
                if "std::__1::" in symbol:
                    namespaces.append("LIBCXX_STD_NAMESPACE")
                if "std::__cxx11::" in symbol or "std::__detail::" in symbol:
                    namespaces.append("LIBSTDCXX_STD_NAMESPACE")
                if namespaces:
                    link.append((str(path), line_no, ",".join(namespaces), symbol))
    print("[compile_private_api_candidates]")
    print("log\tline\tidentifier_or_owner\tdiagnostic")
    for row in private:
        print("\t".join(map(str, row)))
    print("[cross_stdlib_link_candidates]")
    print("log\tline\tabi_namespace\tundefined_symbol")
    for row in link:
        print("\t".join(map(str, row)))
    return 0


def read_symbols(path: pathlib.Path, dynamic: bool):
    option = "--dyn-syms" if dynamic else "-s"
    result = subprocess.run(
        ["readelf", "-W", option, str(path)], text=True, stdout=subprocess.PIPE,
        stderr=subprocess.PIPE, check=False
    )
    symbols = {}
    symbol_re = re.compile(
        r"^\s*\d+:\s+[0-9a-fA-F]+\s+\d+\s+\S+\s+(\S+)\s+(\S+)\s+(\S+)\s+(.+)$"
    )
    for line in result.stdout.splitlines():
        match = symbol_re.match(line)
        if not match:
            continue
        bind, vis, ndx, name = match.groups()
        name = name.split("@", 1)[0]
        if name.startswith(("_ZTI", "_ZTS")):
            symbols[name] = (bind, vis, ndx)
    return symbols


def rtti(args: argparse.Namespace) -> int:
    records = {}
    for name in args.elfs:
        path = pathlib.Path(name)
        full = read_symbols(path, False)
        dynamic = read_symbols(path, True)
        records[str(path)] = (full, dynamic)
    by_symbol = collections.defaultdict(list)
    for path, (full, dynamic) in records.items():
        for symbol, shape in full.items():
            by_symbol[symbol].append((path, *shape, "YES" if symbol in dynamic else "NO"))
    print("symbol\tfile\tbind\tvisibility\tindex\tin_dynsym\trisk_shape")
    for symbol, rows in sorted(by_symbol.items()):
        defined = [row for row in rows if row[3] != "UND"]
        hidden_from_dyn = any(row[3] != "UND" and row[4] == "NO" for row in rows)
        risk = len(defined) > 1 and hidden_from_dyn
        if not risk:
            continue
        for row in rows:
            print("\t".join([symbol, *map(str, row), "DUPLICATE_RTTI_NOT_ALL_DYNAMICALLY_VISIBLE"]))
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    sub = parser.add_subparsers(dest="mode", required=True)
    p_def = sub.add_parser("defensive")
    p_def.add_argument("--left", required=True)
    p_def.add_argument("--right", required=True)
    p_def.set_defaults(func=defensive)
    p_diag = sub.add_parser("diagnostics")
    p_diag.add_argument("logs", nargs="+")
    p_diag.set_defaults(func=diagnostics)
    p_rtti = sub.add_parser("rtti")
    p_rtti.add_argument("elfs", nargs="+")
    p_rtti.set_defaults(func=rtti)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
