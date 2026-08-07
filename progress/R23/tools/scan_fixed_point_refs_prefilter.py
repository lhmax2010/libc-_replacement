#!/usr/bin/env python3
import csv
import re
import shlex
import subprocess
from collections import defaultdict
from pathlib import Path

ROOT = Path.cwd()
OUT = ROOT / "progress/R23/tables"
INP = ROOT / "progress/R23/inputs"
OUT.mkdir(parents=True, exist_ok=True); INP.mkdir(parents=True, exist_ok=True)
ARCHES = ("armv7l", "aarch64", "x86_64")

def read_tsv(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(name, fields, records):
    with (OUT / name).open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, delimiter="\t", fieldnames=fields, lineterminator="\n")
        w.writeheader(); w.writerows(records)

exports = read_tsv(ROOT / "progress/R16/tables/libgcc_export_classification.tsv")
version_rows = read_tsv(ROOT / "progress/R16/tables/libgcc_symbol_versions.tsv")
families = read_tsv(ROOT / "progress/R17/tables/missing_helper_family_members.tsv")
runtime = [r for r in read_tsv(ROOT / "progress/R11/tables/elf_inventory.tsv") if r["runtime_elf"] == "YES"]
binary_packages = read_tsv(ROOT / "progress/R11/tables/binary_package_records.tsv")
vendor_names = {r["vendor_package_name"] for r in read_tsv(ROOT / "progress/R13/tables/vendor_prebuilt_closure_assignment.tsv")}
versions = defaultdict(set)
for r in version_rows: versions[(r["arch"], r["symbol"])].add(r["version_node"])
export_keys = {(r["arch"], r["symbol"]) for r in exports}
fixed = {(r["arch"], r["symbol"]) for r in families if r["family"] == "FIXED_POINT"} & export_keys
fixed_by_arch = {a: {s for aa, s in fixed if aa == a} for a in ARCHES}

write_tsv("libgcc_exports_full.tsv", ["arch", "symbol", "category", "version_nodes"], [
    {"arch": r["arch"], "symbol": r["symbol"], "category": r["category"],
     "version_nodes": ",".join(sorted(versions[(r["arch"], r["symbol"])]))}
    for r in sorted(exports, key=lambda x: (x["arch"], x["symbol"]))])
write_tsv("fixed_point_symbols.tsv", ["arch", "symbol", "version_nodes", "classification_basis"], [
    {"arch": a, "symbol": s, "version_nodes": ",".join(sorted(versions[(a, s)])),
     "classification_basis": "R17_FIXED_POINT_MEMBER_AND_R16_ACTUAL_LIBGCC_EXPORT"}
    for a, s in sorted(fixed)])
pattern_file = INP / "fixed_point_symbol_patterns.txt"
pattern_file.write_text("".join(s + "\n" for s in sorted({s for _, s in fixed})), encoding="utf-8")

def local_path(r):
    sha = r["rpm_sha256"]
    return ROOT / "tmp/R11/extracted" / sha[:2] / sha / r["path"].lstrip("/")

def nevra(r):
    epoch = "" if r["epoch"] in ("", "0") else r["epoch"] + ":"
    return f'{r["name"]}-{epoch}{r["version"]}-{r["release"]}.{r["arch"]}'

paths = [str(local_path(r)) for r in runtime]
path_index = {p: i for i, p in enumerate(paths)}
facts = [{"status": "PENDING", "rc": None, "hits": set(), "raw_candidate": False,
          "producer": "NOT_PROBED_OUTSIDE_VENDOR_OR_RAW_CANDIDATE", "diagnostic": ""} for _ in runtime]
commands, seq = [], 0

def invoke(args, scope):
    global seq
    cp = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                        text=True, errors="replace", check=False)
    seq += 1
    commands.append({"sequence": seq, "scope": scope,
                     "command": " ".join(shlex.quote(x) for x in args), "exit_code": cp.returncode})
    return cp

batch_size = 160
candidates = set()
for start in range(0, len(paths), batch_size):
    indices = list(range(start, min(start + batch_size, len(paths))))
    ps = [paths[i] for i in indices]
    cp = invoke(["grep", "-a", "-l", "-F", "-f", str(pattern_file), "--", *ps],
                f"FIXED_SYMBOL_BYTE_PREFILTER_BATCH_{start // batch_size + 1:04d}")
    if cp.returncode in (0, 1):
        matched = {line for line in cp.stdout.splitlines() if line in path_index}
        for i in indices:
            facts[i]["status"] = "OK_PREFILTER"
            facts[i]["rc"] = cp.returncode
        for p in matched:
            facts[path_index[p]]["raw_candidate"] = True
            candidates.add(path_index[p])
    else:
        for i in indices:
            one = invoke(["grep", "-a", "-l", "-F", "-f", str(pattern_file), "--", paths[i]],
                         "FAILED_PREFILTER_BATCH_INDIVIDUAL_FALLBACK")
            if one.returncode in (0, 1):
                facts[i]["status"] = "OK_PREFILTER"
                facts[i]["rc"] = one.returncode
                if one.returncode == 0:
                    facts[i]["raw_candidate"] = True; candidates.add(i)
            else:
                facts[i]["status"] = "PREFILTER_FAILED"
                facts[i]["rc"] = one.returncode
                facts[i]["diagnostic"] = one.stdout[:2000]

