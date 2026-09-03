#!/usr/bin/env python3
"""Re-run R103's exact strong-symbol/member attribution for one new archive root.

This establishes which members of the newly built archive correspond to code found
in the published consumers.  It does not claim that those old libstdc++ consumers
can link against a libc++ archive; that requires rebuilding a consumer.
"""

import argparse
import csv
import re
import shlex
import subprocess
from collections import defaultdict
from pathlib import Path

STRONG_TYPES = set("TDRBSG")
NM_ARCHIVE = re.compile(r"^(.*)\[([^]]+)\]:\s+(\S+)\s+(\S+)(?:\s+.*)?$")
CONSUMER_NAMES = {"inference-engine-tflite", "nnstreamer", "nntrainer"}


def run(command, ledger):
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    ledger.write(f"COMMAND={shlex.join(command)}\nEXIT_CODE={result.returncode}\n")
    if result.stderr:
        ledger.write("STDERR=" + result.stderr.replace("\n", "\\n") + "\n")
    if result.returncode:
        raise RuntimeError(f"command failed: {shlex.join(command)}")
    return result.stdout


def parse_plain_nm(text):
    found = set()
    for line in text.splitlines():
        parts = line.split()
        if len(parts) >= 2:
            found.add(parts[0])
    return found


parser = argparse.ArgumentParser()
parser.add_argument("--archive-root", required=True, type=Path)
parser.add_argument("--consumer-root", required=True, type=Path)
parser.add_argument("--arch", required=True)
parser.add_argument("--output-dir", required=True, type=Path)
args = parser.parse_args()

args.output_dir.mkdir(parents=True, exist_ok=False)
provider_index = defaultdict(list)
with (args.output_dir / "subcommands.log").open("w", encoding="utf-8") as ledger:
    for archive in sorted(args.archive_root.rglob("*.a")):
        if archive.name not in {"libtensorflow2-lite.a", "libtf_xla_runtime.a"}:
            continue
        text = run(["nm", "-A", "-P", "-g", "--defined-only", str(archive)], ledger)
        for line in text.splitlines():
            match = NM_ARCHIVE.match(line)
            if not match:
                continue
            _, member, symbol, symbol_type = match.groups()
            if symbol_type[:1] in STRONG_TYPES:
                provider_index[symbol].append((archive.name, member, symbol_type))

    rows = []
    member_symbols = defaultdict(set)
    for source_dir in sorted(args.consumer_root.iterdir()):
        if source_dir.name not in CONSUMER_NAMES:
            continue
        arch_dir = source_dir / args.arch
        if not arch_dir.is_dir():
            continue
        for path in sorted(arch_dir.rglob("*")):
            if not path.is_file():
                continue
            with path.open("rb") as candidate:
                if candidate.read(4) != b"\x7fELF":
                    continue
            symbols = set()
            for command in (["nm", "-a", "-P", "--defined-only", str(path)],
                            ["nm", "-D", "-P", "--defined-only", str(path)]):
                try:
                    symbols.update(parse_plain_nm(run(command, ledger)))
                except RuntimeError:
                    pass
            for symbol in sorted(symbols & provider_index.keys()):
                for archive, member, symbol_type in provider_index[symbol]:
                    key = (source_dir.name, str(path.relative_to(arch_dir)), archive, member)
                    member_symbols[key].add(symbol)

    for key, symbols in sorted(member_symbols.items()):
        source, elf_path, archive, member = key
        rows.append({
            "consumer_source": source,
            "arch": args.arch,
            "elf_path": elf_path,
            "archive": archive,
            "archive_member": member,
            "matched_strong_symbols": len(symbols),
            "matched_symbol_names": ";".join(sorted(symbols)),
        })

with (args.output_dir / "member_matches.tsv").open("w", newline="", encoding="utf-8") as stream:
    fields = ["consumer_source", "arch", "elf_path", "archive", "archive_member",
              "matched_strong_symbols", "matched_symbol_names"]
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)

summary = defaultdict(lambda: {"members": set(), "symbols": set(), "elfs": set()})
for row in rows:
    key = (row["consumer_source"], row["archive"])
    summary[key]["members"].add(row["archive_member"])
    summary[key]["symbols"].update(row["matched_symbol_names"].split(";"))
    summary[key]["elfs"].add(row["elf_path"])

with (args.output_dir / "summary.tsv").open("w", newline="", encoding="utf-8") as stream:
    fields = ["consumer_source", "archive", "consumer_elfs", "archive_members_lower_bound",
              "exact_strong_symbols"]
    writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    for (source, archive), values in sorted(summary.items()):
        writer.writerow({
            "consumer_source": source,
            "archive": archive,
            "consumer_elfs": len(values["elfs"]),
            "archive_members_lower_bound": len(values["members"]),
            "exact_strong_symbols": len(values["symbols"]),
        })

print(f"provider_strong_symbols={len(provider_index)}")
print(f"member_match_rows={len(rows)}")
for key, values in sorted(summary.items()):
    print(f"SUMMARY {key[0]} {key[1]} elfs={len(values['elfs'])} "
          f"members={len(values['members'])} symbols={len(values['symbols'])}")
