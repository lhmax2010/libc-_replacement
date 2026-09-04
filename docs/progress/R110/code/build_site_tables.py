#!/usr/bin/env python3
import csv
import hashlib
import io
import json
import re
import subprocess
import tarfile
from collections import Counter, defaultdict
from pathlib import Path


ROOT = Path("/home/toolchain/development/libc++_replacement")
R32_TABLE = ROOT / "docs/progress/R32/tables/forced_unwind_guard_locations.tsv"
R32_CORPUS = ROOT / "tmp/corpus/extracted"
OUT = ROOT / "docs/progress/R110"
TABLES = OUT / "tables"
WORK = ROOT / "progress/R110/context_sources"

CLAUSE_RX = re.compile(
    r"(?P<clause>(?:\bcatch|\b__catch)\s*\(\s*(?:const\s+)?"
    r"(?:abi|__cxxabiv1)\s*::\s*__forced_unwind\s*(?:const\s*)?&\s*\))",
    re.MULTILINE,
)
NEXT_CATCH_RX = re.compile(r"(?:\bcatch|\b__catch)\s*\(")
LITERAL_RETHROW_RX = re.compile(r"\bthrow\s*;")
MACRO_RETHROW_RX = re.compile(r"\b__throw_exception_again\s*;")

CURRENT = {
    "boost": {
        "repo": ROOT / "codes/R104/boost",
        "url": "git://review.tizen.org/git/platform/upstream/boost",
        "branch": "tizen_base",
        "commit": "69203710b8582c489f420ef53a769cc6077c250b",
        "archives": [],
    },
    "gcc": {
        "repo": ROOT / "codes/gcc",
        "url": "git://review.tizen.org/git/platform/upstream/gcc",
        "branch": "tizen_base",
        "commit": "49c5060508ce8a6ed6beca375233688a6bdd9140",
        "archives": [],
    },
    "key-manager": {
        "repo": ROOT / "progress/R110/current_git/key-manager",
        "url": "git://review.tizen.org/git/platform/core/security/key-manager",
        "branch": "tizen",
        "commit": "5be2dc74baede39694fd10d8f72b95f759e799ba",
        "archives": [],
    },
    "libcynara-commons": {
        "repo": ROOT / "progress/R110/current_git/libcynara-commons",
        "url": "git://review.tizen.org/git/platform/core/security/libcynara-commons",
        "branch": "tizen",
        "commit": "1ee5fcdd9529e5b2260b1c365c28a5c56b3adbc1",
        "archives": [],
    },
    "llvm": {
        "repo": ROOT / "codes/R104/llvm",
        "url": "git://review.tizen.org/git/platform/upstream/llvm",
        "branch": "tizen_base",
        "commit": "2d23367d74afbf2bb1e9e4013fce072b3a154109",
        "archives": [
            "packaging/mlgo_arm_model.tar.gz",
            "packaging/mlgo_aarch_model.tar.gz",
            "packaging/mlgo_x86_model.tar.gz",
        ],
    },
    "security-manager": {
        "repo": ROOT / "progress/R110/current_git/security-manager",
        "url": "git://review.tizen.org/git/platform/core/security/security-manager",
        "branch": "tizen",
        "commit": "90010ec2417fd8ee3c7c71ffbf929b219481bc21",
        "archives": [],
    },
    "tensorflow2": {
        "repo": ROOT / "codes/R104/tensorflow2",
        "url": "git://review.tizen.org/git/platform/upstream/tensorflow2",
        "branch": "accepted/tizen_base_toolchain",
        "commit": "1cdba73549f741720f11dd53da1bb516b03763a2",
        "archives": ["packaging/tensorflow2-aot.tar.gz"],
    },
    "webauthn": {
        "repo": ROOT / "progress/R110/current_git/webauthn",
        "url": "git://review.tizen.org/git/platform/core/security/webauthn",
        "branch": "tizen",
        "commit": "88d99a5d655c644cbd384f8e15cca337d1a00b21",
        "archives": [],
    },
}

