#!/usr/bin/env python3
"""Match strong archive definitions against symbols defined by published ELF files."""

import csv
import re
import shlex
import subprocess
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PROVIDER_ROOTS = [
    ("base_toolchain", ROOT / "progress/R103/extracted/provider_archives"),
    ("unified_toolchain", ROOT / "progress/R103/extracted/provider_archives_unified"),
]
CONSUMERS = ROOT / "progress/R103/extracted/consumers"
TABLES = ROOT / "progress/R103/tables"
LEDGER = ROOT / "progress/R103/raw/scan_final_elf_subcommands.log"

ARCHIVE_KIND = {
    "libgmpxx.a": "gmp:libgmpxx.a",
    "libtf_xla_runtime.a": "tensorflow2:libtf_xla_runtime.a",
    "libtensorflow2-lite.a": "tensorflow2:libtensorflow2-lite.a",
}
STRONG_TYPES = set("TDRBSG")
NM_ARCHIVE = re.compile(r"^(.*)\[([^]]+)\]:\s+(\S+)\s+(\S+)(?:\s+.*)?$")


class Ledger:
    def __init__(self):
        self.rows = []

    def run(self, command, *, check=False):
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        self.rows.append((shlex.join(command), result.returncode, result.stderr))
        if check and result.returncode:
            raise RuntimeError(
                f"command failed rc={result.returncode}: {shlex.join(command)}\n{result.stderr}"
            )
        return result

    def write(self):
        with LEDGER.open("w", encoding="utf-8") as stream:
            for index, (command, returncode, stderr) in enumerate(self.rows, 1):
                stream.write(f"SUBCOMMAND={index:06d}\nCOMMAND={command}\n")
                if stderr:
                    stream.write("STDERR=" + stderr.replace("\n", "\\n") + "\n")
                stream.write(f"EXIT_CODE={returncode}\n\n")


def write_tsv(path, fields, rows):
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def parse_plain_nm(text):
    found = {}
    for line in text.splitlines():
        parts = line.split()
        if len(parts) >= 2:
            symbol, symbol_type = parts[0], parts[1]
            found[symbol] = symbol_type
    return found


TABLES.mkdir(parents=True, exist_ok=True)
ledger = Ledger()

provider_rows = []
provider_index = defaultdict(list)
for provider_variant, provider_root in PROVIDER_ROOTS:
    if not provider_root.exists():
        continue
    for archive in sorted(provider_root.rglob("*.a")):
        kind = ARCHIVE_KIND.get(archive.name)
        if not kind:
            continue
        arch = archive.relative_to(provider_root).parts[0]
        probe = ledger.run(["nm", "-A", "-P", "-g", "--defined-only", str(archive)], check=True)
        for line in probe.stdout.splitlines():
            match = NM_ARCHIVE.match(line)
            if not match:
                continue
            _archive_path, member, symbol, symbol_type = match.groups()
            strength = "STRONG" if symbol_type[:1] in STRONG_TYPES else "WEAK_OR_OTHER"
            row = {
                "provider": kind.split(":", 1)[0],
                "provider_variant": provider_variant,
                "archive": archive.name,
                "arch": arch,
                "member": member,
                "symbol": symbol,
                "symbol_type": symbol_type,
                "strength": strength,
            }
            provider_rows.append(row)
            if strength == "STRONG":
                provider_index[(arch, symbol)].append(row)

write_tsv(
    TABLES / "provider_archive_symbols.tsv",
    ["provider", "provider_variant", "archive", "arch", "member", "symbol", "symbol_type", "strength"],
    provider_rows,
)
provider_occurrences = {key: len(rows) for key, rows in provider_index.items()}

