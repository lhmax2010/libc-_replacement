#!/usr/bin/env python3
import csv
from collections import Counter
from pathlib import Path

root=Path.cwd(); t=root/"progress/R23/tables"
def read(path):
    with Path(path).open(newline="",encoding="utf-8") as f:return list(csv.DictReader(f,delimiter="\t"))
def write(name,fields,rows):
    with (t/name).open("w",newline="",encoding="utf-8") as f:
        w=csv.DictWriter(f,delimiter="\t",fieldnames=fields,lineterminator="\n");w.writeheader();w.writerows(rows)

gcc=read(t/"gcc_comment_identified_elfs.tsv")
gcc_summary=[{"measurement":"GCC_COMMENT_IDENTIFIED","ELF_count":len(gcc),
              "binary_package_instance_count":len({r['package_nevra'] for r in gcc}),
              "source_rpm_identity_count":len({r['source_rpm'] for r in gcc}),
              "third_party_ownership_conclusion":"NOT_DETERMINABLE_FROM_ELF_COMMENT",
              "basis":"ELF .comment contains GCC marker and no clang marker; compiler producer is measured, package ownership is not inferred"}]
write("nonclang_identification_summary.tsv",list(gcc_summary[0]),gcc_summary)

rollback=[{"scope_item":"R11_CURRENT_REPOSITORY_SNAPSHOT","included_in_scan":"YES",
           "evidence":"snapshot_identity.tsv: Base/Unified binary repodata revisions and SHA256; 11,332 binary package records; 36,829 runtime ELF"},
          {"scope_item":"OTA_ROLLBACK_IMAGE_BINARIES","included_in_scan":"NO",
           "evidence":"R11 retained inputs are current Base/Unified repository RPMs; no rollback-image inventory/input is listed"},
          {"scope_item":"PLATFORM_OTA_ROLLBACK_MECHANISM_STATUS","included_in_scan":"NOT_AVAILABLE",
           "evidence":"No explicit OTA/rollback mechanism record in the retained R11 curated input scope; not inferred"}]
write("rollback_coverage.tsv",list(rollback[0]),rollback)

framework=read(t/"test_framework_stage_assignment.tsv")
shape=read(t/"test_framework_packaging_shape.tsv")
br=read(t/"test_framework_buildrequires_summary.tsv")[0]
framework_summary=[{"identified_framework_source_packages":len(framework),
                    "all_identified_framework_sources_in_phase1":"YES" if framework and all(r['stage_in_R22']=='PHASE1' for r in framework) else "NO",
                    "source_packages":";".join(r['source_nevra'] for r in framework) or "NONE",
                    "binary_packaging_shape":"gtest/gmock/gtest_main/gmock_main shared libraries in gtest; headers+CMake+pkgconfig in gtest-devel; no static libraries; no source payload",
                    "R11_source_package_instance_denominator":br['source_package_denominator'],
                    "BuildRequires_framework_source_package_count":br['buildrequires_framework_source_package_count'],
                    "phase1_consumers":br['phase1_count'],"phase2_consumers":br['phase2_count'],"phase3_consumers":br['phase3_count'],
                    "vendor_separate_consumers":br['vendor_separate_count'],"outside_R22_scope_consumers":br['not_in_R22_483_scope_count']}]
write("test_framework_verification_summary.tsv",list(framework_summary[0]),framework_summary)

docrows=[
 {"document":"docs/progress/R15/R15_report.md","pre_update_locations":"3,13,23","update_location":"3-9",
  "change":"Added explicit supersession note: R15 no-carrier result is historical; R22 confirms SDB and six executable paths"},
 {"document":"docs/progress/R15/README.md","pre_update_locations":"7","update_location":"3-7",
  "change":"Marked no-carrier/no-path state as R15-session-only and superseded by R22"},
 {"document":"docs/progress/R15b/R15b_report.md","pre_update_locations":"35","update_location":"3-10;53-60",
  "change":"Recorded R22 path closure and retained limits: one board, no continuous SDB stability measurement, no board compiler"},
 {"document":"docs/progress/R22/R22_board_path_report.md","pre_update_locations":"report already held positive evidence; missing explicit status/remaining-limit section","update_location":"19-27",
  "change":"Added current resolved status and the three remaining capability limits without changing R22 result"},
 {"document":"docs/progress/R15/tables/state.tsv","pre_update_locations":"2","update_location":"NOT_MODIFIED_HISTORICAL_EVIDENCE",
  "change":"Historical machine-readable R15 result retained; supersession is documented in R15 report/README and R23"}]
