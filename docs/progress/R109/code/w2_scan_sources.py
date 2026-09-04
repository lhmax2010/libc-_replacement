#!/usr/bin/env python3
import argparse
import csv
import hashlib
import os
import re
import shutil
import subprocess
import tarfile
import zipfile
from pathlib import Path, PurePosixPath


EXTENSIONS = {
    ".c", ".cc", ".cpp", ".cxx", ".C", ".h", ".hh", ".hpp", ".hxx",
    ".ipp", ".tcc", ".inl", ".inc", ".m", ".mm", ".cu", ".cuh", ".hip",
    ".sycl", ".i", ".ii", ".spec", ".cmake",
}
TOKEN = re.compile(rb"(?<![A-Za-z0-9_])wbuffer_convert(?![A-Za-z0-9_])")
STATUS_FIELDS = (
    "source_rpm", "package_name", "status", "detail", "compressed_bytes",
    "sha256_verified", "archives_seen", "archives_expanded", "skipped_members",
    "missing_link_targets", "files_scanned", "hits",
)
HIT_FIELDS = ("source_rpm", "package_name", "path", "line", "source_text")
ARCHIVE_FIELDS = (
    "source_rpm", "archive", "result", "members", "regular_or_directory",
    "skipped_members", "missing_link_targets",
)


def read(path):
    if not path.exists():
        return []
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    with temporary.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
        stream.flush()
        os.fsync(stream.fileno())
    temporary.replace(path)


