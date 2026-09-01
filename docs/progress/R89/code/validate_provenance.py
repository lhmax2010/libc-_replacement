#!/usr/bin/env python3
import pathlib
import re
import subprocess
import sys


if len(sys.argv) != 4:
    raise SystemExit("usage: validate_provenance.py FILE REPO REV")

path = pathlib.Path(sys.argv[1])
repo, rev = sys.argv[2:]
text = path.read_text()
lines = text.splitlines()
values = {}
for line in lines:
    if not line or line.startswith("#"):
        continue
    if "=" not in line:
        raise SystemExit(f"invalid non-comment line: {line!r}")
    key, value = line.split("=", 1)
    if key in values:
        raise SystemExit(f"duplicate key: {key}")
    values[key] = value

required = {
    "provenance_format": "2",
    "package_revision_source": "GBS_GENERATED_SPEC_VCS_FIELD",
    "downstream_patch_topology": "INTENTIONALLY_NOT_RECORDED",
    "downstream_patch_topology_reason": "UNSTABLE_UNDER_SQUASH_REBASE_AND_CHERRY_PICK",
    "component": "llvm-project libcxx+libcxxabi",
    "version": "22.1.8",
    "gbs_export_tag": "upstream/22.1.8",
    "upstream_release_tag": "llvmorg-22.1.8",
    "validated_component_tree_hash_algorithm": "git-sha1",
}
for key, expected in required.items():
    assert values.get(key) == expected, (key, values.get(key), expected)

for key in values:
    assert not key.startswith("export_patch_"), key
    assert key not in {
        "export_patch_count",
        "ehabi_backport_export_patch",
        "ehabi_backport_spec_patch_entry",
        "local_tizen_base_vcs",
        "local_tizen_base_branch",
    }, key

head = subprocess.check_output(["git", "-C", repo, "rev-parse", rev], text=True).strip()
assert head not in text, "self-referential current revision appears in provenance"

for component in ("libcxx", "libcxxabi"):
    actual = subprocess.check_output(
        ["git", "-C", repo, "rev-parse", f"{rev}:{component}"], text=True
    ).strip()
    assert values[f"validated_{component}_tree"] == actual
    print(f"PASS\tvalidated_{component}_tree\t{actual}")

assert any("exact package revision" in line for line in lines if line.startswith("#"))
assert any("patch topology" in line for line in lines if line.startswith("#"))
assert re.fullmatch(r"[0-9a-f]{64}", values["source_tarball_sha256"])
assert re.fullmatch(r"[0-9a-f]{64}", values["official_release_archive_sha256"])
print(f"PASS\tunique_keys\t{len(values)}")
print("PASS\tself_reference\tabsent")
print("PASS\tpatch_topology_fields\tabsent")
print("SUMMARY\tPASS")
