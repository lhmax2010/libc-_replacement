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
OUT = ROOT / "progress/R26"
TABLES = OUT / "tables"
INPUTS = OUT / "inputs"
TABLES.mkdir(parents=True, exist_ok=True)
INPUTS.mkdir(parents=True, exist_ok=True)

PATHS = {
    "r25_crossing_edges": ROOT / "progress/R25/tables/cross_boundary_edge_properties.tsv",
    "r25_cut_edges": ROOT / "progress/R25/tables/cut_cross_boundary_edges.tsv",
    "r25_cut_summary": ROOT / "progress/R25/tables/cut_summary.tsv",
    "r24_packages": ROOT / "progress/R24/tables/migration_packages_483.tsv",
    "r22_source_edges": ROOT / "progress/R22/tables/source_edge_classification.tsv",
    "r22_candidate_evidence": ROOT / "progress/R22/tables/candidate_edge_evidence.tsv",
    "r22_classifier": ROOT / "progress/R22/tools/classify_abi_edges.py",
    "r11_snapshot": ROOT / "progress/R11/tables/snapshot_identity.tsv",
    "r11_binary_records": ROOT / "progress/R11/tables/binary_package_records.tsv",
    "bundle_devel": ROOT / "tmp/R11/rpms/unified_binary/x86_64/bundle-devel-0.18.15-0.x86_64.rpm",
    "flatbuffers_devel": ROOT / "tmp/R11/rpms/unified_binary/x86_64/flatbuffers-devel-24.3.25-1.x86_64.rpm",
    "gtest_devel": ROOT / "tmp/R11/rpms/unified_binary/x86_64/gtest-devel-1.15.2-0.x86_64.rpm",
    "llvm_devel": ROOT / "tmp/R11/rpms/base_binary/x86_64/llvm-devel-22.1.8-19.1.x86_64.rpm",
}

def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        w.writeheader()
        w.writerows(rows)