def hash_file(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(8 * 1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def safe_name(name):
    value = PurePosixPath(name)
    return bool(name) and not value.is_absolute() and ".." not in value.parts


def normalized_link(member_name, link_name, hardlink=False):
    if not safe_name(link_name):
        return None
    if hardlink:
        value = PurePosixPath(link_name)
    else:
        value = PurePosixPath(member_name).parent / link_name
    parts = []
    for part in value.parts:
        if part in ("", "."):
            continue
        if part == "..":
            if not parts:
                return None
            parts.pop()
        else:
            parts.append(part)
    return "/".join(parts)


def extract_archive(path, destination):
    destination.mkdir(parents=True, exist_ok=False)
    try:
        if zipfile.is_zipfile(path):
            with zipfile.ZipFile(path) as archive:
                members = archive.infolist()
                names = {m.filename.rstrip("/") for m in members if safe_name(m.filename)}
                safe = []
                skipped = 0
                missing = 0
                for member in members:
                    mode = (member.external_attr >> 16) & 0o170000
                    if safe_name(member.filename) and mode != 0o120000:
                        safe.append(member)
                    else:
                        skipped += 1
                        if mode == 0o120000:
                            try:
                                target = archive.read(member).decode("utf-8", "replace")
                            except Exception:
                                target = ""
                            resolved = normalized_link(member.filename, target)
                            if resolved not in names:
                                missing += 1
                for member in safe:
                    archive.extract(member, destination)
                result = "ZIP_OK" if not skipped else "ZIP_OK_WITH_SKIPS"
                return True, result, len(members), len(safe), skipped, missing
        if tarfile.is_tarfile(path):
            with tarfile.open(path, "r:*") as archive:
                members = archive.getmembers()
                content_names = {m.name.rstrip("/") for m in members
                                 if safe_name(m.name) and (m.isfile() or m.isdir())}
                safe = [m for m in members if safe_name(m.name) and (m.isfile() or m.isdir())]
                skipped = len(members) - len(safe)
                missing = 0
                for member in members:
                    if member.issym() or member.islnk():
                        target = normalized_link(member.name, member.linkname, member.islnk())
                        if target not in content_names:
                            missing += 1
                    elif not safe_name(member.name) and (member.isfile() or member.isdir()):
                        missing += 1
                archive.extractall(destination, members=safe)
                result = "TAR_OK" if not skipped else "TAR_OK_WITH_SKIPS"
                return True, result, len(members), len(safe), skipped, missing
        destination.rmdir()
        return False, "NOT_SUPPORTED_ARCHIVE", 0, 0, 0, 0
    except (OSError, tarfile.TarError, zipfile.BadZipFile) as error:
        shutil.rmtree(destination, ignore_errors=True)
        return False, f"EXTRACT_ERROR:{type(error).__name__}:{error}", 0, 0, 0, 0


def likely_archive(path):
    return path.name.lower().endswith((
        ".tar", ".tar.gz", ".tgz", ".tar.bz2", ".tbz", ".tbz2", ".tar.xz",
        ".txz", ".zip", ".crate",
    ))


def extract_srpm(path, payload, ledger):
    rpm = subprocess.Popen(["rpm2cpio", str(path)], cwd=payload,
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    assert rpm.stdout is not None
    cpio = subprocess.Popen(["cpio", "-idm", "--quiet", "--no-absolute-filenames"],
                            cwd=payload, stdin=rpm.stdout, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    rpm.stdout.close()
    _, cpio_err = cpio.communicate(timeout=1800)
    rpm_err = rpm.stderr.read() if rpm.stderr is not None else b""
    rpm_rc = rpm.wait(timeout=60)
    with ledger.open("a") as stream:
        stream.write(f"PWD={payload}\nCOMMAND=rpm2cpio {path} | cpio -idm --quiet --no-absolute-filenames\n")
        stream.write(f"PIPE_EXIT_CODE={rpm_rc},{cpio.returncode}\n")
        if rpm_err:
            stream.write("RPM2CPIO_STDERR=" + rpm_err.decode("utf-8", "replace").replace("\n", "\\n") + "\n")
        if cpio_err:
            stream.write("CPIO_STDERR=" + cpio_err.decode("utf-8", "replace").replace("\n", "\\n") + "\n")
        stream.write("\n")
    return rpm_rc == 0 and cpio.returncode == 0, f"rpm2cpio={rpm_rc},cpio={cpio.returncode}"


def wanted(path):
    return path.name == "CMakeLists.txt" or path.suffix in EXTENSIONS


def scan_tree(root, source_rpm, package_name):
    hits = []
    files = 0
    for path in root.rglob("*"):
        if not path.is_file() or path.is_symlink() or not wanted(path):
            continue
        files += 1
        try:
            with path.open("rb") as stream:
                for number, line in enumerate(stream, 1):
                    if TOKEN.search(line):
                        hits.append({
                            "source_rpm": source_rpm, "package_name": package_name,
                            "path": str(path.relative_to(root)), "line": number,
                            "source_text": line.decode("utf-8", "replace").rstrip("\r\n"),
                        })
        except OSError:
            continue
    return files, hits


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--work", required=True, type=Path)
    parser.add_argument("--status", required=True, type=Path)
    parser.add_argument("--hits", required=True, type=Path)
    parser.add_argument("--archives", required=True, type=Path)
    parser.add_argument("--ledger", required=True, type=Path)
    parser.add_argument("--min-free-gib", type=int, default=80)
    args = parser.parse_args()
    manifest = read(args.manifest)
    statuses = read(args.status)
    hits = read(args.hits)
    archive_rows = read(args.archives)
    done = {r["source_rpm"] for r in statuses if r["status"] in {"SCAN_OK", "PARTIAL"}}
    args.ledger.parent.mkdir(parents=True, exist_ok=True)
    for index, item in enumerate(manifest, 1):
        identity = item["source_rpm"]
        if identity in done:
            continue
        if shutil.disk_usage(args.work.parent).free < args.min_free_gib * 1024**3:
            print(f"RED_STOP_DISK index={index}", flush=True)
            return 30
        shutil.rmtree(args.work, ignore_errors=True)
        payload = args.work / "payload"
        expanded = args.work / "expanded"
        payload.mkdir(parents=True)
        expanded.mkdir()
        source = Path(item["local_path"])
        verified = "YES" if source.is_file() and hash_file(source) == item["sha256"] else "NO"
        if verified != "YES":
            statuses.append({
                "source_rpm": identity, "package_name": item["package_name"], "status": "NOT_AVAILABLE",
                "detail": "SOURCE_MISSING_OR_SHA256_MISMATCH", "compressed_bytes": item["compressed_bytes"],
                "sha256_verified": verified, "archives_seen": 0, "archives_expanded": 0,
                "skipped_members": 0, "missing_link_targets": 0, "files_scanned": 0, "hits": 0,
            })
            write(args.status, STATUS_FIELDS, statuses)
            continue
        ok, detail = extract_srpm(source, payload, args.ledger)
        if not ok:
            statuses.append({
                "source_rpm": identity, "package_name": item["package_name"], "status": "NOT_AVAILABLE",
                "detail": detail, "compressed_bytes": item["compressed_bytes"], "sha256_verified": verified,
                "archives_seen": 0, "archives_expanded": 0, "skipped_members": 0,
                "missing_link_targets": 0, "files_scanned": 0, "hits": 0,
            })
            write(args.status, STATUS_FIELDS, statuses)
            continue
        archives = sorted(p for p in payload.rglob("*") if p.is_file() and likely_archive(p))
        expanded_count = skipped_total = missing_total = 0
        unsupported = []
        for number, archive in enumerate(archives, 1):
            destination = expanded / f"{number:03d}_{archive.name.replace('/', '_')}"
            result = extract_archive(archive, destination)
            archive_ok, result_name, members, regular, skipped, missing = result
            archive_rows.append({
                "source_rpm": identity, "archive": str(archive.relative_to(payload)),
                "result": result_name, "members": members, "regular_or_directory": regular,
                "skipped_members": skipped, "missing_link_targets": missing,
            })
            if archive_ok:
                expanded_count += 1
            else:
                unsupported.append(f"{archive.name}:{result_name}")
            skipped_total += skipped
            missing_total += missing
        files, package_hits = scan_tree(args.work, identity, item["package_name"])
        hits.extend(package_hits)
        if unsupported or missing_total:
            status = "PARTIAL"
            status_detail = detail + ";" + ";".join(unsupported or [f"MISSING_LINK_TARGETS={missing_total}"])
        else:
            status = "SCAN_OK"
            status_detail = detail
        statuses.append({
            "source_rpm": identity, "package_name": item["package_name"], "status": status,
            "detail": status_detail, "compressed_bytes": item["compressed_bytes"],
            "sha256_verified": verified, "archives_seen": len(archives),
            "archives_expanded": expanded_count, "skipped_members": skipped_total,
            "missing_link_targets": missing_total, "files_scanned": files, "hits": len(package_hits),
        })
        write(args.archives, ARCHIVE_FIELDS, archive_rows)
        write(args.hits, HIT_FIELDS, hits)
        write(args.status, STATUS_FIELDS, statuses)
        print(f"PROGRESS={index}/{len(manifest)} status={status} files={files} hits={len(package_hits)}", flush=True)
    shutil.rmtree(args.work, ignore_errors=True)
    ok = sum(r["status"] == "SCAN_OK" for r in statuses)
    partial = sum(r["status"] == "PARTIAL" for r in statuses)
    unavailable = sum(r["status"] == "NOT_AVAILABLE" for r in statuses)
    print(f"FINAL scan_ok={ok} partial={partial} not_available={unavailable} hits={len(hits)}", flush=True)
    return 0 if unavailable == 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
