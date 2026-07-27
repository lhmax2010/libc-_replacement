# libc++ replacement: Wave 1 reference repository

This repository is the developer-facing reference set for the Tizen
libstdc++ → libc++ migration Wave 1. It contains frozen conclusions,
execution specifications, adjudication state, gate implementations and the
current 0723 multi-architecture ledger. Raw evidence, databases, build
caches and binary artifacts stay outside Git.

## Directory guide

- `docs/hld/`: frozen HLD, non-mutating errata and governance decisions.
- `docs/schedule/`: corrected migration schedule.
- `wave1/`: cynara execution plan, startup conditions, API adjudication and
  frozen build command.
- `d5/`: per-package libc++ allowlist mechanism and contrast verdicts.
- `gates/`: frozen gate tools and their small red/green fixtures.
- `ledger/`: shared main ledger, per-architecture overlays and mechanical
  preassignment.
- `census/`: frozen armv7l conclusions plus the aligned 0723
  multi-architecture conclusion layer. The v1 reconciliation is retained in
  a directory explicitly marked `SUPERSEDED`.
- `packaging/`: multi-architecture libc++/libc++abi report and combined RPM
  manifest.
- `base_unified/`: Base/Unified split data and Wave 4 owner notices.

## External evidence

Use `REF_INDEX.md` to locate omitted RPM repositories, SQLite databases,
raw logs, review archives and buildroot identities. Per-directory canonical
hash lists are under `ref_manifests/`. `SOURCE_MAP.tsv` proves byte identity
for every file copied directly from the read-only source workspace.
`MANIFEST.sha256` covers every tracked payload file except itself, which
avoids an impossible self-referential digest.

## Repository disciplines

1. This repository is the single developer-facing fact source after P1 exit
   approval; superseded material is explicitly labeled.
2. `/home/toolchain/plan_evaluation/` is read-only reference material and
   must not be modified.
3. Large artifacts are resolved only through `REF_INDEX.md` and their
   recorded hashes.

**PENDING_P1_EXIT_REVIEW —— 三方评审 PASS 前不得作为开发依据**
