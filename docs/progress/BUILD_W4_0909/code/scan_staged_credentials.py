#!/usr/bin/env python3
"""Fail if staged regular files contain QuickBuild credentials or auth headers."""

from __future__ import annotations

import subprocess
from pathlib import Path


def main() -> int:
    credential_path = Path(".quickbuild-credentials")
    values = [line for line in credential_path.read_bytes().splitlines() if line]
    labels = [b"USERNAME", b"PASSWORD"]
    patterns = list(zip(labels, values))
    patterns.extend([
        (b"AUTHORIZATION_HEADER", b"Author" + b"ization:"),
        (b"COOKIE_HEADER", b"Coo" + b"kie:"),
        (b"SET_COOKIE_HEADER", b"Set-" + b"Coo" + b"kie:"),
    ])
    names = subprocess.check_output(
        ["git", "diff", "--cached", "--name-only", "--diff-filter=ACMR", "-z"]
    ).split(b"\0")
    findings = []
    scanned = 0
    for raw_name in filter(None, names):
        name = raw_name.decode("utf-8", errors="surrogateescape")
        try:
            content = subprocess.check_output(["git", "show", f":{name}"], stderr=subprocess.DEVNULL)
        except subprocess.CalledProcessError:
            continue
        scanned += 1
        for label, pattern in patterns:
            if pattern and pattern in content:
                findings.append((name, label.decode("ascii")))
    print(f"SCANNED_STAGED_FILES={scanned}")
    print(f"CREDENTIAL_MATCH_COUNT={len(findings)}")
    for name, label in findings:
        print(f"MATCH={label}\t{name}")
    print("CREDENTIAL_SCAN=PASS" if not findings else "CREDENTIAL_SCAN=FAIL")
    return 0 if not findings else 1


if __name__ == "__main__":
    raise SystemExit(main())
