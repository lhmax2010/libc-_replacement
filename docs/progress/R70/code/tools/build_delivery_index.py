#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import pathlib

ROOT = pathlib.Path("/home/toolchain/development/libc++_replacement")
DELIVERY = ROOT / "docs/progress/R70"


def source_for(relative: str) -> str:
    if relative == "REPORT.md":
        return "progress/R70/REPORT.md"
    if relative.startswith("code/tools/"):
        return "progress/R70/tools/" + relative.removeprefix("code/tools/")
    if relative == "code/tests/r69_new_cases.cpp":
        return "progress/R69/src/r69_new_cases.cpp"
    if relative.startswith("code/tests/"):
        return "docs/progress/R69/code/tests/" + relative.removeprefix("code/tests/")
    if relative.startswith("code/config/"):
        return "docs/progress/R69/code/build_config/" + relative.removeprefix("code/config/")
    if relative == "code/runtime_identity/armv7l_ldd_ld_debug.log":
        return "progress/R70/raw/armv7l/matrix/identity/runtime.log"
    if relative.startswith("code/runtime_identity/"):
        return "docs/progress/R69/code/runtime_identity/" + relative.removeprefix("code/runtime_identity/")
    prefix = "code/evidence/commands/"
    if relative.startswith(prefix):
        return "progress/R70/commands/" + relative.removeprefix(prefix)
    prefix = "code/evidence/raw/"
    if relative.startswith(prefix):
        return "progress/R70/raw/" + relative.removeprefix(prefix)
    prefix = "code/evidence/tables/"
    if relative.startswith(prefix):
        return "progress/R70/tables/" + relative.removeprefix(prefix)
    prefix = "code/evidence/monitor/"
    if relative.startswith(prefix):
        return "progress/R70/monitor/" + relative.removeprefix(prefix)
    prefix = "code/evidence/reused_x86_64/"
    if relative.startswith(prefix):
        tail = relative.removeprefix(prefix)
        if tail in {"023_run_x86_matrix.log", "027_run_x86_upstream.log"}:
            return "progress/R69/raw/" + tail
        return "progress/R69/raw/x86_64/" + tail
    prefix = "code/evidence/build-probes-"
    if relative.startswith(prefix):
        return "docs/progress/R69/code/evidence/raw/" + relative.removeprefix("code/evidence/")
    prefix = "code/evidence/upstream-build-"
    if relative.startswith(prefix):
        return "docs/progress/R69/code/evidence/raw/" + relative.removeprefix("code/evidence/")
    if relative == "code/evidence/original_no_counterpart_8.tsv":
        return "docs/progress/R44b/tables/no_counterpart_8.tsv"
    return "NOT_MAPPED"


def digest(path: pathlib.Path) -> str:
    hasher = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            hasher.update(block)
    return hasher.hexdigest()


def main() -> int:
    rows: list[tuple[str, str, str, int]] = []
    for path in sorted(DELIVERY.rglob("*")):
        if not path.is_file() or path.name in {"INDEX.tsv", "INDEX.tsv.sha256"}:
            continue
        relative = path.relative_to(DELIVERY).as_posix()
        rows.append((relative, source_for(relative), digest(path), path.stat().st_size))
    with (DELIVERY / "INDEX.tsv").open("w") as out:
        out.write("upload_path\tsource_path\tsha256\tsize_bytes\n")
        for row in rows:
            out.write("\t".join(map(str, row)) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
