#!/usr/bin/env python3
import csv
import hashlib
import re
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "progress/R27"
TABLES = OUT / "tables"
INPUTS = OUT / "inputs"
TABLES.mkdir(parents=True, exist_ok=True)
INPUTS.mkdir(parents=True, exist_ok=True)

PATHS = {
    "r11_snapshot": ROOT / "progress/R11/tables/snapshot_identity.tsv",
    "r22_source_edges": ROOT / "progress/R22/tables/source_edge_classification.tsv",
    "r22_classifier": ROOT / "progress/R22/tools/classify_abi_edges.py",
    "r24_packages": ROOT / "progress/R24/tables/migration_packages_483.tsv",
    "r24_topology": ROOT / "progress/R24/tables/stage_topology_order.tsv",
    "r25_cut_edges": ROOT / "progress/R25/tables/cut_cross_boundary_edges.tsv",
    "r25_cut_summary": ROOT / "progress/R25/tables/cut_summary.tsv",
    "r25_vendor_accounting": ROOT / "progress/R25/tables/vendor_five_to_four_accounting.tsv",
    "r26_edges": ROOT / "progress/R26/tables/edge_semantic_classification.tsv",
    "r26_providers": ROOT / "progress/R26/tables/provider_semantic_summary.tsv",
    "r26_method": ROOT / "progress/R26/tables/classification_method.tsv",
}

def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)

