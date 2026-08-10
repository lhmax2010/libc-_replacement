#!/usr/bin/env python3
import csv
import hashlib
import os
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(64 * 1024 * 1024)

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "progress/R24/tables"
OUT.mkdir(parents=True, exist_ok=True)


def read(rel):
    with (ROOT / rel).open(encoding="utf-8", newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))


def write(name, fields, rows):
    with (OUT / name).open("w", encoding="utf-8", newline="") as handle:
        writer = csv.DictWriter(
            handle,
            fieldnames=fields,
            delimiter="\t",
            lineterminator="\n",
            extrasaction="ignore",
        )
        writer.writeheader()
        writer.writerows(rows)


full = read("progress/R22/tables/migration_packages_full.tsv")
full_by = {row["source_rpm"]: row for row in full}
stage = {row["source_rpm"]: row["stage"] for row in full}
vendor_nodes = {node for node, value in stage.items() if value == "VENDOR_PREBUILT_SEPARATE"}
migration_nodes = set(stage) - vendor_nodes

classified = read("progress/R22/tables/source_edge_classification.tsv")
true_all = {
    (row["consumer_source_rpm"], row["provider_source_rpm"])
    for row in classified
    if row["classification"] == "TRUE_CPP_ABI_COUPLING"
}
true_edges = {
    (consumer, provider)
    for consumer, provider in true_all
    if consumer in migration_nodes and provider in migration_nodes
}

# Tarjan SCC on the exact R22 migration graph. Edge direction is consumer -> provider.
graph = defaultdict(set)
for consumer, provider in true_edges:
    graph[consumer].add(provider)

index = 0
stack = []
on_stack = set()
indices = {}
lowlinks = {}
components = []


def strongconnect(node):
    global index
    indices[node] = index
    lowlinks[node] = index
    index += 1
    stack.append(node)
    on_stack.add(node)
    for target in sorted(graph.get(node, set())):
        if target not in indices:
            strongconnect(target)
            lowlinks[node] = min(lowlinks[node], lowlinks[target])
        elif target in on_stack:
            lowlinks[node] = min(lowlinks[node], indices[target])
    if lowlinks[node] == indices[node]:
        component = []
        while True:
            current = stack.pop()
            on_stack.remove(current)
            component.append(current)
            if current == node:
                break
        components.append(sorted(component))


for node in sorted(migration_nodes):
    if node not in indices:
        strongconnect(node)

components.sort(key=lambda members: (-len(members), members))
component_id = {
    node: f"R24SCC{number:04d}"
    for number, members in enumerate(components, 1)
    for node in members
}
component_members = {
    f"R24SCC{number:04d}": members
    for number, members in enumerate(components, 1)
}
nontrivial = {cid: members for cid, members in component_members.items() if len(members) > 1}

scc_membership = []
for cid, members in component_members.items():
    member_stages = sorted({stage[node] for node in members})
    for node in members:
        scc_membership.append(
            {
                "scc_id": cid,
                "scc_size": len(members),
                "source_rpm": node,
                "package_name": full_by[node]["package_name"],
                "stage": stage[node],
                "scc_stage_set": ";".join(member_stages),
                "cross_stage": "YES" if len(member_stages) > 1 else "NO",
                "members": ";".join(members),
            }
        )
write(
    "scc_membership.tsv",
    ["scc_id", "scc_size", "source_rpm", "package_name", "stage", "scc_stage_set", "cross_stage", "members"],
    scc_membership,
)
write(
    "nontrivial_scc.tsv",
    ["scc_id", "scc_size", "stage", "source_rpm", "package_name", "members"],
    [
        {
            "scc_id": cid,
            "scc_size": len(members),
            "stage": stage[node],
            "source_rpm": node,
            "package_name": full_by[node]["package_name"],
            "members": ";".join(members),
        }
        for cid, members in nontrivial.items()
        for node in members
    ],
)
write(
    "scc_summary.tsv",
    ["metric", "numerator", "denominator", "unit", "status_or_note"],
    [
        {"metric": "all SCC", "numerator": len(components), "denominator": len(migration_nodes), "unit": "SCC / migration source RPM", "status_or_note": "MEASURED"},
        {"metric": "nontrivial SCC", "numerator": len(nontrivial), "denominator": len(components), "unit": "SCC", "status_or_note": "MEASURED"},
        {"metric": "packages covered by nontrivial SCC", "numerator": sum(map(len, nontrivial.values())), "denominator": len(migration_nodes), "unit": "source RPM", "status_or_note": "MEASURED"},
        {"metric": "largest SCC", "numerator": max(map(len, components), default=0), "denominator": len(migration_nodes), "unit": "source RPM", "status_or_note": "MEASURED"},
        {"metric": "nontrivial SCC split across stages", "numerator": sum(len({stage[n] for n in members}) > 1 for members in nontrivial.values()), "denominator": len(nontrivial), "unit": "SCC", "status_or_note": "PASS if zero"},
    ],
)

