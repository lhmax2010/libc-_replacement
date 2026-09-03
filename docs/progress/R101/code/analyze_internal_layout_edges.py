#!/usr/bin/env python3
import csv
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)


def read_tsv(path):
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))


if len(sys.argv) != 7:
    raise SystemExit(
        "usage: analyze_internal_layout_edges.py SOURCE_RECORDS DECISIONS "
        "SOURCE_EDGES ELF_EDGES SOURCE_OUTPUT ELF_OUTPUT"
    )

(
    source_records,
    decisions_path,
    edges_path,
    elf_edges_path,
    output,
    elf_output,
) = map(Path, sys.argv[1:])
sourcerpm_to_name = {
    row["location"]: row["name"]
    for row in read_tsv(source_records)
    if row["kind"] == "source"
}
decisions = {row["source_package"]: row["decision"] for row in read_tsv(decisions_path)}

source_meta = {}
for edge in read_tsv(edges_path):
    if edge["classification"] != "LAYOUT_SENSITIVE_STD_TYPE":
        continue
    consumer_rpm = edge["consumer_sourcerpm"]
    provider_rpm = edge["provider_sourcerpm"]
    consumer = sourcerpm_to_name.get(consumer_rpm, "NOT_AVAILABLE")
    provider = sourcerpm_to_name.get(provider_rpm, "NOT_AVAILABLE")
    source_meta[(consumer_rpm, provider_rpm)] = {
        "consumer_source": consumer,
        "provider_source": provider,
        "consumer_sourcerpm": consumer_rpm,
        "provider_sourcerpm": provider_rpm,
    }

elf_rows = []
for edge in read_tsv(elf_edges_path):
    if edge["layout_classification"] not in {
        "DIRECT_STD_LAYOUT",
        "KNOWN_HIDDEN_STD_LAYOUT",
    }:
        continue
    consumer = sourcerpm_to_name.get(edge["consumer_sourcerpm"], "NOT_AVAILABLE")
    provider = sourcerpm_to_name.get(edge["provider_sourcerpm"], "NOT_AVAILABLE")
    elf_rows.append(
        {
            "consumer_source": consumer,
            "consumer_binary": edge["consumer_binary"],
            "consumer_elf": edge["consumer_elf"],
            "provider_source": provider,
            "provider_binary": edge["provider_binary"],
            "provider_elf": edge["provider_elf"],
            "classification": "LAYOUT_SENSITIVE_STD_TYPE",
            "cpp_symbol_count": edge["cpp_symbol_count"],
            "cpp_symbol_examples": ";".join(edge["demangled_cpp_symbols"].split(";")[:5]),
        }
    )

elf_fields = [
    "consumer_source",
    "consumer_binary",
    "consumer_elf",
    "provider_source",
    "provider_binary",
    "provider_elf",
    "classification",
    "cpp_symbol_count",
    "cpp_symbol_examples",
]
with open(elf_output, "w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(
        f, delimiter="\t", fieldnames=elf_fields, lineterminator="\n"
    )
    writer.writeheader()
    writer.writerows(elf_rows)
print(f"layout_sensitive_elf_edges={len(elf_rows)}")

# The source-level classification file labels a source pair layout-sensitive when
# any contained ELF edge is layout-sensitive, but its counts cover every class in
# that pair.  Aggregate the filtered ELF rows here so the published counts describe
# layout-sensitive edges only.
aggregates = {}
for edge in elf_rows:
    key = (edge["consumer_source"], edge["provider_source"])
    aggregate = aggregates.setdefault(
        key,
        {"elf_edge_count": 0, "cpp_symbol_count": 0, "symbol_examples": []},
    )
    aggregate["elf_edge_count"] += 1
    aggregate["cpp_symbol_count"] += int(edge["cpp_symbol_count"])
    for symbol in edge["cpp_symbol_examples"].split(";"):
        if symbol and symbol not in aggregate["symbol_examples"]:
            aggregate["symbol_examples"].append(symbol)

rows = []
for (consumer_rpm, provider_rpm), meta in source_meta.items():
    key = (meta["consumer_source"], meta["provider_source"])
    aggregate = aggregates[key]
    consumer_decision = decisions.get(
        meta["consumer_source"], "NON_CPP_OR_NOT_IN_R100_CANDIDATES"
    )
    provider_decision = decisions.get(
        meta["provider_source"], "NON_CPP_OR_NOT_IN_R100_CANDIDATES"
    )
    rows.append(
        {
            **meta,
            "classification": "LAYOUT_SENSITIVE_STD_TYPE",
            "elf_edge_count": aggregate["elf_edge_count"],
            "cpp_symbol_count": aggregate["cpp_symbol_count"],
            "consumer_r100_decision": consumer_decision,
            "provider_r100_decision": provider_decision,
            "need_consumer_to_no_provider": (
                "YES"
                if consumer_decision == "NEED_LIBCXX"
                and provider_decision == "NO_LIBCXX_NEEDED"
                else "NO"
            ),
            "symbol_examples": ";".join(aggregate["symbol_examples"][:5]),
        }
    )

fieldnames = [
    "consumer_source",
    "provider_source",
    "consumer_sourcerpm",
    "provider_sourcerpm",
    "classification",
    "elf_edge_count",
    "cpp_symbol_count",
    "consumer_r100_decision",
    "provider_r100_decision",
    "need_consumer_to_no_provider",
    "symbol_examples",
]
output.parent.mkdir(parents=True, exist_ok=True)
with open(output, "w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(
        f, delimiter="\t", fieldnames=fieldnames, lineterminator="\n"
    )
    writer.writeheader()
    writer.writerows(rows)

print(f"layout_sensitive_source_edges={len(rows)}")
print(
    "need_consumer_to_no_provider="
    + str(sum(r["need_consumer_to_no_provider"] == "YES" for r in rows))
)
for row in rows:
    print(
        f"{row['consumer_source']} -> {row['provider_source']} "
        f"consumer={row['consumer_r100_decision']} "
        f"provider={row['provider_r100_decision']}"
    )
