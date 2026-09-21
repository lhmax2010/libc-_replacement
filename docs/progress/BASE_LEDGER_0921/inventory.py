import csv,hashlib,json,pathlib,re
base=pathlib.Path(__file__).resolve().parent;root=base.parent.parent
csv.field_size_limit(10000000)
paths=['docs/progress/R101/tables/base_all_source_packages.tsv','docs/progress/R100/tables/base_cpp_package_decisions.tsv','docs/progress/R109/tables/base_all_source_packages_updated.tsv','docs/progress/BUILD_WEEKEND_0918/w4/PACKAGE_STATUS.tsv']
tables=[];identities=[]
for name in paths:
 p=root/name;rows=list(csv.DictReader(p.open(),delimiter='\t'));tables.append(rows)
 identities.append(dict(path=name,sha256=hashlib.sha256(p.read_bytes()).hexdigest(),rows=len(rows)))
full,cpp,updated,published=tables
assert len(full)==254 and len(cpp)==73
names={r['source_package'] for r in cpp}
assert names=={r['源码包名'] for r in full if r['是否含C++及依据'].startswith('是')}
byfull={r['源码包名']:(i+2,r) for i,r in enumerate(full)}
byupdate={r['源码包名']:(i+2,r) for i,r in enumerate(updated)}
bypub={r['package']:(i+2,r) for i,r in enumerate(published)}
result=[]
for i,r in enumerate(cpp):
 name=r['source_package'];vcs=r['source_vcs']
 matches=re.findall(r'([^;# ]+/[^;# ]+)#([a-f0-9]{40})',vcs)
 assert matches,(name,vcs)
 repos=sorted({m[0] for m in matches});assert len(repos)==1,(name,repos)
 result.append(dict(package=name,repo_path=repos[0],url='ssh://lhmax2025@review.tizen.org:29418/'+repos[0],r100_revision=matches[0][1],branch='sandbox/lhmax2025/'+('libcxx-ehabi-backport' if name=='llvm' else 'libcxx-migration'),r100_line=i+2,r100=r,r101_line=byfull[name][0],r101=byfull[name][1],updated_line=byupdate[name][0],updated=byupdate[name][1],published_line=bypub.get(name,(None,None))[0],published=bypub.get(name,(None,None))[1]))
(base/'INPUT_IDENTITIES.json').write_text(json.dumps(identities,indent=2)+'\n')
(base/'inventory.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
print('R101=254; R100 C++=73; candidate names exactly match; published='+str(len(bypub)))
for r in result: print(r['package'],r['r100']['decision'],r['repo_path'],sep='\t')
