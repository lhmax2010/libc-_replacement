#!/usr/bin/env python3
"""只暂存本批已结束的记录；不纳入运行中的全量官方日志或其他任务改动。"""
import pathlib,subprocess,sys
assert subprocess.check_output(['git','branch','--show-current']).decode().strip()=='codex/runtime-validation'
assert not subprocess.check_output(['git','diff','--cached','--name-only']).strip(),'existing staged changes'
r=pathlib.Path('docs/progress/IMPL_0908');stage=r/'W3';paths=[]
receipt=sys.argv[1] if len(sys.argv)>1 else '010_w3_checkpoint_stage'
paths+=[p for p in stage.iterdir() if p.is_file() and p.suffix in ('.md','.tsv','.json','.py','.cpp','.args')]
paths+=[stage/'SNAPSHOT_SHA256SUMS',stage/'PAYLOAD_SHA256SUMS',stage/'PAYLOAD_final_SHA256SUMS',stage/'PAYLOAD_final2_SHA256SUMS']
if (stage/'official_evidence.tgz').exists():paths.append(stage/'official_evidence.tgz')
paths+=[p for p in (stage/'snapshots').rglob('*') if p.is_file()]
paths+=[p for p in (stage/'failure_details').rglob('*') if p.is_file()]
paths+=[p for p in (stage/'changed_to_pass_details').rglob('*') if p.is_file()]
for p in (stage/'raw').rglob('*.exitcode'):
 if any(part.startswith('lit_') for part in p.relative_to(stage/'raw').parts):continue
 stem=str(p)[:-len('.exitcode')]
 for suffix in ['.command.txt','.stdout','.stderr','.exitcode','.time.json']:
  q=pathlib.Path(stem+suffix)
  if q.exists():paths.append(q)
paths+=[p for p in (r/'code').iterdir() if p.is_file() and p.suffix in ('.py','.cpp','.txt')]
paths+=[pathlib.Path('docs/progress/IMPL_STATUS_0908.md'),pathlib.Path('docs/progress/IMPL_QUESTIONS_0908.md')]
for p in (r/'delivery').glob('*.exitcode'):
 stem=str(p)[:-len('.exitcode')]
 for suffix in ['.command.txt','.stdout','.stderr','.exitcode','.time.json']:
  q=pathlib.Path(stem+suffix)
  if q.exists():paths.append(q)
paths=sorted(set(paths));print('CHECKPOINT_FILES',len(paths),flush=True)
assert all(p.stat().st_size<100*1024*1024 for p in paths),'oversize GitHub file'
raise SystemExit(subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(r/'delivery'/receipt),'git','add','--',*map(str,paths)]).returncode)
