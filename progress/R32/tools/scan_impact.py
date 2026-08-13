#!/usr/bin/env python3
import csv
import hashlib
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "progress/R32/tables"
CORPUS = ROOT / "tmp/corpus/extracted"
SRPMS = ROOT / "tmp/corpus/srpms"

ARCHIVE_GLOBS = [
    "!*.tar", "!*.tar.gz", "!*.tgz", "!*.tar.bz2", "!*.tbz2",
    "!*.tar.xz", "!*.txz", "!*.tar.zst", "!*.zip", "!*.7z",
    "!*.rar", "!*.gz", "!*.bz2", "!*.xz", "!*.zst", "!*.rpm",
    "!*.src.rpm", "!*.deb", "!*.jar", "!*.apk", "!*.iso",
]

CODE_GLOBS = [
    "*.c", "*.C", "*.cc", "*.cpp", "*.cxx", "*.h", "*.H", "*.hh",
    "*.hpp", "*.hxx", "*.ipp", "*.tcc", "*.inc", "*.inl", "*.m",
    "*.mm", "*.cu", "*.cl", "*.rs", "*.go", "*.py", "*.sh",
    "*.bash", "*.pl", "*.rb", "*.java", "*.js", "*.ts", "*.cs",
    "*.vala", "*.vapi", "*.idl", "*.def", "*.S", "*.s", "*.asm",
    "*.cmake", "*.mk", "*.am", "*.ac", "*.spec", "*.gyp", "*.gypi",
    "*.gn", "*.ninja", "*.bazel", "*.bzl", "*.pro", "*.pri", "*.txt",
    "*.md", "*.rst", "Makefile*", "CMakeLists.txt", "meson.build",
    "meson_options.txt", "SConstruct", "SConscript", "configure*",
]

RX = re.compile(
    r"catch\s*\(\s*\.\s*\.\s*\.\s*\)|"
    r"(?:__cxxabiv1|abi)\s*::\s*__forced_unwind|"
    r"pthread_cancel|pthread_create|pthread_setcancelstate|pthread_testcancel|"
    r"std\s*::\s*thread"
)
CATCH = re.compile(r"catch\s*\(\s*\.\s*\.\s*\.\s*\)")
FORCED = re.compile(r"(?:__cxxabiv1|abi)\s*::\s*__forced_unwind")
MARKERS = {
    "pthread_cancel": re.compile(r"\bpthread_cancel\b"),
    "pthread_create": re.compile(r"\bpthread_create\b"),
    "pthread_setcancelstate": re.compile(r"\bpthread_setcancelstate\b"),
    "pthread_testcancel": re.compile(r"\bpthread_testcancel\b"),
    "std::thread": re.compile(r"\bstd\s*::\s*thread\b"),
}


