#!/usr/bin/env bash
set -euo pipefail

root=${1:?root required}
profile=${2:?profile required}
arch=${3:?arch required}
out=${4:?output required}
gbs_conf=${5:?gbs config required}
build_conf=${6:?build config required}

mkdir -p "$out/RPMS" "$out/SRPMS" "$out/logs" "$out/config"
cp -a "$root/local/repos/$profile/$arch/RPMS/." "$out/RPMS/"
cp -a "$root/local/repos/$profile/$arch/SRPMS/." "$out/SRPMS/"
cp -a "$root/local/repos/$profile/$arch/logs/success/." "$out/logs/"
cp -a "$gbs_conf" "$build_conf" "$out/config/"
cp -a "$root/RETIRED_SINGLE_USE" "$out/"
find "$out" -type f -print0 | sort -z | xargs -0 sha256sum
