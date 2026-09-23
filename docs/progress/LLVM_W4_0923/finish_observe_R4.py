"""Preserve the current snapshot result; use identified historical metadata separately."""
import csv,gzip,hashlib,json,shlex,subprocess,xml.etree.ElementTree as ET
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923';commands=[]
ns={'r':'http://linux.duke.edu/metadata/repo','c':'http://linux.duke.edu/metadata/common','rpm':'http://linux.duke.edu/metadata/rpm'}
snapshot='tizen-base-toolchain_20260828.101647'
base='https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/'+snapshot+'/repos/standard/'
identities=[];records=[]
for typ in ['packages','source']:
    p=P/('progress/R98/inputs/llvm_base_'+typ+'_primary.xml.gz')
    rep=P/('progress/R98/inputs/llvm_base_'+typ+'_repomd.xml')
    entry=ET.parse(rep).getroot().find('r:data[@type="primary"]',ns)
    check=entry.find('r:checksum',ns)
    data=p.read_bytes();assert hashlib.new(check.get('type'),data).hexdigest()==check.text
    identities.append(dict(snapshot=snapshot,path=str(p.relative_to(P)),sha256=hashlib.sha256(data).hexdigest(),repomd=str(rep.relative_to(P)),repomd_sha256=hashlib.sha256(rep.read_bytes()).hexdigest(),url=base+typ+'/'+entry.find('r:location',ns).get('href'),checksum_verified=True))
    for pkg in ET.fromstring(gzip.decompress(data)):
        name=pkg.findtext('c:name',namespaces=ns)
        if name in ['libc++','libc++-devel','libc++abi','libc++abi-devel','libcxx-runtimes']:
            records.append(dict(repository=typ,name=name,arch=pkg.findtext('c:arch',namespaces=ns),sourcerpm=pkg.findtext('c:format/rpm:sourcerpm',namespaces=ns),location=base+typ+'/'+pkg.find('c:location',ns).get('href'),xml=ET.tostring(pkg,encoding='unicode')))
(E/'HISTORICAL_METADATA_R4.json').write_text(json.dumps(dict(inputs=identities,packages=records),indent=2)+'\n')
assert len([r for r in records if r['repository']=='packages'])==12
assert all(r['sourcerpm']=='libcxx-runtimes-22.1.8-1.1.src.rpm' for r in records if r['repository']=='packages')
rows=list(csv.DictReader((P/'docs/progress/QB_PRECHECK_0922/REMOTE_BRANCHES.tsv').open(),delimiter='\t'))
llvm=next(r for r in rows if r['package']=='llvm');llvm['expected']='5c169afc97945bfb88786d9f38477f6973fe5285'
extra=dict(llvm);extra['package']='libcxx-runtimes';rows.append(extra)
result=[]
for row in rows:
    argv=['git','ls-remote',row['url'],'refs/heads/'+row['branch']]
    r=subprocess.run(argv,capture_output=True,timeout=60)
    commands.append(dict(argv=argv,command=shlex.join(argv),exitcode=r.returncode,stdout=r.stdout.decode(errors='replace'),stderr=r.stderr.decode(errors='replace')))
    (E/'branch_commands_R4.json').write_text(json.dumps(commands,indent=2)+'\n')
    tokens=r.stdout.decode().split();assert r.returncode==0 and len(tokens)==2
    assert tokens[0]==row['expected'], 'STOP: unexpected remote '+row['package']
    result.append(dict(obs_package=row['package'],gerrit_repository=row['url'].split(':29418/',1)[1],branch=row['branch'],remote_sha=tokens[0]))
    print(row['package'],tokens[0],'MATCH',flush=True)
with (E/'QUICKBUILD_BRANCHES_R4.tsv').open('w') as f:
    w=csv.DictWriter(f,list(result[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(result)
print('12 rows checked; runtime separate source-build unit based on fixed historical metadata; live OBS mechanism NOT_OBSERVED')
