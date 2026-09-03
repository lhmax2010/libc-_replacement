#!/usr/bin/env python3
import shutil
import subprocess
import sys
from pathlib import Path


TARGETS = {
    "abseil-cpp": ("platform/upstream/abseil-cpp", "9c39c516dca54c786ddc5da45a5a15acf41ef75e"),
    "bcc-tools": ("platform/upstream/bcc", "97128ee3dbccd2851cfa94dc723edb2abcbefc1d"),
    "boost": ("platform/upstream/boost", "58b70f335aec5641540dbe04dfd56fad5af43658"),
    "bpftrace": ("platform/upstream/bpftrace", "30e51cd665360f85b665308dc13ba27df0c5f739"),
    "icu": ("platform/upstream/icu", "e4a4d7411b0f288581cf020a176b5dddbb504ae7"),
    "jsoncpp": ("platform/upstream/jsoncpp", "21c9dcd2bc8f51f8bfe5173b49780dc707eef9f9"),
    "libsigc++": ("platform/upstream/libsigc++", "5f177f7c98ea29833989b5d0d6a79fece9701545"),
    "llvm": ("platform/upstream/llvm", "08a64b6eb95ed74339f1d862a6c22a2f93fa78f9"),
    "pcre": ("platform/upstream/pcre", "feba6a19627f5d6e74d551ef2fc8b33af0547780"),
    "taglib": ("platform/upstream/taglib", "4e7de646d897ed5a92111799058b21705a3bd34c"),
}


def run(args, cwd=None):
    print("COMMAND", " ".join(args), flush=True)
    subprocess.run(args, cwd=cwd, check=True)


if len(sys.argv) != 2:
    raise SystemExit("usage: fetch_target_packaging_git.py OUTPUT_DIR")

root = Path(sys.argv[1]).resolve()
root.mkdir(parents=True, exist_ok=True)
for name, (repo, expected) in TARGETS.items():
    target = root / name
    if target.exists():
        shutil.rmtree(target)
    url = f"ssh://lhmax2025@review.tizen.org:29418/{repo}"
    run([
        "git", "clone", "--filter=blob:none", "--no-checkout", "--depth", "1",
        "--branch", "tizen_base", url, str(target),
    ])
    actual = subprocess.check_output(
        ["git", "rev-parse", "HEAD"], cwd=target, text=True
    ).strip()
    if actual != expected:
        print(
            f"BRANCH_HEAD_DIFFERS\t{name}\tbranch={actual}\tsnapshot={expected}",
            flush=True,
        )
        run(["git", "fetch", "--depth", "1", "origin", expected], cwd=target)
    run(["git", "cat-file", "-e", f"{expected}^{{commit}}"], cwd=target)
    run(["git", "sparse-checkout", "set", "packaging"], cwd=target)
    run(["git", "checkout", "--detach", expected], cwd=target)
    specs = sorted(str(path.relative_to(target)) for path in target.glob("packaging/*.spec"))
    if not specs:
        raise RuntimeError(f"{name}: no packaging/*.spec")
    print(f"RESULT\t{name}\t{repo}\t{expected}\t{';'.join(specs)}", flush=True)
