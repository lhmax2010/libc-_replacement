#!/usr/bin/env python3
"""只读核对输入，生成审计表；不修改 HQ 报告或原始证据。"""
import collections, csv, hashlib, json, pathlib, re, subprocess

ROOT = pathlib.Path(__file__).resolve().parents[3]
OUT = ROOT / 'docs/progress/P4_0909'
HQ = ROOT / 'HQ_files'

def table(name, header, rows):
    with (OUT / name).open('w', newline='') as f:
        w = csv.writer(f, delimiter='\t'); w.writerow(header); w.writerows(rows)

def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    inputs = ['api_compatibility_report.md', 'abi_comparison_preliminary.md', 'chromium_research_en.md', 'chromium_research_zh.md']
    table('INPUT_SHA256.tsv', ['file','sha256'], [(str(p.relative_to(ROOT)),sha(p)) for p in sorted(HQ.iterdir()) if p.is_file()])
    numbers=[]; paragraphs=[]; links=[]; blocks=[]
    for name in inputs:
        p=HQ/name; text=p.read_text()
        for i,line in enumerate(text.splitlines(),1):
            if line.strip(): paragraphs.append([name,i,line])
            for m in re.finditer(r'\d[\d,]*(?:\.\d+)*',line):
                kind='标识符/URL/版本或编号（须结合上下文）' if ('http' in line or re.search(r'\w'+re.escape(m.group()),line)) else '数值/编号（须结合上下文）'
                numbers.append([name,i,m.group(),kind,line])
            for link in re.findall(r'\]\(([^)]+)\)',line):
                if link.startswith('http'): state='HTTP_PENDING'
                else: state='EXISTS' if (p.parent/link.split('#')[0]).exists() else 'MISSING_AT_DELIVERY_LOCATION'
                links.append([name,i,link,state])
        for j,m in enumerate(re.finditer(r'^```([^\n]*)\n(.*?)^```',text,re.M|re.S),1):
            blocks.append([name,j,text[:m.start()].count('\n')+1,m.group(1),m.group(2)])
    table('NUMBER_OCCURRENCES.tsv',['report','line','token','lexical_kind','context'],numbers)
    table('LINE_INVENTORY.tsv',['report','line','text'],paragraphs)
    table('LINK_INVENTORY.tsv',['report','line','target','local_state'],links)
    table('CODE_BLOCKS.tsv',['report','block','line','language','source'],blocks)
    original=ROOT/'docs/progress/R116/REPORT_EN.md'
    print('CHROMIUM_EN_BYTE_IDENTICAL', (HQ/inputs[2]).read_bytes()==original.read_bytes(),sha(original))
    original_zh=ROOT/'docs/progress/R116/REPORT_ZH.md'
    print('CHROMIUM_ZH_BYTE_IDENTICAL', (HQ/inputs[3]).read_bytes()==original_zh.read_bytes(),sha(original_zh))
    text=(HQ/inputs[2]).read_text()
    sec=text.split("## Why Tizen does not adopt Chromium's approach",1)[1].split('## Applicability conclusions',1)[0]
    allurls=sorted(set(re.findall(r'\]\((https?://[^)]+)\)',text)))
    sectionurls=sorted(set(re.findall(r'\]\((https?://[^)]+)\)',sec)))
    table('URL_TARGETS.tsv',['url','in_added_section'],[(u,u in sectionurls) for u in allurls])
    print('URL_UNIQUE_WHOLE',len(allurls),'URL_UNIQUE_ADDED_SECTION',len(sectionurls))
    local=[x for x in links if not x[2].startswith('http')]
    print('LOCAL_LINK_OCCURRENCES',len(local),'MISSING',sum(x[3].startswith('MISSING') for x in local),'UNIQUE',len({x[2] for x in local}))
    stripped=re.sub(r'\]\([^)]*\)','](LINK)',text)
    print('INTERNAL_IDS_OUTSIDE_LINKS',re.findall(r'\b(?:R\d{2,3}|D-\d+|W[1-4]|P[1-4])\b',stripped))
    rows=list(csv.DictReader((ROOT/'docs/progress/R80/tables/d11_layout_comparison.tsv').open(),delimiter='\t'))
    rows=[r for r in rows if r['family']!='SUMMARY']
    attached=list(csv.DictReader((HQ/'abi_layout_comparison.tsv').open(),delimiter='\t'))
    print('LAYOUT_ROWS',len(rows),'DIFFERENT',sum(x['same_layout_metrics']=='NO' for x in rows))
    attached=[r for r in attached if r['family']!='SUMMARY']
    assert rows==attached
    print('ATTACHED_LAYOUT_IDENTICAL',True,'TYPE_ROWS',len(attached),'EXCLUDED_SUMMARY_ROWS',1)
    table('LAYOUT_RECHECK.tsv',list(rows[0]),[list(x.values()) for x in rows])
    ms=list(csv.DictReader((ROOT/'docs/progress/R116/measurements.tsv').open(),delimiter='\t'))
    sm=list(csv.DictReader((ROOT/'docs/progress/R116/measurement_summary.tsv').open(),delimiter='\t'))
    print('MEASUREMENTS_ROWS',len(ms),'CELLS',len(sm),'RUNS_PER_CELL',sorted({x['runs'] for x in sm}))
    print('MEASUREMENT_COLUMNS',list(ms[0]))
    assert len(ms)==150 and len(sm)==30 and all(x['runs']=='5' for x in sm)
    grouped=collections.Counter(tuple(r[k] for k in ['provider','abi_pair','probe','mode']) for r in ms)
    assert len(grouped)==30 and set(grouped.values())=={5}
    for r in ms:
        ev=ROOT/r['evidence']
        for suffix in ['.stdout','.stderr','.exitcode','.command.txt']:
            assert pathlib.Path(str(ev)+suffix).exists(),(ev,suffix)
    print('MEASUREMENT_PER_CELL_AND_EVIDENCE_PATHS=PASS')
    print('SCRIPT_SHA256',sha(pathlib.Path(__file__)))

if __name__=='__main__':main()
