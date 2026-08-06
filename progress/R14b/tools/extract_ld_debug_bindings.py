#!/usr/bin/env python3
import csv
import re
from pathlib import Path

WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
TABLES = WORKSPACE / "progress/R14b/L2/tables"
TARGETS = {
    "_Unwind_RaiseException",
    "_Unwind_Resume",
    "_Unwind_DeleteException",
    "_Unwind_ForcedUnwind",
    "__cxa_throw",
    "__cxa_begin_catch",
    "__cxa_end_catch",
}
PATTERN = re.compile(
    r"binding file (.*?) \[\d+\] to (.*?) \[\d+\]: normal symbol `([^']+)'(?: \[([^]]+)\])?"
)


def joined(values):
    materialized = sorted(set(values))
    return " | ".join(materialized) if materialized else "NOT_OBSERVED"


def provider_class(path):
    if "libgcc_s.so" in path:
        return "LIBGCC_S"
    if "libunwind.so" in path:
        return "LLVM_LIBUNWIND"
    if "libstdc++.so" in path:
        return "LIBSTDCXX"
    if "libc++abi.so" in path:
        return "LIBCXXABI"
    return f"OTHER:{path}"


def main():
    with (TABLES / "matrix_all.tsv").open(newline="") as stream:
        cases = list(csv.DictReader(stream, delimiter="\t"))
    records = []
    summaries = []
    for case in cases:
        raw = WORKSPACE / case["raw_directory"]
        files = sorted(raw.glob("ld_debug.*"))
        case_records = []
        for path in files:
            for line_number, line in enumerate(path.read_text(errors="replace").splitlines(), 1):
                match = PATTERN.search(line)
                if not match or match.group(3) not in TARGETS:
                    continue
                record = {
                    "case_id": case["case_id"],
                    "stage": case["stage"],
                    "scheme": case["scheme"],
                    "mode": case["mode"],
                    "order": case["order"],
                    "scope": case["scope"],
                    "binding": case["binding"],
                    "direction": case["direction"],
                    "referrer": match.group(1),
                    "provider": match.group(2),
                    "provider_class": provider_class(match.group(2)),
                    "symbol": match.group(3),
                    "requested_version": match.group(4) or "NO_VERSION_SHOWN",
                    "source_file": str(path.relative_to(WORKSPACE)),
                    "source_line": line_number,
                    "raw_line": line.strip(),
                }
                records.append(record)
                case_records.append(record)

        def select(referrer_fragments, symbols):
            return [
                record
                for record in case_records
                if any(fragment in record["referrer"] for fragment in referrer_fragments)
                and record["symbol"] in symbols
            ]

        new_cxa = select(("libnew_",), {"__cxa_throw"})
        legacy_cxa = select(("liblegacy.so",), {"__cxa_throw"})
        new_unwind = select(
            ("libnew_", "libc++abi.so"),
            {symbol for symbol in TARGETS if symbol.startswith("_Unwind_")},
        )
        legacy_unwind = select(
            ("liblegacy.so", "libstdc++.so"),
            {symbol for symbol in TARGETS if symbol.startswith("_Unwind_")},
        )
        summaries.append(
            {
                "case_id": case["case_id"],
                "stage": case["stage"],
                "scheme": case["scheme"],
                "mode": case["mode"],
                "order": case["order"],
                "scope": case["scope"],
                "binding": case["binding"],
                "direction": case["direction"],
                "outcome": case["outcome"],
                "new_cxa_throw_provider": joined(
                    record["provider_class"] for record in new_cxa
                ),
                "legacy_cxa_throw_provider": joined(
                    record["provider_class"] for record in legacy_cxa
                ),
                "new_unwind_provider_classes": joined(
                    record["provider_class"] for record in new_unwind
                ),
                "legacy_unwind_provider_classes": joined(
                    record["provider_class"] for record in legacy_unwind
                ),
                "legacy_requested_versions": joined(
                    record["requested_version"] for record in legacy_unwind
                ),
                "binding_record_count": len(case_records),
            }
        )
    with (TABLES / "ld_debug_bindings.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, list(records[0]), delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(records)
    with (TABLES / "ld_debug_binding_summary.tsv").open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, list(summaries[0]), delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(summaries)
    print(f"CASE_COUNT={len(cases)}")
    print(f"BINDING_RECORD_COUNT={len(records)}")
    print(f"SUMMARY_COUNT={len(summaries)}")


if __name__ == "__main__":
    main()
