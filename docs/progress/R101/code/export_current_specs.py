#!/usr/bin/env python3
import subprocess
import sys
from pathlib import Path


SPEC_BY_SOURCE = {
    "abseil-cpp": "packaging/abseil-cpp.spec",
    "bcc-tools": "packaging/bcc-tools.spec",
    "boost": "packaging/boost.spec",
    "bpftrace": "packaging/bpftrace.spec",
    "icu": "packaging/icu.spec",
    "jsoncpp": "packaging/jsoncpp.spec",
    "libsigc++": "packaging/libsigc++.spec",
    "llvm": "packaging/llvm.spec",
    "pcre": "packaging/pcre.spec",
    "taglib": "packaging/taglib.spec",
}

if len(sys.argv) != 3:
    raise SystemExit("usage: export_current_specs.py REPO_ROOT OUTPUT_DIR")
repo_root = Path(sys.argv[1]).resolve()
output = Path(sys.argv[2]).resolve()
output.mkdir(parents=True, exist_ok=True)
for source, spec in SPEC_BY_SOURCE.items():
    repo = repo_root / source
    revision = subprocess.check_output(
        ["git", "rev-parse", "refs/remotes/origin/tizen_base"],
        cwd=repo,
        text=True,
    ).strip()
    content = subprocess.check_output(
        ["git", "show", f"refs/remotes/origin/tizen_base:{spec}"], cwd=repo
    )
    target = output / f"{source}.spec"
    target.write_bytes(content)
    print(f"{source}\t{revision}\t{spec}\t{target}\t{len(content)}")
