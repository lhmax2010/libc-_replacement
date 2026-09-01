#!/usr/bin/env python3
"""Scan R11's verified published-package ELF corpus for R87 mangled forms."""

from __future__ import annotations

import argparse
import csv
import io
import json
import os
import re
import struct
import subprocess
from pathlib import Path
from typing import BinaryIO


SHT_SYMTAB = 2
SHT_DYNSYM = 11


def read_at(stream: BinaryIO, offset: int, size: int) -> bytes:
    stream.seek(offset)
    data = stream.read(size)
    if len(data) != size:
        raise ValueError(f"short read at {offset}: wanted {size}, got {len(data)}")
    return data


def symbol_names(stream: BinaryIO, base: int = 0, extent: int | None = None):
    ident = read_at(stream, base, 16)
    if ident[:4] != b"\x7fELF":
        raise ValueError("not ELF")
    elf_class, data_encoding = ident[4], ident[5]
    if data_encoding not in (1, 2):
        raise ValueError(f"unsupported EI_DATA={data_encoding}")
    endian = "<" if data_encoding == 1 else ">"
    if elf_class == 1:
        header = read_at(stream, base + 16, 36)
        shoff = struct.unpack_from(endian + "I", header, 16)[0]
        shentsize = struct.unpack_from(endian + "H", header, 30)[0]
        shnum = struct.unpack_from(endian + "H", header, 32)[0]
        shfmt = endian + "IIIIIIIIII"
        sym_default_size = 16
    elif elf_class == 2:
        header = read_at(stream, base + 16, 48)
        shoff = struct.unpack_from(endian + "Q", header, 24)[0]
        shentsize = struct.unpack_from(endian + "H", header, 42)[0]
        shnum = struct.unpack_from(endian + "H", header, 44)[0]
        shfmt = endian + "IIQQQQIIQQ"
        sym_default_size = 24
    else:
        raise ValueError(f"unsupported EI_CLASS={elf_class}")
    if shentsize < struct.calcsize(shfmt):
        raise ValueError("short section-header entry")
    if shnum == 0:
        # ELF gABI: e_shnum == 0 stores the real count in sh_size of section 0.
        raw = read_at(stream, base + shoff, struct.calcsize(shfmt))
        shnum = struct.unpack(shfmt, raw)[5]
        if shnum == 0:
            raise ValueError("invalid extended section count 0")

    sections = []
    for index in range(shnum):
        raw = read_at(stream, base + shoff + index * shentsize, struct.calcsize(shfmt))
        fields = struct.unpack(shfmt, raw)
        if elf_class == 1:
            sh_type, offset, size, link, entsize = fields[1], fields[4], fields[5], fields[6], fields[9]
        else:
            sh_type, offset, size, link, entsize = fields[1], fields[4], fields[5], fields[6], fields[9]
        sections.append((sh_type, offset, size, link, entsize))

    for section_index, (sh_type, offset, size, link, entsize) in enumerate(sections):
        if sh_type not in (SHT_SYMTAB, SHT_DYNSYM) or size == 0:
            continue
        if link >= len(sections):
            raise ValueError(f"bad strtab link {link}")
        _, str_offset, str_size, _, _ = sections[link]
        strings = read_at(stream, base + str_offset, str_size)
        entry_size = entsize or sym_default_size
        if entry_size < 4:
            raise ValueError("bad symbol entry size")
        count = size // entry_size
        table_kind = "DYNSYM" if sh_type == SHT_DYNSYM else "SYMTAB"
        for symbol_index in range(count):
            raw = read_at(stream, base + offset + symbol_index * entry_size, 4)
            name_offset = struct.unpack(endian + "I", raw)[0]
            if name_offset == 0 or name_offset >= len(strings):
                continue
            end = strings.find(b"\0", name_offset)
            if end < 0:
                end = len(strings)
            name = strings[name_offset:end]
            if name.startswith(b"_Z"):
                yield table_kind, name.decode("ascii", errors="replace")


def classify(name: str) -> list[str]:
    kinds: list[str] = []
    targets = (
        "condition_variable4wait",
        "condition_variable15__do_timed_wait",
        "wbuffer_convert",
    )
    present = [target for target in targets if target in name]
    if present:
        kinds.append("SITE_NAME_PRESENT:" + ",".join(present))
    if present and "Do" in name:
        kinds.append("SITE_WITH_NOEXCEPT_TYPE_ENCODING")
    if present and ("PDo" in name or "M" in name and "DoF" in name):
        kinds.append("SITE_WITH_POINTER_NOEXCEPT_ENCODING")
    # A type-only use does not carry the member function's name.  For example,
    # `void f(void (std::condition_variable::*)() noexcept)` contains the class
    # name plus `DoF`, but not `wait`.  Retain those separately for review.
    if "condition_variable" in name and "DoF" in name:
        kinds.append("CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE")
    if "wbuffer_convert" in name and "DoF" in name:
        kinds.append("WBUFFER_CONVERT_NOEXCEPT_FUNCTION_TYPE")
    # noexcept/traits can feed a boolean non-type template argument.  This is a
    # candidate only: the mangling cannot say which expression produced it.
    if "wbuffer_convert" in name and re.search(r"L[bB][01]E", name):
        kinds.append("WBUFFER_CONVERT_BOOL_TEMPLATE_CANDIDATE")
    return kinds


