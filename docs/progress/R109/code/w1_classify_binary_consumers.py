#!/usr/bin/env python3
import argparse
import csv
from collections import Counter, defaultdict
from pathlib import Path


def read_rows(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--callers", required=True, type=Path)
    parser.add_argument("--inventory", required=True, type=Path)
    parser.add_argument("--sources", required=True, type=Path)
    parser.add_argument("--migration", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()

    inventory = {}
    for row in read_rows(args.inventory):
        key = (row["repo_id"], row["name"], row["arch"], row["path"])
        inventory[key] = row["sourcerpm"]
    source_results = {}
    for row in read_rows(args.sources):
        identity = Path(row["location"]).name
        source_results[identity] = row["result"]
    migration = {row["source_rpm_identity"]: row for row in read_rows(args.migration)}

    grouped = defaultdict(lambda: {"symbols": set(), "tables": set()})
    for row in read_rows(args.callers):
        if row["scope"] != "PLATFORM" or row["state"] != "UND":
            continue
        key = (row["repo"], row["package"], row["arch"], row["elf"])
        grouped[key]["symbols"].add(row["demangled_symbol"])
        grouped[key]["tables"].add(row["table"])

    output = []
    for key, values in sorted(grouped.items()):
        sourcerpm = inventory.get(key, "")
        if not sourcerpm:
            source_result = "NOT_AVAILABLE_IN_R11_MAPPING"
        else:
            source_result = source_results.get(sourcerpm, "NOT_AVAILABLE_IN_R11_SOURCE_INDEX")
        mig = migration.get(sourcerpm)
        if mig:
            rebuild_class = "R24_REBUILD_PLAN"
            stage = mig["stage"]
            vendor_prebuilt = mig["vendor_prebuilt"]
        else:
            rebuild_class = "OUTSIDE_R24_REBUILD_PLAN"
            stage = "-"
            vendor_prebuilt = "NOT_OBSERVED"
        output.append({
            "repo": key[0], "package": key[1], "arch": key[2], "elf": key[3],
            "source_rpm": sourcerpm or "NOT_AVAILABLE",
            "source_result": source_result,
            "rebuild_class": rebuild_class, "migration_stage": stage,
            "vendor_prebuilt": vendor_prebuilt,
            "symbols": ";".join(sorted(values["symbols"])),
            "symbol_tables": ";".join(sorted(values["tables"])),
        })

    fields = list(output[0]) if output else []
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(output)

    classes = Counter(r["rebuild_class"] for r in output)
    with args.summary.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("metric", "value"))
        writer.writerow(("consumer_elfs", len(output)))
        writer.writerow(("consumer_binary_packages", len({r["package"] for r in output})))
        writer.writerow(("consumer_source_rpms", len({r["source_rpm"] for r in output})))
        for key in sorted(classes):
            writer.writerow((f"consumer_elfs_{key}", classes[key]))
            writer.writerow((f"binary_packages_{key}", len({r['package'] for r in output if r['rebuild_class'] == key})))
            writer.writerow((f"source_rpms_{key}", len({r['source_rpm'] for r in output if r['rebuild_class'] == key})))
        writer.writerow(("source_mapping_missing", sum(r["source_rpm"] == "NOT_AVAILABLE" for r in output)))
        writer.writerow(("source_download_not_pass", sum(r["source_result"] != "PASS" for r in output)))
    print(f"CONSUMER_ELFS={len(output)}")
    print(f"MAPPED={sum(r['source_rpm'] != 'NOT_AVAILABLE' for r in output)}")
    print(f"R24={classes['R24_REBUILD_PLAN']}")
    return 0 if output and all(r["source_rpm"] != "NOT_AVAILABLE" for r in output) else 2


if __name__ == "__main__":
    raise SystemExit(main())
