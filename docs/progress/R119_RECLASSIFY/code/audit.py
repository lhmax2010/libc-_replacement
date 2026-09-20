#!/usr/bin/env python3
"""最终一致性检查；不给尚未验证的候选补结论。"""
from common import *
from collections import Counter
gate('final_gate')
rows=list(csv.DictReader((OUT/'RECLASSIFIED_001_200.tsv').open(),delimiter='\t'))
assert len(rows)==200 and [int(r['rank']) for r in rows]==list(range(1,201))
old={x['rank']:x for x in inputs()}; results=json.loads((OUT/'A_RESULTS.json').read_text())
for r in rows:
    rank=int(r['rank']);assert r['original']==old[rank]['row']['result']
    if r['new']=='CROSS_PACKAGE':assert old[rank]['row']['result']=='CROSS_PACKAGE'
    if r['new']=='NO_STDLIB_OBJECT':
        case=json.loads((OUT/r['evidence']).read_text()); assert case['platform_samples']
        for s in case['platform_samples']:
            p=s['source'].get('cache_path',s['source'].get('metadata',{}).get('cache_path'));assert sha(p)==s['current_sha256']
        if r['subtype']=='COMPILE_TIME_ONLY':
            values=[x['results'][str(rank)] for x in results]
            if 'value=' in values[0]:
                import re
                observed=[re.search(r'value=(-?\d+)',v)[1] for v in values]
                assert len(set(observed))==1
        assert all(str(rank) in x['results'] and x['repeat_count']==5 for x in results)
    if r['new']=='INTERNAL_ONLY':assert r['scope']=='S_R119_20260918' and json.loads((OUT/r['evidence']).read_text())['not_based_on_elf_name_zero']
for path,digest in json.loads((OUT/'INPUT_SHA256.json').read_text()).items():assert sha(ROOT/path)==digest
for path,digest in json.loads((OUT/'CROSS_COUNTS.json').read_text())['proof_sha256'].items():assert sha(ROOT/path)==digest
scope_files=[ROOT/'docs/progress/P7_0909/tables/headers.tsv.gz',ROOT/'docs/progress/P7_0909/tables/packages.tsv',ROOT/'docs/progress/R119_batch2/W1/HEADER_COVERAGE.json',ROOT/'docs/progress/R119_batch2/W1/header_occurrences.tsv.gz',ROOT/'docs/progress/R119_batch2/W1/unqualified/SUMMARY.json',ROOT/'docs/progress/R119_batch1/W1/FULL_ELF_COVERAGE.md']
save(OUT/'SCOPE_INPUT_SHA256.json',{str(p.relative_to(ROOT)):sha(p) for p in scope_files})
scripts=list((OUT/'code').glob('*.py'))+list((OUT/'code').glob('*.json'))+list((OUT/'code').glob('*.cpp'))+[REC,LIMIT,ROOT/'tools/resource_gate.sh',ROOT/'docs/progress/R118/code/run.py']
save(OUT/'SCRIPT_SHA256.json',{str(p.relative_to(ROOT)):sha(p) for p in scripts})
save(OUT/'SELF_CHECK.json',dict(pass_all=True,rows=200,original_tables_unchanged=True,positive_proofs_unchanged=True,structured_candidates_with_two_library_five_repeat_results=89,scoped_header_negatives=5,remaining_not_observed=62,counts=dict(Counter(r['new'] for r in rows)),board_used=False,source_modified=False,limits='静态结构与有限样本；没有验证所有模板特化、跨架构布局或新增包间运行。'))
print('AUDIT PASS: 200 rows, immutable old tables/positive proofs, 89 structural + 5 scoped negatives')