def sha256(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()

packages = read_tsv(PATHS["r24_packages"])
pkg = {r["source_rpm_identity"]: r for r in packages}
topology = {r["source_rpm"]: r for r in read_tsv(PATHS["r24_topology"])}
source_edges_all = read_tsv(PATHS["r22_source_edges"])
edges = [r for r in source_edges_all if r["classification"] == "TRUE_CPP_ABI_COUPLING"]
edge_by_key = {(r["consumer_source_rpm"], r["provider_source_rpm"]): r for r in edges}
r26_rows = read_tsv(PATHS["r26_edges"])
r26_by_key = {(r["consumer_source_rpm"], r["provider_source_rpm"]): r for r in r26_rows}
seed_rows = read_tsv(PATHS["r26_providers"])
seeds = {r["provider_source_rpm"] for r in seed_rows}
cut_rows = read_tsv(PATHS["r25_cut_edges"])
cut5 = [r for r in cut_rows if r["cut_id"] == "5"]

if len(packages) != 483 or len(pkg) != 483:
    raise SystemExit("package denominator mismatch")
if len(edges) != 1234 or len(edge_by_key) != 1234:
    raise SystemExit("full edge denominator mismatch")
if len(r26_rows) != 1191 or len(r26_by_key) != 1191:
    raise SystemExit("R26 crossing-edge denominator mismatch")
if len(seeds) != 75:
    raise SystemExit("seed denominator mismatch")
if len(cut5) != 12:
    raise SystemExit("cut5 edge denominator mismatch")

seed_table = []
for r in sorted(seed_rows, key=lambda x: x["provider_source_rpm"]):
    seed_table.append({
        "provider_source_rpm": r["provider_source_rpm"],
        "provider_package_name": r["provider_package_name"],
        "stage": pkg[r["provider_source_rpm"]]["stage"],
        "binary_package_count": pkg[r["provider_source_rpm"]]["binary_package_count"],
        "R26_cross_boundary_edge_count": r["cross_boundary_edge_count"],
        "R26_CXX_SEMANTICS_edges": r["CXX_SEMANTICS_edges"],
        "R26_C_ONLY_edges": r["C_ONLY_edges"],
        "R26_UNDETERMINED_edges": r["UNDETERMINED_edges"],
        "R26_provider_class": r["provider_class"],
    })
write_tsv(TABLES / "seed_providers_75.tsv", list(seed_table[0]), seed_table)

# Apply exactly the affirmative R26 rules to the 43 full-graph edges that never cross a cut.
all_symbols = sorted({s for e in edges for s in e["cpp_symbols"].split(";") if s})
proc = subprocess.run(["c++filt"], input="\n".join(all_symbols) + "\n", text=True, capture_output=True, check=True)
demangled = dict(zip(all_symbols, proc.stdout.splitlines()))
exception_words = ("exception", "throw", "catch", "terminate", "unexpected", "bad_alloc", "bad_cast")

def constructor_or_destructor(text):
    before = text.split("(", 1)[0].strip()
    parts = before.split("::")
    if len(parts) < 2:
        return False
    owner = parts[-2].split("<", 1)[0].lstrip("~")
    leaf = parts[-1].split("<", 1)[0].lstrip("~")
    return owner == leaf

def symbol_reason(raw, dm):
    low = dm.lower()
    if raw.startswith(("_ZTI", "_ZTS")) or dm.startswith(("typeinfo for ", "typeinfo name for ")):
        return "RTTI_TYPEINFO"
    if raw.startswith(("_ZTV", "_ZTT")) or dm.startswith(("vtable for ", "VTT for ")):
        return "VTABLE_OR_VTT"
    if raw.startswith(("_ZTh", "_ZTv", "_ZTc")) or "thunk to " in dm:
        return "THUNK"
    if "std::" in dm:
        return "STD_NAMESPACE_TYPE_OR_OBJECT"
    if "[abi:cxx11]" in dm:
        return "CXX11_ABI_TAGGED_SYMBOL"
    if any(word in low for word in exception_words):
        return "EXCEPTION_OR_TERMINATION_SYMBOL"
    if raw.startswith(("_ZNK", "_ZNV", "_ZNr")):
        return "CV_OR_REF_QUALIFIED_MEMBER_FUNCTION_IMPLICIT_CLASS_OBJECT"
    if constructor_or_destructor(dm):
        return "CUSTOM_CLASS_CONSTRUCTOR_OR_DESTRUCTOR"
    args = dm.split("(", 1)[1] if "(" in dm else ""
    if re.search(r"(?:[A-Za-z_]\w*::)+[A-Za-z_]", args):
        return "CUSTOM_NAMESPACED_CLASS_TYPE_IN_PARAMETERS"
    return ""

header_confirmed = {
    ("emulator-kernel-6.18.2.tizen-0.src.rpm", "llvm-22.1.8-19.1.src.rpm"),
    ("nnstreamer-2.7.0-0.src.rpm", "flatbuffers-24.3.25-1.src.rpm"),
    ("pepper-2.0.19-0.src.rpm", "gtest-1.15.2-0.src.rpm"),
}

full_class = {}
full_rows = []
for key in sorted(edge_by_key):
    edge = edge_by_key[key]
    symbols = [s for s in edge["cpp_symbols"].split(";") if s]
    dms = [demangled[s] for s in symbols]
    if key in r26_by_key:
        semantic_class = r26_by_key[key]["semantic_class"]
        reason = r26_by_key[key]["classification_reason"]
        evidence_examples = r26_by_key[key]["evidence_examples"]
        source = "R26_FROZEN_1191"
    else:
        rust = [(s, d) for s, d in zip(symbols, dms) if "$u" in s or re.search(r"::h[0-9a-f]{16}$", d)]
        matched = [(s, d, symbol_reason(s, d)) for s, d in zip(symbols, dms) if symbol_reason(s, d)]
        if rust and len(rust) == len(symbols):
            semantic_class = "UNDETERMINED"
            reason = "RUST_LEGACY_MANGLE_PREFIX_COLLIDES_WITH_ITANIUM_FILTER"
            selected = [(s, d, reason) for s, d in rust]
        elif matched:
            semantic_class = "CXX_SEMANTICS"
            reason = "SYMBOL_LEVEL_CXX_SEMANTICS_CONFIRMED"
            selected = matched
        elif key in header_confirmed:
            semantic_class = "CXX_SEMANTICS"
            reason = "PUBLIC_HEADER_CONFIRMATION_REUSED_FROM_R26"
            selected = [(s, d, reason) for s, d in zip(symbols, dms)]
        elif not symbols:
            semantic_class = "C_ONLY"
            reason = "ALL_REFERENCED_SYMBOLS_UNMANGLED_C"
            selected = []
        else:
            semantic_class = "UNDETERMINED"
            reason = "NO_AFFIRMATIVE_R26_RULE_MATCH"
            selected = [(s, d, reason) for s, d in zip(symbols, dms)]
        evidence_examples = ";".join(f"{s} => {d} [{why}]" for s, d, why in selected[:5])
        source = "R27_R26_RULES_ON_NONCROSSING_EDGE"
    full_class[key] = semantic_class
    full_rows.append({
        "consumer_source_rpm": key[0],
        "consumer_package_name": pkg[key[0]]["package_name"],
        "provider_source_rpm": key[1],
        "provider_package_name": pkg[key[1]]["package_name"],
        "semantic_class": semantic_class,
        "classification_reason": reason,
        "cpp_symbol_count": edge["cpp_symbol_count"],
        "classification_source": source,
        "evidence_examples": evidence_examples,
    })
write_tsv(TABLES / "full_graph_semantic_classification.tsv", list(full_rows[0]), full_rows)

outgoing = defaultdict(list)
for e in edges:
    outgoing[e["consumer_source_rpm"]].append(e["provider_source_rpm"])

def closure(seed, mode):
    current = set(seed)
    introduced = {n: 0 for n in seed}
    rounds = [(0, sorted(seed))]
    while True:
        new = set()
        for consumer in current:
            for provider in outgoing.get(consumer, []):
                key = (consumer, provider)
                if mode == "CONSERVATIVE" or full_class[key] == "CXX_SEMANTICS":
                    if provider not in current:
                        new.add(provider)
        if not new:
            return current, rounds, introduced
        round_id = len(rounds)
        for n in new:
            introduced[n] = round_id
        current.update(new)
        rounds.append((round_id, sorted(new)))

conservative, conservative_rounds, cons_intro = closure(seeds, "CONSERVATIVE")
semantic, semantic_rounds, sem_intro = closure(seeds, "SEMANTIC")

def unit(source):
    stage = pkg[source]["stage"]
    if stage == "PHASE2":
        return f"PHASE2_LAYER{int(topology[source]['provider_first_layer']) + 1}"
    return stage

iteration_rows = []
for mode, rounds in (("CONSERVATIVE", conservative_rounds), ("SEMANTIC", semantic_rounds)):
    cumulative = 0
    for round_id, added in rounds:
        cumulative += len(added)
        iteration_rows.append({
            "closure_kind": mode,
            "round": round_id,
            "added_count": len(added),
            "cumulative_count": cumulative,
            "source_rpm_denominator": 483,
            "added_source_rpms": ";".join(added),
        })
write_tsv(TABLES / "closure_iterations.tsv", list(iteration_rows[0]), iteration_rows)

member_rows = []
for source in sorted(conservative | semantic):
    member_rows.append({
        "source_rpm": source,
        "package_name": pkg[source]["package_name"],
        "stage": pkg[source]["stage"],
        "stage_layer": unit(source),
        "binary_package_count": pkg[source]["binary_package_count"],
        "seed_75": "YES" if source in seeds else "NO",
        "conservative_closure": "YES" if source in conservative else "NO",
        "conservative_introduced_round": cons_intro.get(source, "-"),
        "semantic_closure": "YES" if source in semantic else "NO",
        "semantic_introduced_round": sem_intro.get(source, "-"),
    })
write_tsv(TABLES / "closure_members.tsv", list(member_rows[0]), member_rows)

summary_rows = []
for mode, members in (("SEED", seeds), ("CONSERVATIVE", conservative), ("SEMANTIC", semantic)):
    summary_rows.append({
        "set": mode,
        "source_rpm_count": len(members),
        "source_rpm_denominator_all": 483,
        "source_rpm_denominator_platform_migrated": 479,
        "delta_from_seed": len(members) - len(seeds),
        "binary_package_count_sum": sum(int(pkg[n]["binary_package_count"]) for n in members),
        "covers_all_483": "YES" if len(members) == 483 else "NO",
        "covers_all_479_platform_sources": "YES" if len(members & {n for n in pkg if pkg[n]['stage'] != 'VENDOR_PREBUILT_SEPARATE'}) == 479 else "NO",
    })
write_tsv(TABLES / "closure_summary.tsv", list(summary_rows[0]), summary_rows)

stage_rows = []
for mode, members in (("SEED", seeds), ("CONSERVATIVE", conservative), ("SEMANTIC", semantic)):
    for stage_layer in ("PHASE1", "PHASE2_LAYER1", "PHASE2_LAYER2", "PHASE2_LAYER3", "PHASE3", "VENDOR_PREBUILT_SEPARATE"):
        selected = {n for n in members if unit(n) == stage_layer}
        stage_rows.append({
            "set": mode,
            "stage_layer": stage_layer,
            "source_rpm_count": len(selected),
            "set_source_rpm_denominator": len(members),
            "binary_package_count_sum": sum(int(pkg[n]["binary_package_count"]) for n in selected),
        })
write_tsv(TABLES / "closure_stage_distribution.tsv", list(stage_rows[0]), stage_rows)

propagation_rows = []
for key, edge in sorted(edge_by_key.items()):
    consumer, provider = key
    if consumer not in conservative:
        continue
    propagation_rows.append({
        "consumer_source_rpm": consumer,
        "provider_source_rpm": provider,
        "edge_direction": "consumer_old_requires_provider_old",
        "semantic_class": full_class[key],
        "cpp_symbol_count": edge["cpp_symbol_count"],
        "consumer_seed": "YES" if consumer in seeds else "NO",
        "provider_seed": "YES" if provider in seeds else "NO",
        "provider_added_by_conservative": "YES" if provider in conservative - seeds else "NO",
        "provider_added_by_semantic": "YES" if provider in semantic - seeds else "NO",
        "conservative_propagates": "YES",
        "semantic_propagates": "YES" if full_class[key] == "CXX_SEMANTICS" else "NO",
    })
write_tsv(TABLES / "closure_propagation_edges.tsv", list(propagation_rows[0]), propagation_rows)

excluded_rows = []
for r in full_rows:
    if r["semantic_class"] == "CXX_SEMANTICS":
        continue
    key = (r["consumer_source_rpm"], r["provider_source_rpm"])
    excluded_rows.append({
        "consumer_source_rpm": key[0],
        "provider_source_rpm": key[1],
        "semantic_class": r["semantic_class"],
        "classification_reason": r["classification_reason"],
        "consumer_reached_by_conservative_closure": "YES" if key[0] in conservative else "NO",
        "would_add_new_member_if_propagated": "YES" if key[0] in conservative and key[1] not in semantic else "NO",
    })
write_tsv(TABLES / "closure_semantic_excluded_edges.tsv", list(excluded_rows[0]), excluded_rows)

comparison_rows = [
    {
        "closure_kind": "CONSERVATIVE",
        "eligible_graph_edge_count": len(edges),
        "full_graph_edge_denominator": len(edges),
        "eligible_semantic_classes": "CXX_SEMANTICS;UNDETERMINED",
        "reached_consumer_outgoing_edge_count": len(propagation_rows),
        "excluded_edge_count_vs_conservative": 0,
        "final_source_rpm_count": len(conservative),
        "member_difference_vs_conservative": 0,
    },
    {
        "closure_kind": "SEMANTIC",
        "eligible_graph_edge_count": sum(v == "CXX_SEMANTICS" for v in full_class.values()),
        "full_graph_edge_denominator": len(edges),
        "eligible_semantic_classes": "CXX_SEMANTICS",
        "reached_consumer_outgoing_edge_count": sum(r["semantic_class"] == "CXX_SEMANTICS" for r in propagation_rows),
        "excluded_edge_count_vs_conservative": sum(v != "CXX_SEMANTICS" for v in full_class.values()),
        "final_source_rpm_count": len(semantic),
        "member_difference_vs_conservative": len(conservative - semantic),
    },
]
write_tsv(TABLES / "closure_mode_comparison.tsv", list(comparison_rows[0]), comparison_rows)

# Cut 5: permanent vendor -> platform edges.
permanent_edge_rows = []
for r in sorted(cut5, key=lambda x: (x["consumer_source_rpm"], x["provider_source_rpm"])):
    key = (r["consumer_source_rpm"], r["provider_source_rpm"])
    edge = edge_by_key[key]
    semantic_edge = r26_by_key[key]
    permanent_edge_rows.append({
        "consumer_source_rpm": key[0],
        "consumer_package_name": pkg[key[0]]["package_name"],
        "consumer_stage": pkg[key[0]]["stage"],
        "consumer_is_vendor_source": "YES" if pkg[key[0]]["stage"] == "VENDOR_PREBUILT_SEPARATE" else "NO",
        "provider_source_rpm": key[1],
        "provider_package_name": pkg[key[1]]["package_name"],
        "provider_stage": pkg[key[1]]["stage"],
        "semantic_class": semantic_edge["semantic_class"],
        "classification_reason": semantic_edge["classification_reason"],
        "cpp_symbol_count": edge["cpp_symbol_count"],
        "cpp_symbols": edge["cpp_symbols"],
        "semantic_evidence_examples": semantic_edge["evidence_examples"],
    })
write_tsv(TABLES / "permanent_edges_12.tsv", list(permanent_edge_rows[0]), permanent_edge_rows)

permanent_seeds = {r["provider_source_rpm"] for r in cut5}
permanent_cons, permanent_cons_rounds, permanent_cons_intro = closure(permanent_seeds, "CONSERVATIVE")
permanent_sem, permanent_sem_rounds, permanent_sem_intro = closure(permanent_seeds, "SEMANTIC")
permanent_iterations = []
for mode, rounds in (("CONSERVATIVE", permanent_cons_rounds), ("SEMANTIC", permanent_sem_rounds)):
    cumulative = 0
    for round_id, added in rounds:
        cumulative += len(added)
        permanent_iterations.append({
            "closure_kind": mode,
            "round": round_id,
            "added_count": len(added),
            "cumulative_count": cumulative,
            "added_source_rpms": ";".join(added),
        })
write_tsv(TABLES / "permanent_closure_iterations.tsv", list(permanent_iterations[0]), permanent_iterations)

permanent_members = []
for source in sorted(permanent_cons | permanent_sem):
    consumers = sorted({r["consumer_source_rpm"] for r in cut5 if r["provider_source_rpm"] == source})
    permanent_members.append({
        "source_rpm": source,
        "package_name": pkg[source]["package_name"],
        "stage": pkg[source]["stage"],
        "stage_layer": unit(source),
        "binary_package_count": pkg[source]["binary_package_count"],
        "direct_provider_of_vendor_edge": "YES" if source in permanent_seeds else "NO",
        "vendor_edge_count": sum(r["provider_source_rpm"] == source for r in cut5),
        "vendor_consumer_source_rpms": ";".join(consumers),
        "in_main_conservative_closure": "YES" if source in conservative else "NO",
        "in_main_semantic_closure": "YES" if source in semantic else "NO",
        "in_permanent_conservative_closure": "YES" if source in permanent_cons else "NO",
        "in_permanent_semantic_closure": "YES" if source in permanent_sem else "NO",
    })
write_tsv(TABLES / "permanent_closure_members.tsv", list(permanent_members[0]), permanent_members)

permanent_propagation = []
for key, edge in sorted(edge_by_key.items()):
    if key[0] not in permanent_cons:
        continue
    permanent_propagation.append({
        "consumer_source_rpm": key[0],
        "provider_source_rpm": key[1],
        "semantic_class": full_class[key],
        "cpp_symbol_count": edge["cpp_symbol_count"],
        "provider_already_in_direct_permanent_set": "YES" if key[1] in permanent_seeds else "NO",
        "provider_added_recursively": "YES" if key[1] in permanent_cons - permanent_seeds else "NO",
    })
write_tsv(TABLES / "permanent_closure_propagation_edges.tsv", list(permanent_propagation[0]), permanent_propagation)

vendor_sources = {n for n in pkg if pkg[n]["stage"] == "VENDOR_PREBUILT_SEPARATE"}
vendor_names = read_tsv(PATHS["r25_vendor_accounting"])
endpoint_rows = [
    {"account": "CURRENT_E_B_PLATFORM_OWNED_OLD_BUILD_EXCEPTIONS", "source_rpm_count": 0, "binary_name_count": "NOT_APPLICABLE", "denominator": 479, "fact": "current definition states none"},
    {"account": "OBSERVED_PERMANENT_PLATFORM_PROVIDER_CLOSURE_CONSERVATIVE", "source_rpm_count": len(permanent_cons), "binary_name_count": sum(int(pkg[n]["binary_package_count"]) for n in permanent_cons), "denominator": 479, "fact": "cut5 provider closure"},
    {"account": "OBSERVED_PERMANENT_PLATFORM_PROVIDER_CLOSURE_SEMANTIC", "source_rpm_count": len(permanent_sem), "binary_name_count": sum(int(pkg[n]["binary_package_count"]) for n in permanent_sem), "denominator": 479, "fact": "cut5 provider closure using confirmed CXX semantics"},
    {"account": "VENDOR_PERMANENT_EXCEPTIONS", "source_rpm_count": len(vendor_sources), "binary_name_count": len(vendor_names), "denominator": 483, "fact": "5 binary names map to 4 source RPM identities"},
    {"account": "COMBINED_PERMANENT_SOURCE_IDENTITIES_CONSERVATIVE", "source_rpm_count": len(permanent_cons | vendor_sources), "binary_name_count": "NOT_COMBINABLE_DIFFERENT_UNIT", "denominator": 483, "fact": "platform provider source identities plus vendor source identities"},
]
write_tsv(TABLES / "endpoint_accounting.tsv", list(endpoint_rows[0]), endpoint_rows)

method_rows = [
    {"item": "graph_edge_direction", "rule": "consumer P -> provider Q", "evidence": "R22 source edge: P has UND intersecting Q DEF", "limitation": "dynamic ELF graph only"},
    {"item": "closure_propagation_direction", "rule": "if P is dual-old and P -> Q propagates, add Q", "evidence": "P_old consumes Q old ABI", "limitation": "does not include static/header-only/dlopen blind spots"},
    {"item": "conservative_closure", "rule": "all 1,234 TRUE_CPP_ABI_COUPLING edges propagate", "evidence": "progress/R22/tables/source_edge_classification.tsv", "limitation": "includes 13 semantically UNDETERMINED edges"},
    {"item": "semantic_closure", "rule": "only CXX_SEMANTICS edges propagate", "evidence": "R26 rules; 1,191 frozen classes plus same rules on 43 noncrossing edges", "limitation": "UNDETERMINED does not propagate; blind spots remain"},
    {"item": "seed", "rule": "75 distinct providers from R26 crossing edges", "evidence": "progress/R26/tables/provider_semantic_summary.tsv", "limitation": "source RPM identity unit"},
    {"item": "permanent_seed", "rule": "6 distinct providers on cut 5's 12 vendor->platform edges", "evidence": "progress/R25/tables/cut_cross_boundary_edges.tsv", "limitation": "published-runtime snapshot only"},
]
write_tsv(TABLES / "method_and_scope.tsv", list(method_rows[0]), method_rows)

input_rows = []
for role, path in PATHS.items():
    input_rows.append({"role": role, "path": str(path.relative_to(ROOT)), "sha256": sha256(path), "size_bytes": path.stat().st_size})
write_tsv(INPUTS / "input_identity.tsv", list(input_rows[0]), input_rows)

snapshot_rows = read_tsv(PATHS["r11_snapshot"])
data_rows = []
for r in snapshot_rows:
    data_rows.append({
        "repo_id": r.get("repo_id", ""),
        "snapshot_revision": r.get("repomd_revision", r.get("revision", r.get("snapshot_revision", ""))),
        "repomd_sha256": r.get("repomd_sha256", ""),
        "object_form": "published repository RPM/ELF metadata reused from R11",
        "measurement_scope": "R22/R24/R25/R26 derived source-level C++ ABI graph; no rescan",
    })
write_tsv(TABLES / "data_sources.tsv", list(data_rows[0]), data_rows)

full_counts = Counter(full_class.values())
checks = [
    ("package_rows_483", len(packages) == 483, len(packages)),
    ("full_true_edges_1234", len(edges) == 1234, len(edges)),
    ("crossing_seed_providers_75", len(seeds) == 75, len(seeds)),
    ("full_semantic_partition_1221_0_13", full_counts == Counter({"CXX_SEMANTICS": 1221, "UNDETERMINED": 13}), str(full_counts)),
    ("noncrossing_edges_43_all_CXX", len(set(edge_by_key) - set(r26_by_key)) == 43 and all(full_class[k] == "CXX_SEMANTICS" for k in set(edge_by_key) - set(r26_by_key)), len(set(edge_by_key) - set(r26_by_key))),
    ("conservative_closure_76", len(conservative) == 76, len(conservative)),
    ("semantic_closure_76", len(semantic) == 76, len(semantic)),
    ("closure_added_only_libcynara_commons", conservative - seeds == {"libcynara-commons-0.26.0-1.src.rpm"}, sorted(conservative - seeds)),
    ("closure_binary_count_407", sum(int(pkg[n]["binary_package_count"]) for n in conservative) == 407, sum(int(pkg[n]["binary_package_count"]) for n in conservative)),
    ("cut5_edges_12", len(cut5) == 12, len(cut5)),
    ("cut5_consumers_vendor_4", len({r["consumer_source_rpm"] for r in cut5}) == 4 and all(pkg[r["consumer_source_rpm"]]["stage"] == "VENDOR_PREBUILT_SEPARATE" for r in cut5), sorted({r["consumer_source_rpm"] for r in cut5})),
    ("cut5_providers_6", len(permanent_seeds) == 6, len(permanent_seeds)),
    ("cut5_all_semantic", all(full_class[(r["consumer_source_rpm"], r["provider_source_rpm"])] == "CXX_SEMANTICS" for r in cut5), Counter(full_class[(r["consumer_source_rpm"], r["provider_source_rpm"])] for r in cut5)),
    ("permanent_closures_6", len(permanent_cons) == 6 and len(permanent_sem) == 6, (len(permanent_cons), len(permanent_sem))),
    ("all_permanent_in_main_closure", permanent_cons <= conservative and permanent_sem <= semantic, sorted(permanent_cons - conservative)),
    ("permanent_platform_binary_count_79", sum(int(pkg[n]["binary_package_count"]) for n in permanent_cons) == 79, sum(int(pkg[n]["binary_package_count"]) for n in permanent_cons)),
    ("combined_permanent_source_identities_10", len(permanent_cons | vendor_sources) == 10, len(permanent_cons | vendor_sources)),
]
validation_rows = [{"assertion": name, "observed": str(observed), "status": "PASS" if ok else "FAIL"} for name, ok, observed in checks]
write_tsv(OUT / "validation.tsv", list(validation_rows[0]), validation_rows)
if not all(ok for _, ok, _ in checks):
    raise SystemExit("validation failed")

print(f"full_edges={len(edges)} semantic={dict(full_counts)}")
print(f"seed={len(seeds)} conservative={len(conservative)} semantic={len(semantic)} added={sorted(conservative - seeds)}")
print(f"permanent_edges={len(cut5)} vendor_sources={len({r['consumer_source_rpm'] for r in cut5})} permanent_providers={len(permanent_cons)} combined_source_identities={len(permanent_cons | vendor_sources)}")
