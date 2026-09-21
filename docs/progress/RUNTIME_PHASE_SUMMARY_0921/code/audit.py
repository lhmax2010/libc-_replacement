#!/usr/bin/env python3
"""对汇编产物做一致性检查；不执行技术测量。"""
import collections
import csv
import hashlib
import json
import re
from pathlib import Path
from urllib.parse import unquote

OUT=Path(__file__).resolve().parents[1]
ROOT=OUT.parents[2]

def rows(name):
    with (OUT/name).open() as f:
        return list(csv.DictReader(f,delimiter='\t'))

inputs=json.loads((OUT/'INPUT_SHA256.json').read_text())
assert all(hashlib.sha256((ROOT/p).read_bytes()).hexdigest()==sha for p,sha in inputs.items())
edges=rows('EDGES.tsv')
assert len(edges)==23
assert len({(e['consumer'],e['provider']) for e in edges})==18
assert len(rows('CANDIDATE_MAP.tsv'))==44
assert len(rows('NEXT_STAGE.tsv'))==23
assert len(rows('BUILTIN_TYPES.tsv'))==13
assert len(rows('BACKLOG.tsv'))==62
for e in edges:
    assert e['x86_raw_symbol'] in e['x86_consumer_raw_lines']
    assert e['x86_raw_symbol'] in e['x86_provider_raw_lines']
    assert 'UND' in e['x86_consumer_raw_lines']
    assert re.fullmatch('[0-9a-f]{64}',e['x86_consumer_sha256'])
    assert re.fullmatch('[0-9a-f]{64}',e['x86_provider_sha256'])
arm=[e for e in edges if e['armv7l']=='ELF_UND_DEF_CONFIRMED']
assert [(e['consumer'],e['provider']) for e in arm]==[('dali2-ui-foundation','dali2'),('zypper','libzypp')]
for e in arm:
    assert e['arm_raw_symbol'] in e['arm_consumer_raw_lines']
    assert e['arm_raw_symbol'] in e['arm_provider_raw_lines']
assert edges[13]['armv7l']=='NOT_OBSERVED'
assert '非直接内建差异' in edges[22]['category_zh']
assert 283+114+111+4+60==572
assert 15770+10+13+15793==31586
assert all(r['current_provider_availability']=='NOT_RECHECKED' for r in rows('NEXT_STAGE.tsv'))

checked_links=[]
for p in OUT.glob('*.md'):
    text=p.read_text()
    for dest in re.findall(r'\]\(([^)]+)\)',text):
        if '://' in dest:continue
        file,sep,anchor=unquote(dest).partition('#')
        target=(p.parent/file).resolve() if file else p
        if target not in [OUT/'AUDIT.json',OUT/'SCRIPT_SHA256.json']:
            assert target.exists(),(p.name,dest)
        if anchor and target.parent==OUT:
            content=target.read_text()
            assert f'id="{anchor}"' in content or f'#{anchor}' in content,(p.name,dest,'anchor')
        checked_links.append((p.name,dest))
    if p.name in ['SUMMARY_zh.md','SUMMARY_en.md']:
        assert '{{' not in text
        visible=re.sub(r'\]\([^)]+\)',']',text)
        assert not re.search(r'\b[RP]\d{1,3}(?:_|\b)',visible),p.name
    if p.name.endswith('_en.md'):
        assert not re.search('[\u3400-\u9fff]',text),(p.name,'Chinese text in English report')

# 表格来自共同数据。正文的关键数字也须同时出现，但不把语义自检伪装成机器证明。
counts=['44','16','21','18','23','3,085','10,946','28%','62','39','65','15,793','572','283','114','111','60','15,770','201','495','295','13']
for lang in ['zh','en']:
    text=(OUT/f'SUMMARY_{lang}.md').read_text()
    assert all(n in text for n in counts),(lang,'missing count')

scripts={str(p.relative_to(OUT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (OUT/'code').glob('*') if p.is_file()}
(OUT/'SCRIPT_SHA256.json').write_text(json.dumps(scripts,indent=2)+'\n')
result=dict(status='PASS',scope='artifact consistency, not new technical measurements',
    input_files_checked=len(inputs),inputs_unchanged=True,checked_local_links=len(checked_links),
    original_candidates=44,x86_package_pairs=18,x86_symbol_edges=23,arm_targeted_elf_edges=2,
    backlog_rows=62,type_rows=13,english_cjk_count=0,new_measurements=False,
    semantic_review='manual; see SELF_CHECK.md',no_existing_artifact_written=True)
(OUT/'AUDIT.json').write_text(json.dumps(result,indent=2)+'\n')
assert (OUT/'AUDIT.json').exists() and (OUT/'SCRIPT_SHA256.json').exists()
print(json.dumps(result,ensure_ascii=False))
