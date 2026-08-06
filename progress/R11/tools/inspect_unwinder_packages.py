#!/usr/bin/env python3
import argparse
import csv
import hashlib
import os
import pathlib
import re
import shlex
import stat
import subprocess


SONAME_RE = re.compile(r"\(SONAME\).*Library soname: \[([^]]+)\]")
SYMBOL_RE = re.compile(
    r"^\s*\d+:\s+\S+\s+\d+\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(.+?)\s*$"
)
INDEX_RE = re.compile(r"\s+\(\d+\)$")


def digest(path):
    h = hashlib.sha256()
    with open(path, "rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def clean(value):
    return value.replace("\t", " ").replace("\r", " ").replace("\n", "\\n")


def run(command, commands, label, cwd=None):
    proc = subprocess.run(command, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    commands.append({
        "label": label, "pwd": str(cwd or pathlib.Path.cwd()),
        "command": shlex.join([str(item) for item in command]), "exit_code": str(proc.returncode),
        "stderr": clean(proc.stderr),
    })
    return proc


def write_tsv(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)


def mode_type(mode_text):
    mode = int(mode_text, 8)
    if stat.S_ISREG(mode): return "REGULAR_FILE"
    if stat.S_ISDIR(mode): return "DIRECTORY"
    if stat.S_ISLNK(mode): return "SYMLINK"
    return "OTHER"


def split_symbol_version(name):
    name = INDEX_RE.sub("", name).strip()
    if "@@" in name:
        base, version = name.rsplit("@@", 1)
    elif "@" in name:
        base, version = name.rsplit("@", 1)
    else:
        base, version = name, "NO_VERSION"
    return name, base, version


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest", required=True)
    ap.add_argument("--extract-root", required=True)
    ap.add_argument("--table-root", required=True)
    ap.add_argument("--raw-root", required=True)
    ap.add_argument("--ledger", required=True)
    args = ap.parse_args()
    with open(args.manifest, encoding="utf-8") as stream:
        inputs = list(csv.DictReader(stream, delimiter="\t"))
    extract_root = pathlib.Path(args.extract_root)
    table_root = pathlib.Path(args.table_root)
    raw_root = pathlib.Path(args.raw_root)
    extract_root.mkdir(parents=True, exist_ok=False)
    table_root.mkdir(parents=True, exist_ok=True)
    raw_root.mkdir(parents=True, exist_ok=True)
    commands = []
    packages, files, elfs, exports = [], [], [], []
    files_by_identity = {}
    for index, row in enumerate(inputs, 1):
        rpm_path = pathlib.Path(row["path"]).resolve()
        sha = digest(rpm_path)
        identity = f"{row['set_id']}:{row['role']}:{row['arch']}"
        query = run([
            "rpm", "-qp", "--qf", "%{NAME}\\t%{EPOCHNUM}\\t%{VERSION}\\t%{RELEASE}\\t%{ARCH}\\n", str(rpm_path)
        ], commands, f"rpm_identity:{identity}")
        nevra_fields = query.stdout.rstrip("\n").split("\t") if query.returncode == 0 else ["NOT_AVAILABLE"] * 5
        if len(nevra_fields) != 5:
            nevra_fields = ["PARSE_FAILED"] * 5
        package_name, epoch, version, release, package_arch = nevra_fields
        prefix = {
            "set_id": row["set_id"], "role": row["role"], "arch": row["arch"],
            "name": package_name, "epoch": epoch, "version": version, "release": release,
            "rpm_sha256": sha, "rpm_path": row["path"],
        }
        packages.append(prefix)
        dump = run(["rpm", "-qp", "--dump", str(rpm_path)], commands, f"rpm_dump:{identity}")
        (raw_root / f"special_{index:02d}_{row['set_id']}_{row['role']}_{row['arch']}.rpm_dump.txt").write_text(
            dump.stdout, encoding="utf-8"
        )
        package_files = {}
        if dump.returncode == 0:
            for line in dump.stdout.splitlines():
                parts = line.split()
                if len(parts) < 11:
                    continue
                path, size, mtime, checksum, mode, owner, group, isconfig, isdoc, rdev = parts[:10]
                linkto = " ".join(parts[10:])
                file_row = {
                    **prefix, "path": path, "size": size, "mtime": mtime, "file_sha256": checksum,
                    "mode": mode, "file_type": mode_type(mode), "owner": owner, "group": group,
                    "isconfig": isconfig, "isdoc": isdoc, "rdev": rdev, "link_target": linkto,
                }
                files.append(file_row)
                package_files[path] = file_row
        files_by_identity[identity] = package_files
        out_dir = extract_root / f"{index:02d}_{row['set_id']}_{row['role']}_{row['arch']}"
        out_dir.mkdir(parents=True, exist_ok=False)
        shell = ["bash", "-o", "pipefail", "-c", f"rpm2cpio {shlex.quote(str(rpm_path))} | cpio -idm --quiet"]
        extract = run(shell, commands, f"extract:{identity}", cwd=out_dir)
        if extract.returncode != 0:
            continue
        for root, dirs, filenames in os.walk(out_dir):
            dirs.sort(); filenames.sort()
            for filename in filenames:
                file_path = pathlib.Path(root) / filename
                if file_path.is_symlink() or not file_path.is_file():
                    continue
                try:
                    with open(file_path, "rb") as stream:
                        magic = stream.read(4)
                except OSError:
                    continue
                if magic != b"\x7fELF":
                    continue
                relpath = "/" + str(file_path.relative_to(out_dir))
                if not ("unwind" in filename.lower() or filename == "libgcc_s.so.1"):
                    continue
                dyn = run(["readelf", "-d", "--dyn-syms", "--version-info", "-W", str(file_path)], commands, f"readelf:{identity}:{relpath}")
                raw_name = re.sub(r"[^A-Za-z0-9_.-]+", "_", relpath.strip("/"))
                (raw_root / f"special_{index:02d}_{row['arch']}_{raw_name}.readelf.txt").write_text(dyn.stdout, encoding="utf-8")
                soname_match = SONAME_RE.search(dyn.stdout)
                elfs.append({**prefix, "path": relpath, "soname": soname_match.group(1) if soname_match else "NO_SONAME"})
                for line in dyn.stdout.splitlines():
                    match = SYMBOL_RE.match(line)
                    if not match:
                        continue
                    symbol_type, bind, visibility, ndx, raw_name = match.groups()
                    if ndx == "UND" or bind not in {"GLOBAL", "WEAK"}:
                        continue
                    raw_symbol, symbol, node = split_symbol_version(raw_name)
                    exports.append({
                        **prefix, "path": relpath, "soname": soname_match.group(1) if soname_match else "NO_SONAME",
                        "symbol_type": symbol_type, "bind": bind, "visibility": visibility,
                        "symbol_raw": raw_symbol, "symbol": symbol, "version_node": node,
                    })
    pkg_fields = ["set_id", "role", "arch", "name", "epoch", "version", "release", "rpm_sha256", "rpm_path"]
    write_tsv(table_root / "special_unwinder_packages.tsv", pkg_fields, packages)
    write_tsv(table_root / "special_unwinder_file_lists.tsv", pkg_fields + [
        "path", "size", "mtime", "file_sha256", "mode", "file_type", "owner", "group",
        "isconfig", "isdoc", "rdev", "link_target",
    ], files)
    write_tsv(table_root / "special_unwinder_sonames.tsv", pkg_fields + ["path", "soname"], elfs)
    write_tsv(table_root / "special_unwinder_exports.tsv", pkg_fields + [
        "path", "soname", "symbol_type", "bind", "visibility", "symbol_raw", "symbol", "version_node"
    ], exports)

    collisions = []
    requested = {
        "/usr/include/unwind.h", "/usr/include/unwind_arm_ehabi.h",
        "/usr/include/unwind_itanium.h", "/usr/include/libunwind.h",
        "/usr/include/libunwind.modulemap", "/usr/include/__libunwind_config.h",
        "/usr/include/mach-o/compact_unwind_encoding.h",
    }
    for arch in ("armv7l", "aarch64", "x86_64"):
        pairs = [
            ("platform_repo_llvm", "llvm_devel", "PLATFORM_REPO_LLVM"),
            ("project_c2", "project_llvm_devel", "PROJECT_C2_LLVM"),
        ]
        nongnu = files_by_identity.get(f"platform_nongnu:nongnu_devel:{arch}", {})
        for set_id, role, llvm_label in pairs:
            llvm = files_by_identity.get(f"{set_id}:{role}:{arch}", {})
            paths = set(nongnu) | set(llvm) | requested
            paths |= {p for p in set(nongnu) | set(llvm) if re.match(r"^/usr/lib(64)?/libunwind\.so", p)}
            for path in sorted(paths):
                left, right = nongnu.get(path), llvm.get(path)
                if not left and not right:
                    continue
                if left and right:
                    if left["file_type"] == "REGULAR_FILE" and right["file_type"] == "REGULAR_FILE":
                        classification = "HARD_REGULAR_FILE_TO_REGULAR_FILE"
                    elif left["file_type"] == "DIRECTORY" and right["file_type"] == "DIRECTORY":
                        classification = "SHARED_DIRECTORY_NOT_HARD_CONFLICT"
                    elif left["file_type"] == "SYMLINK" or right["file_type"] == "SYMLINK":
                        classification = "SAME_PATH_WITH_SYMLINK_REQUIRES_RPM_TRANSACTION_CHECK"
                    else:
                        classification = "SAME_PATH_OTHER_TYPE_REQUIRES_RPM_TRANSACTION_CHECK"
                else:
                    classification = "ONE_SIDE_ONLY"
                collisions.append({
                    "arch": arch, "llvm_set": llvm_label, "path": path,
                    "nongnu_package": nongnu.get(path, {}).get("name", "NOT_PRESENT"),
                    "nongnu_type": nongnu.get(path, {}).get("file_type", "NOT_PRESENT"),
                    "nongnu_sha256": nongnu.get(path, {}).get("file_sha256", "NOT_PRESENT"),
                    "llvm_package": llvm.get(path, {}).get("name", "NOT_PRESENT"),
                    "llvm_type": llvm.get(path, {}).get("file_type", "NOT_PRESENT"),
                    "llvm_sha256": llvm.get(path, {}).get("file_sha256", "NOT_PRESENT"),
                    "classification": classification,
                })
    write_tsv(table_root / "libunwind_file_conflicts.tsv", [
        "arch", "llvm_set", "path", "nongnu_package", "nongnu_type", "nongnu_sha256",
        "llvm_package", "llvm_type", "llvm_sha256", "classification",
    ], collisions)
    with open(args.ledger, "w", encoding="utf-8") as out:
        for i, row in enumerate(commands, 1):
            out.write(f"LABEL=special_unwind_{i:05d}:{row['label']}\nPWD={row['pwd']}\n")
            out.write(f"COMMAND={row['command']}\n")
            if row["stderr"]: out.write(f"STDERR={row['stderr']}\n")
            out.write(f"EXIT_CODE={row['exit_code']}\n\n")
        out.write(f"COMMAND_COUNT={len(commands)}\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
