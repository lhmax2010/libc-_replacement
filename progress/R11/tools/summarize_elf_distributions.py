#!/usr/bin/env python3
import argparse
import collections
import csv
import pathlib


PKG_FIELDS = ("repo_id", "name", "arch", "epoch", "version", "release", "rpm_sha256")


def read_tsv(path):
    with open(path, encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def pkg_key(row):
    return tuple(
        row.get("rpm_sha256", row.get("checksum", "")) if field == "rpm_sha256" else row[field]
        for field in PKG_FIELDS
    )


def elf_key(row):
    return pkg_key(row) + (row["path"],)


def write_tsv(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def count_rows_by_arch(category_to_elfs, all_packages, runtime_elfs, output):
    rows = []
    arches = ["ALL"] + sorted({row["arch"] for row in runtime_elfs})
    for arch in arches:
        denom_elfs = {elf_key(row) for row in runtime_elfs if arch == "ALL" or row["arch"] == arch}
        denom_pkgs = {pkg_key(row) for row in all_packages if arch == "ALL" or row["arch"] == arch}
        for category, values in category_to_elfs.items():
            selected = {key for key in values if arch == "ALL" or key[2] == arch}
            selected_pkgs = {key[:-1] for key in selected}
            rows.append({
                "arch": arch,
                "category": category,
                "elf_numerator": len(selected),
                "elf_denominator_runtime_elf": len(denom_elfs),
                "elf_ratio": f"{len(selected) / len(denom_elfs):.9f}" if denom_elfs else "NOT_AVAILABLE",
                "package_numerator_contains_category": len(selected_pkgs),
                "package_denominator_binary_records": len(denom_pkgs),
                "package_ratio": f"{len(selected_pkgs) / len(denom_pkgs):.9f}" if denom_pkgs else "NOT_AVAILABLE",
                "package_counts_overlap_between_categories": "YES",
            })
    write_tsv(output, [
        "arch", "category", "elf_numerator", "elf_denominator_runtime_elf", "elf_ratio",
        "package_numerator_contains_category", "package_denominator_binary_records", "package_ratio",
        "package_counts_overlap_between_categories",
    ], rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--package-records", required=True)
    ap.add_argument("--extraction", required=True)
    ap.add_argument("--elfs", required=True)
    ap.add_argument("--unwind", required=True)
    ap.add_argument("--needed", required=True)
    ap.add_argument("--failures", required=True)
    ap.add_argument("--output-root", required=True)
    args = ap.parse_args()
    output = pathlib.Path(args.output_root)
    output.mkdir(parents=True, exist_ok=True)
    packages = read_tsv(args.package_records)
    extraction = read_tsv(args.extraction)
    elfs = read_tsv(args.elfs)
    unwind = read_tsv(args.unwind)
    needed = read_tsv(args.needed)
    failures = read_tsv(args.failures)
    runtime = [row for row in elfs if row["runtime_elf"] == "YES"]

    counting = [
        {"unit": "source_package_unique_sourcerpm", "count": len({r["sourcerpm"] for r in packages}), "scope": "all frozen binary records"},
        {"unit": "binary_package_repo_record", "count": len(packages), "scope": "all frozen Base+Unified binary records"},
        {"unit": "binary_package_extract_pass", "count": sum(r["result"] == "PASS" for r in extraction), "scope": "downloaded binary records"},
        {"unit": "ELF_all_types", "count": len(elfs), "scope": "regular files with ELF magic and successful readelf"},
        {"unit": "ELF_runtime_ET_EXEC_or_ET_DYN", "count": len(runtime), "scope": "main distribution denominator"},
        {"unit": "ELF_ET_REL_or_other", "count": len(elfs) - len(runtime), "scope": "excluded from runtime graph denominator"},
        {"unit": "ELF_analysis_failure", "count": len(failures), "scope": "ELF magic found but readelf failed"},
    ]
    write_tsv(output / "measured_denominators.tsv", ["unit", "count", "scope"], counting)

    categories = collections.defaultdict(set)
    for row in runtime:
        key = elf_key(row)
        unwind_table = row["eh_frame"] == "YES" or row["arm_exidx"] == "YES" or row["arm_extab"] == "YES"
        if row["cpp_indicator"] == "YES":
            categories["CPP_CONTENT_OPERATIONAL"].add(key)
        elif unwind_table:
            categories["PURE_C_OPERATIONAL_WITH_UNWIND_TABLE"].add(key)
        else:
            categories["PURE_C_OPERATIONAL_WITHOUT_UNWIND_TABLE"].add(key)
    count_rows_by_arch(categories, packages, runtime, output / "c_cpp_unwind_shape.tsv")

    runtime_keys = {elf_key(row) for row in runtime}
    version_categories = collections.defaultdict(set)
    symbol_summary = collections.defaultdict(lambda: {"references": 0, "elfs": set(), "packages": set()})
    per_elf_classes = collections.defaultdict(set)
    for row in unwind:
        key = elf_key(row)
        if key not in runtime_keys:
            continue
        category = row["version_class"]
        version_categories[category].add(key)
        per_elf_classes[key].add(category)
        s_key = (row["arch"], row["symbol"], row["version_node"] or "NO_VERSION", category)
        symbol_summary[s_key]["references"] += 1
        symbol_summary[s_key]["elfs"].add(key)
        symbol_summary[s_key]["packages"].add(pkg_key(row))
    version_categories["ANY_UNWIND_UND"] = set(per_elf_classes)
    version_categories["VERSIONED_ONLY"] = {k for k, v in per_elf_classes.items() if v == {"VERSIONED"}}
    version_categories["UNVERSIONED_ONLY"] = {k for k, v in per_elf_classes.items() if v == {"UNVERSIONED"}}
    version_categories["MIXED_VERSIONED_AND_UNVERSIONED"] = {k for k, v in per_elf_classes.items() if len(v) > 1}
    count_rows_by_arch(version_categories, packages, runtime, output / "unwind_reference_version_distribution.tsv")
    symbol_rows = []
    for (arch, symbol, node, version_class), values in sorted(symbol_summary.items()):
        symbol_rows.append({
            "arch": arch, "symbol": symbol, "version_node": node,
            "version_class": version_class, "reference_records": values["references"],
            "distinct_elf_count": len(values["elfs"]), "distinct_binary_package_count": len(values["packages"]),
            "elf_denominator_runtime_elf_all_arch": len(runtime),
            "binary_package_denominator_all_arch": len(packages),
        })
    write_tsv(output / "unwind_symbol_distribution.tsv", [
        "arch", "symbol", "version_node", "version_class", "reference_records",
        "distinct_elf_count", "distinct_binary_package_count",
        "elf_denominator_runtime_elf_all_arch", "binary_package_denominator_all_arch",
    ], symbol_rows)

    abi_unwind_rows = [
        row for row in unwind
        if elf_key(row) in runtime_keys
        and (row["symbol"].startswith("_Unwind_") or row["symbol"].startswith("__gnu_Unwind_"))
    ]
    abi_categories = collections.defaultdict(set)
    abi_per_elf = collections.defaultdict(set)
    for row in abi_unwind_rows:
        item_key = elf_key(row)
        abi_categories[row["version_class"]].add(item_key)
        abi_per_elf[item_key].add(row["version_class"])
    abi_categories["ANY_ABI_UNWIND_UND"] = set(abi_per_elf)
    abi_categories["VERSIONED_ONLY"] = {k for k, v in abi_per_elf.items() if v == {"VERSIONED"}}
    abi_categories["UNVERSIONED_ONLY"] = {k for k, v in abi_per_elf.items() if v == {"UNVERSIONED"}}
    abi_categories["MIXED_VERSIONED_AND_UNVERSIONED"] = {k for k, v in abi_per_elf.items() if len(v) > 1}
    count_rows_by_arch(
        abi_categories, packages, runtime,
        output / "abi_unwind_reference_version_distribution.tsv",
    )
    write_tsv(output / "abi_unwind_undefined_symbols.tsv", [
        "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256",
        "path", "symbol_raw", "symbol", "version_node", "version_class",
    ], abi_unwind_rows)

    needed_by_elf = collections.defaultdict(set)
    needed_rows_by_key = {}
    for row in needed:
        key = elf_key(row)
        if key not in runtime_keys:
            continue
        needed_by_elf[key].add(row["soname"])
        needed_rows_by_key[key] = row
    fanin_rows = []
    for label, predicate in [
        ("LIBGCC_S_DIRECT", lambda s: s == "libgcc_s.so.1"),
        ("NONGNU_LIBUNWIND_SONAME8_DIRECT", lambda s: s == "libunwind.so.8"),
        ("LLVM_LIBUNWIND_SONAME1_DIRECT", lambda s: s == "libunwind.so.1"),
        ("ANY_LIBUNWIND_DIRECT", lambda s: s.startswith("libunwind") and ".so" in s),
    ]:
        keys = {key for key, sonames in needed_by_elf.items() if any(predicate(item) for item in sonames)}
        for arch in ["ALL"] + sorted({row["arch"] for row in runtime}):
            selected = {key for key in keys if arch == "ALL" or key[2] == arch}
            elf_denom = {elf_key(row) for row in runtime if arch == "ALL" or row["arch"] == arch}
            pkg_denom = {pkg_key(row) for row in packages if arch == "ALL" or row["arch"] == arch}
            fanin_rows.append({
                "arch": arch, "edge_class": label, "direct_elf_count": len(selected),
                "runtime_elf_denominator": len(elf_denom),
                "direct_binary_package_count": len({key[:-1] for key in selected}),
                "binary_package_denominator": len(pkg_denom),
            })
    write_tsv(output / "unwinder_dt_needed_fanin.tsv", [
        "arch", "edge_class", "direct_elf_count", "runtime_elf_denominator",
        "direct_binary_package_count", "binary_package_denominator",
    ], fanin_rows)

    dlopen = {elf_key(row) for row in runtime if row["dlopen_und"] == "YES"}
    plugin_path = {
        elf_key(row) for row in runtime
        if any(token in row["path"].lower() for token in ("/plugin", "/extension", "/engine"))
    }
    plugin_cpp_unwind = {
        elf_key(row) for row in runtime
        if elf_key(row) in plugin_path and row["cpp_indicator"] == "YES"
        and (row["eh_frame"] == "YES" or row["arm_exidx"] == "YES" or row["arm_extab"] == "YES")
    }
    operational = {
        "DLOPEN_UNDEFINED_REFERENCE": dlopen,
        "PLUGIN_PATH_HEURISTIC": plugin_path,
        "PLUGIN_PATH_CPP_AND_UNWIND": plugin_cpp_unwind,
    }
    count_rows_by_arch(operational, packages, runtime, output / "dlopen_and_plugin_operational.tsv")

    plugin_rows = []
    plugin_packages = collections.defaultdict(lambda: {
        "candidate_elfs": set(), "cpp_unwind_elfs": set(), "paths": set(), "mechanisms": set(), "row": None,
    })
    for row in runtime:
        lower_path = row["path"].lower()
        mechanisms = []
        if row["name"].lower().startswith("launchpad"):
            mechanisms.append("HOST_PACKAGE_NAME_LAUNCHPAD")
        if "/plugin" in lower_path: mechanisms.append("PLUGIN_PATH_TOKEN")
        if "/extension" in lower_path: mechanisms.append("EXTENSION_PATH_TOKEN")
        if "/engine" in lower_path: mechanisms.append("ENGINE_PATH_TOKEN")
        if not mechanisms: continue
        unwind_table = row["eh_frame"] == "YES" or row["arm_exidx"] == "YES" or row["arm_extab"] == "YES"
        cpp_unwind = row["cpp_indicator"] == "YES" and unwind_table
        plugin_rows.append({
            **row,
            "mechanism": ";".join(mechanisms),
            "cpp_and_unwind_table": "YES" if cpp_unwind else "NO",
        })
        pkey = pkg_key(row)
        values = plugin_packages[pkey]
        values["row"] = row
        values["candidate_elfs"].add(elf_key(row))
        if cpp_unwind: values["cpp_unwind_elfs"].add(elf_key(row))
        values["paths"].add(row["path"])
        values["mechanisms"].update(mechanisms)
    plugin_fields = [
        "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256",
        "path", "elf_type", "machine", "runtime_elf", "debug_payload", "itanium_mangled_symbol",
        "cpp_runtime_needed", "cpp_indicator", "eh_frame", "arm_exidx", "arm_extab", "dlopen_und",
        "build_id", "needed_count", "mechanism", "cpp_and_unwind_table",
    ]
    write_tsv(output / "plugin_candidate_elfs.tsv", plugin_fields, plugin_rows)
    plugin_package_rows = []
    for values in plugin_packages.values():
        row = values["row"]
        plugin_package_rows.append({
            "repo_id": row["repo_id"], "name": row["name"], "arch": row["arch"],
            "epoch": row["epoch"], "version": row["version"], "release": row["release"],
            "sourcerpm": row["sourcerpm"], "rpm_sha256": row["rpm_sha256"],
            "mechanisms": ";".join(sorted(values["mechanisms"])),
            "candidate_runtime_elf_count": len(values["candidate_elfs"]),
            "cpp_and_unwind_runtime_elf_count": len(values["cpp_unwind_elfs"]),
            "has_cpp_and_unwind_candidate": "YES" if values["cpp_unwind_elfs"] else "NO",
            "evidence_paths": ";".join(sorted(values["paths"])),
        })
    write_tsv(output / "plugin_mechanism_packages.tsv", [
        "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256",
        "mechanisms", "candidate_runtime_elf_count", "cpp_and_unwind_runtime_elf_count",
        "has_cpp_and_unwind_candidate", "evidence_paths",
    ], sorted(plugin_package_rows, key=lambda row: (row["arch"], row["name"], row["version"], row["release"])))

    arm_runtime = [row for row in runtime if row["arch"] == "armv7l"]
    arm_categories = collections.defaultdict(set)
    for row in arm_runtime:
        key = elf_key(row)
        if row["arm_exidx"] == "YES": arm_categories["ARM_EXIDX_PRESENT"].add(key)
        if row["arm_extab"] == "YES": arm_categories["ARM_EXTAB_PRESENT"].add(key)
        if row["eh_frame"] == "YES": arm_categories["EH_FRAME_PRESENT"].add(key)
        if row["cpp_indicator"] == "YES": arm_categories["CPP_CONTENT_OPERATIONAL"].add(key)
    arm_symbols = {"_Unwind_VRS_Get", "_Unwind_VRS_Set", "_Unwind_VRS_Pop"}
    arm_symbol_keys = {
        elf_key(row) for row in unwind
        if row["arch"] == "armv7l" and row["symbol"] in arm_symbols and elf_key(row) in runtime_keys
    }
    arm_categories["ARM_EHABI_VRS_UND"] = arm_symbol_keys
    count_rows_by_arch(arm_categories, [p for p in packages if p["arch"] == "armv7l"], arm_runtime, output / "armv7l_special_distribution.tsv")

    failure_rows = []
    for row in extraction:
        if row["result"] != "PASS":
            failure_rows.append({
                **{field: row[field] for field in PKG_FIELDS},
                "failure_stage": "RPM_EXTRACTION", "result": row["result"], "detail": row["exit_code"],
            })
    for row in failures:
        failure_rows.append({
            **{field: row[field] for field in PKG_FIELDS},
            "failure_stage": row["operation"], "result": "READ_ELF_FAILED",
            "detail": f"{row['path']} exit={row['exit_code']} stderr={row['stderr']}",
        })
    write_tsv(output / "binary_analysis_failures.tsv", list(PKG_FIELDS) + [
        "failure_stage", "result", "detail"
    ], failure_rows)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
