#!/usr/bin/env python3
import csv
import hashlib
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PROGRESS = ROOT / "progress/R24"
CURATED = ROOT / "docs/progress/R24"


def read_tsv(path):
    with path.open(encoding="utf-8", newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))


def verify_manifest(base):
    manifest = base / "MANIFEST.sha256"
    for line in manifest.read_text(encoding="utf-8").splitlines():
        digest, rel = line.split("  ", 1)
        actual = hashlib.sha256((base / rel).read_bytes()).hexdigest()
        if actual != digest:
            raise AssertionError(f"manifest mismatch: {base / rel}")


delivery = read_tsv(PROGRESS / "tables/migration_packages_483.tsv")
assert len(delivery) == 483
assert {row["stage"] for row in delivery} == {"PHASE1", "PHASE2", "PHASE3", "VENDOR_PREBUILT_SEPARATE"}
assert [sum(row["stage"] == stage for row in delivery) for stage in ["PHASE1", "PHASE2", "PHASE3", "VENDOR_PREBUILT_SEPARATE"]] == [13, 450, 16, 4]

internal = read_tsv(PROGRESS / "tables/stage_internal_coupling_summary.tsv")
assert {row["stage"]: int(row["internal_cpp_abi_edge_count"]) for row in internal} == {"PHASE1": 35, "PHASE2": 185, "PHASE3": 8}
assert all(row["accurate_property"] == "DEPENDENCY_ORDERED_NOT_INDEPENDENT_BULK" for row in internal)

scc = read_tsv(PROGRESS / "tables/nontrivial_scc.tsv")
assert len(scc) == 0
assert len(read_tsv(PROGRESS / "tables/scc_membership.tsv")) == 479
assert len(read_tsv(PROGRESS / "tables/initial_462_edge_disposition.tsv")) == 462
assert len(read_tsv(PROGRESS / "tables/deleted_candidate_edges.tsv")) == 471

core = read_tsv(PROGRESS / "tables/core_phase3_same_unit_comparison.tsv")
assert [(row["arch"], int(row["historical_core_source_rpm_count"])) for row in core] == [("armv7l", 62), ("aarch64", 65), ("x86_64", 64)]
assert len(read_tsv(PROGRESS / "tables/historical_core_members_source_mapping.tsv")) == 361

inputs = read_tsv(PROGRESS / "inputs/input_identity.tsv")
for row in inputs:
    path = ROOT / row["path"]
    assert path.stat().st_size == int(row["size_bytes"])
    assert hashlib.sha256(path.read_bytes()).hexdigest() == row["sha256"]

for path in PROGRESS.rglob("*"):
    if path.is_file():
        assert path.stat().st_size <= 5 * 1024 * 1024, f"over 5 MiB: {path}"
for path in CURATED.rglob("*"):
    if path.is_file():
        assert path.stat().st_size <= 5 * 1024 * 1024, f"over 5 MiB: {path}"

verify_manifest(PROGRESS)
verify_manifest(CURATED)

for path in PROGRESS.rglob("*"):
    if not path.is_file() or path.name == "MANIFEST.sha256":
        continue
    if path.relative_to(PROGRESS).as_posix() == "commands/009_final_validation.log":
        continue
    peer = CURATED / path.relative_to(PROGRESS)
    assert peer.exists(), f"curated missing: {peer}"
    assert hashlib.sha256(path.read_bytes()).digest() == hashlib.sha256(peer.read_bytes()).digest(), f"curated mismatch: {peer}"

status = subprocess.run(["git", "status", "--short"], cwd=ROOT, text=True, stdout=subprocess.PIPE, check=True).stdout.splitlines()
unexpected = [line for line in status if not (line.endswith("gbs_llvm.conf") or "progress/R24" in line or "docs/progress/R24" in line)]
assert not unexpected, f"unexpected worktree paths: {unexpected}"

print("validation=PASS")
print("delivery_rows=483")
print("nontrivial_scc=0")
print("phase_internal_edges=35/185/8")
print("curated_byte_identity=PASS")
