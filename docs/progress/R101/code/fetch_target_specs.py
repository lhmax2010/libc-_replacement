#!/usr/bin/env python3
import base64
import json
import sys
import urllib.parse
import urllib.request
from pathlib import Path


TARGETS = {
    "abseil-cpp": ("platform/upstream/abseil-cpp", "9c39c516dca54c786ddc5da45a5a15acf41ef75e"),
    "bcc-tools": ("platform/upstream/bcc", "97128ee3dbccd2851cfa94dc723edb2abcbefc1d"),
    "boost": ("platform/upstream/boost", "58b70f335aec5641540dbe04dfd56fad5af43658"),
    "bpftrace": ("platform/upstream/bpftrace", "30e51cd665360f85b665308dc13ba27df0c5f739"),
    "icu": ("platform/upstream/icu", "e4a4d7411b0f288581cf020a176b5dddbb504ae7"),
    "jsoncpp": ("platform/upstream/jsoncpp", "21c9dcd2bc8f51f8bfe5173b49780dc707eef9f9"),
    "libsigc++": ("platform/upstream/libsigc++", "5f177f7c98ea29833989b5d0d6a79fece9701545"),
    "llvm": ("platform/upstream/llvm", "08a64b6eb95ed74339f1d862a6c22a2f93fa78f9"),
    "pcre": ("platform/upstream/pcre", "feba6a19627f5d6e74d551ef2fc8b33af0547780"),
    "taglib": ("platform/upstream/taglib", "4e7de646d897ed5a92111799058b21705a3bd34c"),
}

BASE = "https://review.tizen.org/gerrit/plugins/gitiles"


def get(url):
    request = urllib.request.Request(url, headers={"User-Agent": "R101-read-only-audit"})
    with urllib.request.urlopen(request, timeout=30) as response:
        return response.read()


if len(sys.argv) != 2:
    raise SystemExit("usage: fetch_target_specs.py OUTPUT_DIR")

output = Path(sys.argv[1])
output.mkdir(parents=True, exist_ok=True)
for name, (repo, revision) in TARGETS.items():
    repo_url = urllib.parse.quote(repo, safe="/")
    listing_url = f"{BASE}/{repo_url}/+/{revision}/packaging/?format=JSON"
    raw = get(listing_url)
    (output / f"{name}.packaging-tree.raw").write_bytes(raw)
    payload = raw.split(b"\n", 1)[1] if raw.startswith(b")]}\'") else raw
    entries = json.loads(payload)["entries"]
    specs = sorted(entry["name"] for entry in entries if entry["name"].endswith(".spec"))
    if not specs:
        raise RuntimeError(f"{name}: no spec in packaging tree")
    for spec in specs:
        spec_url = f"{BASE}/{repo_url}/+/{revision}/packaging/{urllib.parse.quote(spec)}?format=TEXT"
        decoded = base64.b64decode(get(spec_url))
        (output / f"{name}__{spec}").write_bytes(decoded)
    print(f"{name}\t{repo}\t{revision}\t{';'.join(specs)}")
