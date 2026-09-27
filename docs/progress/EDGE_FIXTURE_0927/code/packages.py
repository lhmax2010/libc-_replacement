from common import *
import argparse,collections
ap=argparse.ArgumentParser();ap.add_argument('names',nargs='+');a=ap.parse_args()
gate();cat=json.loads((TMP/'catalog.json').read_text());byname={p['name']:p for p in cat if p['arch']=='x86_64'}
for p in cat:
 if p['arch']=='noarch':byname.setdefault(p['name'],p)
providers=collections.defaultdict(list)
for p in cat:
 for x in p['provides']:providers[x['name']].append(p['name'])
manifest=OUT/'RPMS.json';done=json.loads(manifest.read_text()) if manifest.exists() else []
seen={p['name'] for p in done};q=list(a.names);missing=[]
root=TMP/'root';root.mkdir(exist_ok=True);(TMP/'rpms').mkdir(exist_ok=True)
count=0
while q:
 name=q.pop(0)
 if name in seen:continue
 if name not in byname:missing.append(dict(name=name,reason='PACKAGE_NOT_IN_SNAPSHOT'));continue
 p=byname[name];dest=TMP/'rpms'/p['url'].rsplit('/',1)[1]
 if count%5==0:gate()
 if not dest.exists():run('download_'+name,['curl','-fLsS','--max-time','180','--retry','1','-o',dest,p['url']],timeout=210)
 assert hashlib.new(p['checksum_type'],dest.read_bytes()).hexdigest()==p['checksum'],name
 listing=run('rpm_files_'+name,['rpm','-qpl',dest]);meta=run('rpm_identity_'+name,['rpm','-qp','--qf','%{NAME}\t%{VERSION}\t%{RELEASE}\t%{ARCH}\t%{SOURCERPM}\n',dest])
 for line in listing['stdout'].splitlines():assert '..' not in pathlib.PurePosixPath(line).parts
 # Only unpack files; never execute install scripts or modify the system RPM DB.
 run('extract_'+name,['bash','-o','pipefail','-c','rpm2cpio "$1" | cpio -idm --quiet --no-absolute-filenames','extract',dest],cwd=root,timeout=120)
 rec={**p,'sha256':sha(dest),'rpm_local':str(dest.relative_to(ROOT)),'identity':meta['stdout'].strip(),'file_list_record':listing['record']}
 done.append(rec);save('RPMS.json',done);seen.add(name);count+=1
 for r in p['requires']:
  dep=r['name']
  if '.so' not in dep or not dep.startswith('lib'):continue
  options=sorted(set(providers.get(dep,[])))
  if len(options)==1:q+=options
  elif not any(x in seen or x in q for x in options):missing.append(dict(requester=name,requirement=r,candidates=options,reason='NO_UNIQUE_SONAME_PROVIDER'))
save('DEPENDENCY_GAPS_'+str(time.time_ns())+'.json',missing)
print(json.dumps(dict(downloaded=count,total=len(done),dependency_gaps=missing),ensure_ascii=False))