elf_rows = []
dependency_rows = []
match_rows = []
member_matches = defaultdict(set)
elf_count = 0
for path in sorted(CONSUMERS.rglob("*")):
    if not path.is_file():
        continue
    try:
        with path.open("rb") as stream:
            if stream.read(4) != b"\x7fELF":
                continue
    except OSError:
        continue
    elf_count += 1
    relative = path.relative_to(CONSUMERS)
    source_name, arch, binary_name = relative.parts[:3]
    payload_path = "/" + "/".join(relative.parts[3:])

    header = ledger.run(["readelf", "-h", str(path)], check=True)
    machine = "NOT_OBSERVED"
    for line in header.stdout.splitlines():
        if "Machine:" in line:
            machine = line.split("Machine:", 1)[1].strip()
            break

    dynamic = ledger.run(["readelf", "-d", str(path)])
    needed = []
    if dynamic.returncode == 0:
        for line in dynamic.stdout.splitlines():
            if "(NEEDED)" in line:
                match = re.search(r"\[([^]]+)\]", line)
                if match:
                    needed.append(match.group(1))
    for library in sorted(set(needed)):
        dependency_rows.append(
            {
                "source_name": source_name,
                "binary_name": binary_name,
                "arch": arch,
                "elf_path": payload_path,
                "needed": library,
            }
        )

    static_nm = ledger.run(["nm", "-a", "-P", "--defined-only", str(path)])
    static_symbols = parse_plain_nm(static_nm.stdout) if static_nm.returncode == 0 else {}
    dynamic_nm = ledger.run(["nm", "-D", "-P", "--defined-only", str(path)])
    dynamic_symbols = parse_plain_nm(dynamic_nm.stdout) if dynamic_nm.returncode == 0 else {}
    all_symbols = dict(static_symbols)
    all_symbols.update(dynamic_symbols)
    matched = 0
    for symbol, final_type in sorted(all_symbols.items()):
        for provider in provider_index.get((arch, symbol), []):
            matched += 1
            match_rows.append(
                {
                    "source_name": source_name,
                    "binary_name": binary_name,
                    "arch": arch,
                    "elf_path": payload_path,
                    "provider": provider["provider"],
                    "provider_variant": provider["provider_variant"],
                    "archive": provider["archive"],
                    "archive_member": provider["member"],
                    "symbol": symbol,
                    "archive_symbol_type": provider["symbol_type"],
                    "provider_symbol_occurrences": provider_occurrences[(arch, symbol)],
                    "final_symbol_type": final_type,
                    "present_in_static_symtab": "YES" if symbol in static_symbols else "NO",
                    "present_in_dynamic_symtab": "YES" if symbol in dynamic_symbols else "NO",
                }
            )
            member_matches[
                (
                    source_name,
                    binary_name,
                    arch,
                    payload_path,
                    provider["provider"],
                    provider["provider_variant"],
                    provider["archive"],
                    provider["member"],
                )
            ].add(symbol)

    elf_rows.append(
        {
            "source_name": source_name,
            "binary_name": binary_name,
            "arch": arch,
            "elf_path": payload_path,
            "machine": machine,
            "static_nm_exit": static_nm.returncode,
            "static_symbol_count": len(static_symbols),
            "dynamic_nm_exit": dynamic_nm.returncode,
            "dynamic_symbol_count": len(dynamic_symbols),
            "needed_count": len(set(needed)),
            "exact_provider_symbol_matches": matched,
        }
    )
    if elf_count % 100 == 0:
        print(f"PROGRESS_ELF={elf_count}", flush=True)

write_tsv(
    TABLES / "consumer_elf_inventory.tsv",
    [
        "source_name", "binary_name", "arch", "elf_path", "machine",
        "static_nm_exit", "static_symbol_count", "dynamic_nm_exit",
        "dynamic_symbol_count", "needed_count", "exact_provider_symbol_matches",
    ],
    elf_rows,
)
write_tsv(
    TABLES / "consumer_needed_libraries.tsv",
    ["source_name", "binary_name", "arch", "elf_path", "needed"],
    dependency_rows,
)
write_tsv(
    TABLES / "archive_symbol_matches.tsv",
    [
        "source_name", "binary_name", "arch", "elf_path", "provider", "archive",
        "provider_variant", "archive_member", "symbol", "archive_symbol_type", "final_symbol_type",
        "provider_symbol_occurrences", "present_in_static_symtab", "present_in_dynamic_symtab",
    ],
    match_rows,
)
member_rows = []
for key, symbols in sorted(member_matches.items()):
    source_name, binary_name, arch, payload_path, provider, provider_variant, archive, member = key
    member_rows.append(
        {
            "source_name": source_name,
            "binary_name": binary_name,
            "arch": arch,
            "elf_path": payload_path,
            "provider": provider,
            "provider_variant": provider_variant,
            "archive": archive,
            "archive_member": member,
            "matched_strong_symbol_count": len(symbols),
            "matched_symbols": ";".join(sorted(symbols)),
        }
    )
write_tsv(
    TABLES / "archive_member_matches.tsv",
    [
        "source_name", "binary_name", "arch", "elf_path", "provider", "provider_variant", "archive",
        "archive_member", "matched_strong_symbol_count", "matched_symbols",
    ],
    member_rows,
)
ledger.write()

print(f"provider_symbol_rows={len(provider_rows)}")
print(f"provider_strong_index_keys={len(provider_index)}")
print(f"consumer_elfs={len(elf_rows)}")
print(f"exact_symbol_matches={len(match_rows)}")
print(f"matched_member_rows={len(member_rows)}")
