"""Compare every resolved recorded spec to a read-only reference; ignore only VCS/blank lines."""
import collections,csv,difflib,hashlib,json,os,re,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923/spec-audit_R3';D=E/'diffs';D.mkdir(exist_ok=True);S=E/'snapshots';S.mkdir(exist_ok=True)
records=json.loads((E/'occurrences_R3.json').read_text());commands=[];refs={}
def run(argv):
 r=subprocess.run(argv,capture_output=True,timeout=90,env={**os.environ,'GIT_SSH_COMMAND':'ssh -o BatchMode=yes -o ConnectTimeout=20','GIT_TERMINAL_PROMPT':'0'})
 commands.append(dict(argv=argv,command=shlex.join(argv),exitcode=r.returncode,stdout=r.stdout.decode(errors='replace'),stderr=r.stderr.decode(errors='replace')));(E/'comparison_commands_R3.json').write_text(json.dumps(commands,indent=2)+'\n');assert r.returncode==0;return r.stdout
def norm(s):return [x for x in s.splitlines(True) if x.strip() and not re.match(r'^\s*VCS\s*:',x)]
for row in csv.DictReader((P/'docs/progress/QB_PRECHECK_0922/REMOTE_BRANCHES.tsv').open(),delimiter='\t'):
 name=row['package'];remote=run(['git','ls-remote','--exit-code',row['url'],'refs/heads/'+row['branch']]).decode().split()[0]
 assert remote==row['remote'],('STOP: remote moved',name,remote)
 repo=P/'codes/R104'/name
 if name=='llvm':repo=P/'tmp/BUILD_W1BC_0911/target-fetch'
 if name=='bpftrace':repo=P/'tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push'
 data=run(['git','-C',str(repo),'show',remote+':packaging/'+name+'.spec'])
 refs[name]=dict(data=data,reference=str(repo.relative_to(P))+':'+remote+':packaging/'+name+'.spec',head=remote,branch=row['branch'])
 if name=='llvm':
  refs['libcxx-runtimes']=dict(data=run(['git','-C',str(repo),'show',remote+':packaging/libcxx-runtimes.spec']),reference=str(repo.relative_to(P))+':'+remote+':packaging/libcxx-runtimes.spec',head=remote,branch=row['branch'])
 print('remote',name,remote,flush=True)
authorized={name:norm((P/path).read_text()) for name,path in [('llvm','tmp/WEEKEND_0918/specs/llvm.spec'),('libcxx-runtimes','tmp/NIGHT_0917/specs/libcxx-runtimes.spec')]}
groups=collections.defaultdict(list)
for r in records:
 if r['path'] and r['execution_supported']:groups[r['path']].append(r)
