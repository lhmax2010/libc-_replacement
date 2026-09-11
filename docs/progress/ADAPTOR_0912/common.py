#!/usr/bin/env python3
import pathlib,subprocess,sys,json,hashlib,shutil,datetime
ROOT=pathlib.Path(__file__).resolve().parents[3]
EXT=pathlib.Path('/home/toolchain/development/libc++_replacement')
def record(phase,tag,cmd):
 p=ROOT/'docs/progress/ADAPTOR_0912'/phase/'raw'/tag
 rc=subprocess.run([sys.executable,str(ROOT/'docs/progress/API_0911/record.py'),str(p),'nice','-n','15','ionice','-c','3',*map(str,cmd)]).returncode
 return rc,pathlib.Path(str(p)+'.stdout').read_text(errors='replace')
def seal(phase):
 p=ROOT/'docs/progress/ADAPTOR_0912'/phase
 snap=p/'snapshots';snap.mkdir(exist_ok=True)
 for f in [*p.glob('*.py'),*p.glob('*.cpp'),*p.glob('*.h'),ROOT/'docs/progress/ADAPTOR_0912/common.py',ROOT/'docs/progress/API_0911/record.py',ROOT/'tools/resource_gate.sh']:
  shutil.copyfile(f,snap/f.name)
 files=[f for f in p.rglob('*') if f.is_file() and f.name!='SHA256SUMS' and 'delivery' not in f.parts and '__pycache__' not in f.parts and not f.name.startswith('seal_run')]
 (p/'SHA256SUMS').write_text(''.join(hashlib.sha256(f.read_bytes()).hexdigest()+'  '+str(f.relative_to(p))+'\n' for f in sorted(files)))
 print('SEALED',phase,len(files),datetime.datetime.now(datetime.timezone.utc).isoformat())
if __name__=='__main__':seal(sys.argv[1])
