#!/usr/bin/env python3
"""Compare an omitted --stdlib run with an explicit libstdc++ run."""

import argparse
import hashlib
import subprocess
import tarfile
import tempfile
from pathlib import Path


ARCHES = ("armv7l", "aarch64", "x86_64")
KINDS = ("runtime", "inliner")


def digest(path: Path) -> str:
    value = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            value.update(chunk)
    return value.hexdigest()


def inventory(archive: Path, destination: Path) -> dict:
    destination.mkdir(parents=True)
    with tarfile.open(archive, "r:gz") as handle:
        handle.extractall(destination, filter="data")
    return {
        str(path.relative_to(destination)): digest(path)
        for path in destination.rglob("*")
        if path.is_file() and not path.name.startswith("MANIFEST-")
    }


def archive_members(archive: Path, destination: Path) -> dict:
    destination.mkdir(parents=True)
    subprocess.run(["ar", "x", archive.resolve()], cwd=destination, check=True)
    return {
        path.name: digest(path)
        for path in destination.iterdir()
        if path.is_file()
    }


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--explicit", type=Path, required=True)
    parser.add_argument("--omitted", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    rows = ["kind\tarch\texplicit_files\tomitted_files\tmissing\textra\thash_differences"]
    archive_rows = [
        "arch\texplicit_members\tomitted_members\tmissing\textra\tmember_hash_differences"
    ]
    with tempfile.TemporaryDirectory() as temporary:
        temp = Path(temporary)
        for kind in KINDS:
            for arch in ARCHES:
                name = f"mlgo_{kind}_{arch}.tar.gz"
                left = inventory(args.explicit / name, temp / kind / arch / "explicit")
                right = inventory(args.omitted / name, temp / kind / arch / "omitted")
                common = set(left) & set(right)
                changed = sorted(path for path in common if left[path] != right[path])
                rows.append("\t".join(map(str, [
                    kind, arch, len(left), len(right),
                    len(set(left) - set(right)), len(set(right) - set(left)),
                    len(changed),
                ])))
                for path in changed:
                    rows.append("\t".join([
                        "DETAIL", kind, arch, path, left[path], right[path]
                    ]))
                if kind == "runtime":
                    archive_path = "mlgo_sysroot/xla_aot_runtime_src/libtf_xla_runtime_prebuilt.a"
                    left_members = archive_members(
                        temp / kind / arch / "explicit" / archive_path,
                        temp / kind / arch / "explicit-members",
                    )
                    right_members = archive_members(
                        temp / kind / arch / "omitted" / archive_path,
                        temp / kind / arch / "omitted-members",
                    )
                    member_common = set(left_members) & set(right_members)
                    member_changed = {
                        name for name in member_common
                        if left_members[name] != right_members[name]
                    }
                    archive_rows.append("\t".join(map(str, [
                        arch, len(left_members), len(right_members),
                        len(set(left_members) - set(right_members)),
                        len(set(right_members) - set(left_members)),
                        len(member_changed),
                    ])))
    args.output.write_text("\n".join(rows) + "\n")
    args.output.with_name("default_archive_member_equivalence.tsv").write_text(
        "\n".join(archive_rows) + "\n"
    )
    print("\n".join(rows))
    print("\n".join(archive_rows))


if __name__ == "__main__":
    main()