# Exact deleted/retained edge accounting relative to R20's 1,705 candidate source edges.
deleted = []
for row in classified:
    if row["classification"] == "TRUE_CPP_ABI_COUPLING":
        continue
    deleted.append(
        {
            "consumer_source_rpm": row["consumer_source_rpm"],
            "provider_source_rpm": row["provider_source_rpm"],
            "R20_consumer_phase": row["R20_consumer_phase"],
            "R20_provider_phase": row["R20_provider_phase"],
            "R22_classification": row["classification"],
            "graph_action": "REMOVED_FROM_CPP_ABI_GRAPH",
            "evidence_DT_NEEDED_rows": row["evidence_DT_NEEDED_rows"],
            "cpp_symbol_count": row["cpp_symbol_count"],
            "c_symbol_count": row["c_symbol_count"],
            "cpp_symbols": row["cpp_symbols"],
            "c_symbols": row["c_symbols"],
            "basis": "all intersecting symbols non-_Z" if row["classification"] == "PURE_C_INTERFACE" else "no dynamic-symbol intersection observed",
        }
    )
write(
    "deleted_candidate_edges.tsv",
    ["consumer_source_rpm", "provider_source_rpm", "R20_consumer_phase", "R20_provider_phase", "R22_classification", "graph_action", "evidence_DT_NEEDED_rows", "cpp_symbol_count", "c_symbol_count", "cpp_symbols", "c_symbols", "basis"],
    deleted,
)

reverse_pairs = {
    (row["consumer_source_rpm"], row["provider_source_rpm"])
    for row in read("progress/R19_21/R20/tables/inverse_phase_dependencies.tsv")
}
initial_reverse = []
for row in classified:
    pair = (row["consumer_source_rpm"], row["provider_source_rpm"])
    if pair not in reverse_pairs:
        continue
    initial_reverse.append(
        {
            "consumer_source_rpm": pair[0],
            "provider_source_rpm": pair[1],
            "R20_consumer_phase": row["R20_consumer_phase"],
            "R20_provider_phase": row["R20_provider_phase"],
            "R22_classification": row["classification"],
            "graph_action": "RETAINED_AND_REPHASED" if row["classification"] == "TRUE_CPP_ABI_COUPLING" else "REMOVED_FROM_CPP_ABI_GRAPH",
            "cpp_symbol_count": row["cpp_symbol_count"],
            "c_symbol_count": row["c_symbol_count"],
            "cpp_symbols": row["cpp_symbols"],
            "c_symbols": row["c_symbols"],
        }
    )
