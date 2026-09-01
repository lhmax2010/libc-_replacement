#!/usr/bin/env python3
import csv
import glob
import subprocess
from pathlib import Path

inputs = sorted(glob.glob("docs/progress/R22/tables/split/candidate_edge_evidence.part*.tsv"))
rows = []
for path in inputs:
    with open(path, newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["provider_source_rpm"].startswith("boost-") and row["classification"] == "TRUE_CPP_ABI_COUPLING":
                row["source_table"] = path
                rows.append(row)

def demangle(name: str) -> str:
    return subprocess.check_output(["c++filt", name], text=True).rstrip("\n")

outdir = Path("progress/R82/tables")
outdir.mkdir(parents=True, exist_ok=True)
edge_fields = [
    "edge_id", "consumer_source_rpm", "arch", "consumer_binary", "consumer_elf",
    "provider_binary", "provider_elf", "needed_soname", "cpp_symbol_count", "cpp_symbols",
    "source_table",
]
with (outdir / "boost_edges_90.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.DictWriter(stream, fieldnames=edge_fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    for index, row in enumerate(rows, 1):
        record = {field: row.get(field, "") for field in edge_fields}
        record["edge_id"] = f"E{index:03d}"
        writer.writerow(record)

symbols = {}
for row in rows:
    for symbol in filter(None, row["cpp_symbols"].split(";")):
        symbols[symbol] = demangle(symbol)
with (outdir / "boost_distinct_symbols.tsv").open("w", newline="", encoding="utf-8") as stream:
    writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
    writer.writerow(["mangled", "demangled"])
    for mangled, text in sorted(symbols.items(), key=lambda item: (item[1], item[0])):
        writer.writerow([mangled, text])

print(f"input_parts={len(inputs)}")
print(f"edge_rows={len(rows)}")
print(f"distinct_symbols={len(symbols)}")
print("providers=" + ";".join(sorted({row["provider_binary"] for row in rows})))
print("consumers=" + str(len({row["consumer_source_rpm"] for row in rows})))
print("consumer_elfs=" + str(len({(row["arch"], row["consumer_rpm_sha256"], row["consumer_elf"]) for row in rows})))
