"""Authorized sequential restoration; preserve each modified file and stop on any mismatch."""
import csv,hashlib,json,re,shlex,subprocess
from pathlib import Path
P=Path.cwd();E=P/'progress/BPF_W4_0922';T=P/'tmp/BPF_W4_0922/restore';T.mkdir(parents=True,exist_ok=False);rows=[];commands=[]
def save():
 (E/'RESTORE_RECORDS.json').write_text(json.dumps(rows,indent=2));(E/'restore_commands.json').write_text(json.dumps(commands,indent=2))
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def run(args,label,cwd=None,out=None):
 if out is None:r=subprocess.run(args,capture_output=True,cwd=cwd)
 else:
  with out.open('xb') as f:r=subprocess.run(args,stdout=f,stderr=subprocess.PIPE,cwd=cwd)
 (E/(label+'.stderr.txt')).write_bytes(r.stderr)
 if out is None:(E/(label+'.stdout.txt')).write_bytes(r.stdout)
 commands.append(dict(argv=args,command=shlex.join(args),cwd=str(cwd) if cwd else str(P),exitcode=r.returncode,stdout=str(out) if out else label+'.stdout.txt',stderr=label+'.stderr.txt'));save();assert r.returncode==0,(label,r.returncode)
incident=(P/'docs/progress/BPF_W3R_0922/INCIDENT.md').read_text()
targets=re.findall(r'^\| (tmp/[^|]+?) \| ([0-9a-f]{64}) \| ([0-9a-f]{64}) \|$',incident,re.M);assert len(targets)==4
for i,(rel,expected,modified) in enumerate(targets,1):
 row=dict(order=i,path=rel,expected_original_sha256=expected,expected_modified_sha256=modified,status='STARTED');rows.append(row);save()
 try:
  dest=P/rel;backup=Path(str(dest)+'.objcopy-modified-0922');assert dest.is_file(),('current copy missing',str(dest));assert not backup.exists(),('backup already exists',str(backup))
  row['current_sha256']=sha(dest);save();assert row['current_sha256']==modified,'current file differs from recorded accident'
  roundname='BPF_W1_0921' if '/BPF_W1_0921/' in rel else 'BPF_STATIC_0922';arch='armv7l' if 'armv7l' in rel else 'aarch64'
  inv=P/'docs/progress'/roundname/'RPM_INVENTORY.tsv';table=list(csv.DictReader(inv.open(),delimiter='\t'))
  candidates=[x for x in table if x['arch']==arch and (x.get('name')=='bpftrace' or (x.get('path')=='libcxx' and Path(x['rpm']).name==f'bpftrace-0.24.2-1.{arch}.rpm'))];assert len(candidates)==1
  r=candidates[0];rpm=Path(r.get('rpm',r['path']));assert rpm.is_file(),('RPM missing',str(rpm));rh=sha(rpm);assert rh==r['sha256'],'RPM SHA mismatch'
  row.update(rpm=str(rpm),rpm_sha256=rh,inventory=str(inv),backup=str(backup));save()
  run(['mv','--no-clobber','--',str(dest),str(backup)],f'restore-{i}-rename');assert not dest.exists() and sha(backup)==modified
  new=T/str(i);new.mkdir();cpio=new/'payload.cpio';extract=new/'extract';extract.mkdir()
  run(['rpm2cpio',str(rpm)],f'restore-{i}-rpm2cpio',out=cpio)
  run(['cpio','-idmv','--no-absolute-filenames','-F',str(cpio),'./usr/bin/bpftrace','usr/bin/bpftrace'],f'restore-{i}-extract',cwd=extract)
  elf=extract/'usr/bin/bpftrace';eh=sha(elf);row.update(extracted_path=str(elf),extracted_sha256=eh,extracted_matches_original=eh==expected);save();assert eh==expected,'extracted SHA mismatch'
  assert not dest.exists();run(['cp','-a','--no-clobber','--',str(elf),str(dest)],f'restore-{i}-copy')
  row.update(restored_sha256=sha(dest),preserved_modified_sha256=sha(backup));assert row['restored_sha256']==expected and row['preserved_modified_sha256']==modified
  row['status']='RESTORED_AND_INCIDENT_PRESERVED';save();print(i,row['status'],flush=True)
 except Exception as exc:
  row['status']='STOPPED';row['error']=str(exc);save();raise
print('RESTORED_4_OF_4')