MAIN_ARCHIVE_ROOTS = {
    "boost_1_83_0.tar.bz2": "boost_1.83.0",
    "gcc-14.2.0.tar.bz2": "gcc-14.2.0",
    "key-manager-0.1.90.tar.gz": "key-manager-0.1.90",
    "cynara-0.26.0.tar.gz": "cynara-0.26.0",
    "security-manager-2.4.0.tar.gz": "security-manager-2.4.0",
    "webauthn-1.0.13.tar.gz": "webauthn-1.0.13",
    "dali2-csharp-binder-2.5.26.tar.xz": "dali2-csharp-binder-2.5.26",
}


def run(args, cwd=None, text=True):
    cp = subprocess.run(args, cwd=cwd, capture_output=True, text=text)
    if cp.returncode != 0:
        raise RuntimeError(f"command failed rc={cp.returncode}: {args!r}\n{cp.stderr}")
    return cp.stdout


def original_display_path(raw_path):
    parts = raw_path.split("/")
    assert parts[0] == "unpacked" and len(parts) >= 3, raw_path
    archive = parts[1]
    rest = parts[2:]
    if archive in MAIN_ARCHIVE_ROOTS:
        assert rest[0] == MAIN_ARCHIVE_ROOTS[archive], (archive, rest[0])
        return "/".join(rest[1:])
    return f"packaging/{archive}!/" + "/".join(rest)


def source_line(text, line):
    value = text.splitlines()[line - 1]
    return value.replace("\t", "\\t")


def rethrow_kind(text, match):
    start = text.find("{", match.end())
    if start < 0:
        segment = text[match.end():match.end() + 8000]
    else:
        depth = 0
        state = "code"
        end = min(len(text), start + 50000)
        pos = start
        while pos < end:
            ch = text[pos]
            nxt = text[pos + 1] if pos + 1 < end else ""
            if state == "code":
                if ch == '"':
                    state = "string"
                elif ch == "'":
                    state = "char"
                elif ch == "/" and nxt == "/":
                    state = "line_comment"
                    pos += 1
                elif ch == "/" and nxt == "*":
                    state = "block_comment"
                    pos += 1
                elif ch == "{":
                    depth += 1
                elif ch == "}":
                    depth -= 1
                    if depth == 0:
                        pos += 1
                        break
            elif state in {"string", "char"}:
                if ch == "\\":
                    pos += 1
                elif (state == "string" and ch == '"') or (state == "char" and ch == "'"):
                    state = "code"
            elif state == "line_comment":
                if ch == "\n":
                    state = "code"
            elif state == "block_comment" and ch == "*" and nxt == "/":
                state = "code"
                pos += 1
            pos += 1
        segment = text[start:pos]
    if LITERAL_RETHROW_RX.search(segment):
        return "LITERAL_THROW"
    if MACRO_RETHROW_RX.search(segment):
        return "LIBSTDCXX_RETHROW_MACRO"
    return "NO_RETHROW_OBSERVED"


def macro_context(lines, line):
    idx = line - 1
    for pos in range(idx, max(-1, idx - 80), -1):
        match = re.match(r"\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)", lines[pos])
        if match:
            if all(lines[n].rstrip().endswith("\\") for n in range(pos, idx)):
                return f"macro {match.group(1)} (line {pos + 1})"
            break
        if pos < idx and not lines[pos].rstrip().endswith("\\"):
            break
    return ""


_ctags_cache = {}


