#!/usr/bin/env python3
"""Inventory source-tree differences and require a rationale for every hunk.

Exit codes:
  0  every observed difference has a non-empty rationale
  2  one or more differences have no rationale (or a stale rationale exists)
  10 usage, input, or script error

The inventory mode intentionally does not make a gate decision.  It emits the
stable hunk identifiers needed to prepare the rationale TSV, then exits 0.
"""

from __future__ import annotations

import argparse
import difflib
import hashlib
import os
from pathlib import Path
import stat
import sys
from typing import Iterable, NamedTuple


class Difference(NamedTuple):
    path: str
    kind: str
    old_range: str
    new_range: str
    body: tuple[str, ...]

    @property
    def hunk_id(self) -> str:
        material = "\0".join(
            (self.path, self.kind, self.old_range, self.new_range, "\n".join(self.body))
        ).encode("utf-8", "surrogateescape")
        return "H-" + hashlib.sha256(material).hexdigest()[:16]


DEFAULT_EXCLUDES = (".git", "__pycache__")


def fail(message: str) -> "None":
    print(f"TREE_DIFF_GATE_ERROR: {message}", file=sys.stderr)
    raise SystemExit(10)


def excluded(relative: Path, patterns: tuple[str, ...]) -> bool:
    return any(part in patterns for part in relative.parts) or relative.name.endswith(".pyc")


def inventory(
    root: Path, patterns: tuple[str, ...], include_paths: tuple[str, ...]
) -> dict[str, Path]:
    result: dict[str, Path] = {}
    starts = (root,) if not include_paths else tuple(root / item for item in include_paths)
    for start in starts:
        if not start.exists():
            fail(f"included path does not exist below {root}: {start.relative_to(root)}")
        if start.is_file() or start.is_symlink():
            relative = start.relative_to(root)
            if not excluded(relative, patterns):
                result[relative.as_posix()] = start
            continue
        for directory, names, files in os.walk(start, followlinks=False):
            directory_path = Path(directory)
            relative_directory = directory_path.relative_to(root)
            names[:] = sorted(
                name
                for name in names
                if not excluded(relative_directory / name, patterns)
            )
            for name in sorted(files):
                relative = relative_directory / name
                if not excluded(relative, patterns):
                    result[relative.as_posix()] = directory_path / name
    return result


def mode_string(path: Path | None) -> str:
    if path is None:
        return "MISSING"
    value = path.lstat().st_mode
    if stat.S_ISLNK(value):
        return "SYMLINK"
    return oct(stat.S_IMODE(value))


def bytes_for(path: Path | None) -> bytes | None:
    if path is None:
        return None
    if path.is_symlink():
        return os.readlink(path).encode("utf-8", "surrogateescape")
    return path.read_bytes()


def text_lines(data: bytes) -> list[str] | None:
    if b"\0" in data:
        return None
    try:
        return data.decode("utf-8", "surrogateescape").splitlines(keepends=True)
    except UnicodeDecodeError:
        return None


def parse_hunks(path: str, kind: str, diff: list[str]) -> list[Difference]:
    result: list[Difference] = []
    current_header: str | None = None
    current_body: list[str] = []

    def flush() -> None:
        nonlocal current_header, current_body
        if current_header is None:
            return
        pieces = current_header.split()
        result.append(
            Difference(
                path,
                kind,
                pieces[1] if len(pieces) > 1 else "NOT_AVAILABLE",
                pieces[2] if len(pieces) > 2 else "NOT_AVAILABLE",
                tuple(current_body),
            )
        )
        current_header = None
        current_body = []

    for line in diff:
        if line.startswith("@@ "):
            flush()
            current_header = line.rstrip("\n")
        elif current_header is not None:
            current_body.append(line.rstrip("\n"))
    flush()
    return result


def compare(
    old_root: Path,
    new_root: Path,
    patterns: tuple[str, ...],
    include_paths: tuple[str, ...],
) -> tuple[list[Difference], list[str]]:
    old_files = inventory(old_root, patterns, include_paths)
    new_files = inventory(new_root, patterns, include_paths)
    differences: list[Difference] = []
    patch: list[str] = []

    for relative in sorted(old_files.keys() | new_files.keys()):
        old_path = old_files.get(relative)
        new_path = new_files.get(relative)
        old_mode = mode_string(old_path)
        new_mode = mode_string(new_path)
        old_data = bytes_for(old_path)
        new_data = bytes_for(new_path)
        if old_data == new_data and old_mode == new_mode:
            continue

        kind = "modified"
        if old_path is None:
            kind = "added"
        elif new_path is None:
            kind = "deleted"
        elif old_mode != new_mode:
            kind = "mode-or-type-changed"

        old_text = text_lines(old_data or b"")
        new_text = text_lines(new_data or b"")
        if old_text is None or new_text is None:
            old_hash = "MISSING" if old_data is None else hashlib.sha256(old_data).hexdigest()
            new_hash = "MISSING" if new_data is None else hashlib.sha256(new_data).hexdigest()
            body = (
                f"old_mode={old_mode}",
                f"new_mode={new_mode}",
                f"old_sha256={old_hash}",
                f"new_sha256={new_hash}",
            )
            item = Difference(relative, kind + "-binary", "BINARY", "BINARY", body)
            differences.append(item)
            patch.extend((f"diff --tree-gate a/{relative} b/{relative}\n", *(line + "\n" for line in body)))
            continue

        diff = list(
            difflib.unified_diff(
                old_text,
                new_text,
                fromfile=f"a/{relative}",
                tofile=f"b/{relative}",
                n=3,
            )
        )
        if old_mode != new_mode:
            diff.insert(0, f"mode {old_mode} -> {new_mode}\n")
        patch.extend(diff)
        parsed = parse_hunks(relative, kind, diff)
        if parsed:
            differences.extend(parsed)
        else:
            differences.append(
                Difference(relative, kind, "MODE", "MODE", (f"mode {old_mode} -> {new_mode}",))
            )
    return differences, patch


