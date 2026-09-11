#!/usr/bin/env python3
import pathlib,json,hashlib,shutil,datetime
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT'
def read(n):return json.loads((o/n).read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
summary=read('matrix_summary.json');assert len(summary)==16 and all(x['repeats']==5 for x in summary)
assert sum(x['repeats'] for x in summary)==80 and sum(x['exact'] for x in summary)==60 and sum(x['maps'] for x in summary)==70
for x in read('ewk_shim_results.json'):assert x['returncode']==0 and x['maps'] and x['exact'] and x['destroyed']
states=read('ewk_reused_output_results.json');assert len(states)==10
for x in states:
 assert x['maps'] and x['destroyed']
 assert x['matches_direct']==(x['consumer']=='gnu') and x['returncode']==(0 if x['consumer']=='gnu' else 28)
assert len(read('vector_gnu14_results.json')['runs'])==5
for x in read('vector_gnu14_results.json')['runs']+read('vector_representation_results.json'):
 assert x['returncode']==0 and x['maps'] and x['representation'] and x['exact_and_destroy'] and not x['actual_SetSyncBinaryReply_called']
assert read('arm_board_cleanup.json')['target_exit']==0
integrity=read('source_integrity.json')['summary'];assert [x['regular_files'] for x in integrity]==[1100,477]
assert all(not x['mismatches'] and x['in_memory_changed_byte_positive_control'] for x in integrity)
expected={'033_angle_files.stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c','033_angle_files.stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
for name,value in expected.items():assert sha(r/'docs/progress/R115/W3/raw'/name)==value
snap=o/'snapshots/scripts';snap.mkdir(parents=True,exist_ok=True)
sources=sorted(p for p in o.iterdir() if p.suffix in {'.py','.cpp','.c','.h'})+[r/'docs/progress/API_0911/record.py']
for p in sources:shutil.copyfile(p,snap/p.name)
(o/'SCRIPT_SHA256SUMS').write_text(''.join(sha(p)+'  '+str(p.relative_to(o))+'\n' for p in sorted(snap.iterdir()) if p.is_file()))
now=datetime.datetime.now(datetime.timezone.utc);start=datetime.datetime.fromisoformat(read('raw/001_resource.time.json')['start'])
result=dict(primary_cells=16,primary_attempts=80,exact_value_attempts=60,complete_maps_attempts=70,requirements_status='PARTIAL',selfcheck='OBSERVED_COUNTS_VERIFIED_WITH_LISTED_GAPS',sealed_at=now.isoformat(),elapsed_minutes=round((now-start).total_seconds()/60,2),protected_user_files=expected)
(o/'selfcheck.json').write_text(json.dumps(result,indent=2)+'\n')
files=sorted(p for p in o.rglob('*') if p.is_file() and p.name!='SHA256SUMS' and '__pycache__' not in p.parts and 'delivery' not in p.parts and not p.name.startswith('seal_run'))
(o/'SHA256SUMS').write_text(''.join(sha(p)+'  '+str(p.relative_to(o))+'\n' for p in files))
print(json.dumps(result,indent=2));print('SEALED_FILES',len(files),'FINAL_SCRIPT_SNAPSHOTS',len(sources))
