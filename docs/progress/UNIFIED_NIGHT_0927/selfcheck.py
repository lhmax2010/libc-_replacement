import pathlib,json,ast,csv,hashlib,re,subprocess
E=pathlib.Path(__file__).resolve().parent;checks={}
for p in E.glob('*.py'):ast.parse(p.read_text(),filename=str(p))
checks['scripts_parse']=True
rs=list(csv.DictReader((E/'RESULTS.tsv').open(),delimiter='\t'))
assert len(rs)==36 and all(r['status']=='NOT_OBSERVED' and r['exitcode']=='NOT_OBSERVED' for r in rs)
checks['36_cells_no_build_claim']=True
g=json.loads((E/'ARCH_INPUT_GATE.json').read_text());assert all(g['missing'].values())
checks['all_architectures_missing_input']=True
src=json.loads((E/'PROVIDER_SOURCE_METADATA.json').read_text());assert len(src)==12 and all(re.fullmatch(r'.+#[0-9a-f]{40}',x['version']['vcs']) for x in src)
checks['12_snapshot_VCS_complete']=True
for p in ('RESOURCE_inventory.json','RESOURCE_inventory_r3.json'):
    v=json.loads((E/p).read_text());assert int(v['memory.max'])<=v['total_memory_bytes']//2 and v['nice']==19 and v['ionice'].strip()=='idle'
checks['scoped_inventory_resource_limits']=True
# Avoid reading any secret store. Only scan the intended evidence files for credential-shaped content.
hits=[]
patterns=[rb'-----BEGIN (?:OPENSSH |RSA |EC |DSA )?PRIVATE KEY-----',rb'(?im)^Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'(?im)^Cookie:\s*\S+',rb'gh[pousr]_[A-Za-z0-9]{30,}',rb'(?i)(?:password|passwd|access_token|refresh_token)\s*[:=]\s*["\']?[^\s"\']{8,}']
for p in E.rglob('*'):
    if not p.is_file() or p.suffix=='.gz' or p.name=='selfcheck.py':continue
    data=p.read_bytes()
    if any(re.search(pat,data) for pat in patterns):hits.append(str(p))
assert not hits,{'paths_only':hits}
checks['credential_pattern_scan']='PASS (pattern scan, no secret stores read)'
with (E/'SCRIPT_SHA256.tsv').open('w') as f:
    f.write('path\tsha256\n')
    for p in sorted(E.glob('*.py')):f.write(f'{p.name}\t{hashlib.sha256(p.read_bytes()).hexdigest()}\n')
(E/'SELFCHECK.json').write_text(json.dumps(checks,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(checks,ensure_ascii=False,indent=2))
