"""Trace installed RPM identities without inventing a category for missing evidence."""
import csv,difflib,gzip,hashlib,json,subprocess,xml.etree.ElementTree as ET
from pathlib import Path
P=Path.cwd(); E=P/'docs/progress/LLVM_W4_0923/inputs_R2'
def sha(p):
 with p.open('rb') as f:
  h=hashlib.sha256()
  for block in iter(lambda:f.read(1024*1024),b''):h.update(block)
  return h.hexdigest()
published=json.loads((P/'docs/progress/BUILD_NIGHT_0916/repository_static_packages.json').read_text())['records']
published+=json.loads((P/'docs/progress/BUILD_WEEKEND_0918/w1/ffi-dependencies.json').read_text())
pub={r['sha256']:r for r in published}
meta=P/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/cache/92b056697f250e5b095232782601f8c6/repodata/abe32fe31df569c59946b57b9ac651af4ffe0ba3e57a661d9c06480c9bc5dbb6-primary.xml.gz'
ns={'c':'http://linux.duke.edu/metadata/common'}; index={}
for x in ET.fromstring(gzip.decompress(meta.read_bytes())).findall('c:package',ns):
 v=x.find('c:version',ns); key=(x.findtext('c:name',namespaces=ns),v.get('ver')+'-'+v.get('rel'),x.findtext('c:arch',namespaces=ns))
 index[key]=dict(checksum=x.findtext('c:checksum',namespaces=ns),algorithm=x.find('c:checksum',ns).get('type'),location=x.find('c:location',ns).get('href'))
(E/'metadata_identity_R2.json').write_text(json.dumps(dict(path=str(meta),sha256=sha(meta),packages=len(index),source='Base-Toolchain URL verified in retained .build.log; immutable local metadata, not a fresh reference lookup'),indent=2)+'\n')
records=[]; commands=[]; introduced={}
for arch in ['armv7l','aarch64']:
 manifest='docs/progress/BPF_W1_0921/installed-'+arch+'-inputs.json'
 for r in json.loads((P/manifest).read_text()):
  q=Path(r['path']); digest=sha(q); assert digest==r['sha256']
  name,version,architecture=r['identity'].split('\t'); key=(name,version,architecture)
  row=dict(root='bpf-'+arch,name=name,version=version,arch=arch,sha256=digest,sha_source='RPM file rehashed',rpm_path=str(q),category='',evidence=manifest,note='')
  if digest in pub:
   row.update(category='A',evidence=pub[digest]['url'],note='Original download manifest SHA equals retained RPM SHA')
  elif name.startswith(('libc++','libc++abi')):
   row.update(category='C',evidence='tmp/NIGHT_0917/specs/libcxx-runtimes.spec; UNPUSHED_LIBCXX_RUNTIME.diff',note='Authorized runtime recipe; 待推送后转 B (not yet pushed)')
  elif name in ['llvm','llvm-devel','llvm-static-devel','libllvm','clang','clang-devel','libomp','libomp-devel']:
   row.update(category='C',evidence='SPEC_IDENTITY.json; BASELINE_TO_VERIFIED_LLVM.diff',note='Authorized libclang recipe; 待推送后转 B (not yet pushed)')
  elif name=='bcc-tools-static':
   row.update(category='B',evidence='recipe-audit/bcc-tools/result.json; BUILD_W2_0912/raw/016; BUILD_W2C_0915/build-w2c-bcc-tools-armv7l-libcxx.build.log',note='platform/upstream/bcc sandbox/lhmax2025/libcxx-migration @5bd0654e24bb231035ea684d278f674aa95a2cd2; spec diff checked separately')
  args=['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\t%{SHA1HEADER}\n',str(q)]
  result=subprocess.run(args,capture_output=True); commands.append(dict(argv=args,exitcode=result.returncode,stdout=result.stdout.decode(),stderr=result.stderr.decode()))
  assert result.returncode==0
  row['header_sha1']=result.stdout.decode().strip().split('\t')[-1]
  records.append(row);introduced[(row['root'],*key)]=row
(E/'introduced_R2.json').write_text(json.dumps(records,indent=2)+'\n')
(E/'rpm_queries_R2.json').write_text(json.dumps(commands,indent=2)+'\n')
allrows=[]
for f in sorted(E.glob('*-rpmdb.tsv')):
 root=f.name.removesuffix('-rpmdb.tsv')
 for line in f.read_text().splitlines():
  name,version,arch,srpm,vcs,header=line.split('\t');key=(name,version,arch)
  row=dict(root=root,name=name,version=version,arch=arch,source_rpm=srpm,vcs=vcs,header_sha1=header,sha256='NOT_OBSERVED',sha_source='NOT_OBSERVED',category='',evidence=str(f.relative_to(P)),note='NOT_OBSERVED: A/B/C proof incomplete')
  if (root,*key) in introduced:
   extra=introduced[(root,*key)];assert extra['header_sha1']==header;row.update(extra)
  elif key in index and root!='llvm-aarch64':
   m=index[key]
   if m['algorithm']=='sha256':
    row.update(sha256=m['checksum'],sha_source='retained repository metadata; RPM blob not rehashed',category='A',evidence=str(meta.relative_to(P))+' :: '+m['location'],note='Exact name/version/arch in retained Base-Toolchain metadata; installed header identity separately retained')
  allrows.append(row)
fields=['root','name','version','arch','category','sha256','sha_source','header_sha1','evidence','note']
with (E/'ALL_INPUTS_R2.tsv').open('w') as out:
 w=csv.DictWriter(out,fields,delimiter='\t',extrasaction='ignore');w.writeheader();w.writerows(allrows)
(E/'ALL_INPUTS_R2.json').write_text(json.dumps(allrows,indent=2)+'\n')
summary={root:{c:sum(r['root']==root and r['category']==c for r in allrows) for c in ['A','B','C','']} for root in sorted({r['root'] for r in allrows})}
(E/'SUMMARY_R2.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
