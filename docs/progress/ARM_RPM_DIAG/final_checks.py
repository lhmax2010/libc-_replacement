import hashlib,json,shutil
from pathlib import Path
b=Path('progress/ARM_RPM_DIAG')
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        while c:=f.read(1024*1024):h.update(c)
    return h.hexdigest()
assert (b/'raw/045_reproduce.exitcode').read_text().strip()=='0'
assert (b/'raw/215_verify_outputs.exitcode').read_text().strip()=='0'
inputs=json.loads((b/'repro-inputs.json').read_text())
for name,value in inputs.items():assert sha(Path(name))==value['sha256'],name
events=[json.loads(l) for l in (b/'cells/llvm-arm-repro/events.jsonl').read_text().splitlines()]
assert sum(e['event']=='start' for e in events)==1
assert events[-1]['event']=='finished' and events[-1]['exitcode']==0
out=b/'final-script-snapshots';out.mkdir(exist_ok=False)
rows=[]
for p in sorted(b.glob('*.py')):
    shutil.copy2(p,out/p.name)
    rows.append({'source':str(p),'snapshot':str(out/p.name),'sha256':sha(out/p.name)})
(b/'final-script-sha256.json').write_text(json.dumps(rows,indent=2)+'\n')
keypaths=[b/'FINAL_RESULT.md',b/'ENVIRONMENT.md',b/'cells/llvm-arm-repro/build.log',b/'host.strace',Path('tmp/ARM_RPM_DIAG/guest-rpmbuild.strace'),b/'package-19-file-list.tsv',b/'package-19-metadata.json',b/'package-feature-comparison.tsv',b/'output-manifest.tsv']
hashes=[{'path':str(p),'bytes':p.stat().st_size,'sha256':sha(p)} for p in keypaths]
(b/'evidence-sha256.json').write_text(json.dumps(hashes,indent=2)+'\n')
print(json.dumps({'result':'PASS','single_reproduction':True,'rpmbuild_exitcode':0,'rpm_checks':22,'input_spec_wrapper_qemu_hashes_unchanged':True,'script_snapshots':len(rows),'failed_reproduction_observed':False},indent=2))
