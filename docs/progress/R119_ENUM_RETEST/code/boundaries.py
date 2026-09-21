from common import *
import csv,sqlite3,collections
gate('boundary_gate')
db=ROOT/'tmp/R119_batch1/full_elf/symbols.sqlite3'
con=sqlite3.connect('file:'+str(db)+'?mode=ro',uri=True)
elves={i:dict(metadata=json.loads(meta),sha256=digest,status=status) for i,meta,digest,status in con.execute('select id,metadata,sha256,status from elf')}
assert len(elves)==3085 and all(x['metadata']['arch']=='x86_64' for x in elves.values())
control_path=ROOT/'docs/progress/R119_batch1/W1/positive/bundle_add.json'
control=json.loads(control_path.read_text());known=control['elf_matches'][0]
identity=known['symbol']['demangled']
hits=list(con.execute('select elf_id,ndx from normalized_symbol where name=?',(identity,)))
defs=[i for i,n in hits if n!='UND'];refs=[i for i,n in hits if n=='UND']
assert any(elves[i]['sha256']==known['provider']['sha256'] for i in defs)
assert any(elves[i]['sha256']==known['consumer']['sha256'] for i in refs)
assert any(elves[i]['metadata']['sourcerpm']!=elves[j]['metadata']['sourcerpm'] for i in defs for j in refs)
save('BOUNDARY_CONTROL.json',dict(passed=True,identity=identity,hits=hits,matched_original_sha256=True,original_proof=str(control_path.relative_to(ROOT)),original_sha256=sha(control_path)))
decl=list(csv.DictReader((ROOT/'docs/progress/R119_DIVERGENT/PUBLIC_DECLARATIONS.tsv').open(),delimiter='\t'))
decl=[r for r in decl if r['id'] in ['T0008','T0009','T0010','T0011']]
reports=[]
for r in decl:
 assert sha(r['source_path'])==r['source_sha256']
 query=r['identity']+'('
 ds=list(con.execute("select distinct elf_id,name,ndx from normalized_symbol where instr(name,?)>0 and ndx!='UND'",(query,)))
 und=list(con.execute("select distinct elf_id,name from normalized_symbol where instr(name,?)>0 and ndx='UND'",(query,)))
 pairs=[]
 for eid,name,ndx in ds:
  for cid,cname in und:
   if cname==name and elves[eid]['metadata']['sourcerpm']!=elves[cid]['metadata']['sourcerpm']:
    pairs.append(dict(provider_id=eid,consumer_id=cid,symbol=name,provider=elves[eid],consumer=elves[cid]))
 reports.append(dict(id=r['id'],expression=r['expression'],query=query,declaration=r,definitions=[dict(id=i,name=name,elf=elves[i]) for i,name,ndx in ds],all_references=[dict(id=i,name=name,elf=elves[i]) for i,name in und],other_source_links=pairs,scope='S_R119_20260918: 3085 x86_64 indexed ELF; exact function identity; no virtual/indirect-call exclusion'))
 print(query,'definitions',len(ds),'references',len(und),'cross_source',len(pairs),flush=True)
save('BOUNDARY_QUERIES.json',reports)
assert not any(r['other_source_links'] for r in reports),'发现候选新边：须逐一核对真实ELF与结构，当前不自动升级'
save('EDGE_COUNTS.json',dict(prior_x86_pairs=18,prior_x86_symbol_edges=23,new_confirmed_pairs=0,new_confirmed_symbol_edges=0,updated_x86_pairs=18,updated_x86_symbol_edges=23,arm_targeted_pairs=2,arm_targeted_symbol_edges=2,arm_full_scope='NOT_OBSERVED',interpretation='无新增边是本轮已确认声明的限定身份查询结果；不排除未定签名、虚调用/函数指针/静态链接/范围外产物'))
save('PUBLIC_DECLARATIONS.json',decl)
