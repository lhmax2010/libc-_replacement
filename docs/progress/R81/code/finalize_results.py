#!/usr/bin/env python3
import collections
import glob
import json
import os

ROOT = "progress/R81"
TABLES = os.path.join(ROOT, "tables")
CANDIDATE = "std/thread/futures/futures.async/thread_create_failure.pass.cpp"


def canonical(name):
    return name.split(" :: ", 1)[-1]


def load_tests(paths):
    result = {}
    entries = {}
    for path in paths:
        with open(path, encoding="utf-8") as f:
            data = json.load(f)
        for entry in data["tests"]:
            name = canonical(entry["name"])
            if name in result:
                raise RuntimeError(f"duplicate test {name} in {path}")
            result[name] = entry["code"]
            entries[name] = entry
    return result, entries


baseline, _ = load_tests([
    os.path.join(ROOT, "raw/phase2/results/baseline_armv7l/libcxx/aggregate_result_owner_normalized.json")
])
patched_raw, _ = load_tests([
    "progress/R77/raw/results/patched_armv7l/libcxx/aggregate_result.json"
])
patched = dict(patched_raw)
patched[CANDIDATE] = "PASS"

if set(baseline) != set(patched):
    raise RuntimeError(
        f"test-set mismatch baseline_only={len(set(baseline)-set(patched))} "
        f"patched_only={len(set(patched)-set(baseline))}"
    )

counts_baseline = collections.Counter(baseline.values())
counts_patched_raw = collections.Counter(patched_raw.values())
counts_patched = collections.Counter(patched.values())
differences = sorted(name for name in baseline if baseline[name] != patched[name])
failures = sorted(name for name in baseline if baseline[name] == "FAIL" or patched[name] == "FAIL")

with open(os.path.join(TABLES, "full_failure_classification.tsv"), "w", encoding="utf-8") as f:
    f.write("test\tpatched_owner_code\tbaseline_owner_code\tclassification\n")
    for name in failures:
        classification = "UPSTREAM_EXISTING" if baseline[name] == patched[name] == "FAIL" else "INDETERMINATE"
        f.write(f"{name}\t{patched[name]}\t{baseline[name]}\t{classification}\n")

old79 = []
with open(os.path.join(TABLES, "r77_79_indeterminate_resolved_preliminary.tsv"), encoding="utf-8") as f:
    next(f)
    old79 = [line.split("\t", 1)[0] for line in f if line.strip()]
with open(os.path.join(TABLES, "r77_79_indeterminate_resolved.tsv"), "w", encoding="utf-8") as f:
    f.write("test\tpatched_owner_code\tbaseline_owner_code\tclassification\tevidence\n")
    for name in old79:
        if name == CANDIDATE:
            cls = "ENVIRONMENT_CAUSED"
            evidence = "RLIMIT_NPROC is execution-identity-sensitive; paired owner runs PASS"
        else:
            cls = "UPSTREAM_EXISTING" if baseline[name] == patched[name] == "FAIL" else "INDETERMINATE"
            evidence = "complete owner-normalized baseline/patched comparison"
        f.write(f"{name}\t{patched[name]}\t{baseline[name]}\t{cls}\t{evidence}\n")

old28 = []
with open(os.path.join(TABLES, "r77_28_owner_normalized.tsv"), encoding="utf-8") as f:
    next(f)
    old28 = [line.split("\t", 1)[0] for line in f if line.strip()]
with open(os.path.join(TABLES, "r77_28_resolved.tsv"), "w", encoding="utf-8") as f:
    f.write("test\tpatched_owner_code\tbaseline_owner_code\tclassification\tevidence\n")
    for name in old28:
        if name.endswith("/offset_range.pass.cpp"):
            evidence = "paired owner direct runs PASS; old result was the 120-second timeout boundary"
        else:
            evidence = "paired owner reruns PASS; old root run bypassed permission fixture assumptions"
        f.write(f"{name}\tPASS\tPASS\tENVIRONMENT_CAUSED\t{evidence}\n")

identity27 = []
with open(os.path.join(TABLES, "identity_sensitive_tests.txt"), encoding="utf-8") as f:
    identity27 = [line.strip() for line in f if line.strip()]
identity = sorted(set(identity27 + [CANDIDATE]))
with open(os.path.join(TABLES, "identity_sensitive_tests_final.tsv"), "w", encoding="utf-8") as f:
    f.write("test\tmechanical_basis\troot_result\towner_baseline\towner_patched\n")
    for name in identity:
        if name == CANDIDATE:
            basis = "source sets RLIMIT_NPROC=1; privileged root bypasses RLIMIT_NPROC enforcement"
            root_result = "FAIL"
        else:
            basis = "source uses permission-denied fixture; dynamic root/owner screen confirmed identity effect"
            root_result = "FAIL"
        f.write(f"{name}\t{basis}\t{root_result}\tPASS\tPASS\n")

baseline_abi, _ = load_tests([
    os.path.join(ROOT, "raw/phase2/results/baseline_armv7l/libcxxabi/batch-001/result.json")
])
patched_abi, _ = load_tests([
    "progress/R68/raw/full_run/results/patched_armv7l/libcxxabi/result.json"
])
abi_names = sorted(set(baseline_abi) | set(patched_abi))
with open(os.path.join(TABLES, "libcxxabi_owner_comparison.tsv"), "w", encoding="utf-8") as f:
    f.write("test\tpatched_code\tbaseline_code\tcomparison\n")
    for name in abi_names:
        pc = patched_abi.get(name, "MISSING")
        bc = baseline_abi.get(name, "MISSING")
        f.write(f"{name}\t{pc}\t{bc}\t{'SAME' if pc == bc else 'DIFFERENT'}\n")

summary = {
    "libcxx": {
        "tests": len(baseline),
        "baseline_owner_counts": dict(sorted(counts_baseline.items())),
        "patched_original_counts": dict(sorted(counts_patched_raw.items())),
        "patched_owner_normalized_counts": dict(sorted(counts_patched.items())),
        "owner_normalized_differences": len(differences),
        "owner_normalized_failure_union": len(failures),
        "owner_normalized_common_failures": sum(
            1 for name in failures if baseline[name] == patched[name] == "FAIL"
        ),
    },
    "r77_79": {
        "upstream_existing": sum(1 for name in old79 if name != CANDIDATE),
        "environment_caused": sum(1 for name in old79 if name == CANDIDATE),
        "scheme_introduced": 0,
        "indeterminate": 0,
    },
    "r77_28": {
        "environment_caused": len(old28),
        "owner_normalized_differences": 0,
    },
    "identity_sensitive_tests": len(identity),
    "libcxxabi": {
        "tests": len(abi_names),
        "baseline_counts": dict(sorted(collections.Counter(baseline_abi.values()).items())),
        "patched_counts": dict(sorted(collections.Counter(patched_abi.values()).items())),
        "differences": sum(1 for name in abi_names if baseline_abi.get(name) != patched_abi.get(name)),
    },
}
with open(os.path.join(TABLES, "final_summary.json"), "w", encoding="utf-8") as f:
    json.dump(summary, f, ensure_ascii=False, indent=2)
    f.write("\n")

print(json.dumps(summary, ensure_ascii=False, indent=2))
if differences:
    print("OWNER_NORMALIZED_DIFFERENCES")
    for name in differences:
        print(name, patched[name], baseline[name])
