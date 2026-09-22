"""Copy recipe inputs to fresh temporary paths and generate exact build argv."""
import hashlib,json,subprocess,shlex
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_STATIC_0922'; T=P/'tmp/BPF_STATIC_0922'; T.mkdir(exist_ok=False)
(T/'specs').mkdir(); (E/'cell_commands').mkdir(); records=[]; identities=[]
def sha(f):
    with f.open('rb') as s:return hashlib.file_digest(s,'sha256').hexdigest()
def cp(src,dst):
    cmd=['cp','-a',str(src),str(dst)]; r=subprocess.run(cmd,capture_output=True,text=True)
    records.append(dict(command=shlex.join(cmd),exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr))
    (E/'setup-commands.json').write_text(json.dumps(records,indent=2)); assert r.returncode==0
original=P/'tmp/BPF_W1_0921/specs/bpftrace.spec'; cp(original,T/'specs/bpftrace.spec')
identities.append(dict(kind='original_spec',source=str(original),copy=str(T/'specs/bpftrace.spec'),sha256=sha(original)))
patch=P/'docs/progress/BPF_LIBBPF_0922/armv7l-static-libbpf.patch'
for arch in ('armv7l','aarch64'):
    label=f'bpf-{arch}-libcxx'; cmd=json.loads((P/'progress/BPF_W1_0921/cell_commands'/f'{label}.json').read_text())
    sources=Path(cmd[cmd.index('/tmp/bpf-sources')-1]); dest=T/f'sources-{arch}'; cp(sources,dest)
    for f in sorted(sources.iterdir()):
        if f.is_file():
            h=sha(f); assert h==sha(dest/f.name)
            identities.append(dict(kind='source',arch=arch,source=str(f),copy=str(dest/f.name),sha256=h))
    cp(patch,dest/'static-link-libbpf.patch')
    for leaf in ('BUILD','BUILDROOT','RPMS','SRPMS','TMP'):(T/f'rpm-{label}'/leaf).mkdir(parents=True)
    cmd[cmd.index('/tmp/task')-1]=str(T)
    cmd[cmd.index('/tmp/bpf-sources')-1]=str(dest)
    cmd=[x.replace('/tmp/task/specs/bpftrace.spec','/tmp/task/specs/bpftrace.spec') for x in cmd]
    (E/'cell_commands'/f'{label}.json').write_text(json.dumps(cmd,indent=2))
    pre=cmd[:cmd.index('--')+1]; rpm=cmd[cmd.index('--')+1:]
    # Same target, macro files and definitions as the actual RPM invocation.
    pre+=['/bin/sh','-ec',shlex.join(['rpm',*rpm[1:rpm.index('--noclean')],'--eval','%{_binary_payload}'])+'; '+shlex.join(['rpmspec',*rpm[1:rpm.index('--noclean')],'-P','/tmp/task/specs/bpftrace.spec'])]
    (E/'cell_commands'/f'precheck-{arch}.json').write_text(json.dumps(pre,indent=2))
(E/'INPUTS.json').write_text(json.dumps(identities,indent=2))
print('COPIED_INPUTS_VERIFIED',len(identities),'NO_SPEC_EDIT_PERFORMED_BY_SETUP')
