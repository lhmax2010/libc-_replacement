"""Extract executed recipe paths, retaining unresolved paths and source-line evidence.

Never resolve a guest path by matching its basename to an unrelated host file.
GBS 'processing recipe' records give the exported spec directly.
"""
import collections,datetime,hashlib,json,os,re,shlex
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923/spec-audit_R3';records=[];skipped=[]
wrappers={'env','nice','ionice','timeout','time','exec','sudo','strace','bwrap','systemd-run','qemu-arm','qemu-arm-static','qemu-aarch64','qemu-aarch64-static'}
def commands(tokens,cwd=None):
 if not tokens:return []
 first=Path(tokens[0]).name
 if first in ('bash','sh','dash'):
  for opt in ('-c','-lc'):
   if opt in tokens:
    i=tokens.index(opt)
    return shell(tokens[i+1],cwd) if i+1<len(tokens) else []
 if first not in wrappers|{'rpmbuild','gbs'} and not re.match(r'^[A-Za-z_][A-Za-z_0-9]*=',tokens[0]):return []
 for i,x in enumerate(tokens):
  if Path(x).name=='rpmbuild':
   tail=tokens[i+1:]
   if not any(re.match(r'^-(?:b|t)[abcipsfl]|^--(?:rebuild|recompile)',v) for v in tail):return []
   defs={};binds=[];guest_cwd=None
   for j,v in enumerate(tokens):
    if v in ('--bind','--ro-bind') and j+2<len(tokens):binds.append((tokens[j+2],tokens[j+1]))
    if v in ('--define','--with') and j+1<len(tokens):
     pair=tokens[j+1].split(None,1)
     if len(pair)==2:defs[pair[0]]=pair[1]
    if v=='--chdir' and j+1<len(tokens):guest_cwd=tokens[j+1]
   if '_specdir' not in defs and '_topdir' in defs:defs['_specdir']=defs['_topdir']+'/SPECS'
   out=[]
   for spec in tail:
    if not spec.endswith('.spec') or '\n' in spec or spec.startswith('-'):continue
    s=spec
    for _ in range(4):
     s=re.sub(r'%\{([^}]+)\}',lambda m:defs.get(m.group(1),m.group(0)),s)
    if not s.startswith('/') and guest_cwd:s=guest_cwd.rstrip('/')+'/'+s
    resolved=None;method='UNRESOLVED'
    if not any(x in s for x in ('$','%{')):
     applicable=[(dst,src) for dst,src in binds if s==dst or s.startswith(dst.rstrip('/')+'/')]
     if applicable:
      dst,src=max(applicable,key=lambda p:len(p[0]));resolved=src.rstrip('/')+'/'+s[len(dst):].lstrip('/');method='bwrap binding'
     elif not binds and s.startswith(str(P)+'/'):resolved=s;method='explicit host path'
     elif not binds and cwd:resolved=str(Path(cwd)/s);method='recorded cwd'
    out.append(dict(tool='rpmbuild',spec_argument=spec,path=os.path.normpath(resolved) if resolved else None,resolution=method,defines=defs,argv=tokens))
   return out
  if Path(x).name=='gbs' and 'build' in tokens[i+1:]:
   spec=None;pack='packaging'
   for j,v in enumerate(tokens):
    if v=='--spec' and j+1<len(tokens):spec=tokens[j+1]
    if v.startswith('--spec='):spec=v.split('=',1)[1]
    if v=='--packaging-dir' and j+1<len(tokens):pack=tokens[j+1]
   if not spec:return []
   resolved=str(Path(cwd)/pack/spec) if cwd and not Path(spec).is_absolute() else (spec if Path(spec).is_absolute() else None)
   return [dict(tool='gbs',spec_argument=spec,path=resolved,resolution='recorded cwd' if resolved else 'UNRESOLVED: GBS cwd absent; use exported processing-recipe log if available',defines={},argv=tokens)]
 return []
def shell(text,cwd=None):
 out=[]
 try:lex=shlex.shlex(text,posix=True,punctuation_chars=';&|');lex.whitespace_split=True;tokens=list(lex)
 except ValueError:return []
 segments=[];part=[]
 for tok in tokens:
  if tok and all(c in ';&|' for c in tok):segments.append(part);part=[]
  else:part.append(tok)
 segments.append(part)
 for part in segments:
  if len(part)==2 and part[0]=='cd' and part[1].startswith('/'):cwd=part[1];continue
  out.extend(commands(part,cwd))
 return out
def objects(obj,cwd=None):
 if isinstance(obj,dict):
  cwd=obj.get('cwd',obj.get('workdir',cwd))
  for k,v in obj.items():
   if k in ('argv','command','cmd'):
    if isinstance(v,list) and all(isinstance(x,str) for x in v):yield from commands(v,cwd)
    elif isinstance(v,str):yield from shell(v,cwd)
   elif isinstance(v,(dict,list)):yield from objects(v,cwd)
 elif isinstance(obj,list):
  if obj and all(isinstance(x,str) for x in obj):yield from commands(obj,cwd)
  else:
   for v in obj:yield from objects(v,cwd)
