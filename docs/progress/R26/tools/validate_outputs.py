#!/usr/bin/env python3
import csv
import hashlib
import sys
from collections import Counter
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path(__file__).resolve().parents[3]
P = ROOT / "progress/R26"
D = ROOT / "docs/progress/R26"

def rows(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

checks = []
def check(name, ok, observed):
    checks.append((name, bool(ok), observed))

edge = rows(P / "tables/edge_semantic_classification.tsv")
ec = Counter(r["semantic_class"] for r in edge)
check("edge_rows_1191", len(edge) == 1191, len(edge))
check("edge_partition_1178_0_13", ec == Counter({"CXX_SEMANTICS":1178,"UNDETERMINED":13}), str(ec))
check("every_edge_has_mangled_match", all(int(r["referenced_mangled_symbol_count"]) >= 1 for r in edge), min(int(r["referenced_mangled_symbol_count"]) for r in edge))

provider = rows(P / "tables/provider_semantic_summary.tsv")
pc = Counter(r["provider_class"] for r in provider)
check("provider_rows_75", len(provider) == 75, len(provider))
check("provider_partition_74_0_1", pc == Counter({"HAS_CXX_SEMANTICS":74,"UNDETERMINED_WITHOUT_CXX":1}), str(pc))

cut = rows(P / "tables/cut_semantic_distribution.tsv")
check("cut_rows_5", len(cut) == 5, len(cut))
check("cut_partitions", [(int(r["CXX_SEMANTICS_edges"]),int(r["C_ONLY_edges"]),int(r["UNDETERMINED_edges"])) for r in cut] == [(952,0,1),(562,0,12),(259,0,0),(118,0,0),(12,0,0)], [(r["CXX_SEMANTICS_edges"],r["C_ONLY_edges"],r["UNDETERMINED_edges"]) for r in cut])

sample = rows(P / "tables/c_only_sample_review.tsv")
check("C_ONLY_empty_sample", len(sample)==1 and sample[0]["population_edge_count"]=="0" and sample[0]["sample_size"]=="0", sample)
headers = rows(P / "tables/public_header_reviews.tsv")
check("header_reviews_four", len(headers)==4, len(headers))

validation = rows(P / "validation.tsv")
check("analyzer_assertions_pass", all(r["status"]=="PASS" for r in validation), [r for r in validation if r["status"]!="PASS"])
check("progress_required", all((P/x).is_file() for x in ("REPORT.md","METHODOLOGY.md","README.md","inputs/input_identity.tsv","tables/data_sources.tsv","state.tsv","EXCLUDED.tsv")), "required set")

if D.exists() and (P / "tables/CURATION_MAP.tsv").exists():
    mapping = rows(P / "tables/CURATION_MAP.tsv")
    check("split_parts_nonempty", len(mapping) > 1, len(mapping))
    source = P / mapping[0]["source_path"]
    source_lines = source.read_bytes().splitlines(keepends=True)
    reconstructed_lines = [source_lines[0]]
    for r in mapping:
        reconstructed_lines.extend((D / r["curated_part_path"]).read_bytes().splitlines(keepends=True)[1:])
    reconstructed = b"".join(reconstructed_lines)
    check("split_reconstruct_sha", hashlib.sha256(reconstructed).hexdigest() == hashlib.sha256(source.read_bytes()).hexdigest(), hashlib.sha256(reconstructed).hexdigest())
    check("curated_files_le_5MiB", all(x.stat().st_size <= 5*1024*1024 for x in D.rglob("*") if x.is_file()), max(x.stat().st_size for x in D.rglob("*") if x.is_file()))
    mismatches=[]
    for path in P.rglob("*"):
        if not path.is_file() or path.name=="MANIFEST.sha256" or path.relative_to(P)==Path("tables/semantic_evidence_symbols.tsv"):
            continue
        other=D/path.relative_to(P)
        if not other.is_file() or hashlib.sha256(path.read_bytes()).digest()!=hashlib.sha256(other.read_bytes()).digest(): mismatches.append(str(path.relative_to(P)))
    check("curated_regular_files_match", not mismatches, mismatches)

for name, ok, observed in checks:
    print(f"{name}\t{observed}\t{'PASS' if ok else 'FAIL'}")
if not all(ok for _,ok,_ in checks):
    raise SystemExit(1)
