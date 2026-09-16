import hashlib, json, re, shutil, subprocess, sys, time
from pathlib import Path

p = Path('progress/BUILD_STATIC_0917')
t = Path('tmp/STATIC_0917').resolve()
t.mkdir(parents=True, exist_ok=True)
c = json.loads(Path('progress/BUILD_NIGHT_0917/bpftrace-x86_64-commands.json').read_text())['build']
base = c[:c.index('--')]
base[base.index('--bind')] = '--ro-bind'
base += ['--bind', str(t), '/tmp/static0917']
inside = '/tmp/static0917'
libs = '/tmp/night/builds/runtime-x86_64-libcxx/lib'

def run(name, cmd, timeout=90):
    before = time.time()
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', timeout=timeout)
        row = dict(command=cmd, exitcode=r.returncode, stdout=r.stdout, stderr=r.stderr)
    except subprocess.TimeoutExpired as e:
        row = dict(command=cmd, exitcode='TIMEOUT', stdout=str(e.stdout), stderr=str(e.stderr))
    row.update(start_epoch=before, duration_seconds=time.time()-before)
    out = p / (name+'.json')
    assert not out.exists(), out
    out.write_text(json.dumps(row, indent=2))
    print(name, row['exitcode'], flush=True)
    return row

if __name__ == '__main__':
    family = sys.argv[1]
    source = Path('docs/progress/BUILD_STATIC_0916B/inputs/condition_cancel_probe.cpp')
    shutil.copy2(source, t/'condition_cancel_probe.cpp')
    (p/'probe_sha256.txt').write_text(hashlib.sha256(source.read_bytes()).hexdigest()+'  condition_cancel_probe.cpp\n')
    common = ['x86_64-tizen-linux-gnu-clang++','-std=c++20','-O0','-g','-fno-inline',inside+'/condition_cancel_probe.cpp','-pthread','-latomic']
    run(family+'-compiler', base+['--',common[0],'--version'])
    matrix=[]
    for mode in (['static','dynamic'] if family == 'gnu' else ['dynamic-gcc']):
        exe = inside+'/'+family+'-'+mode
        link = ['-Wl,-Map,'+exe+'.map']
        flags = []
        if family == 'gnu':
            flags = ['-stdlib=libstdc++']
            if mode == 'static': link += ['-static-libgcc','-static-libstdc++']
            else: link += ['-shared-libgcc']
        else:
            flags = ['-stdlib=libc++','-nostdinc++','-I/tmp/night/builds/runtime-x86_64-libcxx/include/c++/v1','-L'+libs]
            link += ['-static-libstdc++','-Wl,--start-group',libs+'/libc++.a',libs+'/libc++abi.a','-Wl,--end-group']
        cmd = base+['--']+common+flags+link+['-o',exe]
        run(family+'-'+mode+'-driver', cmd+['-###'])
        r = run(family+'-'+mode+'-compile', cmd)
        if r['exitcode'] != 0: raise SystemExit(1)
        dep = run(family+'-'+mode+'-needed', base+['--','readelf','-d',exe])
        needed = re.findall(r'Shared library: \[(.*?)\]',dep['stdout'])
        if mode == 'static': assert not any(x in needed for x in ['libstdc++.so.6','libgcc_s.so.1','libc++.so.1'])
        else: assert 'libgcc_s.so.1' in needed
        for wait in ['wait','system_clock','steady_clock','custom_clock']:
            for n in range(1,6):
                r = run(f'{family}-{mode}-{wait}-{n}',base+['--',exe,wait],20)
                ok = r['exitcode']==0 and 'event=wait_confirmed' in r['stdout'] and 'event=cleanup' in r['stdout'] and 'cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in r['stdout'] and 'event=terminate' not in r['stdout']
                matrix.append(dict(family=family, mode=mode, wait=wait, repetition=n, exitcode=r['exitcode'], assertions_pass=ok))
        lines=(t/(family+'-'+mode+'.map')).read_text(errors='replace').splitlines()
        (p/(family+'-'+mode+'-map-excerpts.txt')).write_text('\n'.join(x for x in lines if any(y in x for y in ['libgcc_eh','libgcc_s','libunwind','cxa_personality','eh_personality','__forced_unwind']))+'\n')
    (p/(family+'-matrix.json')).write_text(json.dumps(matrix,indent=2))
    print('PASS',sum(r['assertions_pass'] for r in matrix),'/',len(matrix))
