"""扫描实际 devel 安装头，产生候选上下文；不自动把函数体用法判为公开签名。"""
from common import *
import csv,gzip,re,collections,time
start=time.time(); target=OUT/'W1'
with (target/'BATCH_100.tsv').open()as f: rows=list(csv.DictReader(f,delimiter='\t'))
names={r['entity'] for r in rows};counts=collections.Counter();stats=collections.Counter();samples=collections.defaultdict(list)
token=re.compile(r'\bstd\s*::\s*[A-Za-z_]\w*(?:\s*::\s*[A-Za-z_]\w*)*')
comments=re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',re.S)
def normalize(s):return re.sub(r'\s+','',s).replace('std::__cxx11::','std::').replace('std::__1::','std::')
def scan(s):
    code=comments.sub(lambda m:'\n'*m.group().count('\n')+' ',s)
    for m in token.finditer(code):
        name=normalize(m.group())
        while name and name not in names:name=name.rpartition('::')[0]
        if name:yield name,code.count('\n',0,m.start())+1
# 正负对照：多行限定名、注释/字符串、类型名作为长限定名的前缀。
control='// std::vector<int> fake;\nconst char* s="std::map";\nstd ::\n string f();\nstd::vector<int> g();\n'
observed=list(scan(control));assert observed==[('std::string',3),('std::vector',5)],observed
save(target/'HEADER_SCAN_CONTROLS.json',dict(input=control,observed=observed,expected=observed))
pkgs={}
with (ROOT/'docs/progress/P7_0909/tables/packages.tsv').open()as f:
    for r in csv.DictReader(f,delimiter='\t'):pkgs[r['checksum']]=r
fields=['entity','package','sourcerpm','installed_path','cache_path','sha256','line','text','context','private_path','signature_status']
issues=[]
with gzip.open(target/'header_occurrences.tsv.gz','wt',newline='')as out:
    w=csv.DictWriter(out,fieldnames=fields,delimiter='\t',lineterminator='\n');w.writeheader()
    with gzip.open(ROOT/'docs/progress/P7_0909/tables/headers.tsv.gz','rt')as f:
        for r in csv.DictReader(f,delimiter='\t'):
            stats['inventory_headers']+=1
            meta=pkgs.get(r['rpm_sha256'],{})
            if meta.get('devel_name')!='YES' or r['scope']!='INCLUDE_TREE':continue
            stats['devel_include_headers']+=1
            if r['status']!='PASS':issues.append(dict(path=r['path'],package=r['name'],reason=r['status']));continue
            # Plain-C headers cannot contain std-qualified type declarations;
            # unresolved/macro-only headers remain included, not silently dropped.
            if r['form']=='C_DECLARATION_SHAPE':stats['plain_c_shape_skipped']+=1;continue
            p=pathlib.Path(r['cache_path'])
            try:data=p.read_bytes()
            except OSError as e:issues.append(dict(path=str(p),reason=str(e)));continue
            sha=__import__('hashlib').sha256(data).hexdigest()
            if sha!=r['file_sha256']:issues.append(dict(path=str(p),reason='SHA256_CHANGED'));continue
            stats['read_headers']+=1;s=data.decode(errors='replace');lines=s.splitlines()
            found=set(scan(s))
            if found:stats['matched_headers']+=1
            for name,n in sorted(found):
                item=dict(entity=name,package=r['name'],sourcerpm=r['sourcerpm'],installed_path=r['path'],cache_path=str(p),sha256=sha,line=n,text=lines[n-1].strip(),context=' | '.join(lines[max(0,n-3):n+3]),private_path=r['private_path'],signature_status='HEADER_OCCURRENCE_REQUIRES_REVIEW')
                w.writerow(item);counts[name]+=1
                # retain an auditable diverse set, prefer non-private non-toolchain headers.
                rank=(r['private_path']=='YES',r['sourcerpm'].startswith(('gcc-','llvm-')),len(item['text'])>400)
                samples[name].append((rank,item))
                if len(samples[name])>80:samples[name]=sorted(samples[name],key=lambda x:x[0])[:40]
            if stats['read_headers']%10000==0:print(dict(stats),flush=True)
save(target/'HEADER_COVERAGE.json',dict(stats=stats,issues=issues,occurrences=counts,elapsed_seconds=time.time()-start))
save(target/'HEADER_REVIEW_QUEUE.json',{name:[r for _,r in sorted(items,key=lambda x:x[0])[:30]]for name,items in samples.items()})
print(dict(stats),dict(counts),len(issues),flush=True)
