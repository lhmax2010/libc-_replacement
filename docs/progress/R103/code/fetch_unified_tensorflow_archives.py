#!/usr/bin/env python3
import hashlib
import shutil
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "progress/R103/downloads/unified_tensorflow"
BASE = (
    "https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/"
    "tizen-unified-toolchain_20260829.015247/repos/standard/packages/"
)
ROWS = [
    ("aarch64/tensorflow2-lite-devel-2.18.0-1.aarch64.rpm", "bf60f566fa275866acbb1eef2157da827f2b6031c507f08c08e778d4e9d4280f"),
    ("armv7l/tensorflow2-lite-devel-2.18.0-1.armv7l.rpm", "59b38a877c7103e2d192c77fc54867288bce082a05d6a57f6c0454d812d91f5c"),
    ("x86_64/tensorflow2-lite-devel-2.18.0-1.x86_64.rpm", "34f02f5d85ad1621606fd83628030e458a3ca8119ee801ab3fe448f1699877b2"),
]


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            value.update(block)
    return value.hexdigest()


OUT.mkdir(parents=True, exist_ok=True)
for location, expected in ROWS:
    target = OUT / Path(location).name
    if not target.exists() or digest(target) != expected:
        temporary = target.with_suffix(target.suffix + ".part")
        with urllib.request.urlopen(BASE + location, timeout=120) as source, temporary.open("wb") as sink:
            shutil.copyfileobj(source, sink, 1024 * 1024)
        actual = digest(temporary)
        if actual != expected:
            raise RuntimeError(f"SHA256 mismatch: {target.name} expected={expected} actual={actual}")
        temporary.replace(target)
    print(f"PASS\t{location}\t{target.stat().st_size}\t{digest(target)}")
