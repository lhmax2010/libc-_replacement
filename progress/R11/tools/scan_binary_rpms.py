#!/usr/bin/env python3
"""Extract every downloaded binary RPM and inventory its ELF objects.

The scanner deliberately records all ELF files, while marking ET_EXEC/ET_DYN
as the runtime-ELF population used for DT_NEEDED and unwinder fan-in counts.
Each package produces a self-contained JSON shard so a partial run remains
auditable if an individual RPM cannot be unpacked or inspected.
"""

import argparse
import concurrent.futures
import csv
import hashlib
import json
import os
import pathlib
import re
import shlex
import shutil
import subprocess
import sys
import threading


HEADER_TYPE_RE = re.compile(r"^\s*Type:\s+(\S+)", re.MULTILINE)
HEADER_MACHINE_RE = re.compile(r"^\s*Machine:\s+(.+?)\s*$", re.MULTILINE)
SECTION_RE = re.compile(r"^\s*\[\s*\d+\]\s+(\S+)", re.MULTILINE)
NEEDED_RE = re.compile(r"\(NEEDED\).*Shared library: \[([^]]+)\]")
BUILD_ID_RE = re.compile(r"Build ID:\s*([0-9a-fA-F]+)")
SYMBOL_RE = re.compile(
    r"^\s*\d+:\s+\S+\s+\d+\s+\S+\s+\S+\s+\S+\s+(\S+)\s+(.+?)\s*$"
)
VERSION_INDEX_RE = re.compile(r"\s+\(\d+\)$")


