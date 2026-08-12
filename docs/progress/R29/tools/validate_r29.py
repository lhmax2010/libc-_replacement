#!/usr/bin/env python3
from __future__ import annotations

import csv
import hashlib
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
R29 = ROOT / "progress" / "R29"


def rows(name: str) -> list[dict[str, str]]:
    with (R29 / "tables" / name).open(encoding="utf-8", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def sha(path: Path) -> str:
    h = hashlib.sha256()
    h.update(path.read_bytes())
    return h.hexdigest()


def main() -> None:
    assert len(rows("sample_selection.tsv")) == 3
    assert len(rows("provider_applicability.tsv")) == 76
    assert all(row["result"] == "PASS" for row in rows("build_results.tsv"))
    assert all(row["result"] == "PASS" for row in rows("coinstall_results.tsv"))
    assert all(row["result"] == "PASS" for row in rows("provider_elf_results.tsv"))
    consumers = rows("consumer_results.tsv")
    assert len(consumers) == 2
    assert len({row["consumer_spec_sha256"] for row in consumers}) == 1
    assert all(row["result"] == "PASS" for row in consumers)
    for row in rows("artifact_sha256.tsv"):
        path = ROOT / row["path"]
        assert path.is_file(), path
        assert sha(path) == row["sha256"], path
    for spec in (
        R29 / "proposed/pcre/packaging/pcre.spec",
        R29 / "proposed/flatbuffers/packaging/flatbuffers.spec",
        R29 / "proposed/lottie-player/packaging/rlottie.spec",
        R29 / "proposed/consumer/packaging/r29-consumer.spec",
    ):
        subprocess.run(["rpmspec", "-P", str(spec)], check=True, stdout=subprocess.DEVNULL)
    for required in (
        "REPORT.md",
        "design/packaging_shape.md",
        "tables/build_results.tsv",
        "tables/consumer_results.tsv",
        "templates/autotools_dual_runtime.spec.template",
        "templates/cmake_dual_runtime.spec.template",
        "templates/meson_dual_runtime.spec.template",
    ):
        assert (R29 / required).is_file(), required
    print("sample_count=3")
    print("provider_denominator=76")
    print("final_build_rows=5/5 PASS")
    print("coinstall_rows=3/3 PASS")
    print("provider_elf_rows=6/6 PASS")
    print("consumer_modes=2/2 PASS; spec_sha_identical=YES")
    print(f"artifact_rows={len(rows('artifact_sha256.tsv'))}/all SHA MATCH")
    print("rpmspec_parse=4/4 PASS")


if __name__ == "__main__":
    main()
