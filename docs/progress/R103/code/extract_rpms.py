#!/usr/bin/env python3
import csv
import os
import subprocess
import sys
from pathlib import Path

csv.field_size_limit(sys.maxsize)
ROOT = Path(__file__).resolve().parents[3]
CONSUMER_STATUS = ROOT / "progress/R103/tables/consumer_download_status.tsv"
CONSUMER_RPMS = ROOT / "progress/R103/downloads/consumers"
CONSUMER_OUT = ROOT / "progress/R103/extracted/consumers"
ARCHIVE_OUT = ROOT / "progress/R103/extracted/provider_archives"
TARGET_ROOT = ROOT / "progress/R100/downloads/static_archives/base"


def extract(rpm_path, destination, patterns=None):
    destination.mkdir(parents=True, exist_ok=True)
    rpm2cpio = subprocess.Popen(
        ["rpm2cpio", str(rpm_path)], stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    command = ["cpio", "-idm", "--quiet", "--no-preserve-owner"]
    if patterns:
        command.extend(patterns)
    cpio = subprocess.run(
        command,
        cwd=destination,
        stdin=rpm2cpio.stdout,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=False,
    )
    assert rpm2cpio.stdout is not None
    rpm2cpio.stdout.close()
    rpm_stderr = rpm2cpio.stderr.read().decode("utf-8", "replace")
    rpm_rc = rpm2cpio.wait()
    cpio_stderr = cpio.stderr.decode("utf-8", "replace")
    if rpm_rc != 0 or cpio.returncode != 0:
        raise RuntimeError(
            f"extract failed rpm={rpm_path} rpm2cpio_rc={rpm_rc} "
            f"cpio_rc={cpio.returncode} rpm2cpio_stderr={rpm_stderr!r} "
            f"cpio_stderr={cpio_stderr!r}"
        )


archive_specs = [
    ("gmp-devel", "libgmpxx.a"),
    ("libtensorflow2-aot-runtime", "libtf_xla_runtime.a"),
    ("tensorflow2-lite-devel", "libtensorflow2-lite.a"),
]
for package, archive in archive_specs:
    rpms = sorted(TARGET_ROOT.glob(f"{package}-*.rpm"))
    if len(rpms) != 3:
        raise RuntimeError(f"expected 3 RPMs for {package}, found {len(rpms)}")
    for rpm_path in rpms:
        arch = rpm_path.name.removesuffix(".rpm").rsplit(".", 1)[-1]
        destination = ARCHIVE_OUT / arch / package
        extract(
            rpm_path,
            destination,
            [f"./usr/lib/{archive}", f"./usr/lib64/{archive}"],
        )
        matches = list(destination.rglob(archive))
        if len(matches) != 1:
            raise RuntimeError(
                f"expected one {archive} in {destination}, found {len(matches)}"
            )
        print(f"PROVIDER\t{package}\t{arch}\t{matches[0].relative_to(ROOT)}")

with CONSUMER_STATUS.open(newline="", encoding="utf-8") as stream:
    rows = list(csv.DictReader(stream, delimiter="\t"))

for index, row in enumerate(rows, 1):
    if row["source"] == "NOT_AVAILABLE":
        raise RuntimeError(f"consumer RPM unavailable: {row}")
    rpm_path = CONSUMER_RPMS / Path(row["location"]).name
    if not rpm_path.is_file():
        raise RuntimeError(f"consumer RPM missing: {rpm_path}")
    destination = CONSUMER_OUT / row["source_name"] / row["arch"] / row["binary_name"]
    extract(rpm_path, destination)
    regular_files = sum(1 for path in destination.rglob("*") if path.is_file())
    print(
        f"CONSUMER\t{index}/{len(rows)}\t{row['source_name']}\t{row['arch']}\t"
        f"{row['binary_name']}\tfiles={regular_files}"
    )