def context_for(text, label, line):
    key = hashlib.sha256((label + "\0" + text).encode("utf-8", "replace")).hexdigest()
    if key not in _ctags_cache:
        suffix = Path(label.split("!/")[-1]).suffix or ".hpp"
        WORK.mkdir(parents=True, exist_ok=True)
        temp = WORK / f"{key}{suffix}"
        temp.write_text(text, encoding="utf-8")
        cp = subprocess.run(
            [
                "ctags",
                "--output-format=json",
                "--fields=+nKsS",
                "--c++-kinds=f",
                "--language-force=C++",
                "-o",
                "-",
                str(temp),
            ],
            capture_output=True,
            text=True,
        )
        tags = []
        if cp.returncode == 0:
            for raw in cp.stdout.splitlines():
                try:
                    obj = json.loads(raw)
                except json.JSONDecodeError:
                    continue
                if obj.get("_type") == "tag" and isinstance(obj.get("line"), int):
                    tags.append(obj)
        _ctags_cache[key] = tags
    lines = text.splitlines()
    macro = macro_context(lines, line)
    if macro:
        return macro
    eligible = [tag for tag in _ctags_cache[key] if tag["line"] <= line]
    if not eligible:
        return "NOT_OBSERVED"
    tag = max(eligible, key=lambda item: item["line"])
    scope = tag.get("scope", "")
    name = tag.get("name", "")
    signature = tag.get("signature", "")
    if name.startswith("__anon"):
        outer = [item for item in eligible if not item.get("name", "").startswith("__anon")]
        outer_text = ""
        if outer:
            enclosing = max(outer, key=lambda item: item["line"])
            outer_name = "::".join(filter(None, [enclosing.get("scope", ""), enclosing.get("name", "")]))
            outer_text = f" in {outer_name}{enclosing.get('signature', '')}"
        return f"lambda (line {tag['line']}){outer_text}"
    qualified = "::".join(filter(None, [scope, name]))
    return f"{qualified}{signature} (line {tag['line']})"


def scan_text(package, source_instance, path, text, source_kind):
    rows = []
    for match in CLAUSE_RX.finditer(text):
        line = text.count("\n", 0, match.start()) + 1
        clause = " ".join(match.group("clause").split())
        rows.append(
            {
                "source_instance": source_instance,
                "package_name": package,
                "file": path,
                "line": str(line),
                "actual_code_line": source_line(text, line).strip(),
                "clause_text": clause,
                "context": context_for(text, f"{source_instance}:{path}", line),
                "rethrow_kind": rethrow_kind(text, match),
                "source_kind": source_kind,
            }
        )
    return rows


def r32_scope(raw_path):
    value = raw_path.lower().replace("\\", "/")
    parts = value.split("/")
    if any(item in parts for item in ("test", "tests", "testing", "unittest", "unittests")) or re.search(r"(^|/)(test|tests?)[_.-]", value):
        return "TEST"
    if any(item in parts for item in ("doc", "docs", "documentation")) or value.endswith((".md", ".rst", ".txt")):
        return "DOC"
    if any(item in parts for item in ("example", "examples", "sample", "samples")):
        return "EXAMPLE"
    if value.endswith((".spec", ".cmake")) or Path(value).name in ("meson.build", "configure.ac", "makefile"):
        return "BUILD"
    return "PRODUCTION_LIKE"


def read_original_rows():
    with R32_TABLE.open(encoding="utf-8", newline="") as stream:
        locations = list(csv.DictReader(stream, delimiter="\t"))

    reported = [
        row for row in locations
        if row["guard_status"] == "CONFIRMED_RETHROW" and row["scope"] != "DOC"
    ]
    assert len(reported) == 50

    reported_audit = []
    for row in reported:
        actual_path = R32_CORPUS / row["source_rpm"] / row["file"]
        text = actual_path.read_text(encoding="utf-8", errors="replace")
        raw_actual = text.splitlines()[int(row["line"]) - 1]
        actual = raw_actual.replace("\t", "\\t")
        assert raw_actual.strip() == row["source_text"].strip(), (actual_path, row["line"])
        is_clause = bool(CLAUSE_RX.search(raw_actual))
        reported_audit.append(
            {
                "source_rpm": row["source_rpm"],
                "package_name": row["package_name"],
                "file": original_display_path(row["file"]),
                "line": row["line"],
                "actual_code_line": actual.strip(),
                "context": context_for(text, f"original:{row['source_rpm']}:{row['file']}", int(row["line"])),
                "row_nature": "ACTUAL_CATCH_CLAUSE" if is_clause else "NON_CLAUSE_TOKEN_LINE",
                "r32_status": row["guard_status"],
            }
        )

    strict = []
    source_packages = {}
    for row in locations:
        if CLAUSE_RX.search(row["source_text"]):
            source_packages[row["source_rpm"]] = row["package_name"]
    assert len(source_packages) == 11
    for source_rpm, package in sorted(source_packages.items()):
        source_root = R32_CORPUS / source_rpm
        cp = subprocess.run(
            ["rg", "-l", "-I", "--hidden", "--no-ignore", "--fixed-strings", "__forced_unwind", str(source_root)],
            capture_output=True,
            text=True,
        )
        if cp.returncode not in (0, 1):
            raise RuntimeError(f"rg failed for {source_rpm}: {cp.stderr}")
        for raw in cp.stdout.splitlines():
            actual_path = Path(raw)
            relative = str(actual_path.relative_to(source_root))
            if r32_scope(relative) == "DOC":
                continue
            text = actual_path.read_text(encoding="utf-8", errors="replace")
            strict.extend(
                scan_text(
                    package,
                    source_rpm,
                    original_display_path(relative),
                    text,
                    "R32_FROZEN_SOURCE_RPM_COMPREHENSIVE_TEXT_RESCAN",
                )
            )
    strict.sort(key=lambda row: (row["source_instance"], row["file"], int(row["line"])))
    return locations, reported_audit, strict


