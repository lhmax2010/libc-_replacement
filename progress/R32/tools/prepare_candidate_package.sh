#!/usr/bin/env bash
set -euo pipefail

base=${1:?workspace root}
pkg="$base/tmp/R32/candidate-package"
mkdir -p "$pkg/packaging"
cp "$base/progress/R32/src/forced_unwind_candidates.cpp" "$pkg/forced_unwind_candidates.cpp"
cp "$base/progress/R32/inputs/r32-forced-unwind-candidates.spec" \
   "$pkg/packaging/r32-forced-unwind-candidates.spec"
git -C "$pkg" init
git -C "$pkg" config user.name "Codex R32 evidence"
git -C "$pkg" config user.email "codex-r32@localhost"
git -C "$pkg" add forced_unwind_candidates.cpp packaging/r32-forced-unwind-candidates.spec
git -C "$pkg" commit -m "Add R32 forced unwind candidate fixture"
git -C "$pkg" status --porcelain
git -C "$pkg" rev-parse HEAD
sha256sum "$pkg/forced_unwind_candidates.cpp" \
          "$pkg/packaging/r32-forced-unwind-candidates.spec"
