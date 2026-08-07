#!/usr/bin/env python3
import csv
import hashlib
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
P = ROOT / "progress/R22"
D = ROOT / "docs/progress/R22"
checks = []

def read(path):
    with path.open(encoding="utf-8", newline="") as f: return list(csv.DictReader(f, delimiter="\t"))
def check(name, ok, observed):
    checks.append((name, "PASS" if ok else "FAIL", str(observed)))

full = read(P / "tables/migration_packages_full.tsv")
check("migration rows", len(full) == 483, len(full))
check("migration unique source", len({r["source_rpm"] for r in full}) == 483, len({r["source_rpm"] for r in full}))
check("stage counts", Counter(r["stage"] for r in full) == Counter({"PHASE1":13,"PHASE2":450,"PHASE3":16,"VENDOR_PREBUILT_SEPARATE":4}), Counter(r["stage"] for r in full))
cross = read(P / "tables/cross_checks.tsv")
check("cross checks all pass", bool(cross) and all(r["status"] == "PASS" for r in cross), f"{sum(r['status']=='PASS' for r in cross)}/{len(cross)}")
check("corrected inverse zero", len(read(P / "tables/corrected_inverse_edges.tsv")) == 0, len(read(P / "tables/corrected_inverse_edges.tsv")))
check("multi SCC zero", len(read(P / "tables/coupling_components.tsv")) == 0, len(read(P / "tables/coupling_components.tsv")))
edge = read(P / "tables/edge_attribution_summary.tsv")
obs = {(r["scope"],r["classification"]):int(r["edge_count"]) for r in edge}
expected = {
    ("ALL_R20_SOURCE_CANDIDATE_EDGES","TRUE_CPP_ABI_COUPLING"):1234,
    ("ALL_R20_SOURCE_CANDIDATE_EDGES","PURE_C_INTERFACE"):445,
    ("ALL_R20_SOURCE_CANDIDATE_EDGES","OTHER_NO_SYMBOL_INTERSECTION"):26,
    ("R20_REVERSE_462","TRUE_CPP_ABI_COUPLING"):452,
    ("R20_REVERSE_462","PURE_C_INTERFACE"):8,
    ("R20_REVERSE_462","OTHER_NO_SYMBOL_INTERSECTION"):2,
}
check("edge classifications", all(obs.get(k)==v for k,v in expected.items()), {k:obs.get(k) for k in expected})
check("reverse attribution rows", len(read(P / "tables/reverse_edge_attribution.tsv")) == 462, len(read(P / "tables/reverse_edge_attribution.tsv")))
check("old unknown rows", len(read(P / "tables/unknown_121_attribution.tsv")) == 121, len(read(P / "tables/unknown_121_attribution.tsv")))

board = read(P / "board/path_probe_results_v2.tsv")
check("board confirmed paths", sum(r["status"]=="WRITABLE_AND_EXECUTABLE_CONFIRMED" for r in board) == 6, sum(r["status"]=="WRITABLE_AND_EXECUTABLE_CONFIRMED" for r in board))
check("board missing path", sum(r["status"]=="NOT_WRITABLE_OR_NOT_FOUND" for r in board) == 1, sum(r["status"]=="NOT_WRITABLE_OR_NOT_FOUND" for r in board))
check("board cleanup remote", all(r["cleanup_remote_exit_code"]=="0" and r["deletion_verify_remote_exit_code"]=="0" for r in board), "all zero")
check("propagation rows", len(read(P / "patches/forced_unwind_propagation.tsv")) == 44, len(read(P / "patches/forced_unwind_propagation.tsv")))
idx = read(ROOT / "docs/patches/INDEX.tsv")
check("patch index", len(idx)==2 and all(r["状态"]=="待开发" for r in idx), len(idx))
for package in ["libcynara-commons","security-manager"]:
    text = (ROOT / f"docs/patches/{package}/README.md").read_text(encoding="utf-8")
    heads = ["错误现象","触发条件","错误分析","影响面","解决方案","兼容性","验证记录","提交时机"]
    check(f"README template {package}", all(f"## {h}" in text for h in heads), ",".join(h for h in heads if f"## {h}" not in text) or "complete")
    check(f"patch empty {package}", list((ROOT / f"docs/patches/{package}/patch").iterdir()) == [ROOT / f"docs/patches/{package}/patch/.gitkeep"], [p.name for p in (ROOT / f"docs/patches/{package}/patch").iterdir()])

# Verify every curated file is <= 5 MiB and every split table reconstructs byte-for-byte.
big = [(p.relative_to(D),p.stat().st_size) for p in D.rglob("*") if p.is_file() and p.stat().st_size > 5*1024*1024]
check("curated max 5MiB", not big, big)
registry = read(D / "SPLIT_REGISTRY.tsv")
by = {}
for r in registry:
    if r["mode"] == "SPLIT": by.setdefault(r["source"], []).append(D / r["curated_part"])
for source, parts in by.items():
    original = (P / source).read_bytes()
    rebuilt = bytearray()
    for i, part in enumerate(sorted(parts)):
        data = part.read_bytes()
        if i == 0: rebuilt.extend(data)
        else: rebuilt.extend(data.split(b"\n",1)[1])
    check(f"split reconstruct {source}", bytes(rebuilt)==original, f"{len(parts)} parts")

def digest(path):
    h=hashlib.sha256(); h.update(path.read_bytes()); return h.hexdigest()
manifest_ok=True; entries=0
for line in (D/"MANIFEST.sha256").read_text(encoding="utf-8").splitlines():
    expected_sha, rel = line.split("  ",1); path=D/rel; entries+=1
    if not path.is_file() or digest(path)!=expected_sha: manifest_ok=False
check("curated manifest", manifest_ok, entries)

out = P / "validation.tsv"
with out.open("w",encoding="utf-8",newline="") as f:
    w=csv.writer(f,delimiter="\t",lineterminator="\n"); w.writerow(["check","status","observed"]); w.writerows(checks)
for r in checks: print("\t".join(r))
raise SystemExit(0 if all(r[1]=="PASS" for r in checks) else 1)