write(
    "initial_462_edge_disposition.tsv",
    ["consumer_source_rpm", "provider_source_rpm", "R20_consumer_phase", "R20_provider_phase", "R22_classification", "graph_action", "cpp_symbol_count", "c_symbol_count", "cpp_symbols", "c_symbols"],
    initial_reverse,
)
write(
    "edge_disposition_summary.tsv",
    ["scope", "classification_or_action", "edge_count", "denominator", "unit"],
    [
        {"scope": "R20_ALL_CANDIDATES", "classification_or_action": "RETAINED_TRUE_CPP_ABI", "edge_count": len(true_all), "denominator": len(classified), "unit": "directed source RPM edge"},
        {"scope": "R20_ALL_CANDIDATES", "classification_or_action": "REMOVED_PURE_C_INTERFACE", "edge_count": sum(r["classification"] == "PURE_C_INTERFACE" for r in classified), "denominator": len(classified), "unit": "directed source RPM edge"},
        {"scope": "R20_ALL_CANDIDATES", "classification_or_action": "REMOVED_NO_SYMBOL_INTERSECTION", "edge_count": sum(r["classification"] == "OTHER_NO_SYMBOL_INTERSECTION" for r in classified), "denominator": len(classified), "unit": "directed source RPM edge"},
        {"scope": "R20_INITIAL_REVERSE", "classification_or_action": "RETAINED_AND_REPHASED", "edge_count": sum(r["graph_action"] == "RETAINED_AND_REPHASED" for r in initial_reverse), "denominator": len(initial_reverse), "unit": "directed source RPM edge"},
        {"scope": "R20_INITIAL_REVERSE", "classification_or_action": "REMOVED_FROM_CPP_ABI_GRAPH", "edge_count": sum(r["graph_action"] == "REMOVED_FROM_CPP_ABI_GRAPH" for r in initial_reverse), "denominator": len(initial_reverse), "unit": "directed source RPM edge"},
    ],
)

# Internal coupling and provider-first layers for each stage's induced subgraph.
edge_by_pair = {
    (row["consumer_source_rpm"], row["provider_source_rpm"]): row
    for row in classified
    if row["classification"] == "TRUE_CPP_ABI_COUPLING"
}
internal_rows = []
for consumer, provider in sorted(true_edges):
    if stage[consumer] != stage[provider]:
        continue
    evidence = edge_by_pair[(consumer, provider)]
    internal_rows.append(
        {
            "stage": stage[consumer],
            "consumer_source_rpm": consumer,
            "consumer_package_name": full_by[consumer]["package_name"],
            "provider_source_rpm": provider,
            "provider_package_name": full_by[provider]["package_name"],
            "evidence_DT_NEEDED_rows": evidence["evidence_DT_NEEDED_rows"],
            "cpp_symbol_count": evidence["cpp_symbol_count"],
            "cpp_symbols": evidence["cpp_symbols"],
        }
    )
write(
    "stage_internal_edges.tsv",
    ["stage", "consumer_source_rpm", "consumer_package_name", "provider_source_rpm", "provider_package_name", "evidence_DT_NEEDED_rows", "cpp_symbol_count", "cpp_symbols"],
    internal_rows,
)

layer_rows = []
order_rows = []
stage_summaries = []
for stage_name in ["PHASE1", "PHASE2", "PHASE3"]:
    stage_nodes = {node for node in migration_nodes if stage[node] == stage_name}
    stage_edges = {(a, b) for a, b in true_edges if a in stage_nodes and b in stage_nodes}
    remaining = set(stage_nodes)
    layer = 0
    assigned = {}
    while remaining:
        ready = sorted(node for node in remaining if not ({b for a, b in stage_edges if a == node} & remaining))
        if not ready:
            # This branch is not expected after SCC condensation. Preserve evidence if encountered.
            ready = sorted(remaining)
        for node in ready:
            assigned[node] = layer
        layer_rows.append(
            {
                "stage": stage_name,
                "provider_first_layer": layer,
                "source_package_count": len(ready),
                "stage_source_package_denominator": len(stage_nodes),
                "package_names": ";".join(full_by[node]["package_name"] for node in ready),
                "source_rpms": ";".join(ready),
            }
        )
        remaining -= set(ready)
        layer += 1
    for node in sorted(stage_nodes, key=lambda n: (assigned[n], full_by[n]["package_name"], n)):
        providers = sorted(b for a, b in stage_edges if a == node)
        order_rows.append(
            {
                "stage": stage_name,
                "provider_first_layer": assigned[node],
                "source_rpm": node,
                "package_name": full_by[node]["package_name"],
                "internal_provider_dependency_count": len(providers),
                "internal_provider_source_rpms": ";".join(providers),
                "scc_id": component_id[node] if len(component_members[component_id[node]]) > 1 else "-",
            }
        )
    stage_nontrivial = {
        component_id[node]
        for node in stage_nodes
        if len(component_members[component_id[node]]) > 1
    }
    property_text = (
        "INDEPENDENT_WITHIN_MEASURED_CPP_ABI_GRAPH"
        if not stage_edges
        else "DEPENDENCY_ORDERED_NOT_INDEPENDENT_BULK"
    )
    stage_summaries.append(
        {
            "stage": stage_name,
            "source_package_count": len(stage_nodes),
            "denominator_all_483_source_packages": len(full),
            "internal_cpp_abi_edge_count": len(stage_edges),
            "denominator_migration_graph_true_edges": len(true_edges),
            "internal_nontrivial_scc_count": len(stage_nontrivial),
            "packages_in_internal_nontrivial_scc": sum(len(component_members[cid]) for cid in stage_nontrivial),
            "provider_first_layer_count": layer,
            "accurate_property": property_text,
        }
    )
