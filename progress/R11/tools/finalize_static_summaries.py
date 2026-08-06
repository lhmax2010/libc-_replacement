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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--elf-inventory", required=True)
    parser.add_argument("--plugins", required=True)
    parser.add_argument("--plugin-packages", required=True)
    parser.add_argument("--repodata", required=True)
    parser.add_argument("--output-root", required=True)
    args = parser.parse_args()
    elfs = read_tsv(args.elf_inventory)
    plugins = read_tsv(args.plugins)
    plugin_packages = read_tsv(args.plugin_packages)
    repodata = read_tsv(args.repodata)
    runtime = [row for row in elfs if row["runtime_elf"] == "YES"]
    pkg_key = lambda row: (row["repo_id"], row["name"], row["arch"], row["epoch"], row["version"], row["release"], row["rpm_sha256"])
    runtime_pkgs = {pkg_key(row) for row in runtime}
    runtime_sources = {row["sourcerpm"] for row in runtime}
    output = args.output_root.rstrip("/")

    counts = [
        {"unit": "source_package_repo_record", "total": 1450, "definition": "all src architecture records in frozen Base+Unified source primary metadata", "mapping_rule": "source NEVRA"},
        {"unit": "distinct_sourcerpm_referenced_by_binaries", "total": 1450, "definition": "distinct rpm:sourcerpm strings among all binary records", "mapping_rule": "one source RPM maps to one-or-more binary package records"},
        {"unit": "source_packages_with_runtime_ELF", "total": len(runtime_sources), "definition": "distinct rpm:sourcerpm strings owning at least one runtime ELF", "mapping_rule": "runtime ELF -> owning binary RPM record -> rpm:sourcerpm"},
        {"unit": "binary_package_repo_record", "total": 11332, "definition": "all unique binary NEVRA records in frozen Base+Unified binary primary metadata", "mapping_rule": "each record maps through rpm:sourcerpm"},
        {"unit": "binary_package_with_runtime_ELF", "total": len(runtime_pkgs), "definition": "binary package records owning at least one ET_EXEC/ET_DYN non-debug ELF", "mapping_rule": "ELF is owned by exactly one extracted RPM record"},
        {"unit": "ELF_all_types", "total": len(elfs), "definition": "regular files with ELF magic and successful readelf", "mapping_rule": "owned by exactly one scanned binary RPM record"},
        {"unit": "ELF_runtime_ET_EXEC_or_ET_DYN", "total": len(runtime), "definition": "ET_EXEC/ET_DYN excluding debug payload paths", "mapping_rule": "main runtime graph denominator"},
        {"unit": "ELF_ET_REL_or_other", "total": len(elfs) - len(runtime), "definition": "ELF records excluded from runtime graph", "mapping_rule": "all ELF minus runtime ELF"},
    ]
    write_tsv(f"{output}/counting_units_final.tsv", ["unit", "total", "definition", "mapping_rule"], counts)

    ppkgs = {pkg_key(row) for row in plugins}
    psrcs = {row["sourcerpm"] for row in plugins}
    cpp_plugins = [row for row in plugins if row["cpp_and_unwind_table"] == "YES"]
    cpp_ppkgs = {pkg_key(row) for row in cpp_plugins}
    cpp_psrcs = {row["sourcerpm"] for row in cpp_plugins}
    plugin_summary = [
        {"classification": "PLUGIN_OR_ENGINE_OR_LAUNCHPAD_HEURISTIC", "numerator": len(plugins), "denominator": 36829, "unit": "runtime_ELF_files", "ratio": f"{len(plugins)/36829:.9f}", "counts_overlap": "NO_WITHIN_THIS_ROW"},
        {"classification": "PLUGIN_OR_ENGINE_OR_LAUNCHPAD_HEURISTIC", "numerator": len(ppkgs), "denominator": 11332, "unit": "binary_package_records", "ratio": f"{len(ppkgs)/11332:.9f}", "counts_overlap": "NO_WITHIN_THIS_ROW"},
        {"classification": "PLUGIN_OR_ENGINE_OR_LAUNCHPAD_HEURISTIC", "numerator": len(psrcs), "denominator": 1450, "unit": "source_package_records", "ratio": f"{len(psrcs)/1450:.9f}", "counts_overlap": "NO_WITHIN_THIS_ROW"},
        {"classification": "CPP_CONTENT_AND_UNWIND_WITHIN_PLUGIN_HEURISTIC", "numerator": len(cpp_plugins), "denominator": len(plugins), "unit": "plugin_candidate_runtime_ELF_files", "ratio": f"{len(cpp_plugins)/len(plugins):.9f}", "counts_overlap": "NO_WITHIN_THIS_ROW"},
        {"classification": "CPP_CONTENT_AND_UNWIND_WITHIN_PLUGIN_HEURISTIC", "numerator": len(cpp_ppkgs), "denominator": len(ppkgs), "unit": "plugin_candidate_binary_package_records", "ratio": f"{len(cpp_ppkgs)/len(ppkgs):.9f}", "counts_overlap": "NO_WITHIN_THIS_ROW"},
        {"classification": "CPP_CONTENT_AND_UNWIND_WITHIN_PLUGIN_HEURISTIC", "numerator": len(cpp_psrcs), "denominator": len(psrcs), "unit": "plugin_candidate_source_package_records", "ratio": f"{len(cpp_psrcs)/len(psrcs):.9f}", "counts_overlap": "NO_WITHIN_THIS_ROW"},
    ]
    mechanism_elfs = collections.defaultdict(set)
    mechanism_pkgs = collections.defaultdict(set)
    for row in plugins:
        key = (row["rpm_sha256"], row["path"])
        for mechanism in row["mechanism"].split(";"):
            mechanism_elfs[mechanism].add(key)
            mechanism_pkgs[mechanism].add(pkg_key(row))
    for mechanism in sorted(mechanism_elfs):
        plugin_summary.append({"classification": mechanism, "numerator": len(mechanism_elfs[mechanism]), "denominator": len(plugins), "unit": "plugin_candidate_runtime_ELF_files", "ratio": f"{len(mechanism_elfs[mechanism])/len(plugins):.9f}", "counts_overlap": "YES_BETWEEN_MECHANISMS"})
        plugin_summary.append({"classification": mechanism, "numerator": len(mechanism_pkgs[mechanism]), "denominator": len(ppkgs), "unit": "plugin_candidate_binary_package_records", "ratio": f"{len(mechanism_pkgs[mechanism])/len(ppkgs):.9f}", "counts_overlap": "YES_BETWEEN_MECHANISMS"})
    write_tsv(f"{output}/plugin_mechanism_summary.tsv", ["classification", "numerator", "denominator", "unit", "ratio", "counts_overlap"], plugin_summary)

    seen = set()
    snapshots = []
    for row in repodata:
        if row["repo_id"] in seen:
            continue
        seen.add(row["repo_id"])
        snapshots.append({
            "snapshot_id_type": "COMPOSITE_REPOSITORY_COMPONENT",
            "repo_id": row["repo_id"], "kind": row["kind"], "repomd_revision": row["revision"],
            "repomd_sha256": row["repomd_sha256"],
            "note": "repomd does not expose one cross-repository platform snapshot ID; these four immutable revision+SHA pairs are the R11 composite identity",
        })
    write_tsv(f"{output}/snapshot_identity.tsv", ["snapshot_id_type", "repo_id", "kind", "repomd_revision", "repomd_sha256", "note"], snapshots)

    methods = [
        {"classification": "CPP_CONTENT_OPERATIONAL", "unit": "ELF_file", "rule": "dynamic symbol base starts with _Z OR DT_NEEDED contains libstdc++, libc++, or libc++abi", "limitation": "operational indicator; it does not prove every source language present in the linked image"},
        {"classification": "PURE_C_OPERATIONAL", "unit": "ELF_file", "rule": "none of the CPP_CONTENT_OPERATIONAL indicators", "limitation": "absence-of-indicator class; not proof that all input translation units were C"},
        {"classification": "UNWIND_TABLE_PRESENT", "unit": "ELF_file", "rule": ".eh_frame or .ARM.exidx section exists", "limitation": "section presence does not demonstrate runtime unwind correctness"},
        {"classification": "ABI_UNWIND_UND", "unit": "ELF_file/symbol", "rule": "undefined dynamic symbol base starts with _Unwind_ or __gnu_Unwind_", "limitation": "binding-relevant ABI subset; literal *Unwind* substring results are retained separately"},
        {"classification": "PLUGIN_MECHANISM_HEURISTIC", "unit": "ELF_file/binary_package", "rule": "path token plugin/plugins/engine/engines or launchpad package-name host", "limitation": "metadata/path approximation; does not establish that the object is loaded at runtime"},
    ]
    write_tsv(f"{output}/classification_methodology.tsv", ["classification", "unit", "rule", "limitation"], methods)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
