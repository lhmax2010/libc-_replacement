#!/usr/bin/env python3
"""报告数值、双语片段及链接集合自检；生成逐行索引及完整差异，不重跑运行时实验。"""
import collections
import csv
import difflib
import hashlib
import pathlib
import re

base = pathlib.Path('docs/progress/P5_0909')
hq = pathlib.Path('HQ_files')
def table(name, header, rows):
    with (base / name).open('w') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(header); w.writerows(rows)
def rows(path):
    return list(csv.DictReader(path.open(), delimiter='\t'))
def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()
gnu = {r['type']: r for r in rows(pathlib.Path('docs/progress/R80/raw/layout_libstdcxx.tsv'))}
llvm = {r['type']: r for r in rows(pathlib.Path('docs/progress/R80/raw/layout_libcxx.tsv'))}
assert len(gnu) == len(llvm) == 33 and gnu.keys() == llvm.keys()
different = {t for t in gnu if (gnu[t]['size'], gnu[t]['align']) != (llvm[t]['size'], llvm[t]['align'])}
assert len(different) == 10
table('LAYOUT_RECHECK.tsv', ['type','gnu_size','llvm_size','delta','gnu_align','llvm_align'],
      [[t,gnu[t]['size'],llvm[t]['size'],int(llvm[t]['size'])-int(gnu[t]['size']),gnu[t]['align'],llvm[t]['align']] for t in gnu])
assert sha(hq / 'abi_layout_comparison.tsv') == '08a99bb4ac7d314ba20c05befd92f40301dc4bcd3572fb126c8cf68c249509a7'
print('RAW_LAYOUT: 33 concrete types; 10 different metrics; 23 matching metrics; attachment unchanged')
numbers=[]; lines=[]; hashes=[]; occurrences=[]
for path in sorted(hq.glob('*.md')):
    old = (base / 'before' / path.name).read_text()
    text = path.read_text()
    hashes.append([path.name,sha(base/'before'/path.name),sha(path)])
    directory = base / 'diffs'; directory.mkdir(exist_ok=True)
    (directory/(path.name+'.diff')).write_text(''.join(difflib.unified_diff(old.splitlines(True),text.splitlines(True),fromfile='before/'+path.name,tofile='after/'+path.name)))
    for i,line in enumerate(text.splitlines(),1):
        lines.append([path.name,i,line])
        for token in re.findall(r'\d[\d,]*(?:\.\d+)*',line):
            numbers.append([path.name,i,token,line])
        for url in re.findall(r'\]\(([^)]+)\)',line):
            occurrences.append([path.name,i,url])
    if path.name.startswith(('api_', 'abi_')):
        checked=set()
        for line in text.splitlines():
            match=re.match(r'\| `std::([^`]+)` \| (.*?) \| (.*?) \|',line)
            if not match:continue
            t=match[1]; a=int(re.search(r'\d+',match[2])[0]); b=int(re.search(r'\d+',match[3])[0])
            assert (a,b)==(int(gnu[t]['size']),int(llvm[t]['size'])),(path,t)
            checked.add(t)
            if path.name.startswith('abi_'):
                delta=int(re.search(r'[+−-]\d+',line)[0].replace('−','-'))
                assert delta==b-a
        assert checked==different,(path,checked)
        for token in ['33','10','23','13.3.0','21.1.1','220108','22.1.8','106884723786536','_Z12make_payloadv','LLVM_22_TIZEN_1','49/90','22']:
            assert token in text,(path,token)
    if path.name.endswith(('_EN.md','_en.md')):
        assert re.search(r'[\u3400-\u9fff]', '正向对照')
        found=re.findall(r'[\u3400-\u9fff]',text)
        assert not found,(path,found)
        print('EN_HAN_SCAN',path.name,'0; positive control PASS')
table('NUMBER_OCCURRENCES.tsv',['file','line','token','context'],numbers)
table('LINE_INVENTORY.tsv',['file','line','text'],lines)
table('REPORT_SHA256.tsv',['file','before_sha256','after_sha256'],hashes)
table('FINAL_LINK_OCCURRENCES.tsv',['file','line','target'],occurrences)
def blocks(name):
    text=(hq/name).read_text()
    extracted=re.findall(r'^```(?:cpp|c)\n(.*?)^```',text,re.M|re.S)
    return [re.sub(r'\s+','',re.sub(r'//[^\n]*','',block)) for block in extracted]
for a,b in [('api_compatibility_report.md','api_compatibility_report_EN.md'),('abi_comparison_preliminary.md','abi_comparison_preliminary_EN.md')]:
    assert blocks(a)==blocks(b),(a,b)
    print('BILINGUAL_CODE_IDENTICAL_EXCLUDING_COMMENTS',a,len(blocks(a)))
summary=rows(pathlib.Path('docs/progress/R116/measurement_summary.tsv'))
measurements=rows(pathlib.Path('docs/progress/R116/measurements.tsv'))
grouped=collections.Counter(tuple(r[k] for k in ['provider','abi_pair','probe','mode']) for r in measurements)
assert len(summary)==len(grouped)==30 and len(measurements)==150 and set(grouped.values())=={5}
print('R116_CELL_RECOUNT',len(grouped),'RUNS',len(measurements),'PER_CELL',set(grouped.values()))
url_results=rows(base/'URL_RESULTS.tsv')
success={r['url'] for r in url_results if r['curl_exit']=='0' and 'HTTP_CODE=200' in r['http_output']}
assert {r[2] for r in occurrences}==success
chromium={r[2] for r in occurrences if r[0].startswith('chromium_')}
assert len(chromium)==63
assert all(r[2].startswith('https://') for r in occurrences)
for name,title,end in [('chromium_research_zh.md','## 为什么 Tizen 不采用 Chromium 的方案','## 适用性结论'),('chromium_research_en.md',"## Why Tizen does not adopt Chromium's approach",'## Applicability conclusions')]:
    text=(hq/name).read_text()
    section=text.split(title,1)[1].split(end,1)[0]
    assert len(set(re.findall(r'\]\((https://[^)]+)\)',section)))==13
    assert text.splitlines()[2] == (base/'before'/name).read_text().splitlines()[2]
print('ALL_CURRENT_URLS_HTTP_200',len(success),'CHROMIUM_UNIQUE',len(chromium),'ADDED_SECTION_UNIQUE_EACH=13')
print('SELF_CHECK_PASS; numeric tokens indexed, semantic translation and coverage reviewed separately')