write(
    "stage_topology_layers.tsv",
    ["stage", "provider_first_layer", "source_package_count", "stage_source_package_denominator", "package_names", "source_rpms"],
    layer_rows,
)
write(
    "stage_topology_order.tsv",
    ["stage", "provider_first_layer", "source_rpm", "package_name", "internal_provider_dependency_count", "internal_provider_source_rpms", "scc_id"],
    order_rows,
)
write(
    "stage_internal_coupling_summary.tsv",
    ["stage", "source_package_count", "denominator_all_483_source_packages", "internal_cpp_abi_edge_count", "denominator_migration_graph_true_edges", "internal_nontrivial_scc_count", "packages_in_internal_nontrivial_scc", "provider_first_layer_count", "accurate_property"],
    stage_summaries,
)

stage_edge_matrix = []
for consumer_stage in ["PHASE1", "PHASE2", "PHASE3"]:
    for provider_stage in ["PHASE1", "PHASE2", "PHASE3"]:
        count = sum(
            stage[consumer] == consumer_stage and stage[provider] == provider_stage
            for consumer, provider in true_edges
        )
        relation = "INTERNAL" if consumer_stage == provider_stage else (
            "PROVIDER_IN_EARLIER_STAGE"
            if int(provider_stage[-1]) < int(consumer_stage[-1])
            else "REVERSE_STAGE_VIOLATION"
        )
        stage_edge_matrix.append(
            {
                "consumer_stage": consumer_stage,
                "provider_stage": provider_stage,
                "edge_count": count,
                "denominator_migration_graph_true_edges": len(true_edges),
                "relation": relation,
            }
        )
write(
    "stage_edge_matrix.tsv",
    ["consumer_stage", "provider_stage", "edge_count", "denominator_migration_graph_true_edges", "relation"],
    stage_edge_matrix,
)

# Third-stage composition: use the implemented R22 rule, not the earlier R20 prose.
special = read("progress/R22/tables/special_cases.tsv")
plugin_sources = {row["source_rpm"] for row in special if row["type"] == "CPP_PLUGIN_HOST_SOURCE"}
phase3_nodes = {node for node in migration_nodes if stage[node] == "PHASE3"}
phase3_rows = []
for node in sorted(phase3_nodes):
    cid = component_id[node]
    is_nontrivial = len(component_members[cid]) > 1
    is_plugin = node in plugin_sources
    depth = next(
        row["topological_depth"]
        for row in read("progress/R22/tables/scc_topology.tsv")
        if node in row["members"].split(";")
    )
    phase3_rows.append(
        {
            "source_rpm": node,
            "package_name": full_by[node]["package_name"],
            "nontrivial_scc_member": "YES" if is_nontrivial else "NO",
            "plugin_host": "YES" if is_plugin else "NO",
            "both": "YES" if is_nontrivial and is_plugin else "NO",
            "R22_provider_first_depth": depth,
            "actual_inclusion_basis": "provider-first depth > cutoff 2",
            "binary_package_name_count": full_by[node]["binary_package_count"],
        }
    )
