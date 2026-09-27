"""Pin Unified metadata and runtime edge inputs; retain read-only command evidence."""
import csv,gzip,hashlib,json,shlex,subprocess,xml.etree.ElementTree as X
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/UNIFIED_NIGHT_0927';T=P/'tmp/UNIFIED_NIGHT_0927';T.mkdir(exist_ok=True)
commands=[]
def run(argv):
 r=subprocess.run(argv,capture_output=True,timeout=180)
 commands.append(dict(argv=argv,command=shlex.join(argv),exitcode=r.returncode,stdout=r.stdout.decode(errors='replace'),stderr=r.stderr.decode(errors='replace')))
 (E/'metadata_commands.json').write_text(json.dumps(commands,indent=2)+'\n')
 if r.returncode:raise RuntimeError('command failed: '+shlex.join(argv))
 return r.stdout
ref='72c0ad91858db03f84a653d029767efebde9dfa7'
for name in ['EDGES.tsv','NEXT_STAGE.tsv']:
 data=run(['git','show',ref+':docs/progress/RUNTIME_PHASE_SUMMARY_0921/'+name]);(E/name).write_bytes(data)
edges=list(csv.DictReader((E/'EDGES.tsv').open(),delimiter='\t'));providers=sorted({e['provider'] for e in edges})
assert len(edges)==23 and len(providers)==14
unified=sorted(set(providers)-{'abseil-cpp','jsoncpp'});assert len(unified)==12
snapshot=X.parse(E/'unified-build.xml').getroot().findtext('id')
base='https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Unified-Toolchain/'+snapshot+'/repos/standard/'
ns={'r':'http://linux.duke.edu/metadata/repo','c':'http://linux.duke.edu/metadata/common','rpm':'http://linux.duke.edu/metadata/rpm'}
ids=[];indexes={}
for typ in ['packages','source']:
 md=E/(typ+'-repomd.xml')
 run(['curl','-fLsS','--max-time','120','-o',str(md),base+typ+'/repodata/repomd.xml'])
 entry=X.parse(md).getroot().find('r:data[@type="primary"]',ns);check=entry.find('r:checksum',ns);href=entry.find('r:location',ns).get('href')
 primary=E/(typ+'-primary.xml.gz');run(['curl','-fLsS','--max-time','120','-o',str(primary),base+typ+'/'+href])
 data=primary.read_bytes();assert hashlib.new(check.get('type'),data).hexdigest()==check.text
 ids.append(dict(snapshot=snapshot,url=base+typ+'/'+href,sha256=hashlib.sha256(data).hexdigest(),metadata_verified=True))
 index=[]
 for x in X.fromstring(gzip.decompress(data)):
  name=x.findtext('c:name',namespaces=ns);loc=x.find('c:location',ns);v=x.find('c:version',ns);fmt=x.find('c:format',ns)
  def entries(tag):
   parent=fmt.find('rpm:'+tag,ns)
   return [z.attrib for z in parent] if parent is not None else []
  index.append(dict(name=name,arch=x.findtext('c:arch',namespaces=ns),version=v.attrib,source_rpm=x.findtext('c:format/rpm:sourcerpm',namespaces=ns),url=base+typ+'/'+loc.get('href'),checksum=x.findtext('c:checksum',namespaces=ns),checksum_type=x.find('c:checksum',ns).get('type'),bytes=int(x.find('c:size',ns).get('package')),provides=entries('provides'),requires=entries('requires'),header_range=fmt.find('rpm:header-range',ns).attrib if fmt.find('rpm:header-range',ns) is not None else {}))
 indexes[typ]=index;(T/(typ+'-index.json')).write_text(json.dumps(index,indent=2)+'\n')
selected=[x for x in indexes['source'] if x['name'] in unified]
(E/'PROVIDER_SOURCE_METADATA.json').write_text(json.dumps(selected,indent=2)+'\n')
(E/'INPUT_IDENTITIES.json').write_text(json.dumps(dict(runtime_ref=ref,edges_sha256=hashlib.sha256((E/'EDGES.tsv').read_bytes()).hexdigest(),next_sha256=hashlib.sha256((E/'NEXT_STAGE.tsv').read_bytes()).hexdigest(),snapshot=snapshot,metadata=ids,providers=unified),indent=2)+'\n')
print('23 edges; 14 providers; 12 Unified:',','.join(unified))
for x in selected:print(x['name'],x['bytes'],x['url'],x['header_range'])