def sha256(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()

crossing = read_tsv(PATHS["r25_crossing_edges"])
cut_edges = read_tsv(PATHS["r25_cut_edges"])
packages = read_tsv(PATHS["r24_packages"])
source_edges_all = read_tsv(PATHS["r22_source_edges"])
pkg = {r["source_rpm_identity"]: r for r in packages}
source_edges = {(r["consumer_source_rpm"], r["provider_source_rpm"]): r for r in source_edges_all}
keys = [(r["consumer_source_rpm"], r["provider_source_rpm"]) for r in crossing]
if len(crossing) != 1191 or len(set(keys)) != 1191:
    raise SystemExit("R25 crossing-edge denominator mismatch")
if any(source_edges[k]["classification"] != "TRUE_CPP_ABI_COUPLING" for k in keys):
    raise SystemExit("crossing edge is not frozen TRUE_CPP_ABI_COUPLING")

all_symbols = sorted({s for k in keys for s in source_edges[k]["cpp_symbols"].split(";") if s})
demangle_proc = subprocess.run(["c++filt"], input="\n".join(all_symbols) + "\n", text=True, capture_output=True, check=True)
demangled = dict(zip(all_symbols, demangle_proc.stdout.splitlines()))

header_confirmed = {
    ("emulator-kernel-6.18.2.tizen-0.src.rpm", "llvm-22.1.8-19.1.src.rpm"):
        ("PUBLIC_HEADER_CONFIRMS_CLASS_METHOD", "llvm/Support/raw_ostream.h declares class raw_ostream and raw_ostream::write"),
    ("nnstreamer-2.7.0-0.src.rpm", "flatbuffers-24.3.25-1.src.rpm"):
        ("PUBLIC_HEADER_CONFIRMS_CLASS_STATIC_OBJECT", "flatbuffers/util.h declares class ClassicLocale and static ClassicLocale instance_"),
    ("pepper-2.0.19-0.src.rpm", "gtest-1.15.2-0.src.rpm"):
        ("PUBLIC_HEADER_CONFIRMS_CLASS_AND_ABI_TAGGED_OBJECT", "gtest headers declare class UnitTest; referenced flag is [abi:cxx11]"),
}

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

classification_rows = []
evidence_rows = []
class_by_key = {}
for key in sorted(keys):
    edge = source_edges[key]
    symbols = [s for s in edge["cpp_symbols"].split(";") if s]
    dms = [demangled[s] for s in symbols]
    rust = [(s, d) for s, d in zip(symbols, dms) if "$u" in s or re.search(r"::h[0-9a-f]{16}$", d)]
    matched = [(s, d, symbol_reason(s, d)) for s, d in zip(symbols, dms) if symbol_reason(s, d)]
    header = header_confirmed.get(key)
    if rust and len(rust) == len(symbols):
        cls = "UNDETERMINED"
        reason = "RUST_LEGACY_MANGLE_PREFIX_COLLIDES_WITH_ITANIUM_FILTER"
        selected = [(s, d, reason) for s, d in rust]
    elif matched:
        cls = "CXX_SEMANTICS"
        reason = "SYMBOL_LEVEL_CXX_SEMANTICS_CONFIRMED"
        selected = matched
    elif header:
        cls = "CXX_SEMANTICS"
        reason = header[0]
        selected = [(s, d, header[0]) for s, d in zip(symbols, dms)]
    else:
        cls = "UNDETERMINED"
        reason = "MANGLED_DATA_OR_SCOPE_SYMBOL_TYPE_NOT_ENCODED_AND_PUBLIC_DECLARATION_NOT_FOUND"
        selected = [(s, d, reason) for s, d in zip(symbols, dms)]
    # A C_ONLY edge would require zero Itanium/Rust-style _Z symbols. The frozen graph guarantees >=1.
    if not symbols:
        cls = "C_ONLY"
        reason = "ALL_REFERENCED_SYMBOLS_UNMANGLED_C"
        selected = []
    class_by_key[key] = cls
    digest = hashlib.sha256("\n".join(sorted(symbols)).encode()).hexdigest()
    classification_rows.append({
        "consumer_source_rpm": key[0],
        "consumer_package_name": pkg[key[0]]["package_name"],
        "provider_source_rpm": key[1],
        "provider_package_name": pkg[key[1]]["package_name"],
        "semantic_class": cls,
        "classification_reason": reason,
        "referenced_mangled_symbol_count": len(symbols),
        "referenced_unmangled_c_symbol_count": int(edge["c_symbol_count"]),
        "semantic_evidence_symbol_count": len(selected),
        "all_mangled_symbols_sha256": digest,
        "crossed_cut_ids": next(r["crossed_cut_ids"] for r in crossing if (r["consumer_source_rpm"], r["provider_source_rpm"]) == key),
        "evidence_examples": ";".join(f"{s} => {d} [{why}]" for s, d, why in selected[:5]),
        "header_review": header[1] if header else ("NOT_FOUND" if key == ("capi-system-media-key-0.2.13-6.src.rpm", "bundle-0.18.15-0.src.rpm") else "NOT_REQUIRED_OR_NOT_APPLICABLE"),
    })
    for s, d, why in selected:
        evidence_rows.append({
            "consumer_source_rpm": key[0],
            "provider_source_rpm": key[1],
            "semantic_class": cls,
            "raw_symbol": s,
            "demangled_symbol": d,
            "evidence_rule": why,
        })

fields = list(classification_rows[0])
write_tsv(TABLES / "edge_semantic_classification.tsv", fields, classification_rows)
write_tsv(TABLES / "semantic_evidence_symbols.tsv", list(evidence_rows[0]), evidence_rows)

summary = []
for cls in ("CXX_SEMANTICS", "C_ONLY", "UNDETERMINED"):
    es = [r for r in classification_rows if r["semantic_class"] == cls]
    summary.append({
        "semantic_class": cls,
        "edge_count": len(es),
        "edge_denominator": 1191,
        "distinct_provider_count": len({r["provider_source_rpm"] for r in es}),
        "provider_denominator": 75,
        "distinct_consumer_count": len({r["consumer_source_rpm"] for r in es}),
        "consumer_union_denominator": len({r["consumer_source_rpm"] for r in classification_rows}),
    })
write_tsv(TABLES / "semantic_class_summary.tsv", list(summary[0]), summary)
reason_counts = Counter((r["semantic_class"], r["classification_reason"]) for r in classification_rows)
reason_rows = [
    {"semantic_class": cls, "classification_reason": reason, "edge_count": count, "edge_denominator": 1191}
    for (cls, reason), count in sorted(reason_counts.items())
]
write_tsv(TABLES / "edge_reason_summary.tsv", list(reason_rows[0]), reason_rows)

cut_distribution = []
cut_by_id = defaultdict(list)
for r in cut_edges:
    cut_by_id[int(r["cut_id"])].append((r["consumer_source_rpm"], r["provider_source_rpm"]))
for cut in range(1, 6):
    cc = Counter(class_by_key[k] for k in cut_by_id[cut])
    cut_distribution.append({
        "cut_id": cut,
        "cut_name": cut_by_id[cut] and next(r["cut_name"] for r in cut_edges if int(r["cut_id"]) == cut) or "",
        "edge_denominator_at_cut": len(cut_by_id[cut]),
        "CXX_SEMANTICS_edges": cc["CXX_SEMANTICS"],
        "C_ONLY_edges": cc["C_ONLY"],
        "UNDETERMINED_edges": cc["UNDETERMINED"],
        "CXX_provider_count": len({k[1] for k in cut_by_id[cut] if class_by_key[k] == "CXX_SEMANTICS"}),
        "C_ONLY_provider_count": len({k[1] for k in cut_by_id[cut] if class_by_key[k] == "C_ONLY"}),
        "UNDETERMINED_provider_count": len({k[1] for k in cut_by_id[cut] if class_by_key[k] == "UNDETERMINED"}),
    })
write_tsv(TABLES / "cut_semantic_distribution.tsv", list(cut_distribution[0]), cut_distribution)

by_provider = defaultdict(list)
for r in classification_rows:
    by_provider[r["provider_source_rpm"]].append(r)
provider_rows = []
for provider, es in sorted(by_provider.items()):
    cc = Counter(r["semantic_class"] for r in es)
    if cc["CXX_SEMANTICS"]:
        pclass = "HAS_CXX_SEMANTICS"
    elif cc["UNDETERMINED"]:
        pclass = "UNDETERMINED_WITHOUT_CXX"
    else:
        pclass = "ALL_C_ONLY"
    provider_rows.append({
        "provider_source_rpm": provider,
        "provider_package_name": pkg[provider]["package_name"],
        "cross_boundary_edge_count": len(es),
        "cross_boundary_edge_denominator": 1191,
        "CXX_SEMANTICS_edges": cc["CXX_SEMANTICS"],
        "C_ONLY_edges": cc["C_ONLY"],
        "UNDETERMINED_edges": cc["UNDETERMINED"],
        "distinct_consumer_count": len({r["consumer_source_rpm"] for r in es}),
        "provider_class": pclass,
        "crossed_cut_ids": ";".join(map(str, sorted({int(x) for r in es for x in r["crossed_cut_ids"].split(";") if x}))),
    })
write_tsv(TABLES / "provider_semantic_summary.tsv", list(provider_rows[0]), provider_rows)
top20 = sorted(provider_rows, key=lambda r: (-int(r["cross_boundary_edge_count"]), r["provider_source_rpm"]))[:20]
for i, r in enumerate(top20, 1):
    r["rank"] = i
write_tsv(TABLES / "provider_top20.tsv", ["rank"] + list(provider_rows[0]), top20)

pclass = Counter(r["provider_class"] for r in provider_rows)
provider_class_rows = [
    {"provider_class": k, "provider_count": pclass[k], "provider_denominator": 75}
    for k in ("HAS_CXX_SEMANTICS", "ALL_C_ONLY", "UNDETERMINED_WITHOUT_CXX")
]
write_tsv(TABLES / "provider_class_summary.tsv", list(provider_class_rows[0]), provider_class_rows)

# C_ONLY population is mechanically empty because every frozen edge has >=1 _Z-prefixed match.
c_only_review = [{
    "population": "C_ONLY_EDGES",
    "population_edge_count": sum(r["semantic_class"] == "C_ONLY" for r in classification_rows),
    "population_edge_denominator": 1191,
    "sampling_method": "CENSUS_IF_NONEMPTY; population is empty",
    "sample_size": 0,
    "review_result": "NOT_APPLICABLE_EMPTY_POPULATION",
    "public_header_review": "No C_ONLY edge exists to sample; no edge was silently skipped",
    "corrections": 0,
}]
write_tsv(TABLES / "c_only_sample_review.tsv", list(c_only_review[0]), c_only_review)

header_rows = [
    {"consumer_source_rpm":"emulator-kernel-6.18.2.tizen-0.src.rpm","provider_source_rpm":"llvm-22.1.8-19.1.src.rpm","rpm_input":"tmp/R11/rpms/base_binary/x86_64/llvm-devel-22.1.8-19.1.x86_64.rpm","header":"/usr/include/llvm/Support/raw_ostream.h","symbol":"llvm::raw_ostream::write","observation":"class raw_ostream plus member declaration found","classification_effect":"CXX_SEMANTICS"},
    {"consumer_source_rpm":"nnstreamer-2.7.0-0.src.rpm","provider_source_rpm":"flatbuffers-24.3.25-1.src.rpm","rpm_input":"tmp/R11/rpms/unified_binary/x86_64/flatbuffers-devel-24.3.25-1.x86_64.rpm","header":"/usr/include/flatbuffers/util.h","symbol":"flatbuffers::ClassicLocale::instance_","observation":"class ClassicLocale and static ClassicLocale instance_ found","classification_effect":"CXX_SEMANTICS"},
    {"consumer_source_rpm":"pepper-2.0.19-0.src.rpm","provider_source_rpm":"gtest-1.15.2-0.src.rpm","rpm_input":"tmp/R11/rpms/unified_binary/x86_64/gtest-devel-1.15.2-0.x86_64.rpm","header":"/usr/include/gtest/gtest.h; /usr/include/gmock/gmock.h","symbol":"testing::UnitTest methods; FLAGS_gtest_death_test_style[abi:cxx11]","observation":"class UnitTest and InitGoogleMock declarations found; ABI-tagged flag observed","classification_effect":"CXX_SEMANTICS"},
    {"consumer_source_rpm":"capi-system-media-key-0.2.13-6.src.rpm","provider_source_rpm":"bundle-0.18.15-0.src.rpm","rpm_input":"tmp/R11/rpms/unified_binary/x86_64/bundle-devel-0.18.15-0.x86_64.rpm","header":"/usr/include/bundle.h; /usr/include/bundle_cpp.h; /usr/include/bundle_internal.h","symbol":"tizen_base::logger","observation":"declaration NOT_FOUND in all public headers","classification_effect":"UNDETERMINED"},
]
write_tsv(TABLES / "public_header_reviews.tsv", list(header_rows[0]), header_rows)

# Name-token approximation for callback candidates among unmangled companion symbols.
callback_re = re.compile(r"(?:callback|_cb(?:_|$)|register|subscribe|handler|listener|hook|notify)", re.I)
callback_candidates = []
for r in classification_rows:
    edge = source_edges[(r["consumer_source_rpm"], r["provider_source_rpm"])]
    hits = sorted(s for s in edge["c_symbols"].split(";") if s and callback_re.search(s))
    if hits:
        callback_candidates.append({
            "consumer_source_rpm": r["consumer_source_rpm"],
            "provider_source_rpm": r["provider_source_rpm"],
            "semantic_class": r["semantic_class"],
            "name_token_candidate_count": len(hits),
            "candidate_symbols": ";".join(hits),
            "status": "NAME_TOKEN_APPROXIMATION_NOT_CALLBACK_PROOF",
        })
write_tsv(TABLES / "callback_name_candidates.tsv", ["consumer_source_rpm","provider_source_rpm","semantic_class","name_token_candidate_count","candidate_symbols","status"], callback_candidates)

blind_rows = [
    {"blind_spot":"C callback can throw across boundary","measurable_candidate_edges":len(callback_candidates),"candidate_provider_count":len({r['provider_source_rpm'] for r in callback_candidates}),"candidate_consumer_count":len({r['consumer_source_rpm'] for r in callback_candidates}),"edge_denominator":1191,"measurement":"unmangled C symbol name token approximation only","status":"UNDETERMINED; names do not prove registration or thrown exception"},
    {"blind_spot":"void* carries C++ object ownership","measurable_candidate_edges":"NOT_AVAILABLE","candidate_provider_count":"NOT_AVAILABLE","candidate_consumer_count":"NOT_AVAILABLE","edge_denominator":1191,"measurement":"parameter types absent from unmangled dynamic symbol names","status":"UNDETERMINED"},
    {"blind_spot":"header-only inline/template interface","measurable_candidate_edges":"NOT_AVAILABLE","candidate_provider_count":"NOT_AVAILABLE","candidate_consumer_count":"NOT_AVAILABLE","edge_denominator":1191,"measurement":"no cross-package dynamic symbol by definition","status":"OUTSIDE_GRAPH"},
    {"blind_spot":"non-template function return type omitted by Itanium name","measurable_candidate_edges":"NOT_AVAILABLE","candidate_provider_count":"NOT_AVAILABLE","candidate_consumer_count":"NOT_AVAILABLE","edge_denominator":1191,"measurement":"return type generally not encoded","status":"UNDETERMINED"},
    {"blind_spot":"Rust legacy _ZN prefix false-positive in R22 filter","measurable_candidate_edges":sum(r["classification_reason"]=="RUST_LEGACY_MANGLE_PREFIX_COLLIDES_WITH_ITANIUM_FILTER" for r in classification_rows),"candidate_provider_count":1,"candidate_consumer_count":12,"edge_denominator":1191,"measurement":"c++filt output with Rust hash suffix and $u escapes","status":"OBSERVED_AND_CLASSIFIED_UNDETERMINED"},
]
write_tsv(TABLES / "blind_spots.tsv", list(blind_rows[0]), blind_rows)

method_rows = [
    {"item":"original_dynamic_symbol_extraction","command_or_rule":"readelf --dyn-syms -W <ELF>","evidence":"progress/R22/tools/classify_abi_edges.py","limitation":"reused; not rerun"},
    {"item":"version_normalization","command_or_rule":"split symbol name at first @","evidence":"progress/R22/tools/classify_abi_edges.py","limitation":"weak UND not filtered"},
    {"item":"demangle","command_or_rule":"c++filt < newline-separated frozen symbol list","evidence":"progress/R26/tools/analyze_r26.py","limitation":"Rust legacy mangling also decoded"},
    {"item":"CXX_SEMANTICS","command_or_rule":"std:: / ABI tag / RTTI / vtable / thunk / exception / ctor-dtor / namespaced custom class parameter / confirmed public header","evidence":"semantic_evidence_symbols.tsv","limitation":"only affirmative rules"},
    {"item":"C_ONLY","command_or_rule":"all referenced symbols unmangled C and no _Z-style match","evidence":"edge_semantic_classification.tsv","limitation":"impossible in frozen TRUE_CPP_ABI_COUPLING population"},
    {"item":"UNDETERMINED","command_or_rule":"no affirmative semantic proof, Rust legacy collision, callback/void*/return/header-only blind spot","evidence":"edge_semantic_classification.tsv; blind_spots.tsv","limitation":"never guessed into C_ONLY"},
]
write_tsv(TABLES / "classification_method.tsv", list(method_rows[0]), method_rows)

input_rows = []
for role, path in PATHS.items():
    input_rows.append({"role":role,"path":str(path.relative_to(ROOT)),"sha256":sha256(path),"size_bytes":path.stat().st_size})
write_tsv(INPUTS / "input_identity.tsv", list(input_rows[0]), input_rows)

cls_counts = Counter(r["semantic_class"] for r in classification_rows)
assertions = [
    ("edge_total_1191", len(classification_rows) == 1191, len(classification_rows)),
    ("classification_partition", sum(cls_counts.values()) == 1191, str(cls_counts)),
    ("no_C_ONLY_due_frozen_graph", cls_counts["C_ONLY"] == 0, cls_counts["C_ONLY"]),
    ("all_edges_have_Z_style_symbol", all(int(r["referenced_mangled_symbol_count"]) >= 1 for r in classification_rows), min(int(r["referenced_mangled_symbol_count"]) for r in classification_rows)),
    ("provider_total_75", len(provider_rows) == 75, len(provider_rows)),
    ("provider_partition", sum(pclass.values()) == 75, str(pclass)),
    ("five_cut_denominators", [len(cut_by_id[i]) for i in range(1,6)] == [953,574,259,118,12], str([len(cut_by_id[i]) for i in range(1,6)])),
    ("public_header_reviews_four", len(header_rows) == 4, len(header_rows)),
    ("C_ONLY_sample_population_zero", c_only_review[0]["population_edge_count"] == 0, c_only_review[0]["population_edge_count"]),
]
write_tsv(OUT / "validation.tsv", ["assertion","observed","status"], [{"assertion":n,"observed":str(v),"status":"PASS" if ok else "FAIL"} for n,ok,v in assertions])
if not all(ok for _,ok,_ in assertions):
    raise SystemExit("validation assertion failed")

print(f"classes={dict(cls_counts)} providers={dict(pclass)} consumers={len({r['consumer_source_rpm'] for r in classification_rows})}")
print("cuts=" + ",".join(f"{r['cut_id']}:{r['CXX_SEMANTICS_edges']}/{r['C_ONLY_edges']}/{r['UNDETERMINED_edges']}" for r in cut_distribution))
print(f"evidence_symbols={len(evidence_rows)} callback_name_candidates={len(callback_candidates)}")
