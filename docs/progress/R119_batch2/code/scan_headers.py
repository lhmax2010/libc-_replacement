#!/usr/bin/env python3
"""第二批安装头限定名索引；含 C 形态头；不自动做语义阴性分类。"""
import collections,csv,gzip,hashlib,json,pathlib,re,subprocess,time
root=pathlib.Path(__file__).resolve().parents[4]
base=root/'docs/progress/R119_batch2'
first=root/'docs/progress/R119_batch1'
(base/'W1').mkdir(parents=True,exist_ok=True)
start=time.monotonic()
with (first/'W1/ORDER_495.tsv').open() as f: order=list(csv.DictReader(f,delimiter='\t'))[100:200]
with (base/'W1/BATCH_100.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(order[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(order)
names={r['entity'] for r in order}
tokens=re.compile(r'\bstd\s*::\s*[A-Za-z_]\w*(?:\s*::\s*[A-Za-z_]\w*)*')
comments=re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',re.S)
def scan(s):
    s=comments.sub(lambda m:'\n'*m.group().count('\n')+' ',s)
    found=[]
    for m in tokens.finditer(s):
        n=re.sub(r'\s+','',m.group()).replace('std::__cxx11::','std::').replace('std::__1::','std::')
        while n:
            if n in names:found.append((n,s.count('\n',0,m.start())+1))
            n=n.rpartition('::')[0]
    return sorted(set(found))
control='// std::bitset<4> fake;\nstd ::\n bitset<4> bits;\nconst char* s="std::locale";\nstd::chrono::steady_clock::time_point stamp;\n'
assert scan(control)==[('std::bitset',2),('std::chrono::steady_clock::time_point',5)]
(base/'W1/CONTROLS.json').write_text(json.dumps(dict(input=control,observed=scan(control),pass_control=True),indent=2)+'\n')
with (root/'docs/progress/P7_0909/tables/packages.tsv').open() as f:
    devel={r['checksum'] for r in csv.DictReader(f,delimiter='\t') if r['devel_name']=='YES'}
stats=collections.Counter();issues=[];counts=collections.Counter();samples=collections.defaultdict(list);sample_seen=collections.defaultdict(set)
fields=['entity','package','sourcerpm','arch','installed_path','cache_path','sha256','line','text','context','private_path','signature_status']
def gate(index):
    for attempt in range(6):
        stem=base/f'raw/gate_{index}_{attempt}'
        cmd=['tools/resource_gate.sh','--level','light']
        run=subprocess.run(['python3','docs/progress/API_0911/record.py',str(stem),*cmd],cwd=root)
        if run.returncode==0:return
        if run.returncode in (20,21) and attempt<5:
            print('gate retry in 600 seconds',flush=True);time.sleep(600);continue
        raise SystemExit(run.returncode)
gate(0)
with gzip.open(base/'W1/header_occurrences.tsv.gz','wt',newline='') as stream:
    writer=csv.DictWriter(stream,fieldnames=fields,delimiter='\t',lineterminator='\n');writer.writeheader()
    with gzip.open(root/'docs/progress/P7_0909/tables/headers.tsv.gz','rt') as f:
        for r in csv.DictReader(f,delimiter='\t'):
            stats['inventory_headers']+=1
            if r['rpm_sha256'] not in devel or r['scope']!='INCLUDE_TREE':continue
            stats['selected_headers']+=1
            if r['status']!='PASS':issues.append(dict(path=r['path'],package=r['name'],reason=r['status']));continue
            p=pathlib.Path(r['cache_path'])
            try:data=p.read_bytes()
            except OSError as e:issues.append(dict(path=str(p),reason=str(e)));continue
            sha=hashlib.sha256(data).hexdigest()
            if sha!=r['file_sha256']:issues.append(dict(path=str(p),reason='SHA256_CHANGED'));continue
            text=data.decode(errors='replace');lines=text.splitlines();stats['read_headers']+=1
            for n,line in scan(text):
                item=dict(entity=n,package=r['name'],sourcerpm=r['sourcerpm'],arch=r['arch'],installed_path=r['path'],cache_path=str(p),sha256=sha,line=line,text=lines[line-1].strip(),context=' | '.join(lines[max(0,line-4):line+4]),private_path=r['private_path'],signature_status='REQUIRES_SEMANTIC_REVIEW')
                writer.writerow(item);counts[n]+=1
                key=(sha,line)
                if key not in sample_seen[n]:
                    sample_seen[n].add(key);samples[n].append(item)
            if stats['read_headers']%20000==0:
                print(dict(stats),flush=True);gate(stats['read_headers'])
(base/'W1/HEADER_COVERAGE.json').write_text(json.dumps(dict(stats=stats,issues=issues,occurrences=counts,elapsed_seconds=time.monotonic()-start,limit='限定名扫描不展开宏/模板/using，不把零命中作为无公开签名证明。'),ensure_ascii=False,indent=2)+'\n')
for n,items in samples.items():
    (base/'W1/entity_full').mkdir(exist_ok=True)
    (base/'W1/entity_full'/(n.replace('::','_')+'.json')).write_text(json.dumps(items,ensure_ascii=False,indent=2)+'\n')
(base/'W1/COUNTS.json').write_text(json.dumps(dict(counts),ensure_ascii=False,indent=2)+'\n')
print(json.dumps(dict(stats=stats,seconds=time.monotonic()-start,unique_contexts={n:len(v) for n,v in samples.items()}),ensure_ascii=False))
