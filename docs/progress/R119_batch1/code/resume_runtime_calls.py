#!/usr/bin/env python3
"""运行库必须按实际库文件、符号交集及消费者 NEEDED 确认。"""
from common import *
import csv,gzip,json,re
d=OUT/'W1/full_elf'
inv=json.loads((d/'INVENTORY.json').read_text())
with (OUT/'W1/ORDER_495.tsv').open() as f:
    names={r['entity'] for r in list(csv.DictReader(f,delimiter='\t'))[:200]}
alias={'basic_string<char,':'string','basic_string<wchar_t,':'wstring','basic_istream<char,':'istream','basic_ostream<char,':'ostream','basic_ifstream<char,':'ifstream','basic_ofstream<char,':'ofstream','basic_stringstream<char,':'stringstream','basic_ostringstream<char,':'ostringstream','basic_istringstream<char,':'istringstream','basic_streambuf<char,':'streambuf','basic_ios<char,':'ios','basic_string_view<char,':'string_view'}
def entities(s):
    s=s.replace('std::__cxx11::','std::').replace('std::__1::','std::')
    found=set()
    for n in re.findall(r'\bstd::[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*',s):
        while n:
            if n in names:found.add(n)
            n=n.rpartition('::')[0]
    for a,n in alias.items():
        if 'std::'+a in s and 'std::'+n in names:found.add('std::'+n)
    return found
def symbols(i):
    with gzip.open(d/f'{i:04d}.stdout.gz','rt') as f:
        for line in f:
            fields=line.split(None,7)
            if len(fields)==8 and fields[0].rstrip(':').isdigit():
                yield fields[6],re.sub(r'\s+\(\d+\)$','',fields[7].strip()).replace('@@','@')
runtime={i:m for i,m in enumerate(inv,1) if re.match(r'^lib(?:stdc\+\+|c\+\+|c\+\+abi|supc\+\+)\.so(?:\.|$)',pathlib.PurePosixPath(m['path']).name)}
defs={}
for i,m in runtime.items():
    path=OLD/'tmp/R11/extracted'/m['rpm_sha256'][:2]/m['rpm_sha256']/m['path'].lstrip('/')
    rc,txt,raw=record(f'resume_runtime_provider_{i}',['readelf','--dynamic','--wide',str(path)]);assert rc==0
    soname=re.findall(r'\(SONAME\).*?\[(.*?)\]',txt);assert len(soname)==1
    for ndx,name in symbols(i):
        es=entities(name)
        if ndx!='UND' and es:defs.setdefault(name,[]).append((i,soname[0],es))
found={}
for i,m in enumerate(inv,1):
    if i in runtime:continue
    candidates=[]
    for ndx,name in symbols(i):
        if ndx!='UND' or name not in defs:continue
        for p,soname,es in defs[name]:
            unseen=es-found.keys()
            if unseen:candidates.append((p,soname,name,unseen))
    if not candidates:continue
    path=OLD/'tmp/R11/extracted'/m['rpm_sha256'][:2]/m['rpm_sha256']/m['path'].lstrip('/')
    rc,txt,raw=record(f'resume_runtime_consumer_{i}',['readelf','--dynamic','--wide',str(path)]);assert rc==0
    needed=re.findall(r'\(NEEDED\).*?\[(.*?)\]',txt)
    for p,soname,name,es in candidates:
        if soname not in needed:continue
        for n in es:
            found.setdefault(n,dict(entity=n,consumer_id=i,provider_id=p,consumer=m,provider=inv[p-1],symbol=name,needed=soname,dynamic_record=raw,consumer_symbols=f'W1/full_elf/{i:04d}.stdout.gz',provider_symbols=f'W1/full_elf/{p:04d}.stdout.gz',status='RUNTIME_CALL',limit='仅证明至少一个运行库符号关系；类型可仅出现在模板参数内，不证明对象包间传递。'))
assert 'std::string' in found,'实际正向对照：平台 string 运行库调用'
save(OUT/'W1/RUNTIME_CALL_PROOFS.json',found)
print(json.dumps(dict(runtime_libraries=list(runtime),positive_entities=sorted(found),count=len(found),negative_classifications=0),ensure_ascii=False))
