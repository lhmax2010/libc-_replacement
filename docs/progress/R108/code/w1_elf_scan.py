#!/usr/bin/env python3
"""Find wbuffer_convert instantiations in ELF symbol tables."""

from __future__ import annotations

import argparse
import csv
import json
import os
import struct
import subprocess
from pathlib import Path
from typing import BinaryIO, Iterator

SHT_SYMTAB = 2
SHT_DYNSYM = 11


def read_at(stream: BinaryIO, offset: int, size: int) -> bytes:
    stream.seek(offset)
    data = stream.read(size)
    if len(data) != size:
        raise ValueError(f"short read at {offset}: wanted {size}, got {len(data)}")
    return data


def symbols(stream: BinaryIO) -> Iterator[tuple[str, str]]:
    ident = read_at(stream, 0, 16)
    if ident[:4] != b"\x7fELF":
        raise ValueError("not ELF")
    elf_class, data_encoding = ident[4], ident[5]
    endian = "<" if data_encoding == 1 else ">" if data_encoding == 2 else None
    if endian is None:
        raise ValueError(f"unsupported EI_DATA={data_encoding}")
    if elf_class == 1:
        header = read_at(stream, 16, 36)
        shoff = struct.unpack_from(endian + "I", header, 16)[0]
        shentsize = struct.unpack_from(endian + "H", header, 30)[0]
        shnum = struct.unpack_from(endian + "H", header, 32)[0]
        shfmt, default_entsize = endian + "IIIIIIIIII", 16
    elif elf_class == 2:
        header = read_at(stream, 16, 48)
        shoff = struct.unpack_from(endian + "Q", header, 24)[0]
        shentsize = struct.unpack_from(endian + "H", header, 42)[0]
        shnum = struct.unpack_from(endian + "H", header, 44)[0]
        shfmt, default_entsize = endian + "IIQQQQIIQQ", 24
    else:
        raise ValueError(f"unsupported EI_CLASS={elf_class}")
    if shentsize < struct.calcsize(shfmt):
        raise ValueError("short section-header entry")
    if shnum == 0:
        raw = read_at(stream, shoff, struct.calcsize(shfmt))
        shnum = struct.unpack(shfmt, raw)[5]
        if shnum == 0:
            raise ValueError("invalid extended section count 0")
    sections = []
    for index in range(shnum):
        raw = read_at(stream, shoff + index * shentsize, struct.calcsize(shfmt))
        fields = struct.unpack(shfmt, raw)
        sections.append((fields[1], fields[4], fields[5], fields[6], fields[9]))
    for sh_type, offset, size, link, entsize in sections:
        if sh_type not in (SHT_SYMTAB, SHT_DYNSYM) or not size:
            continue
        if link >= len(sections):
            raise ValueError(f"bad strtab link {link}")
        _, str_offset, str_size, _, _ = sections[link]
        strings = read_at(stream, str_offset, str_size)
        entry_size = entsize or default_entsize
        table = "DYNSYM" if sh_type == SHT_DYNSYM else "SYMTAB"
        for index in range(size // entry_size):
            name_offset = struct.unpack(endian + "I", read_at(stream, offset + index * entry_size, 4))[0]
            if not name_offset or name_offset >= len(strings):
                continue
            end = strings.find(b"\0", name_offset)
            raw_name = strings[name_offset:end if end >= 0 else len(strings)]
            if b"wbuffer_convert" in raw_name:
                yield table, raw_name.decode("ascii", errors="replace")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--shards", required=True, type=Path)
    parser.add_argument("--extracted", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    rows = []
    failures = []
    package_records = elf_records = 0
    for entry in sorted(os.scandir(args.shards), key=lambda item: item.name):
        if not entry.name.endswith(".json"):
            continue
        package_records += 1
        try:
            record = json.loads(Path(entry.path).read_text())
            package = record["package"]
            sha = package["actual_sha256"]
            root = args.extracted / sha[:2] / sha
            for elf in record["elfs"]:
                elf_records += 1
                disk_path = root / elf["path"].lstrip("/")
                try:
                    with disk_path.open("rb") as stream:
                        for table, name in set(symbols(stream)):
                            rows.append((package["repo_id"], package["name"], package["arch"],
                                         sha, elf["path"], table, name))
                except (OSError, ValueError, struct.error) as error:
                    failures.append((str(disk_path), str(error)))
        except (OSError, ValueError, KeyError) as error:
            failures.append((entry.path, f"metadata: {error}"))
    names = sorted({row[-1] for row in rows})
    demangled = {}
    if names:
        proc = subprocess.run(["c++filt"], input="\n".join(names) + "\n", text=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
        demangled = dict(zip(names, proc.stdout.splitlines()))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("repo", "package", "arch", "rpm_sha256", "elf_path",
                         "symbol_table", "raw_symbol", "demangled_symbol"))
        for row in sorted(rows):
            writer.writerow((*row, demangled.get(row[-1], "NOT_AVAILABLE")))
    args.summary.write_text(
        "metric\tvalue\n"
        f"package_records\t{package_records}\n"
        f"elf_records\t{elf_records}\n"
        f"matching_symbol_rows\t{len(rows)}\n"
        f"parse_failures\t{len(failures)}\n" +
        "".join(f"failure:{path}\t{reason}\n" for path, reason in failures)
    )
    print(f"package_records={package_records}")
    print(f"elf_records={elf_records}")
    print(f"matching_symbol_rows={len(rows)}")
    print(f"parse_failures={len(failures)}")
    return 0 if not failures else 2


if __name__ == "__main__":
    raise SystemExit(main())
