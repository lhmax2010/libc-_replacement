#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path

root = Path(__file__).resolve().parents[3]
tables = root / "progress/R32/tables"

with (tables / "impact_package_classification.tsv").open(newline="") as f:
    packages = list(csv.DictReader(f, delimiter="\t"))
with (tables / "impact_locations.tsv").open(newline="") as f:
    locations = list(csv.DictReader(f, delimiter="\t"))
with (tables / "forced_unwind_guard_locations.tsv").open(newline="") as f:
    guard_locations = list(csv.DictReader(f, delimiter="\t"))

category_by_srpm = {r["source_rpm"]: r["category"] for r in packages}
stage = Counter((r["stage"], r["category"]) for r in packages)

def write(name, header, rows):
    with (tables / name).open("w", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(header)
        w.writerows(rows)

write("impact_by_stage.tsv",
      ["stage", "category", "source_package_count", "denominator_scanned_source_packages"],
      [(s, c, n, 371) for (s, c), n in sorted(stage.items())])

category_locations = []
for r in locations:
    category = category_by_srpm[r["source_rpm"]]
    include = False
    if category in ("UNGUARDED_RISK_PATTERN", "CATCH_ONLY_NO_THREAD_EVIDENCE") and r["feature"] == "catch_all":
        include = True
    if include:
        category_locations.append((category, r["source_rpm"], r["package_name"],
                                   r["stage"], r["feature"], r["file"], r["line"],
                                   r["scope"], r["macro_name"], r["source_text"]))
for r in guard_locations:
    if (category_by_srpm[r["source_rpm"]] == "EXISTING_GUARD"
            and r["guard_status"] == "CONFIRMED_RETHROW"
            and r["scope"] != "DOC"):
        category_locations.append((
            "EXISTING_GUARD", r["source_rpm"], r["package_name"], r["stage"],
            "forced_unwind_confirmed_rethrow", r["file"], r["line"], r["scope"],
            "-", r["source_text"]))
write("impact_category_locations.tsv",
      ["category", "source_rpm", "package_name", "stage", "feature", "file",
       "line", "scope", "macro_name", "source_text"], category_locations)

package_counts = Counter(r["category"] for r in packages)
location_counts = Counter(r[0] for r in category_locations)
write("impact_summary_detailed.tsv",
      ["category", "source_package_count", "relevant_location_count",
       "denominator_source_packages", "location_count_rule"],
      [(c, package_counts[c], location_counts[c], 371,
        "EXISTING_GUARD counts confirmed non-DOC catch-and-rethrow locations; other risk categories count catch-all locations")
       for c in sorted(package_counts)])

write("existing_guard_packages.tsv",
      ["source_rpm", "package_name", "stage", "confirmed_guard_locations_nondoc",
       "confirmed_guard_locations_production_like", "forced_catch_without_rethrow_locations_nondoc"],
      [(r["source_rpm"], r["package_name"], r["stage"],
        r["confirmed_guard_locations_nondoc"], r["confirmed_guard_locations_production_like"],
        r["forced_catch_without_rethrow_locations_nondoc"])
       for r in packages if r["category"] == "EXISTING_GUARD"])
