"""Verify small public static RPM payloads; no package installation or build."""
import hashlib
from html.parser import HTMLParser
import json
from pathlib import Path
import re
import shlex
import subprocess

p=Path('progress/BUILD_NIGHT_0916')
downloads=Path('tmp/BUILD_NIGHT_0916.h4taGY/dependency-rpms')
downloads.mkdir(exist_ok=True)
base='https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/reference/repos/standard/packages/'
log=(p/'repository_commands.jsonl').open('w',buffering=1)
class Links(HTMLParser):
 def __init__(self):super().__init__();self.links=[]
 def handle_starttag(self,tag,attrs):
  if tag=='a':
   href=dict(attrs).get('href','')
   if href.endswith('.rpm'):self.links.append(href)
def run(args,required=True):
 r=subprocess.run(args,capture_output=True,text=True,errors='replace')
 log.write(json.dumps(dict(command=shlex.join(args),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))+'\n')
 if required:assert r.returncode==0,(args,r.returncode)
 return r
records=[];indices=[]
for arch in ['x86_64','aarch64','armv7l']:
 path=p/f'reference_{arch}_index.html'
 if arch!='aarch64':
  run(['curl','-q','--fail','--location','--silent','--show-error','--max-time','60','--output',str(path),base+arch+'/'])
 parser=Links();parser.feed(path.read_text())
 links=sorted(set(parser.links))
 indices.append(dict(arch=arch,url=base+arch+'/',sha256=hashlib.sha256(path.read_bytes()).hexdigest(),rpm_entries=len(links)))
 for package in ['libbpf-static','liblzma-static','zlib-devel-static','libxml2-devel','libffi-devel-static']:
  names=[x for x in links if re.match(re.escape(package)+r'-[0-9]',x)]
  if not names:
   records.append(dict(arch=arch,package=package,status='NOT_AVAILABLE_IN_CURRENT_INDEX',tried=base+arch+'/'));continue
  assert len(names)==1,(arch,package,names)
  name=names[0];target=downloads/name
  run(['curl','-q','--fail','--location','--silent','--show-error','--max-time','120','--output',str(target),base+arch+'/'+name])
  run(['rpm','--checksig','--nosignature',str(target)])
  meta=run(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\t%{VCS}\n',str(target)]).stdout.strip()
  files=run(['rpm','-qpl',str(target)]).stdout.splitlines()
  row=dict(arch=arch,package=package,url=base+arch+'/'+name,path=str(target),
           bytes=target.stat().st_size,sha256=hashlib.sha256(target.read_bytes()).hexdigest(),
           rpm=meta,archives=[x for x in files if '.a'==Path(x).suffix],files=files,
           status='RPM_PAYLOAD_LIST_VERIFIED')
  records.append(row)
  print(arch,package,row['archives'],flush=True)
(p/'repository_static_packages.json').write_text(json.dumps(dict(indices=indices,records=records,
scope='Current rolling reference package indices, queried in this run; not a pinned snapshot or the old local RPM set. RPM digests checked, signatures not authenticated.'),indent=2)+'\n')
