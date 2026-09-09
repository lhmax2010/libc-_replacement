#!/usr/bin/env python3
"""Fail if staged blobs contain either QuickBuild credential line or session material."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
CREDENTIALS = ROOT / ".quickbuild-credentials"
SESSION_PATTERNS = (
    re.compile(b"JSESSION" + br"ID_[A-Za-z0-9_]*="),
    re.compile(br"(?im)^Set" + b"-Cookie:"),
    re.compile(br"(?im)^Coo" + b"kie:"),
    re.compile(br"(?im)^Author" + b"ization:"),
    re.compile(br"(?im)^Proxy-Author" + b"ization:"),
)


def git(*args: str) -> bytes:
    return subprocess.run(
        ["git", *args], cwd=ROOT, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    ).stdout


def main() -> int:
    if not CREDENTIALS.is_file():
        print("ERROR: .quickbuild-credentials is unavailable; staged credential scan cannot run.")
        return 2
    secrets = tuple(value for value in CREDENTIALS.read_bytes().splitlines() if value)
    if len(secrets) != 2:
        print("ERROR: credential file does not contain exactly two non-empty lines.")
        return 2
    paths = [
        Path(item.decode("utf-8", errors="surrogateescape"))
        for item in git("diff", "--cached", "--name-only", "-z").split(b"\0")
        if item
    ]
    hits: list[str] = []
    for path in paths:
        try:
            data = git("show", f":{path.as_posix()}")
        except subprocess.CalledProcessError:
            continue
        if any(secret in data for secret in secrets) or any(pattern.search(data) for pattern in SESSION_PATTERNS):
            hits.append(path.as_posix())
    print(f"staged_files_scanned={len(paths)}")
    print(f"credential_or_session_hits={len(hits)}")
    for path in hits:
        print(f"HIT {path}")
    return 2 if hits else 0


if __name__ == "__main__":
    raise SystemExit(main())