dynsym_re = re.compile(r"^\s*\d+:\s+[0-9a-fA-F]+\s+\d+\s+\S+\s+\S+\s+\S+\s+UND\s+(.+?)\s*$")
for i in sorted(candidates):
    cp = invoke(["readelf", "--dyn-syms", "-W", paths[i]], "RAW_CANDIDATE_UND_CONFIRMATION")
    if cp.returncode != 0:
        facts[i]["status"] = "READELF_CONFIRMATION_FAILED"
        facts[i]["rc"] = cp.returncode
        facts[i]["diagnostic"] = cp.stdout[:2000]
        continue
    facts[i]["status"] = "OK_CONFIRMED"
    facts[i]["rc"] = 0
    for line in cp.stdout.splitlines():
        m = dynsym_re.match(line)
        if not m: continue
        raw = m.group(1).split()[0]
        base = raw.split("@", 1)[0]
        if base in fixed_by_arch.get(runtime[i]["arch"], set()):
            facts[i]["hits"].add((base, raw))

# Compiler comments are probed only for the five adjudicated vendor-prebuilt
# package names and raw symbol candidates.  Ownership/third-party status for all
# other packages is not present in R11 and is not inferred from compiler strings.
comment_indices = sorted({i for i, r in enumerate(runtime) if r["name"] in vendor_names} | candidates)
for i in comment_indices:
    cp = invoke(["readelf", "--string-dump=.comment", "-W", paths[i]], "TARGETED_COMPILER_COMMENT_PROBE")
    t = cp.stdout
    has_gcc = bool(re.search(r"\bGCC:\s*\(|\bGCC:\s", t))
    has_clang = "clang version" in t or ("Android (" in t and "clang" in t.lower())
    facts[i]["producer"] = ("MIXED_GCC_AND_CLANG_COMMENT" if has_gcc and has_clang else
                            "GCC_COMMENT_IDENTIFIED" if has_gcc else
                            "CLANG_COMMENT_IDENTIFIED" if has_clang else
                            "NOT_IDENTIFIABLE_FROM_ELF_COMMENT")

scan_rows, prefilter_rows, hit_rows, vendor_rows, producer_rows, failures = [], [], [], [], [], []
for i, (r, p, f) in enumerate(zip(runtime, paths, facts)):
    symbols = sorted({s for s, _ in f["hits"]})
    common = {"repo_id": r["repo_id"], "package_nevra": nevra(r), "package_name": r["name"],
              "source_rpm": r["sourcerpm"], "arch": r["arch"], "elf_path": r["path"], "rpm_sha256": r["rpm_sha256"]}
    scan_rows.append({**common, "local_elf_path": p, "probe_status": f["status"], "probe_exit_code": f["rc"],
                      "raw_symbol_byte_candidate": "YES" if f["raw_candidate"] else "NO",
                      "fixed_point_und_count": len(symbols), "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE"})
    if f["raw_candidate"]:
        prefilter_rows.append({**common, "confirmation_status": f["status"],
                               "fixed_point_und_count": len(symbols), "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE"})
    for base, raw in sorted(f["hits"]):
        hit_rows.append({"symbol": base, "raw_dynamic_symbol": raw, **common})
    if r["name"] in vendor_names:
        vendor_rows.append({"vendor_package": r["name"], "package_nevra": nevra(r), "arch": r["arch"],
                            "elf_path": r["path"], "probe_status": f["status"], "raw_symbol_byte_candidate": "YES" if f["raw_candidate"] else "NO",
                            "fixed_point_und_count": len(symbols), "fixed_point_und_symbols": ",".join(symbols) if symbols else "NONE",
                            "compiler_producer_evidence": f["producer"]})
    if i in comment_indices:
        producer_rows.append({"package_nevra": nevra(r), "package_name": r["name"], "source_rpm": r["sourcerpm"],
                              "arch": r["arch"], "elf_path": r["path"], "producer_class": f["producer"],
                              "third_party_or_prebuilt_status": "VENDOR_PREBUILT_ADJUDICATED" if r["name"] in vendor_names else "NOT_DETERMINABLE_FROM_ELF_COMMENT"})
    if not f["status"].startswith("OK_"):
        failures.append({"package_nevra": nevra(r), "arch": r["arch"], "elf_path": r["path"], "local_elf_path": p,
                         "status": f["status"], "exit_code": f["rc"], "diagnostic": f["diagnostic"].replace("\t", " ").replace("\n", " | ")})

