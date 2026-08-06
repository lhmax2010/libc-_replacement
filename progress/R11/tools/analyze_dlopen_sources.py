#!/usr/bin/env python3
import argparse
import collections
import csv
import hashlib
import os
import pathlib
import re
import shlex
import subprocess
import tarfile
import zipfile


TEXT_EXTENSIONS = {
    ".c", ".cc", ".cpp", ".cxx", ".c++", ".h", ".hh", ".hpp", ".hxx", ".inc",
    ".m", ".mm", ".S", ".s", ".cmake", ".py", ".sh", ".spec", ".patch", ".diff",
}
ARCHIVE_SUFFIXES = (".tar", ".tar.gz", ".tgz", ".tar.bz2", ".tbz2", ".tar.xz", ".txz", ".zip")
TOKEN_RE = re.compile(r"\bdlopen\b|\bRTLD_LOCAL\b|\bRTLD_GLOBAL\b")


def read_tsv(path):
    with open(path, encoding="utf-8") as stream: return list(csv.DictReader(stream, delimiter="\t"))


def safe_name(name):
    candidate = pathlib.PurePosixPath(name)
    return not candidate.is_absolute() and ".." not in candidate.parts


def expand_archive(path, destination):
    destination.mkdir(parents=True, exist_ok=False)
    lower = path.name.lower()
    if lower.endswith(".zip"):
        with zipfile.ZipFile(path) as archive:
            members = archive.infolist()
            unsafe = [item.filename for item in members if not safe_name(item.filename)]
            if unsafe: return "UNSAFE_ARCHIVE_MEMBER", len(unsafe)
            for item in members:
                mode = (item.external_attr >> 16) & 0o170000
                if mode == 0o120000: continue
                archive.extract(item, destination)
        return "PASS", 0
    try:
        with tarfile.open(path, "r:*") as archive:
            safe = [item for item in archive.getmembers() if safe_name(item.name) and (item.isfile() or item.isdir())]
            unsafe_count = len(archive.getmembers()) - len(safe)
            archive.extractall(destination, members=safe)
        return "PASS_SKIPPED_LINK_OR_UNSAFE_MEMBERS" if unsafe_count else "PASS", unsafe_count
    except (tarfile.TarError, OSError) as error:
        return f"ARCHIVE_EXTRACT_FAILED:{error}", 0


