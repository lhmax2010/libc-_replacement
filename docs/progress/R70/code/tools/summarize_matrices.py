#!/usr/bin/env python3
from __future__ import annotations

import collections
import pathlib
import re

ROOT = pathlib.Path("/home/toolchain/development/libc++_replacement")


def normalize(name: str) -> tuple[str, str]:
    variant = "unknown"
    for candidate in ("baseline", "fixed", "reference"):
        prefix = candidate + "_"
        if name.startswith(prefix):
            variant = candidate
            name = name[len(prefix) :]
            break
        if name == candidate:
            variant = candidate
            name = "default"
            break
    return variant, name


def normalize_iteration(group: str, case: str) -> str:
    if group in {"new_cases", "async", "concurrency"}:
        return re.sub(r"_\d+$", "", case)
    if group in {"m1_concurrency", "m2_lock"} and case.isdigit():
        return "default"
    return case


def parse_x86() -> dict[tuple[str, str, str], list[int]]:
    rows: dict[tuple[str, str, str], list[int]] = collections.defaultdict(list)
    sources = [
        ROOT / "progress/R69/raw/023_run_x86_matrix.log",
        ROOT / "progress/R69/raw/027_run_x86_upstream.log",
    ]
    for source in sources:
        for line in source.read_text(errors="replace").splitlines():
            match = re.fullmatch(r"([A-Za-z0-9_]+)\t([^\t]+)\t([0-9]+)", line)
            if not match:
                continue
            group, name, rc_text = match.groups()
            if group in {"libcxx", "libcxxabi", "libstdcxx"}:
                case = f"{group}_{name}"
                variant = "reference" if group == "libstdcxx" else "fixed"
                group = "upstream"
            else:
                variant, case = normalize(name)
            case = normalize_iteration(group, case)
            rows[(group, case, variant)].append(int(rc_text))
    for log in sorted((ROOT / "progress/R70/raw/x86_64/waiter_baseline").glob("*.log")):
        values = re.findall(r"^EXIT_CODE=([0-9]+)$", log.read_text(errors="replace"), re.M)
        if values:
            rows[("new_cases", "waiting_side_cancel", "baseline")].append(int(values[-1]))
    return rows


def parse_arm() -> dict[tuple[str, str, str], list[int]]:
    rows: dict[tuple[str, str, str], list[int]] = collections.defaultdict(list)
    source = ROOT / "progress/R70/commands/011_run_armv7l_matrix.log"
    for line in source.read_text(errors="replace").splitlines():
        match = re.fullmatch(r"([A-Za-z0-9_]+)\t([^\t]+)\t([^\t]+)\t([0-9]+)", line)
        if not match:
            continue
        group, name, stated_variant, rc_text = match.groups()
        inferred_variant, case = normalize(name)
        case = normalize_iteration(group, case)
        variant = inferred_variant if inferred_variant != "unknown" else stated_variant
        rows[(group, case, variant)].append(int(rc_text))
    return rows


def distribution(values: list[int]) -> str:
    counts = collections.Counter(values)
    return ",".join(f"{key}:{counts[key]}" for key in sorted(counts))


def write_summary(path: pathlib.Path, rows: dict[tuple[str, str, str], list[int]]) -> None:
    with path.open("w") as out:
        out.write("group\tcase\tvariant\tpass\ttotal\texit_distribution\n")
        for key in sorted(rows):
            values = rows[key]
            out.write("\t".join((*key, str(values.count(0)), str(len(values)), distribution(values))) + "\n")


def main() -> int:
    tables = ROOT / "progress/R70/tables"
    tables.mkdir(parents=True, exist_ok=True)
    x86 = parse_x86()
    arm = parse_arm()
    write_summary(tables / "x86_64_summary.tsv", x86)
    write_summary(tables / "armv7l_summary.tsv", arm)
    with (tables / "cross_architecture.tsv").open("w") as out:
        out.write("group\tcase\tvariant\tx86_pass\tx86_total\tx86_exit_distribution\tarm_pass\tarm_total\tarm_exit_distribution\tcomparison\n")
        for key in sorted(set(x86) | set(arm)):
            xv = x86.get(key, [])
            av = arm.get(key, [])
            if not xv or not av:
                comparison = "NOT_OBSERVED_ONE_ARCH"
            elif collections.Counter(xv) == collections.Counter(av):
                comparison = "IDENTICAL_EXIT_DISTRIBUTION"
            else:
                comparison = "DIFFERENT_EXIT_DISTRIBUTION"
            out.write(
                "\t".join(
                    (
                        *key,
                        str(xv.count(0)),
                        str(len(xv)),
                        distribution(xv) if xv else "NOT_OBSERVED",
                        str(av.count(0)),
                        str(len(av)),
                        distribution(av) if av else "NOT_OBSERVED",
                        comparison,
                    )
                )
                + "\n"
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
