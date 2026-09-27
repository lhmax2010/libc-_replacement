from common import *
import csv,gzip,xml.etree.ElementTree as E
gate()
inputs=ROOT/'docs/progress/RUNTIME_PHASE_SUMMARY_0921'
edges=list(csv.DictReader((inputs/'EDGES.tsv').open(),delimiter='\t'))
plans=list(csv.DictReader((inputs/'NEXT_STAGE.tsv').open(),delimiter='\t'))
assert len(edges)==len(plans)==23
for name in ('EDGES.tsv','NEXT_STAGE.tsv'):
 (OUT/name).write_bytes((inputs/name).read_bytes())
save('INPUTS.json',{name:dict(path=str((inputs/name).relative_to(ROOT)),sha256=sha(inputs/name)) for name in ('EDGES.tsv','NEXT_STAGE.tsv')})
catalog=[];snaps=[]
for repo in ('Tizen-Unified-Toolchain','Tizen-Base-Toolchain'):
 base='https://download.tizen.org/snapshots/TIZEN/Tizen/'+repo+'/'
 b=TMP/(repo+'.build.xml')
 run('snapshot_'+repo,['curl','-fLsS','--max-time','60','-o',b,base+'reference/build.xml'])
 sid=E.parse(b).getroot().findtext('id');url=base+sid+'/repos/standard/packages/'
 r=TMP/(repo+'.repomd.xml')
 run('repomd_'+repo,['curl','-fLsS','--max-time','60','-o',r,url+'repodata/repomd.xml'])
 ns={'r':'http://linux.duke.edu/metadata/repo'}
 datum=E.parse(r).getroot().find("r:data[@type='primary']",ns)
 loc=datum.find('r:location',ns).attrib['href'];check=datum.find('r:checksum',ns)
 compressed=TMP/(repo+'.primary.gz')
 run('primary_'+repo,['curl','-fLsS','--max-time','120','-o',compressed,url+loc])
 assert hashlib.new(check.attrib['type'],compressed.read_bytes()).hexdigest()==check.text
 n={'c':'http://linux.duke.edu/metadata/common','rpm':'http://linux.duke.edu/metadata/rpm'}
 primary=E.fromstring(gzip.decompress(compressed.read_bytes()))
 for p in primary:
  arch=p.findtext('c:arch',namespaces=n)
  if arch not in ('x86_64','noarch'):continue
  v=p.find('c:version',n).attrib;fmt=p.find('c:format',n);src=fmt.findtext('rpm:sourcerpm',namespaces=n)
  row=dict(name=p.findtext('c:name',namespaces=n),arch=arch,version=v,source_rpm=src,source_package=src.rsplit('-',2)[0],url=url+p.find('c:location',n).attrib['href'],checksum=p.findtext('c:checksum',namespaces=n),checksum_type=p.find('c:checksum',n).attrib['type'],repo=repo,snapshot=sid,files=[x.text for x in fmt.findall('c:file',n)])
  for kind in ('provides','requires'):
   el=fmt.find('rpm:'+kind,n);row[kind]=[dict(x.attrib) for x in el] if el is not None else []
  catalog.append(row)
 snaps.append(dict(repo=repo,snapshot=sid,base=url,reference_build_url=base+'reference/build.xml',build_sha256=sha(b),repomd_sha256=sha(r),primary_sha256=sha(compressed)))
 for p in (b,r): (OUT/p.name).write_bytes(p.read_bytes())
save('SNAPSHOTS.json',snaps)
(TMP/'catalog.json').write_text(json.dumps(catalog))
providers=set(x['provider'] for x in edges)
selected=[p for p in catalog if p['source_package'] in providers and not any(t in p['name'] for t in ('debuginfo','debugsource','-doc','-tests','-test','-examples','-static'))]
save('PROVIDER_CANDIDATES.json',selected)
print(json.dumps({k:[(p['name'],p['version']) for p in selected if p['source_package']==k] for k in sorted(providers)},indent=2))
