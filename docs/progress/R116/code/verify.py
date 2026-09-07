#!/usr/bin/env python3
"""核对正式矩阵、快照身份和报告本地链接，不重新运行或修改实验。"""
import ast, collections, csv, hashlib, pathlib, re
r=pathlib.Path('docs/progress/R116'); rows=list(csv.DictReader((r/'measurements.tsv').open(),delimiter='\t'));assert len(rows)==150
cells=collections.defaultdict(list)
for row in rows:
 key=tuple(row[x] for x in ['provider','abi_pair','probe','mode']);cells[key].append(row)
 stem=pathlib.Path(row['evidence']);out=pathlib.Path(str(stem)+'.stdout').read_text();err=pathlib.Path(str(stem)+'.stderr').read_text()
 rc=int(pathlib.Path(str(stem)+'.exitcode').read_text());assert rc==int(row['actual_exit']);assert 'PROVIDER_ASSERT=PASS' in out
 if row['mode']=='rethrow':
  if row['abi_pair']=='patched':assert rc==0 and 'canceled=1 caught=1 cleaned=1' in out and 'ASSERTIONS=PASS' in out
  else:assert rc==-6 and 'caught_foreign=1 rethrowing=1' in out and 'uncaught foreign exception' in err
 elif row['probe']=='wait_retained':assert rc==86 and 'event=terminate cleanup_count=0' in out
 elif row['probe']=='wait_removed':assert rc==0 and 'canceled=1 cleanup_count=1' in out
 else:assert rc==0 and 'ASSERTIONS=PASS' in out
assert len(cells)==30
for key,group in cells.items():assert sorted(int(x['repeat']) for x in group)==[1,2,3,4,5]
print('MATRIX_AND_VALUE_ASSERTIONS=PASS cells=30 runs=150')
for row in csv.DictReader((r/'snapshot_origins.tsv').open(),delimiter='\t'):
 assert hashlib.sha256(pathlib.Path(row['snapshot']).read_bytes()).hexdigest()==row['sha256']
print('SNAPSHOT_HASHES=PASS')
for p in (r/'code').glob('*.py'):ast.parse(p.read_text())
print('SCRIPT_SYNTAX=PASS')
missing=[]
for p in r.glob('*.md'):
 for link in re.findall(r'\]\(([^)]+)\)',p.read_text()):
  if '://' in link or link.startswith('#'):continue
  target=link.split('#')[0]
  if not (p.parent/target).exists():missing.append((str(p),link))
assert not missing,missing
print('LOCAL_REPORT_LINKS=PASS')
for p in [r/'REPORT_ZH.md',r/'REPORT_EN.md']:
 assert not re.search(r'\bR\d{2,3}\b',p.read_text())
print('NO_INTERNAL_TASK_LABELS_IN_HQ_REPORTS=PASS')
assert pathlib.Path('/proc/self/exe').resolve().is_file();print('PROCESS_SCAN_POSITIVE_CONTROL=PASS')
experiment=pathlib.Path('tmp/R116').absolute();active=[]
for p in pathlib.Path('/proc').glob('[0-9]*/exe'):
 try:
  value=str(p.readlink())
  if value.startswith(str(experiment)+'/'):active.append((str(p),value))
 except (FileNotFoundError,PermissionError,ProcessLookupError):pass
assert not active,active
print('EXPERIMENT_EXECUTABLES_RUNNING=0')
