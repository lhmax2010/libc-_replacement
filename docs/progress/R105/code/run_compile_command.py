#!/usr/bin/env python3
"""Run one captured cross-compiler command outside the build root.

Only absolute build-root paths are rebased. This avoids privileged namespace
operations while preserving the compiler, flags, source and output target.
"""

import argparse
import shlex
import subprocess
from pathlib import Path


parser = argparse.ArgumentParser()
parser.add_argument("--commands", type=Path, required=True)
parser.add_argument("--needle", required=True)
parser.add_argument("--scratch", type=Path, required=True)
parser.add_argument("--build-dir", type=Path, required=True)
parser.add_argument("--emulator")
args = parser.parse_args()

matches = [line.rstrip("\n") for line in args.commands.read_text().splitlines()
           if args.needle in line]
if not matches:
    raise SystemExit("captured command not found")

tokens = shlex.split(matches[-1])
scratch = str(args.scratch.resolve())


def rebase(value: str) -> str:
    for prefix in ("/home/", "/opt/", "/usr/include/"):
        if value.startswith(prefix):
            return scratch + value
    for option_prefix in ("-I/home/", "-I/opt/", "-I/usr/include/",
                          "-isystem/home/", "-isystem/opt/", "-isystem/usr/include/"):
        if value.startswith(option_prefix):
            slash = value.index("/")
            return value[:slash] + scratch + value[slash:]
    return value


compiler = tokens[0]
if not compiler.startswith("/bin/"):
    raise SystemExit(f"unexpected compiler path: {compiler}")
tokens[0] = scratch + compiler
tokens[1:] = [rebase(token) for token in tokens[1:]]
tokens.insert(1, "--sysroot=" + scratch)

if args.emulator:
    tokens = [args.emulator, "-L", scratch, *tokens]

print("EXEC=" + shlex.join(tokens))
completed = subprocess.run(tokens, cwd=args.build_dir)
print(f"EXIT_CODE={completed.returncode}")
raise SystemExit(completed.returncode)