def sha(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()


def scope(rel):
    p = rel.lower().replace("\\", "/")
    parts = p.split("/")
    if any(x in parts for x in ("test", "tests", "testing", "unittest", "unittests")) or re.search(r"(^|/)(test|tests?)[_.-]", p):
        return "TEST"
    if any(x in parts for x in ("doc", "docs", "documentation")) or p.endswith((".md", ".rst", ".txt")):
        return "DOC"
    if any(x in parts for x in ("example", "examples", "sample", "samples")):
        return "EXAMPLE"
    if p.endswith((".spec", ".cmake")) or os.path.basename(p) in ("meson.build", "configure.ac", "makefile"):
        return "BUILD"
    return "PRODUCTION_LIKE"


def read_lines(path):
    try:
        return path.read_text(errors="replace").splitlines()
    except OSError:
        return []


def macro_for(lines, idx):
    # idx is zero based. A catch token in a continued #define belongs to the
    # nearest preceding #define while every intervening previous line ends '\\'.
    j = idx
    while j >= 0 and idx - j <= 50:
        m = re.match(r"\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)", lines[j])
        if m:
            if all(lines[k].rstrip().endswith("\\") for k in range(j, idx)):
                return m.group(1)
            return ""
        if j < idx and not lines[j].rstrip().endswith("\\"):
            break
        j -= 1
    return ""


def guard_status(lines, idx):
    lo = max(0, idx - 3)
    hi = min(len(lines), idx + 15)
    text = "\n".join(lines[lo:hi])
    if not FORCED.search(text):
        return "NOT_FORCED_CATCH"
    # Locate the forced catch line and bound the body at the next catch.
    start = None
    for n in range(lo, hi):
        if FORCED.search(lines[n]):
            start = n
            break
    if start is None:
        return "UNDETERMINED"
    end = min(len(lines), start + 16)
    for n in range(start + 1, end):
        if re.search(r"\bcatch\s*\(", lines[n]):
            end = n
            break
    body = "\n".join(lines[start:end])
    return "CONFIRMED_RETHROW" if re.search(r"\bthrow\s*;", body) else "FORCED_CATCH_NO_RETHROW"


def rpm_name(srpm):
    p = SRPMS / srpm
    cp = subprocess.run(["rpm", "-qp", "--qf", "%{NAME}", str(p)], text=True, capture_output=True)
    return cp.stdout.strip() if cp.returncode == 0 else "NOT_AVAILABLE"


def run_rg(root):
    cmd = ["rg", "--threads", "1", "--json", "--line-number", "--hidden", "--no-ignore", "--pcre2", "-e", RX.pattern]
    for g in CODE_GLOBS:
        cmd += ["-g", g]
    cmd += [str(root)]
    cp = subprocess.run(cmd, text=True, capture_output=True)
    if cp.returncode not in (0, 1):
        raise RuntimeError(f"rg failed rc={cp.returncode}: {cp.stderr}")
    rows = []
    for raw in cp.stdout.splitlines():
        import json
        obj = json.loads(raw)
        if obj.get("type") != "match":
            continue
        d = obj["data"]
        path = Path(d["path"]["text"])
        rows.append((path, int(d["line_number"]), d["lines"]["text"].rstrip("\n")))
    return rows


def write_tsv(name, header, rows):
    OUT.mkdir(parents=True, exist_ok=True)
    with open(OUT / name, "w", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n")
        w.writerow(header)
        w.writerows(rows)


def main():
    extraction = ROOT / "progress/T1/extraction_results.tsv"
    migration = ROOT / "progress/R24/tables/migration_packages_483.tsv"
    with open(extraction, newline="") as f:
        erows = list(csv.DictReader(f, delimiter="\t"))
    scanned = [r["source_rpm"] for r in erows if r["status"] == "EXTRACTED_OK"]
    if len(scanned) != 371:
        raise SystemExit(f"expected 371 EXTRACTED_OK, got {len(scanned)}")

    with open(migration, newline="") as f:
        mrows = list(csv.DictReader(f, delimiter="\t"))
    exact_stage = {r["source_rpm_identity"]: r["stage"] for r in mrows}
    name_stages = defaultdict(set)
    for r in mrows:
        name_stages[r["package_name"]].add(r["stage"])

    locations = []
    guard_rows = []
    macro_defs = []
    package_stats = {}
    status_rows = []
    macro_names_by_pkg = defaultdict(set)
    def scan_one(srpm):
        root = CORPUS / srpm
        name = rpm_name(srpm)
        if srpm in exact_stage:
            stage, stage_method = exact_stage[srpm], "EXACT_SOURCE_RPM"
        elif len(name_stages.get(name, set())) == 1:
            stage, stage_method = next(iter(name_stages[name])), "UNIQUE_SOURCE_NAME"
        elif name in name_stages:
            stage, stage_method = ",".join(sorted(name_stages[name])), "AMBIGUOUS_SOURCE_NAME"
        else:
            stage, stage_method = "NOT_IN_483", "NOT_IN_483"
        stats = Counter()
        local_locations = []
        local_guards = []
        local_macro_defs = []
        local_macro_names = set()
        line_cache = {}
        try:
            matches = run_rg(root)
            for path, lineno, text in matches:
                rel = str(path.relative_to(root))
                sc = scope(rel)
                if path not in line_cache:
                    line_cache[path] = read_lines(path)
                lines = line_cache[path]
                idx = lineno - 1
                macro = macro_for(lines, idx) if CATCH.search(text) else ""
                if macro:
                    local_macro_names.add(macro)
                    local_macro_defs.append((srpm, name, stage, rel, lineno, sc, macro, text.strip()))
                if CATCH.search(text):
                    local_locations.append((srpm, name, stage, stage_method, "catch_all", rel, lineno, sc, macro or "-", text.strip()))
                    stats["catch_all_all"] += 1
                    if sc != "DOC":
                        stats["catch_all_nondoc"] += 1
                    if sc == "PRODUCTION_LIKE":
                        stats["catch_all_production"] += 1
                for marker, rx in MARKERS.items():
                    if rx.search(text):
                        local_locations.append((srpm, name, stage, stage_method, marker, rel, lineno, sc, "-", text.strip()))
                        stats[marker + "_all"] += 1
                        if sc != "DOC":
                            stats[marker + "_nondoc"] += 1
                        if sc == "PRODUCTION_LIKE":
                            stats[marker + "_production"] += 1
                if FORCED.search(text):
                    gs = guard_status(lines, idx)
                    local_locations.append((srpm, name, stage, stage_method, "forced_unwind", rel, lineno, sc, macro or "-", text.strip()))
                    local_guards.append((srpm, name, stage, rel, lineno, sc, gs, text.strip()))
                    stats["forced_all"] += 1
                    if gs == "CONFIRMED_RETHROW" and sc != "DOC":
                        stats["guard_rethrow_nondoc"] += 1
                    if gs == "CONFIRMED_RETHROW" and sc == "PRODUCTION_LIKE":
                        stats["guard_rethrow_production"] += 1
                    if gs == "FORCED_CATCH_NO_RETHROW" and sc != "DOC":
                        stats["forced_no_rethrow_nondoc"] += 1
            status = (srpm, name, str(root), "SCAN_OK", len(matches), "")
        except Exception as e:
            status = (srpm, name, str(root), "SCAN_FAILED", 0, str(e))
            stats["scan_failed"] = 1
        return (srpm, name, stage, stage_method, stats, status,
                local_locations, local_guards, local_macro_defs,
                local_macro_names)

    with ThreadPoolExecutor(max_workers=4) as pool:
        futures = {pool.submit(scan_one, srpm): srpm for srpm in scanned}
        for number, future in enumerate(as_completed(futures), 1):
            (srpm, name, stage, stage_method, stats, status, local_locations,
             local_guards, local_macro_defs, local_macro_names) = future.result()
            package_stats[srpm] = (name, stage, stage_method, stats)
            status_rows.append(status)
            locations.extend(local_locations)
            guard_rows.extend(local_guards)
            macro_defs.extend(local_macro_defs)
            macro_names_by_pkg[srpm].update(local_macro_names)
            if number % 25 == 0:
                print(f"scanned {number}/{len(scanned)}", file=sys.stderr, flush=True)

    macro_uses = []
    for srpm, names in sorted(macro_names_by_pkg.items()):
        root = CORPUS / srpm
        name, stage, _, _ = package_stats[srpm]
        for macro in sorted(names):
            cmd = ["rg", "--threads", "1", "--json", "--line-number", "--hidden", "--no-ignore", "-w", "-e", macro]
            for g in CODE_GLOBS:
                cmd += ["-g", g]
            cmd += [str(root)]
            cp = subprocess.run(cmd, text=True, capture_output=True)
            if cp.returncode not in (0, 1):
                continue
            import json
            for raw in cp.stdout.splitlines():
                obj = json.loads(raw)
                if obj.get("type") != "match":
                    continue
                d = obj["data"]
                path = Path(d["path"]["text"])
                rel = str(path.relative_to(root))
                line = d["lines"]["text"].rstrip("\n")
                kind = "DEFINITION" if re.search(r"#\s*define\s+" + re.escape(macro) + r"\b", line) else "USE"
                macro_uses.append((srpm, name, stage, macro, kind, rel, d["line_number"], scope(rel), line.strip()))

    classifications = []
    summary = Counter()
    for srpm in scanned:
        name, stage, method, s = package_stats[srpm]
        thread_nondoc = sum(s[x + "_nondoc"] for x in MARKERS)
        thread_prod = sum(s[x + "_production"] for x in MARKERS)
        if s["scan_failed"]:
            category = "UNDETERMINED_SCAN_FAILED"
        elif s["guard_rethrow_nondoc"]:
            category = "EXISTING_GUARD"
        elif s["catch_all_nondoc"] and thread_nondoc:
            category = "UNGUARDED_RISK_PATTERN"
        elif s["catch_all_nondoc"]:
            category = "CATCH_ONLY_NO_THREAD_EVIDENCE"
        else:
            category = "NO_CATCH_ALL"
        summary[category] += 1
        classifications.append((
            srpm, name, stage, method, category,
            s["catch_all_all"], s["catch_all_nondoc"], s["catch_all_production"],
            s["guard_rethrow_nondoc"], s["guard_rethrow_production"],
            s["forced_no_rethrow_nondoc"], thread_nondoc, thread_prod,
            s["pthread_cancel_nondoc"], s["pthread_create_nondoc"],
            s["pthread_setcancelstate_nondoc"], s["pthread_testcancel_nondoc"],
            s["std::thread_nondoc"],
        ))

    write_tsv("scan_status.tsv", ["source_rpm", "package_name", "root", "status", "rg_match_lines", "detail"], status_rows)
    write_tsv("impact_locations.tsv", ["source_rpm", "package_name", "stage", "stage_mapping", "feature", "file", "line", "scope", "macro_name", "source_text"], sorted(locations))
    write_tsv("forced_unwind_guard_locations.tsv", ["source_rpm", "package_name", "stage", "file", "line", "scope", "guard_status", "source_text"], sorted(guard_rows))
    write_tsv("catchall_macro_definitions.tsv", ["source_rpm", "package_name", "stage", "file", "line", "scope", "macro_name", "source_text"], sorted(macro_defs))
    write_tsv("catchall_macro_uses.tsv", ["source_rpm", "package_name", "stage", "macro_name", "kind", "file", "line", "scope", "source_text"], sorted(macro_uses))
    write_tsv("impact_package_classification.tsv", [
        "source_rpm", "package_name", "stage", "stage_mapping", "category",
        "catch_all_locations_all", "catch_all_locations_nondoc", "catch_all_locations_production_like",
        "confirmed_guard_locations_nondoc", "confirmed_guard_locations_production_like",
        "forced_catch_without_rethrow_locations_nondoc", "thread_cancel_locations_nondoc",
        "thread_cancel_locations_production_like", "pthread_cancel_locations_nondoc",
        "pthread_create_locations_nondoc", "pthread_setcancelstate_locations_nondoc",
        "pthread_testcancel_locations_nondoc", "std_thread_locations_nondoc",
    ], classifications)
    write_tsv("impact_summary.tsv", ["category", "source_package_count", "denominator_scanned_source_packages", "scope"],
              [(k, summary[k], 371, "package-level co-occurrence; DOC excluded; TEST/EXAMPLE/BUILD retained and tagged") for k in sorted(summary)])
    write_tsv("input_identity.tsv", ["input", "sha256", "purpose"], [
        (str(extraction.relative_to(ROOT)), sha(extraction), "371 extracted-source status and denominator"),
        ("progress/T1/corpus_hits.tsv", sha(ROOT / "progress/T1/corpus_hits.tsv"), "existing pthread_cancel/setcancelstate/testcancel pattern hits"),
        ("progress/T1/scan_status.tsv", sha(ROOT / "progress/T1/scan_status.tsv"), "original scan coverage"),
        ("progress/R1/package_three_tiers.tsv", sha(ROOT / "progress/R1/package_three_tiers.tsv"), "R1 classification identity"),
        (str(migration.relative_to(ROOT)), sha(migration), "migration stage mapping"),
    ])
    print(f"packages={len(scanned)} locations={len(locations)} classifications={dict(summary)}")


if __name__ == "__main__":
    main()