write(
    "phase3_composition.tsv",
    ["source_rpm", "package_name", "nontrivial_scc_member", "plugin_host", "both", "R22_provider_first_depth", "actual_inclusion_basis", "binary_package_name_count"],
    phase3_rows,
)
write(
    "phase3_composition_summary.tsv",
    ["category", "source_package_count", "denominator_phase3_source_packages", "unit"],
    [
        {"category": "nontrivial SCC member", "source_package_count": sum(r["nontrivial_scc_member"] == "YES" for r in phase3_rows), "denominator_phase3_source_packages": len(phase3_rows), "unit": "source RPM"},
        {"category": "plugin host", "source_package_count": sum(r["plugin_host"] == "YES" for r in phase3_rows), "denominator_phase3_source_packages": len(phase3_rows), "unit": "source RPM"},
        {"category": "both", "source_package_count": sum(r["both"] == "YES" for r in phase3_rows), "denominator_phase3_source_packages": len(phase3_rows), "unit": "source RPM"},
        {"category": "depth-only under implemented R22 rule", "source_package_count": sum(r["nontrivial_scc_member"] == "NO" and r["plugin_host"] == "NO" for r in phase3_rows), "denominator_phase3_source_packages": len(phase3_rows), "unit": "source RPM"},
    ],
)

# Reconcile the historical 0723 core component (binary package graph) with R22 phase 3.
presence = {row["package"]: row for row in read("census/multiarch_v2/comparison/package_presence.tsv")}
binary_records = read("progress/R11/tables/binary_package_records.tsv")
elf_inventory = read("progress/R11/tables/elf_inventory.tsv")


def arch_value(field, arch):
    values = [item.split(":", 1)[1] for item in field.split(";") if item and item.split(":", 1)[0] == arch]
    if len(values) != 1:
        raise RuntimeError(f"expected one {arch} value, got {values!r}")
    return values[0]


core_member_rows = []
reconcile_rows = []
core_source_names_union = set()
core_source_rpms_union = set()
phase3_source_names = {full_by[node]["package_name"] for node in phase3_nodes}
for arch in ["armv7l", "aarch64", "x86_64"]:
    overlay = read(f"ledger/overlay_{arch}_tier1.tsv")
    core = next(row for row in overlay if row["component_id"] == "tier1_production_0001")
    packages = core["pkg_list"].split(",")
    source_rpms = set()
    source_names = set()
    for package in packages:
        row = presence[package]
        source_rpm = arch_value(row["source_rpm_bases"], arch)
        source_rpms.add(source_rpm)
        # Establish source name from another member sharing this exact source RPM when needed.
        candidates = []
        for package2, row2 in presence.items():
            try:
                if arch_value(row2["source_rpm_bases"], arch) != source_rpm:
                    continue
                version = arch_value(row2["versions"], arch)
            except RuntimeError:
                continue
            suffix = f"-{version}.src.rpm"
            if source_rpm.endswith(suffix):
                candidates.append(source_rpm[: -len(suffix)])
        source_name = sorted(set(candidates))[0] if len(set(candidates)) == 1 else "NOT_AVAILABLE"
        if source_name != "NOT_AVAILABLE":
            source_names.add(source_name)
        core_member_rows.append(
            {
                "arch": arch,
                "historical_binary_package": package,
                "historical_source_rpm": source_rpm,
                "historical_source_package_name": source_name,
                "historical_component": core["component_id"],
                "historical_snapshot": core["snapshot"],
                "R22_phase3_source_name_match": "YES" if source_name in phase3_source_names else "NO",
            }
        )
    core_source_names_union |= source_names
    core_source_rpms_union |= source_rpms
    phase3_binary = [row for row in binary_records if row["arch"] == arch and row["sourcerpm"] in phase3_nodes]
    phase3_runtime_elf = [row for row in elf_inventory if row["arch"] == arch and row["runtime_elf"] == "YES" and row["sourcerpm"] in phase3_nodes]
    phase3_binary_names = {row["name"] for row in phase3_binary}
    reconcile_rows.append(
        {
            "arch": arch,
            "historical_core_binary_package_count": len(packages),
            "historical_core_source_rpm_count": len(source_rpms),
            "historical_core_source_name_count": len(source_names),
            "historical_core_internal_effective_edge_count": {"armv7l": 198, "aarch64": 201, "x86_64": 204}[arch],
            "R22_phase3_source_rpm_count": len(phase3_nodes),
            "R22_phase3_binary_rpm_record_count": len(phase3_binary),
            "R22_phase3_runtime_elf_count": len(phase3_runtime_elf),
            "binary_package_name_intersection_count": len(set(packages) & phase3_binary_names),
            "source_package_name_intersection_count": len(source_names & phase3_source_names),
            "same_object": "NO",
        }
    )
