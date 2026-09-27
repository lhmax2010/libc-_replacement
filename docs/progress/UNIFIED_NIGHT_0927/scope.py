import csv,json,pathlib,collections
E=pathlib.Path(__file__).resolve().parent; T=pathlib.Path('tmp/UNIFIED_NIGHT_0927')
sources=json.loads((T/'source-index.json').read_text()); bins=json.loads((T/'packages-index.json').read_text())
providers=json.loads((E/'PROVIDER_SOURCE_METADATA.json').read_text()); names={s['name'] for s in providers}
src_by_rpm={s['url'].split('/')[-1]:s for s in sources}; src_by_name={s['name']:s for s in sources}
provides=collections.defaultdict(list); cxx=collections.defaultdict(set)
for b in bins:
    s=src_by_rpm.get(b['source_rpm'])
    if not s:continue
    for p in b['provides']:provides[p['name']].append((s['name'],b['name'],b['arch']))
    for r in b['requires']:
        if any(x in r['name'] for x in ('libstdc++.so','libc++.so','libc++abi.so')):cxx[s['name']].add(b['name']+':'+r['name'])
rows=[]; direct=set(); edges=set()
for s in providers:
    for r in s['requires']:
        matches=provides.get(r['name'],[])
        for name,binary,arch in sorted(set(matches)):
            iscpp=bool(cxx.get(name)) or name in names
            rows.append([s['name'],r['name'],name,binary,arch,'CXX_OBSERVED' if iscpp else 'CXX_NOT_OBSERVED',';'.join(sorted(cxx.get(name,())))])
            if iscpp and name!=s['name']:direct.add(name);edges.add((name,s['name']))
selected=names|direct
internal=[]
for n in sorted(selected):
    for r in src_by_name[n]['requires']:
        for dep,binary,arch in sorted(set(provides.get(r['name'],[]))):
            if dep in selected and dep!=n:
                edges.add((dep,n));internal.append([dep,n,r['name'],binary,arch])
pending=set(selected); order=[]; layers=[]
while pending:
    ready=sorted(n for n in pending if not any(a in pending and b==n for a,b in edges))
    if not ready:break
    layers.append(ready);order+=ready;pending-=set(ready)
def tsv(name,head,rows):
    with (E/name).open('w') as f:w=csv.writer(f,delimiter='\t');w.writerow(head);w.writerows(rows)
tsv('DIRECT_BUILDREQUIRES.tsv',['provider','BuildRequires','Unified_source','binary','arch','CXX_evidence_status','runtime_dependency_evidence'],rows)
tsv('SET_INTERNAL_EDGES.tsv',['dependency','consumer','BuildRequires','binary','arch'],internal)
tsv('BUILD_SET.tsv',['source','role','repository','snapshot_VCS','snapshot','order','layer'],[[n,'provider' if n in names else 'direct_CXX_BuildRequires',src_by_name[n]['version']['vcs'].split('#')[0],src_by_name[n]['version']['vcs'].split('#')[-1],'tizen-unified-toolchain_20260917.132101',order.index(n)+1 if n in order else 'CYCLE',next((i+1 for i,x in enumerate(layers) if n in x),'CYCLE')] for n in sorted(selected)])
(E/'SCOPE_RESULT.json').write_text(json.dumps({'providers':sorted(names),'direct_CXX_dependencies':sorted(direct-names),'selected_count':len(selected),'layers':layers,'cycle':sorted(pending),'method':'Source RPM primary BuildRequires mapped through binary Provides and SOURCERPM; C++ positive evidence is standard-library runtime Requires or the runtime edge table. No negative proof for CXX_NOT_OBSERVED. Direct layer only; conditional source requirements reflect snapshot build settings.','unresolved_BuildRequires':sorted({r['name'] for s in providers for r in s['requires'] if r['name'] not in provides})},ensure_ascii=False,indent=2)+'\n')
print((E/'SCOPE_RESULT.json').read_text())
