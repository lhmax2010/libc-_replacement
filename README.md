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

Historical count note: the initial repository had 169 tracked files while
its manifest had 168 entries because the manifest excludes itself. The P1
review corpus contained those 169 files plus `REVIEW_LINEAGE.md`, so its
manifest had 170 entries; the tar contained 171 files after adding that
review manifest. These are different, explicit counting scopes rather than
missing payloads.

## Repository disciplines

1. This repository is the single developer-facing fact source after P1 exit
   approval; superseded material is explicitly labeled.
2. `/home/toolchain/plan_evaluation/` is read-only reference material and
   must not be modified.
3. Large artifacts are resolved only through `REF_INDEX.md` and their
   recorded hashes.

**P1_EXIT_REVIEW_PASSED — 2026-07-30；GPT `reviewers/p1_exit/r6/gpt_closure_verdict_r6.md` SHA256 `163f54f7076bf6cd9d2e84c8b70773743303fd118bc7738552cf7860101f4210`；Claude `reviewers/p1_exit/r6/claude_confirmation_neutral-registry.md` SHA256 `109aa9de9ccfe9b45f2ae7fda2b6be0f6f1013988444c6db732ae26ac4822524`；Kimi `reviewers/p1_exit/r6/kimi_confirmation_r6.md` SHA256 `769abb562e491fca2e2f06cde5905bc15a871af3242e2f4d76673714cdc282b6`。**