write(
    "historical_core_members_source_mapping.tsv",
    ["arch", "historical_binary_package", "historical_source_rpm", "historical_source_package_name", "historical_component", "historical_snapshot", "R22_phase3_source_name_match"],
    core_member_rows,
)
write(
    "core_phase3_same_unit_comparison.tsv",
    ["arch", "historical_core_binary_package_count", "historical_core_source_rpm_count", "historical_core_source_name_count", "historical_core_internal_effective_edge_count", "R22_phase3_source_rpm_count", "R22_phase3_binary_rpm_record_count", "R22_phase3_runtime_elf_count", "binary_package_name_intersection_count", "source_package_name_intersection_count", "same_object"],
    reconcile_rows,
)
write(
    "historical_core_cross_arch_union.tsv",
    ["metric", "count", "unit", "denominator_or_note"],
    [
        {"metric": "historical core distinct source package names across three architectures", "count": len(core_source_names_union), "unit": "source package Name", "denominator_or_note": "union of exact 0723 tier1_production_0001 members"},
        {"metric": "historical core distinct source RPM NVR identities across three architectures", "count": len(core_source_rpms_union), "unit": "source RPM identity", "denominator_or_note": "union; arch-specific releases remain distinct"},
        {"metric": "R22 phase3 distinct source RPM identities", "count": len(phase3_nodes), "unit": "source RPM identity", "denominator_or_note": "R11/R22 later composite snapshot"},
    ],
)

reconciliation_differences = []
for row in reconcile_rows:
    arch = row["arch"]
    reconciliation_differences.extend(
        [
            {
                "arch": arch,
                "unit": "source package",
                "historical_core_count": row["historical_core_source_rpm_count"],
                "R22_phase3_count": row["R22_phase3_source_rpm_count"],
                "intersection_count": row["source_package_name_intersection_count"],
                "historical_only_count": int(row["historical_core_source_name_count"]) - int(row["source_package_name_intersection_count"]),
                "phase3_only_count": int(row["R22_phase3_source_rpm_count"]) - int(row["source_package_name_intersection_count"]),
                "scope_note": "historical side counted by source Name for overlap; historical source-RPM NVR count is equal on each arch",
            },
            {
                "arch": arch,
                "unit": "binary package",
                "historical_core_count": row["historical_core_binary_package_count"],
                "R22_phase3_count": row["R22_phase3_binary_rpm_record_count"],
                "intersection_count": row["binary_package_name_intersection_count"],
                "historical_only_count": int(row["historical_core_binary_package_count"]) - int(row["binary_package_name_intersection_count"]),
                "phase3_only_count": int(row["R22_phase3_binary_rpm_record_count"]) - int(row["binary_package_name_intersection_count"]),
                "scope_note": "historical side is binary package Name; R22 side is one binary RPM record per Name/arch in this subset",
            },
            {
                "arch": arch,
                "unit": "runtime ELF",
                "historical_core_count": "NOT_AVAILABLE",
                "R22_phase3_count": row["R22_phase3_runtime_elf_count"],
                "intersection_count": "NOT_AVAILABLE",
                "historical_only_count": "NOT_AVAILABLE",
                "phase3_only_count": "NOT_AVAILABLE",
                "scope_note": "aligned-0723 conclusion artifacts do not retain a core-member runtime-ELF count",
            },
        ]
    )
