#!/usr/bin/env python3
"""Read-only audit of consumers outside R47's default delivery assumptions."""

import csv
import re
import subprocess
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CORPUS = ROOT / "tmp/corpus/extracted"
OUT = ROOT / "progress/W3/tables"
STATUS = ROOT / "progress/R32/tables/scan_status.tsv"

CODE_GLOBS = [
    "*.C", "*.cc", "*.cpp", "*.cxx", "*.h", "*.hh", "*.hpp",
    "*.hxx", "*.ipp", "*.tcc", "*.inc", "*.inl", "*.mm", "*.S",
    "*.spec", "*.cmake", "*.mk", "*.am", "*.ac", "*.gyp", "*.gypi",
    "*.gn", "*.ninja", "*.bazel", "*.bzl", "*.pro", "*.pri",
    "Makefile*", "CMakeLists.txt", "meson.build", "meson_options.txt",
    "configure*", "SConstruct", "SConscript",
]

EXCLUDED_GLOBS = [
    "!**/test/**", "!**/tests/**", "!**/testing/**", "!**/testsuite/**",
    "!**/unittest/**", "!**/unittests/**", "!**/benchmarks/**",
    "!**/docs/**", "!**/doc/**", "!**/examples/**",
    "!**/example/**", "!**/samples/**", "!**/sample/**",
]

COMBINED = re.compile(
    r"libc\+\+\.a|(?:-Bstatic|-static).*?(?:-lc\+\+|libc\+\+)|"
    r"(?:-lc\+\+|libc\+\+).*?(?:-Bstatic|-static)|"
    r"(?:^|[\s=\"'])-flto(?:[=\s\"']|$)|INTERPROCEDURAL_OPTIMIZATION|"
    r"\bb_lto\s*[:=]\s*true\b|\buse_lto\s*[:=]\s*(?:1|true)\b|"
    r"_LIBCPP_DISABLE_EXTERN_TEMPLATE|_LIBCPP_HAS_NO_EXTERN_TEMPLATES|"
    r"(?:extern\s+)?template\s+(?:class|struct)\s+.*?basic_(?:i|o|io)?stream|"
    r"basic_(?:i|o|io)?stream(?:buf)?\s*<"
)

STATIC = re.compile(
    r"libc\+\+\.a|(?:-Bstatic|-static).*?(?:-lc\+\+|libc\+\+)|"
    r"(?:-lc\+\+|libc\+\+).*?(?:-Bstatic|-static)"
)
LTO = re.compile(
    r"(?:^|[\s=\"'])-flto(?:[=\s\"']|$)|"
    r"(?:CMAKE_)?INTERPROCEDURAL_OPTIMIZATION\s+(?:TRUE|ON|1)|"
    r"\bb_lto\s*[:=]\s*true\b|\buse_lto\s*[:=]\s*(?:1|true)\b",
    re.I,
)
EXTERN_OFF = re.compile(r"_LIBCPP_DISABLE_EXTERN_TEMPLATE|_LIBCPP_HAS_NO_EXTERN_TEMPLATES")
EXPLICIT = re.compile(
    r"\btemplate\s+(?:class|struct)\s+[^;]*?std(?:::(?:__1))?::basic_"
    r"(?P<kind>istream|ostream|iostream)\s*<\s*(?P<char>char|wchar_t)\b"
)
STREAM = re.compile(r"basic_(?:i|o|io)?stream(?:buf)?\s*<\s*(?P<char>[^,>]+)")

STANDARD_CHAR = {"char", "wchar_t", "std::char_traits<char", "std::char_traits<wchar_t"}
GENERIC_CHAR = {
    "chart", "char_t", "_chart", "_char_t", "char_type", "_char_type",
    "c", "_c", "t", "_t", "typename", "class",
}

BUILD_NAMES = {
    "cmakelists.txt", "meson.build", "meson_options.txt", "makefile",
    "configure", "configure.ac", "configure.in", "sconstruct", "sconscript",
}

SITE_PATTERNS = {
    "S01": re.compile(r"operator\s*>>\s*\([^\n]*(?:streambuf|rdbuf)"),
    "S02": re.compile(r"\.get\s*\(\s*\)"),
    "S03_S04": re.compile(r"\.get\s*\("),
    "S05": re.compile(r"\.getline\s*\("),
    "S06": re.compile(r"\.ignore\s*\("),
    "S07": re.compile(r"\.peek\s*\("),
    "S08": re.compile(r"\.read\s*\("),
    "S09": re.compile(r"\.readsome\s*\("),
    "S10": re.compile(r"\.putback\s*\("),
    "S11": re.compile(r"\.unget\s*\("),
    "S12": re.compile(r"\.sync\s*\("),
    "S13": re.compile(r"\.tellg\s*\("),
    "S14_S15": re.compile(r"\.seekg\s*\("),
    "S16_S17": re.compile(r">>\s*[A-Za-z_(]"),
    "S33": re.compile(r"(?:basic_ostream|std\s*::\s*(?:cout|cerr|clog)|<<)"),
}


