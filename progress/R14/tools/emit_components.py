#!/usr/bin/env python3
import csv
import hashlib
import re
import shlex
import subprocess
from pathlib import Path

WORKSPACE = Path("/home/toolchain/development/libc++_replacement")
TMP = WORKSPACE / "tmp/R14"
TABLES = WORKSPACE / "progress/R14/tables"


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def run(argv):
    print(f"PWD={shlex.quote(str(WORKSPACE))}")
    print(f"COMMAND={shlex.join(str(item) for item in argv)}")
    completed = subprocess.run(
        [str(item) for item in argv],
        cwd=WORKSPACE,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    print(completed.stdout, end="")
    print(completed.stderr, end="")
    print(f"EXIT_CODE={completed.returncode}")
    if completed.returncode != 0:
        raise SystemExit(completed.returncode)
    return completed.stdout


def write_table(name, fields, rows):
    path = TABLES / name
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(
            stream, fieldnames=fields, delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)
    print(f"OUTPUT={path}")


def main():
    rpm_inputs = [
        (
            "scheme_b_platform_llvm_runtime",
            TMP / "downloads/libc++-22.1.8-19.1.x86_64.rpm",
            "d26445e70789d212f9eb59d31f36cad3aa3eae051f7f9712ec0aab118cf3bfe7",
        ),
        (
            "legacy_platform_libstdcpp",
            TMP / "downloads/libstdc++-14.2.0-1.13.x86_64.rpm",
            "b8155206a459a575f0f701c66a553cd2029728963fa3938768131e9028e4ee6b",
        ),
        (
            "platform_libgcc",
            TMP / "downloads/libgcc-14.2.0-1.13.x86_64.rpm",
            "fe622fb74e245c939c61ea5cca98dcbfc342fa7e12b1de0d8602be19ed35c441",
        ),
        (
            "scheme_a_project_libcxx",
            TMP / "downloads/libc++-22.1.8-1.x86_64.rpm",
            "2f37335ae5bfd7e83fd631a3b138d4ca564dc2c568325ebfa60197c72b73e95b",
        ),
        (
            "scheme_a_project_libcxxabi",
            TMP / "downloads/libc++abi-22.1.8-1.x86_64.rpm",
            "c141c89a8d7781cb0f06f035c73d56d4bf40fbedc54385767ab83afe8f0a151d",
        ),
    ]
    input_rows = []
    for role, path, expected in rpm_inputs:
        nevra = run(
            [
                "rpm",
                "-qp",
                "--qf",
                "%{NAME}-%{EPOCHNUM}:%{VERSION}-%{RELEASE}.%{ARCH}",
                path,
            ]
        )
        measured = sha256(path)
        input_rows.append(
            {
                "role": role,
                "nevra": nevra,
                "sha256": measured,
                "expected_sha256": expected,
                "sha_status": "MATCH" if measured == expected else "MISMATCH",
                "path": str(path.relative_to(WORKSPACE)),
            }
        )
    if any(row["sha_status"] != "MATCH" for row in input_rows):
        raise SystemExit(1)
    write_table(
        "input_rpm_identity.tsv",
        ["role", "nevra", "sha256", "expected_sha256", "sha_status", "path"],
        input_rows,
    )

    components = [
        ("B", "new_dso", TMP / "runtime/B/libnew_B.so"),
        ("B", "legacy_dso", TMP / "runtime/B/liblegacy.so"),
        ("B", "libcxxabi", TMP / "runtime/B/libc++abi.so.1.0"),
        ("B", "llvm_libunwind", TMP / "runtime/B/libunwind.so.1.0"),
        ("B", "libstdcpp", TMP / "runtime/B/libstdc++.so.6.0.33"),
        ("B", "libgcc_s", TMP / "runtime/B/libgcc_s.so.1"),
        ("A", "new_dso", TMP / "runtime/A/libnew_A.so"),
        ("A", "legacy_dso", TMP / "runtime/A/liblegacy.so"),
        ("A", "libcxxabi", TMP / "runtime/A/libc++abi.so.1.0"),
        ("A", "libstdcpp", TMP / "runtime/A/libstdc++.so.6.0.33"),
        ("A", "libgcc_s", TMP / "runtime/A/libgcc_s.so.1"),
    ]
    identity_rows = []
    needed_rows = []
    unwind_rows = []
    needed_pattern = re.compile(r"Shared library: \[([^]]+)\]")
    for scheme, role, path in components:
        dynamic = run(["readelf", "-d", path])
        symbols = run(["readelf", "--dyn-syms", "--wide", path])
        identity_rows.append(
            {
                "scheme": scheme,
                "role": role,
                "sha256": sha256(path),
                "size": path.stat().st_size,
                "path": str(path.relative_to(WORKSPACE)),
            }
        )
        for needed in needed_pattern.findall(dynamic):
            needed_rows.append(
                {
                    "scheme": scheme,
                    "role": role,
                    "needed": needed,
                    "path": str(path.relative_to(WORKSPACE)),
                }
            )
        for line in symbols.splitlines():
            if " UND " not in line or "_Unwind_" not in line:
                continue
            match = re.search(r"(_Unwind_\S+?)(?:\s+\(\d+\))?$", line)
            if not match:
                raise SystemExit(f"UNPARSED_UNWIND_SYMBOL: {line}")
            symbol_text = match.group(1)
            base = symbol_text.split("@", 1)[0]
            version = symbol_text.split("@", 1)[1].split()[0] if "@" in symbol_text else "UNVERSIONED"
            unwind_rows.append(
                {
                    "scheme": scheme,
                    "role": role,
                    "symbol": base,
                    "symbol_text": symbol_text,
                    "version": version,
                    "path": str(path.relative_to(WORKSPACE)),
                }
            )
    write_table(
        "component_identity.tsv",
        ["scheme", "role", "sha256", "size", "path"],
        identity_rows,
    )
    write_table(
        "component_dt_needed.tsv", ["scheme", "role", "needed", "path"], needed_rows
    )
    write_table(
        "component_unwind_undefined.tsv",
        ["scheme", "role", "symbol", "symbol_text", "version", "path"],
        unwind_rows,
    )

    source_rows = []
    for path in sorted((WORKSPACE / "progress/R14/experiment_sources").glob("*")):
        source_rows.append(
            {
                "sha256": sha256(path),
                "size": path.stat().st_size,
                "path": str(path.relative_to(WORKSPACE)),
            }
        )
    write_table("experiment_source_identity.tsv", ["sha256", "size", "path"], source_rows)


if __name__ == "__main__":
    main()
