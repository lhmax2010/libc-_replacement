#!/usr/bin/env python3
"""按函数/外层对象身份检索实际提供者，再找精确符号消费；不搜投影名字。"""
from common import *
import sqlite3,re,collections
gate('boundary_gate');start=time.time()
objects=json.loads((OUT/'STDLIB_OBJECT_OCCURRENCES.json').read_text())
db=ROOT/'tmp/R119_batch1/full_elf/symbols.sqlite3'
con=sqlite3.connect('file:'+str(db)+'?mode=ro',uri=True)
elves={r[0]:dict(metadata=json.loads(r[1]),sha256=r[2]) for r in con.execute('select id,metadata,sha256 from elf')}
specs={
 'errors.h':('PrepareForStrCat',['PrepareForStrCat']),
 'ostreamMethods.h':('operator<<',['operator<<']),
 'CLI11.h':('tuple_name',['tuple_name']),
 'streamOut.h':('VtStreamOut',['VtStreamOut']),
 'any.hpp':('linb::any / BT::Any / 外层对象方法',['BT::Any','linb::any','BT::Blackboard::']),
 'event.h':('TraceEvent 负载子对象',['TraceEvent'])
}
cache={};reports=[]
for h in objects:
    r=h['references'][0];p=pathlib.Path(r['cache_path']);assert sha(p)==r['sha256'];lines=p.read_text().splitlines();line=h['line'];identity,tokens=specs[p.name]
    # 同内容文件来自多个架构/RPM：保留全部源码 RPM，不因首个去重样本丢掉对应关系。
    source_rpms=sorted({x['sourcerpm'] for x in h['references']})
    key=(tuple(source_rpms),tuple(tokens))
    if key not in cache:
        providers=[i for i,e in elves.items() if e['metadata']['sourcerpm'] in source_rpms]
        hits=[]
        for token in tokens:
            for eid,name,ndx in con.execute('select elf_id,name,ndx from normalized_symbol where instr(name,?)>0',(token,)):
                if eid in providers and ndx!='UND':hits.append((eid,name))
        hits=sorted(set(hits));links=[]
        for eid,name in hits:
            for cid in sorted({x[0] for x in con.execute("select elf_id from normalized_symbol where name=? and ndx='UND'",(name,))}):
                links.append(dict(provider_id=eid,consumer_id=cid,name=name,provider=elves[eid],consumer=elves[cid],different_source_rpm=elves[cid]['metadata']['sourcerpm']!=elves[eid]['metadata']['sourcerpm']))
        cache[key]=dict(source_rpms=source_rpms,identity=identity,tokens=tokens,provider_elfs=[dict(id=i,**elves[i]) for i in providers],provider_defined_symbols=[dict(elf_id=i,name=n) for i,n in hits],exact_consumer_links=links)
    result=cache[key]
    expanded='\n'.join(f'{i+1}: {lines[i]}' for i in range(max(0,line-25),min(len(lines),line+45)))
    class_lines=[dict(line=i+1,text=s) for i,s in enumerate(lines) if re.search(r'\b(class|struct|union)\s+(any|Any|TraceEvent|storage_union)\b|\b(_any|_storage|_payload|PayloadStorage|stack_storage_t)\b',s)]
    reports.append(dict(occurrence_id=h['id'],rank=h['rank'],candidate=h['candidate'],installed_path=r['installed_path'],line=line,source_sha256=r['sha256'],source_rpms=source_rpms,result_type=h['decision']['result_type'],projection_condition=h['decision'].get('condition',''),expanded_source=expanded,outer_object_storage_lines=class_lines,query_result=result))
save(OUT/'BOUNDARY_SEARCH.json',reports)
# 名字交集方法的正向对照：已证实 bundle::Add 的旧原始证据独立保留。
control=json.loads((ROOT/'docs/progress/R119_batch1/W1/positive/bundle_add.json').read_text())
symbol=control['elf_matches'][0]['symbol']['demangled']
control_hits=list(con.execute('select elf_id,ndx from normalized_symbol where name=?',(symbol,)))
assert any(ndx=='UND' for i,ndx in control_hits) and any(ndx!='UND' for i,ndx in control_hits)
save(OUT/'BOUNDARY_QUERY_CONTROL.json',dict(symbol=symbol,observed=control_hits,assert_consumer_and_definition=True,proof='docs/progress/R119_batch1/W1/positive/bundle_add.json',proof_sha256=sha(ROOT/'docs/progress/R119_batch1/W1/positive/bundle_add.json')))
print('identity groups',len(cache),'objects',len(reports),'seconds',time.time()-start)
for h in reports:
    q=h['query_result'];print(h['occurrence_id'],h['installed_path'],'providers',len(q['provider_elfs']),'definitions',len(q['provider_defined_symbols']),'consumer_links',len(q['exact_consumer_links']),'different_source',sum(x['different_source_rpm'] for x in q['exact_consumer_links']))
    print(h['expanded_source']);print('STORAGE',h['outer_object_storage_lines'])
for key,q in cache.items():
    for edge in q['exact_consumer_links']:
        if edge['different_source_rpm']:print('CROSS_SOURCE_CANDIDATE',json.dumps(edge,ensure_ascii=False))
