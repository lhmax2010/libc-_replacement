"""Supplementary read-only checks under the human's revised R4 gates."""
import collections, difflib, hashlib, json, pathlib, re, shlex, subprocess, sys
E=pathlib.Path(__file__).resolve().parent; R=E.parents[2]; T=R/'tmp/QB_1194606_FIX'
O=E/'raw/adjudication_R4';O.mkdir(exist_ok=True)
def run(label,args,ok=(0,)):
    p=O/label.replace('.', '_')
    if p.with_suffix('.command.txt').exists() and '--resume' in sys.argv:
        assert p.with_suffix('.command.txt').read_text()==shlex.join(map(str,args))+'\n'
        assert int(p.with_suffix('.exitcode').read_text()) in ok
        return p.with_suffix('.stdout').read_text()
    assert not p.with_suffix('.command.txt').exists(),label
    p.with_suffix('.command.txt').write_text(shlex.join(map(str,args))+'\n')
    q=subprocess.run(list(map(str,args)),capture_output=True)
    p.with_suffix('.stdout').write_bytes(q.stdout);p.with_suffix('.stderr').write_bytes(q.stderr);p.with_suffix('.exitcode').write_text(str(q.returncode)+'\n')
    assert q.returncode in ok,(label,q.returncode)
    return q.stdout.decode(errors='replace')
def sha(p):return hashlib.file_digest(p.open('rb'),'sha256').hexdigest() if hasattr(hashlib,'file_digest') else hashlib.sha256(p.read_bytes()).hexdigest()
mode=sys.argv[1]
if mode=='icu':
    rows=[]
    for p in sorted((T/'verified_R4/icu/candidate/usr/lib64').glob('libicu*.so.78.1')):
        nm=run(p.name+'-nm',['nm','-D','--undefined-only',p]);dyn=run(p.name+'-needed',['readelf','-d',p])
        std=sum('_ZNSt3__1' in x for x in nm.splitlines());versions=sum(bool(re.search('GLIBCXX|CXXABI_',x)) for x in nm.splitlines())
        origin=[x for x in nm.splitlines() if re.search(r'\b_ZN(?:K|V|VK)?St3__1',x)]
        passed='[libstdc++.so.6]' not in dyn and versions==0 and (not origin or '[libc++.so.1]' in dyn)
        if p.name.startswith('libicudata'):passed=passed and not re.search(r'\b(?:_ZNSt3__1|_ZSt)',nm)
        if p.name.startswith('libicui18n'):passed=passed and std==0 and '[libc++abi.so.1]' in dyn
        rows.append(dict(file=p.name,sha256=sha(p),std_prefix_count=std,std_symbol_origin_count=len(origin),version_refs=versions,direct_libcxx='[libc++.so.1]' in dyn,result='PASS' if passed else 'FAIL'))
    (E/'ICU_ADJUDICATION_R4.json').write_text(json.dumps(rows,indent=2)+'\n');print(json.dumps(rows,indent=2));assert len(rows)==6 and all(x['result']=='PASS' for x in rows)
elif mode=='previous':
    rpm=T/'previous_R4/clang-accel-0.4-2.1.x86_64.rpm';dest=T/'adjudication_R4/previous';dest.mkdir(parents=True,exist_ok=False)
    run('previous-rpm-header',['rpm','-qp','--qf','%{NAME} %{VERSION}-%{RELEASE}\n%{VCS}\n',rpm]);run('previous-rpm-requires',['rpm','-qp','--requires',rpm])
    run('previous-clang-extract',['nice','-n','19','ionice','-c','3','bash','-o','pipefail','-c','cd '+shlex.quote(str(dest))+' && rpm2cpio '+shlex.quote(str(rpm))+' | cpio -idm --quiet --no-absolute-filenames "./emul/usr/bin/clang-22" "emul/usr/bin/clang-22"'])
    p=dest/'emul/usr/bin/clang-22';run('previous-clang-needed',['readelf','-d',p]);run('previous-clang-sha',['sha256sum',p])
else:
    arch=mode;root=T/f'GBS-R4/qemu-accel-{arch}/local/BUILD-ROOTS/scratch.x86_64.0';source=root/'home/abuild/rpmbuild/SOURCES'
    files=run(arch+'-clang-accel-list',['cat',source/'clang-accel.list']).splitlines()
    for name in ['qemu-accel','python-accel']:run(arch+'-'+name+'-list',['cat',source/(name+'.list')])
    baseline=(T/'GBS-R4/qemu-accel-aarch64/local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/SOURCES/clang-accel.list').read_text().splitlines()
    d=list(difflib.unified_diff(baseline,files,fromfile='aarch64',tofile=arch,lineterm=''));(O/(arch+'-variant-list.diff')).write_text('\n'.join(d)+'\n')
    target={'armv7l':'armv7l-tizen-linux-gnueabi','aarch64':'aarch64-tizen-linux-gnu','riscv64':'riscv64-tizen-linux-gnu'}[arch]
    normalized=[x.replace('/'+target+'-clang','/TARGET-clang') for x in files];bn=[x.replace('/aarch64-tizen-linux-gnu-clang','/TARGET-clang') for x in baseline]
    required=['/emul/usr/lib64/libc++.so.1','/emul/usr/lib64/libc++abi.so.1'];assert all(files.count(x)==1 for x in required)
    # Lists are sorted by the original triple. After replacing that triple,
    # compare sorted lists (not sets), retaining duplicate-count checks.
    assert sorted(normalized)==sorted(bn),'Unrelated cross-variant differences'
    common=['bwrap','--ro-bind',root,'/','--dev','/dev','--proc','/proc','--tmpfs','/tmp','--setenv','PATH','/usr/bin:/bin','--']
    run(arch+'-input-rpm',common+['rpm','-q','--qf','%{NAME} %{VERSION}-%{RELEASE} %{VCS}\n','llvm','clang','libc++','libc++abi'])
    run(arch+'-clang-needed',common+['readelf','-d','/usr/bin/clang-22'])
    # Exactly the glob inputs used by prepare_package_list clang; read-only ldd,
    # not a re-run of the modifying list-generation function.
    cmd='for executable in /usr/bin/clang-* /usr/bin/clang++ /usr/bin/opt /usr/bin/lli /usr/bin/llc /usr/bin/llvm-* /usr/bin/lld /usr/bin/ld.lld /usr/lib64/libLTO.so* /usr/lib64/LLVMgold.so*; do printf "EXECUTABLE %s\\n" "$executable"; ldd "$executable"; done'
    ldd=run(arch+'-ldd-original-inputs',common+['sh','-c',cmd])
    needed=['libLLVM.so.22.1','libclang-cpp.so.22.1','libatomic.so.1'];assert all(x in ldd for x in needed)
    run(arch+'-ldd-realpaths-v2',common+['sh','-c','for p in /lib64/libatomic.so.1 /usr/lib64/libc++.so.1 /usr/lib64/libc++abi.so.1; do printf "%s => " "$p"; readlink -f "$p" || exit; done'])
    result=dict(arch=arch,required_once=True,cross_variant_only_target_entries=True,libcxx_entries=[x for x in files if '/libc++' in x],ldd_libraries_present=needed)
    (E/('QEMU_PROVENANCE_'+arch+'_R4.json')).write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