write(
    "reconciliation_same_unit_differences.tsv",
    ["arch", "unit", "historical_core_count", "R22_phase3_count", "intersection_count", "historical_only_count", "phase3_only_count", "scope_note"],
    reconciliation_differences,
)
write(
    "reconciliation_rule_snapshot_differences.tsv",
    ["dimension", "historical_core_118_121_122", "R22_phase3_16", "quantifiable_contribution"],
    [
        {"dimension": "snapshot", "historical_core_118_121_122": "Base tizen-base_20260709.234055 + Unified tizen-unified_20260723.231947", "R22_phase3_16": "R11 composite revisions 1785866115/1785904307/1785866095/1785904240", "quantifiable_contribution": "NOT_SEPARABLY_IDENTIFIABLE_FROM_RETAINED_INPUTS"},
        {"dimension": "node", "historical_core_118_121_122": "production binary package Name", "R22_phase3_16": "exact source RPM identity", "quantifiable_contribution": "source-unit values 62/65/64 versus 16"},
        {"dimension": "edge/rule", "historical_core_118_121_122": "D2-retained CPP_ABI plus A8/A9 promoted evidence; one largest SCC", "R22_phase3_16": "dynamic UND/DEF _Z intersection; provider-first depth 3-5", "quantifiable_contribution": "NOT_SEPARABLY_IDENTIFIABLE_FROM_RETAINED_INPUTS"},
        {"dimension": "membership overlap", "historical_core_118_121_122": "per-arch core member sets", "R22_phase3_16": "per-arch binary outputs/source names of phase3", "quantifiable_contribution": "13 binary Names and 7 source Names overlap on each architecture"},
    ],
)

# Fixed 483-row handoff list.
delivery = []
for row in sorted(full, key=lambda item: (item["stage"], item["package_name"], item["source_rpm"])):
    node = row["source_rpm"]
    cid = component_id.get(node)
    nontrivial_id = cid if cid and len(component_members[cid]) > 1 else "-"
    delivery.append(
        {
            "package_name": row["package_name"],
            "source_rpm_identity": node,
            "stage": row["stage"],
            "binary_package_count": row["binary_package_count"],
            "source_modification_required": row["source_change_requirement"],
            "build_config_modification_required": row["build_system_change_requirement"],
            "scc_id": nontrivial_id,
            "plugin_host": "YES" if node in plugin_sources else "NO",
            "vendor_prebuilt": "YES" if node in vendor_nodes else "NO",
            "in_dual_runtime_risk_set": row["in_R13_risk_set"],
            "remarks": row["remarks"],
        }
    )
write(
    "migration_packages_483.tsv",
    ["package_name", "source_rpm_identity", "stage", "binary_package_count", "source_modification_required", "build_config_modification_required", "scc_id", "plugin_host", "vendor_prebuilt", "in_dual_runtime_risk_set", "remarks"],
    delivery,
)

summary = []
for stage_name in ["PHASE1", "PHASE2", "PHASE3", "VENDOR_PREBUILT_SEPARATE"]:
    rows = [row for row in delivery if row["stage"] == stage_name]
    summary.append(
        {
            "stage": stage_name,
            "source_package_count": len(rows),
            "denominator_all_source_packages": len(delivery),
            "binary_package_count_sum": sum(int(row["binary_package_count"]) for row in rows),
            "source_modification_yes": sum(row["source_modification_required"] == "YES" for row in rows),
            "source_modification_unknown": sum(row["source_modification_required"] == "UNKNOWN" for row in rows),
            "build_config_modification_yes": sum(row["build_config_modification_required"] == "YES" for row in rows),
            "build_config_modification_unknown": sum(row["build_config_modification_required"] == "UNKNOWN" for row in rows),
            "plugin_host_count": sum(row["plugin_host"] == "YES" for row in rows),
            "vendor_prebuilt_count": sum(row["vendor_prebuilt"] == "YES" for row in rows),
            "risk_set_yes_count": sum(row["in_dual_runtime_risk_set"] == "YES" for row in rows),
        }
    )
write(
    "migration_packages_483_summary.tsv",
    ["stage", "source_package_count", "denominator_all_source_packages", "binary_package_count_sum", "source_modification_yes", "source_modification_unknown", "build_config_modification_yes", "build_config_modification_unknown", "plugin_host_count", "vendor_prebuilt_count", "risk_set_yes_count"],
    summary,
)
write(
    "migration_packages_483_INDEX.tsv",
    ["file", "row_count", "unit", "sha256", "size_bytes", "scope"],
    [
        {
            "file": "tables/migration_packages_483.tsv",
            "row_count": len(delivery),
            "unit": "source RPM identity",
            "sha256": hashlib.sha256((OUT / "migration_packages_483.tsv").read_bytes()).hexdigest(),
            "size_bytes": (OUT / "migration_packages_483.tsv").stat().st_size,
            "scope": "R22 adaptation set; 483/483",
        },
        {
            "file": "tables/migration_packages_483_summary.tsv",
            "row_count": len(summary),
            "unit": "stage",
            "sha256": hashlib.sha256((OUT / "migration_packages_483_summary.tsv").read_bytes()).hexdigest(),
            "size_bytes": (OUT / "migration_packages_483_summary.tsv").stat().st_size,
            "scope": "four stage/separate categories",
        },
    ],
)

