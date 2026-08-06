#!/usr/bin/env python3
import argparse
import collections
import csv
import pathlib


def read(path):
    with open(path, encoding="utf-8") as stream: return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--packages", required=True)
    ap.add_argument("--files", required=True)
    ap.add_argument("--sonames", required=True)
    ap.add_argument("--exports", required=True)
    ap.add_argument("--dependencies", required=True)
    ap.add_argument("--output-root", required=True)
    args = ap.parse_args()
    packages, files, sonames, exports, deps = map(read, [args.packages, args.files, args.sonames, args.exports, args.dependencies])
    root = pathlib.Path(args.output_root)
    export_groups = collections.defaultdict(list)
    for row in exports:
        key = tuple(row[field] for field in ["set_id", "role", "arch", "name", "path", "soname"])
        export_groups[key].append(row)
    export_summary = []
    for key, rows in sorted(export_groups.items()):
        set_id, role, arch, name, path, soname = key
        unwind = [row for row in rows if "Unwind" in row["symbol"]]
        export_summary.append({
            "set_id": set_id, "role": role, "arch": arch, "name": name, "path": path, "soname": soname,
            "exported_global_or_weak_symbol_count": len({row["symbol_raw"] for row in rows}),
            "exported_unwind_symbol_count": len({row["symbol_raw"] for row in unwind}),
            "all_export_version_nodes": ";".join(sorted({row["version_node"] for row in rows})),
            "unwind_export_version_nodes": ";".join(sorted({row["version_node"] for row in unwind})) or "NONE",
        })
    write(root / "special_unwinder_export_summary.tsv", [
        "set_id", "role", "arch", "name", "path", "soname",
        "exported_global_or_weak_symbol_count", "exported_unwind_symbol_count",
        "all_export_version_nodes", "unwind_export_version_nodes",
    ], export_summary)

    soname_by = collections.defaultdict(set)
    for row in sonames: soname_by[(row["set_id"], row["role"], row["arch"])].add(row["soname"])
    soname_rows = []
    for arch in ("armv7l", "aarch64", "x86_64"):
        soname_rows.append({
            "arch": arch,
            "platform_nongnu_runtime_sonames": ";".join(sorted(soname_by[("platform_nongnu", "nongnu_runtime", arch)])),
            "platform_repo_llvm_runtime_sonames": ";".join(sorted(soname_by[("platform_repo_llvm", "llvm_runtime", arch)])),
            "project_c2_libunwind_soname": "NOT_PRESENT_PROJECT_USES_LIBGCC_S",
            "same_runtime_soname_between_nongnu_and_platform_repo_llvm": "YES" if soname_by[("platform_nongnu", "nongnu_runtime", arch)] & soname_by[("platform_repo_llvm", "llvm_runtime", arch)] else "NO",
        })
    write(root / "unwinder_soname_comparison.tsv", [
        "arch", "platform_nongnu_runtime_sonames", "platform_repo_llvm_runtime_sonames",
        "project_c2_libunwind_soname", "same_runtime_soname_between_nongnu_and_platform_repo_llvm",
    ], soname_rows)

    by_file = {(row["set_id"], row["role"], row["arch"], row["path"]): row for row in files}
    reference_rows = []
    base_paths = [
        "/usr/include/unwind.h", "/usr/include/unwind_arm_ehabi.h", "/usr/include/unwind_itanium.h",
        "/usr/include/libunwind.h", "/usr/include/libunwind.modulemap", "/usr/include/__libunwind_config.h",
        "/usr/include/mach-o/compact_unwind_encoding.h",
    ]
    sets = [
        ("platform_nongnu", "nongnu_devel", "PLATFORM_NONGNU"),
        ("platform_repo_llvm", "llvm_devel", "PLATFORM_REPO_LLVM"),
        ("project_c2", "project_llvm_devel", "PROJECT_C2_CURRENT_LIBGCC_SCHEME"),
    ]
    for arch in ("armv7l", "aarch64", "x86_64"):
        libdir = "/usr/lib" if arch == "armv7l" else "/usr/lib64"
        for path in base_paths + [libdir + "/libunwind.so"]:
            for set_id, role, label in sets:
                row = by_file.get((set_id, role, arch, path))
                reference_rows.append({
                    "arch": arch, "package_set": label, "path": path,
                    "presence": "PRESENT" if row else "NOT_PRESENT",
                    "package_name": row["name"] if row else "NOT_PRESENT",
                    "file_type": row["file_type"] if row else "NOT_PRESENT",
                    "file_sha256": row["file_sha256"] if row else "NOT_PRESENT",
                    "link_target": row["link_target"] if row else "NOT_PRESENT",
                })
    write(root / "llvm_libunwind_reference_path_presence.tsv", [
        "arch", "package_set", "path", "presence", "package_name", "file_type", "file_sha256", "link_target",
    ], reference_rows)

    dep_summary = []
    for scope in sorted({row["dependency_scope"] for row in deps}):
        for classification in sorted({row["classification"] for row in deps if row["dependency_scope"] == scope}):
            selected = [row for row in deps if row["dependency_scope"] == scope and row["classification"] == classification]
            dep_summary.append({
                "dependency_scope": scope, "classification": classification,
                "relation_record_count": len(selected),
                "distinct_package_record_count": len({row["package_nevra"] for row in selected}),
                "distinct_package_name_count": len({row["package_name"] for row in selected}),
                "package_names": ";".join(sorted({row["package_name"] for row in selected})),
                "denominator": "11332_binary_records" if scope == "binary_Requires" else "1450_source_records",
            })
    write(root / "libunwind_dependency_metadata_summary.tsv", [
        "dependency_scope", "classification", "relation_record_count", "distinct_package_record_count",
        "distinct_package_name_count", "package_names", "denominator",
    ], dep_summary)
    return 0


if __name__ == "__main__": raise SystemExit(main())
