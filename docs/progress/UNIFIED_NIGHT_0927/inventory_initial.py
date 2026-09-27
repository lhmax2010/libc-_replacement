"""Read known output repositories and retained RPM manifests; never use buildroot-installed files as RPM substitutes."""
import pathlib,json,glob,csv,subprocess,hashlib,collections,time
E=pathlib.Path(__file__).resolve().parent
pkgs={'abseil-cpp','boost','icu','jsoncpp','libsigc++','pcre','taglib','tensorflow2','llvm','bcc-tools','bpftrace','libcxx-runtimes'}
paths=set(); patterns=['tmp/GBS-ROOT/*/local/repos/*/*/RPMS/*.rpm','tmp/*/rpms/*/*.rpm','tmp/*/rpm*/RPMS/*/*.rpm','tmp/*/llvm-rpms/*/*.rpm','tmp/*/rpms/*.rpm','progress/R10*/artifacts/*/*.rpm']
for pattern in patterns:paths.update(glob.glob(pattern))
for arch in ('armv7l','aarch64'):
    for r in json.loads(pathlib.Path(f'docs/progress/BPF_W1_0921/installed-{arch}-inputs.json').read_text()):
        for k in ('source','path'):
            if k in r:paths.add(r[k])
commands=[];out=[]
def call(argv):
    p=subprocess.run(argv,capture_output=True,text=True);commands.append({'argv':argv,'exitcode':p.returncode,'stdout':p.stdout,'stderr':p.stderr});return p
for path in sorted(paths):
    p=pathlib.Path(path)
    if not p.is_file() or any(x in p.name for x in ('-debuginfo-','-debugsource-','.src.rpm')):continue
    # Prefilter filenames before opening RPM headers.
    if not any(p.name.startswith(x) for x in ('abseil','boost','icu','libicu','jsoncpp','libsigc','pcre','taglib','tensorflow','llvm','libllvm','clang','libomp','bcc','bpftrace','libc++')):continue
    r=call(['rpm','-qp','--qf','%{NAME}\t%{VERSION}-%{RELEASE}\t%{ARCH}\t%{SOURCERPM}\t%{VCS}\n',str(p)])
    if r.returncode:continue
    name,ver,arch,src,vcs=r.stdout.strip().split('\t')
    source=src.rsplit('-',2)[0]
    if source not in pkgs:continue
    req=call(['rpm','-qp','--requires',str(p)])
    sha=hashlib.file_digest(p.open('rb'),'sha256').hexdigest()
    out.append({'source':source,'name':name,'version':ver,'arch':arch,'path':str(p),'sha256':sha,'bytes':p.stat().st_size,'VCS':vcs,'Requires':req.stdout.splitlines(),'query_exitcode':req.returncode})
(E/'RPM_INVENTORY.json').write_text(json.dumps(out,ensure_ascii=False,indent=2)+'\n')
(E/'inventory_commands.json').write_text(json.dumps(commands,ensure_ascii=False,indent=2)+'\n')
(E/'INVENTORY_SEARCH.json').write_text(json.dumps({'patterns':patterns,'candidate_paths':len(paths),'rpm_records':len(out)},indent=2)+'\n')
with (E/'RPM_INVENTORY.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['source','name','version','arch','path','sha256','bytes','VCS','libcxx_requires','libstdcxx_requires'])
    for r in out:w.writerow([r[k] for k in ('source','name','version','arch','path','sha256','bytes','VCS')]+[';'.join(x for x in r['Requires'] if 'libc++.so' in x),';'.join(x for x in r['Requires'] if 'libstdc++.so' in x)])
for pkg in sorted(pkgs):
    for arch in ('armv7l','aarch64','x86_64'):
        rs=[r for r in out if r['source']==pkg and r['arch']==arch]
        print(pkg,arch,len(rs),'libcxx',sum(any('libc++.so' in x for x in r['Requires']) for r in rs),'libstdcxx',sum(any('libstdc++.so' in x for x in r['Requires']) for r in rs),flush=True)
