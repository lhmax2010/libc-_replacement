#!/usr/bin/env python3
"""Create compact, reviewable summaries from R103's raw symbol matches."""

import csv
import subprocess
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TABLES = ROOT / "tables"


def read_tsv(name):
    with (TABLES / name).open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))


def write_tsv(name, fields, rows):
    with (TABLES / name).open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
        w.writeheader()
        w.writerows(rows)


matches = read_tsv("archive_symbol_matches.tsv")
members = read_tsv("archive_member_matches.tsv")

# The build logs identify the published consumers' tensorflow2-lite-devel as
# Unified Toolchain release 2.18.0-1, so use that provider variant for the
# primary counts. Base counts remain available in the full tables.
primary = [r for r in matches if r["provider_variant"] == "unified_toolchain"]
groups = defaultdict(list)
for r in primary:
    key = (
        r["source_name"], r["arch"], r["elf_path"], r["provider"], r["archive"]
    )
    groups[key].append(r)

summary = []
for key, rows in sorted(groups.items()):
    source, arch, elf_path, provider, archive = key
    symbols = {r["symbol"] for r in rows}
    archive_members = {r["archive_member"] for r in rows}
    summary.append(
        {
            "consumer_source": source,
            "arch": arch,
            "final_elf": elf_path,
            "provider": provider,
            "archive": archive,
            "exact_strong_symbols": len(symbols),
            "attributed_archive_members_lower_bound": len(archive_members),
            "cxx_mangled_symbols": sum(s.startswith("_Z") for s in symbols),
        }
    )

write_tsv(
    "consumer_linkage_summary.tsv",
    [
        "consumer_source", "arch", "final_elf", "provider", "archive",
        "exact_strong_symbols", "attributed_archive_members_lower_bound",
        "cxx_mangled_symbols",
    ],
    summary,
)

# Select representative high-confidence archive members. Require several exact
# strong-symbol matches to avoid treating a generic one-symbol coincidence as
# proof. The complete, unfiltered member table is retained separately.
wanted = {
    "interpreter.cc.o", "interpreter_builder.cc.o", "model_builder.cc.o",
    "subgraph.cc.o", "gpu_info.cc.o", "flatbuffer_conversions.cc.o",
    "opencl_wrapper.cc.o",
}
examples = []
for r in members:
    if r["provider_variant"] != "unified_toolchain":
        continue
    count = int(r["matched_strong_symbol_count"])
    if r["archive_member"] in wanted and count >= 5:
        examples.append(r)

write_tsv(
    "representative_archive_members.tsv",
    list(examples[0].keys()) if examples else list(members[0].keys()),
    sorted(
        examples,
        key=lambda r: (r["source_name"], r["arch"], r["elf_path"], r["archive_member"]),
    ),
)

# Provider-level observed/not-observed result across the named consumers.
provider_counts = defaultdict(lambda: {"symbols": set(), "members": set(), "elfs": set()})
for r in primary:
    d = provider_counts[(r["provider"], r["archive"])]
    d["symbols"].add(r["symbol"])
    d["members"].add(r["archive_member"])
    d["elfs"].add((r["source_name"], r["arch"], r["elf_path"]))

provider_rows = []
for provider, archive in [
    ("gmp", "libgmpxx.a"),
    ("tensorflow2", "libtf_xla_runtime.a"),
    ("tensorflow2", "libtensorflow2-lite.a"),
]:
    d = provider_counts[(provider, archive)]
    provider_rows.append({
        "provider": provider,
        "archive": archive,
        "actual_linkage_observed": "YES" if d["symbols"] else "NO",
        "distinct_exact_strong_symbols": len(d["symbols"]),
        "distinct_archive_members_lower_bound": len(d["members"]),
        "consumer_elf_arch_tuples": len(d["elfs"]),
    })

write_tsv(
    "provider_observation_summary.tsv",
    [
        "provider", "archive", "actual_linkage_observed",
        "distinct_exact_strong_symbols", "distinct_archive_members_lower_bound",
        "consumer_elf_arch_tuples",
    ],
    provider_rows,
)

print(f"primary_match_rows={len(primary)}")
print(f"summary_rows={len(summary)}")
print(f"representative_member_rows={len(examples)}")
for row in provider_rows:
    print("provider_summary=" + "\t".join(str(row[k]) for k in row))
