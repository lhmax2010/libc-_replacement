"""Check the R4 staged scope, identities and credential safety without secret output."""
import base64,csv,hashlib,json,re,subprocess
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923';C=P/'tmp/BUILD_W1BC_0911/target-fetch'
def git(*args,cwd=P):return subprocess.check_output(['git',*args],cwd=cwd)
names=[n for n in git('diff','--cached','--name-only','-z').decode().split('\0') if n]
allowed={'docs/LINE_STATUS.md','docs/progress/BUILD_WEEKEND_0918/w4/PACKAGE_STATUS.md','docs/progress/LLVM_W4_0923/STATUS.md'}
assert names and all(n in allowed or (n.startswith('docs/progress/LLVM_W4_0923/') and ('_R4.' in n or '/raw/R4_' in n)) for n in names)
secret=[];blobs=[]
for filename in ['.quickbuild-credentials','password.md']:
    f=P/filename
    if f.is_file():
        lines=f.read_bytes().splitlines();secret.extend(v for v in (lines[1:] if filename=='.quickbuild-credentials' else lines) if len(v)>3)
for f in [Path.home()/'.oscrc',Path.home()/'.netrc']:
    if f.is_file():
        data=f.read_bytes();blobs.append(data)
        secret.extend(m.group(1) for m in re.finditer(rb'(?im)(?:password|passwd|token|pass)\s*(?:=\s*|\s+)([^\s]+)',data) if len(m.group(1))>3)
ssh=Path.home()/'.ssh'
if ssh.is_dir():
    for f in ssh.iterdir():
        if f.is_file():
            data=f.read_bytes()
            if len(data)>32:blobs.append(data)
            if b'PRIVATE KEY' in data:secret.extend(s for s in data.splitlines() if len(s)>40 and not s.startswith(b'-----'))
needles=[v for s in secret for v in (s,base64.b64encode(s),s.hex().encode())]+[b for b in blobs if len(b)>32]
for name in names:
    data=git('show',':'+name)
    assert not any(s in data for s in needles),'STOP: credential match'
    assert not re.search(rb'-----BEGIN (?:OPENSSH |RSA |EC )?PRIVATE KEY-----|gh[pousr]_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{40,}|(?im:^Authorization:\s*\S+)|(?im:^Cookie:\s*\S+)',data),'STOP: sensitive pattern'
    assert len(data)<5*1024*1024,'STOP: large staged artifact'
sha='5c169afc97945bfb88786d9f38477f6973fe5285'
assert git('rev-parse','FETCH_HEAD',cwd=C).decode().strip()==sha
assert not git('diff','db5b49afa7836db0414140208b2d3ab4f4aa7293',sha,cwd=C)
for commit in [sha,'da14498e07fa4c841b5a6bc7f88766d993982a16']:
    text=git('show','-s','--format=%an <%ae>%n%cn <%ce>%n%(trailers)',commit,cwd=C).decode()
    assert text.splitlines()[:2]==['Hao Lin <hao.lin@samsung.com>']*2
    assert 'Signed-off-by: Hao Lin <hao.lin@samsung.com>' in text
rows=list(csv.DictReader((E/'QUICKBUILD_BRANCHES_R4.tsv').open(),delimiter='\t'))
assert len(rows)==12 and len({r['gerrit_repository'] for r in rows})==11
assert {r['remote_sha'] for r in rows if r['obs_package'] in ['llvm','libcxx-runtimes']}=={sha}
commands=json.loads((E/'branch_commands_R4.json').read_text())
assert len(commands)==12 and all(r['exitcode']==0 for r in commands)
assert all(c['stdout'].split()[0]==r['remote_sha'] for c,r in zip(commands,rows))
target='docs/progress/BUILD_WEEKEND_0918/w4/PACKAGE_STATUS.md'
old=git('show','HEAD:'+target).decode().splitlines();new=(P/target).read_text().splitlines()
assert [x for x in old if not x.startswith('| llvm |')]==[x for x in new if not x.startswith('| llvm |')]
assert sha in next(x for x in new if x.startswith('| llvm |'))
audit=(E/'LOCAL_SPECS_R4.md').read_text()
assert audit.count('CLOSED_BY_HUMAN_DECISION')==26 and audit.count('OUT_OF_SCOPE_BY_HUMAN_DECISION')==27 and audit.count('PUSHED_5c169afc')==6
for path in ['inputs_R2','spec-audit_R3','prep-check/runtime_R2','prep-check/runtime_R3']:
    assert not git('diff','HEAD','--',str(E.relative_to(P)/path))
result=dict(secret_scan='PASS',staged_scope='PASS',staged_files=len(names),remote_rows=12,unique_repositories=11,remote_matches=12,llvm_tree_identical=True,human_closed=26,out_of_scope=27,authorized_copies_pushed=6,package_status_only_llvm_changed=True,old_evidence_preserved=True,package_push=False,build=False,quickbuild=False)
(E/'SELFCHECK_R4.json').write_text(json.dumps(result,indent=2)+'\n')
(E/'SCRIPT_SHA256_R4.json').write_text(json.dumps({f.name:hashlib.sha256(f.read_bytes()).hexdigest() for f in E.glob('*_R4.py')},indent=2)+'\n')
print(json.dumps(result))
