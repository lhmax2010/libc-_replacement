#!/usr/bin/env python3
"""全量 ELF 名字交集的定位队列；不自动升级为 CROSS_PACKAGE。"""
import collections
import csv
import gzip
import json
import pathlib
import re
import sqlite3

root=pathlib.Path(__file__).resolve().parents[4]
base=root/'docs/progress/R119_batch1'
db=sqlite3.connect(root/'tmp/R119_batch1/full_elf/symbols.sqlite3')
db.execute('PRAGMA cache_size=-4096')
db.execute('DROP TABLE IF EXISTS normalized_symbol')
db.execute("CREATE TABLE normalized_symbol AS SELECT elf_id, rtrim(substr(name,1,instr(name||'@','@')-1)) AS name, ndx FROM symbol")
db.execute('CREATE INDEX normalized_symbol_name ON normalized_symbol(name)')
meta={i:json.loads(m) for i,m in db.execute('SELECT id,metadata FROM elf WHERE status="PASS"')}
with (base/'W1/ORDER_495.tsv').open() as f:
    order=list(csv.DictReader(f,delimiter='\t'))[:200]
names={r['entity'] for r in order}
alias={'basic_string<char,':'string','basic_string<wchar_t,':'wstring','basic_istream<char,':'istream','basic_ostream<char,':'ostream','basic_ifstream<char,':'ifstream','basic_ofstream<char,':'ofstream','basic_stringstream<char,':'stringstream','basic_ostringstream<char,':'ostringstream','basic_istringstream<char,':'istringstream','basic_streambuf<char,':'streambuf','basic_ios<char,':'ios','basic_string_view<char,':'string_view'}
token=re.compile(r'\bstd::[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*')
def entities(s):
    s=s.replace('std::__cxx11::','std::').replace('std::__1::','std::').replace('std::filesystem::__cxx11::','std::filesystem::').replace('std::chrono::_V2::','std::chrono::')
    found=set()
    for m in token.finditer(s):
        n=m.group()
        while n:
            if n in names:
                found.add(n)
            n=n.rpartition('::')[0]
    for a,n in alias.items():
        if 'std::'+a in s:
            found.add('std::'+n)
    return found
assert {'std::string','std::basic_string'}<=entities('Example::f(std::__cxx11::basic_string<char, std::char_traits<char> > const&)')
assert not entities('Example::f(std::mapper&)')
with (base/'W1/full_elf/QUERY_CONTROLS.json').open('w') as f:
    json.dump({'positive':'Example::f(std::__cxx11::basic_string<char, std::char_traits<char> > const&)','negative':'Example::f(std::mapper&)','pass':True},f,indent=2)
queue=collections.defaultdict(list)
counts=collections.Counter()
seen=set()
runtime=lambda m:bool(re.match(r'lib(?:stdc\+\+|c\+\+|c\+\+abi|supc\+\+)\.so(?:\.|$)',pathlib.PurePosixPath(m['path']).name))
with gzip.open(base/'W1/full_elf/intersections.tsv.gz','wt',newline='') as stream:
    writer=csv.writer(stream,delimiter='\t',lineterminator='\n')
    writer.writerow(['entity','consumer_id','provider_id','consumer_package','provider_package','symbol','status'])
    for name, in db.execute('SELECT DISTINCT name FROM normalized_symbol WHERE ndx="UND"'):
        found=entities(name)
        if not found:
            continue
        cs={i for i, in db.execute('SELECT elf_id FROM normalized_symbol WHERE name=? AND ndx="UND"',(name,))}
        ps={i for i, in db.execute('SELECT elf_id FROM normalized_symbol WHERE name=? AND ndx!="UND"',(name,))}
        for c in cs:
            for p in ps:
                if c not in meta or p not in meta or runtime(meta[p]) or meta[c]['sourcerpm']==meta[p]['sourcerpm']:
                    continue
                for n in found:
                    pair=(n,meta[c]['sourcerpm'],meta[p]['sourcerpm'],name)
                    writer.writerow([n,c,p,meta[c]['sourcerpm'],meta[p]['sourcerpm'],name,'NAME_INTERSECTION_ONLY'])
                    if pair in seen:
                        continue
                    seen.add(pair)
                    counts[n]+=1
                    if len(queue[n])<40:
                        queue[n].append(dict(consumer_id=c,provider_id=p,consumer=meta[c],provider=meta[p],symbol=name))
(base/'W1/full_elf/CANDIDATE_QUEUE.json').write_text(json.dumps(queue,ensure_ascii=False,indent=2)+'\n')
(base/'W1/full_elf/QUERY_COUNTS.json').write_text(json.dumps(counts,ensure_ascii=False,indent=2)+'\n')
for r in order:
    n=r['entity']
    print(r['rank'],n,'candidate_intersections',counts[n])
    for e in queue[n][:2]:
        print(' ',e['consumer']['sourcerpm'],'->',e['provider']['sourcerpm'],e['symbol'])
print('LIMIT: 仅反修饰名字交集，需原始符号/版本节点、DT_NEEDED、公开头和包含链复核；零命中不是阴性分类。')
