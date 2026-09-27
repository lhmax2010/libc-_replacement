import pathlib,subprocess,xml.etree.ElementTree as X,json,gzip,hashlib,csv
E=pathlib.Path(__file__).resolve().parent;commands=[]
base='https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260914.073422/repos/standard/packages/'
ns={'r':'http://linux.duke.edu/metadata/repo','c':'http://linux.duke.edu/metadata/common','rpm':'http://linux.duke.edu/metadata/rpm'}
def get(url,path):
    argv=['curl','-fLsS','--max-time','120','-o',str(path),url];p=subprocess.run(argv,capture_output=True,text=True)
    commands.append({'argv':argv,'exitcode':p.returncode,'stdout':p.stdout,'stderr':p.stderr});(E/'base_metadata_commands.json').write_text(json.dumps(commands,indent=2)+'\n');assert p.returncode==0
get(base+'repodata/repomd.xml',E/'base-repomd.xml')
ent=X.parse(E/'base-repomd.xml').getroot().find('r:data[@type="primary"]',ns);url=base+ent.find('r:location',ns).get('href');get(url,E/'base-primary.xml.gz')
data=(E/'base-primary.xml.gz').read_bytes();h=ent.find('r:checksum',ns);assert hashlib.new(h.get('type'),data).hexdigest()==h.text
provides={}
for x in X.fromstring(gzip.decompress(data)):
    ps=x.find('c:format/rpm:provides',ns)
    if ps is None:continue
    for p in ps:provides.setdefault(p.get('name'),set()).add((x.findtext('c:name',namespaces=ns),x.findtext('c:arch',namespaces=ns),x.findtext('c:format/rpm:sourcerpm',namespaces=ns)))
scope=json.loads((E/'SCOPE_RESULT.json').read_text());rows=[]
for r in scope['unresolved_BuildRequires']:
    for v in sorted(provides.get(r,{('NOT_AVAILABLE','NOT_AVAILABLE','NOT_AVAILABLE')})):rows.append([r,*v])
with (E/'BASE_BUILDREQUIRES.tsv').open('w') as f:w=csv.writer(f,delimiter='\t');w.writerow(['BuildRequires','Base_binary','arch','SOURCERPM']);w.writerows(rows)
(E/'BASE_METADATA_IDENTITY.json').write_text(json.dumps({'snapshot':'tizen-base-toolchain_20260914.073422','url':url,'sha256':hashlib.sha256(data).hexdigest(),'verified_repomd_checksum':True},indent=2)+'\n')
print('Base metadata verified; unresolved in both:',[r for r in scope['unresolved_BuildRequires'] if r not in provides])
