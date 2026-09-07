#!/usr/bin/env python3
import csv
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
EDGES = ROOT / "docs/progress/R82/tables/boost_edges_90.tsv"
SPLIT = ROOT / "docs/progress/R22/tables/split"
EXTRACTED = Path("/home/toolchain/development/libc++_replacement/tmp/R11/extracted")
OUTPUT = ROOT / "docs/progress/R112/tables/item4_consumer_binding_tags.tsv"

def candidate_rows():
    for path in sorted(SPLIT.glob("candidate_edge_evidence.part*.tsv")):
        with path.open(newline="") as stream:
            for row in csv.DictReader(stream, delimiter="\t"):
                yield row

def main() -> int:
    with EDGES.open(newline="") as stream:
        edges = list(csv.DictReader(stream, delimiter="\t"))
    wanted = {(e["consumer_source_rpm"], e["arch"], e["consumer_binary"],
               e["consumer_elf"], e["provider_binary"], e["provider_elf"])
              for e in edges}
    hashes = {}
    for row in candidate_rows():
        key = (row["consumer_source_rpm"], row["arch"], row["consumer_binary"],
               row["consumer_elf"], row["provider_binary"], row["provider_elf"])
        if key in wanted:
            hashes[key] = row["consumer_rpm_sha256"]
    assert len(hashes) == len(wanted), (len(hashes), len(wanted))
    unique = {}
    for edge in edges:
        key = (edge["consumer_source_rpm"], edge["arch"], edge["consumer_binary"],
               edge["consumer_elf"], edge["provider_binary"], edge["provider_elf"])
        sha = hashes[key]
        unique[(edge["arch"], sha, edge["consumer_elf"])] = edge
    rows = []
    for (arch, sha, elf_path), edge in sorted(unique.items()):
        path = EXTRACTED / sha[:2] / sha / elf_path.lstrip("/")
        exists = path.is_file()
        dynamic = subprocess.run(["readelf", "-d", str(path)], text=True,
                                 stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout if exists else ""
        program = subprocess.run(["readelf", "-W", "-l", str(path)], text=True,
                                 stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout if exists else ""
        sections = subprocess.run(["readelf", "-W", "-S", str(path)], text=True,
                                  stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout if exists else ""
        rows.append({
            "arch": arch, "consumer_source_rpm": edge["consumer_source_rpm"],
            "consumer_binary": edge["consumer_binary"], "consumer_elf": elf_path,
            "consumer_sha256": sha, "file_available": int(exists),
            "gnu_relro": int("GNU_RELRO" in program),
            "bind_now": int("BIND_NOW" in dynamic or "Flags: NOW" in dynamic),
            "jmprel": int("(JMPREL)" in dynamic),
            "plt_section": int(".plt" in sections),
        })
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    with OUTPUT.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t")
        writer.writeheader()
        writer.writerows(rows)
    print(f"edge_rows={len(edges)} unique_consumer_elf={len(rows)}")
    for arch in sorted({row["arch"] for row in rows}):
        selected = [row for row in rows if row["arch"] == arch]
        print(arch, "count", len(selected), "available", sum(r["file_available"] for r in selected),
              "relro", sum(r["gnu_relro"] for r in selected),
              "now", sum(r["bind_now"] for r in selected),
              "jmprel", sum(r["jmprel"] for r in selected),
              "plt", sum(r["plt_section"] for r in selected))
    assert all(row["file_available"] for row in rows)
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
