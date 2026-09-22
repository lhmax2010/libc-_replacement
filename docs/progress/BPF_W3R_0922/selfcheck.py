import hashlib,json
from pathlib import Path
E=Path('progress/BPF_W3R_0922');B=E/'board-test'
def j(p):return json.loads(p.read_text())
pairs=j(B/'pairs.json');cancel=j(B/'cancel_summary.json')
assert len(pairs)==12 and all(x[k]['exitcode']==0 for x in pairs for k in ('candidate','original'))
assert all(cancel[k]['executed']==cancel[k]['passed']==20 for k in ('shared-gcc','static-gcc'))
assert j(B/'CLEANUP.json')['all_pass'] and j(B/'ROOT_RESTORED.json')['status']=='PASS'
assert (B/'mounts-before.txt').read_bytes()==(B/'mounts-after.txt').read_bytes()
assert 'TASK_DIRECTORY_ABSENT_RC=0' in (E/'raw/020_cleanup_verify.stdout.txt').read_text()
assert j(B/'EXTRACTION.json')['sha256']=='6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba'
assert j(B/'WORK_DIRECTORY.json')['free_bytes']>=500*1024*1024
comparison=[]
for x in pairs:
 comparison.append(dict(name=x['name'],stdout_equal=x['candidate']['stdout']==x['original']['stdout'],stderr_equal=x['candidate']['stderr']==x['original']['stderr']))
elf=j(E/'build-audit/ELF_COMPARISON.json')
for arch in ('armv7l','aarch64'):
 a,b=[x for x in elf if x['arch']==arch];assert (a['sha256']==b['sha256'])==(arch=='aarch64');assert (a['text_sha256']==b['text_sha256'])==(arch=='aarch64')
 assert (E/'history'/f'{arch}-targets.diff').stat().st_size==0
receipts=[]
for p in E.rglob('*.py'):receipts.append(dict(path=str(p.relative_to(E)),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
assert json.loads((E/'RPM_READONLY_CHECK.json').read_text())['status']=='PASS'
result=dict(status='EVIDENCE_MATCHED_WITH_READONLY_VIOLATION',read_only_compliance=False,incident='INCIDENT.md: four previous tmp main ELF copies rewritten by llvm-objcopy',paired_calls=12,cancel_assertions=cancel,output_equalities=comparison,source_code_sha256=receipts,size_bytes=90882270,size_mib=90882270/1048576,reported_92_mib_bytes=92*1048576)
(E/'SELFCHECK.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
