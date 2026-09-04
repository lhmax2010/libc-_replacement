#!/usr/bin/env python3
from pathlib import Path

root = Path(__file__).resolve().parents[1]
failures = []
checks = 0


def require(condition: bool, message: str) -> None:
    global checks
    checks += 1
    if not condition:
        failures.append(message)


for name in ("REPORT.md", "W1_REPORT.md", "W2_REPORT.md", "W3_REPORT.md", "README.md"):
    require((root / name).is_file(), f"missing:{name}")

raw = root / "raw"
for command in raw.glob("*.command.txt"):
    stem = command.name.removesuffix(".command.txt")
    if stem.startswith("060") and "validate_artifacts" in stem:
        continue
    for suffix in ("stdout", "stderr", "exitcode"):
        require((raw / f"{stem}.{suffix}").is_file(), f"missing_log:{stem}.{suffix}")


def metrics(path: Path) -> dict[str, str]:
    rows = path.read_text(errors="replace").splitlines()[1:]
    result = {}
    for row in rows:
        fields = row.split("\t")
        if len(fields) == 2:
            result[fields[0]] = fields[1]
        elif len(fields) == 3:
            result[f"{fields[0]}.{fields[1]}"] = fields[2]
    return result


w1 = metrics(root / "tables/w1_published_elf_summary.tsv")
require(w1.get("package_records") == "11332", "w1_package_denominator")
require(w1.get("elf_records") == "39166", "w1_elf_denominator")
require(w1.get("matching_symbol_rows") == "0", "w1_target_rows")
require(w1.get("parse_failures") == "0", "w1_parse_failures")

w2s = metrics(root / "tables/w2_static_summary.tsv")
require(w2s.get("PUBLISHED_STATIC_ARCHIVE.TOTAL") == "8911", "w2_static_total")
require(w2s.get("PUBLISHED_STATIC_ARCHIVE.NM_EXIT_0") == "8906", "w2_static_readable")
require(w2s.get("PUBLISHED_STATIC_ARCHIVE.NM_EXIT_1") == "5", "w2_static_opaque")
require(w2s.get("PUBLISHED_STATIC_ARCHIVE.TARGET_NO") == "8911", "w2_static_target")

w2 = metrics(root / "tables/w2_inventory_summary.tsv")
require(w2.get("SOURCE_EMBEDDED_OBJECT.TOTAL") == "3236", "w2_object_total")
require(w2.get("SOURCE_EMBEDDED_SHARED_CANDIDATE.TOTAL") == "1027", "w2_shared_total")

for number in ("052", "056"):
    outputs = list(raw.glob(f"{number}_*.stdout"))
    require(len(outputs) == 1, f"w3_output_count:{number}")
    if outputs:
        text = outputs[0].read_text(errors="replace")
        require(text.count("run=") == 5, f"w3_runs:{number}")
        require(text.count("normal_control=PASS") == 5, f"w3_normal:{number}")
        require(text.count("cancellation_cases=5/5") == 5, f"w3_cases:{number}")

status = root.parent / "RUNTIME_STATUS_0905.md"
require("IN_PROGRESS" not in status.read_text(), "status_in_progress")

print(f"checks={checks}")
print(f"failures={len(failures)}")
for failure in failures:
    print(f"failure={failure}")
raise SystemExit(1 if failures else 0)
