#!/usr/bin/env python3
"""封存前检查本轮结论、引用、输入保护及源证据；不修改原报告。"""
import collections,csv,datetime,hashlib,pathlib,re,subprocess
b=pathlib.Path('docs/progress/P4_0909')
expected={
'api_compatibility_report.md':'58c324b956694f27d9419305ea1f9e86d140d4c3d9c729474047b5067c72bcb3',
'abi_comparison_preliminary.md':'5eea8efcefd9ae6873817b3cd5e634cce3636a5d88bdd36b87a3532df32a4d60',
'chromium_research_en.md':'d03fe0f381fe2078835b7efa399f51a6b4ae19da431078cd8f5e87317015fcf6',
'chromium_research_zh.md':'258aaed8e90f642acef89910f8f305ab4030d521add10b7c9174069e070f61da',
'api_compatibility_report_EN.md':'a00194980edf616ebe8c64f064158e8fe4937379c713fea5ccc0dfbce631f25a',
'abi_comparison_preliminary_EN.md':'a3a73416e9e2fcc4e30a28f3562ca859480d490a4da9032611065fe3d72f8811',
'abi_layout_comparison.tsv':'08a99bb4ac7d314ba20c05befd92f40301dc4bcd3572fb126c8cf68c249509a7'}
for name,h in expected.items():assert hashlib.sha256((pathlib.Path('HQ_files')/name).read_bytes()).hexdigest()==h,name
print('ALL_SEVEN_HQ_INPUTS_UNCHANGED=PASS')
count=0
for p in b.glob('*.md'):
    for link in re.findall(r'\]\(([^)]+)\)',p.read_text()):
        if link.startswith(('http://','https://')):continue
        assert (p.parent/link.split('#')[0]).exists(),(p,link)
        count+=1
print('AUDIT_LOCAL_DOCUMENT_LINKS_PASS',count)
for lang in ['en','zh']:
    s=(pathlib.Path('HQ_files')/f'chromium_research_{lang}.md').read_text()
    s=re.sub(r'\]\([^)]*\)','](LINK)',s)
    assert not re.search(r'\b(?:R\d{2,3}|D-\d+|W[1-4]|P[1-4])\b',s)
assert re.search(r'\b(?:R\d{2,3}|D-\d+|W[1-4]|P[1-4])\b','R116 D-11 W4')
print('BILINGUAL_INTERNAL_IDS_OUTSIDE_URLS_ZERO_WITH_POSITIVE_CONTROL=PASS')
urls=lambda s:set(re.findall(r'\]\((https?://[^)]+)\)',s))
assert urls(pathlib.Path('HQ_files/chromium_research_en.md').read_text())==urls(pathlib.Path('HQ_files/chromium_research_zh.md').read_text())
print('WHOLE_BILINGUAL_URL_SET_EQUAL=PASS')
baseline=list(csv.DictReader((b/'LAYOUT_RECHECK.tsv').open(),delimiter='\t'))
for side in ['libstdcxx','libcxx']:
    original=list(csv.DictReader(pathlib.Path(f'docs/progress/R80/raw/layout_{side}.tsv').open(),delimiter='\t'))
    bytype={x['type']:x for x in original}
    assert len(bytype)==33
    for x in baseline:assert (x[side+'_size'],x[side+'_align'])==(bytype[x['type']]['size'],bytype[x['type']]['align'])
print('ALL_33_LAYOUT_ROWS_MATCH_BOTH_RAW_EXECUTIONS=PASS')
for x in csv.DictReader((b/'RELATIVE_LINK_REPLACEMENTS.tsv').open(),delimiter='\t'):
    path=x['suggested_target'].split('859126994bd7e3b8bb03ee86154e23e1b04b2153/',1)[1]
    subprocess.run(['git','cat-file','-e','859126994bd7e3b8bb03ee86154e23e1b04b2153:'+path],check=True)
print('ALL_57_PROPOSED_RELATIVE_TARGETS_PRESENT_IN_PUSHED_COMMIT=PASS')
for p in b.glob('*.py'):compile(p.read_text(),str(p),'exec')
print('AUDIT_SCRIPTS_SYNTAX=PASS')
print('UTC',datetime.datetime.now(datetime.timezone.utc).isoformat())
