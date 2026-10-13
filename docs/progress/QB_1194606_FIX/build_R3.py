"""Serial GBS launcher with real cgroup limit, low priority, I/O pauses and records."""
import datetime, hashlib, json, os, pathlib, re, shlex, subprocess, sys, time
ROOT=pathlib.Path(__file__).resolve().parents[3]
E=ROOT/'docs/progress/QB_1194606_FIX'
T=ROOT/'tmp/QB_1194606_FIX'
def now(): return datetime.datetime.now().astimezone().isoformat()
def limit():
    total=int(next(x.split()[1] for x in pathlib.Path('/proc/meminfo').read_text().splitlines() if x.startswith('MemTotal:')))*1024
    return total//2//4096*4096
def probe():
    p=T/('io-probe-'+str(os.getpid()))
    start=time.monotonic()
    data=b'R3-io-probe-20261013\n'*800000
    with p.open('xb') as f: f.write(data);f.flush();os.fsync(f.fileno())
    assert hashlib.sha256(p.read_bytes()).digest()==hashlib.sha256(data).digest()
    p.unlink()
    print(json.dumps({'seconds':time.monotonic()-start,'bytes':len(data)}),flush=True)
    return 0
if sys.argv[1]=='probe': sys.exit(probe())
if sys.argv[1]=='entry':
    pkg=sys.argv[2]
    cg=pathlib.Path('/proc/self/cgroup').read_text()
    group=pathlib.Path('/sys/fs/cgroup')/cg.strip().split('::')[-1].lstrip('/')
    mem=(group/'memory.max').read_text().strip()
    io=subprocess.check_output(['ionice','-p',str(os.getpid())],text=True).strip()
    assert mem==str(limit()) and os.getpriority(os.PRIO_PROCESS,0)==19 and 'idle' in io
    (E/f'RESOURCE-{pkg}_R3.json').write_text(json.dumps({'pid':os.getpid(),'cgroup':cg,'memory.max':mem,'nice':19,'ionice':io,'argv':sys.argv[3:]},indent=2)+'\n')
    os.environ.update(PATH='/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin',MAKEFLAGS='-j1',CMAKE_BUILD_PARALLEL_LEVEL='1')
    os.execvp(sys.argv[3],sys.argv[3:])
pkg=sys.argv[1]
assert pkg in ['cereal','icu','qemu-accel']
cell=pkg+('-'+sys.argv[2] if len(sys.argv)>2 else '')
out=E/('build-'+cell+'_R3');out.mkdir(exist_ok=False)
def record(label,cmd,timeout=None):
    (out/(label+'.command.txt')).write_text(shlex.join(cmd)+'\n')
    try:
        r=subprocess.run(cmd,capture_output=True,timeout=timeout)
        (out/(label+'.stdout')).write_bytes(r.stdout);(out/(label+'.stderr')).write_bytes(r.stderr)
        code=r.returncode
    except subprocess.TimeoutExpired as exc:
        (out/(label+'.stdout')).write_bytes(exc.stdout or b'');(out/(label+'.stderr')).write_bytes(exc.stderr or b'')
        code=124
    (out/(label+'.exitcode')).write_text(str(code)+'\n')
    return code
assert record('resource',['bash','tools/resource_gate.sh','--level','medium'])==0
assert record('disk',['df','-h',str(T)])==0
v=os.statvfs(T);assert v.f_bavail*v.f_frsize>=20*1024**3
assert record('initial-io',[sys.executable,__file__,'probe'],30)==0
gbs=['gbs','-c',str(T/'gbs.conf'),'build','-A','x86_64','--threads','1','--define','_binary_payload w5.xzdio','--define','_smp_mflags -j1','--define','_smp_build_ncpus 1','--no-patch-export','--spec',('qemu-accel-armv7l.spec' if pkg=='qemu-accel' else pkg+'.spec'),str(T/'repos'/pkg)]
unit='qb1194606-fix-'+pkg+'-'+str(os.getpid())
cmd=['systemd-run','--user','--scope','--unit',unit,'-p','MemoryMax='+str(limit()),'nice','-n','19','ionice','-c','3',sys.executable,__file__,'entry',cell,*gbs]
(out/'argv.json').write_text(json.dumps(cmd,indent=2)+'\n');(out/'command.txt').write_text(shlex.join(cmd)+'\n')
(out/'started.txt').write_text(now()+'\n')
start=time.monotonic();hour=0;targets=0;checked=0;position=0;group=None
with (out/'build.log').open('wb') as log:
    p=subprocess.Popen(cmd,cwd=T/'repos'/pkg,stdout=log,stderr=subprocess.STDOUT,stdin=subprocess.DEVNULL)
    while p.poll() is None:
        time.sleep(5)
        resource=E/f'RESOURCE-{cell}_R3.json'
        if resource.exists() and group is None:
            info=json.loads(resource.read_text());group=pathlib.Path('/sys/fs/cgroup')/info['cgroup'].strip().split('::')[-1].lstrip('/')
        with (out/'build.log').open('rb') as f:
            f.seek(position);chunk=f.read();position=f.tell()
        targets+=len(re.findall(rb'(?:Building (?:C|CXX)|\] (?:clang|gcc)| -c |\[[0-9]+/[0-9]+\])',chunk))
        if time.monotonic()-start>=hour*3600:
            with (E/'HOURLY_STATUS_R3.md').open('a') as h:h.write(f'- {now()} {pkg}: RUNNING, elapsed={time.monotonic()-start:.0f}s, observed compile/target lines={targets}\n')
            hour+=1
        if targets>=checked+500:
            checked=targets
            rc=record(f'io-{targets}',[sys.executable,__file__,'probe'],30)
            if rc:
                assert group is not None
                (group/'cgroup.freeze').write_text('1')
                passed=False
                for attempt in range(1,4):
                    with (E/'HOURLY_STATUS_R3.md').open('a') as h:h.write(f'- {now()} {pkg}: IO_PAUSED attempt={attempt}\n')
                    time.sleep(600)
                    if record(f'io-{targets}-retry{attempt}',[sys.executable,__file__,'probe'],30)==0:passed=True;break
                (group/'cgroup.freeze').write_text('0')
                if not passed:
                    subprocess.run(['systemctl','--user','stop',unit+'.scope'])
                    p.wait();break
code=p.wait()
(out/'exitcode').write_text(str(code)+'\n');(out/'finished.txt').write_text(now()+'\n')
(out/'elapsed-seconds.txt').write_text(str(time.monotonic()-start)+'\n')
print(pkg,'exitcode',code,'seconds',time.monotonic()-start,flush=True)
sys.exit(code)
