import datetime, difflib, hashlib, json, os, shutil, subprocess
from pathlib import Path

p=Path('progress/BUILD_STATIC_0916B'); old=Path('progress/BUILD_NIGHT_0917')
t=Path('tmp/NIGHT_0917/finish_validation'); t.mkdir(exist_ok=True)
rows=[]
def run(name, cmd, timeout=90):
    start=datetime.datetime.now().astimezone().isoformat()
    try:
        r=subprocess.run(cmd, capture_output=True, text=True, errors='replace', timeout=timeout)
        row=dict(name=name,command=cmd,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr,started=start)
    except subprocess.TimeoutExpired as e:
        row=dict(name=name,command=cmd,exitcode='TIMEOUT',stdout=str(e.stdout),stderr=str(e.stderr),started=start)
    rows.append(row); (p/'w1_commands.json').write_text(json.dumps(rows,indent=2))
    print(name,row['exitcode'],flush=True)
    return row

tests=[('version',['--version']),('help',['--help']),('info',['--info']),('probes',['-l']),
       ('hello',['-e','BEGIN { printf("hello\\n"); exit(); }']),
       ('arithmetic',['-e','BEGIN { printf("value=%d\\n", 2+3); exit(); }']),
       ('map',['-e','BEGIN { @x = 7; print(@x); clear(@x); exit(); }'])]
comparisons=[]; sizes=[]
for arch in ['x86_64','aarch64']:
    c=json.loads((old/f'bpftrace-{arch}-commands.json').read_text())['build']; base=c[:c.index('--')]
    candidate=Path(f'tmp/NIGHT_0917/builds/bpftrace-{arch}-static/src/bpftrace')
    expected={'x86_64':'242855842b9b07ed21d3bbb12e590601e7996b88dc29f335aa701fd3c72a5cc7','aarch64':'0981193d70d2a768ecef078a1980b58185f9d5904469a6de21d6532521561836'}[arch]
    assert hashlib.sha256(candidate.read_bytes()).hexdigest()==expected
    original=Path(f'tmp/NIGHT_0917/original_assets/prebuilt-static-bpftrace/{arch}/usr/bin/bpftrace')
    variants={'candidate':str(candidate).replace('tmp/NIGHT_0917','/tmp/night',1)}
    if original.exists(): variants['original']=str(original).replace('tmp/NIGHT_0917','/tmp/night',1)
    outputs={}
    for variant,binary in variants.items():
        outputs[variant]={name:run(f'{arch}-{variant}-{name}',base+['--',binary]+args) for name,args in tests}
    if 'original' in outputs:
        for name,_ in tests:
            a=outputs['original'][name]; b=outputs['candidate'][name]
            comparisons.append(dict(arch=arch,test=name,original_exitcode=a['exitcode'],candidate_exitcode=b['exitcode'],
                stdout_diff=list(difflib.unified_diff(a['stdout'].splitlines(),b['stdout'].splitlines(),fromfile='original',tofile='candidate',lineterm='')),
                stderr_diff=list(difflib.unified_diff(a['stderr'].splitlines(),b['stderr'].splitlines(),fromfile='original',tofile='candidate',lineterm=''))))
    run(arch+'-rpm-strip-macros',base+['--','rpm','--eval','%{__debug_install_post}\n%{?_rpm_strip_option}\n%{?_rpm_strip_disable}'])
    stripped=t/f'bpftrace-{arch}.stripped'; shutil.copy2(candidate,stripped)
    inside='/tmp/night/finish_validation/'+stripped.name
    r=run(arch+'-strip',base+['--','eu-strip','--remove-comment','-f',inside+'.debug',inside],timeout=180)
    sizes.append(dict(arch=arch,unstripped_bytes=candidate.stat().st_size,strip_exitcode=r['exitcode'],
        stripped_bytes=stripped.stat().st_size,sha256=hashlib.sha256(stripped.read_bytes()).hexdigest(),
        original_bytes=original.stat().st_size if original.exists() else 'NOT_AVAILABLE',
        ratio=stripped.stat().st_size/original.stat().st_size if original.exists() else 'NOT_AVAILABLE'))
    if r['exitcode']==0: run(arch+'-stripped-version',base+['--',inside,'--version'])
(p/'w1_comparisons.json').write_text(json.dumps(comparisons,indent=2))
(p/'w1_sizes.json').write_text(json.dumps(sizes,indent=2))
source=Path('tmp/BUILD_NIGHT_0916.h4taGY/bpftrace-push/docs/stdlib.md')
(p/'builtin_documentation_index.txt').write_text('\n'.join(f'{i}: {s}' for i,s in enumerate(source.read_text().splitlines(),1) if s.startswith('#'))+'\n')
print(json.dumps(sizes,indent=2))
