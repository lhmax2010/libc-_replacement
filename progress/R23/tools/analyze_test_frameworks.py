#!/usr/bin/env python3
import csv
import re
import shlex
import subprocess
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path.cwd()
OUT = ROOT / "progress/R23/tables"
OUT.mkdir(parents=True, exist_ok=True)

def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(name, fields, rows):
    with (OUT / name).open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader(); w.writerows(rows)

migration = read_tsv(ROOT / "progress/R22/tables/migration_packages_full.tsv")
sources = read_tsv(ROOT / "progress/R11/tables/source_package_records.tsv")
relations = read_tsv(ROOT / "progress/R11/tables/source_relations.tsv")
downloads = read_tsv(ROOT / "progress/R11/tables/binary_download_status.tsv")

phase_by_nevra = {r["source_rpm"].removesuffix(".rpm"): r for r in migration}
phase1 = sorted((r for r in migration if r["stage"] == "PHASE1"), key=lambda r: (r["package_name"], r["source_rpm"]))
write_tsv("phase1_source_packages.tsv",
          ["source_rpm", "package_name", "stage", "binary_package_count", "remarks"],
          [{k: r[k] for k in ["source_rpm", "package_name", "stage", "binary_package_count", "remarks"]} for r in phase1])

# Explicit catalog terms are recorded rather than silently expanding “similar”.
catalog_re = re.compile(r"(?:^|[-_])(gtest|gmock|googletest|catch2|cppunit|doctest)(?:$|[-_])", re.I)
candidates = [r for r in sources if catalog_re.search(r["name"])]
framework_rows = []
for r in candidates:
    src_key = f'{r["name"]}-{r["version"]}-{r["release"]}.src'
    m = phase_by_nevra.get(src_key)
    framework_rows.append({
        "framework_source_name": r["name"], "source_nevra": src_key,
        "repo_id": r["repo_id"], "stage_in_R22": m["stage"] if m else "NOT_IN_R22_483_SCOPE",
        "R22_source_rpm": m["source_rpm"] if m else "NOT_AVAILABLE",
        "catalog_match_basis": "SOURCE_NAME_REGEX:gtest|gmock|googletest|catch2|cppunit|doctest",
    })
# gmock is provided by the gtest source package in this snapshot; preserve that as a
# binary/provides fact rather than inventing a separate source package.
write_tsv("test_framework_stage_assignment.tsv",
          ["framework_source_name", "source_nevra", "repo_id", "stage_in_R22", "R22_source_rpm", "catalog_match_basis"],
          framework_rows)

framework_source_rpms = {f'{r["name"]}-{r["version"]}-{r["release"]}.src.rpm' for r in candidates}
rpm_rows = [r for r in downloads if r["sourcerpm"] in framework_source_rpms]
file_rows, command_rows, shape_rows = [], [], []
for seq, r in enumerate(sorted(rpm_rows, key=lambda x: (x["name"], x["arch"])), 1):
    cmd = ["rpm", "-qlp", r["target"]]
    cp = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                        text=True, errors="replace", check=False)
    command_rows.append({"sequence": seq, "command": " ".join(shlex.quote(x) for x in cmd),
                         "exit_code": cp.returncode, "package": r["name"], "arch": r["arch"]})
    classes = Counter()
    paths = []
    if cp.returncode == 0:
        paths = [line for line in cp.stdout.splitlines() if line.startswith("/")]
    else:
        paths = ["RPM_QUERY_FAILED:" + cp.stdout.replace("\t", " ").replace("\n", " | ")[:1000]]
    for path in paths:
        if path.endswith(".a"):
            cls = "STATIC_LIBRARY"
        elif re.search(r"\.so(?:\.|$)", path):
            cls = "SHARED_LIBRARY_OR_LINK"
        elif path.endswith((".h", ".hpp", ".hh")):
            cls = "HEADER"
        elif path.endswith((".cc", ".cpp", ".cxx", ".c")):
            cls = "SOURCE"
        elif path.endswith(".pc"):
            cls = "PKGCONFIG"
        elif "cmake" in path.lower():
            cls = "CMAKE_METADATA"
        else:
            cls = "OTHER"
        classes[cls] += 1
        file_rows.append({"package_name": r["name"], "arch": r["arch"], "nevra": f'{r["name"]}-{r["version"]}-{r["release"]}.{r["arch"]}',
                          "rpm_sha256": r["actual_sha256"], "path": path, "file_path_class": cls})
    shape_rows.append({"package_name": r["name"], "arch": r["arch"],
                       "nevra": f'{r["name"]}-{r["version"]}-{r["release"]}.{r["arch"]}',
                       "rpm_sha256": r["actual_sha256"], "rpm_query_exit_code": cp.returncode,
                       "file_count": len(paths), "static_library_count": classes["STATIC_LIBRARY"],
                       "shared_library_or_link_count": classes["SHARED_LIBRARY_OR_LINK"],
                       "source_file_count": classes["SOURCE"], "header_count": classes["HEADER"],
                       "packaging_shape": ("STATIC_LIBRARY" if classes["STATIC_LIBRARY"] and not classes["SHARED_LIBRARY_OR_LINK"] else
                                           "SHARED_LIBRARY" if classes["SHARED_LIBRARY_OR_LINK"] and not classes["STATIC_LIBRARY"] else
                                           "STATIC_AND_SHARED" if classes["STATIC_LIBRARY"] and classes["SHARED_LIBRARY_OR_LINK"] else
                                           "SOURCE_OR_HEADERS_ONLY" if classes["SOURCE"] or classes["HEADER"] else "NO_LIBRARY_PAYLOAD_IDENTIFIED")})

