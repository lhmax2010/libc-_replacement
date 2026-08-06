#!/usr/bin/env python3
import argparse
import csv
import gzip
import xml.etree.ElementTree as ET

COMMON = "http://linux.duke.edu/metadata/common"
RPM = "http://linux.duke.edu/metadata/rpm"


def parse(repo_id, kind, path, writer):
    ns = {"c": COMMON, "r": RPM}
    with gzip.open(path, "rb") as stream:
        for _, elem in ET.iterparse(stream, events=("end",)):
            if elem.tag != f"{{{COMMON}}}package":
                continue
            name = elem.findtext("c:name", default="", namespaces=ns)
            arch = elem.findtext("c:arch", default="", namespaces=ns)
            ver = elem.find("c:version", ns)
            nevra = f"{name}-{ver.attrib.get('ver','')}-{ver.attrib.get('rel','')}.{arch}"
            fmt = elem.find("c:format", ns)
            if fmt is not None:
                for relation_type in ("provides", "requires", "conflicts", "obsoletes"):
                    parent = fmt.find(f"r:{relation_type}", ns)
                    if parent is None:
                        continue
                    for entry in parent.findall("r:entry", ns):
                        writer.writerow({
                            "repo_id": repo_id,
                            "kind": kind,
                            "package_name": name,
                            "package_arch": arch,
                            "package_nevra": nevra,
                            "relation_type": relation_type,
                            "relation_name": entry.attrib.get("name", ""),
                            "flags": entry.attrib.get("flags", ""),
                            "epoch": entry.attrib.get("epoch", ""),
                            "version": entry.attrib.get("ver", ""),
                            "release": entry.attrib.get("rel", ""),
                        })
            elem.clear()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--output", required=True)
    ap.add_argument("datasets", nargs="+", help="repo_id:kind:path")
    args = ap.parse_args()
    fields = ["repo_id", "kind", "package_name", "package_arch", "package_nevra",
              "relation_type", "relation_name", "flags", "epoch", "version", "release"]
    with open(args.output, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        for spec in args.datasets:
            repo_id, kind, path = spec.split(":", 2)
            parse(repo_id, kind, path, writer)


if __name__ == "__main__":
    main()
