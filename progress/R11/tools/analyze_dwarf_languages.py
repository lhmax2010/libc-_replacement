#!/usr/bin/env python3
import argparse
import collections
import csv
import pathlib
import re
import shlex
import subprocess


LANG_RE = re.compile(r"DW_AT_language\s*:\s*(.*)$")
PAREN_RE = re.compile(r"\(([^()]*(?:C\+\+|C11|C17|C99|ANSI C|Rust|Fortran|Ada|Go|Swift|Objective)[^()]*)\)")
PKG_FIELDS = ["repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256"]


def read(path):
    with open(path, encoding="utf-8") as stream: return list(csv.DictReader(stream, delimiter="\t"))


def key(row): return tuple(row[field] for field in PKG_FIELDS) + (row["path"],)


def classify(languages):
    cxx = any("C++" in item or "C Plus Plus" in item for item in languages)
    c = any(
        (item in {"C", "ANSI C", "C89", "C99", "C11", "C17"} or item.startswith("ANSI C") or re.match(r"^C\d", item))
        and "C++" not in item
        for item in languages
    )
    if c and cxx: return "C_AND_CPP_COMPILE_UNITS_MEASURED"
    if cxx: return "CPP_COMPILE_UNITS_ONLY_MEASURED"
    if c: return "C_COMPILE_UNITS_ONLY_MEASURED"
    if languages: return "OTHER_LANGUAGE_COMPILE_UNITS_MEASURED"
    return "NO_DW_AT_LANGUAGE_FOUND"


def write(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--elf-inventory", required=True)
    ap.add_argument("--extract-root", required=True)
    ap.add_argument("--output-root", required=True)
    ap.add_argument("--ledger", required=True)
    args = ap.parse_args()
    rows = read(args.elf_inventory)
    runtime = [row for row in rows if row["runtime_elf"] == "YES"]
    debug = [row for row in rows if row.get("debug_payload") == "YES" and row["build_id"]]
    debug_by_id = collections.defaultdict(list)
    for row in debug: debug_by_id[row["build_id"]].append(row)
    debug_by_path = collections.defaultdict(list)
    for row in rows:
        if row.get("debug_payload") != "YES": continue
        debug_by_path[(row["arch"], row["sourcerpm"], row["path"])].append(row)
    results, commands = [], []
    cache = {}
    extract = pathlib.Path(args.extract_root)
    for index, row in enumerate(runtime, 1):
        base = {field: row[field] for field in PKG_FIELDS} | {"path": row["path"], "build_id": row["build_id"]}
        candidates = debug_by_id.get(row["build_id"], []) if row["build_id"] else []
        match_method = "MATCHED_BY_BUILD_ID" if candidates else ""
        if not candidates:
            expected_path = "/usr/lib/debug" + row["path"] + ".debug"
            candidates = debug_by_path.get((row["arch"], row["sourcerpm"], expected_path), [])
            if candidates: match_method = "MATCHED_BY_DEBUG_PATH_AND_SOURCERPM"
        if not candidates:
            missing = "NO_MATCHING_DEBUG_ELF" if row["build_id"] else "NO_BUILD_ID_OR_DEBUG_PATH_MATCH"
            results.append({**base, "debug_match": missing, "debug_rpm_sha256": "", "debug_path": "", "readelf_exit_code": "NOT_RUN", "languages": "", "classification": "UNDETERMINED_NO_DEBUG_INFO"})
            continue
        candidate = candidates[0]
        debug_key = (candidate["rpm_sha256"], candidate["path"])
        if debug_key not in cache:
            debug_file = extract / candidate["rpm_sha256"][:2] / candidate["rpm_sha256"] / candidate["path"].lstrip("/")
            cmd = ["readelf", "--debug-dump=info", "--dwarf-depth=1", "-W", str(debug_file)]
            proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            languages = set()
            if proc.returncode == 0:
                for line in proc.stdout.splitlines():
                    match = LANG_RE.search(line)
                    if not match: continue
                    value = match.group(1).strip()
                    decoded = PAREN_RE.search(value)
                    languages.add(decoded.group(1).strip() if decoded else value)
            cache[debug_key] = (proc.returncode, sorted(languages), proc.stderr)
            commands.append({
                "label": f"dwarf_language:{candidate['path']}", "pwd": str(pathlib.Path.cwd()),
                "command": shlex.join(cmd), "exit_code": str(proc.returncode),
                "stderr": proc.stderr.replace("\t", " ").replace("\r", " ").replace("\n", "\\n"),
            })
        rc, languages, stderr = cache[debug_key]
        results.append({
            **base, "debug_match": match_method, "debug_rpm_sha256": candidate["rpm_sha256"],
            "debug_path": candidate["path"], "readelf_exit_code": str(rc),
            "languages": ";".join(languages),
            "classification": classify(languages) if rc == 0 else "UNDETERMINED_DWARF_READELF_FAILED",
        })
    root = pathlib.Path(args.output_root)
    fields = PKG_FIELDS + [
        "path", "build_id", "debug_match", "debug_rpm_sha256", "debug_path",
        "readelf_exit_code", "languages", "classification",
    ]
    write(root / "dwarf_language_per_runtime_elf.tsv", fields, results)
    summary = []
    for arch in ["ALL"] + sorted({row["arch"] for row in results}):
        selected = [row for row in results if arch == "ALL" or row["arch"] == arch]
        counts = collections.Counter(row["classification"] for row in selected)
        for category, count in sorted(counts.items()):
            packages = {
                tuple(row[field] for field in PKG_FIELDS)
                for row in selected if row["classification"] == category
            }
            summary.append({
                "arch": arch, "classification": category, "elf_count": count,
                "runtime_elf_denominator": len(selected), "binary_package_count": len(packages),
                "binary_package_with_runtime_elf_denominator": len({tuple(row[field] for field in PKG_FIELDS) for row in selected}),
                "package_counts_overlap_between_categories": "YES",
            })
    write(root / "dwarf_language_distribution.tsv", [
        "arch", "classification", "elf_count", "runtime_elf_denominator", "binary_package_count",
        "binary_package_with_runtime_elf_denominator", "package_counts_overlap_between_categories",
    ], summary)
    with open(args.ledger, "w", encoding="utf-8") as out:
        for i, item in enumerate(commands, 1):
            out.write(f"LABEL=dwarf_{i:06d}:{item['label']}\nPWD={item['pwd']}\nCOMMAND={item['command']}\n")
            if item["stderr"]: out.write(f"STDERR={item['stderr']}\n")
            out.write(f"EXIT_CODE={item['exit_code']}\n\n")
        out.write(f"UNIQUE_DEBUG_ELF_COMMANDS={len(commands)}\nRUNTIME_ELF_RECORDS={len(results)}\n")
    return 0


if __name__ == "__main__": raise SystemExit(main())
