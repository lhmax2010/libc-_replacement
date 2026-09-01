#!/usr/bin/env python3
import hashlib
import subprocess
import sys


if len(sys.argv) != 4:
    raise SystemExit("usage: compare_squash_files.py REPO VALIDATED SQUASH")

repo, validated, squash = sys.argv[1:]


def git(*args: str) -> bytes:
    return subprocess.check_output(["git", "-C", repo, *args])


parent = git("rev-parse", f"{squash}^").decode().strip()
changed = git("diff-tree", "--no-commit-id", "--name-only", "-r", squash).decode().splitlines()

print("path\tvalidated_present\tsquash_present\tvalidated_sha256\tsquash_sha256\tvalidated_size\tsquash_size\tbyte_equal")
all_equal = True
for path in changed:
    blobs = []
    for tree in (validated, squash):
        try:
            blobs.append(git("show", f"{tree}:{path}"))
        except subprocess.CalledProcessError:
            blobs.append(None)
    left, right = blobs
    equal = left == right
    all_equal &= equal
    fields = [
        path,
        "yes" if left is not None else "no",
        "yes" if right is not None else "no",
        hashlib.sha256(left).hexdigest() if left is not None else "-",
        hashlib.sha256(right).hexdigest() if right is not None else "-",
        str(len(left)) if left is not None else "-",
        str(len(right)) if right is not None else "-",
        "yes" if equal else "no",
    ]
    print("\t".join(fields))

print(f"SUMMARY\tchanged_files={len(changed)}\tall_byte_equal={'yes' if all_equal else 'no'}\tsquash_parent={parent}")
