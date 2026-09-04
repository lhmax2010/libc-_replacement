#!/usr/bin/env python3
import argparse
import csv
import shutil
import tarfile
import zipfile
from pathlib import Path

from w2_scan_sources import extract_srpm, likely_archive, normalized_link, safe_name, wanted


def read(path):
    with path.open(newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def relevance(*names):
    paths = [Path(name) for name in names if name]
    if any(wanted(path) for path in paths):
        return "CXX_SOURCE_PATH"
    if any(not path.suffix for path in paths):
        return "UNKNOWN_PATH_KIND"
    return "NON_CXX_PATH"


def resolve(name, links, content):
    seen = set()
    current = name
    while current in links:
        if current in seen:
            return current, "LINK_CYCLE"
        seen.add(current)
        member_name, link_name, hardlink = links[current]
        target = normalized_link(member_name, link_name, hardlink)
        if target is None:
            return link_name, "UNSAFE_TARGET"
        current = target
    if current in content:
        return current, "CLOSED_TO_ARCHIVE_CONTENT"
    return current, "TARGET_NOT_IN_ARCHIVE"


def tar_links(path, source_rpm, relative_archive):
    rows = []
    with tarfile.open(path, "r:*") as archive:
        members = archive.getmembers()
        content = {m.name.rstrip("/") for m in members if safe_name(m.name) and (m.isfile() or m.isdir())}
        links = {m.name.rstrip("/"): (m.name, m.linkname, m.islnk()) for m in members
                 if safe_name(m.name) and (m.issym() or m.islnk())}
        for name, (member_name, link_name, _) in links.items():
            final_target, result = resolve(name, links, content)
            rows.append({
                "source_rpm": source_rpm, "archive": relative_archive,
                "member": member_name, "declared_target": link_name,
                "final_target": final_target, "resolution": result,
                "relevance": "CLOSED" if result == "CLOSED_TO_ARCHIVE_CONTENT" else relevance(member_name, final_target),
            })
    return rows


def zip_links(path, source_rpm, relative_archive):
    rows = []
    with zipfile.ZipFile(path) as archive:
        members = archive.infolist()
        content = {m.filename.rstrip("/") for m in members
                   if safe_name(m.filename) and ((m.external_attr >> 16) & 0o170000) != 0o120000}
        links = {}
        for member in members:
            mode = (member.external_attr >> 16) & 0o170000
            if safe_name(member.filename) and mode == 0o120000:
                target = archive.read(member).decode("utf-8", "replace")
                links[member.filename.rstrip("/")] = (member.filename, target, False)
        for name, (member_name, link_name, _) in links.items():
            final_target, result = resolve(name, links, content)
            rows.append({
                "source_rpm": source_rpm, "archive": relative_archive,
                "member": member_name, "declared_target": link_name,
                "final_target": final_target, "resolution": result,
                "relevance": "CLOSED" if result == "CLOSED_TO_ARCHIVE_CONTENT" else relevance(member_name, final_target),
            })
    return rows


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--status", required=True, type=Path)
    parser.add_argument("--work", required=True, type=Path)
    parser.add_argument("--ledger", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--summary", required=True, type=Path)
    args = parser.parse_args()
    manifest = {r["source_rpm"]: r for r in read(args.manifest)}
    partial = [r for r in read(args.status) if r["status"] == "PARTIAL"]
    output = []
    args.ledger.parent.mkdir(parents=True, exist_ok=True)
    for index, item in enumerate(partial, 1):
        shutil.rmtree(args.work, ignore_errors=True)
        payload = args.work / "payload"
        payload.mkdir(parents=True)
        ok, detail = extract_srpm(Path(manifest[item["source_rpm"]]["local_path"]), payload, args.ledger)
        if not ok:
            raise RuntimeError(f"re-extraction failed: {item['source_rpm']} {detail}")
        for archive in sorted(p for p in payload.rglob("*") if p.is_file() and likely_archive(p)):
            relative = str(archive.relative_to(payload))
            if tarfile.is_tarfile(archive):
                output.extend(tar_links(archive, item["source_rpm"], relative))
            elif zipfile.is_zipfile(archive):
                output.extend(zip_links(archive, item["source_rpm"], relative))
        print(f"PROGRESS={index}/{len(partial)}", flush=True)
    shutil.rmtree(args.work, ignore_errors=True)
    fields = ("source_rpm", "archive", "member", "declared_target", "final_target", "resolution", "relevance")
    write(args.output, fields, output)
    summary = []
    for item in partial:
        selected = [r for r in output if r["source_rpm"] == item["source_rpm"]]
        unresolved = [r for r in selected if r["resolution"] != "CLOSED_TO_ARCHIVE_CONTENT"]
        relevant = [r for r in unresolved if r["relevance"] in {"CXX_SOURCE_PATH", "UNKNOWN_PATH_KIND"}]
        summary.append({
            "source_rpm": item["source_rpm"], "link_members": len(selected),
            "closed_by_link_chain": len(selected) - len(unresolved),
            "targets_not_in_archive": len(unresolved),
            "cxx_source_paths": sum(r["relevance"] == "CXX_SOURCE_PATH" for r in unresolved),
            "unknown_path_kind": sum(r["relevance"] == "UNKNOWN_PATH_KIND" for r in unresolved),
            "non_cxx_paths": sum(r["relevance"] == "NON_CXX_PATH" for r in unresolved),
            "wbuffer_coverage": "PARTIAL" if relevant else "CLOSED_FOR_WBUFFER_SCAN",
        })
    write(args.summary, tuple(summary[0]), summary)
    print(f"packages={len(summary)}")
    print(f"closed_packages={sum(r['wbuffer_coverage'] == 'CLOSED_FOR_WBUFFER_SCAN' for r in summary)}")
    print(f"partial_packages={sum(r['wbuffer_coverage'] == 'PARTIAL' for r in summary)}")
    print(f"unresolved_links={sum(r['targets_not_in_archive'] for r in summary)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
