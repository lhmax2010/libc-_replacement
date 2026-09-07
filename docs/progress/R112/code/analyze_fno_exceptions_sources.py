#!/usr/bin/env python3
import csv
import subprocess
from collections import defaultdict
from pathlib import Path

ROOT = Path("/home/toolchain/development/libc++_replacement/tmp/corpus/extracted")
OUTPUT = Path(__file__).resolve().parents[4] / "docs/progress/R112/tables/item1_fno_exceptions_source_occurrences.tsv"

def files_for(pattern: str):
    result = subprocess.run(["rg", "--no-ignore", "-l", "--fixed-strings", "--", pattern, str(ROOT)],
                            text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode not in (0, 1):
        raise RuntimeError(result.stderr)
    return [Path(line) for line in result.stdout.splitlines()]

def main() -> int:
    negative = files_for("-fno-exceptions")
    positive = files_for("-fexceptions")
    grouped = defaultdict(list)
    for path in negative:
        grouped[path.relative_to(ROOT).parts[0]].append(path)
    rows = []
    for package, paths in sorted(grouped.items()):
        spec = [path for path in paths if path.suffix == ".spec"]
        ninja = [path for path in paths if path.suffix == ".ninja"]
        build = [path for path in paths if path.name == "CMakeLists.txt" or
                 path.suffix in {".cmake", ".mk", ".gn", ".gyp", ".gypi"} or
                 path.name.startswith("Makefile")]
        rows.append({"source_rpm": package, "matching_files": len(paths),
                     "spec_files": len(spec), "generated_ninja_files": len(ninja),
                     "other_build_description_files": len(set(build) - set(ninja)),
                     "sample_file": str(paths[0].relative_to(ROOT / package))})
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    with OUTPUT.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    print("corpus_source_rpm_dirs", len([p for p in ROOT.iterdir() if p.is_dir()]))
    print("fno_matching_files", len(negative), "fno_source_rpms", len(grouped),
          "fno_spec_files", sum(r["spec_files"] for r in rows),
          "fno_generated_ninja_files", sum(r["generated_ninja_files"] for r in rows))
    print("positive_control_pattern=-fexceptions matching_files", len(positive),
          "source_rpms", len({p.relative_to(ROOT).parts[0] for p in positive}))
    for row in rows:
        print(row)
    assert positive
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
