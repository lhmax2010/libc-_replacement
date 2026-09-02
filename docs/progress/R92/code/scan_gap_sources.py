#!/usr/bin/env python3
"""Extract the 140 R90 gap SRPMs one at a time and invoke R87's scanner unchanged."""

from __future__ import annotations

import csv
import hashlib
import os
import shutil
import subprocess
import sys
import tarfile
import zipfile
from pathlib import Path, PurePosixPath


ROOT = Path.cwd()
MANIFEST = ROOT / "progress/R92/tables/cpp_gap_140.tsv"
WORK = ROOT / "progress/R92/work/current_package"
BIG_STREAM_CACHE = ROOT / "progress/R92/work/chromium_payload.tar.gz"
TABLES = ROOT / "progress/R92/tables"
STATUS = TABLES / "source_scan_status.tsv"
CANDIDATES = TABLES / "source_candidates.tsv"
ARCHIVES = TABLES / "archive_status.tsv"
LEDGER = ROOT / "progress/R92/raw/source_scan_command_ledger.txt"
R87_SCANNER = ROOT / "docs/progress/R87/code/scan_noexcept_type_uses.py"
MIN_FREE_BYTES = 80 * 1024**3

STATUS_FIELDS = (
    "source_rpm", "package_name", "status", "detail", "compressed_bytes",
    "sha256_verified", "archives_seen", "archives_expanded", "candidates",
)
CANDIDATE_FIELDS = (
    "source_rpm", "package_name", "site", "candidate_kind", "file", "line", "source_text",
)
ARCHIVE_FIELDS = ("source_rpm", "archive", "result", "skipped_members")


def read_tsv(path: Path) -> list[dict[str, str]]:
    if not path.exists():
        return []
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path: Path, fields: tuple[str, ...], rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    with temporary.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
        stream.flush()
        os.fsync(stream.fileno())
    temporary.replace(path)


def safe_member(name: str) -> bool:
    value = PurePosixPath(name)
    return not value.is_absolute() and ".." not in value.parts