def write_tsv(path, header, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as handle:
        writer = csv.writer(handle, delimiter="\t", lineterminator="\n")
        writer.writerow(header)
        writer.writerows(rows)


def scope(path):
    lower = str(path).lower().replace("\\", "/")
    parts = lower.split("/")
    name = path.name.lower()
    if path.suffix.lower() in {".patch", ".diff"}:
        return "PATCH"
    if any(part in {"test", "tests", "testing", "unittest", "unittests"} for part in parts):
        return "TEST"
    if any(part in {"doc", "docs", "documentation", "examples", "example", "samples", "sample"} for part in parts):
        return "DOC_OR_EXAMPLE"
    if path.suffix.lower() == ".spec":
        return "SPEC"
    if name in BUILD_NAMES or path.suffix.lower() in {
        ".cmake", ".mk", ".am", ".ac", ".gyp", ".gypi", ".gn", ".ninja",
        ".bazel", ".bzl", ".pro", ".pri",
    }:
        return "BUILD"
    return "SOURCE"


def active_line(text):
    stripped = text.lstrip()
    return not (stripped.startswith("#") and not stripped.startswith("#define")) and not stripped.startswith("//")


def concrete_nonstandard_char(value):
    value = re.sub(r"\b(?:const|volatile|typename|class|struct)\b", "", value)
    value = re.sub(r"\s+", " ", value).strip()
    compact = value.replace(" ", "")
    if compact in {"char", "wchar_t"}:
        return False
    if compact.lower() in GENERIC_CHAR or compact.startswith("_"):
        return False
    if compact in {"char8_t", "char16_t", "char32_t", "signedchar", "unsignedchar"}:
        return True
    if "::" in compact and not compact.startswith("std::char_traits"):
        return True
    if re.fullmatch(r"[A-Z][A-Za-z0-9_]*", compact):
        return True
    return False


def run_rg(root):
    cmd = ["rg", "--threads", "2", "--json", "--line-number", "--hidden", "--no-ignore", "--pcre2", "-e", COMBINED.pattern]
    for glob in CODE_GLOBS:
        cmd += ["-g", glob]
    for glob in EXCLUDED_GLOBS:
        cmd += ["-g", glob]
    cmd.append(str(root))
    result = subprocess.run(cmd, text=True, capture_output=True)
    if result.returncode not in (0, 1):
        raise RuntimeError(f"rg failed for {root}: rc={result.returncode}: {result.stderr}")
    import json
    for raw in result.stdout.splitlines():
        obj = json.loads(raw)
        if obj.get("type") != "match":
            continue
        data = obj["data"]
        yield Path(data["path"]["text"]), int(data["line_number"]), data["lines"]["text"].rstrip("\n")


def facility_sites(root):
    pattern = "|".join(f"(?:{rx.pattern})" for rx in SITE_PATTERNS.values())
    cmd = ["rg", "--threads", "1", "--json", "--line-number", "--hidden", "--no-ignore", "--pcre2", "-e", pattern]
    for glob in CODE_GLOBS[:16]:
        cmd += ["-g", glob]
    for glob in EXCLUDED_GLOBS:
        cmd += ["-g", glob]
    cmd.append(str(root))
    result = subprocess.run(cmd, text=True, capture_output=True)
    if result.returncode not in (0, 1):
        return {}, f"RG_ERROR_{result.returncode}"
    import json
    sites = defaultdict(list)
    for raw in result.stdout.splitlines():
        obj = json.loads(raw)
        if obj.get("type") != "match":
            continue
        data = obj["data"]
        path = Path(data["path"]["text"])
        if scope(path) in {"TEST", "DOC_OR_EXAMPLE", "PATCH"}:
            continue
        text = data["lines"]["text"].rstrip("\n")
        for site, rx in SITE_PATTERNS.items():
            if rx.search(text):
                sites[site].append((path, int(data["line_number"]), text))
    return sites, "OK"


def main():
    with STATUS.open(newline="") as handle:
        status_rows = list(csv.DictReader(handle, delimiter="\t"))
    rows = [row for row in status_rows if row["status"] == "SCAN_OK"]
    if len(rows) != 371:
        raise SystemExit(f"expected 371 SCAN_OK packages, got {len(rows)}")

    raw_hits = []
    confirmed = []
    package_conditions = defaultdict(set)
    package_identity = {}

    for row in rows:
        srpm = row["source_rpm"]
        package = row["package_name"]
        root = CORPUS / srpm
        package_identity[srpm] = package
        if not root.is_dir():
            raw_hits.append((srpm, package, "INPUT_MISSING", "NOT_AVAILABLE", str(root), 0, ""))

    known_srpms = set(package_identity)
    print("global rg scan started (371 extracted SRPM roots, 2 threads)", flush=True)
    for match_index, (path, line, text) in enumerate(run_rg(CORPUS), 1):
        corpus_rel = path.relative_to(CORPUS)
        if not corpus_rel.parts:
            continue
        srpm = corpus_rel.parts[0]
        if srpm not in known_srpms:
            continue
        package = package_identity[srpm]
        rel = Path(*corpus_rel.parts[1:])
        item_scope = scope(rel)
        kinds = []
        if STATIC.search(text):
            kinds.append(("STATIC_LIBCXX", "CONFIRMED" if item_scope in {"SPEC", "BUILD"} and active_line(text) else "CANDIDATE"))
        if LTO.search(text):
            kinds.append(("LTO", "CONFIRMED" if item_scope in {"SPEC", "BUILD"} and active_line(text) else "CANDIDATE"))
        if EXTERN_OFF.search(text):
            kinds.append(("EXTERN_TEMPLATE_DISABLED", "CONFIRMED" if item_scope in {"SPEC", "BUILD", "SOURCE"} and active_line(text) else "CANDIDATE"))
        explicit = EXPLICIT.search(text)
        if explicit and not re.search(r"\bextern\s+template\b", text):
            verdict = "PROVIDER_SOURCE" if package in {"gcc", "llvm"} else ("CONFIRMED" if item_scope == "SOURCE" else "CANDIDATE")
            kinds.append(("SAME_SYMBOL_DEFINITION", verdict))
        for match in STREAM.finditer(text):
            char_type = match.group("char").strip()
            if concrete_nonstandard_char(char_type):
                verdict = "PROVIDER_SOURCE" if package in {"gcc", "llvm"} else ("CONFIRMED" if item_scope == "SOURCE" else "CANDIDATE")
                kinds.append((f"NONSTANDARD_CHAR:{char_type}", verdict))
        for kind, verdict in kinds:
            raw_hits.append((srpm, package, kind, verdict, str(rel), line, text.strip()))
            if verdict == "CONFIRMED":
                base_kind = kind.split(":", 1)[0]
                package_conditions[srpm].add(base_kind)
                confirmed.append((srpm, package, base_kind, str(rel), line, text.strip()))
        if match_index % 10000 == 0:
            print(f"matched lines processed={match_index}", flush=True)

    facility_rows = []
    impacted_sites = set()
    for srpm in sorted(package_conditions):
        package = package_identity[srpm]
        conditions = sorted(package_conditions[srpm])
        sites, scan_status = facility_sites(CORPUS / srpm)
        for site, evidence in sorted(sites.items()):
            impacted_sites.update(site.split("_"))
            first_path, first_line, first_text = evidence[0]
            facility_rows.append((srpm, package, ",".join(conditions), site, len(evidence), str(first_path.relative_to(CORPUS / srpm)), first_line, first_text.strip(), scan_status))
        if not sites:
            facility_rows.append((srpm, package, ",".join(conditions), "NOT_OBSERVED", 0, "", 0, "", scan_status))

    all_package_rows = []
    for row in rows:
        srpm = row["source_rpm"]
        package = row["package_name"]
        conditions = package_conditions.get(srpm, set())
        all_package_rows.append((srpm, package) + tuple("CONFIRMED" if kind in conditions else "NOT_OBSERVED" for kind in (
            "STATIC_LIBCXX", "LTO", "SAME_SYMBOL_DEFINITION", "EXTERN_TEMPLATE_DISABLED", "NONSTANDARD_CHAR"
        )))

    write_tsv(OUT / "raw_hits.tsv", ["source_rpm", "package", "kind", "verdict", "file", "line", "text"], raw_hits)
    write_tsv(OUT / "confirmed_hits.tsv", ["source_rpm", "package", "kind", "file", "line", "text"], confirmed)
    write_tsv(OUT / "package_audit.tsv", ["source_rpm", "package", "static_libcxx", "lto", "same_symbol_definition", "extern_template_disabled", "nonstandard_char"], all_package_rows)
    write_tsv(OUT / "facility_site_evidence.tsv", ["source_rpm", "package", "conditions", "site_or_group", "hit_count", "first_file", "first_line", "first_text", "scan_status"], facility_rows)

    confirmed_counts = Counter(row[2] for row in confirmed)
    unique_packages = {package_identity[srpm] for srpm in package_conditions}
    summary = [
        ("denominator_srpm_identities", 371),
        ("denominator_unique_package_names", len({row["package_name"] for row in rows})),
        ("confirmed_exception_srpm_identities", len(package_conditions)),
        ("confirmed_exception_unique_package_names", len(unique_packages)),
    ]
    for kind in ("STATIC_LIBCXX", "LTO", "SAME_SYMBOL_DEFINITION", "EXTERN_TEMPLATE_DISABLED", "NONSTANDARD_CHAR"):
        srpms = {row[0] for row in confirmed if row[2] == kind}
        names = {row[1] for row in confirmed if row[2] == kind}
        summary.append((f"{kind.lower()}_srpm_identities", len(srpms)))
        summary.append((f"{kind.lower()}_unique_package_names", len(names)))
        summary.append((f"{kind.lower()}_evidence_lines", confirmed_counts[kind]))
    write_tsv(OUT / "summary.tsv", ["metric", "value"], summary)
    print(f"confirmed_srpms={len(package_conditions)} unique_names={len(unique_packages)} raw_hits={len(raw_hits)} confirmed_lines={len(confirmed)}")


if __name__ == "__main__":
    main()
