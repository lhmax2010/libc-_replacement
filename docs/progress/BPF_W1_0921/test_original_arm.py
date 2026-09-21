"""Test normal declared-input replacement against a private copy of the original RPM database; no installation."""
import hashlib,json,shlex,subprocess
from pathlib import Path
p=Path.cwd(); out=p/'progress/BPF_W1_0921'; task=p/'tmp/BPF_W1_0921'
root=p/'tmp/GBS-ROOT/BUILD-W2C-bpftrace-armv7l-libcxx/local/BUILD-ROOTS/scratch.armv7l.0'
records=[]
def run(cmd):
    r=subprocess.run(cmd,capture_output=True,text=True,errors='replace')
    records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (out/'original-arm-transaction-test.json').write_text(json.dumps(records,indent=2))
    print(shlex.join(cmd),'exitcode',r.returncode,flush=True); print(r.stdout,r.stderr,flush=True)
    assert r.returncode==0
    return r.stdout
db=task/'rpmdb-arm-origin-test'; assert not db.exists()
run(['cp','-a',str(root/'var/lib/rpm'),str(db)])
hashes=[]
for f in sorted((root/'var/lib/rpm').rglob('*')):
    if f.is_file() and not f.is_symlink():
        rel=f.relative_to(root/'var/lib/rpm'); a=hashlib.sha256(f.read_bytes()).hexdigest(); b=hashlib.sha256((db/rel).read_bytes()).hexdigest(); assert a==b
        hashes.append(dict(path=str(rel),source_sha256=a,copy_sha256=b))
(out/'original-arm-rpmdb-copy.json').write_text(json.dumps(hashes,indent=2))
base=['bwrap','--unshare-user','--uid','0','--gid','0','--ro-bind',str(root),'/',
      '--bind',str(db),'/var/lib/rpm','--dev','/dev','--proc','/proc','--tmpfs','/tmp',
      '--setenv','PATH','/bin:/usr/bin:/sbin:/usr/sbin','--setenv','QEMU_RESERVED_VA','0x100000000','--setenv','LC_ALL','C']
run(base+['--','rpm','-qa','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\n'])
rows=json.loads((out/'installed-armv7l-inputs.json').read_text()); guest=[]
for i,row in enumerate(rows):
    f=Path(row['path'])
    with f.open('rb') as stream: digest=hashlib.file_digest(stream,'sha256').hexdigest()
    assert digest==row['sha256']
    g=f'/tmp/input-rpms/{i}.rpm'; guest.append(g); base+=['--ro-bind',str(f),g]
run(base+['--','rpm','-Uvh','--ignorearch','--oldpackage','--replacepkgs','--test',*guest])
(out/'original-arm-test-result.json').write_text(json.dumps(dict(status='NORMAL_TRANSACTION_TEST_PASS',installation_performed=False,replacefiles_used=False,original_root_modified=False),indent=2))
