#!/usr/bin/env python3
"""Fetch and checksum the minimal RPM metadata and image manifests for R98."""

import argparse
import hashlib
import re
import urllib.parse
import urllib.request
import xml.etree.ElementTree as ET
from pathlib import Path

REPO = "{http://linux.duke.edu/metadata/repo}"


def fetch(url, path):
    request = urllib.request.Request(url, headers={"User-Agent": "R98-read-only-audit/1"})
    with urllib.request.urlopen(request, timeout=180) as response:
        data = response.read()
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    print(f"FETCH\t{url}\t{path}\t{len(data)}\t{hashlib.sha256(data).hexdigest()}")
    return data


def repo_metadata(root, label, out):
    for tree in ("packages", "source"):
        repo_root = f"{root}/repos/standard/{tree}"
        repomd_path = out / f"{label}_{tree}_repomd.xml"
        data = fetch(f"{repo_root}/repodata/repomd.xml", repomd_path)
        xml = ET.fromstring(data)
        wanted = {"primary", "filelists"} if tree == "packages" else {"primary"}
        found = set()
        for node in xml.findall(REPO + "data"):
            kind = node.get("type")
            if kind not in wanted:
                continue
            loc = node.find(REPO + "location")
            checksum = node.find(REPO + "checksum")
            href = loc.get("href")
            destination = out / f"{label}_{tree}_{kind}.xml.gz"
            payload = fetch(f"{repo_root}/{href}", destination)
            digest = hashlib.new(checksum.get("type"), payload).hexdigest()
            if digest != checksum.text:
                raise SystemExit(f"checksum mismatch: {destination}: {digest} != {checksum.text}")
            print(f"REPOMD_CHECK\t{destination}\t{checksum.get('type')}\tPASS")
            found.add(kind)
        if found != wanted:
            raise SystemExit(f"missing metadata for {label}/{tree}: wanted={wanted}, found={found}")


def image_manifests(root, out):
    base = f"{root}/images/standard/"
    html = fetch(base, out / "unified_images_standard_index.html").decode("utf-8", "replace")
    names = sorted(set(re.findall(r'href="([^"/]+)/"', html)) - {".."})
    if not names:
        raise SystemExit("no standard images discovered")
    for name in names:
        directory = f"{base}{urllib.parse.quote(name)}/"
        image_html = fetch(directory, out / "image_indices" / f"{name}.html").decode("utf-8", "replace")
        package_links = sorted(set(re.findall(r'href="([^"]+\.packages)"', image_html)))
        if len(package_links) != 1:
            raise SystemExit(f"expected one package manifest for {name}, found {package_links}")
        fetch(urllib.parse.urljoin(directory, package_links[0]),
              out / "image_packages" / f"{name}.packages")
    print(f"IMAGE_MANIFEST_COUNT\t{len(names)}")
    for name in names:
        print(f"IMAGE\t{name}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base", required=True)
    parser.add_argument("--unified", required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    repo_metadata(args.base.rstrip("/"), "llvm_base", args.output)
    repo_metadata(args.unified.rstrip("/"), "llvm_unified", args.output)
    image_manifests(args.unified.rstrip("/"), args.output)


if __name__ == "__main__":
    main()
