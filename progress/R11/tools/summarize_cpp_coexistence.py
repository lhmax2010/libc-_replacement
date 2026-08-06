#!/usr/bin/env python3
import argparse
import collections
import csv


def read_tsv(path):
    with open(path, encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def classify(values):
    if values == {"YES", "NO"}:
        return "PACKAGE_CONTAINS_CPP_AND_OPERATIONAL_PURE_C_ELFS"
    if values == {"YES"}:
        return "PACKAGE_CONTAINS_CPP_ELFS_ONLY"
    return "PACKAGE_CONTAINS_OPERATIONAL_PURE_C_ELFS_ONLY"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--elf-inventory", required=True)
    parser.add_argument("--dwarf", required=True)
    parser.add_argument("--output-package", required=True)
    parser.add_argument("--output-same-elf", required=True)
    args = parser.parse_args()
    runtime = [row for row in read_tsv(args.elf_inventory) if row["runtime_elf"] == "YES"]
    pkg_fields = ("repo_id", "name", "arch", "epoch", "version", "release", "rpm_sha256")
    package_values = collections.defaultdict(set)
    source_values = collections.defaultdict(set)
    for row in runtime:
        package_values[tuple(row[field] for field in pkg_fields)].add(row["cpp_indicator"])
        source_values[row["sourcerpm"]].add(row["cpp_indicator"])
    package_rows = []
    arches = sorted({key[2] for key in package_values})
    for arch in ["ALL"] + arches:
        selected = {key: values for key, values in package_values.items() if arch == "ALL" or key[2] == arch}
        counts = collections.Counter(classify(values) for values in selected.values())
        for category in sorted(counts):
            count = counts[category]
            package_rows.append({"scope": arch, "classification": category, "numerator": count, "denominator": len(selected), "unit": "binary_package_records_with_runtime_ELF", "ratio": f"{count/len(selected):.9f}"})
    source_counts = collections.Counter(classify(values).replace("PACKAGE_", "SOURCE_PACKAGE_") for values in source_values.values())
    for category in sorted(source_counts):
        count = source_counts[category]
        package_rows.append({"scope": "ALL", "classification": category, "numerator": count, "denominator": len(source_values), "unit": "source_package_records_with_runtime_ELF", "ratio": f"{count/len(source_values):.9f}"})
    write_tsv(args.output_package, ["scope", "classification", "numerator", "denominator", "unit", "ratio"], package_rows)

    dwarf = read_tsv(args.dwarf)
    counts = collections.Counter(row["classification"] for row in dwarf)
    measured = sum(value for key, value in counts.items() if key != "UNDETERMINED_NO_DEBUG_INFO")
    same_elf = [
        {"classification": "C_AND_CPP_COMPILE_UNITS_IN_SAME_ELF_MEASURED", "numerator": counts.get("C_AND_CPP_COMPILE_UNITS_MEASURED", 0), "denominator": measured, "unit": "runtime_ELF_with_usable_debug_language_evidence", "full_runtime_elf_denominator": len(dwarf), "status": "MEASURED_WITH_LIMITED_COVERAGE"},
        {"classification": "C_COMPILE_UNITS_ONLY_MEASURED", "numerator": counts.get("C_COMPILE_UNITS_ONLY_MEASURED", 0), "denominator": measured, "unit": "runtime_ELF_with_usable_debug_language_evidence", "full_runtime_elf_denominator": len(dwarf), "status": "MEASURED_WITH_LIMITED_COVERAGE"},
        {"classification": "CPP_COMPILE_UNITS_ONLY_MEASURED", "numerator": counts.get("CPP_COMPILE_UNITS_ONLY_MEASURED", 0), "denominator": measured, "unit": "runtime_ELF_with_usable_debug_language_evidence", "full_runtime_elf_denominator": len(dwarf), "status": "MEASURED_WITH_LIMITED_COVERAGE"},
        {"classification": "UNDETERMINED_NO_DEBUG_INFO", "numerator": counts.get("UNDETERMINED_NO_DEBUG_INFO", 0), "denominator": len(dwarf), "unit": "runtime_ELF_files", "full_runtime_elf_denominator": len(dwarf), "status": "UNDETERMINED"},
    ]
    write_tsv(args.output_same_elf, ["classification", "numerator", "denominator", "unit", "full_runtime_elf_denominator", "status"], same_elf)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