def datestamp(path):
 for ending in ('.argv.json','.command.txt','.stdout.txt','.stdout'):
  if str(path).endswith(ending):
   f=Path(str(path)[:-len(ending)]+'.started.txt')
   if f.is_file():return f.read_text().strip()
 if '.stdout' in path.name or path.name.endswith('build.log'):
  with path.open(errors='replace') as f:header=f.read(5000)
  m=re.search(r'(?m)^(2026-\d{2}-\d{2}[ T]\d{2}:\d{2}(?::\d{2})?(?:\s*[+-]\d{4})?)\s*$',header)
  if m:return m.group(1)
 return None
tests=[
 (['rpmbuild','-bb','/tmp/test.spec'],None),
 (['bwrap','--ro-bind','/HOST','/','--bind','/TASK','/tmp/task','--','rpmbuild','--define','_topdir /tmp/task/rpm','-bp','%{_specdir}/a.spec'],'/TASK/rpm/SPECS/a.spec'),
 (['bwrap','--ro-bind','/HOST','/','--','rpmbuild','-bb','/home/abuild/a.spec'],'/HOST/home/abuild/a.spec'),
 (['rg','rpmbuild','/tmp/x.spec'],False),
 (['cat','rpmbuild','/tmp/x.spec'],False),
 (['rpmspec','-P','/tmp/x.spec'],False),
 (['printf','rpmbuild -bb /tmp/x.spec'],False)]
cal=[]
for argv,expected in tests:
 got=commands(argv);passed=(not got) if expected is False else bool(got) and got[0]['path']==expected
 cal.append(dict(argv=argv,expected=expected,actual=got,pass_test=passed));assert passed
(E/'extractor_calibration_R3.json').write_text(json.dumps(cal,indent=2)+'\n')
source_meta=[]
for rel in json.loads((E/'discovered_files_R3.json').read_text()):
 f=P/rel;data=f.read_bytes();text=data.decode(errors='replace');hits=[];round_name=rel.split('/')[2]
 source_meta.append(dict(path=rel,sha256=hashlib.sha256(data).hexdigest(),bytes=len(data)))
 if rel.endswith('.json'):
  try:hits=list(objects(json.loads(text)))
  except ValueError:skipped.append(dict(path=rel,reason='invalid JSON'))
  for h in hits:h.update(evidence=rel,line=None,kind='recorded argv',timestamp=datestamp(f))
 elif rel.endswith('.command.txt'):
  hits=shell(text)
  for h in hits:h.update(evidence=rel,line=1,kind='logged command',timestamp=datestamp(f))
 else:
  # Exclude output of clearly read-only inspection commands: quoted logs are not new execution.
  companion=Path(re.sub(r'\.stdout(?:\.txt)?$','.command.txt',str(f)))
  if re.search(r'\.stdout(?:\.txt)?$',rel) and companion.is_file():
   ct=companion.read_text(errors='replace').strip()
   if re.match(r'^(?:cat|rg|grep|sed|head|tail|nl|git|sha256sum|ls)\b',ct):continue
   if re.match(r'^(?:bash|sh)\s',ct) and not shell(ct):continue
  recipe_lines=[]
  for ln,line in enumerate(text.splitlines(),1):
   m=re.search(r'processing recipe\s+(\S+\.spec)\s',line)
   if m:
    h=dict(tool='gbs/rpmbuild',spec_argument=m.group(1),path=m.group(1) if m.group(1).startswith(str(P)+'/') else None,resolution='GBS processing recipe log',defines={},evidence=rel,line=ln,kind='executed exported recipe',timestamp=datestamp(f));hits.append(h);recipe_lines.append(h)
   if 'rpmbuild ' not in line and not ('gbs ' in line and ' build ' in line):continue
   # Only shell trace or explicitly logged command; do not parse prose/source excerpts.
   stripped=re.sub(r'^\[\s*\d+(?:\.\d+)?s\]\s*','',line).lstrip()
   if stripped.startswith('+ '):stripped=stripped[2:]
   elif not re.match(r'^(?:rpmbuild|gbs|nice|ionice|bwrap|strace|exec|env)\s',stripped):continue
   for h in shell(stripped):
    if not h['path'] and recipe_lines and Path(h['spec_argument']).name==Path(recipe_lines[-1]['spec_argument']).name:
     h.update(path=recipe_lines[-1]['path'],resolution='same execution log processing-recipe path')
    h.update(evidence=rel,line=ln,kind='executed command line',timestamp=datestamp(f));hits.append(h)
 for h in hits:h['round']=round_name;records.append(h)
# A copied argv comparison is not an execution of the comparison round.
for r in records:
 r['execution_supported']=True
 r['execution_limit']='Command recorded, not proof of build success; exported recipe logs are stronger evidence.'
 if '/comparison/' in r['evidence'] or 'original-argv' in Path(r['evidence']).name and '/cell_commands/' not in r['evidence']:r['execution_supported']=False
(E/'occurrences_R3.json').write_text(json.dumps(records,ensure_ascii=False,indent=2)+'\n')
(E/'scan_inputs_sha256_R3.json').write_text(json.dumps(source_meta,indent=2)+'\n')
(E/'parse_notes_R3.json').write_text(json.dumps(skipped,indent=2)+'\n')
paths={r['path'] for r in records if r['path'] and r['execution_supported']}
summary=dict(records=len(records),resolved_distinct_paths=len(paths),existing_paths=sum(Path(p).is_file() for p in paths),unresolved=sum(not r['path'] for r in records),rounds=sorted({r['round'] for r in records}))
(E/'extraction_summary_R3.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
