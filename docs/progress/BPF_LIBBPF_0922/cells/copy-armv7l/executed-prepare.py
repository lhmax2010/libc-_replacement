"""Create isolated command recipes; copy one historical tree without editing it."""
import hashlib,json,os,shlex,stat,subprocess,sys
from pathlib import Path
P=Path.cwd(); E=P/'progress/BPF_LIBBPF_0922'; T=P/'tmp/BPF_LIBBPF_0922'; OLD=P/'tmp/BPF_W1_0921'
def sha(f):
    with f.open('rb') as s: return hashlib.file_digest(s,'sha256').hexdigest()
def tree(root):
    rows=[]
    for f in sorted(root.rglob('*')):
        st=f.lstat(); d=dict(path=str(f.relative_to(root)),mode=stat.S_IMODE(st.st_mode),size=st.st_size)
        if f.is_symlink(): d.update(kind='symlink',target=os.readlink(f)); d.pop('size')
        elif f.is_file(): d.update(kind='file',sha256=sha(f))
        elif f.is_dir(): d.update(kind='dir'); d.pop('size')
        else: raise RuntimeError('unexpected special input '+str(f))
        rows.append(d)
    return rows
if sys.argv[1]=='recipes':
    T.mkdir(exist_ok=False); (E/'cell_commands').mkdir(exist_ok=True); (E/'snapshots').mkdir(exist_ok=True)
    for arch in ('armv7l','aarch64'):
        d=T/f'rpm-bpf-{arch}-libcxx/BUILD'; d.mkdir(parents=True)
        dest=d/'bpftrace-0.24.2'; guest=f'/tmp/task/rpm-bpf-{arch}-libcxx/BUILD/bpftrace-0.24.2'
        old=json.loads((P/f'progress/BPF_W1_0921/cell_commands/bpf-{arch}-libcxx.json').read_text())
        b=old[:old.index('--')]
        for i in range(len(b)-2):
            if b[i]=='--bind':
                if b[i+1]==str(OLD): b[i+1]=str(T)
                else: b[i]='--ro-bind'
        (T/'outputs'/arch).mkdir(parents=True)
        libdir='lib' if arch=='armv7l' else 'lib64'
        macros=['--rcfile','/usr/lib/rpm/rpmrc:/home/abuild/.rpmrc','--macros','/usr/lib/rpm/macros:/usr/lib/rpm/macros.d/macros.*:/usr/lib/rpm/platform/%{_target}/macros:/usr/lib/rpm/fileattrs/*.attr:/usr/lib/rpm/tizen/macros:/etc/rpm/macros.*:/etc/rpm/macros:/etc/rpm/%{_target}/macros:/home/abuild/.rpmmacros','--target',arch+'-tizen-linux','--define','_binary_payload w5.xzdio']
        commands={
          'copy-'+arch:['/usr/bin/python3',str(E/'prepare.py'),'copy',arch],
          'input-'+arch:b+['--','/bin/sh','-ec',f'rpm -qf /usr/{libdir}/libbpf.a; sha256sum /usr/{libdir}/libbpf.a; '+shlex.join(['rpm',*macros,'--eval','%{_binary_payload}'])],
          'before-'+arch:b+['--','/bin/sh','-ec',f'cd {guest}/build-static/src; cmake -E cmake_link_script CMakeFiles/bpftrace.dir/link.txt --verbose=1; cp -a bpftrace /tmp/task/outputs/{arch}/before.unstripped'],
          'after-'+arch:b+['--','/bin/sh','-ec',f'cmake -S {guest} -B {guest}/build-static; cmake --build {guest}/build-static --target bpftrace --verbose -- -j1; cp -a {guest}/build-static/src/bpftrace /tmp/task/outputs/{arch}/bpftrace-static.unstripped; /bin/strip -o /tmp/task/outputs/{arch}/bpftrace-static /tmp/task/outputs/{arch}/bpftrace-static.unstripped'],
        }
        for name,cmd in commands.items(): (E/'cell_commands'/f'{name}.json').write_text(json.dumps(cmd,indent=2))
        (E/f'guest-{arch}.json').write_text(json.dumps(b,indent=2))
    specs=[]
    for name in ('bpftrace.spec','bpftrace-undefined-harness.spec'):
        f=OLD/'specs'/name; specs.append(dict(path=str(f),sha256=sha(f)))
    (E/'original-specs.json').write_text(json.dumps(specs,indent=2))
    print('Recipes prepared; no build or source edit')
elif sys.argv[1]=='copy':
    arch=sys.argv[2]; source=OLD/f'rpm-bpf-{arch}-libcxx/BUILD/bpftrace-0.24.2'; dest=T/f'rpm-bpf-{arch}-libcxx/BUILD/bpftrace-0.24.2'
    assert not dest.exists()
    baseline=tree(source); (E/f'original-tree-{arch}.json').write_text(json.dumps(baseline,indent=2))
    cmd=['cp','-a',str(source),str(dest)]; print(shlex.join(cmd),flush=True); r=subprocess.run(cmd); print('copy exitcode',r.returncode,flush=True); assert r.returncode==0
    copied=tree(dest); assert baseline==copied, 'copied tree differs'
    (E/f'copy-{arch}.json').write_text(json.dumps(dict(status='PASS',source=str(source),dest=str(dest),entries=len(copied),bytes=sum(x.get('size',0) for x in copied)),indent=2))
    for name,rel in [('ast-CMakeLists.txt','src/ast/CMakeLists.txt'),('FindLibBpf.cmake','cmake/FindLibBpf.cmake'),('root-CMakeLists.txt','CMakeLists.txt'),('src-CMakeLists.txt','src/CMakeLists.txt'),('before-link.txt','build-static/src/CMakeFiles/bpftrace.dir/link.txt')]:
        f=E/'snapshots'/f'{arch}-{name}'; f.write_bytes((source/rel).read_bytes())
    print('Copy verified',arch,len(copied),'entries',flush=True)
