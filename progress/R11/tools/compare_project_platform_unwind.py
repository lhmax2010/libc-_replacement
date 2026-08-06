#!/usr/bin/env python3
import argparse
import collections
import csv


def read(path):
    with open(path, encoding="utf-8") as stream: return list(csv.DictReader(stream, delimiter="\t"))


def write(path, fields, rows):
    with open(path, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(rows)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--platform-abi-unwind", required=True)
    ap.add_argument("--project-unwind", required=True)
    ap.add_argument("--output", required=True)
    args = ap.parse_args()
    platform = read(args.platform_abi_unwind)
    project = [
        row for row in read(args.project_unwind)
        if row["elf_type"] in {"DYN", "EXEC"} and not row["path"].startswith("/usr/lib/debug/")
        and row["symbol"].startswith("_Unwind_")
    ]
    rows = []
    for arch in ("armv7l", "aarch64", "x86_64"):
        p_arch = [row for row in platform if row["arch"] == arch]
        p_elf_classes = collections.defaultdict(set)
        for row in p_arch:
            p_elf_classes[(row["repo_id"], row["name"], row["rpm_sha256"], row["path"])].add(row["version_class"])
        versioned_only = {key for key, value in p_elf_classes.items() if value == {"VERSIONED"}}
        unversioned_only = {key for key, value in p_elf_classes.items() if value == {"UNVERSIONED"}}
        repo_libcpp = [row for row in p_arch if row["repo_id"] == "base_binary" and row["name"] == "libc++"]
        repo_libcpp_classes = sorted({row["version_class"] for row in repo_libcpp})
        project_arch = [row for row in project if row["arch"] == arch]
        project_elf_classes = collections.defaultdict(set)
        for row in project_arch:
            project_elf_classes[(row["name"], row["rpm_sha256"], row["path"])].add(row["version_class"])
        project_versioned_only = {key for key, value in project_elf_classes.items() if value == {"VERSIONED"}}
        project_unversioned_only = {key for key, value in project_elf_classes.items() if value == {"UNVERSIONED"}}
        project_nodes = sorted({row["version_node"] for row in project_arch if row["version_node"]})
        project_classes = sorted({row["version_class"] for row in project_arch})
        platform_nodes = sorted({row["version_node"] for row in p_arch if row["version_node"]})
        rows.append({
            "arch": arch,
            "platform_abi_unwind_referencing_runtime_elf": len(p_elf_classes),
            "platform_versioned_only_elf": len(versioned_only),
            "platform_unversioned_only_elf": len(unversioned_only),
            "platform_version_nodes": ";".join(platform_nodes),
            "platform_repo_libcpp_22_1_8_19_1_elf": len({(r["rpm_sha256"], r["path"]) for r in repo_libcpp}),
            "platform_repo_libcpp_reference_classes": ";".join(repo_libcpp_classes),
            "project_c2_referencing_runtime_elf": len(project_elf_classes),
            "project_c2_versioned_only_elf": len(project_versioned_only),
            "project_c2_unversioned_only_elf": len(project_unversioned_only),
            "project_c2_version_nodes": ";".join(project_nodes),
            "matches_platform_dominant_reference_class": "MATCH" if project_elf_classes and len(project_versioned_only) == len(project_elf_classes) and len(versioned_only) > len(unversioned_only) else "MISMATCH_OR_UNDETERMINED",
            "matches_platform_repo_libcpp_22_1_8_19_1_reference_class": "MATCH" if repo_libcpp_classes and repo_libcpp_classes == project_classes else "MISMATCH",
        })
    write(args.output, [
        "arch", "platform_abi_unwind_referencing_runtime_elf", "platform_versioned_only_elf",
        "platform_unversioned_only_elf", "platform_version_nodes",
        "platform_repo_libcpp_22_1_8_19_1_elf", "platform_repo_libcpp_reference_classes",
        "project_c2_referencing_runtime_elf", "project_c2_versioned_only_elf",
        "project_c2_unversioned_only_elf", "project_c2_version_nodes",
        "matches_platform_dominant_reference_class",
        "matches_platform_repo_libcpp_22_1_8_19_1_reference_class",
    ], rows)
    return 0


if __name__ == "__main__": raise SystemExit(main())