def write(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader(); writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--elf-inventory", required=True)
    ap.add_argument("--source-download-status", required=True)
    ap.add_argument("--output-root", required=True)
    ap.add_argument("--work-root", required=True)
    ap.add_argument("--ledger", required=True)
    args = ap.parse_args()
    elfs = read_tsv(args.elf_inventory)
    source_status = read_tsv(args.source_download_status)
    dlopen_elfs = [row for row in elfs if row["runtime_elf"] == "YES" and row["dlopen_und"] == "YES"]
    targets = {row["sourcerpm"] for row in dlopen_elfs}
    source_by_filename = {pathlib.Path(row["location"]).name: row for row in source_status}
    root = pathlib.Path(args.work_root)
    root.mkdir(parents=True, exist_ok=False)
    output = pathlib.Path(args.output_root)
    commands, status_rows, archive_rows, hit_rows, call_rows = [], [], [], [], []
    for index, sourcerpm in enumerate(sorted(targets), 1):
        source = source_by_filename.get(sourcerpm)
        related_elfs = [row for row in dlopen_elfs if row["sourcerpm"] == sourcerpm]
        related_packages = {
            (row["repo_id"], row["name"], row["arch"], row["epoch"], row["version"], row["release"])
            for row in related_elfs
        }
        base = {
            "sourcerpm": sourcerpm, "dlopen_runtime_elf_count": len(related_elfs),
            "dlopen_binary_package_count": len(related_packages),
        }
        if source is None:
            status_rows.append({**base, "source_download_result": "NOT_FOUND_IN_FROZEN_SOURCE_REPODATA", "srpm_extract_result": "NOT_RUN", "text_file_count": 0, "hit_count": 0})
            continue
        if source["result"] != "PASS":
            status_rows.append({**base, "source_download_result": source["result"], "srpm_extract_result": "NOT_RUN", "text_file_count": 0, "hit_count": 0})
            continue
        package_root = root / f"{index:04d}_{source['checksum']}"
        payload = package_root / "payload"
        expanded = package_root / "expanded"
        payload.mkdir(parents=True, exist_ok=False)
        expanded.mkdir(parents=True, exist_ok=False)
        rpm_path = pathlib.Path(source["target"]).resolve()
        cmd = ["bash", "-o", "pipefail", "-c", f"rpm2cpio {shlex.quote(str(rpm_path))} | cpio -idm --quiet"]
        proc = subprocess.run(cmd, cwd=payload, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        commands.append({
            "label": f"extract_source_rpm:{sourcerpm}", "pwd": str(payload), "command": shlex.join(cmd),
            "exit_code": str(proc.returncode), "stderr": proc.stderr.replace("\t", " ").replace("\r", " ").replace("\n", "\\n"),
        })
        if proc.returncode != 0:
            status_rows.append({**base, "source_download_result": "PASS", "srpm_extract_result": "FAILED", "text_file_count": 0, "hit_count": 0})
            continue
        archive_index = 0
        for path in sorted(item for item in payload.rglob("*") if item.is_file() and not item.is_symlink()):
            if not path.name.lower().endswith(ARCHIVE_SUFFIXES): continue
            archive_index += 1
            destination = expanded / f"{archive_index:03d}_{path.name.replace('/', '_')}"
            result, skipped = expand_archive(path, destination)
            archive_rows.append({
                **base, "archive": str(path.relative_to(payload)), "result": result,
                "skipped_or_unsafe_member_count": skipped,
            })
        search_roots = [payload, expanded]
        seen_files = set()
        text_file_count = 0
        package_hits = 0
        for search_root in search_roots:
            for path in sorted(item for item in search_root.rglob("*") if item.is_file() and not item.is_symlink()):
                resolved = path.resolve()
                if resolved in seen_files: continue
                seen_files.add(resolved)
                if path.suffix not in TEXT_EXTENSIONS and path.name not in {"CMakeLists.txt", "configure", "meson.build", "Makefile"}: continue
                try:
                    data = path.read_bytes()
                except OSError:
                    continue
                if b"\0" in data: continue
                text_file_count += 1
                lines = data.decode("utf-8", "replace").splitlines()
                for lineno, line in enumerate(lines, 1):
                    tokens = sorted(set(TOKEN_RE.findall(line)))
                    if not tokens: continue
                    package_hits += 1
                    hit_rows.append({
                        **base, "file": str(path.relative_to(package_root)), "line": lineno,
                        "tokens": ";".join(tokens), "original": line.replace("\t", "\\t"),
                    })
                    if "dlopen" in tokens:
                        window = "\n".join(lines[max(0, lineno - 11): min(len(lines), lineno + 10)])
                        if "RTLD_LOCAL" in window and "RTLD_GLOBAL" in window: scope = "BOTH_LITERAL_SCOPES_WITHIN_PLUS_MINUS_10_LINES"
                        elif "RTLD_LOCAL" in window: scope = "RTLD_LOCAL_LITERAL_WITHIN_PLUS_MINUS_10_LINES"
                        elif "RTLD_GLOBAL" in window: scope = "RTLD_GLOBAL_LITERAL_WITHIN_PLUS_MINUS_10_LINES"
                        else: scope = "NO_LITERAL_SCOPE_WITHIN_PLUS_MINUS_10_LINES"
                        call_rows.append({
                            **base, "file": str(path.relative_to(package_root)), "line": lineno,
                            "scope_window_class": scope, "dlopen_line": line.replace("\t", "\\t"),
                        })
        status_rows.append({
            **base, "source_download_result": "PASS", "srpm_extract_result": "PASS",
            "text_file_count": text_file_count, "hit_count": package_hits,
        })
        print(f"PROGRESS={index}/{len(targets)} source={sourcerpm}", flush=True)
    write(output / "dlopen_source_analysis_status.tsv", [
        "sourcerpm", "dlopen_runtime_elf_count", "dlopen_binary_package_count", "source_download_result",
        "srpm_extract_result", "text_file_count", "hit_count",
    ], status_rows)
    write(output / "dlopen_source_archive_status.tsv", [
        "sourcerpm", "dlopen_runtime_elf_count", "dlopen_binary_package_count", "archive", "result",
        "skipped_or_unsafe_member_count",
    ], archive_rows)
    write(output / "dlopen_source_token_hits.tsv", [
        "sourcerpm", "dlopen_runtime_elf_count", "dlopen_binary_package_count", "file", "line", "tokens", "original",
    ], hit_rows)
    write(output / "dlopen_source_call_scope_approximation.tsv", [
        "sourcerpm", "dlopen_runtime_elf_count", "dlopen_binary_package_count", "file", "line",
        "scope_window_class", "dlopen_line",
    ], call_rows)
    with open(args.ledger, "w", encoding="utf-8") as out:
        for i, item in enumerate(commands, 1):
            out.write(f"LABEL=dlopen_source_{i:05d}:{item['label']}\nPWD={item['pwd']}\nCOMMAND={item['command']}\n")
            if item["stderr"]: out.write(f"STDERR={item['stderr']}\n")
            out.write(f"EXIT_CODE={item['exit_code']}\n\n")
        out.write(f"SOURCE_RPM_EXTRACTION_COMMANDS={len(commands)}\nTARGET_SOURCE_PACKAGES={len(targets)}\n")
    return 0


if __name__ == "__main__": raise SystemExit(main())
