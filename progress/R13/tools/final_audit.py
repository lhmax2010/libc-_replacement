#!/usr/bin/env python3
import csv
import hashlib
import re
from collections import Counter
from pathlib import Path


ROOT = Path("/home/toolchain/development/libc++_replacement")
OUT = ROOT / "progress/R13"


def rows(path):
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def sha(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


checks = []


def check(name, condition, evidence):
    checks.append({"assertion": name, "status": "PASS" if condition else "FAIL", "evidence": evidence})


identities = rows(OUT / "inputs/input_identity.tsv")
bad_hash = []
for item in identities:
    path = ROOT / item["path"]
    actual = sha(path) if path.is_file() else "NOT_FOUND"
    if actual != item["sha256"]:
        bad_hash.append(f"{item['path']}:{actual}")
check("all_frozen_input_sha256_unchanged", not bad_hash, "mismatch=" + (";".join(bad_hash) or "NONE"))

edges = rows(OUT / "tables/dependency_edge_resolution.tsv")
edge_counts = Counter(row["resolution_status"] for row in edges)
check("dt_needed_edge_count", len(edges) == 151403, f"actual={len(edges)} expected=151403")
check(
    "edge_resolution_partition",
    edge_counts["RESOLVED_UNIQUE"] == 138236 and len(edges) - edge_counts["RESOLVED_UNIQUE"] == 13167,
    f"resolved={edge_counts['RESOLVED_UNIQUE']} unresolved={len(edges)-edge_counts['RESOLVED_UNIQUE']}",
)

elfs = rows(OUT / "tables/elf_closure_classification.tsv")
elf_classes = Counter(row["libgcc_s_classification"] for row in elfs)
check("runtime_elf_count", len(elfs) == 36829, f"actual={len(elfs)} expected=36829")
check(
    "elf_classification_partition",
    elf_classes == Counter({"DIRECT": 12722, "TRANSITIVE": 3390, "ABSENT": 12293, "UNDETERMINED": 8424}),
    repr(dict(elf_classes)),
)

packages = rows(OUT / "tables/package_closure_classification.tsv")
package_classes = Counter(row["libgcc_s_classification"] for row in packages)
check("runtime_package_count", len(packages) == 6064, f"actual={len(packages)} expected=6064")
check(
    "package_classification_partition",
    package_classes == Counter({"DIRECT": 3607, "TRANSITIVE": 977, "ABSENT": 667, "UNDETERMINED": 813}),
    repr(dict(package_classes)),
)
check("risk_package_list_count", len(rows(OUT / "tables/risk_package_set.tsv")) == 4584, "expected=4584")
check("clean_package_list_count", len(rows(OUT / "tables/clean_package_set.tsv")) == 667, "expected=667")

risk_strata = rows(OUT / "tables/risk_package_strata.tsv")
risk_strata_counts = Counter(row["risk_stratum"] for row in risk_strata)
check(
    "risk_package_strata_partition",
    risk_strata_counts == Counter({
        "A_CPP_CONTENT_WITH_UNWIND_TABLE": 2424,
        "B_OPERATIONAL_PURE_C_WITH_UNWIND_TABLE": 2160,
    }),
    repr(dict(risk_strata_counts)),
)

plugins = rows(OUT / "tables/plugin_host_closure_assignment.tsv")
vendors = rows(OUT / "tables/vendor_prebuilt_name_summary.tsv")
c2 = rows(OUT / "tables/project_c2_closure_assignment.tsv")
vrs_all = rows(OUT / "tables/armv7l_vrs_package_assignment.tsv")
vrs_risk = rows(OUT / "tables/armv7l_risk_vrs_packages.tsv")
conflict_names = rows(OUT / "tables/libunwind_conflict_impact_seven_names.tsv")
check("plugin_input_row_count", len(plugins) == 121, f"actual={len(plugins)}")
check("five_vendor_names", len(vendors) == 5, f"actual={len(vendors)}")
check("vendor_names_all_risk_three_arch", all(row["risk_arch_records"] == "3" for row in vendors), repr(vendors))
check("project_c2_six_runtime_rows", len(c2) == 6, f"actual={len(c2)}")
check("project_c2_all_direct", all(row["libgcc_s_classification"] == "DIRECT" for row in c2), "six rows")
check("armv7l_vrs_total_and_risk", len(vrs_all) == 20 and len(vrs_risk) == 19, f"all={len(vrs_all)} risk={len(vrs_risk)}")
check("libunwind_conflict_seven_names", len(conflict_names) == 7, f"actual={len(conflict_names)}")

report = (OUT / "R13_report.md").read_text(encoding="utf-8")
check("report_has_no_placeholder", not re.search(r"\b(?:TODO|TBD|PLACEHOLDER)\b", report), "regex TODO|TBD|PLACEHOLDER")
check("report_declares_no_recommendation", "不做方案推荐" in report and "没有给出人日" in report, "boundary text present")

required = [
    "clean_risk_summary.tsv", "closure_coverage.tsv", "closure_distribution.tsv",
    "dependency_edge_resolution.tsv", "unresolved_dependency_edges.tsv",
    "package_closure_classification.tsv", "elf_closure_classification.tsv",
    "risk_package_set.tsv", "clean_package_set.tsv", "transitive_intermediate_top10.tsv",
    "risk_effective_strata_summary.tsv", "vendor_prebuilt_name_summary.tsv",
    "plugin_host_closure_assignment.tsv", "armv7l_vrs_package_assignment.tsv",
    "workload_mapping.tsv",
]
missing = [name for name in required if not (OUT / "tables" / name).is_file()]
check("required_output_tables_present", not missing, "missing=" + (";".join(missing) or "NONE"))

with (OUT / "tables/assertion_results.tsv").open("w", encoding="utf-8", newline="") as stream:
    writer = csv.DictWriter(stream, fieldnames=["assertion", "status", "evidence"], delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(checks)

for item in checks:
    print(f"{item['assertion']}={item['status']}\t{item['evidence']}")
failures = [item for item in checks if item["status"] != "PASS"]
print(f"ASSERTION_COUNT={len(checks)}")
print(f"FAILURE_COUNT={len(failures)}")
raise SystemExit(1 if failures else 0)