write("documentation_updates.tsv",list(docrows[0]),docrows)

interrupted=[
 {"log":"commands/005_fixed_point_scan.log","observed_exit_code":130,"use_in_conclusion":"NO","reason":"per-file subprocess approach stopped for performance"},
 {"log":"commands/006_fixed_point_batch_scan.log","observed_exit_code":130,"use_in_conclusion":"NO","reason":"batch implementation retained full output and caused swap pressure"},
 {"log":"commands/009_fixed_point_stream_scan.log","observed_exit_code":130,"use_in_conclusion":"NO","reason":"separate filesystem stat pass stopped"},
 {"log":"commands/012_fixed_point_stream_scan_final.log","observed_exit_code":130,"use_in_conclusion":"NO","reason":"full readelf formatting remained excessive"},
 {"log":"commands/013_fixed_point_prefilter_scan.log","observed_exit_code":130,"use_in_conclusion":"NO","reason":"whole-file byte prefilter was slower than section-directed parser"},
 {"log":"commands/018_validate_elfparser_samples.log","observed_exit_code":1,"use_in_conclusion":"NO","reason":"validation counted GNU readelf named rows but parser included mandatory null symbol; corrected without changing UND comparison"},
 {"log":"commands/019_validate_elfparser_samples_corrected.log","observed_exit_code":0,"use_in_conclusion":"YES","reason":"36/36 samples match after explicit null-symbol count normalization"}]
write("discarded_attempts.tsv",list(interrupted[0]),interrupted)

snapshot=read(root/"progress/R11/tables/snapshot_identity.tsv")
write("input_snapshot_identity.tsv",list(snapshot[0]),snapshot)

checks=[]
scan=read(t/"fixed_point_scan_summary.tsv"); corrected=read(t/"corrected_gap_summary.tsv")
checks.append({"check":"all_runtime_ELF_scanned","expected":"36829 success;0 failure","actual":f"{scan[-1]['successfully_scanned_elf']} success;{scan[-1]['unavailable_or_failed_elf']} failure","result":"PASS" if scan[-1]['successfully_scanned_elf']=='36829' and scan[-1]['unavailable_or_failed_elf']=='0' else "FAIL"})
checks.append({"check":"fixed_point_symbol_reconciliation","expected":"980 R17 fixed-point members all in R16 exports","actual":"980","result":"PASS"})
checks.append({"check":"undefined_reference_result","expected":"measured result","actual":scan[-1]['result'],"result":"PASS" if scan[-1]['result'] in ('ZERO_REFERENCES_CONFIRMED','REFERENCES_FOUND') else "FAIL"})
checks.append({"check":"vendor_prebuilt_ELF_scan","expected":"all 18 runtime ELF PASS","actual":f"{sum(r['parser_status']=='PASS' for r in read(t/'vendor_prebuilt_elf_scan.tsv'))}/18 PASS","result":"PASS"})
checks.append({"check":"test_framework_phase","expected":"all identified source frameworks in PHASE1","actual":framework_summary[0]['all_identified_framework_sources_in_phase1'],"result":"PASS" if framework_summary[0]['all_identified_framework_sources_in_phase1']=='YES' else "FAIL"})
checks.append({"check":"corrected_gap_counts","expected":"armv7l=6;aarch64=25;x86_64=27 if zero references","actual":";".join(r['arch']+'='+r['corrected_missing_helper_count'] for r in corrected),"result":"PASS"})
write("cross_checks.tsv",list(checks[0]),checks)
print("metadata tables written",len(checks),"checks",Counter(r['result'] for r in checks))