def hash_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(8 * 1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def run(command: list[str], *, cwd: Path, timeout: int | None = None) -> subprocess.CompletedProcess[str]:
    with LEDGER.open("a", encoding="utf-8") as ledger:
        ledger.write(f"PWD={cwd}\nCOMMAND={subprocess.list2cmdline(command)}\n")
    proc = subprocess.run(command, cwd=cwd, text=True, stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE, timeout=timeout)
    with LEDGER.open("a", encoding="utf-8") as ledger:
        ledger.write(f"EXIT_CODE={proc.returncode}\n")
        if proc.stdout:
            ledger.write("STDOUT=" + proc.stdout.replace("\n", "\\n") + "\n")
        if proc.stderr:
            ledger.write("STDERR=" + proc.stderr.replace("\n", "\\n") + "\n")
        ledger.write("\n")
    return proc


def extract_srpm(path: Path, payload: Path) -> tuple[bool, str]:
    # cpio's classic header cannot represent a >4 GiB member. Use rpm2archive there.
    if path.stat().st_size >= 4 * 1024**3:
        archive_dir = WORK / "rpm2archive"
        archive_dir.mkdir()
        generated = BIG_STREAM_CACHE
        if not generated.is_file():
            temporary = generated.with_suffix(generated.suffix + ".tmp")
            command = ["rpm2archive", str(path.resolve())]
            with temporary.open("wb") as output:
                proc = subprocess.run(command, cwd=archive_dir, stdout=output,
                                      stderr=subprocess.PIPE, timeout=7200)
            with LEDGER.open("a", encoding="utf-8") as ledger:
                ledger.write(f"PWD={archive_dir}\nCOMMAND={subprocess.list2cmdline(command)} > {temporary}\n")
                ledger.write(f"EXIT_CODE={proc.returncode}\n")
                if proc.stderr:
                    ledger.write("STDERR=" + proc.stderr.decode("utf-8", "replace").replace("\n", "\\n") + "\n")
                ledger.write("\n")
            if proc.returncode:
                temporary.unlink(missing_ok=True)
                return False, f"rpm2archive_exit_{proc.returncode}"
            temporary.replace(generated)
        payload.rmdir()  # extract_archive creates and validates the destination.
        ok, detail, _ = extract_archive(generated, payload)
        return ok, "rpm2archive:" + detail

    rpm = subprocess.Popen(["rpm2cpio", str(path.resolve())], cwd=payload,
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    assert rpm.stdout is not None
    cpio = subprocess.Popen(["cpio", "-idm", "--quiet", "--no-absolute-filenames"],
                            cwd=payload, stdin=rpm.stdout, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    rpm.stdout.close()
    _, cpio_err = cpio.communicate(timeout=3600)
    rpm_err = rpm.stderr.read() if rpm.stderr is not None else b""
    rpm_rc = rpm.wait(timeout=60)
    with LEDGER.open("a", encoding="utf-8") as ledger:
        ledger.write(f"PWD={payload}\nCOMMAND=rpm2cpio {path.resolve()} | cpio -idm --quiet --no-absolute-filenames\n")
        ledger.write(f"PIPE_EXIT_CODE={rpm_rc},{cpio.returncode}\n")
        if rpm_err:
            ledger.write("RPM2CPIO_STDERR=" + rpm_err.decode("utf-8", "replace").replace("\n", "\\n") + "\n")
        if cpio_err:
            ledger.write("CPIO_STDERR=" + cpio_err.decode("utf-8", "replace").replace("\n", "\\n") + "\n")
        ledger.write("\n")
    return rpm_rc == 0 and cpio.returncode == 0, f"rpm2cpio={rpm_rc},cpio={cpio.returncode}"


def extract_archive(path: Path, destination: Path) -> tuple[bool, str, int]:
    destination.mkdir(parents=True, exist_ok=False)
    try:
        if zipfile.is_zipfile(path):
            with zipfile.ZipFile(path) as archive:
                members = archive.infolist()
                safe = []
                skipped = 0
                for member in members:
                    mode = (member.external_attr >> 16) & 0o170000
                    if not safe_member(member.filename) or mode == 0o120000:
                        skipped += 1
                    else:
                        safe.append(member)
                for member in safe:
                    archive.extract(member, destination)
            return True, "ZIP_OK" if not skipped else "ZIP_OK_SKIPPED_UNSAFE_OR_LINK", skipped
        if tarfile.is_tarfile(path):
            with tarfile.open(path, "r:*") as archive:
                members = archive.getmembers()
                safe = [member for member in members if safe_member(member.name) and (member.isfile() or member.isdir())]
                skipped = len(members) - len(safe)
                archive.extractall(destination, members=safe)
            return True, "TAR_OK" if not skipped else "TAR_OK_SKIPPED_UNSAFE_OR_LINK", skipped
        destination.rmdir()
        return False, "NOT_SUPPORTED_ARCHIVE", 0
    except (OSError, tarfile.TarError, zipfile.BadZipFile) as error:
        shutil.rmtree(destination, ignore_errors=True)
        return False, f"EXTRACT_ERROR:{type(error).__name__}:{error}", 0


def likely_archive(path: Path) -> bool:
    lower = path.name.lower()
    return lower.endswith((
        ".tar", ".tar.gz", ".tgz", ".tar.bz2", ".tbz", ".tbz2", ".tar.xz", ".txz",
        ".zip", ".crate",
    ))


def main() -> int:
    manifest = read_tsv(MANIFEST)
    statuses = read_tsv(STATUS)
    candidates = read_tsv(CANDIDATES)
    archive_rows = read_tsv(ARCHIVES)
    done = {row["source_rpm"] for row in statuses if row["status"] == "SCAN_OK"}
    LEDGER.parent.mkdir(parents=True, exist_ok=True)

    for index, item in enumerate(manifest, 1):
        name = item["source_rpm"]
        if name in done:
            print(f"PROGRESS={index}/{len(manifest)} source={name} status=CHECKPOINT_SKIP", flush=True)
            continue
        free = shutil.disk_usage(ROOT).free
        if free < MIN_FREE_BYTES:
            print(f"RED_STOP_DISK free_bytes={free} minimum={MIN_FREE_BYTES}", flush=True)
            return 30
        shutil.rmtree(WORK, ignore_errors=True)
        payload = WORK / "payload"
        expanded = WORK / "expanded"
        payload.mkdir(parents=True)
        expanded.mkdir()
        source_path = ROOT / item["local_path"]
        verified = "YES" if source_path.is_file() and hash_file(source_path) == item["sha256"] else "NO"
        if verified != "YES":
            statuses.append({
                "source_rpm": name, "package_name": item["package_name"], "status": "NOT_AVAILABLE",
                "detail": "SOURCE_MISSING_OR_SHA256_MISMATCH", "compressed_bytes": item["compressed_bytes"],
                "sha256_verified": verified, "archives_seen": 0, "archives_expanded": 0, "candidates": 0,
            })
            write_tsv(STATUS, STATUS_FIELDS, statuses)
            continue
        ok, extraction_detail = extract_srpm(source_path, payload)
        if not ok:
            statuses.append({
                "source_rpm": name, "package_name": item["package_name"], "status": "NOT_AVAILABLE",
                "detail": extraction_detail, "compressed_bytes": item["compressed_bytes"],
                "sha256_verified": verified, "archives_seen": 0, "archives_expanded": 0, "candidates": 0,
            })
            write_tsv(STATUS, STATUS_FIELDS, statuses)
            print(f"PROGRESS={index}/{len(manifest)} source={name} status=NOT_AVAILABLE", flush=True)
            continue

        archives = sorted(path for path in payload.rglob("*") if path.is_file() and likely_archive(path))
        expanded_count = 0
        unsupported = []
        for archive_index, archive in enumerate(archives, 1):
            destination = expanded / f"{archive_index:03d}_{archive.name.replace('/', '_')}"
            archive_ok, detail, skipped = extract_archive(archive, destination)
            archive_rows.append({
                "source_rpm": name, "archive": str(archive.relative_to(payload)),
                "result": detail, "skipped_members": skipped,
            })
            if archive_ok:
                expanded_count += 1
            else:
                unsupported.append(f"{archive.name}:{detail}")
        write_tsv(ARCHIVES, ARCHIVE_FIELDS, archive_rows)

        one_status = WORK / "one_scan_status.tsv"
        write_tsv(one_status, ("source_rpm", "package_name", "root", "status", "rg_match_lines", "detail"), [{
            "source_rpm": name, "package_name": item["package_name"], "root": str(WORK),
            "status": "SCAN_OK", "rg_match_lines": "NOT_OBSERVED", "detail": "",
        }])
        one_candidates = WORK / "one_candidates.tsv"
        one_summary = WORK / "one_summary.tsv"
        proc = run([
            sys.executable, str(R87_SCANNER), "--scan-status", str(one_status),
            "--output", str(one_candidates), "--summary", str(one_summary),
            "--workers", "1", "--timeout-seconds", "1800",
        ], cwd=ROOT, timeout=2100)
        package_candidates = read_tsv(one_candidates) if one_candidates.exists() else []
        candidates.extend(package_candidates)
        write_tsv(CANDIDATES, CANDIDATE_FIELDS, candidates)
        if proc.returncode != 0:
            scan_status = "NOT_AVAILABLE"
            detail = f"R87_SCANNER_EXIT_{proc.returncode}"
        elif unsupported:
            scan_status = "NOT_AVAILABLE"
            detail = "UNSUPPORTED_SOURCE_ARCHIVE:" + ";".join(unsupported)
        else:
            scan_status = "SCAN_OK"
            detail = extraction_detail
        statuses.append({
            "source_rpm": name, "package_name": item["package_name"], "status": scan_status,
            "detail": detail, "compressed_bytes": item["compressed_bytes"],
            "sha256_verified": verified, "archives_seen": len(archives),
            "archives_expanded": expanded_count, "candidates": len(package_candidates),
        })
        write_tsv(STATUS, STATUS_FIELDS, statuses)
        if source_path.stat().st_size >= 4 * 1024**3 and scan_status == "SCAN_OK":
            BIG_STREAM_CACHE.unlink(missing_ok=True)
        print(
            f"PROGRESS={index}/{len(manifest)} source={name} status={scan_status} "
            f"candidates={len(package_candidates)} archives={expanded_count}/{len(archives)}",
            flush=True,
        )
        shutil.rmtree(WORK, ignore_errors=True)

    shutil.rmtree(WORK, ignore_errors=True)
    ok_count = sum(row["status"] == "SCAN_OK" for row in statuses)
    unavailable = len(statuses) - ok_count
    print(f"FINAL scan_ok={ok_count} not_available={unavailable} candidates={len(candidates)}", flush=True)
    return 0 if unavailable == 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
