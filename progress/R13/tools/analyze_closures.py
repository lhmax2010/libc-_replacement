#!/usr/bin/env python3
"""R13: derive DSO closures exclusively from already-landed R11/R12 TSVs."""

from __future__ import annotations

import argparse
import csv
import hashlib
import os
from collections import Counter, defaultdict, deque
from pathlib import Path


LIBGCC = "libgcc_s.so.1"


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path: Path, fields: list[str], rows) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(
            stream, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore"
        )
        writer.writeheader()
        writer.writerows(rows)


def sha256(path: Path) -> str:
    value = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            value.update(chunk)
    return value.hexdigest()


def pct(numerator: int, denominator: int) -> str:
    return f"{100.0 * numerator / denominator:.6f}" if denominator else "NOT_APPLICABLE"


def package_id(row: dict[str, str]) -> str:
    epoch = "" if row.get("epoch", "0") in {"", "0"} else f"{row['epoch']}:"
    return f"{row['name']}-{epoch}{row['version']}-{row['release']}.{row['arch']}"


def resolution_arch(elf: dict[str, str]) -> str:
    if elf["arch"] != "noarch":
        return elf["arch"]
    machine = elf["machine"]
    mapping = {
        "AArch64": "aarch64",
        "ARM": "armv7l",
        "Advanced Micro Devices X86-64": "x86_64",
        "Intel 80386": "i586",
        "RISC-V": "riscv64",
    }
    return mapping.get(machine, "UNDETERMINED_NOARCH_MACHINE")


def unversioned_soname_provide(name: str) -> str | None:
    value = name
    if value.endswith("(64bit)"):
        value = value[:-7]
    if value.endswith("()"):
        value = value[:-2]
    if "(" in value or ")" in value or ".so" not in value:
        return None
    return value


