#!/usr/bin/env python3
"""Assemble the current three-file LLVM MLGO interface from split packages.

The regenerated inliner and runtime payloads are used.  Regalloc is copied
unchanged from the existing combined package because it is outside this run's
scope and has no libstdc++/libc++ ABI symbols.
"""

import argparse
import shutil
import tarfile
import tempfile
from pathlib import Path


ARCH_NAMES = {
    "armv7l": "arm",
    "aarch64": "aarch",
    "x86_64": "x86",
}


def extract(archive: Path, destination: Path) -> None:
    with tarfile.open(archive, "r:gz") as handle:
        handle.extractall(destination, filter="data")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--old-dir", type=Path, required=True)
    parser.add_argument("--runtime-dir", type=Path, required=True)
    parser.add_argument("--inliner-dir", type=Path, required=True)
    parser.add_argument("--out-dir", type=Path, required=True)
    args = parser.parse_args()
    args.out_dir.mkdir(parents=True, exist_ok=True)

    for arch, old_stem in ARCH_NAMES.items():
        old_archive = args.old_dir / f"mlgo_{old_stem}_model.tar.gz"
        runtime_archive = args.runtime_dir / f"mlgo_runtime_{arch}.tar.gz"
        inliner_archive = args.inliner_dir / f"mlgo_inliner_{arch}.tar.gz"
        for required in (old_archive, runtime_archive, inliner_archive):
            if not required.is_file():
                raise SystemExit(f"missing input: {required}")

        with tempfile.TemporaryDirectory(dir=args.out_dir) as temporary:
            temporary_path = Path(temporary)
            old = temporary_path / "old"
            runtime = temporary_path / "runtime"
            inliner = temporary_path / "inliner"
            stage = temporary_path / "stage"
            for directory in (old, runtime, inliner, stage):
                directory.mkdir()
            extract(old_archive, old)
            extract(runtime_archive, runtime)
            extract(inliner_archive, inliner)

            for child in runtime.iterdir():
                if not child.name.startswith("MANIFEST-"):
                    shutil.move(str(child), stage / child.name)
            for name in ("InlinerSizeModel.h", "InlinerSizeModel.o"):
                shutil.copy2(inliner / name, stage / name)
            for name in ("RegAllocEvictModel.h", "RegAllocEvictModel.o"):
                shutil.copy2(old / name, stage / name)

            output = args.out_dir / f"mlgo_{old_stem}_model.tar.gz"
            with tarfile.open(output, "w:gz") as handle:
                for child in sorted(stage.iterdir(), key=lambda item: item.name):
                    handle.add(child, arcname=f"./{child.name}")
            print(f"{arch}\t{output}\t{output.stat().st_size}")


if __name__ == "__main__":
    main()
