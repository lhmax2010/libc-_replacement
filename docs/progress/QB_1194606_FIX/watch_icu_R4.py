"""Supplement the running guard for ICU's quiet compiler output (no parallel build)."""
import datetime,json,pathlib,re,subprocess,sys,time
E=pathlib.Path(__file__).resolve().parent
B=E/'build-icu_R4'; checked=0
info=json.loads((E/'RESOURCE-icu_R4.json').read_text())
group=pathlib.Path('/sys/fs/cgroup')/info['cgroup'].strip().split('::')[-1].lstrip('/')
def stamp(msg):
    with (B/'quiet-io-watch.log').open('a') as f:f.write(datetime.datetime.now().astimezone().isoformat()+' '+msg+'\n')
def probe(label):
    return subprocess.run([sys.executable,str(E/'run.py'),label,'timeout','30',sys.executable,str(E/'build_R4.py'),'probe'],stdout=subprocess.DEVNULL).returncode
while not (B/'exitcode').exists():
    text=(B/'build.log').read_text(errors='replace')
    count=len(re.findall(r'\] +\S*(?:clang\+\+|clang|gcc|g\+\+)\s+\.\.\.',text))
    if count>=checked+500:
        checked=count;stamp(f'compiler targets={count}')
        rc=probe(f'R4_icu_io_{count}')
        if rc:
            (group/'cgroup.freeze').write_text('1');stamp('IO_PAUSED')
            passed=False
            for attempt in range(1,4):
                time.sleep(600)
                if probe(f'R4_icu_io_{count}_retry{attempt}')==0:passed=True;break
            (group/'cgroup.freeze').write_text('0')
            if not passed:
                unit=group.name
                subprocess.run(['systemctl','--user','stop',unit]);stamp('STOPPED_IO');sys.exit(1)
        stamp(f'probe exitcode={rc}')
    time.sleep(2)
stamp(f'FINISHED targets={count}')
