#!/usr/bin/env python3
"""Read-only mechanical audit of the R113 delivery archive contents."""

from __future__ import annotations

import csv
import hashlib
import re
from collections import Counter
from pathlib import Path


ROOT = Path("/home/toolchain/development/libc++_replacement")
DELIVERY = Path("/tmp/r113_hq_20260904")


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


expected = {
    "weekly_report_20260904.md",
    "base_adaptation_report.md",
    "base_method_explained.md",
    "base_packages_final.md",
    "base_packages_final.tsv",
    "attach_50_row_audit.tsv",
    "attach_172_row_sites.tsv",
}
actual = {p.name for p in DELIVERY.iterdir() if p.name != "README.md"}
print(f"DELIVERY_FILES expected={len(expected)} actual={len(actual)} match={actual == expected}")
for path in sorted(DELIVERY.iterdir()):
    if path.is_file():
        print(f"SHA256 {path.name} {sha256(path)}")

packages = tsv(DELIVERY / "base_packages_final.tsv")
print(f"BASE_ROWS {len(packages)}")
print("BASE_DECISIONS", dict(sorted(Counter(r["判定结果"] for r in packages).items())))
print("BASE_IMPLEMENTATION", dict(sorted(Counter(r["当前实施状态"] for r in packages).items())))
print("BASE_CPP_PREFIX", dict(sorted(Counter(r["是否含C++及依据"].split("：", 1)[0] for r in packages).items())))
print("BASE_INTERNAL_R100_ROWS", sum("R100" in "\t".join(r.values()) for r in packages))
internal_codes = Counter()
for row in packages:
    evidence = row["证据"]
    match = re.search(r"R100=([^；]+)", evidence)
    if match:
        internal_codes[match.group(1)] += 1
    elif "R100 分母扫描" in evidence:
        internal_codes["R100 denominator wording"] += 1
print("BASE_INTERNAL_CODES", dict(sorted(internal_codes.items())))

rows50 = tsv(DELIVERY / "attach_50_row_audit.tsv")
print(f"ATTACH50_ROWS {len(rows50)}")
print("ATTACH50_NATURE", dict(sorted(Counter(r["row_nature"] for r in rows50).items())))
print("ATTACH50_STATUS", dict(sorted(Counter(r["r32_status"] for r in rows50).items())))
print("ATTACH50_HEADER_HAS_INTERNAL_R32", "r32_status" in rows50[0])

rows172 = tsv(DELIVERY / "attach_172_row_sites.tsv")
logical = {
    (r["package_name"], r["file"], r["line"], r["clause_text"], r["rethrow_kind"])
    for r in rows172
}
print(f"ATTACH172_ROWS {len(rows172)}")
print(f"ATTACH172_LOGICAL_KEYS {len(logical)}")
print("ATTACH172_RETHROW_KIND", dict(sorted(Counter(r["rethrow_kind"] for r in rows172).items())))
print(f"ATTACH172_SOURCE_INSTANCES {len({r['source_instance'] for r in rows172})}")
print(f"ATTACH172_PACKAGE_NAMES {len({r['package_name'] for r in rows172})}")
print("ATTACH172_SOURCE_KIND", dict(sorted(Counter(r["source_kind"] for r in rows172).items())))
extensionless = [r for r in rows172 if Path(r["file"]).suffix == ""]
extensionless_logical = {
    (r["package_name"], r["file"], r["line"], r["clause_text"], r["rethrow_kind"])
    for r in extensionless
}
print(f"ATTACH172_EXTENSIONLESS occurrences={len(extensionless)} logical={len(extensionless_logical)}")

source_equivalents = {
    "base_method_explained.md": ROOT / "docs/progress/R111/BASE_LIBCXX_ADAPTATION_METHOD.md",
    "base_packages_final.md": ROOT / "docs/progress/R109/tables/base_all_source_packages_updated.md",
    "base_packages_final.tsv": ROOT / "docs/progress/R109/tables/base_all_source_packages_updated.tsv",
    "attach_50_row_audit.tsv": ROOT / "docs/progress/R110/tables/reported_50_audit.tsv",
    "attach_172_row_sites.tsv": ROOT / "docs/progress/R110/tables/strict_rethrow_sites_original.tsv",
}
for name, source in source_equivalents.items():
    delivery_path = DELIVERY / name
    print(
        "SOURCE_IDENTITY",
        name,
        f"delivery={sha256(delivery_path)}",
        f"source={sha256(source)}",
        f"identical={delivery_path.read_bytes() == source.read_bytes()}",
    )

for path in sorted(DELIVERY.iterdir()):
    if not path.is_file():
        continue
    body = path.read_text(encoding="utf-8", errors="replace")
    internal = len(re.findall(r"\bR(?:95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112)\b|R32", body))
    absolute = len(re.findall(r"(?:/home/toolchain|/tmp/|docs/progress/)", body))
    print(f"MARKERS {path.name} internal={internal} absolute_or_progress_path={absolute}")

normalized = tsv(ROOT / "docs/progress/R106/tables/normalized_binary_records.tsv")
boost_toolchain = {
    r["name"]
    for r in normalized
    if r["repo"] == "Tizen-Base-Toolchain" and r["source_name"] == "boost"
}
print(f"BOOST_TOOLCHAIN_UNIQUE_BINARY_NAMES {len(boost_toolchain)}")

matrix = tsv(ROOT / "docs/progress/R108/ADJUSTED_MATRIX.tsv")
print(f"TENSORFLOW_MATRIX_ROWS {len(matrix)}")
for row in matrix:
    print("TENSORFLOW_MATRIX", row)

print("AUDIT_COMPLETE")