write_tsv("test_framework_rpm_query_commands.tsv", ["sequence", "command", "exit_code", "package", "arch"], command_rows)
write_tsv("test_framework_payload_files.tsv", ["package_name", "arch", "nevra", "rpm_sha256", "path", "file_path_class"], file_rows)
write_tsv("test_framework_packaging_shape.tsv", ["package_name", "arch", "nevra", "rpm_sha256", "rpm_query_exit_code", "file_count", "static_library_count", "shared_library_or_link_count", "source_file_count", "header_count", "packaging_shape"], shape_rows)

relation_names = {"gtest", "gtest-devel", "gmock", "gmock-devel", "pkgconfig(gtest)", "pkgconfig(gmock)"}
consumers = [r for r in relations if r["kind"] == "source" and r["relation_type"] == "requires" and r["relation_name"] in relation_names]
consumer_rows = []
for r in consumers:
    m = phase_by_nevra.get(r["package_nevra"])
    consumer_rows.append({
        "source_package_nevra": r["package_nevra"], "source_package_name": r["package_name"],
        "repo_id": r["repo_id"], "buildrequire_name": r["relation_name"],
        "stage_in_R22": m["stage"] if m else "NOT_IN_R22_483_SCOPE",
        "counting_basis": "R11_SOURCE_RPM_REQUIRES_METADATA_AS_BUILDREQUIRES",
    })
consumer_rows.sort(key=lambda x: (x["source_package_nevra"], x["buildrequire_name"]))
write_tsv("test_framework_buildrequires_consumers.tsv", ["source_package_nevra", "source_package_name", "repo_id", "buildrequire_name", "stage_in_R22", "counting_basis"], consumer_rows)

unique = {}
for r in consumer_rows:
    unique[r["source_package_nevra"]] = r["stage_in_R22"]
counts = Counter(unique.values())
summary = [{"scope": "ALL_R11_SOURCE_PACKAGE_INSTANCES", "source_package_denominator": len(sources),
            "buildrequires_framework_source_package_count": len(unique),
            "phase1_count": counts["PHASE1"], "phase2_count": counts["PHASE2"],
            "phase3_count": counts["PHASE3"], "vendor_separate_count": counts["VENDOR_PREBUILT_SEPARATE"],
            "not_in_R22_483_scope_count": counts["NOT_IN_R22_483_SCOPE"]}]
write_tsv("test_framework_buildrequires_summary.tsv", list(summary[0]), summary)

vendored = [{"method": "R11_RETAINED_DATA_REVIEW", "searched_inputs": "source_spec_inventory.tsv;source_spec_text_hits.tsv;R11 did not retain or index full source-tree add_subdirectory calls",
             "identified_vendored_source_package_count": "NOT_AVAILABLE", "status": "NOT_AVAILABLE_FULL_SOURCE_CONTENT_NOT_RETAINED",
             "limitation": "Cannot identify vendored/add_subdirectory use without a full-source token corpus; no platform rescan or new source extraction was performed."}]
write_tsv("vendored_test_framework_status.tsv", list(vendored[0]), vendored)

print("phase1", len(phase1), "framework_sources", len(framework_rows), "framework_rpms", len(rpm_rows))
print("buildrequires_unique_sources", len(unique), "stage_counts", dict(sorted(counts.items())))
print("rpm_query_failures", sum(r["rpm_query_exit_code"] != 0 for r in shape_rows))
