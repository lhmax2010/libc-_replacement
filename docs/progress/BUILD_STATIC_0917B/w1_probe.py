import hashlib
import json
import subprocess
import time
from pathlib import Path

p = Path('progress/BUILD_STATIC_0917B')
t = Path('tmp/STATIC_0917B').resolve()
t.mkdir(parents=True, exist_ok=True)
source = Path('docs/progress/BUILD_STATIC_0917/inputs/condition_cancel_probe.cpp').resolve()
old = json.loads(Path('progress/BUILD_NIGHT_0917/bpftrace-x86_64-commands.json').read_text())['build']
base = old[:old.index('--')]
base[base.index('--bind')] = '--ro-bind'
base += ['--bind', str(t), '/tmp/static0917b', '--ro-bind', str(source), '/tmp/static0917b/probe.cpp']

def run(name, command, timeout=120):
    start = time.time()
    try:
        r = subprocess.run(command, capture_output=True, text=True, errors='replace', timeout=timeout)
        row = dict(command=command, exitcode=r.returncode, stdout=r.stdout, stderr=r.stderr)
    except subprocess.TimeoutExpired:
        row = dict(command=command, exitcode='TIMEOUT', stdout='', stderr='timeout')
    row.update(start_epoch=start, seconds=time.time()-start)
    dest = p / (name+'.json')
    assert not dest.exists(), dest
    dest.write_text(json.dumps(row, indent=2))
    print(name, row['exitcode'], flush=True)
    return row

if __name__ == '__main__':
    (p/'w1_probe_input.json').write_text(json.dumps(dict(source=str(source), sha256=hashlib.sha256(source.read_bytes()).hexdigest(), scope='Conditional approximation, not recovered original recipe'), indent=2))
    run('w1-compiler', base+['--','x86_64-tizen-linux-gnu-clang++','--version'])
    matrix=[]
    for mode in ['static-gcc', 'shared-gcc']:
        exe='/tmp/static0917b/probe-'+mode
        command=base+['--','x86_64-tizen-linux-gnu-clang++','-std=c++20','-O0','-g','-fno-inline','/tmp/static0917b/probe.cpp','-pthread','-latomic','-stdlib=libstdc++','-static-libstdc++','-static-libgcc' if mode=='static-gcc' else '-shared-libgcc','-Wl,-Map,'+exe+'.map','-o',exe]
        if run('w1-'+mode+'-compile',command)['exitcode'] != 0:
            raise SystemExit(1)
        run('w1-'+mode+'-needed',base+['--','readelf','-d',exe])
        for wait in ['wait','system_clock','steady_clock','custom_clock']:
            for n in range(1,6):
                row=run(f'w1-{mode}-{wait}-{n}',base+['--',exe,wait],20)
                assertion=(row['exitcode']==0 and 'event=wait_confirmed' in row['stdout'] and 'cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in row['stdout'])
                matrix.append(dict(mode=mode, wait=wait, repetition=n, exitcode=row['exitcode'], cancellation_assertions=assertion))
    (p/'w1_matrix.json').write_text(json.dumps(matrix,indent=2))
    for mode in ['static-gcc','shared-gcc']:
        rows=[r for r in matrix if r['mode']==mode]
        print(mode, 'runs',len(rows),'passes',sum(r['cancellation_assertions'] for r in rows),'exitcodes',sorted(set(r['exitcode'] for r in rows)),flush=True)
