#!/usr/bin/env python3
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUTPUT = ROOT / "progress/R103/inputs/source_git"
TARGETS = {
    "gmp": ("product/upstream/gmp", "tizen_base", "490fe651469987ccb00b5cba908d69609345e226"),
    "tensorflow2-base": ("platform/upstream/tensorflow2", "accepted/tizen_base_toolchain", "1cdba73549f741720f11dd53da1bb516b03763a2"),
    "eigen": ("platform/upstream/eigen", "tizen", "e8e4707a11713b39909fc9db916b5db1ecbfdf6a"),
    "python-pycrypto": ("platform/upstream/python-pycrypto", "tizen", "fa4a7de575838f0884364ebd05423927f9f79734"),
    "inference-engine-tflite": ("platform/core/multimedia/inference-engine-tflite", "tizen", "e59b18b7575bf713023442a51d386bec006f2e16"),
    "nnstreamer": ("platform/upstream/nnstreamer", "accepted/tizen_unified_toolchain", "40ae293fe1e293e5d9bfc332e4b293a27ddcb1a4"),
    "nntrainer": ("platform/core/ml/nntrainer", "accepted/tizen_unified_toolchain", "97ddb43dcd9e5173d7cb4245ff9bae5d760e5834"),
}


def run(command, cwd=None, check=True):
    print("COMMAND", " ".join(command), flush=True)
    return subprocess.run(command, cwd=cwd, check=check)


OUTPUT.mkdir(parents=True, exist_ok=True)
for name, (repo, branch, revision) in TARGETS.items():
    target = OUTPUT / name
    if target.exists():
        actual = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=target, text=True).strip()
        if actual == revision:
            print(f"REUSE\t{name}\t{repo}\t{revision}", flush=True)
            continue
        raise RuntimeError(f"refusing to replace existing path: {target} head={actual}")
    url = f"ssh://lhmax2025@review.tizen.org:29418/{repo}"
    run(
        [
            "git", "clone", "--filter=blob:none", "--no-checkout", "--depth", "1",
            "--branch", branch, url, str(target),
        ]
    )
    if run(["git", "cat-file", "-e", f"{revision}^{{commit}}"], cwd=target, check=False).returncode:
        run(["git", "fetch", "--depth", "1", "origin", revision], cwd=target)
    run(["git", "sparse-checkout", "set", "packaging"], cwd=target)
    run(["git", "checkout", "--detach", revision], cwd=target)
    actual = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=target, text=True).strip()
    if actual != revision:
        raise RuntimeError(f"revision mismatch {name}: expected={revision} actual={actual}")
    specs = sorted(str(path.relative_to(target)) for path in target.glob("packaging/*.spec"))
    print(f"RESULT\t{name}\t{repo}\t{revision}\t{';'.join(specs) or 'NO_SPEC'}", flush=True)
