# External reference index

The source workspace `/home/toolchain/plan_evaluation/` is a read-only
reference. Large artifacts and raw evidence are intentionally not stored in
this repository. A file SHA-256 identifies a file directly; a tree-manifest
SHA-256 identifies the checked-in canonical per-file hash list named in the
`digest basis` column.

| Kind | Absolute path | Bytes / files | SHA-256 | Digest basis | Purpose |
|---|---|---:|---|---|---|
| RPM repository | `/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/armv7l/artifacts/` | 25,602,632 / 16 | `5e26d3a4377b1d67a11c138cc1085f27de7c6f3114afefd8658ab80cc10a21f3` | `ref_manifests/libcxx_armv7l_artifacts.sha256` | armv7l libc++/libc++abi candidate RPMs and repodata |
| RPM repository | `/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/aarch64/artifacts/` | 25,688,321 / 16 | `6e28b15745fbadf5277f6dbacb4e7cf283aeec60a322f388f52c71230c55120a` | `ref_manifests/libcxx_aarch64_artifacts.sha256` | aarch64 candidate RPMs and repodata |
| RPM repository | `/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/x86_64/artifacts/` | 25,709,436 / 16 | `b0863cfb3947d5c5a958d43a3a8af7c1d1160e383d0194ec175a49343fa1cdd4` | `ref_manifests/libcxx_x86_64_artifacts.sha256` | x86_64 candidate RPMs and repodata |
| SQLite | `/home/toolchain/plan_evaluation/abi_census/elf_analysis.sqlite3` | 1,711,427,584 | `9c5ff11c75c5476f1e70e652c9264d25eafc9e48103ccb1987d53637dad3f4c9` | file | frozen-arm analysis database |
| SQLite | `/home/toolchain/plan_evaluation/census_multiarch_v2/armv7l/elf_analysis.sqlite3` | 1,712,386,048 | `dd3ab56384fcf543fa8aee0a160afb5ad37f7f857f19e88d8e636a98783220ff` | file | aligned 0723 armv7l analysis database |
| SQLite | `/home/toolchain/plan_evaluation/census_multiarch_v2/aarch64/elf_analysis.sqlite3` | 1,705,730,048 | `f8aa5d6c7f5c7d440053dbe1202e226edafd14f11a8fb20409d518477189b1a9` | file | aligned 0723 aarch64 analysis database |
| SQLite | `/home/toolchain/plan_evaluation/census_multiarch_v2/x86_64/elf_analysis.sqlite3` | 1,563,951,104 | `73d4320f8aa1287dbfe3b73607feac5511cc857c5ea9402f635092c6e497de9a` | file | aligned 0723 x86_64 analysis database |
| Raw evidence | `/home/toolchain/plan_evaluation/freeze_v1.1/evidence/` | 2,396,919,742 / 4,700 | `5cc82ae14ceb064930c9bb651d77ac9b36b805cc556c08b379610e45c1e22eb1` | SHA-256 of `/home/toolchain/plan_evaluation/freeze_v1.1/MANIFEST.sha256` | frozen census, ABI PoC, board and independent spot-check evidence |
| Raw packaging evidence | `/home/toolchain/plan_evaluation/p1/libcxx_packaging/multiarch/` | see source manifest | `31f281a8518daf5fc16c7f1511531db29637299cb260d32109fd2af295b2c72e` | SHA-256 of source `MANIFEST.sha256` | build logs, readelf output, fixtures and runtime evidence |
| Raw D5 evidence | `/home/toolchain/plan_evaluation/p1/d5_allowlist/evidence/` | 857,627 / 151 | `db9adf1fec36be08f1b395428aac1f120df72843668bb7098a00c0457afb89fe` | `ref_manifests/d5_evidence.sha256` | allowlist positive, negative and mixed-state evidence |
| Raw gate output | `/home/toolchain/plan_evaluation/p1/gate_tools_validation/raw/` | 2,234 / 39 | `c78634630f67305996949acc9361cf9fd1807a1f1ce2447e51e9f1258ff801e0` | `ref_manifests/gate_raw_outputs.sha256` | red/green fixture execution output |
| Raw gate v1.2 output | `/home/toolchain/governance/p1_exit_review/remediation_r4/evidence/gate_v1_2/raw/` | 25,862 / 132 | `21748cfe1f31463feb9954147b50ba1ca4fe87d04b29340427ec1fb77673302f` | `ref_manifests/gate_v1_2_raw_outputs.sha256` | four-tool full 33-fixture execution output and exact commands |
| Raw gate v1.3 output | `/home/toolchain/governance/p1_exit_review/remediation_r5/evidence/gate_v1_3/raw/` | 34,781 / 156 | `0c4e1a528930b42cf55ead431b337ee2f44a2b05b2447b00b7f5dccf135e5f5e` | `ref_manifests/gate_v1_3_raw_outputs.sha256` | four-tool full 39-fixture execution output and exact commands |
| Frozen noarch RPM | `/home/toolchain/governance/p1_exit_review/remediation_r4/evidence/noarch/boost-license-1.83.0-5.1.noarch.rpm` | 8,554 | `39484d87349fe00b7dbc9382518cb258b694d3126d0c64358da3e3abf3871cf0` | file | independent Base noarch source-output identity and license-only content proof |
| Frozen noarch RPM | `/home/toolchain/governance/p1_exit_review/remediation_r4/evidence/noarch/boost-doc-html-1.83.0-5.1.noarch.rpm` | 61,108,070 | `434a09ae5959c89d3fcf9014c02748786e10268148db0be72e58ddfc7b61fe6a` | file | independent Base noarch source-output identity for HOLD sibling |
| Review tar | `/home/toolchain/plan_evaluation/p1/wave1_cynara/cynara_api_review_v2.tar.gz` | 418,669 | `2e437083bfb9490476c6362c409755e9efa3978f51db11460e82fd2c30e43646` | file | 27-API developer review corpus |
| Review tar | `/home/toolchain/plan_evaluation/p1/wave1_cynara/blocker_adjudication/round2/blocker_adjudication_review_v1.tar.gz` | 51,616 | `5350b6ff25fa276170bb3cd86d07a4bd4b6511c25f492541e5db048c20a16a66` | file | blocker review v1 |
| Review tar | `/home/toolchain/plan_evaluation/p1/wave1_cynara/blocker_adjudication/round2/blocker_adjudication_review_v2.tar.gz` | 73,891 | `7267368b3a1ac90e7f49c14278b98e0130b46944b65af42d4c2d99bca50b266c` | file | blocker review v2 |
| Review tar | `/home/toolchain/plan_evaluation/p1/wave1_cynara/blocker_adjudication/round3/blocker_adjudication_review_v3.tar.gz` | 147,850 | `c4a9e30ea220340131fa2053c3cc97241e1d1e96921de5046144cc7a43033966` | file | 14-item closure corpus and v3 sign-off baseline |
| Buildroot identities | `/home/toolchain/GBS-ROOT-LIBCXX-MULTIARCH-{armv7l,aarch64,x86_64}` and `/home/toolchain/tmp/GBS-ROOT-D5-BASELINE` | ephemeral | `1cefc7fec451618aae8c06ad7af6e576b698e37da622d9f08fc3b3e4ff70d46e` | `ref_manifests/buildroot_identity.tsv` | build-input identity for non-content-addressed caches |
| Census v2 implementation | `/home/toolchain/plan_evaluation/census_multiarch_v2/pipeline/compare_v2.py` | 26,996 | `b44c9d63b1a7efb31f01ca45ff4c991977e1b3b7f3e4975ab12f2de2d50f96c0` | file | aligned-v2 cross-architecture comparison implementation (EG-01) |
| Census v2 implementation | `/home/toolchain/plan_evaluation/census_multiarch_v2/pipeline/corrected_graph.py` | 30,920 | `e54d03158db875f1db5a9cb5265bf776fabb7490b67614fadc90379638a1fc56` | file | D2/A8/A9 corrected graph replay implementation (EG-01) |
| Census v2 implementation | `/home/toolchain/plan_evaluation/census_multiarch_v2/pipeline/finalize_reports.py` | 32,707 | `36d0fcefefa10091d6b5477729b4b0450b10314f110fb042c8439fba23ed0853` | file | aligned-v2 report finalizer (EG-01) |
| Census v2 implementation | `/home/toolchain/plan_evaluation/census_multiarch_v2/pipeline/run_finalize_raw.py` | 1,683 | `6c5d16cf3ce2108b7022abe73920ff736d866356be87a4f1f89d07443a3d9c0e` | file | raw-finalization entry point (EG-01) |
| Census v2 implementation | `/home/toolchain/plan_evaluation/census_multiarch_v2/pipeline/run_reused_phase.py` | 5,754 | `3a873ad331b46f502e4ef76c376275b6efdd4a2fa844d7572627e0a023ccc9ee` | file | reused collection phase entry point (EG-01) |
| Base-first audit | `/home/toolchain/plan_evaluation/p1/base_unified_split/base_first_8_verification.tsv` | 3,563 | `14090bab10d1c05de5e0f2bf625b70c23b533ba27a49cb124bfffd68c570932a` | file | original eight-provider existence and stability evidence (OQ-1/EG-03) |
| Base-first audit | `/home/toolchain/plan_evaluation/p1/base_unified_split/base_first_component_diffs.tsv` | 835 | `f620cdac8de499b01cadb447d07b52dab4697a4100705174950f970fff669ce3` | file | original Base-first component difference detail (OQ-1/EG-03) |
| Wave assignment audit | `/home/toolchain/plan_evaluation/p1/base_unified_split/wave_assignment_sets.tsv` | 8,972 | `6fbb8c9d33abb40c8eb9a71c8fe0e807ec3a9d7e70b45f231dc40b88a03e8d2f` | file | mechanical wave-assignment source sets (OQ-1/EG-03) |

The buildroot digest is deliberately an input-identity digest, not a claim
that every cache byte is immutable. The buildroots contain root-owned,
unreadable paths and are reproducible caches rather than retained evidence.
Their commands and source-input hashes are recorded in
`ref_manifests/buildroot_identity.tsv`.
