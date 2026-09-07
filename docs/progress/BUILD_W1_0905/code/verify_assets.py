#!/usr/bin/env python3
"""Compare regenerated MLGO payloads with the three existing combined assets."""

import argparse
import hashlib
import subprocess
import tarfile
import tempfile
from pathlib import Path


ARCH_NAMES = {"armv7l": "arm", "aarch64": "aarch", "x86_64": "x86"}


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def extract(archive: Path, destination: Path) -> None:
    with tarfile.open(archive, "r:gz") as handle:
        handle.extractall(destination, filter="data")


def files(root: Path) -> dict:
    return {
        str(path.relative_to(root)): path
        for path in root.rglob("*")
        if path.is_file()
    }


def symbols(root: Path) -> dict:
    result = {"defined": set(), "undefined": set()}
    for path in sorted(root.rglob("*")):
        if not path.is_file() or path.suffix not in {".o", ".a"}:
            continue
        proc = subprocess.run(
            ["nm", "-g", "--format=posix", str(path)],
            check=True, capture_output=True, text=True,
        )
        for line in proc.stdout.splitlines():
            fields = line.split()
            if len(fields) < 2 or fields[0].endswith(":"):
                continue
            name, kind = fields[0], fields[1]
            result["undefined" if kind.upper() == "U" else "defined"].add(name)
    return result


def demangle(names: set) -> dict:
    ordered = sorted(names)
    if not ordered:
        return {}
    proc = subprocess.run(
        ["c++filt"], input="\n".join(ordered) + "\n",
        check=True, capture_output=True, text=True,
    )
    return dict(zip(ordered, proc.stdout.splitlines()))


def write_lines(path: Path, values) -> None:
    path.write_text("\n".join(sorted(values)) + ("\n" if values else ""))


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--old-dir", type=Path, required=True)
    parser.add_argument("--default-dir", type=Path, required=True)
    parser.add_argument("--libcxx-dir", type=Path, required=True)
    parser.add_argument("--out-dir", type=Path, required=True)
    args = parser.parse_args()
    args.out_dir.mkdir(parents=True, exist_ok=True)

    summary = [
        "arch\tvariant\tfile_count\tpath_missing\tpath_extra\t"
        "nonbinary_hash_differences\tdefined_symbols\tundefined_symbols\t"
        "defined_missing_vs_old\tdefined_extra_vs_old\t"
        "undefined_missing_vs_old\tundefined_extra_vs_old\t"
        "std___1_symbols\tstd___cxx11_symbols"
    ]

    with tempfile.TemporaryDirectory() as temporary:
        temporary_path = Path(temporary)
        for arch, stem in ARCH_NAMES.items():
            archives = {
                "old": args.old_dir / f"mlgo_{stem}_model.tar.gz",
                "default": args.default_dir / f"mlgo_{stem}_model.tar.gz",
                "libcxx": args.libcxx_dir / f"mlgo_{stem}_model.tar.gz",
            }
            roots = {}
            inventories = {}
            symbol_sets = {}
            for variant, archive in archives.items():
                root = temporary_path / arch / variant
                root.mkdir(parents=True)
                extract(archive, root)
                roots[variant] = root
                inventories[variant] = files(root)
                symbol_sets[variant] = symbols(root)

            old_paths = set(inventories["old"])
            old_symbols = symbol_sets["old"]
            for variant in ("old", "default", "libcxx"):
                inv = inventories[variant]
                paths = set(inv)
                missing_paths = old_paths - paths
                extra_paths = paths - old_paths
                nonbinary_common = {
                    name for name in old_paths & paths
                    if not name.endswith((".o", ".a"))
                    and not name.startswith("MANIFEST-")
                }
                hash_diff = {
                    name for name in nonbinary_common
                    if sha256(inventories["old"][name]) != sha256(inv[name])
                }
                write_lines(args.out_dir / f"{arch}_{variant}_path_missing.txt", missing_paths)
                write_lines(args.out_dir / f"{arch}_{variant}_path_extra.txt", extra_paths)
                write_lines(args.out_dir / f"{arch}_{variant}_nonbinary_hash_diff.txt", hash_diff)

                diffs = {}
                for state in ("defined", "undefined"):
                    diffs[f"{state}_missing"] = old_symbols[state] - symbol_sets[variant][state]
                    diffs[f"{state}_extra"] = symbol_sets[variant][state] - old_symbols[state]
                    write_lines(args.out_dir / f"{arch}_{variant}_{state}_missing_vs_old.txt",
                                diffs[f"{state}_missing"])
                    write_lines(args.out_dir / f"{arch}_{variant}_{state}_extra_vs_old.txt",
                                diffs[f"{state}_extra"])

                all_symbols = symbol_sets[variant]["defined"] | symbol_sets[variant]["undefined"]
                demangled = demangle(all_symbols)
                std1 = {f"{raw}\t{text}" for raw, text in demangled.items() if "std::__1" in text}
                cxx11 = {f"{raw}\t{text}" for raw, text in demangled.items() if "std::__cxx11" in text}
                # Full missing/extra interface sets are retained above.  ABI
                # namespace files are evidence samples only: demangled deep
                # templates can make a complete listing hundreds of MiB.
                write_lines(args.out_dir / f"{arch}_{variant}_std___1.samples.tsv",
                            sorted(std1)[:50])
                write_lines(args.out_dir / f"{arch}_{variant}_std___cxx11.samples.tsv",
                            sorted(cxx11)[:50])

                summary.append("\t".join(map(str, [
                    arch, variant, len(inv), len(missing_paths), len(extra_paths),
                    len(hash_diff), len(symbol_sets[variant]["defined"]),
                    len(symbol_sets[variant]["undefined"]),
                    len(diffs["defined_missing"]), len(diffs["defined_extra"]),
                    len(diffs["undefined_missing"]), len(diffs["undefined_extra"]),
                    len(std1), len(cxx11),
                ])))

    (args.out_dir / "summary.tsv").write_text("\n".join(summary) + "\n")
    print("\n".join(summary))


if __name__ == "__main__":
    main()
