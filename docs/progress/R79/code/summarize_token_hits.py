#!/usr/bin/env python3
from collections import Counter, defaultdict
from pathlib import Path


source = Path("progress/R79/tables/rg_all_token_hits.txt")
output = Path("progress/R79/tables/package_token_counts.tsv")
tokens = (
    "pthread_setcanceltype",
    "PTHREAD_CANCEL_ASYNCHRONOUS",
    "PTHREAD_CANCEL_DEFERRED",
    "pthread_setcancelstate",
    "pthread_cancel",
)
counts = defaultdict(Counter)
files = defaultdict(set)
for line in source.read_text(errors="replace").splitlines():
    path, _, text = line.split(":", 2)
    relative = path.removeprefix("tmp/corpus/extracted/")
    package, file_path = relative.split("/", 1)
    files[package].add(file_path)
    for token in tokens:
        if token in text:
            counts[package][token] += 1

with output.open("w") as stream:
    stream.write("package\thit_files\t" + "\t".join(tokens) + "\n")
    for package in sorted(counts):
        stream.write(
            package
            + f"\t{len(files[package])}\t"
            + "\t".join(str(counts[package][token]) for token in tokens)
            + "\n"
        )

print(f"packages={len(counts)}")
print(f"files={sum(len(value) for value in files.values())}")
print(f"lines={sum(1 for _ in source.open(errors='replace'))}")
