#!/usr/bin/env python3
import csv
import pathlib
import re
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[3]
closure = ROOT / "progress/R27/tables/closure_members.tsv"
sources = ROOT / "progress/R11/tables/source_package_records.tsv"
binaries = ROOT / "progress/R11/tables/binary_package_records.tsv"
out = ROOT / "progress/R29/tables/provider_applicability.tsv"
summary = ROOT / "progress/R29/tables/provider_applicability_summary.tsv"

def rows(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

closure_rows = rows(closure)
source_by_rpm = {}
for row in rows(sources):
    source_by_rpm.setdefault(row["location"], []).append(row)
binary_by_source = {}
for row in rows(binaries):
    binary_by_source.setdefault(row["sourcerpm"], []).append(row)

result = []
for member in closure_rows:
    srpm = member["source_rpm"]
    candidates = source_by_rpm.get(srpm, [])
    # Prefer the source record whose repository matches the binary mapping.
    source = candidates[0] if candidates else None
    spec_paths = []
    if source:
        checksum = source["checksum"]
        d = ROOT / "tmp/R11/source_specs_v3" / checksum[:2] / checksum
        if d.is_dir():
            spec_paths = sorted(d.glob("*.spec"))
    text = "\n".join(p.read_text(errors="replace") for p in spec_paths)
    kinds = []
    if re.search(r"(?im)(?:^|\s)(?:%\{?cmake\}?|cmake)\s", text):
        kinds.append("CMAKE")
    if re.search(r"(?im)(?:^|\s)(?:%\{?meson\}?|meson\s+(?:setup|configure))", text):
        kinds.append("MESON")
    if re.search(r"(?im)(?:%configure|autoreconf|autoconf|./configure)", text):
        kinds.append("AUTOTOOLS")
    build_system = "+".join(kinds) if kinds else ("OTHER" if spec_paths else "NOT_AVAILABLE")

    package_rows = binary_by_source.get(srpm, [])
    paths = []
    rpm_failures = 0
    for b in package_rows:
        rpm_path = ROOT / "tmp/R11/rpms" / b["repo_id"] / b["location"]
        if not rpm_path.is_file():
            rpm_failures += 1
            continue
        proc = subprocess.run(["rpm", "-qlp", str(rpm_path)], text=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if proc.returncode:
            rpm_failures += 1
        else:
            paths.extend(proc.stdout.splitlines())
    has_static = any(p.endswith(".a") for p in paths)
    has_pc = any(p.endswith(".pc") for p in paths)
    has_cmake = any(re.search(r"(?i)(config|targets)(?:-release)?\.cmake$", p) for p in paths)
    result.append({
        "source_rpm": srpm,
        "package": member["package_name"],
        "stage": member["stage"],
        "stage_layer": member["stage_layer"],
        "build_system": build_system,
        "spec_count": str(len(spec_paths)),
        "spec_paths": ";".join(str(p.relative_to(ROOT)) for p in spec_paths) or "NOT_AVAILABLE",
        "binary_package_records": str(len(package_rows)),
        "rpm_qlp_failures": str(rpm_failures),
        "has_static_library": "YES" if has_static else "NO",
        "has_pkg_config": "YES" if has_pc else "NO",
        "has_cmake_config": "YES" if has_cmake else "NO",
    })

fields = list(result[0])
with out.open("w", newline="", encoding="utf-8") as f:
    w = csv.DictWriter(f, fields, delimiter="\t", lineterminator="\n")
    w.writeheader(); w.writerows(result)

with summary.open("w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["metric", "numerator", "denominator", "scope"])
    for kind in ["AUTOTOOLS", "CMAKE", "MESON", "OTHER", "NOT_AVAILABLE"]:
        n = sum(kind in r["build_system"].split("+") for r in result)
        w.writerow([f"build_system_{kind.lower()}", n, len(result), "R27 closure source RPM identities"])
    for key in ["has_static_library", "has_pkg_config", "has_cmake_config"]:
        n = sum(r[key] == "YES" for r in result)
        w.writerow([key, n, len(result), "existing binary RPM payloads; all architectures/records mapped to each source identity"])
    w.writerow(["rpm_qlp_failures", sum(int(r["rpm_qlp_failures"]) for r in result),
                sum(int(r["binary_package_records"]) for r in result), "binary RPM records"])
