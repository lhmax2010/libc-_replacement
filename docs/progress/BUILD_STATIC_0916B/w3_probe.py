import csv, datetime, hashlib, json, re, subprocess
from pathlib import Path

p=Path('progress/BUILD_STATIC_0916B'); t=Path('tmp/NIGHT_0917/finish_validation')
old=Path('progress/BUILD_NIGHT_0917'); rows=[]
def run(name,cmd,timeout=90):
    try:
        r=subprocess.run(cmd,capture_output=True,text=True,errors='replace',timeout=timeout)
        row=dict(name=name,command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)
    except subprocess.TimeoutExpired as e:
        row=dict(name=name,command=cmd,exitcode='TIMEOUT',stdout=str(e.stdout),stderr=str(e.stderr))
    rows.append(row); (p/'w3_commands.json').write_text(json.dumps(rows,indent=2)); print(name,row['exitcode'],flush=True)
    return row
c=json.loads((old/'bpftrace-x86_64-commands.json').read_text())['build']; base=c[:c.index('--')]
inside='/tmp/night/finish_validation'; libs='/tmp/night/builds/runtime-x86_64-libcxx/lib'
src=t/'condition_cancel_probe.cpp'
expected=subprocess.check_output(['git','show','2955f679d0be0b7aee96ae49125955940d01467f:docs/progress/R111/code/condition_cancel_probe.cpp'])
assert src.read_bytes().rstrip()==expected.rstrip()
(p/'probe_source_identity.json').write_text(json.dumps(dict(revision='2955f679d0be0b7aee96ae49125955940d01467f',source='docs/progress/R111/code/condition_cancel_probe.cpp',source_sha256=hashlib.sha256(expected).hexdigest(),copied_sha256=hashlib.sha256(src.read_bytes()).hexdigest(),only_possible_terminal_newline_difference=True),indent=2))
common=['x86_64-tizen-linux-gnu-clang++','-std=c++20','-O0','-g','-fno-inline','-stdlib=libc++','-nostdinc++',
 '-I/tmp/night/builds/runtime-x86_64-libcxx/include/c++/v1',str(inside+'/condition_cancel_probe.cpp'),'-L'+libs,'-pthread','-latomic']
matrix=[]; summaries=[]
for mode in ['static','dynamic']:
    exe=inside+'/cancel-'+mode
    link=['-Wl,-Map,'+exe+'.map']
    if mode=='static': link+=['-static-libgcc','-static-libstdc++','-Wl,--start-group',libs+'/libc++.a',libs+'/libc++abi.a','-Wl,--end-group']
    else: link+=['--rtlib=libgcc','-Wl,-rpath,'+libs,'-lc++abi']
    r=run(mode+'-compile',base+['--']+common+link+['-o',exe])
    if r['exitcode']!=0: continue
    dep=run(mode+'-needed',base+['--','readelf','-d',exe])
    needed=re.findall(r'Shared library: \[(.*?)\]',dep['stdout'])
    if mode=='static': assert not any(x in needed for x in ['libc++.so.1','libc++abi.so.1','libgcc_s.so.1','libstdc++.so.6'])
    else: assert all(x in needed for x in ['libc++.so.1','libc++abi.so.1','libgcc_s.so.1'])
    run(mode+'-resolved-libraries',base+['--','ldd',exe])
    summaries.append(dict(mode=mode,needed=needed,sha256=hashlib.sha256((t/('cancel-'+mode)).read_bytes()).hexdigest()))
    for wait in ['wait','system_clock','steady_clock','custom_clock']:
        for n in range(1,6):
            r=run(f'{mode}-{wait}-{n}',base+['--',exe,wait],timeout=20)
            passed=r['exitcode']==0 and 'event=wait_confirmed' in r['stdout'] and 'event=cleanup' in r['stdout'] and 'cancel_rc=0 join_rc=0 canceled=1 cleanup_count=1' in r['stdout'] and 'event=terminate' not in r['stdout']
            matrix.append(dict(mode=mode,wait=wait,repetition=n,exitcode=r['exitcode'],assertions_pass=passed))
(p/'cancel_matrix.json').write_text(json.dumps(matrix,indent=2))
(p/'cancel_binaries.json').write_text(json.dumps(summaries,indent=2))
print('Cancellation assertions:',sum(x['assertions_pass'] for x in matrix),'/',len(matrix))

# Link-map evidence, rather than matching symbol names alone, identifies archives.
for name in ['cancel-static','cancel-dynamic']:
    f=t/(name+'.map')
    if not f.exists(): continue
    hits=[line for line in f.read_text(errors='replace').splitlines() if any(s in line for s in ['libgcc_eh','libgcc_s','libunwind','cxa_personality','__forced_unwind'])]
    (p/(name+'-map-excerpts.txt')).write_text('\n'.join(hits)+'\n')
for arch in ['x86_64','aarch64']:
    binary=Path(f'tmp/NIGHT_0917/builds/bpftrace-{arch}-static/src/bpftrace')
    command=['nm','-C',str(binary)]
    r=subprocess.run(command,capture_output=True,text=True,errors='replace')
    wanted=['_Unwind_ForcedUnwind','__gxx_personality_v0','__gcc_personality_v0','__forced_unwind']
    hits=[s for s in r.stdout.splitlines() if any(w in s for w in wanted)]
    (p/f'{arch}-unwinder-symbols.json').write_text(json.dumps(dict(command=command,exitcode=r.returncode,binary_sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),matching_lines=hits,unmatched_queries=[w for w in wanted if not any(w in s for s in hits)]),indent=2))
    members=list(csv.DictReader((old/f'bpftrace-{arch}-pulled_members.tsv').open(),delimiter='\t'))
    (p/f'{arch}-unwinder-members.json').write_text(json.dumps([x for x in members if any(s in str(x) for s in ['libgcc_eh','libunwind','cxa_personality','libc++abi.a'])],indent=2))
