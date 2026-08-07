#!/usr/bin/env python3
import csv
import re
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
R16 = ROOT / "progress/R16/tables"
OUT = ROOT / "progress/R17/tables"
OUT.mkdir(parents=True, exist_ok=True)


def read_tsv(path):
    with path.open(newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))


def write_tsv(name, header, rows):
    with (OUT / name).open("w", newline="") as f:
        writer = csv.writer(f, delimiter="\t", lineterminator="\n")
        writer.writerow(header)
        writer.writerows(rows)


def family(symbol):
    if symbol.startswith("__aeabi_"):
        return "AEABI"
    if re.match(r"^__(?:bid_|dpd_|dec)", symbol):
        return "DECIMAL_FLOAT"
    if symbol.startswith("__gnu_") or re.match(
        r"^__(?:ssadd|usadd|ssub|ussub|mulha|mulhq|mulsa|mulsq|muluda|muludq|muluha|muluhq|muluqq|mulusa|mulusq)", symbol
    ):
        return "FIXED_POINT"
    if re.match(
        r"^__(?:absv|add|ash|bswap|clrsb|clz|cmp|ctz|div|eq|extend|ffs|fix|float|ge|get|gt|le|lshr|lt|mod|mul|ne|neg|parity|pop|powi|sub|trunc|ucmp|udiv|umod|unord)", symbol
    ):
        return "CONVENTIONAL_INT_FLOAT"
    return "OTHER"


missing_rows = read_tsv(R16 / "compiler_rt_missing_helpers.tsv")
version_rows = read_tsv(R16 / "libgcc_symbol_versions.tsv")
export_rows = read_tsv(R16 / "libgcc_export_classification.tsv")
aeabi_rows = read_tsv(R16 / "armv7l_aeabi_symbols.tsv")

versions = defaultdict(set)
for row in version_rows:
    versions[(row["arch"], row["symbol"])].add(row["version_node"])

categories = {(row["arch"], row["symbol"]): row["category"] for row in export_rows}
missing = defaultdict(set)
for row in missing_rows:
    missing[row["arch"]].add(row["libgcc_helper_not_in_compiler_rt"])

members = []
family_summary = []
reference_status = []
missing_version_counts = Counter()
for arch in ("armv7l", "aarch64", "x86_64"):
    denom = len(missing[arch])
    grouped = defaultdict(list)
    for symbol in sorted(missing[arch]):
        fam = family(symbol)
        grouped[fam].append(symbol)
        nodes = ",".join(sorted(versions[(arch, symbol)])) or "UNVERSIONED_OR_NOT_FOUND"
        members.append((arch, symbol, fam, nodes))
        reference_status.append((
            arch, symbol, "NOT_AVAILABLE", "NOT_AVAILABLE",
            "R11_UNFILTERED_UNDEFINED_SYMBOLS_NOT_RETAINED",
        ))
        for node in versions[(arch, symbol)] or {"UNVERSIONED_OR_NOT_FOUND"}:
            missing_version_counts[(arch, node)] += 1
    for fam in ("AEABI", "DECIMAL_FLOAT", "FIXED_POINT", "CONVENTIONAL_INT_FLOAT", "OTHER"):
        symbols = grouped[fam]
        count = len(symbols)
        family_summary.append((
            arch, fam, count, denom,
            f"{count / denom:.6%}" if denom else "NOT_AVAILABLE",
            ",".join(symbols[:5]) if symbols else "NONE",
        ))

write_tsv(
    "missing_helper_family_members.tsv",
    ["arch", "symbol", "family", "version_nodes"], members,
)
write_tsv(
    "missing_helper_family_summary.tsv",
    ["arch", "family", "symbol_count", "missing_helper_denominator", "share_of_missing", "representative_symbols"],
    family_summary,
)
write_tsv(
    "missing_helper_actual_reference_status.tsv",
    ["arch", "symbol", "referencing_runtime_elf_count", "referencing_binary_package_count", "status"],
    reference_status,
)
write_tsv(
    "missing_helper_version_node_summary.tsv",
    ["arch", "version_node", "missing_helper_symbol_version_pairs"],
    sorted((arch, node, count) for (arch, node), count in missing_version_counts.items()),
)

full_node_counts = Counter()
for row in version_rows:
    full_node_counts[(row["arch"], row["version_node"], row["category"])] += 1
write_tsv(
    "libgcc_version_node_distribution.tsv",
    ["arch", "version_node", "category", "symbol_version_pairs"],
    sorted((arch, node, category, count) for (arch, node, category), count in full_node_counts.items()),
)

dedup = []
duplicate_detail = []
for arch in ("armv7l", "aarch64", "x86_64"):
    rows = [row for row in version_rows if row["arch"] == arch]
    names = defaultdict(set)
    for row in rows:
        names[row["symbol"]].add(row["version_node"])
    duplicate_names = sorted(name for name, nodes in names.items() if len(nodes) > 1)
    dedup.append((arch, len(rows), len(names), len(rows) - len(names), len(duplicate_names)))
    for name in duplicate_names:
        duplicate_detail.append((arch, name, ",".join(sorted(names[name])), len(names[name])))
write_tsv(
    "libgcc_export_deduplication.tsv",
    ["arch", "defined_symbol_version_pairs", "unique_base_symbol_names", "duplicate_version_pairs_removed", "base_names_with_multiple_nodes"],
    dedup,
)
write_tsv(
    "libgcc_multiple_version_base_names.tsv",
    ["arch", "symbol", "version_nodes", "node_count"], duplicate_detail,
)

aeabi_missing = []
for row in aeabi_rows:
    if row["compiler_rt_exact_name_match"] != "NO":
        continue
    symbol = row["symbol"]
    aeabi_missing.append((
        row["arch"], symbol, categories.get((row["arch"], symbol), "NOT_FOUND"),
        "YES" if symbol in missing[row["arch"]] else "NO_CROSS_CLASS_EHABI",
        ",".join(sorted(versions[(row["arch"], symbol)])) or "UNVERSIONED_OR_NOT_FOUND",
        "NOT_AVAILABLE", "NOT_AVAILABLE", "R11_UNFILTERED_UNDEFINED_SYMBOLS_NOT_RETAINED",
    ))
write_tsv(
    "armv7l_missing_aeabi_symbols.tsv",
    ["arch", "symbol", "R16_export_category", "in_R16_missing_helper_set", "version_nodes", "referencing_runtime_elf_count", "referencing_binary_package_count", "reference_status"],
    aeabi_missing,
)

write_tsv(
    "true_gap_summary.tsv",
    ["arch", "missing_helper_symbols", "actually_referenced_missing_symbols", "zero_reference_missing_symbols", "status"],
    [(arch, len(missing[arch]), "NOT_AVAILABLE", "NOT_AVAILABLE", "R11_UNFILTERED_UNDEFINED_SYMBOLS_NOT_RETAINED") for arch in ("armv7l", "aarch64", "x86_64")],
)
write_tsv(
    "true_gap_version_nodes.tsv",
    ["arch", "status", "missing_information"],
    [(arch, "NOT_AVAILABLE", "R11 lacks unfiltered per-ELF UND symbol records") for arch in ("armv7l", "aarch64", "x86_64")],
)
