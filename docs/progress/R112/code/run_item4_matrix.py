#!/usr/bin/env python3
import csv
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[4]
OUT = ROOT / "docs/progress/R112/build/item4"
RUNS = OUT / "runs"
TABLES = ROOT / "docs/progress/R112/tables"
EXTRACTED = Path("/home/toolchain/development/libc++_replacement/tmp/R11/extracted")
QEMU = EXTRACTED / "41/4154ea903c230bf1f887ef759c43e5f7dcb48de108fd2445550ce1eaff3af553/usr/bin/qemu-aarch64"
GLIBC = EXTRACTED / "76/7606e2955a44b9255ca19db9bfc1ff9b96c047c5dd1aee7a85472a0a67af84a7"
LIBCXX = Path("/home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/R105-boost-aarch64/local/BUILD-ROOTS/scratch.aarch64.0/usr/lib64")
LIBSTD = Path("/tmp/r110_w3_build/root/usr/lib64")
LIBGCC = Path("/tmp/r110_w3_build/root/usr/lib64")
PROVIDER = Path("/home/toolchain/development/libc++_replacement/progress/R105/artifacts/boost_aarch64_libcxx/boost-filesystem-1.83.0-105.10.5.aarch64/usr/lib64")


def main() -> int:
    RUNS.mkdir(parents=True, exist_ok=True)
    TABLES.mkdir(parents=True, exist_ok=True)
    loader = GLIBC / "lib64/ld-linux-aarch64.so.1"
    library_path = ":".join(map(str, (PROVIDER, LIBCXX, LIBSTD, LIBGCC, GLIBC / "lib64")))
    rows = []
    for cell in ("libstd_consumer_libcxx_provider", "libcxx_consumer_libcxx_provider"):
        for repeat in range(1, 6):
            command = [str(QEMU), str(loader), "--library-path", library_path, str(OUT / cell)]
            completed = subprocess.run(command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
                                       stderr=subprocess.PIPE, timeout=10)
            stem = RUNS / f"{cell}_{repeat}"
            stem.with_suffix(".stdout").write_text(completed.stdout)
            stem.with_suffix(".stderr").write_text(completed.stderr)
            stem.with_suffix(".exitcode").write_text(f"{completed.returncode}\n")
            stem.with_suffix(".command.txt").write_text(f"cwd: {ROOT}\ncommand: {' '.join(command)}\n")
            rows.append({
                "cell": cell, "repeat": repeat, "returncode": completed.returncode,
                "maps_libcxx": int("runtime_maps=libcxx:1" in completed.stdout),
                "maps_libstdcxx": int("libstdcxx:1" in completed.stdout),
                "maps_provider": int("provider:1" in completed.stdout),
                "exact_value": int("status_ok=1 value_ok=1 returned_size=11 returned=payload.txt" in completed.stdout),
                "destruction_completed": int("event=destruction_completed" in completed.stdout),
            })
    with (TABLES / "item4_actual_direction_matrix.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    for cell in sorted({str(row["cell"]) for row in rows}):
        selected = [row for row in rows if row["cell"] == cell]
        print(cell, "returncodes", sorted({int(row["returncode"]) for row in selected}),
              "maps", [(row["maps_libcxx"], row["maps_libstdcxx"], row["maps_provider"]) for row in selected],
              "exact", sum(int(row["exact_value"]) for row in selected),
              "destroy", sum(int(row["destruction_completed"]) for row in selected))
    controls = [row for row in rows if row["cell"].startswith("libcxx_consumer")]
    assert all(row["returncode"] == 0 and row["maps_libcxx"] and not row["maps_libstdcxx"] and
               row["maps_provider"] and row["exact_value"] and row["destruction_completed"] for row in controls)
    mixed = [row for row in rows if row["cell"].startswith("libstd_consumer")]
    assert all(row["maps_libcxx"] and row["maps_libstdcxx"] and row["maps_provider"] for row in mixed)
    print("ASSERTIONS=PASS architecture=aarch64_qemu repeats=5 control=PASS maps=PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
