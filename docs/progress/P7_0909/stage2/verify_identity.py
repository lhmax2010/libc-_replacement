#!/usr/bin/env python3
"""核对选读源码→归档成员→源码 RPM→冻结清单，避免仅凭同名版本拼接。"""
import csv,hashlib,json,pathlib,re,subprocess,sys,tarfile,collections
P=pathlib.Path('docs/progress/P7_0909');O=P/'stage2';C=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/R11')
def sha(p):
    h=hashlib.sha256()
    with pathlib.Path(p).open('rb') as f:
        for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
    return h.hexdigest()
source_records={r['location']:r for r in csv.DictReader((P/'inputs/source_package_records.tsv').open(),delimiter='\t')}
observed=[];archive_groups=collections.defaultdict(list)
for p in sorted((O/'raw').glob('*.stdout')):
    first=p.open().readline()
    if not first.startswith('{"source":'):continue
    r=json.loads(first);assert sha(r['snapshot'])==r['sha256']==sha(r['source'])
    r['record']=str(p.relative_to(O));observed.append(r)
    m=re.search(r'/corpus/extracted/([^/]+)/unpacked/([^/]+)/(.*)',r['source'])
    if m:archive_groups[(m[1],m[2])].append((m[3],r))
    elif r['source'].startswith('tmp/P7_0909/csapi/'):
        archive_groups[('csapi-tizenfx-15.0.0.19383-1.src.rpm','csapi-tizenfx-15.0.0.19383.tar.gz')].append((r['source'].split('tmp/P7_0909/csapi/',1)[1],r))
results=[];rpm_checked={}
for (srpm,archive),members in archive_groups.items():
    s=source_records[srpm];rpm=C/'srpms'/s['repo_id']/srpm;assert rpm.is_file(),rpm
    if srpm not in rpm_checked:
        actual=sha(rpm);assert actual==s['checksum'],(srpm,actual,s['checksum'])
        stem=O/'raw'/('source_identity_'+s['name'])
        if not pathlib.Path(str(stem)+'.stdout').exists():
            subprocess.run([sys.executable,str(P/'record.py'),str(stem),'rpm','-qp','--qf','[%{FILENAMES}\t%{FILEDIGESTS}\n]',str(rpm)],check=True)
        else:
            assert pathlib.Path(str(stem)+'.exitcode').read_text().strip()=='0'
        rpm_checked[srpm]=dict(line.split('\t',1) for line in pathlib.Path(str(stem)+'.stdout').read_text().splitlines() if '\t' in line)
    ap=pathlib.Path('tmp/P7_0909/csapi')/archive if s['name']=='csapi-tizenfx' else pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/corpus/extracted')/srpm/'payload'/archive
    ah=sha(ap);assert ah==rpm_checked[srpm][archive],(ap,'archive does not match frozen RPM')
    expected={n:r['sha256'] for n,r in members};found={}
    with tarfile.open(ap,'r|*') as tf:
        for member in tf:
            n=member.name.removeprefix('./')
            if n in expected:
                assert member.isfile();found[n]=hashlib.sha256(tf.extractfile(member).read()).hexdigest();assert found[n]==expected[n],(n,'source differs from archive')
    assert found==expected,(srpm,set(expected)-set(found))
    for n,r in members:results.append(dict(source_rpm=srpm,rpm_sha256=s['checksum'],archive=archive,archive_sha256=ah,member=n,file_sha256=r['sha256'],snapshot=r['snapshot'],record=r['record'],status='MATCH'))
with (O/'tables/source_identity.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=list(results[0]),delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(results)
with (O/'tables/evidence_files.tsv').open('w',newline='') as f:
    w=csv.DictWriter(f,fieldnames=['source','snapshot','sha256','range','record'],delimiter='\t',lineterminator='\n');w.writeheader();w.writerows(observed)
print(json.dumps(dict(selected_read_events=len(observed),source_read_events=len(results),source_rpms=len(rpm_checked),result='PASS'),ensure_ascii=False))