def git_blob(repo, commit, path):
    return run(["git", "show", f"{commit}:{path}"], cwd=repo)


def current_rows():
    identities = []
    rows = []
    for package, info in CURRENT.items():
        repo = info["repo"]
        commit = info["commit"]
        observed = run(["git", "rev-parse", commit], cwd=repo).strip()
        assert observed == commit
        identities.append(
            {
                "package_name": package,
                "repository_url": info["url"],
                "branch": info["branch"],
                "head": commit,
                "source_scope": "tracked Git text plus listed embedded source archives",
            }
        )
        cp = subprocess.run(
            ["git", "grep", "-l", "-I", "-e", "__forced_unwind", commit, "--"],
            cwd=repo,
            capture_output=True,
            text=True,
        )
        if cp.returncode not in (0, 1):
            raise RuntimeError(f"git grep failed for {package}: {cp.stderr}")
        prefix = commit + ":"
        for raw in cp.stdout.splitlines():
            path = raw[len(prefix):] if raw.startswith(prefix) else raw
            text = git_blob(repo, commit, path)
            rows.extend(scan_text(package, commit, path, text, "CURRENT_GIT_TRACKED_TEXT"))

        for archive_path in info["archives"]:
            blob = subprocess.run(
                ["git", "show", f"{commit}:{archive_path}"],
                cwd=repo,
                capture_output=True,
            )
            if blob.returncode != 0:
                raise RuntimeError(f"cannot read {package}:{archive_path}: {blob.stderr!r}")
            with tarfile.open(fileobj=io.BytesIO(blob.stdout), mode="r:*") as archive:
                for member in archive:
                    if not member.isfile():
                        continue
                    extracted = archive.extractfile(member)
                    if extracted is None:
                        continue
                    data = extracted.read()
                    if b"__forced_unwind" not in data:
                        continue
                    text = data.decode("utf-8", "replace")
                    member_name = member.name[2:] if member.name.startswith("./") else member.name
                    display = f"{archive_path}!/{member_name}"
                    rows.extend(scan_text(package, commit, display, text, "CURRENT_EMBEDDED_ARCHIVE_TEXT"))

    rows.sort(key=lambda row: (row["package_name"], row["file"], int(row["line"])))
    return identities, rows


def write_tsv(path, rows, fields):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def md(value):
    return str(value).replace("|", "\\|").replace("\n", "<br>")


def write_grouped_markdown(path, title, intro, rows, instance_field):
    grouped = defaultdict(list)
    for row in rows:
        grouped[row["package_name"]].append(row)
    with path.open("w", encoding="utf-8") as stream:
        stream.write(f"# {title}\n\n{intro}\n\n")
        for package in sorted(grouped):
            stream.write(f"## `{package}`\n\n")
            stream.write("| 源码实例 | 文件与行号 | 实际代码原文 | 所在函数或上下文 | 核对结果 |\n")
            stream.write("|---|---|---|---|---|\n")
            for row in grouped[package]:
                nature = row.get("row_nature", row.get("rethrow_kind", ""))
                stream.write(
                    f"| `{md(row[instance_field])}` | `{md(row['file'])}:{row['line']}` | "
                    f"`{md(row['actual_code_line'])}` | {md(row['context'])} | `{md(nature)}` |\n"
                )
            stream.write("\n")