def read_rationales(path: Path) -> dict[str, tuple[str, str]]:
    if not path.is_file():
        fail(f"rationale file is not readable: {path}")
    result: dict[str, tuple[str, str]] = {}
    lines = path.read_text(encoding="utf-8").splitlines()
    if not lines or lines[0] != "hunk_id\ttask\treason":
        fail("rationale TSV header must be: hunk_id\\ttask\\treason")
    for number, line in enumerate(lines[1:], 2):
        if not line.strip():
            continue
        fields = line.split("\t", 2)
        if len(fields) != 3:
            fail(f"rationale TSV line {number} does not have three fields")
        hunk_id, task, reason = fields
        if not hunk_id or not task.strip() or not reason.strip():
            fail(f"rationale TSV line {number} has an empty required field")
        if hunk_id in result:
            fail(f"duplicate rationale for {hunk_id}")
        result[hunk_id] = (task.strip(), reason.strip())
    return result


def write_inventory(path: Path, differences: Iterable[Difference]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as stream:
        stream.write("hunk_id\tpath\tkind\told_range\tnew_range\n")
        for item in differences:
            stream.write(
                f"{item.hunk_id}\t{item.path}\t{item.kind}\t{item.old_range}\t{item.new_range}\n"
            )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--old", required=True, type=Path)
    parser.add_argument("--new", required=True, type=Path)
    parser.add_argument("--diff-output", required=True, type=Path)
    parser.add_argument("--hunks-output", required=True, type=Path)
    parser.add_argument("--rationale", type=Path)
    parser.add_argument("--inventory-only", action="store_true")
    parser.add_argument("--exclude", action="append", default=[])
    parser.add_argument(
        "--include",
        action="append",
        default=[],
        help="compare only this relative path below each tree (repeatable)",
    )
    args = parser.parse_args()

    if args.inventory_only and args.rationale is not None:
        fail("--inventory-only and --rationale are mutually exclusive")
    if not args.inventory_only and args.rationale is None:
        fail("gate mode requires --rationale; use --inventory-only to prepare it")
    old_root = args.old.resolve()
    new_root = args.new.resolve()
    if not old_root.is_dir() or not new_root.is_dir():
        fail("--old and --new must both name existing directories")

    patterns = tuple(dict.fromkeys((*DEFAULT_EXCLUDES, *args.exclude)))
    include_paths = tuple(dict.fromkeys(args.include))
    for item in include_paths:
        candidate = Path(item)
        if candidate.is_absolute() or ".." in candidate.parts:
            fail(f"--include must be a relative path without '..': {item}")
    differences, patch = compare(old_root, new_root, patterns, include_paths)
    args.diff_output.parent.mkdir(parents=True, exist_ok=True)
    args.diff_output.write_text("".join(patch), encoding="utf-8", errors="surrogateescape")
    write_inventory(args.hunks_output, differences)

    print(f"old_tree={old_root}")
    print(f"new_tree={new_root}")
    print(f"difference_hunks={len(differences)}")
    print(f"diff_output={args.diff_output}")
    print(f"hunks_output={args.hunks_output}")
    print(f"excluded_names={','.join(patterns)}")
    print(f"included_paths={','.join(include_paths) if include_paths else 'ALL'}")
    if args.inventory_only:
        print("gate_result=INVENTORY_ONLY")
        return 0

    rationales = read_rationales(args.rationale)
    observed = {item.hunk_id for item in differences}
    missing = sorted(observed - rationales.keys())
    stale = sorted(rationales.keys() - observed)
    print(f"rationale_file={args.rationale.resolve()}")
    print(f"missing_rationales={len(missing)}")
    for hunk_id in missing:
        print(f"MISSING_RATIONALE\t{hunk_id}")
    print(f"stale_rationales={len(stale)}")
    for hunk_id in stale:
        print(f"STALE_RATIONALE\t{hunk_id}")
    if missing or stale:
        print("gate_result=FAIL")
        return 2
    print("gate_result=PASS")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except OSError as error:
        fail(str(error))
