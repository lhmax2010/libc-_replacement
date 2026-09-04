#!/usr/bin/env python3
import argparse
import csv
import json
import os
import struct
import subprocess
from pathlib import Path

SHT_SYMTAB = 2
SHT_DYNSYM = 11
TARGETS = (b"condition_variable4wait", b"condition_variable15__do_timed_wait")


def read_at(stream, offset, size):
    stream.seek(offset)
    data = stream.read(size)
    if len(data) != size:
        raise ValueError("short read")
    return data


def target_symbols(stream):
    ident = read_at(stream, 0, 16)
    if ident[:4] != b"\x7fELF":
        raise ValueError("not ELF")
    elf_class, encoding = ident[4], ident[5]
    endian = "<" if encoding == 1 else ">" if encoding == 2 else None
    if endian is None:
        raise ValueError("unsupported encoding")
    if elf_class == 1:
        header = read_at(stream, 16, 36)
        shoff = struct.unpack_from(endian + "I", header, 16)[0]
        shentsize = struct.unpack_from(endian + "H", header, 30)[0]
        shnum = struct.unpack_from(endian + "H", header, 32)[0]
        shfmt, default_entsize, shndx_offset = endian + "IIIIIIIIII", 16, 14
    elif elf_class == 2:
        header = read_at(stream, 16, 48)
        shoff = struct.unpack_from(endian + "Q", header, 24)[0]
        shentsize = struct.unpack_from(endian + "H", header, 42)[0]
        shnum = struct.unpack_from(endian + "H", header, 44)[0]
        shfmt, default_entsize, shndx_offset = endian + "IIQQQQIIQQ", 24, 6
    else:
        raise ValueError("unsupported class")
    if shnum == 0:
        shnum = struct.unpack(shfmt, read_at(stream, shoff, struct.calcsize(shfmt)))[5]
    sections = []
    for index in range(shnum):
        fields = struct.unpack(shfmt, read_at(stream, shoff + index * shentsize,
                                              struct.calcsize(shfmt)))
        sections.append((fields[1], fields[4], fields[5], fields[6], fields[9]))
    for section_type, offset, size, link, entsize in sections:
        if section_type not in (SHT_SYMTAB, SHT_DYNSYM) or not size:
            continue
        if link >= len(sections):
            raise ValueError("bad string-table link")
        _, strings_offset, strings_size, _, _ = sections[link]
        strings = read_at(stream, strings_offset, strings_size)
        entry_size = entsize or default_entsize
        table = "DYNSYM" if section_type == SHT_DYNSYM else "SYMTAB"
        for index in range(size // entry_size):
            entry = read_at(stream, offset + index * entry_size, entry_size)
            name_offset = struct.unpack_from(endian + "I", entry, 0)[0]
            if not name_offset or name_offset >= len(strings):
                continue
            end = strings.find(b"\0", name_offset)
            name = strings[name_offset:end if end >= 0 else len(strings)]
            if not any(target in name for target in TARGETS):
                continue
            shndx = struct.unpack_from(endian + "H", entry, shndx_offset)[0]
            yield table, name.decode("ascii", errors="replace"), "UND" if shndx == 0 else "DEF"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--shards", required=True, type=Path)
    parser.add_argument("--extracted", required=True, type=Path)
    parser.add_argument("--extra", action="append", default=[])
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    rows, failures = [], []
    package_records = elf_records = 0
    for entry in sorted(os.scandir(args.shards), key=lambda value: value.name):
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
                path = root / elf["path"].lstrip("/")
                try:
                    with path.open("rb") as stream:
                        for table, name, state in set(target_symbols(stream)):
                            rows.append(("PLATFORM", package["repo_id"], package["name"],
                                         package["arch"], elf["path"], table, state, name))
                except (OSError, ValueError, struct.error) as error:
                    failures.append((str(path), str(error)))
        except (OSError, ValueError, KeyError) as error:
            failures.append((entry.path, f"metadata: {error}"))
    for extra in args.extra:
        path = Path(extra)
        try:
            with path.open("rb") as stream:
                for table, name, state in set(target_symbols(stream)):
                    rows.append(("POSITIVE_CONTROL", "CONTROL", "CONTROL", "x86_64",
                                 str(path), table, state, name))
        except (OSError, ValueError, struct.error) as error:
            failures.append((str(path), str(error)))
    names = sorted({row[-1] for row in rows})
    decoded = {}
    if names:
        proc = subprocess.run(["c++filt"], input="\n".join(names) + "\n", text=True,
                              stdout=subprocess.PIPE, check=True)
        decoded = dict(zip(names, proc.stdout.splitlines()))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("scope", "repo", "package", "arch", "elf", "table",
                         "state", "raw_symbol", "demangled_symbol"))
        for row in sorted(rows):
            writer.writerow((*row, decoded.get(row[-1], "NOT_AVAILABLE")))
    platform = [row for row in rows if row[0] == "PLATFORM"]
    control = [row for row in rows if row[0] == "POSITIVE_CONTROL"]
    args.summary.write_text(
        "metric\tvalue\n"
        f"package_records\t{package_records}\n"
        f"elf_records\t{elf_records}\n"
        f"platform_rows\t{len(platform)}\n"
        f"platform_undefined_rows\t{sum(row[6] == 'UND' for row in platform)}\n"
        f"platform_consumer_elfs\t{len({(row[3], row[4]) for row in platform if row[6] == 'UND'})}\n"
        f"platform_consumer_packages\t{len({row[2] for row in platform if row[6] == 'UND'})}\n"
        f"control_rows\t{len(control)}\n"
        f"parse_failures\t{len(failures)}\n" +
        "".join(f"failure:{path}\t{reason}\n" for path, reason in failures))
    print(f"platform_rows={len(platform)}")
    print(f"platform_consumer_elfs={len({(row[3], row[4]) for row in platform if row[6] == 'UND'})}")
    print(f"control_rows={len(control)}")
    print(f"parse_failures={len(failures)}")
    return 0 if control and not failures else 2


if __name__ == "__main__":
    raise SystemExit(main())
