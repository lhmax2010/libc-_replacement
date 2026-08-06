#!/usr/bin/env python3
import argparse
import csv
import gzip
import xml.etree.ElementTree as ET

COMMON = "http://linux.duke.edu/metadata/common"
RPM = "http://linux.duke.edu/metadata/rpm"


def text(node, path, ns):
    child = node.find(path, ns)
    return "" if child is None or child.text is None else child.text


def parse(repo_id, kind, path, writer):
    ns = {"c": COMMON, "r": RPM}
    with gzip.open(path, "rb") as stream:
        for _, elem in ET.iterparse(stream, events=("end",)):
            if elem.tag != f"{{{COMMON}}}package":
                continue
            version = elem.find("c:version", ns)
            location = elem.find("c:location", ns)
            checksum = elem.find("c:checksum", ns)
            size = elem.find("c:size", ns)
            time = elem.find("c:time", ns)
            fmt = elem.find("c:format", ns)
            row = {
                "repo_id": repo_id,
                "kind": kind,
                "name": text(elem, "c:name", ns),
                "arch": text(elem, "c:arch", ns),
                "epoch": "" if version is None else version.attrib.get("epoch", ""),
                "version": "" if version is None else version.attrib.get("ver", ""),
                "release": "" if version is None else version.attrib.get("rel", ""),
                "sourcerpm": "" if fmt is None else text(fmt, "r:sourcerpm", ns),
                "location": "" if location is None else location.attrib.get("href", ""),
                "checksum_type": "" if checksum is None else checksum.attrib.get("type", ""),
                "checksum": "" if checksum is None or checksum.text is None else checksum.text,
                "package_size": "" if size is None else size.attrib.get("package", ""),
                "build_time": "" if time is None else time.attrib.get("build", ""),
            }
            writer.writerow(row)
            elem.clear()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--output", required=True)
    ap.add_argument("datasets", nargs="+", help="repo_id:kind:path")
    args = ap.parse_args()
    fields = [
        "repo_id", "kind", "name", "arch", "epoch", "version", "release",
        "sourcerpm", "location", "checksum_type", "checksum", "package_size",
        "build_time",
    ]
    with open(args.output, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        for spec in args.datasets:
            repo_id, kind, path = spec.split(":", 2)
            parse(repo_id, kind, path, writer)


if __name__ == "__main__":
    main()
