#!/usr/bin/env python3
import csv
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
EDGE_TABLE = ROOT / "docs/progress/R82/tables/boost_edges_90.tsv"
OLD = Path("/home/toolchain/development/libc++_replacement/tmp/R11/extracted/56/56604a6f573ded2b8b135a76fd11c9e1569d7a53224b62d9b91e932bbb2af665/usr/lib64/libboost_program_options.so.1.83.0")
NEW = Path("/home/toolchain/development/libc++_replacement/progress/R105/artifacts/boost_aarch64_libcxx/boost-program-options-1.83.0-105.10.5.aarch64/usr/lib64/libboost_program_options.so.1.83.0")
OUTPUT = ROOT / "docs/progress/R112/tables/item4_e029_symbol_retention.tsv"

def defined(path: Path):
    output = subprocess.run(["nm", "-D", "--defined-only", str(path)], check=True,
                            text=True, stdout=subprocess.PIPE).stdout
    return {line.split()[-1].split("@@", 1)[0] for line in output.splitlines() if line.split()}

def main() -> int:
    with EDGE_TABLE.open(newline="") as stream:
        edge = next(row for row in csv.DictReader(stream, delimiter="\t")
                    if row["edge_id"] == "E029")
    symbols = edge["cpp_symbols"].split(";")
    old, new = defined(OLD), defined(NEW)
    rows = [{"symbol": symbol, "old_provider_defined": int(symbol in old),
             "adapted_provider_same_mangled_defined": int(symbol in new)}
            for symbol in symbols]
    with OUTPUT.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    print("edge=E029 consumer=security-manager arch=aarch64 recorded", len(rows),
          "old_defined", sum(r["old_provider_defined"] for r in rows),
          "adapted_same_mangled_defined", sum(r["adapted_provider_same_mangled_defined"] for r in rows),
          "missing", sum(not r["adapted_provider_same_mangled_defined"] for r in rows))
    print("missing_symbols:")
    for row in rows:
        if not row["adapted_provider_same_mangled_defined"]:
            print(row["symbol"])
    assert len(rows) == 33
    assert sum(r["old_provider_defined"] for r in rows) == 33
    assert sum(r["adapted_provider_same_mangled_defined"] for r in rows) == 21
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