# Assertions are evidence, not an attempt to alter the input classification.
assertions = []


def check(name, condition, observed):
    assertions.append({"assertion": name, "status": "PASS" if condition else "FAIL", "observed": observed})


check("delivery list is exactly 483 source RPM rows", len(delivery) == 483, str(len(delivery)))
check("migration graph has exactly 479 non-vendor nodes", len(migration_nodes) == 479, str(len(migration_nodes)))
check("classified candidate edge count is 1705", len(classified) == 1705, str(len(classified)))
check("true ABI edge count is 1234 before vendor separation", len(true_all) == 1234, str(len(true_all)))
check("migration graph true ABI edge count is 1222", len(true_edges) == 1222, str(len(true_edges)))
check("nontrivial SCC count", len(nontrivial) == 0, str(len(nontrivial)))
check("all nontrivial SCC stay within one stage", all(len({stage[n] for n in members}) == 1 for members in nontrivial.values()), str(sum(len({stage[n] for n in members}) > 1 for members in nontrivial.values())))
check("initial reverse disposition has 462 rows", len(initial_reverse) == 462, str(len(initial_reverse)))
check("initial reverse true edges retained", sum(r["graph_action"] == "RETAINED_AND_REPHASED" for r in initial_reverse) == 452, str(sum(r["graph_action"] == "RETAINED_AND_REPHASED" for r in initial_reverse)))
check("initial reverse false edges removed", sum(r["graph_action"] == "REMOVED_FROM_CPP_ABI_GRAPH" for r in initial_reverse) == 10, str(sum(r["graph_action"] == "REMOVED_FROM_CPP_ABI_GRAPH" for r in initial_reverse)))
check("phase counts remain 13/450/16/4", [sum(r["stage"] == value for r in delivery) for value in ["PHASE1", "PHASE2", "PHASE3", "VENDOR_PREBUILT_SEPARATE"]] == [13, 450, 16, 4], "/".join(str(sum(r["stage"] == value for r in delivery)) for value in ["PHASE1", "PHASE2", "PHASE3", "VENDOR_PREBUILT_SEPARATE"]))
check("phase 2 internal edge count is nonzero", next(r for r in stage_summaries if r["stage"] == "PHASE2")["internal_cpp_abi_edge_count"] > 0, str(next(r for r in stage_summaries if r["stage"] == "PHASE2")["internal_cpp_abi_edge_count"]))
check("historical core source mappings complete", len(core_member_rows) == 361 and all(r["historical_source_package_name"] != "NOT_AVAILABLE" for r in core_member_rows), f"{sum(r['historical_source_package_name'] != 'NOT_AVAILABLE' for r in core_member_rows)}/{len(core_member_rows)}")
write("assertions.tsv", ["assertion", "status", "observed"], assertions)

failed = [row for row in assertions if row["status"] != "PASS"]
if failed:
    raise SystemExit("assertion failures: " + repr(failed))

print(f"delivery_rows={len(delivery)}")
print(f"migration_nodes={len(migration_nodes)} true_edges={len(true_edges)}")
print(f"scc_total={len(components)} nontrivial={len(nontrivial)} largest={max(map(len, components), default=0)}")
print("internal_edges=" + ",".join(f"{row['stage']}:{row['internal_cpp_abi_edge_count']}" for row in stage_summaries))
print("layers=" + ",".join(f"{row['stage']}:{row['provider_first_layer_count']}" for row in stage_summaries))
print("core_source_rpm_units=" + ",".join(f"{row['arch']}:{row['historical_core_source_rpm_count']}" for row in reconcile_rows))
