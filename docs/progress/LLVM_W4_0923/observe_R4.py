"""Read-only remote checks and public repository metadata collection."""
import csv, gzip, hashlib, json, shlex, subprocess, xml.etree.ElementTree as ET
from pathlib import Path

P=Path.cwd(); E=P/'docs/progress/LLVM_W4_0923'; commands=[]
def run(argv):
    r=subprocess.run(argv,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    commands.append(dict(argv=argv,command=shlex.join(argv),exitcode=r.returncode,stdout=r.stdout.decode(errors='replace'),stderr=r.stderr.decode(errors='replace')))
    (E/'commands_R4.json').write_text(json.dumps(commands,ensure_ascii=False,indent=2)+'\n')
    return r

build=ET.parse(E/'build_R4.xml').getroot().findtext('id')
base='https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/'+build+'/'
ns={'r':'http://linux.duke.edu/metadata/repo','c':'http://linux.duke.edu/metadata/common','rpm':'http://linux.duke.edu/metadata/rpm'}
identities=[]; selected=[]
for typ in ['packages','source']:
    repo=base+'repos/standard/'+typ+'/'
    repomd=E/(typ+'-repomd_R4.xml')
    r=run(['curl','-fLsS','--max-time','60','-w','%{http_code} %{url_effective} %{size_download}\n','-o',str(repomd),repo+'repodata/repomd.xml'])
    assert r.returncode==0
    entry=ET.parse(repomd).getroot().find('r:data[@type="primary"]',ns)
    href=entry.find('r:location',ns).get('href'); checksum=entry.find('r:checksum',ns)
    primary=E/(typ+'-primary_R4.xml.gz')
    r=run(['curl','-fLsS','--max-time','60','-w','%{http_code} %{url_effective} %{size_download}\n','-o',str(primary),repo+href])
    assert r.returncode==0
    data=primary.read_bytes(); actual=hashlib.new(checksum.get('type'),data).hexdigest()
    assert actual==checksum.text
    identities.append(dict(snapshot=build,repomd_url=repo+'repodata/repomd.xml',primary_url=repo+href,path=str(primary.relative_to(P)),sha256=hashlib.sha256(data).hexdigest(),metadata_checksum=actual,checksum_type=checksum.get('type')))
    root=ET.fromstring(gzip.decompress(data))
    for pkg in root.findall('c:package',ns):
        name=pkg.findtext('c:name',namespaces=ns)
        if name in ['libc++','libc++-devel','libc++abi','libc++abi-devel','libcxx-runtimes','llvm','libllvm','llvm-static-devel']:
            selected.append(dict(repository=typ,name=name,arch=pkg.findtext('c:arch',namespaces=ns),version=pkg.find('c:version',ns).attrib,sourcerpm=pkg.findtext('c:format/rpm:sourcerpm',namespaces=ns),location=repo+pkg.find('c:location',ns).get('href'),checksum=pkg.findtext('c:checksum',namespaces=ns),xml=ET.tostring(pkg,encoding='unicode')))
(E/'METADATA_IDENTITIES_R4.json').write_text(json.dumps(identities,indent=2)+'\n')
(E/'RUNTIME_METADATA_R4.json').write_text(json.dumps(selected,indent=2)+'\n')
print('snapshot',build)
for p in selected: print(p['repository'],p['name'],p['arch'],p['sourcerpm'])

rows=list(csv.DictReader((P/'docs/progress/QB_PRECHECK_0922/REMOTE_BRANCHES.tsv').open(),delimiter='\t'))
llvm=next(r for r in rows if r['package']=='llvm'); llvm['expected']='5c169afc97945bfb88786d9f38477f6973fe5285'
runtime=[p for p in selected if p['repository']=='packages' and p['name'] in ['libc++','libc++abi']]
assert runtime and all(p['sourcerpm'].startswith('libcxx-runtimes-') for p in runtime)
assert any(p['repository']=='source' and p['name']=='libcxx-runtimes' for p in selected)
extra=dict(llvm); extra['package']='libcxx-runtimes'; rows.append(extra)
result=[]
for row in rows:
    r=run(['git','ls-remote',row['url'],'refs/heads/'+row['branch']])
    observed=r.stdout.decode().split()
    assert r.returncode==0 and len(observed)==2
    assert observed[0]==row['expected'], 'remote changed: '+row['package']
    path=row['url'].split(':29418/',1)[1]
    result.append(dict(obs_package=row['package'],gerrit_repository=path,branch=row['branch'],remote_sha=observed[0]))
with (E/'QUICKBUILD_BRANCHES_R4.tsv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=list(result[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(result)
print('remote checks',len(result),'PASS')