def sha256(path):
    h = hashlib.sha256()
    with open(path, "rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def clean_text(value):
    return value.replace("\t", " ").replace("\r", " ").replace("\n", "\\n")


def is_elf(path):
    try:
        with open(path, "rb") as stream:
            return stream.read(4) == b"\x7fELF"
    except OSError:
        return False


def symbol_name(raw):
    return VERSION_INDEX_RE.sub("", raw).strip()


def split_version(name):
    if "@@" in name:
        base, node = name.rsplit("@@", 1)
        return base, node, "VERSIONED"
    if "@" in name:
        base, node = name.rsplit("@", 1)
        return base, node, "VERSIONED"
    return name, "", "UNVERSIONED"


def scan_one(row, extract_root, shard_root, readelf, sequence, total, progress_lock):
    rpm_path = pathlib.Path(row["target"])
    key = row["checksum"]
    out_dir = pathlib.Path(extract_root) / key[:2] / key
    shard = pathlib.Path(shard_root) / f"{key}.json"
    result = {
        "package": row,
        "extract": {},
        "elfs": [],
        "unwind": [],
        "needed": [],
        "failures": [],
        "commands": [],
    }
    if row.get("result") != "PASS":
        result["extract"] = {"exit_code": "NOT_RUN", "result": "DOWNLOAD_NOT_PASS"}
    elif sha256(rpm_path) != key:
        result["extract"] = {"exit_code": "NOT_RUN", "result": "RPM_SHA256_MISMATCH"}
    else:
        out_dir.mkdir(parents=True, exist_ok=False)
        cmd_display = (
            f"cd {shlex.quote(str(out_dir))} && rpm2cpio {shlex.quote(str(rpm_path))} "
            "| cpio -idm --quiet"
        )
        p1 = subprocess.Popen(["rpm2cpio", str(rpm_path)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p2 = subprocess.Popen(
            ["cpio", "-idm", "--quiet"], cwd=out_dir, stdin=p1.stdout,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        )
        assert p1.stdout is not None
        p1.stdout.close()
        cpio_stdout, cpio_stderr = p2.communicate()
        rpm2cpio_stderr = p1.stderr.read() if p1.stderr is not None else b""
        rpm2cpio_rc = p1.wait()
        pipeline_rc = p2.returncode if p2.returncode != 0 else rpm2cpio_rc
        result["commands"].append({
            "label": "extract_rpm",
            "pwd": os.getcwd(),
            "command": cmd_display,
            "exit_code": str(pipeline_rc),
            "stderr": clean_text((rpm2cpio_stderr + cpio_stderr).decode("utf-8", "replace")),
        })
        result["extract"] = {
            "exit_code": str(pipeline_rc),
            "rpm2cpio_exit_code": str(rpm2cpio_rc),
            "cpio_exit_code": str(p2.returncode),
            "result": "PASS" if pipeline_rc == 0 else "EXTRACT_FAILED",
        }
        if pipeline_rc == 0:
            for root, dirs, files in os.walk(out_dir):
                dirs.sort()
                files.sort()
                for filename in files:
                    path = pathlib.Path(root) / filename
                    if path.is_symlink() or not path.is_file() or not is_elf(path):
                        continue
                    relpath = "/" + str(path.relative_to(out_dir))
                    inspect_cmd = [readelf, "-hSWd", "--dyn-syms", "-W", str(path)]
                    proc = subprocess.run(inspect_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                    result["commands"].append({
                        "label": f"readelf:{relpath}",
                        "pwd": os.getcwd(),
                        "command": shlex.join(inspect_cmd),
                        "exit_code": str(proc.returncode),
                        "stderr": clean_text(proc.stderr),
                    })
                    if proc.returncode != 0:
                        result["failures"].append({
                            "path": relpath, "operation": "readelf_inventory",
                            "exit_code": str(proc.returncode), "stderr": clean_text(proc.stderr),
                        })
                        continue
                    text = proc.stdout
                    elf_type_match = HEADER_TYPE_RE.search(text)
                    machine_match = HEADER_MACHINE_RE.search(text)
                    elf_type = elf_type_match.group(1) if elf_type_match else "UNKNOWN"
                    machine = machine_match.group(1) if machine_match else "UNKNOWN"
                    sections = sorted(set(SECTION_RE.findall(text)))
                    needed = sorted(set(NEEDED_RE.findall(text)))
                    symbols = []
                    und = []
                    for line in text.splitlines():
                        match = SYMBOL_RE.match(line)
                        if not match:
                            continue
                        ndx, raw_name = match.groups()
                        name = symbol_name(raw_name)
                        symbols.append(name)
                        if ndx == "UND":
                            und.append(name)
                    cpp_symbol = any(split_version(name)[0].startswith("_Z") for name in symbols)
                    cpp_needed = any(
                        re.match(r"^(libstdc\+\+|libc\+\+|libc\+\+abi)\.so", item)
                        for item in needed
                    )
                    is_debug_payload = (
                        relpath.startswith("/usr/lib/debug/")
                        or relpath.startswith("/usr/src/debug/")
                        or relpath.endswith(".debug")
                    )
                    is_runtime = elf_type in {"EXEC", "DYN"} and not is_debug_payload
                    note_cmd = [readelf, "-n", "-W", str(path)]
                    note = subprocess.run(note_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                    result["commands"].append({
                        "label": f"readelf_notes:{relpath}",
                        "pwd": os.getcwd(),
                        "command": shlex.join(note_cmd),
                        "exit_code": str(note.returncode),
                        "stderr": clean_text(note.stderr),
                    })
                    build_id_match = BUILD_ID_RE.search(note.stdout) if note.returncode == 0 else None
                    result["elfs"].append({
                        "path": relpath,
                        "elf_type": elf_type,
                        "machine": machine,
                        "runtime_elf": "YES" if is_runtime else "NO",
                        "debug_payload": "YES" if is_debug_payload else "NO",
                        "itanium_mangled_symbol": "YES" if cpp_symbol else "NO",
                        "cpp_runtime_needed": "YES" if cpp_needed else "NO",
                        "cpp_indicator": "YES" if cpp_symbol or cpp_needed else "NO",
                        "eh_frame": "YES" if ".eh_frame" in sections else "NO",
                        "arm_exidx": "YES" if ".ARM.exidx" in sections else "NO",
                        "arm_extab": "YES" if ".ARM.extab" in sections else "NO",
                        "dlopen_und": "YES" if any(split_version(name)[0] == "dlopen" for name in und) else "NO",
                        "build_id": build_id_match.group(1).lower() if build_id_match else "",
                        "needed_count": str(len(needed)),
                    })
                    for soname in needed:
                        result["needed"].append({"path": relpath, "soname": soname})
                    for name in und:
                        base, node, kind = split_version(name)
                        if "Unwind" not in base:
                            continue
                        result["unwind"].append({
                            "path": relpath,
                            "symbol_raw": name,
                            "symbol": base,
                            "version_node": node,
                            "version_class": kind,
                        })
    shard.parent.mkdir(parents=True, exist_ok=True)
    temp = pathlib.Path(str(shard) + ".part")
    with open(temp, "w", encoding="utf-8") as out:
        json.dump(result, out, ensure_ascii=False, sort_keys=True)
        out.write("\n")
    os.replace(temp, shard)
    with progress_lock:
        sequence[0] += 1
        if sequence[0] % 100 == 0 or sequence[0] == total:
            print(f"PROGRESS={sequence[0]}/{total}", flush=True)
    return key


def write_tsv(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(
            out, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore"
        )
        writer.writeheader()
        writer.writerows(rows)


def package_prefix(package):
    return {
        "repo_id": package["repo_id"], "name": package["name"],
        "arch": package["arch"], "epoch": package["epoch"],
        "version": package["version"], "release": package["release"],
        "sourcerpm": package["sourcerpm"], "rpm_sha256": package["checksum"],
    }


def merge(shard_root, table_root, ledger_path):
    extracts, elfs, unwind, needed, failures = [], [], [], [], []
    ledger = []
    for shard in sorted(pathlib.Path(shard_root).glob("*.json")):
        with open(shard, encoding="utf-8") as stream:
            data = json.load(stream)
        prefix = package_prefix(data["package"])
        extracts.append({**prefix, **data["extract"]})
        for row in data["elfs"]:
            elfs.append({**prefix, **row})
        for row in data["unwind"]:
            unwind.append({**prefix, **row})
        for row in data["needed"]:
            needed.append({**prefix, **row})
        for row in data["failures"]:
            failures.append({**prefix, **row})
        for row in data["commands"]:
            ledger.append({**prefix, **row})
    pathlib.Path(table_root).mkdir(parents=True, exist_ok=True)
    pfx = ["repo_id", "name", "arch", "epoch", "version", "release", "sourcerpm", "rpm_sha256"]
    write_tsv(pathlib.Path(table_root) / "rpm_extraction_status.tsv", pfx + [
        "exit_code", "rpm2cpio_exit_code", "cpio_exit_code", "result"
    ], extracts)
    write_tsv(pathlib.Path(table_root) / "elf_inventory.tsv", pfx + [
        "path", "elf_type", "machine", "runtime_elf", "debug_payload", "itanium_mangled_symbol",
        "cpp_runtime_needed", "cpp_indicator", "eh_frame", "arm_exidx", "arm_extab",
        "dlopen_und", "build_id", "needed_count"
    ], elfs)
    write_tsv(pathlib.Path(table_root) / "unwind_undefined_symbols.tsv", pfx + [
        "path", "symbol_raw", "symbol", "version_node", "version_class"
    ], unwind)
    write_tsv(pathlib.Path(table_root) / "dt_needed_edges.tsv", pfx + ["path", "soname"], needed)
    write_tsv(pathlib.Path(table_root) / "elf_analysis_failures.tsv", pfx + [
        "path", "operation", "exit_code", "stderr"
    ], failures)
    with open(ledger_path, "w", encoding="utf-8") as out:
        for index, row in enumerate(ledger, 1):
            out.write(f"LABEL=elf_scan_{index:07d}:{row['label']}\n")
            out.write(f"PACKAGE={row['name']}-{row['version']}-{row['release']}.{row['arch']}\n")
            out.write(f"RPM_SHA256={row['rpm_sha256']}\n")
            out.write(f"PWD={row['pwd']}\n")
            out.write(f"COMMAND={row['command']}\n")
            if row.get("stderr"):
                out.write(f"STDERR={row['stderr']}\n")
            out.write(f"EXIT_CODE={row['exit_code']}\n\n")
        out.write(f"RECORDED_COMMANDS={len(ledger)}\n")
        out.write(f"EXTRACTION_RECORDS={len(extracts)}\n")
        out.write(f"ELF_RECORDS={len(elfs)}\n")
        out.write(f"UNWIND_UND_RECORDS={len(unwind)}\n")
        out.write(f"NEEDED_EDGE_RECORDS={len(needed)}\n")
        out.write(f"ANALYSIS_FAILURES={len(failures)}\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--download-status", required=True)
    ap.add_argument("--extract-root", required=True)
    ap.add_argument("--shard-root", required=True)
    ap.add_argument("--table-root", required=True)
    ap.add_argument("--ledger", required=True)
    ap.add_argument("--jobs", type=int, default=12)
    args = ap.parse_args()
    with open(args.download_status, encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    extract_root = pathlib.Path(args.extract_root)
    shard_root = pathlib.Path(args.shard_root)
    extract_root.mkdir(parents=True, exist_ok=False)
    shard_root.mkdir(parents=True, exist_ok=False)
    readelf = shutil.which("readelf")
    if not readelf:
        raise SystemExit("readelf NOT_FOUND")
    sequence = [0]
    progress_lock = threading.Lock()
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = [
            pool.submit(
                scan_one, row, extract_root, shard_root, readelf,
                sequence, len(rows), progress_lock,
            )
            for row in rows
        ]
        for future in concurrent.futures.as_completed(futures):
            future.result()
    merge(shard_root, args.table_root, args.ledger)
    return 0


if __name__ == "__main__":
    sys.exit(main())