results=[]
for idx,(path,events) in enumerate(sorted(groups.items()),1):
 ident=f'S{idx:03d}';p=Path(path);rounds=sorted({r['round'] for r in events});existing=p.is_file()
 dates=sorted((r['timestamp'],r['round']) for r in events if r['timestamp'])
 if len(rounds)==1:first=last=rounds[0]
 elif dates and {r[1] for r in dates}==set(rounds):first=dates[0][1];last=dates[-1][1]
 else:first=last='NOT_OBSERVED (all rounds listed separately)'
 row=dict(id=ident,path=str(p.relative_to(P)) if p.is_relative_to(P) else path,rounds=rounds,first_round=first,last_round=last,evidence=[dict(file=r['evidence'],line=r['line'],kind=r['kind'],timestamp=r['timestamp']) for r in events],scope='Current retained file hash, not proof that a reused path never changed historically')
 if not existing:row.update(package=p.stem,sha256='NOT_AVAILABLE',classification='NOT_AVAILABLE',reason='Recorded path no longer available');results.append(row);continue
 data=p.read_bytes();text=data.decode(errors='replace');m=re.search(r'^Name:\s*(\S+)',text,re.M|re.I);name=m.group(1) if m else p.stem
 row.update(package=name,sha256=hashlib.sha256(data).hexdigest())
 ref=refs.get(name);original=False
 if ref is None:
  candidates=[P/'codes'/name/'packaging'/p.name,P/'codes/R104'/name/'packaging'/p.name]
  q=next((q for q in candidates if q.is_file()),None)
  if q:
   original=True;ref=dict(data=q.read_bytes(),reference=str(q.relative_to(P)),head='NOT_OBSERVED',branch='original codes spec')
 if ref is None:
  row.update(classification='NOT_AVAILABLE',reason='No uniquely identified original spec; no basename substitution');results.append(row);continue
 a=ref['data'].decode(errors='replace');b=text
 (S/(ident+'-reference.spec')).write_bytes(ref['data']);(S/(ident+'-used.spec')).write_bytes(data)
 raw=''.join(difflib.unified_diff(a.splitlines(True),b.splitlines(True),fromfile=ref['reference'],tofile=row['path']))
 delta=''.join(difflib.unified_diff(norm(a),norm(b),fromfile=ref['reference']+' (VCS/blank removed)',tofile=row['path']+' (VCS/blank removed)'))
 (D/(ident+'-raw.diff')).write_text(raw);(D/(ident+'-functional.diff')).write_text(delta)
 match=norm(a)==norm(b)
 row.update(reference=ref['reference'],reference_sha256=hashlib.sha256(ref['data']).hexdigest(),remote_head=ref['head'],classification=('与原仓一致（未改过）' if original else '与 sandbox 一致') if match else '有功能差异未推送',authorized_current_change=not match and name in authorized and norm(b)==authorized[name],diff=str((D/(ident+'-functional.diff')).relative_to(P)),raw_diff=str((D/(ident+'-raw.diff')).relative_to(P)))
 if not match:row['qualification']='This label means different from the requested current reference. A historical baseline/obsolete trial is not thereby proven to be a new change still needed by QuickBuild.'
 results.append(row)
(E/'LOCAL_SPECS_R3.json').write_text(json.dumps(results,ensure_ascii=False,indent=2)+'\n')
fields=['id','package','path','sha256','first_round','last_round','rounds','classification','authorized_current_change','reference','diff']
with (E/'LOCAL_SPECS_R3.tsv').open('w',newline='') as f:
 w=csv.DictWriter(f,fields,delimiter='\t',lineterminator='\n',extrasaction='ignore');w.writeheader();w.writerows({**r,'rounds':','.join(r['rounds'])} for r in results)
md=['# 实际命令记录中的本地 spec 清单','', '仅忽略 VCS 字段和纯空行；SHA 是当前保留文件身份。完整执行证据见同名 JSON。未确定时间顺序的保留 NOT_OBSERVED，不按文件修改时间猜测。','', '| ID | 包 | 路径 | SHA256 | 最早轮次 | 最近轮次 | 结论 |','| --- | --- | --- | --- | --- | --- | --- |']
for r in results:md.append('| '+' | '.join(str(r.get(k,'')) for k in ['id','package','path','sha256','first_round','last_round','classification'])+' |')
(E/'LOCAL_SPECS_R3.md').write_text('\n'.join(md)+'\n')
extra=[r for r in results if r['classification']=='有功能差异未推送' and not r['authorized_current_change']]
summary=dict(paths=len(results),classifications=dict(collections.Counter(r['classification'] for r in results)),authorized_different=sum(r.get('authorized_current_change',False) for r in results),other_different_ids=[r['id'] for r in extra],status='STOP_FOR_REVIEW' if extra else 'NO_EXTRA_FUNCTIONAL_DIFF_OBSERVED')
(E/'COMPARISON_SUMMARY_R3.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2)+'\n')
(E/'EXTRA_DIFFERENCES_R3.diff').write_text('\n'.join('## '+r['id']+' '+r['path']+'\n'+(P/r['diff']).read_text() for r in extra))
print(json.dumps(summary,ensure_ascii=False,indent=2))
