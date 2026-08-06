#!/usr/bin/env python3
import argparse
import csv
import hashlib
import os
import pathlib
import re
import shlex
import subprocess


TYPE_RE = re.compile(r"^\s*Type:\s+(\S+)", re.MULTILINE)
SECTION_RE = re.compile(r"^\s*\[\s*\d+\]\s+(\S+)", re.MULTILINE)
SYMBOL_RE = re.compile(r"^\s*\d+:\s+\S+\s+\d+\s+\S+\s+\S+\s+\S+\s+(\S+)\s+(.+?)\s*$")
INDEX_RE = re.compile(r"\s+\(\d+\)$")


def sha(path):
    value = hashlib.sha256()
    with open(path, "rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""): value.update(chunk)
    return value.hexdigest()


def clean(value): return value.replace("\t", " ").replace("\r", " ").replace("\n", "\\n")


def run(command, commands, label, cwd=None):
    proc = subprocess.run(command, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    commands.append({
        "label": label, "pwd": str(cwd or pathlib.Path.cwd()), "command": shlex.join([str(x) for x in command]),
        "exit_code": str(proc.returncode), "stderr": clean(proc.stderr),
    })
    return proc


def write(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(rows)


def split_version(name):
    name = INDEX_RE.sub("", name).strip()
    if "@@" in name: base, node = name.rsplit("@@", 1)
    elif "@" in name: base, node = name.rsplit("@", 1)
    else: base, node = name, ""
    return name, base, node, "VERSIONED" if node else "UNVERSIONED"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--artifacts-root", required=True)
    ap.add_argument("--extract-root", required=True)
    ap.add_argument("--table-root", required=True)
    ap.add_argument("--ledger", required=True)
    args = ap.parse_args()
    artifact_root = pathlib.Path(args.artifacts_root)
    extract_root = pathlib.Path(args.extract_root)
    extract_root.mkdir(parents=True, exist_ok=False)
    commands, packages, elfs, unwind = [], [], [], []
    rpms = sorted(path for path in artifact_root.glob("*/*.rpm") if not path.name.endswith(".src.rpm"))
    for index, rpm in enumerate(rpms, 1):
        identity = run(["rpm", "-qp", "--qf", "%{NAME}\\t%{EPOCHNUM}\\t%{VERSION}\\t%{RELEASE}\\t%{ARCH}\\n", str(rpm)], commands, f"identity:{rpm}")
        values = identity.stdout.rstrip("\n").split("\t")
        if identity.returncode != 0 or len(values) != 5:
            values = ["NOT_AVAILABLE"] * 5
        name, epoch, version, release, arch = values
        prefix = {
            "name": name, "epoch": epoch, "version": version, "release": release,
            "arch": arch, "rpm_sha256": sha(rpm), "rpm_path": str(rpm),
        }
        packages.append(prefix)
        out_dir = extract_root / f"{index:02d}_{arch}_{name}"
        out_dir.mkdir(parents=True, exist_ok=False)
        extract = run(["bash", "-o", "pipefail", "-c", f"rpm2cpio {shlex.quote(str(rpm.resolve()))} | cpio -idm --quiet"], commands, f"extract:{rpm}", cwd=out_dir)
        if extract.returncode != 0: continue
        for root, dirs, files in os.walk(out_dir):
            dirs.sort(); files.sort()
            for filename in files:
                path = pathlib.Path(root) / filename
                if path.is_symlink() or not path.is_file(): continue
                try:
                    with open(path, "rb") as stream: magic = stream.read(4)
                except OSError:
                    continue
                if magic != b"\x7fELF": continue
                relpath = "/" + str(path.relative_to(out_dir))
                inspected = run(["readelf", "-hS", "--dyn-syms", "--version-info", "-W", str(path)], commands, f"readelf:{rpm}:{relpath}")
                if inspected.returncode != 0: continue
                type_match = TYPE_RE.search(inspected.stdout)
                elf_type = type_match.group(1) if type_match else "UNKNOWN"
                sections = set(SECTION_RE.findall(inspected.stdout))
                is_debug_payload = (
                    relpath.startswith("/usr/lib/debug/")
                    or relpath.startswith("/usr/src/debug/")
                    or relpath.endswith(".debug")
                )
                is_runtime = elf_type in {"EXEC", "DYN"} and not is_debug_payload
                elfs.append({
                    **prefix, "path": relpath, "elf_type": elf_type,
                    "runtime_elf": "YES" if is_runtime else "NO",
                    "debug_payload": "YES" if is_debug_payload else "NO",
                    "eh_frame": "YES" if ".eh_frame" in sections else "NO",
                    "arm_exidx": "YES" if ".ARM.exidx" in sections else "NO",
                    "arm_extab": "YES" if ".ARM.extab" in sections else "NO",
                })
                for line in inspected.stdout.splitlines():
                    match = SYMBOL_RE.match(line)
                    if not match or match.group(1) != "UND": continue
                    raw, base, node, version_class = split_version(match.group(2))
                    if "Unwind" not in base: continue
                    unwind.append({
                        **prefix, "path": relpath, "elf_type": elf_type, "symbol_raw": raw,
                        "symbol": base, "version_node": node, "version_class": version_class,
                    })
    pfx = ["name", "epoch", "version", "release", "arch", "rpm_sha256", "rpm_path"]
    root = pathlib.Path(args.table_root)
    write(root / "project_c2_packages.tsv", pfx, packages)
    write(root / "project_c2_elf_inventory.tsv", pfx + [
        "path", "elf_type", "runtime_elf", "debug_payload", "eh_frame", "arm_exidx", "arm_extab"
    ], elfs)
    write(root / "project_c2_unwind_undefined_symbols.tsv", pfx + [
        "path", "elf_type", "symbol_raw", "symbol", "version_node", "version_class"
    ], unwind)
    with open(args.ledger, "w", encoding="utf-8") as out:
        for i, item in enumerate(commands, 1):
            out.write(f"LABEL=project_c2_{i:05d}:{item['label']}\nPWD={item['pwd']}\nCOMMAND={item['command']}\n")
            if item["stderr"]: out.write(f"STDERR={item['stderr']}\n")
            out.write(f"EXIT_CODE={item['exit_code']}\n\n")
        out.write(f"COMMAND_COUNT={len(commands)}\n")
    return 0


if __name__ == "__main__": raise SystemExit(main())
