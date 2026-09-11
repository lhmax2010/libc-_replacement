#!/usr/bin/env python3
"""为核对材料生成摘要与时间记录；不包含正在写入的日志或自哈希。"""
import csv,datetime,hashlib,json,pathlib
b=pathlib.Path('docs/progress/P4_0909')
start=json.loads((b/'raw/001_resource.time.json').read_text())['start']
now=datetime.datetime.now(datetime.timezone.utc)
print('START_UTC',start,'SEAL_UTC',now.isoformat())
print('ELAPSED_SECONDS',(now-datetime.datetime.fromisoformat(start)).total_seconds())
assert (now-datetime.datetime.fromisoformat(start)).total_seconds()<3*3600
files=[]
for p in sorted(b.rglob('*')):
    if not p.is_file() or '__pycache__' in p.parts or 'delivery' in p.parts:continue
    if p.name in ['SHA256SUMS','EVIDENCE_INDEX.tsv']:continue
    if '/raw/' in str(p) and not pathlib.Path(str(p).rsplit('.',1)[0]+'.exitcode').exists():
        # command.txt/time.json have compound suffixes; test their actual recorder stem.
        stem=str(p)
        for suffix in ['.command.txt','.time.json','.stdout','.stderr','.exitcode']:
            if stem.endswith(suffix):stem=stem[:-len(suffix)];break
        if not pathlib.Path(stem+'.exitcode').exists():continue
    files.append((hashlib.sha256(p.read_bytes()).hexdigest(),str(p.relative_to(b))))
with (b/'SHA256SUMS').open('w') as f:
    for h,p in files:f.write(h+'  '+p+'\n')
print('HASHED_ARTIFACTS',len(files))
for p in sorted(b.glob('*.py')):print('SCRIPT_SHA256',hashlib.sha256(p.read_bytes()).hexdigest(),p)
sources=[
'docs/progress/R80/raw/036_retry_cross_layout_case_with_deque.stdout',
'docs/progress/R80/raw/041_rebuild_extended_layout_probe.stdout',
'docs/progress/R80/raw/017_check_compilers_and_platform_edges.stdout',
'docs/progress/R80/raw/032_extract_libxml_and_verify_clang.stdout',
'docs/progress/R80/raw/d09_libcxx_run.txt','docs/progress/R80/raw/d09_libstdcxx_run.txt',
'docs/progress/R80/code/layout_provider.cpp','docs/progress/R80/code/layout_consumer.cpp',
'docs/progress/R80/code/layout_probe.cpp','docs/progress/R80/tables/d09_common17.tsv',
'docs/progress/R78/code/icu_probe.cpp','docs/progress/R78/code/boundary_error.hpp',
'docs/progress/R78/code/old_plugin.cpp','docs/progress/R78/code/new_main.cpp',
'docs/progress/R78/raw/050_run_armv7l_installed_icu_real_coexistence.stdout',
'docs/progress/R78/raw/029_run_armv7l_cross_runtime_exception.stdout',
'docs/progress/R78/raw/038_run_x86_64_cross_runtime_exception.stderr',
'docs/progress/R69/code/evidence/commands/013_three_tree_gate_semantic_audit.log',
'docs/progress/R114/W2/raw/012_glibc240.stdout','docs/progress/R114/W2/platform_shared_mutex.hpp',
'docs/progress/R116/measurements.tsv','docs/progress/R116/measurement_summary.tsv',
'docs/progress/R116/raw/054_scan_full_archive.stdout','docs/progress/R116/raw/057_supplement.stdout',
'docs/progress/R116/raw/036_platform_symbols.stdout','docs/progress/R116/raw/037_experiment_symbols.stdout',
'docs/progress/R116/raw/052_cros_changes.stdout','docs/progress/R116/sources/deps_pinned.txt',
'docs/progress/R116/sources/configsite_actual.txt','docs/progress/R116/sources/unwind_config.txt',
'docs/progress/R46/tables/no_counterpart_8.tsv','docs/progress/R58/KNOWN_GAPS.md',
'docs/progress/R110/tables/w3_edges_90_results.tsv','docs/progress/R110/tables/w3_selected_edges.tsv']
with (b/'EVIDENCE_INDEX.tsv').open('w',newline='') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['source','sha256'])
    for name in sources:
        p=pathlib.Path(name);assert p.is_file(),p
        w.writerow([name,hashlib.sha256(p.read_bytes()).hexdigest()])
print('PRIMARY_SOURCE_INDEX',len(sources))
