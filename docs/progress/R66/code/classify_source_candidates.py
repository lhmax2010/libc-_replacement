#!/usr/bin/env python3
"""Classify production-like source hits by ownership and migration relevance."""

from __future__ import annotations

import argparse
import csv
import pathlib
import re


ABI_API = re.compile(r"\b(?:__cxxabiv1|abi)::(__[A-Za-z0-9_]+)")
CONDITIONAL = re.compile(r"^\s*#\s*(?:if|ifdef|ifndef|elif)\b|\bdefined\s*\(")


def all_text(root: pathlib.Path) -> str:
    return "\n".join(
        path.read_text(errors="replace")
        for path in root.rglob("*")
        if path.is_file() and path.stat().st_size < 10 * 1024 * 1024
    )


def write_tsv(path: pathlib.Path, header, rows) -> None:
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(header)
        writer.writerows(rows)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--hits", type=pathlib.Path, required=True)
    parser.add_argument("--gcc-include", type=pathlib.Path, required=True)
    parser.add_argument("--libcxx-include", type=pathlib.Path, required=True)
    parser.add_argument("--libcxxabi-include", type=pathlib.Path, required=True)
    parser.add_argument("--output", type=pathlib.Path, required=True)
    parser.add_argument("--package-summary", type=pathlib.Path, required=True)
    args = parser.parse_args()

    abi_text = all_text(args.libcxxabi_include)
    rows = []
    confirmed_by_package = {}
    with args.hits.open(newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["scope"] != "PRODUCTION_LIKE":
                continue
            family = row["family"]
            token = row["token"]
            text = row["text"].replace("\\t", "\t").replace("\\n", "\n")
            disposition = "CANDIDATE_REQUIRES_SEMANTIC_REVIEW"
            detail = ""
            confirmed = False
            if family == "LIBSTDCXX_INTERNAL_HEADER":
                gcc_path = args.gcc_include / token
                libcxx_path = args.libcxx_include / token
                if gcc_path.exists() and not libcxx_path.exists():
                    disposition = "CONFIRMED_LIBSTDCXX_HEADER_DEPENDENCY"
                    detail = str(gcc_path)
                    confirmed = True
                elif gcc_path.exists() and libcxx_path.exists():
                    disposition = "SAME_PATH_AVAILABLE_IN_LIBCXX"
                    detail = str(libcxx_path)
                else:
                    disposition = "PREFIX_COLLISION_NOT_LIBSTDCXX_HEADER"
            elif family == "EXPLICIT_LIBSTDCXX_LINK":
                disposition = "CONFIRMED_BUILD_CONFIGURATION_DEPENDENCY"
                confirmed = True
            elif family == "GNU_EXTENSION_NAMESPACE":
                disposition = "CONFIRMED_GNU_EXTENSION_DEPENDENCY"
                confirmed = True
            elif family == "LIBSTDCXX_ABI_NAMESPACE":
                disposition = "CONFIRMED_LIBSTDCXX_ABI_NAMESPACE_DEPENDENCY"
                confirmed = True
            elif family == "STD_PRIVATE_MEMBER":
                disposition = "CONFIRMED_RESERVED_STD_MEMBER_REFERENCE"
                confirmed = True
            elif family == "LIBSTDCXX_CONFIG_MACRO":
                if CONDITIONAL.search(text):
                    disposition = "PORTABILITY_DISPATCH_OR_FEATURE_PROBE"
                else:
                    disposition = "CONFIG_MACRO_NONCONDITIONAL_CANDIDATE"
            elif family == "CXX_ABI_RUNTIME_API":
                apis = sorted(set(ABI_API.findall(text)))
                if not apis:
                    disposition = "CXXABI_HEADER_ONLY_OR_UNPARSED"
                    detail = "selected platform pairs libc++ with libc++abi"
                else:
                    missing = [api for api in apis if api not in abi_text]
                    if missing:
                        disposition = "CXXABI_API_MISSING_IN_LIBCXXABI_HEADER"
                        detail = ";".join(missing)
                        confirmed = True
                    else:
                        disposition = "CXXABI_API_AVAILABLE_IN_LIBCXXABI_HEADER"
                        detail = ";".join(apis)
            rows.append((
                row["source_rpm"], row["known45"], family, token,
                disposition, detail, row["file"], row["line"], row["text"],
            ))
            if confirmed:
                package = row["source_rpm"]
                item = confirmed_by_package.setdefault(package, {"known": row["known45"], "kinds": set(), "locations": set()})
                item["kinds"].add(disposition)
                item["locations"].add(f'{row["file"]}:{row["line"]}')

    write_tsv(
        args.output,
        ("source_rpm", "known45", "family", "token", "disposition", "detail", "file", "line", "text"),
        rows,
    )
    package_rows = []
    for package, item in sorted(confirmed_by_package.items()):
        package_rows.append((
            package, item["known"], len(item["kinds"]), ";".join(sorted(item["kinds"])),
            len(item["locations"]), ";".join(sorted(item["locations"])),
        ))
    write_tsv(
        args.package_summary,
        ("source_rpm", "known45", "confirmed_dependency_kind_count", "confirmed_dependency_kinds", "location_count", "locations"),
        package_rows,
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