def unwind_table(elf: dict[str, str]) -> bool:
    return elf["eh_frame"] == "YES" or elf["arm_exidx"] == "YES"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--workspace", required=True)
    parser.add_argument("--output", required=True)
    args = parser.parse_args()
    workspace = Path(args.workspace).resolve()
    output = Path(args.output).resolve()
    tables = output / "tables"
    raw = output / "raw"
    inputs_dir = output / "inputs"

    inputs = {
        "R11_SNAPSHOT": workspace / "progress/R11/tables/snapshot_identity.tsv",
        "R11_BINARY_PACKAGES": workspace / "progress/R11/tables/binary_package_records.tsv",
        "R11_SOURCE_PACKAGES": workspace / "progress/R11/tables/source_package_records.tsv",
        "R11_ELF_INVENTORY": workspace / "progress/R11/tables/elf_inventory.tsv",
        "R11_DT_NEEDED": workspace / "progress/R11/tables/dt_needed_edges.tsv",
        "R11_BINARY_RELATIONS": workspace / "progress/R11/tables/binary_relations.tsv",
        "R11_ABI_UNWIND_UND": workspace / "progress/R11/tables/abi_unwind_undefined_symbols.tsv",
        "R11_PLUGIN_PACKAGES": workspace / "progress/R11/tables/plugin_mechanism_packages.tsv",
        "R11_PROJECT_C2_ELF": workspace / "progress/R11/tables/project_c2_elf_inventory.tsv",
        "R11_PROJECT_C2_PACKAGES": workspace / "progress/R11/tables/project_c2_packages.tsv",
        "R11_PROJECT_C2_UNWIND": workspace / "progress/R11/tables/project_c2_unwind_undefined_symbols.tsv",
        "R11_LIBUNWIND_DEPS": workspace / "progress/R11/tables/libunwind_dependency_metadata.tsv",
        "R12_PROJECT_C2_NEEDED_SUPPLEMENT": workspace / "progress/R12/tables/dt_needed.tsv",
        "T1_SOURCE_SCOPE": workspace / "progress/T1/source_packages.tsv",
        "T1_COVERAGE": workspace / "progress/T1/coverage_and_occurrence.tsv",
        "VENDOR_ARMV7L_ANCHORS": workspace / "census/frozen_armv7l/anchors.tsv",
    }
    missing = [str(path) for path in inputs.values() if not path.is_file()]
    if missing:
        raise RuntimeError("missing frozen inputs: " + ";".join(missing))
    write_tsv(
        inputs_dir / "input_identity.tsv",
        ["role", "path", "size_bytes", "sha256", "usage"],
        [
            {
                "role": role,
                "path": str(path.relative_to(workspace)),
                "size_bytes": str(path.stat().st_size),
                "sha256": sha256(path),
                "usage": (
                    "SUPPLEMENT_ALREADY_LANDED_NO_RESCAN"
                    if role.startswith("R12_")
                    else "AUXILIARY_SCOPE_IDENTITY"
                    if role.startswith("T1_") or role.startswith("VENDOR_")
                    else "R11_PRIMARY_INPUT"
                ),
            }
            for role, path in inputs.items()
        ],
    )

    packages = read_tsv(inputs["R11_BINARY_PACKAGES"])
    source_packages = read_tsv(inputs["R11_SOURCE_PACKAGES"])
    all_elfs = read_tsv(inputs["R11_ELF_INVENTORY"])
    elfs = [row for row in all_elfs if row["runtime_elf"] == "YES"]
    needed_rows = read_tsv(inputs["R11_DT_NEEDED"])
    relations = read_tsv(inputs["R11_BINARY_RELATIONS"])

    package_by_sha = {row["checksum"]: row for row in packages}
    if len(package_by_sha) != len(packages):
        raise RuntimeError("binary package SHA256 is not unique")
    package_lookup: dict[tuple[str, str], dict[str, str]] = {}
    duplicate_lookup = []
    for row in packages:
        relation_nevra = f"{row['name']}-{row['version']}-{row['release']}.{row['arch']}"
        key = (row["repo_id"], relation_nevra)
        if key in package_lookup:
            duplicate_lookup.append(key)
        package_lookup[key] = row
    if duplicate_lookup:
        raise RuntimeError(f"repo/package_nevra key not unique: {duplicate_lookup[:5]}")

    node_by_key: dict[tuple[str, str], dict[str, str]] = {}
    nodes_by_package: dict[str, list[tuple[str, str]]] = defaultdict(list)
    for row in elfs:
        key = (row["rpm_sha256"], row["path"])
        if key in node_by_key:
            raise RuntimeError(f"duplicate runtime ELF key: {key}")
        node_by_key[key] = row
        nodes_by_package[row["rpm_sha256"]].append(key)
        if row["rpm_sha256"] not in package_by_sha:
            raise RuntimeError(f"runtime ELF package not found: {key}")

    needed_by_node: dict[tuple[str, str], set[str]] = defaultdict(set)
    for row in needed_rows:
        key = (row["rpm_sha256"], row["path"])
        if key not in node_by_key:
            raise RuntimeError(f"DT_NEEDED consumer is not a runtime ELF: {key}")
        needed_by_node[key].add(row["soname"])
    if sum(len(values) for values in needed_by_node.values()) != len(needed_rows):
        raise RuntimeError("duplicate DT_NEEDED rows detected")

    provider_packages: dict[tuple[str, str], set[str]] = defaultdict(set)
    provider_relation_rows = 0
    provider_package_lookup_failures = []
    for row in relations:
        if row["relation_type"] != "provides":
            continue
        soname = unversioned_soname_provide(row["relation_name"])
        if soname is None:
            continue
        provider_relation_rows += 1
        package = package_lookup.get((row["repo_id"], row["package_nevra"]))
        if package is None:
            provider_package_lookup_failures.append(row)
            continue
        provider_packages[(row["package_arch"], soname)].add(package["checksum"])
    if provider_package_lookup_failures:
        raise RuntimeError(
            f"provider relation package lookup failures: {len(provider_package_lookup_failures)}"
        )

    provider_cache: dict[tuple[str, str], tuple[str, list[tuple[str, str]], list[str], str]] = {}

    def resolve(arch: str, soname: str):
        cache_key = (arch, soname)
        if cache_key in provider_cache:
            return provider_cache[cache_key]
        shas = sorted(provider_packages.get(cache_key, set()))
        candidates: set[tuple[str, str]] = set()
        details = []
        for rpm_sha in shas:
            exact = [key for key in nodes_by_package.get(rpm_sha, []) if os.path.basename(key[1]) == soname]
            prefixed = [
                key
                for key in nodes_by_package.get(rpm_sha, [])
                if os.path.basename(key[1]).startswith(soname + ".")
            ]
            matched = exact if exact else prefixed
            candidates.update(matched)
            details.append(
                f"{package_id(package_by_sha[rpm_sha])}:exact={len(exact)},prefix={len(prefixed)}"
            )
        if len(candidates) == 1:
            status = "RESOLVED_UNIQUE"
            reason = "unique unversioned RPM SONAME Provide and unique exact/prefix runtime ELF basename"
        elif not shas:
            status = "UNRESOLVED_NO_UNVERSIONED_SONAME_PROVIDE"
            reason = "no matching unversioned RPM Provides row for resolution architecture"
        elif not candidates:
            status = "UNRESOLVED_PROVIDER_HAS_NO_MATCHING_RUNTIME_ELF"
            reason = "provider package exists but no exact/prefix runtime ELF basename"
        else:
            status = "UNRESOLVED_AMBIGUOUS_PROVIDER_ELF"
            reason = "more than one candidate provider runtime ELF"
        result = (status, sorted(candidates), shas, reason + ";" + ";".join(details))
        provider_cache[cache_key] = result
        return result

    adjacency: dict[tuple[str, str], set[tuple[str, str]]] = defaultdict(set)
    reverse: dict[tuple[str, str], set[tuple[str, str]]] = defaultdict(set)
    unresolved_nodes: set[tuple[str, str]] = set()
    edge_resolution_rows = []
    for key in sorted(node_by_key):
        elf = node_by_key[key]
        arch = resolution_arch(elf)
        for soname in sorted(needed_by_node.get(key, set())):
            status, candidates, provider_shas, reason = resolve(arch, soname)
            provider_key = candidates[0] if status == "RESOLVED_UNIQUE" else None
            if provider_key:
                adjacency[key].add(provider_key)
                reverse[provider_key].add(key)
            else:
                unresolved_nodes.add(key)
            edge_resolution_rows.append(
                {
                    "consumer_repo_id": elf["repo_id"],
                    "consumer_name": elf["name"],
                    "consumer_arch": elf["arch"],
                    "resolution_arch": arch,
                    "consumer_rpm_sha256": key[0],
                    "consumer_path": key[1],
                    "needed_soname": soname,
                    "resolution_status": status,
                    "provider_package_count": str(len(provider_shas)),
                    "provider_elf_count": str(len(candidates)),
                    "provider_rpm_sha256": provider_key[0] if provider_key else "",
                    "provider_path": provider_key[1] if provider_key else "",
                    "reason_and_candidates": reason,
                }
            )
    edge_fields = [
        "consumer_repo_id", "consumer_name", "consumer_arch", "resolution_arch",
        "consumer_rpm_sha256", "consumer_path", "needed_soname", "resolution_status",
        "provider_package_count", "provider_elf_count", "provider_rpm_sha256", "provider_path",
        "reason_and_candidates",
    ]
    write_tsv(tables / "dependency_edge_resolution.tsv", edge_fields, edge_resolution_rows)
    write_tsv(
        tables / "unresolved_dependency_edges.tsv",
        edge_fields,
        [row for row in edge_resolution_rows if row["resolution_status"] != "RESOLVED_UNIQUE"],
    )

    incomplete_nodes = set(unresolved_nodes)
    queue = deque(unresolved_nodes)
    while queue:
        target = queue.popleft()
        for parent in reverse.get(target, set()):
            if parent not in incomplete_nodes:
                incomplete_nodes.add(parent)
                queue.append(parent)

    direct_nodes = {key for key, values in needed_by_node.items() if LIBGCC in values}
    reaches_libgcc = set(direct_nodes)
    queue = deque(direct_nodes)
    while queue:
        target = queue.popleft()
        for parent in reverse.get(target, set()):
            if parent not in reaches_libgcc:
                reaches_libgcc.add(parent)
                queue.append(parent)

    def elf_class(key):
        if key in direct_nodes:
            return "DIRECT"
        if key in reaches_libgcc:
            return "TRANSITIVE"
        if key in incomplete_nodes:
            return "UNDETERMINED"
        return "ABSENT"

    elf_rows = []
    for key in sorted(node_by_key):
        elf = node_by_key[key]
        classification = elf_class(key)
        elf_rows.append(
            {
                **{field: elf[field] for field in [
                    "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm",
                    "rpm_sha256", "path", "cpp_indicator", "eh_frame", "arm_exidx", "arm_extab",
                ]},
                "resolution_arch": resolution_arch(elf),
                "closure_status": "INCOMPLETE" if key in incomplete_nodes else "COMPLETE",
                "libgcc_s_classification": classification,
                "libgcc_s_presence_query_status": (
                    "UNDETERMINED" if classification == "UNDETERMINED" else "DETERMINED"
                ),
                "own_dt_needed_libgcc_s": "YES" if key in direct_nodes else "NO",
                "reachable_libgcc_s_observed": "YES" if key in reaches_libgcc else "NO",
                "unwind_table": "YES" if unwind_table(elf) else "NO",
            }
        )
    elf_fields = list(elf_rows[0].keys())
    write_tsv(tables / "elf_closure_classification.tsv", elf_fields, elf_rows)

    package_rows = []
    package_class_by_sha = {}
    for rpm_sha, keys in sorted(nodes_by_package.items()):
        package = package_by_sha[rpm_sha]
        classes = [elf_class(key) for key in keys]
        if "DIRECT" in classes:
            classification = "DIRECT"
        elif "TRANSITIVE" in classes:
            classification = "TRANSITIVE"
        elif "UNDETERMINED" in classes:
            classification = "UNDETERMINED"
        else:
            classification = "ABSENT"
        full_closure_complete = not any(key in incomplete_nodes for key in keys)
        row = {
            "repo_id": package["repo_id"],
            "name": package["name"],
            "arch": package["arch"],
            "epoch": package["epoch"],
            "version": package["version"],
            "release": package["release"],
            "sourcerpm": package["sourcerpm"],
            "rpm_sha256": rpm_sha,
            "runtime_elf_count": str(len(keys)),
            "closure_status": "COMPLETE" if full_closure_complete else "INCOMPLETE",
            "libgcc_s_classification": classification,
            "libgcc_s_presence_query_status": (
                "UNDETERMINED" if classification == "UNDETERMINED" else "DETERMINED"
            ),
            "direct_elf_count": str(sum(value == "DIRECT" for value in classes)),
            "transitive_elf_count": str(sum(value == "TRANSITIVE" for value in classes)),
            "absent_elf_count": str(sum(value == "ABSENT" for value in classes)),
            "undetermined_elf_count": str(sum(value == "UNDETERMINED" for value in classes)),
            "incomplete_full_closure_elf_count": str(sum(key in incomplete_nodes for key in keys)),
        }
        package_rows.append(row)
        package_class_by_sha[rpm_sha] = row
    package_fields = list(package_rows[0].keys())
    write_tsv(tables / "package_closure_classification.tsv", package_fields, package_rows)
    write_tsv(
        tables / "clean_package_set.tsv", package_fields,
        [row for row in package_rows if row["libgcc_s_classification"] == "ABSENT"],
    )
    write_tsv(
        tables / "risk_package_set.tsv", package_fields,
        [row for row in package_rows if row["libgcc_s_classification"] in {"DIRECT", "TRANSITIVE"}],
    )
    write_tsv(
        tables / "incomplete_package_closures.tsv", package_fields,
        [row for row in package_rows if row["closure_status"] == "INCOMPLETE"],
    )

    distribution_rows = []
    for unit, rows, arch_field, class_field in [
        ("ELF_file", elf_rows, "arch", "libgcc_s_classification"),
        ("binary_package_with_runtime_ELF", package_rows, "arch", "libgcc_s_classification"),
    ]:
        arches = sorted({row[arch_field] for row in rows})
        for arch in ["ALL"] + arches:
            selected = rows if arch == "ALL" else [row for row in rows if row[arch_field] == arch]
            counts = Counter(row[class_field] for row in selected)
            determined = len(selected) - counts["UNDETERMINED"]
            full_complete = sum(row["closure_status"] == "COMPLETE" for row in selected)
            for classification in ["DIRECT", "TRANSITIVE", "ABSENT", "UNDETERMINED"]:
                numerator = counts[classification]
                distribution_rows.append(
                    {
                        "arch": arch,
                        "unit": unit,
                        "classification": classification,
                        "numerator": str(numerator),
                        "full_population_denominator": str(len(selected)),
                        "determined_query_denominator": str(determined),
                        "full_dependency_complete_count": str(full_complete),
                        "percent_of_full_population": pct(numerator, len(selected)),
                        "percent_of_determined_queries": (
                            pct(numerator, determined) if classification != "UNDETERMINED" else "EXCLUDED"
                        ),
                    }
                )
    write_tsv(
        tables / "closure_distribution.tsv",
        ["arch", "unit", "classification", "numerator", "full_population_denominator",
         "determined_query_denominator", "full_dependency_complete_count", "percent_of_full_population",
         "percent_of_determined_queries"],
        distribution_rows,
    )
    clean_risk_rows = []
    for unit, rows in [("ELF_file", elf_rows), ("binary_package_with_runtime_ELF", package_rows)]:
        for arch in ["ALL"] + sorted({row["arch"] for row in rows}):
            selected = rows if arch == "ALL" else [row for row in rows if row["arch"] == arch]
            risk = sum(row["libgcc_s_classification"] in {"DIRECT", "TRANSITIVE"} for row in selected)
            clean = sum(row["libgcc_s_classification"] == "ABSENT" for row in selected)
            undetermined = len(selected) - risk - clean
            determined = risk + clean
            for assignment, value in [("RISK", risk), ("CLEAN", clean), ("UNDETERMINED", undetermined)]:
                clean_risk_rows.append(
                    {
                        "arch": arch,
                        "unit": unit,
                        "assignment": assignment,
                        "numerator": str(value),
                        "full_population_denominator": str(len(selected)),
                        "determined_query_denominator": str(determined),
                        "percent_of_full_population": pct(value, len(selected)),
                        "percent_of_determined_queries": (
                            pct(value, determined) if assignment != "UNDETERMINED" else "EXCLUDED"
                        ),
                    }
                )
    write_tsv(
        tables / "clean_risk_summary.tsv",
        ["arch", "unit", "assignment", "numerator", "full_population_denominator",
         "determined_query_denominator", "percent_of_full_population", "percent_of_determined_queries"],
        clean_risk_rows,
    )

    edge_status = Counter(row["resolution_status"] for row in edge_resolution_rows)
    coverage_rows = []
    for scope, selected in [("ALL", edge_resolution_rows)] + [
        (arch, [row for row in edge_resolution_rows if row["consumer_arch"] == arch])
        for arch in sorted({row["consumer_arch"] for row in edge_resolution_rows})
    ]:
        resolved = sum(row["resolution_status"] == "RESOLVED_UNIQUE" for row in selected)
        coverage_rows.append(
            {
                "scope": scope,
                "unit": "DT_NEEDED_edge",
                "total": str(len(selected)),
                "complete_or_resolved": str(resolved),
                "incomplete_or_unresolved": str(len(selected) - resolved),
                "coverage_percent": pct(resolved, len(selected)),
                "denominator": "all R11 DT_NEEDED edge rows in scope",
            }
        )
    for unit, rows in [("ELF_file", elf_rows), ("binary_package_with_runtime_ELF", package_rows)]:
        for scope, selected in [("ALL", rows)] + [
            (arch, [row for row in rows if row["arch"] == arch]) for arch in sorted({row["arch"] for row in rows})
        ]:
            complete = sum(row["closure_status"] == "COMPLETE" for row in selected)
            coverage_rows.append(
                {
                    "scope": scope,
                    "unit": unit,
                    "total": str(len(selected)),
                    "complete_or_resolved": str(complete),
                    "incomplete_or_unresolved": str(len(selected) - complete),
                    "coverage_percent": pct(complete, len(selected)),
                    "denominator": f"all R11 {unit} rows in scope",
                }
            )
    write_tsv(
        tables / "closure_coverage.tsv",
        ["scope", "unit", "total", "complete_or_resolved", "incomplete_or_unresolved",
         "coverage_percent", "denominator"],
        coverage_rows,
    )
    write_tsv(
        tables / "edge_resolution_status_summary.tsv",
        ["resolution_status", "edge_count", "denominator", "percent"],
        [
            {
                "resolution_status": status,
                "edge_count": str(count),
                "denominator": str(len(edge_resolution_rows)),
                "percent": pct(count, len(edge_resolution_rows)),
            }
            for status, count in sorted(edge_status.items())
        ],
    )

    risk_package_shas = {
        row["rpm_sha256"] for row in package_rows if row["libgcc_s_classification"] in {"DIRECT", "TRANSITIVE"}
    }
    risk_strata_package_rows = []
    for rpm_sha in sorted(risk_package_shas):
        owned = [node_by_key[key] for key in nodes_by_package[rpm_sha]]
        cpp_unwind = [elf for elf in owned if elf["cpp_indicator"] == "YES" and unwind_table(elf)]
        pure_c_unwind = [elf for elf in owned if elf["cpp_indicator"] != "YES" and unwind_table(elf)]
        no_unwind = [elf for elf in owned if not unwind_table(elf)]
        if cpp_unwind:
            stratum = "A_CPP_CONTENT_WITH_UNWIND_TABLE"
        elif pure_c_unwind:
            stratum = "B_OPERATIONAL_PURE_C_WITH_UNWIND_TABLE"
        else:
            stratum = "C_NO_UNWIND_TABLE"
        package = package_class_by_sha[rpm_sha]
        risk_strata_package_rows.append(
            {
                **{field: package[field] for field in [
                    "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256",
                    "libgcc_s_classification",
                ]},
                "risk_stratum": stratum,
                "cpp_with_unwind_elf_count": str(len(cpp_unwind)),
                "pure_c_with_unwind_elf_count": str(len(pure_c_unwind)),
                "no_unwind_elf_count": str(len(no_unwind)),
            }
        )
    risk_package_fields = list(risk_strata_package_rows[0].keys()) if risk_strata_package_rows else []
    write_tsv(tables / "risk_package_strata.tsv", risk_package_fields, risk_strata_package_rows)

    risk_elf_strata = []
    for row in elf_rows:
        if row["libgcc_s_classification"] not in {"DIRECT", "TRANSITIVE"}:
            continue
        if row["unwind_table"] == "NO":
            stratum = "C_NO_UNWIND_TABLE"
        elif row["cpp_indicator"] == "YES":
            stratum = "A_CPP_CONTENT_WITH_UNWIND_TABLE"
        else:
            stratum = "B_OPERATIONAL_PURE_C_WITH_UNWIND_TABLE"
        risk_elf_strata.append({**row, "risk_stratum": stratum})
    write_tsv(tables / "risk_elf_strata.tsv", list(risk_elf_strata[0].keys()), risk_elf_strata)

    risk_summary_rows = []
    for unit, rows in [("binary_package", risk_strata_package_rows), ("ELF_file", risk_elf_strata)]:
        for arch in ["ALL"] + sorted({row["arch"] for row in rows}):
            selected = rows if arch == "ALL" else [row for row in rows if row["arch"] == arch]
            counts = Counter(row["risk_stratum"] for row in selected)
            for stratum in [
                "A_CPP_CONTENT_WITH_UNWIND_TABLE",
                "B_OPERATIONAL_PURE_C_WITH_UNWIND_TABLE",
                "C_NO_UNWIND_TABLE",
            ]:
                risk_summary_rows.append(
                    {
                        "arch": arch,
                        "unit": unit,
                        "risk_stratum": stratum,
                        "numerator": str(counts[stratum]),
                        "risk_set_denominator": str(len(selected)),
                        "percent_of_risk_set": pct(counts[stratum], len(selected)),
                    }
                )
    write_tsv(
        tables / "risk_effective_strata_summary.tsv",
        ["arch", "unit", "risk_stratum", "numerator", "risk_set_denominator", "percent_of_risk_set"],
        risk_summary_rows,
    )

    intermediate_rows = []
    for rpm_sha in sorted(risk_package_shas):
        best: dict[tuple[str, str], list[tuple[str, str]]] = {}
        for root in nodes_by_package[rpm_sha]:
            queue_paths = deque([(root, [root])])
            visited = {root}
            while queue_paths:
                current, path = queue_paths.popleft()
                if current != root and current in direct_nodes:
                    previous = best.get(current)
                    if previous is None or len(path) < len(previous):
                        best[current] = path
                    continue
                for child in sorted(adjacency.get(current, set())):
                    if child not in visited:
                        visited.add(child)
                        queue_paths.append((child, path + [child]))
        root_package = package_by_sha[rpm_sha]
        for terminal, path in sorted(best.items()):
            terminal_elf = node_by_key[terminal]
            terminal_package = package_by_sha[terminal[0]]
            intermediate_rows.append(
                {
                    "root_package": package_id(root_package),
                    "root_arch": root_package["arch"],
                    "root_rpm_sha256": rpm_sha,
                    "intermediate_library_basename": os.path.basename(terminal[1]),
                    "intermediate_provider_package": package_id(terminal_package),
                    "intermediate_provider_rpm_sha256": terminal[0],
                    "intermediate_provider_path": terminal[1],
                    "shortest_edge_count_before_libgcc_s": str(len(path)),
                    "shortest_path": " -> ".join(node_by_key[key]["path"] for key in path) + " -> " + LIBGCC,
                }
            )
    intermediate_fields = [
        "root_package", "root_arch", "root_rpm_sha256", "intermediate_library_basename",
        "intermediate_provider_package", "intermediate_provider_rpm_sha256",
        "intermediate_provider_path", "shortest_edge_count_before_libgcc_s", "shortest_path",
    ]
    write_tsv(tables / "transitive_intermediate_paths.tsv", intermediate_fields, intermediate_rows)

    top_rows = []
    for arch in ["ALL"] + sorted({row["root_arch"] for row in intermediate_rows}):
        selected = intermediate_rows if arch == "ALL" else [row for row in intermediate_rows if row["root_arch"] == arch]
        grouped: dict[str, set[str]] = defaultdict(set)
        paths_by_group: dict[str, set[str]] = defaultdict(set)
        providers_by_group: dict[str, set[str]] = defaultdict(set)
        for row in selected:
            group = row["intermediate_library_basename"]
            grouped[group].add(row["root_rpm_sha256"])
            paths_by_group[group].add(row["shortest_path"])
            providers_by_group[group].add(row["intermediate_provider_package"])
        ranked = sorted(grouped, key=lambda item: (-len(grouped[item]), item))[:10]
        for rank, group in enumerate(ranked, 1):
            top_rows.append(
                {
                    "scope_arch": arch,
                    "rank": str(rank),
                    "intermediate_library_basename": group,
                    "provider_packages": ";".join(sorted(providers_by_group[group])),
                    "distinct_risk_binary_package_count": str(len(grouped[group])),
                    "risk_packages_with_transitive_path_denominator": str(len({row["root_rpm_sha256"] for row in selected})),
                    "representative_shortest_path": sorted(paths_by_group[group])[0],
                }
            )
    write_tsv(
        tables / "transitive_intermediate_top10.tsv",
        ["scope_arch", "rank", "intermediate_library_basename", "provider_packages",
         "distinct_risk_binary_package_count", "risk_packages_with_transitive_path_denominator",
         "representative_shortest_path"],
        top_rows,
    )

    plugin_rows = read_tsv(inputs["R11_PLUGIN_PACKAGES"])
    plugin_assignment = []
    for row in plugin_rows:
        result = package_class_by_sha.get(row["rpm_sha256"])
        plugin_assignment.append(
            {
                **row,
                "closure_status": result["closure_status"] if result else "NOT_FOUND",
                "libgcc_s_classification": result["libgcc_s_classification"] if result else "NOT_FOUND",
                "set_assignment": (
                    "RISK" if result and result["libgcc_s_classification"] in {"DIRECT", "TRANSITIVE"}
                    else "CLEAN" if result and result["libgcc_s_classification"] == "ABSENT"
                    else "UNDETERMINED_OR_NOT_FOUND"
                ),
            }
        )
    write_tsv(tables / "plugin_host_closure_assignment.tsv", list(plugin_assignment[0].keys()), plugin_assignment)
    plugin_summary = []
    for arch in ["ALL"] + sorted({row["arch"] for row in plugin_assignment}):
        selected = plugin_assignment if arch == "ALL" else [row for row in plugin_assignment if row["arch"] == arch]
        counts = Counter(row["set_assignment"] for row in selected)
        for assignment in ["RISK", "CLEAN", "UNDETERMINED_OR_NOT_FOUND"]:
            plugin_summary.append(
                {
                    "arch": arch,
                    "assignment": assignment,
                    "package_count": str(counts[assignment]),
                    "plugin_candidate_package_denominator": str(len(selected)),
                    "percent": pct(counts[assignment], len(selected)),
                }
            )
    write_tsv(
        tables / "plugin_host_closure_summary.tsv",
        ["arch", "assignment", "package_count", "plugin_candidate_package_denominator", "percent"],
        plugin_summary,
    )

    vendor_names = []
    with inputs["VENDOR_ARMV7L_ANCHORS"].open(encoding="utf-8") as stream:
        reader = csv.DictReader((line for line in stream if not line.startswith("#")), delimiter="\t")
        vendor_names = [row["pkg"] for row in reader if row["closure_v2_status"] == "ANCHOR"]
    if len(vendor_names) != 5:
        raise RuntimeError(f"expected five frozen armv7l vendor anchors, got {len(vendor_names)}")
    vendor_assignment = []
    for name in vendor_names:
        matches = [row for row in package_rows if row["name"] == name]
        if not matches:
            vendor_assignment.append(
                {"vendor_package_name": name, "arch": "NOT_FOUND", "nevra": "NOT_FOUND",
                 "rpm_sha256": "", "closure_status": "NOT_FOUND", "libgcc_s_classification": "NOT_FOUND",
                    "direct_elf_count": "", "transitive_elf_count": "", "set_assignment": "NOT_FOUND"}
            )
        for row in matches:
            vendor_assignment.append(
                {
                    "vendor_package_name": name,
                    "arch": row["arch"],
                    "nevra": package_id(package_by_sha[row["rpm_sha256"]]),
                    "rpm_sha256": row["rpm_sha256"],
                    "closure_status": row["closure_status"],
                    "libgcc_s_classification": row["libgcc_s_classification"],
                    "direct_elf_count": row["direct_elf_count"],
                    "transitive_elf_count": row["transitive_elf_count"],
                    "set_assignment": (
                        "RISK" if row["libgcc_s_classification"] in {"DIRECT", "TRANSITIVE"}
                        else "CLEAN" if row["libgcc_s_classification"] == "ABSENT" else "UNDETERMINED"
                    ),
                }
            )
    write_tsv(
        tables / "vendor_prebuilt_closure_assignment.tsv",
        ["vendor_package_name", "arch", "nevra", "rpm_sha256", "closure_status",
         "libgcc_s_classification", "direct_elf_count", "transitive_elf_count", "set_assignment"],
        vendor_assignment,
    )
    vendor_summary_rows = []
    for name in vendor_names:
        selected = [row for row in vendor_assignment if row["vendor_package_name"] == name]
        counts = Counter(row["set_assignment"] for row in selected)
        vendor_summary_rows.append(
            {
                "vendor_package_name": name,
                "binary_package_record_count": str(len(selected)),
                "risk_arch_records": str(counts["RISK"]),
                "clean_arch_records": str(counts["CLEAN"]),
                "undetermined_arch_records": str(counts["UNDETERMINED"] + counts["NOT_FOUND"]),
                "arch_assignments": ";".join(
                    f"{row['arch']}:{row['set_assignment']}" for row in sorted(selected, key=lambda item: item["arch"])
                ),
            }
        )
    write_tsv(
        tables / "vendor_prebuilt_name_summary.tsv",
        ["vendor_package_name", "binary_package_record_count", "risk_arch_records", "clean_arch_records",
         "undetermined_arch_records", "arch_assignments"],
        vendor_summary_rows,
    )

    abi_rows = read_tsv(inputs["R11_ABI_UNWIND_UND"])
    vrs_keys = {
        (row["rpm_sha256"], row["path"])
        for row in abi_rows
        if row["arch"] == "armv7l" and "Unwind_VRS" in row["symbol"]
    }
    arm_vrs_assignments = []
    for rpm_sha in sorted({key[0] for key in vrs_keys}):
        result = package_class_by_sha[rpm_sha]
        symbols = sorted({
            row["symbol_raw"] for row in abi_rows
            if row["rpm_sha256"] == rpm_sha and "Unwind_VRS" in row["symbol"]
        })
        arm_vrs_assignments.append(
            {
                **{field: result[field] for field in [
                    "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256",
                    "libgcc_s_classification",
                ]},
                "vrs_referencing_elf_count": str(len({key for key in vrs_keys if key[0] == rpm_sha})),
                "vrs_symbols": ";".join(symbols),
                "set_assignment": (
                    "RISK" if result["libgcc_s_classification"] in {"DIRECT", "TRANSITIVE"}
                    else "CLEAN" if result["libgcc_s_classification"] == "ABSENT" else "UNDETERMINED"
                ),
            }
        )
    arm_fields = list(arm_vrs_assignments[0].keys()) if arm_vrs_assignments else [
        "repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256",
        "libgcc_s_classification", "vrs_referencing_elf_count", "vrs_symbols", "set_assignment",
    ]
    write_tsv(tables / "armv7l_vrs_package_assignment.tsv", arm_fields, arm_vrs_assignments)
    arm_vrs_packages = [row for row in arm_vrs_assignments if row["set_assignment"] == "RISK"]
    write_tsv(tables / "armv7l_risk_vrs_packages.tsv", arm_fields, arm_vrs_packages)

    # Project c2 closure: R11 supplies identities/sections; R12 supplies the already-landed NEEDED rows absent from R11.
    c2_elfs = [row for row in read_tsv(inputs["R11_PROJECT_C2_ELF"]) if row["runtime_elf"] == "YES"]
    c2_needed_all = [row for row in read_tsv(inputs["R12_PROJECT_C2_NEEDED_SUPPLEMENT"]) if row["scheme"] == "scheme_a_libgcc"]
    c2_needed: dict[tuple[str, str], set[str]] = defaultdict(set)
    for row in c2_needed_all:
        c2_needed[(row["arch"], row["object"])].add(row["needed_soname"])
    c2_by_arch_object = {
        (row["arch"], "libcxx" if row["name"] == "libc++" else "libcxxabi"): row for row in c2_elfs
    }
    c2_rows = []
    for (arch, object_name), row in sorted(c2_by_arch_object.items()):
        values = c2_needed.get((arch, object_name), set())
        unresolved = []
        transitive_incomplete = []
        for soname in values:
            if soname == "libc++abi.so.1" and (arch, "libcxxabi") in c2_by_arch_object:
                continue
            status, candidates, _, reason = resolve(arch, soname)
            if status != "RESOLVED_UNIQUE":
                unresolved.append(f"{soname}:{status}")
            elif candidates[0] in incomplete_nodes:
                transitive_incomplete.append(f"{soname}:{candidates[0][1]}")
        complete = not unresolved and not transitive_incomplete
        classification = "DIRECT" if LIBGCC in values else "TRANSITIVE_OR_ABSENT_NOT_NEEDED_HERE"
        c2_rows.append(
            {
                "name": row["name"],
                "arch": arch,
                "rpm_sha256": row["rpm_sha256"],
                "path": row["path"],
                "dt_needed": ";".join(sorted(values)),
                "closure_status": "COMPLETE" if complete else "INCOMPLETE",
                "libgcc_s_classification": classification,
                "libgcc_s_presence_query_status": (
                    "DETERMINED" if LIBGCC in values else "UNDETERMINED"
                ),
                "own_dt_needed_libgcc_s": "YES" if LIBGCC in values else "NO",
                "unresolved_edges": ";".join(sorted(unresolved)) or "NONE",
                "reachable_incomplete_platform_nodes": ";".join(sorted(transitive_incomplete)) or "NONE",
            }
        )
    write_tsv(
        tables / "project_c2_closure_assignment.tsv",
        ["name", "arch", "rpm_sha256", "path", "dt_needed", "closure_status",
         "libgcc_s_classification", "libgcc_s_presence_query_status", "own_dt_needed_libgcc_s", "unresolved_edges",
         "reachable_incomplete_platform_nodes"],
        c2_rows,
    )

    r11_runtime_sourcerpms = {row["sourcerpm"] for row in elfs}
    r11_cpp_sourcerpms = {row["sourcerpm"] for row in elfs if row["cpp_indicator"] == "YES"}
    t1_rows = read_tsv(inputs["T1_SOURCE_SCOPE"])
    t1_sourcerpms = {row["source_rpm"] for row in t1_rows}
    source_comparison = []
    for sourcerpm in sorted(r11_cpp_sourcerpms | t1_sourcerpms):
        source_comparison.append(
            {
                "sourcerpm": sourcerpm,
                "in_R11_cpp_operational_source_set": "YES" if sourcerpm in r11_cpp_sourcerpms else "NO",
                "in_T1_libstdcxx_dependent_source_set": "YES" if sourcerpm in t1_sourcerpms else "NO",
                "exact_identity_intersection": "YES" if sourcerpm in r11_cpp_sourcerpms & t1_sourcerpms else "NO",
            }
        )
    write_tsv(
        tables / "r11_cpp_vs_t1_source_scope.tsv",
        ["sourcerpm", "in_R11_cpp_operational_source_set", "in_T1_libstdcxx_dependent_source_set",
         "exact_identity_intersection"],
        source_comparison,
    )

    dep_rows = read_tsv(inputs["R11_LIBUNWIND_DEPS"])
    binary_exact_names = sorted({
        row["package_name"] for row in dep_rows
        if row["dependency_scope"] == "binary_Requires" and row["classification"] == "EXACT_PACKAGE_NAME"
    })
    source_exact_names = sorted({
        row["package_name"] for row in dep_rows
        if row["dependency_scope"] == "source_BuildRequires" and row["classification"] == "EXACT_PACKAGE_NAME"
    })
    conflict_impact_rows = [
        {
            "scope": "binary_package_name",
            "package_name": name,
            "record_count": str(len({
                (row["repo_id"], row["package_name"], row["package_arch"], row["package_nevra"])
                for row in dep_rows
                if row["dependency_scope"] == "binary_Requires"
                and row["classification"] == "EXACT_PACKAGE_NAME"
                and row["package_name"] == name
            })),
            "denominator": "11332_binary_package_records",
            "predicate": "R11 exact Requires on package name libunwind or libunwind-devel",
        }
        for name in binary_exact_names
    ] + [
        {
            "scope": "source_package_name",
            "package_name": name,
            "record_count": "1",
            "denominator": "1450_source_package_records",
            "predicate": "R11 exact BuildRequires on package name libunwind or libunwind-devel",
        }
        for name in source_exact_names
    ]
    if len(conflict_impact_rows) != 7:
        raise RuntimeError(f"R11 seven-name conflict impact audit failed: {len(conflict_impact_rows)}")
    write_tsv(
        tables / "libunwind_conflict_impact_seven_names.tsv",
        ["scope", "package_name", "record_count", "denominator", "predicate"],
        conflict_impact_rows,
    )

    workload_rows = [
        {
            "item": "R11_CPP_OPERATIONAL_SOURCE_PACKAGES_WITH_RUNTIME_ELF",
            "numerator": str(len(r11_cpp_sourcerpms)),
            "denominator": str(len(r11_runtime_sourcerpms)),
            "unit": "source_RPM_identity",
            "predicate": "at least one owned runtime ELF has cpp_indicator=YES",
            "relation_to_T1": "different predicate and later R11 snapshot; exact identity relation reported separately",
        },
        {
            "item": "R11_CPP_OPERATIONAL_SOURCE_PACKAGES_OVER_ALL_SOURCE_RECORDS",
            "numerator": str(len(r11_cpp_sourcerpms)),
            "denominator": str(len(source_packages)),
            "unit": "source_RPM_identity",
            "predicate": "same R11 operational C++ set; denominator is all R11 source records",
            "relation_to_T1": "not a replacement for T1 dependency-defined scope",
        },
        {
            "item": "T1_LIBSTDCXX_DEPENDENT_SOURCE_PACKAGES_EXPECTED",
            "numerator": str(len(t1_sourcerpms)),
            "denominator": str(len(t1_sourcerpms)),
            "unit": "source_RPM_identity",
            "predicate": "T1 source set mapped from binary packages depending on libstdc++",
            "relation_to_T1": "T1 exact expected scope; 371/372 were corpus-scanned, Chromium was not",
        },
        {
            "item": "R11_CPP_AND_T1_EXACT_SOURCE_IDENTITY_INTERSECTION",
            "numerator": str(len(r11_cpp_sourcerpms & t1_sourcerpms)),
            "denominator": str(len(t1_sourcerpms)),
            "unit": "source_RPM_identity",
            "predicate": "exact sourcerpm string is present in both sets",
            "relation_to_T1": "version-sensitive exact identity join; no name-level inference",
        },
        {
            "item": "LIBUNWIND_FILE_CONFLICT_IMPACT_PACKAGE_NAMES_MIXED_UNITS",
            "numerator": str(len(conflict_impact_rows)),
            "denominator": "NOT_APPLICABLE_MIXED_UNITS",
            "unit": "3_binary_package_names_plus_4_source_package_names",
            "predicate": "R11 exact Requires/BuildRequires package-name metadata",
            "relation_to_T1": "independent of T1",
        },
    ]
    for arch in ["ALL"] + sorted({row["arch"] for row in risk_strata_package_rows}):
        selected = risk_strata_package_rows if arch == "ALL" else [
            row for row in risk_strata_package_rows if row["arch"] == arch
        ]
        validation = sum(row["risk_stratum"] != "C_NO_UNWIND_TABLE" for row in selected)
        workload_rows.append(
            {
                "item": f"RUNTIME_VALIDATION_PACKAGES_{arch}",
                "numerator": str(validation),
                "denominator": str(len(selected)),
                "unit": "positive_witness_risk_binary_package_record",
                "predicate": "risk stratum A plus B; package priority is A, then B, then C",
                "relation_to_T1": "runtime validation unit; not source rebuild unit",
            }
        )
    write_tsv(
        tables / "workload_mapping.tsv",
        ["item", "numerator", "denominator", "unit", "predicate", "relation_to_T1"],
        workload_rows,
    )

    methodology_rows = [
        {"item": "graph_node", "definition": "one R11 runtime ELF keyed by rpm_sha256 + path"},
        {"item": "graph_edge", "definition": "one measured R11 DT_NEEDED soname resolved by same-arch unversioned RPM Provides to a unique provider runtime ELF basename"},
        {"item": "provider_path_rule", "definition": "prefer exact basename==SONAME; otherwise require one basename beginning SONAME+'.'; multiple or zero candidates are unresolved"},
        {"item": "noarch_rule", "definition": "resolution architecture is the measured ELF machine mapped to a platform architecture; unknown machine is unresolved"},
        {"item": "closure_complete", "definition": "every transitively reachable DT_NEEDED edge resolves uniquely; reverse reachability propagates any unresolved edge to all roots"},
        {"item": "DIRECT", "definition": "root ELF itself has DT_NEEDED libgcc_s.so.1; positive evidence determines presence even when another, unrelated branch is unresolved"},
        {"item": "TRANSITIVE", "definition": "no direct edge at the classified unit, but a fully resolved positive dependency path reaches an ELF that DT_NEEDED libgcc_s.so.1"},
        {"item": "ABSENT", "definition": "complete measured closure contains no DT_NEEDED path to libgcc_s.so.1"},
        {"item": "UNDETERMINED", "definition": "no positive libgcc_s path was found and at least one reachable edge is unresolved; excluded from DIRECT/TRANSITIVE/ABSENT numerators"},
        {"item": "package_aggregation", "definition": "priority DIRECT then TRANSITIVE; ABSENT requires every owned runtime ELF closure complete; otherwise UNDETERMINED"},
        {"item": "scope_limit", "definition": "static repository closure, not a measured process image; dlopen/runtime search order and optional dependencies are outside this graph"},
    ]
    write_tsv(tables / "closure_methodology.tsv", ["item", "definition"], methodology_rows)

    summary = {
        "R11_BINARY_PACKAGE_RECORDS": len(packages),
        "R11_RUNTIME_ELF": len(elfs),
        "RUNTIME_ELF_PACKAGES": len(package_rows),
        "DT_NEEDED_EDGES": len(edge_resolution_rows),
        "RESOLVED_EDGES": edge_status["RESOLVED_UNIQUE"],
        "UNRESOLVED_EDGES": len(edge_resolution_rows) - edge_status["RESOLVED_UNIQUE"],
        "COMPLETE_ELF": sum(row["closure_status"] == "COMPLETE" for row in elf_rows),
        "INCOMPLETE_ELF": sum(row["closure_status"] == "INCOMPLETE" for row in elf_rows),
        "UNDETERMINED_ELF_QUERY": sum(row["libgcc_s_classification"] == "UNDETERMINED" for row in elf_rows),
        "COMPLETE_PACKAGES": sum(row["closure_status"] == "COMPLETE" for row in package_rows),
        "INCOMPLETE_PACKAGES": sum(row["closure_status"] == "INCOMPLETE" for row in package_rows),
        "UNDETERMINED_PACKAGE_QUERY": sum(row["libgcc_s_classification"] == "UNDETERMINED" for row in package_rows),
        "RISK_PACKAGES": len(risk_package_shas),
        "CLEAN_PACKAGES": sum(row["libgcc_s_classification"] == "ABSENT" for row in package_rows),
        "R11_CPP_SOURCE_PACKAGES": len(r11_cpp_sourcerpms),
        "T1_SOURCE_PACKAGES": len(t1_sourcerpms),
        "R11_T1_EXACT_INTERSECTION": len(r11_cpp_sourcerpms & t1_sourcerpms),
        "ARMV7L_RISK_VRS_PACKAGES": len(arm_vrs_packages),
        "ARMV7L_RISK_VRS_ELFS": len(vrs_keys & {key for key in node_by_key if key[0] in risk_package_shas}),
        "PLUGIN_ROWS": len(plugin_assignment),
        "VENDOR_NAMES": len(vendor_names),
        "PROJECT_C2_ROWS": len(c2_rows),
        "PROVIDER_RELATION_ROWS_USED": provider_relation_rows,
    }
    write_tsv(
        raw / "analysis_summary.tsv", ["metric", "value"],
        [{"metric": key, "value": str(value)} for key, value in summary.items()],
    )
    for key, value in summary.items():
        print(f"{key}={value}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
