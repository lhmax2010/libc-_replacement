#!/usr/bin/env python3
import argparse
import csv


def rows(path):
    with open(path, encoding="utf-8") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--files", required=True)
    ap.add_argument("--output", required=True)
    args = ap.parse_args()
    data = rows(args.files)
    by_key = {}
    for row in data:
        by_key[(row["set_id"], row["role"], row["arch"], row["path"])] = row
    output = []
    pairs = [
        ("platform_repo_llvm", "llvm_devel", "PLATFORM_REPO_LLVM"),
        ("project_c2", "project_llvm_devel", "PROJECT_C2_LLVM"),
    ]
    for arch in ("armv7l", "aarch64", "x86_64"):
        nongnu = {
            path: row for (set_id, role, item_arch, path), row in by_key.items()
            if set_id == "platform_nongnu" and role == "nongnu_devel" and item_arch == arch
        }
        for set_id, role, label in pairs:
            llvm = {
                path: row for (item_set, item_role, item_arch, path), row in by_key.items()
                if item_set == set_id and item_role == role and item_arch == arch
            }
            for path in sorted(set(nongnu) & set(llvm)):
                left, right = nongnu[path], llvm[path]
                lt, rt = left["file_type"], right["file_type"]
                if lt == rt == "DIRECTORY":
                    result = "NO_HARD_CONFLICT_SHARED_DIRECTORY"
                elif lt == rt == "REGULAR_FILE":
                    result = "HARD_CONFLICT_REGULAR_FILE_TO_REGULAR_FILE"
                elif lt == rt == "SYMLINK" and left["link_target"] == right["link_target"]:
                    result = "SAME_SYMLINK_TARGET"
                elif lt == rt == "SYMLINK":
                    result = "HARD_CONFLICT_DIFFERENT_SYMLINK_TARGET"
                else:
                    result = "SAME_PATH_DIFFERENT_FILE_TYPE"
                output.append({
                    "arch": arch, "llvm_set": label, "path": path,
                    "nongnu_package": left["name"], "nongnu_file_type": lt,
                    "nongnu_sha256": left["file_sha256"], "nongnu_link_target": left["link_target"],
                    "llvm_package": right["name"], "llvm_file_type": rt,
                    "llvm_sha256": right["file_sha256"], "llvm_link_target": right["link_target"],
                    "static_path_classification": result,
                })
    fields = [
        "arch", "llvm_set", "path", "nongnu_package", "nongnu_file_type",
        "nongnu_sha256", "nongnu_link_target", "llvm_package", "llvm_file_type",
        "llvm_sha256", "llvm_link_target", "static_path_classification",
    ]
    with open(args.output, "w", newline="", encoding="utf-8") as out:
        writer = csv.DictWriter(out, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader(); writer.writerows(output)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
