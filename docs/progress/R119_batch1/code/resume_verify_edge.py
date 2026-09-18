#!/usr/bin/env python3
"""复核人工选定的扩大索引包边，保留原符号、DT_NEEDED 和安装头。"""
from common import *
import csv,gzip,json,re,time

d=OUT/'W1/full_elf'
inventory=json.loads((d/'INVENTORY.json').read_text())
specs=json.loads((OUT/'code/resume_edge_specs.json').read_text())
if len(sys.argv)>1:specs=[s for s in specs if s['key'] in sys.argv[1:]]
with (ROOT/'docs/progress/P7_0909/tables/packages.tsv').open() as f:
    devel={r['checksum'] for r in csv.DictReader(f,delimiter='\t') if r['devel_name']=='YES'}
headers={}
with gzip.open(ROOT/'docs/progress/P7_0909/tables/headers.tsv.gz','rt') as f:
    for h in csv.DictReader(f,delimiter='\t'):
        if h['arch']!='x86_64' or h['status']!='PASS' or h['rpm_sha256'] not in devel:continue
        for s in specs:
            if h['path']==s['header'] and h['sourcerpm']==inventory[s['provider_id']-1]['sourcerpm']:
                headers[s['key']]=h
for s in specs:
    started=time.monotonic();matched={};dynamic={};metadata={}
    for side in ['consumer','provider']:
        i=s[side+'_id'];meta=inventory[i-1];metadata[side]=meta
        p=OLD/'tmp/R11/extracted'/meta['rpm_sha256'][:2]/meta['rpm_sha256']/meta['path'].lstrip('/')
        sha=digest(p);frozen=json.loads((d/f'{i:04d}.json').read_text());assert sha==frozen['sha256']
        indexes=[]
        with gzip.open(d/f'{i:04d}.stdout.gz','rt') as f:
            for line in f:
                fields=line.split(None,7)
                if len(fields)==8 and fields[0].rstrip(':').isdigit() and fields[7].strip().split('@')[0].strip()==s['symbol']:
                    indexes.append(fields[0])
        assert indexes,(s['key'],side)
        rc,text,raw=record('resume_'+s['key']+'_'+side+'_raw',['readelf','--dyn-syms','--wide',str(p)]);assert rc==0
        hits=[l for l in text.splitlines() if l.split() and l.split()[0] in indexes]
        hits=[l for l in hits if (l.split()[6]=='UND')==(side=='consumer')]
        assert hits,(s['key'],side)
        matched[side]=dict(path=str(p),sha256=sha,inventory=meta,record=raw,lines=hits)
        rc,text,raw=record('resume_'+s['key']+'_'+side+'_dynamic',['readelf','--dynamic','--wide',str(p)]);assert rc==0
        dynamic[side]=dict(record=raw,needed=re.findall(r'\(NEEDED\).*?\[(.*?)\]',text),soname=re.findall(r'\(SONAME\).*?\[(.*?)\]',text))
    consumer_names={l.split()[7].split('@')[0] for l in matched['consumer']['lines']}
    provider_names={l.split()[7].split('@')[0] for l in matched['provider']['lines']}
    assert consumer_names & provider_names
    assert set(dynamic['provider']['soname']) & set(dynamic['consumer']['needed']),dynamic
    h=headers[s['key']];p=pathlib.Path(h['cache_path']);assert digest(p)==h['file_sha256'];lines=p.read_text().splitlines()
    hs=[dict(metadata=h,line=i+1,context='\n'.join(f'{j+1}: {lines[j]}' for j in range(max(0,i-3),min(len(lines),i+8)))) for i,t in enumerate(lines) if s['token'] in t]
    assert hs
    src=OLD/'tmp/corpus/extracted'/metadata['consumer']['sourcerpm']/'unpacked'
    rc,text,raw=record('resume_'+s['key']+'_include',['rg','-n','-uuu',s['include_pattern'],str(src)])
    proof=dict(spec=s,header_matches=hs,elf_matches=[dict(symbol={'demangled':s['symbol'],'raw_symbol':sorted(consumer_names&provider_names)},consumer=matched['consumer'],provider=matched['provider'],dynamic=dynamic)],
               source_includes=[dict(root=str(src.parent),record=raw,exit=rc,matches=text.splitlines())],verification_seconds=time.monotonic()-started,
               limit='静态原符号交集 + SONAME/DT_NEEDED + 公开安装头。源码包含记录仍需核对条件和中间头，不等于运行覆盖。')
    save(OUT/'W1/positive'/(s['key']+'.json'),proof)
    print(s['key'],len(hs),'headers',len(text.splitlines()),'includes','dynamic',dynamic,flush=True)
