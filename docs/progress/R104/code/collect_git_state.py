#!/usr/bin/env python3
import csv
import hashlib
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CODE_ROOT = ROOT / "codes" / "R104"
OUT = ROOT / "progress" / "R104" / "tables"
OUT.mkdir(parents=True, exist_ok=True)

targets = [
    ("abseil-cpp", "platform/upstream/abseil-cpp", "9c39c516dca54c786ddc5da45a5a15acf41ef75e", "packaging/abseil-cpp.spec"),
    ("bcc-tools", "platform/upstream/bcc", "dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2", "packaging/bcc-tools.spec"),
    ("boost", "platform/upstream/boost", "69203710b8582c489f420ef53a769cc6077c250b", "packaging/boost.spec"),
    ("bpftrace", "platform/upstream/bpftrace", "30e51cd665360f85b665308dc13ba27df0c5f739", "packaging/bpftrace.spec"),
    ("icu", "platform/upstream/icu", "e4a4d7411b0f288581cf020a176b5dddbb504ae7", "packaging/icu.spec"),
    ("jsoncpp", "platform/upstream/jsoncpp", "96bf0c36eed62a6d7bfc9bdb30f0e41d877aa6f0", "packaging/jsoncpp.spec"),
    ("libsigc++", "platform/upstream/libsigc++", "5f177f7c98ea29833989b5d0d6a79fece9701545", "packaging/libsigc++.spec"),
    ("llvm", "platform/upstream/llvm", "2d23367d74afbf2bb1e9e4013fce072b3a154109", "packaging/llvm.spec"),
    ("pcre", "platform/upstream/pcre", "feba6a19627f5d6e74d551ef2fc8b33af0547780", "packaging/pcre.spec"),
    ("taglib", "platform/upstream/taglib", "4e7de646d897ed5a92111799058b21705a3bd34c", "packaging/taglib.spec"),
    # R101 did not include tensorflow2. R103's exact Base source revision is
    # used as the previous recorded baseline for this newly added package.
    ("tensorflow2", "platform/upstream/tensorflow2", "1cdba73549f741720f11dd53da1bb516b03763a2", "packaging/tensorflow2.spec"),
]


def git(path, *args):
    return subprocess.check_output(["git", "-C", str(path), *args], text=True).rstrip("\n")


def sha(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


fields = [
    "package", "local_path", "remote_url", "branch", "head",
    "previous_recorded_revision", "revision_drift", "commits_ahead_of_previous",
    "spec_path", "spec_sha256_current", "spec_sha256_r101_or_r103",
    "spec_changed_from_record", "status_clean", "last_3_commits",
]
rows = []
for name, repo, old, spec_rel in targets:
    path = CODE_ROOT / name
    head = git(path, "rev-parse", "HEAD")
    branch = git(path, "branch", "--show-current")
    status = git(path, "status", "--porcelain")
    spec = path / spec_rel
    archived = ROOT / "docs" / "progress" / "R101" / "inputs" / "current_specs" / Path(spec_rel).name
    if name == "tensorflow2":
        archived = ROOT / "progress" / "R103" / "inputs" / "source_git" / "tensorflow2-base" / spec_rel
    archived_sha = sha(archived) if archived.exists() else "NOT_AVAILABLE"
    current_sha = sha(spec)
    if head == old:
        ahead = "0"
    else:
        probe = subprocess.run(
            ["git", "-C", str(path), "cat-file", "-e", f"{old}^{{commit}}"],
            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
        )
        ahead = git(path, "rev-list", "--count", f"{old}..{head}") if probe.returncode == 0 else "NOT_AVAILABLE_SHALLOW"
    log = git(path, "log", "-3", "--format=%H %s").replace("\n", " | ")
    rows.append({
        "package": name,
        "local_path": str(path.relative_to(ROOT)),
        "remote_url": git(path, "remote", "get-url", "origin"),
        "branch": branch,
        "head": head,
        "previous_recorded_revision": old,
        "revision_drift": "NO" if head == old else "YES",
        "commits_ahead_of_previous": ahead,
        "spec_path": spec_rel,
        "spec_sha256_current": current_sha,
        "spec_sha256_r101_or_r103": archived_sha,
        "spec_changed_from_record": "NO" if current_sha == archived_sha else "YES" if archived_sha != "NOT_AVAILABLE" else "NOT_AVAILABLE",
        "status_clean": "YES" if not status else "NO",
        "last_3_commits": log,
    })

with (OUT / "git_state_before_branch.tsv").open("w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)

for row in rows:
    print("\t".join(row[field] for field in fields))