write_tsv("elf_probe_commands.tsv", ["sequence", "scope", "command", "exit_code"], commands)
write_tsv("runtime_elf_scan_results.tsv", list(scan_rows[0]), scan_rows)
write_tsv("fixed_point_prefilter_candidates.tsv", list(prefilter_rows[0]) if prefilter_rows else ["repo_id"], prefilter_rows)
write_tsv("fixed_point_undefined_references.tsv", ["symbol", "raw_dynamic_symbol", "repo_id", "package_nevra", "package_name", "source_rpm", "arch", "elf_path", "rpm_sha256"], hit_rows)
write_tsv("vendor_prebuilt_elf_scan.tsv", list(vendor_rows[0]) if vendor_rows else ["vendor_package"], vendor_rows)
write_tsv("targeted_compiler_comment_probes.tsv", list(producer_rows[0]) if producer_rows else ["package_nevra"], producer_rows)
write_tsv("elf_probe_failures.tsv", ["package_nevra", "arch", "elf_path", "local_elf_path", "status", "exit_code", "diagnostic"], failures)

summary = []
for arch in (*ARCHES, "ALL"):
    subset = [x for x in scan_rows if arch == "ALL" or x["arch"] == arch]
    ok = [x for x in subset if x["probe_status"].startswith("OK_")]
    h = [x for x in ok if x["fixed_point_und_count"]]
    ah = [x for x in hit_rows if arch == "ALL" or x["arch"] == arch]
    result = "ZERO_REFERENCES_CONFIRMED" if len(ok) == len(subset) and not h else ("REFERENCES_FOUND" if h else "UNDETERMINED")
    summary.append({"arch": arch, "runtime_elf_denominator": len(subset), "successfully_scanned_elf": len(ok),
                    "unavailable_or_failed_elf": len(subset) - len(ok),
                    "fixed_point_export_symbol_count": sum(1 for a, _ in fixed if arch == "ALL" or a == arch),
                    "raw_symbol_byte_candidate_elf_count": sum(x["raw_symbol_byte_candidate"] == "YES" for x in subset),
                    "referenced_fixed_point_symbol_count": len({x["symbol"] for x in ah}),
                    "referencing_elf_count": len(h), "referencing_binary_package_count": len({x["package_nevra"] for x in h}), "result": result})
write_tsv("fixed_point_scan_summary.tsv", list(summary[0]), summary)

original = {"armv7l": 986, "aarch64": 25, "x86_64": 27}
corrected = []
for arch in ARCHES:
    s = next(x for x in summary if x["arch"] == arch)
    if s["result"] == "ZERO_REFERENCES_CONFIRMED":
        value, rule = original[arch] - s["fixed_point_export_symbol_count"], "ORIGINAL_GAP_MINUS_MEASURED_ZERO_REFERENCE_FIXED_POINT_FAMILY"
    elif s["result"] == "REFERENCES_FOUND":
        value, rule = original[arch], "ORIGINAL_GAP_RETAINED_FIXED_POINT_FAMILY_HAS_REFERENCES"
    else:
        value, rule = "UNDETERMINED", "SCAN_INCOMPLETE_NO_NUMERIC_CORRECTION"
    corrected.append({"arch": arch, "original_missing_helper_count": original[arch], "fixed_point_family_count": s["fixed_point_export_symbol_count"],
                      "referenced_fixed_point_symbol_count": s["referenced_fixed_point_symbol_count"], "corrected_missing_helper_count": value,
                      "correction_rule": rule})
write_tsv("corrected_gap_summary.tsv", list(corrected[0]), corrected)
scope = [{"counting_unit": "binary_package_instance", "denominator": len(binary_packages), "source": "R11_binary_package_records.tsv"},
         {"counting_unit": "runtime_ELF", "denominator": len(runtime), "source": "R11_elf_inventory.tsv runtime_elf=YES"},
         {"counting_unit": "source_package", "denominator": "NOT_USED_IN_REFERENCE_RATIO", "source": "no source-package ratio in this scan"}]
write_tsv("scan_scope_denominators.tsv", list(scope[0]), scope)

print(f"runtime_elf={len(runtime)} binary_packages={len(binary_packages)} success={sum(x['probe_status'].startswith('OK_') for x in scan_rows)} failures={len(failures)} commands={len(commands)}")
print(f"fixed_symbols={len(fixed)} raw_candidates={len(candidates)} hit_rows={len(hit_rows)} vendor_elf={len(vendor_rows)}")
for r in summary: print("SUMMARY", *(f"{k}={v}" for k, v in r.items()))
