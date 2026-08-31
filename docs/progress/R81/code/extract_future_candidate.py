#!/usr/bin/env python3
import glob
import json
import os

TEST = "std/thread/futures/futures.async/thread_create_failure.pass.cpp"
OUT = "progress/R81/raw/phase2/candidate_future"
os.makedirs(OUT, exist_ok=True)


def load_test(path):
    with open(path, encoding="utf-8") as f:
        data = json.load(f)
    for test in data.get("tests", []):
        name = test.get("name", "")
        if name == TEST or name.endswith("/" + TEST) or name.endswith(" :: " + TEST):
            return test
    return None


inputs = [
    "progress/R81/raw/phase2/results/baseline_armv7l/libcxx/aggregate_result_owner_normalized.json"
]
inputs += sorted(glob.glob("progress/R77/raw/results/patched_armv7l/libcxx/batch-*/result.json"))
inputs += sorted(glob.glob("progress/R68/raw/full_run/results/*/libcxx/result.json"))
inputs += sorted(glob.glob("progress/R68/raw/full_run/results/*/libcxx/batch-*/result.json"))

seen = set()
rows = []
for path in inputs:
    if path in seen or not os.path.isfile(path):
        continue
    seen.add(path)
    test = load_test(path)
    if test is None:
        continue
    label = path.replace("/", "_").replace(".json", "")
    output = test.get("output", "")
    with open(os.path.join(OUT, label + ".output.txt"), "w", encoding="utf-8") as f:
        f.write(output)
    with open(os.path.join(OUT, label + ".entry.json"), "w", encoding="utf-8") as f:
        json.dump(test, f, ensure_ascii=False, indent=2)
        f.write("\n")
    rows.append((path, test.get("code"), len(output), test.get("elapsed")))

with open(os.path.join(OUT, "index.tsv"), "w", encoding="utf-8") as f:
    f.write("path\tcode\toutput_bytes\telapsed\n")
    for row in rows:
        f.write("\t".join(map(str, row)) + "\n")

for row in rows:
    print("\t".join(map(str, row)))
