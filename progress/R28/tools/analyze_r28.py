#!/usr/bin/env python3
import csv
import hashlib
import sys
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "progress/R28"
TABLES = OUT / "tables"
INPUTS = OUT / "inputs"
TABLES.mkdir(parents=True, exist_ok=True)
INPUTS.mkdir(parents=True, exist_ok=True)

PATHS = {
    "r11_binary_records": ROOT / "progress/R11/tables/binary_package_records.tsv",
    "r11_snapshot": ROOT / "progress/R11/tables/snapshot_identity.tsv",
    "r22_elf_edge_evidence": ROOT / "progress/R22/tables/candidate_edge_evidence.tsv",
    "r24_packages": ROOT / "progress/R24/tables/migration_packages_483.tsv",
    "r25_vendor_accounting": ROOT / "progress/R25/tables/vendor_five_to_four_accounting.tsv",
    "r27_permanent_edges": ROOT / "progress/R27/tables/permanent_edges_12.tsv",
    "r27_permanent_members": ROOT / "progress/R27/tables/permanent_closure_members.tsv",
}

def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)

def sha256(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()

packages = read_tsv(PATHS["r24_packages"])
pkg = {r["source_rpm_identity"]: r for r in packages}
permanent = read_tsv(PATHS["r27_permanent_edges"])
members = read_tsv(PATHS["r27_permanent_members"])
evidence = read_tsv(PATHS["r22_elf_edge_evidence"])
binary_records = read_tsv(PATHS["r11_binary_records"])

if len(permanent) != 12:
    raise SystemExit("R27 permanent edge denominator mismatch")
gcc_edges = [r for r in permanent if r["provider_package_name"] == "gcc"]
gcc_keys = {(r["consumer_source_rpm"], r["provider_source_rpm"]) for r in gcc_edges}
gcc_evidence = [r for r in evidence if (r["consumer_source_rpm"], r["provider_source_rpm"]) in gcc_keys and r["classification"] == "TRUE_CPP_ABI_COUPLING"]

elf_rows = []
symbol_rows = []
for r in sorted(gcc_evidence, key=lambda x: (x["provider_source_rpm"], x["arch"], x["consumer_source_rpm"], x["consumer_binary"], x["consumer_elf"])):
    symbols = [s for s in r["cpp_symbols"].split(";") if s]
    elf_rows.append({
        "consumer_source_rpm": r["consumer_source_rpm"],
        "consumer_binary_package": r["consumer_binary"],
        "arch": r["arch"],
        "consumer_elf": r["consumer_elf"],
        "provider_source_rpm": r["provider_source_rpm"],
        "needed_soname": r["needed_soname"],
        "provider_binary_package": r["provider_binary"],
        "provider_elf": r["provider_elf"],
        "provider_rpm_sha256": r["provider_rpm_sha256"],
        "cpp_symbol_count": len(symbols),
        "all_symbols_from_libstdcxx": "YES" if r["needed_soname"] == "libstdc++.so.6" and r["provider_binary"] == "libstdc++" and "libstdc++.so.6" in r["provider_elf"] else "NO",
    })
    for symbol in symbols:
        symbol_rows.append({
            "symbol": symbol,
            "consumer_source_rpm": r["consumer_source_rpm"],
            "consumer_binary_package": r["consumer_binary"],
            "arch": r["arch"],
            "consumer_elf": r["consumer_elf"],
            "provider_source_rpm": r["provider_source_rpm"],
            "provider_binary_package": r["provider_binary"],
            "provider_elf": r["provider_elf"],
            "needed_soname": r["needed_soname"],
        })
write_tsv(TABLES / "gcc_edge_elf_evidence.tsv", list(elf_rows[0]), elf_rows)
write_tsv(TABLES / "gcc_symbols_by_consumer_binary.tsv", list(symbol_rows[0]), symbol_rows)

symbol_summary = []
by_symbol = defaultdict(list)
for r in symbol_rows:
    by_symbol[r["symbol"]].append(r)
for symbol, rows in sorted(by_symbol.items()):
    symbol_summary.append({
        "symbol": symbol,
        "consumer_source_rpm_count": len({r["consumer_source_rpm"] for r in rows}),
        "consumer_binary_package_count": len({(r["consumer_binary_package"], r["arch"]) for r in rows}),
        "consumer_source_rpms": ";".join(sorted({r["consumer_source_rpm"] for r in rows})),
        "consumer_binary_packages": ";".join(sorted({f"{r['consumer_binary_package']}[{r['arch']}]" for r in rows})),
        "provider_source_rpms": ";".join(sorted({r["provider_source_rpm"] for r in rows})),
        "provider_binary_package": "libstdc++",
        "provider_soname": "libstdc++.so.6",
    })
write_tsv(TABLES / "gcc_distinct_symbol_summary.tsv", list(symbol_summary[0]), symbol_summary)

gcc_sources = sorted({r["provider_source_rpm"] for r in gcc_edges})
distribution_rows = []
for source in gcc_sources:
    records = [r for r in binary_records if r["sourcerpm"] == source]
    std = [r for r in records if r["name"] == "libstdc++"]
    distribution_rows.append({
        "source_rpm": source,
        "repo_ids": ";".join(sorted({r["repo_id"] for r in records})),
        "architectures": ";".join(sorted({r["arch"] for r in records})),
        "binary_package_record_count": len(records),
        "libstdcxx_nevras": ";".join(sorted(f"{r['name']}-{r['version']}-{r['release']}.{r['arch']}" for r in std)),
        "libstdcxx_sha256": ";".join(sorted(r["checksum"] for r in std)),
        "identity_interpretation": "ARCH_SPECIFIC_RELEASE_NOT_DUPLICATE" if source.endswith("1.13.src.rpm") or source.endswith("1.15.src.rpm") else "UNDETERMINED",
    })
write_tsv(TABLES / "gcc_source_identity_distribution.tsv", list(distribution_rows[0]), distribution_rows)

edge_rows = []
for r in sorted(permanent, key=lambda x: (x["consumer_source_rpm"], x["provider_source_rpm"])):
    provider_kind = "LIBSTDCXX_RUNTIME" if r["provider_package_name"] == "gcc" else "NON_GCC_PLATFORM_PROVIDER"
    edge_rows.append({
        "consumer_source_rpm": r["consumer_source_rpm"],
        "consumer_package_name": r["consumer_package_name"],
        "provider_source_rpm": r["provider_source_rpm"],
        "provider_package_name": r["provider_package_name"],
        "semantic_class": r["semantic_class"],
        "cpp_symbol_count": r["cpp_symbol_count"],
        "provider_kind_after_R28": provider_kind,
    })
write_tsv(TABLES / "permanent_edges_12_consumer_provider.tsv", list(edge_rows[0]), edge_rows)

consumer_rows = []
for consumer in sorted({r["consumer_source_rpm"] for r in permanent}):
    rows = [r for r in permanent if r["consumer_source_rpm"] == consumer]
    nongcc = sorted({r["provider_source_rpm"] for r in rows if r["provider_package_name"] != "gcc"})
    gcc = sorted({r["provider_source_rpm"] for r in rows if r["provider_package_name"] == "gcc"})
    consumer_rows.append({
        "vendor_consumer_source_rpm": consumer,
        "vendor_package_name": pkg[consumer]["package_name"],
        "permanent_edge_count": len(rows),
        "all_provider_source_rpms": ";".join(sorted({r["provider_source_rpm"] for r in rows})),
        "gcc_provider_source_rpms": ";".join(gcc),
        "non_gcc_provider_source_rpms": ";".join(nongcc),
        "consumer_pattern": "CONSUMES_NON_GCC_AND_GCC" if nongcc and gcc else ("GCC_ONLY" if gcc else "NON_GCC_ONLY"),
    })
write_tsv(TABLES / "vendor_consumer_distribution.tsv", list(consumer_rows[0]), consumer_rows)

corrected = [r for r in members if r["package_name"] != "gcc"]
corrected_rows = []
for r in sorted(corrected, key=lambda x: x["source_rpm"]):
    consumers = sorted({x["consumer_source_rpm"] for x in permanent if x["provider_source_rpm"] == r["source_rpm"]})
    corrected_rows.append({
        "platform_provider_source_rpm": r["source_rpm"],
        "package_name": r["package_name"],
        "stage": r["stage"],
        "permanent_vendor_consumers": ";".join(consumers),
        "permanent_old_build_required": "YES",
        "basis": "NON_GCC_PROVIDER_ON_CONFIRMED_PERMANENT_CXX_EDGE",
    })
write_tsv(TABLES / "corrected_permanent_platform_providers.tsv", list(corrected_rows[0]), corrected_rows)

vendor_sources = {r["consumer_source_rpm"] for r in permanent}
accounting = [
    {"item": "R27_platform_provider_source_identities", "count": len(members), "unit": "source RPM identity", "detail": "includes two architecture-specific gcc source identities"},
    {"item": "R28_excluded_gcc_source_identities", "count": len([r for r in members if r["package_name"] == "gcc"]), "unit": "source RPM identity", "detail": "both provide libstdc++.so.6; not dual libc++/libstdc++ provider builds"},
    {"item": "R28_corrected_platform_provider_source_identities", "count": len(corrected), "unit": "source RPM identity", "detail": ";".join(sorted(r["source_rpm"] for r in corrected))},
    {"item": "vendor_permanent_source_identities", "count": len(vendor_sources), "unit": "source RPM identity", "detail": ";".join(sorted(vendor_sources))},
    {"item": "corrected_permanent_total_source_identities", "count": len(corrected) + len(vendor_sources), "unit": "source RPM identity", "detail": "platform providers plus vendor sources"},
]
write_tsv(TABLES / "corrected_permanent_accounting.tsv", list(accounting[0]), accounting)

input_rows = []
for role, path in PATHS.items():
    input_rows.append({"role": role, "path": str(path.relative_to(ROOT)), "sha256": sha256(path), "size_bytes": path.stat().st_size})
write_tsv(INPUTS / "input_identity.tsv", list(input_rows[0]), input_rows)

checks = [
    ("permanent_edges_12", len(permanent) == 12, len(permanent)),
    ("gcc_source_edges_8", len(gcc_edges) == 8, len(gcc_edges)),
    ("gcc_elf_evidence_rows_18", len(gcc_evidence) == 18, len(gcc_evidence)),
    ("all_gcc_evidence_is_libstdcxx", all(r["all_symbols_from_libstdcxx"] == "YES" for r in elf_rows), Counter((r["needed_soname"], r["provider_binary_package"], r["provider_elf"]) for r in elf_rows)),
    ("gcc_identity_13_x86_only", next(r for r in distribution_rows if "1.13" in r["source_rpm"])["architectures"] == "noarch;x86_64", next(r for r in distribution_rows if "1.13" in r["source_rpm"])["architectures"]),
    ("gcc_identity_15_aarch64_armv7l", next(r for r in distribution_rows if "1.15" in r["source_rpm"])["architectures"] == "aarch64;armv7l;noarch", next(r for r in distribution_rows if "1.15" in r["source_rpm"])["architectures"]),
    ("vendor_consumers_4", len(consumer_rows) == 4, len(consumer_rows)),
    ("only_hal_api_has_non_gcc", [r["vendor_consumer_source_rpm"] for r in consumer_rows if r["non_gcc_provider_source_rpms"]] == ["hal-api-drm-1.0.5-1.src.rpm"], [r["vendor_consumer_source_rpm"] for r in consumer_rows if r["non_gcc_provider_source_rpms"]]),
    ("corrected_platform_providers_4", len(corrected) == 4, [r["source_rpm"] for r in corrected]),
    ("gtest_retained_with_hal_api_consumer", any(r["package_name"] == "gtest" and r["permanent_vendor_consumers"] == "hal-api-drm-1.0.5-1.src.rpm" for r in corrected_rows), [r for r in corrected_rows if r["package_name"] == "gtest"]),
    ("corrected_total_8", len(corrected) + len(vendor_sources) == 8, len(corrected) + len(vendor_sources)),
]
validation_rows = [{"assertion": name, "observed": str(observed), "status": "PASS" if ok else "FAIL"} for name, ok, observed in checks]
write_tsv(OUT / "validation.tsv", list(validation_rows[0]), validation_rows)
if not all(ok for _, ok, _ in checks):
    raise SystemExit("validation failed")

print(f"gcc_source_edges={len(gcc_edges)} elf_evidence_rows={len(gcc_evidence)} exploded_symbol_rows={len(symbol_rows)} distinct_symbols={len(symbol_summary)}")
print(f"vendor_consumers={len(consumer_rows)} non_gcc_consumer={[r['vendor_consumer_source_rpm'] for r in consumer_rows if r['non_gcc_provider_source_rpms']]}")
print(f"corrected_platform_providers={len(corrected)} corrected_total_sources={len(corrected) + len(vendor_sources)}")
