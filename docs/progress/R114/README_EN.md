# Headquarters delivery package — corrected review copy

This package incorporates the mandatory corrections identified by the independent delivery review and the adopted wording clarifications. It does not change any technical decision, package classification, or measured result.

## Suggested reading order

1. `weekly_report_20260904.md` — weekly status, completed validation, open decisions, and the corrected forced-unwind site counts.
2. `base_adaptation_report.md` — Base-layer libc++ adaptation scope, the 11-package result, implementation status, and stated evidence boundaries.
3. `base_method_explained.md` — reproducible methodology, data identity, decision rules, and limitations.
4. `base_packages_final.md` — human-readable English decision table for all 254 source packages.
5. `base_packages_final.tsv` — the same 254-row English table in machine-readable form.
6. `attach_50_row_audit.tsv` — audit of the 50 rows returned by the historical heuristic.
7. `attach_172_row_sites.tsv` — strict list of 172 propagation occurrences; applying the documented logical-site key yields 104 logical sites.

## Attachment semantics

In `attach_50_row_audit.tsv`, `containing_handler_propagation` describes whether the handler containing the matched row propagates the exception. For the four rows marked `NON_CLAUSE_TOKEN_LINE`, `RETHROW_CONFIRMED` does **not** mean that the log line itself is a catch clause.

The constant internal provenance column formerly present in `attach_172_row_sites.tsv` was removed because it added no analytical information. All 172 data rows and the fields used by the logical-site key remain unchanged.

## Language note

The two complete package tables are fully translated into English across all nine columns. The three narrative reports remain the corrected source-language review copies supplied for this task. The unchanged Chinese package-table input is retained separately in the task evidence directory for direct comparison and is not duplicated inside this delivery archive.