def main():
    _, reported, original_strict = read_original_rows()
    identities, current = current_rows()

    original_rethrow = [row for row in original_strict if row["rethrow_kind"] != "NO_RETHROW_OBSERVED"]
    current_rethrow = [row for row in current if row["rethrow_kind"] != "NO_RETHROW_OBSERVED"]

    assert len(reported) == 50
    assert Counter(row["row_nature"] for row in reported) == {
        "ACTUAL_CATCH_CLAUSE": 46,
        "NON_CLAUSE_TOKEN_LINE": 4,
    }
    assert len(original_strict) == 175
    assert Counter(row["rethrow_kind"] for row in original_strict) == {
        "LITERAL_THROW": 46,
        "LIBSTDCXX_RETHROW_MACRO": 126,
        "NO_RETHROW_OBSERVED": 3,
    }
    assert len(original_rethrow) == 172

    # De-duplicate source-RPM variants of the same package by logical path,
    # line and exact clause/rethrow form. This intentionally retains separate
    # paths for architecture-specific embedded archives.
    def logical_key(row):
        return (
            row["package_name"], row["file"], row["line"], row["clause_text"], row["rethrow_kind"]
        )

    original_unique = {logical_key(row): row for row in original_rethrow}
    assert len(original_unique) == 104
    assert len(current_rethrow) == 140

    reported_fields = [
        "source_rpm", "package_name", "file", "line", "actual_code_line",
        "context", "row_nature", "r32_status",
    ]
    site_fields = [
        "source_instance", "package_name", "file", "line", "actual_code_line",
        "clause_text", "context", "rethrow_kind", "source_kind",
    ]
    write_tsv(TABLES / "reported_50_audit.tsv", reported, reported_fields)
    write_tsv(TABLES / "strict_clause_sites_original.tsv", original_strict, site_fields)
    write_tsv(TABLES / "strict_rethrow_sites_original.tsv", original_rethrow, site_fields)
    write_tsv(TABLES / "current_rethrow_sites.tsv", current_rethrow, site_fields)
    write_tsv(TABLES / "current_source_identity.tsv", identities, list(identities[0]))

    packages = sorted(set(row["package_name"] for row in original_strict) | set(CURRENT))
    summary = []
    for package in packages:
        original_instances = {row["source_instance"] for row in original_strict if row["package_name"] == package}
        original_rows = [row for row in original_strict if row["package_name"] == package]
        original_protective = [row for row in original_rethrow if row["package_name"] == package]
        original_unique_n = len({logical_key(row) for row in original_protective})
        current_package = [row for row in current_rethrow if row["package_name"] == package]
        summary.append(
            {
                "package_name": package,
                "r32_source_rpm_instances": str(len(original_instances)),
                "reported_50_rows": str(sum(1 for row in reported if row["package_name"] == package)),
                "reported_rows_that_are_clauses": str(sum(1 for row in reported if row["package_name"] == package and row["row_nature"] == "ACTUAL_CATCH_CLAUSE")),
                "strict_clause_occurrences_r32": str(len(original_rows)),
                "strict_rethrow_occurrences_r32": str(len(original_protective)),
                "strict_rethrow_unique_package_path_r32": str(original_unique_n),
                "current_head_rethrow_occurrences": str(len(current_package)) if package in CURRENT else "NOT_OBSERVED",
            }
        )
    summary_fields = list(summary[0])
    write_tsv(TABLES / "package_summary.tsv", summary, summary_fields)

    duplicate_groups = defaultdict(list)
    for row in original_rethrow:
        duplicate_groups[logical_key(row)].append(row["source_instance"])
    dedup_rows = []
    for key, instances in sorted(duplicate_groups.items()):
        if len(instances) > 1:
            dedup_rows.append(
                {
                    "package_name": key[0],
                    "file": key[1],
                    "line": key[2],
                    "rethrow_kind": key[4],
                    "source_rpm_instances": ";".join(sorted(instances)),
                    "multiplicity": str(len(instances)),
                }
            )
    write_tsv(
        TABLES / "source_instance_duplicates.tsv",
        dedup_rows,
        ["package_name", "file", "line", "rethrow_kind", "source_rpm_instances", "multiplicity"],
    )
    assert len(dedup_rows) == 68

    write_grouped_markdown(
        OUT / "REPORTED_50_AUDIT.md",
        "历史“50 个处理位置”的逐行核对",
        "“历史 50 行”按当时表中 `CONFIRMED_RETHROW` 且非文档路径筛出。"
        "下表逐行保留源码 RPM、包内路径、行号和代码原文；核对结果明确标出该行"
        "是否真的是强制展开 catch 子句。这里的“位置”以源码 RPM 实例、文件和行号共同标识。",
        reported,
        "source_rpm",
    )
    write_grouped_markdown(
        OUT / "STRICT_RETHROW_SITE_LIST.md",
        "强制展开识别并继续传播的严格站点清单",
        "严格站点必须是源码中的 `catch`/`__catch` 子句，捕获 "
        "`abi::__forced_unwind` 或 `__cxxabiv1::__forced_unwind`，并在该 handler 中通过 "
        "`throw;` 或 libstdc++ 的 `__throw_exception_again` 继续传播。下表使用历史冻结"
        "源码 RPM 实例，因此同一 Git 内容若作为两个源码 RPM 出现，会各列一遍。",
        original_rethrow,
        "source_instance",
    )
    write_grouped_markdown(
        OUT / "CURRENT_RETHROW_SITE_LIST.md",
        "当前开发分支中的强制展开识别与传播站点",
        "本表只复核历史阳性包名集合的当前开发分支；Git 跟踪文本与明确列出的"
        "MLGO/TensorFlow 嵌套源码资产均参与。它用于核对已有站点漂移，不能证明"
        "平台其他源码包没有新增站点。",
        current_rethrow,
        "source_instance",
    )
    for markdown_name in (
        "REPORTED_50_AUDIT.md",
        "STRICT_RETHROW_SITE_LIST.md",
        "CURRENT_RETHROW_SITE_LIST.md",
    ):
        markdown_path = OUT / markdown_name
        markdown_path.write_text(
            markdown_path.read_text(encoding="utf-8").rstrip() + "\n",
            encoding="utf-8",
        )

    metrics = [
        ("r32_forced_token_rows", 157),
        ("historical_reported_rows", len(reported)),
        ("historical_reported_actual_clauses", 46),
        ("historical_reported_non_clause_token_lines", 4),
        ("strict_clause_occurrences_r32", len(original_strict)),
        ("strict_literal_rethrow_occurrences_r32", 46),
        ("strict_libstdcxx_macro_rethrow_occurrences_r32", 126),
        ("strict_no_rethrow_occurrences_r32", 3),
        ("strict_rethrow_occurrences_r32", len(original_rethrow)),
        ("strict_rethrow_unique_package_path_r32", len(original_unique)),
        ("current_reported_package_set_rethrow_occurrences", len(current_rethrow)),
    ]
    with (TABLES / "audit_metrics.tsv").open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(["metric", "value"])
        writer.writerows(metrics)

    print("reported_50=50 actual_clause_rows=46 non_clause_rows=4")
    print("strict_r32_clauses=175 literal_rethrow=46 macro_rethrow=126 no_rethrow=3")
    print("strict_r32_rethrow=172 unique_package_path=104")
    print(f"current_positive_package_set_rethrow={len(current_rethrow)}")
    print("current_by_package=" + ";".join(f"{k}={v}" for k, v in sorted(Counter(row['package_name'] for row in current_rethrow).items())))


if __name__ == "__main__":
    main()
