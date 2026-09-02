#!/usr/bin/env python3
"""Classify members excluded by R92's safe tar extraction rule, without extraction."""

from __future__ import annotations

import argparse
import csv
import posixpath
import sys
import tarfile
from pathlib import PurePosixPath


def safe_name(name: str) -> bool:
    candidate = PurePosixPath(name)
    return not candidate.is_absolute() and ".." not in candidate.parts


def normalized(name: str) -> str | None:
    if name.startswith("/"):
        return None
    value = posixpath.normpath(name)
    if value == ".." or value.startswith("../"):
        return None
    while value.startswith("./"):
        value = value[2:]
    return value


def kind(member: tarfile.TarInfo) -> str:
    if member.issym():
        return "SYMLINK"
    if member.islnk():
        return "HARDLINK"
    if member.isfile():
        return "REGULAR_FILE"
    if member.isdir():
        return "DIRECTORY"
    if member.isfifo():
        return "FIFO"
    if member.ischr():
        return "CHAR_DEVICE"
    if member.isblk():
        return "BLOCK_DEVICE"
    return "OTHER"


def initial_target(member: tarfile.TarInfo) -> str | None:
    if member.issym():
        return normalized(posixpath.join(posixpath.dirname(normalized(member.name) or ""), member.linkname))
    if member.islnk():
        return normalized(member.linkname)
    return None


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source-rpm", required=True)
    parser.add_argument("--archive", required=True)
    parser.add_argument("--output", required=True)
    args = parser.parse_args()

    members: list[tarfile.TarInfo] = []
    with tarfile.open(fileobj=sys.stdin.buffer, mode="r|*") as archive:
        for member in archive:
            members.append(member)

    by_name = {normalized(member.name): member for member in members if normalized(member.name) is not None}

    def resolve(member: tarfile.TarInfo) -> tuple[str, str]:
        target = initial_target(member)
        if target is None:
            return "NOT_A_LINK", ""
        seen: set[str] = set()
        current = target
        while True:
            if current in seen:
                return "LINK_CYCLE", current
            seen.add(current)
            target_member = by_name.get(current)
            if target_member is None:
                return "TARGET_NOT_IN_ARCHIVE", current
            if safe_name(target_member.name) and (target_member.isfile() or target_member.isdir()):
                return "INTERNAL_TARGET_INCLUDED", current
            if target_member.issym() or target_member.islnk():
                next_target = initial_target(target_member)
                if next_target is None:
                    return "TARGET_UNRESOLVED", current
                current = next_target
                continue
            return "INTERNAL_TARGET_EXCLUDED", current

    rows = []
    for member in members:
        included = safe_name(member.name) and (member.isfile() or member.isdir())
        if included:
            continue
        member_kind = kind(member)
        if not safe_name(member.name):
            excluded_class = f"UNSAFE_PATH_{member_kind}"
        else:
            excluded_class = member_kind
        target_status, resolved_target = resolve(member)
        rows.append({
            "source_rpm": args.source_rpm,
            "archive": args.archive,
            "member": member.name,
            "member_type": member_kind,
            "excluded_class": excluded_class,
            "link_target": member.linkname if member.issym() or member.islnk() else "",
            "target_status": target_status,
            "resolved_target": resolved_target,
        })

    fields = ("source_rpm", "archive", "member", "member_type", "excluded_class", "link_target", "target_status", "resolved_target")
    with open(args.output, "w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(rows)
    print(f"source_rpm={args.source_rpm} archive={args.archive} total_members={len(members)} excluded={len(rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