def demangle(names: list[str]) -> dict[str, str]:
    if not names:
        return {}
    proc = subprocess.run(["c++filt"], input="\n".join(names) + "\n", text=True,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    return dict(zip(names, proc.stdout.splitlines()))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--shards", required=True, type=Path)
    parser.add_argument("--extracted", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()

    json_files = sorted(entry.path for entry in os.scandir(args.shards) if entry.name.endswith(".json"))
    package_records = 0
    elf_records = 0
    unique_elf_paths: set[str] = set()
    symbols_seen = 0
    rows: list[tuple[str, str, str, str, str, str, str, str]] = []
    failures: list[tuple[str, str]] = []

    for json_path in json_files:
        try:
            with open(json_path, encoding="utf-8") as stream:
                record = json.load(stream)
        except (OSError, ValueError) as error:
            failures.append((json_path, f"JSON:{error}"))
            continue
        package_records += 1
        package = record["package"]
        sha = package["actual_sha256"]
        root = args.extracted / sha[:2] / sha
        for elf in record["elfs"]:
            elf_records += 1
            path = root / elf["path"].lstrip("/")
            unique_elf_paths.add(str(path))
            try:
                with path.open("rb") as stream:
                    per_file_seen: set[tuple[str, str]] = set()
                    for table_kind, name in symbol_names(stream):
                        key = (table_kind, name)
                        if key in per_file_seen:
                            continue
                        per_file_seen.add(key)
                        symbols_seen += 1
                        for kind in classify(name):
                            rows.append((
                                package["repo_id"], package["name"], package["arch"], sha,
                                elf["path"], table_kind, kind, name,
                            ))
            except (OSError, ValueError, struct.error) as error:
                failures.append((str(path), f"ELF:{error}"))

    names = sorted({row[-1] for row in rows})
    demangled = demangle(names)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("repo", "package", "arch", "rpm_sha256", "elf_path", "symbol_table",
                         "classification", "raw_symbol", "demangled_symbol"))
        for row in sorted(rows):
            writer.writerow((*row, demangled.get(row[-1], "NOT_AVAILABLE")))

    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("metric", "value"))
        writer.writerow(("json_package_records", package_records))
        writer.writerow(("elf_records", elf_records))
        writer.writerow(("unique_elf_paths", len(unique_elf_paths)))
        writer.writerow(("mangled_symbols_scanned", symbols_seen))
        writer.writerow(("site_symbol_rows", len(rows)))
        writer.writerow(("site_with_noexcept_type_encoding_rows",
                         sum(1 for row in rows if row[-2] == "SITE_WITH_NOEXCEPT_TYPE_ENCODING")))
        writer.writerow(("site_with_pointer_noexcept_encoding_rows",
                         sum(1 for row in rows if row[-2] == "SITE_WITH_POINTER_NOEXCEPT_ENCODING")))
        writer.writerow(("condition_variable_noexcept_function_type_rows",
                         sum(1 for row in rows if row[-2] == "CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE")))
        writer.writerow(("wbuffer_convert_noexcept_function_type_rows",
                         sum(1 for row in rows if row[-2] == "WBUFFER_CONVERT_NOEXCEPT_FUNCTION_TYPE")))
        writer.writerow(("wbuffer_convert_bool_template_candidate_rows",
                         sum(1 for row in rows if row[-2] == "WBUFFER_CONVERT_BOOL_TEMPLATE_CANDIDATE")))
        writer.writerow(("parse_failures", len(failures)))
        for path, reason in failures:
            writer.writerow((f"FAILURE:{path}", reason))

    print(f"json_package_records={package_records}")
    print(f"elf_records={elf_records}")
    print(f"unique_elf_paths={len(unique_elf_paths)}")
    print(f"mangled_symbols_scanned={symbols_seen}")
    print(f"site_symbol_rows={len(rows)}")
    print(f"noexcept_type_rows={sum(1 for row in rows if row[-2] == 'SITE_WITH_NOEXCEPT_TYPE_ENCODING')}")
    print(f"pointer_noexcept_rows={sum(1 for row in rows if row[-2] == 'SITE_WITH_POINTER_NOEXCEPT_ENCODING')}")
    print(f"condition_variable_noexcept_type_rows={sum(1 for row in rows if row[-2] == 'CONDITION_VARIABLE_NOEXCEPT_FUNCTION_TYPE')}")
    print(f"wbuffer_convert_noexcept_type_rows={sum(1 for row in rows if row[-2] == 'WBUFFER_CONVERT_NOEXCEPT_FUNCTION_TYPE')}")
    print(f"wbuffer_convert_bool_candidate_rows={sum(1 for row in rows if row[-2] == 'WBUFFER_CONVERT_BOOL_TEMPLATE_CANDIDATE')}")
    print(f"parse_failures={len(failures)}")
    return 0 if not failures else 2


if __name__ == "__main__":
    raise SystemExit(main())
