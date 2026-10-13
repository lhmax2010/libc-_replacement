"""Apply the explicitly adjudicated provenance gates; do not erase old results."""
import csv,difflib,hashlib,json,pathlib,shlex,subprocess
E=pathlib.Path(__file__).resolve().parent;R=E.parents[2];T=R/'tmp/QB_1194606_FIX';O=E/'raw/qemu_gate_revised_R4';O.mkdir(exist_ok=False)
def run(label,args):
    p=O/label;p.with_suffix('.command.txt').write_text(shlex.join(map(str,args))+'\n')
    q=subprocess.run(list(map(str,args)),capture_output=True);p.with_suffix('.stdout').write_bytes(q.stdout);p.with_suffix('.stderr').write_bytes(q.stderr);p.with_suffix('.exitcode').write_text(str(q.returncode)+'\n');assert q.returncode==0,(label,q.returncode)
    return q.stdout.decode(errors='replace')
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
    return h.hexdigest()
old=T/'previous_R4/clang-accel-0.4-2.1.x86_64.rpm';previous=run('previous-list',['rpm','-qlp',old]).splitlines()
A={'/emul/usr/lib64/libc++.so.1','/emul/usr/lib64/libc++abi.so.1'}
B={'/emul/usr/lib64/'+x for x in ['libc++.so.1.0','libc++abi.so.1.0','libLLVM.so.22.1','libclang-cpp.so.22.1','libatomic.so.1','libatomic.so.1.2.0']}
summary=[];provenance=[];inventory=[]
for arch in ['armv7l','aarch64','riscv64']:
    cell='qemu-accel-'+arch;root=T/'GBS-R4'/cell;build=E/('build-'+cell+'_R4')
    assert (build/'exitcode').read_text().strip()=='0'
    log=(build/'build.log').read_text();assert 'Installed (but unpackaged) file(s) found' not in log
    source=root/'local/BUILD-ROOTS/scratch.x86_64.0/home/abuild/rpmbuild/SOURCES'
    data=json.loads((E/('QEMU_PROVENANCE_'+arch+'_R4.json')).read_text());assert data['required_once'] and data['cross_variant_only_target_entries']
    named={}
    for i,p in enumerate(sorted((root/'local/repos/qb1194606/x86_64/RPMS').rglob('*.rpm'))):
        run(arch+f'-{i}-digest',['rpm','-K','--nosignature',p]);name=run(arch+f'-{i}-name',['rpm','-qp','--qf','%{NAME}',p]);files=run(arch+f'-{i}-list',['rpm','-qlp',p]).splitlines()
        record=dict(arch=arch,name=name,path=str(p),sha256=sha(p),bytes=p.stat().st_size,files=files);inventory.append(record);named[name]=record
    for name in ['qemu-accel','python-accel','clang-accel']:
        listed=run(arch+'-'+name+'-source-list',['cat',source/(name+'.list')]).splitlines()
        missing=sorted(set(listed)-set(named[name]['files']));assert not missing,(arch,name,missing)
        if name!='clang-accel':assert not (A & set(named[name]['files'])),(arch,name,'new paths leaked to other subpackage')
    files=named['clang-accel']['files'];assert all(files.count(x)==1 for x in A)
    added=set(files)-set(previous);removed=set(previous)-set(files)
    tools={x for x in added|removed if x.startswith('/emul/usr/bin/') and x.endswith(('-clang','-clang++'))}
    assert added-tools==A|B and not (removed-tools),(arch,added,removed)
    diff=list(difflib.unified_diff(previous,files,fromfile='published-aarch64',tofile=arch,lineterm=''));(O/(arch+'-previous.diff')).write_text('\n'.join(diff)+'\n')
    for path in sorted(added|removed):
        kind='A' if path in A else 'B' if path in B else 'ARCH'
        evidence='raw/066_R3_qemu_show.stdout.txt' if kind=='A' else ('../QB_1194606/excerpts/qemu-accel-armv7l-x86_64.txt:33-34' if path.endswith(('libc++.so.1.0','libc++abi.so.1.0')) else 'raw/adjudication_R4/previous-clang-needed.stdout; raw/adjudication_R4/'+arch+'-clang-needed.stdout; '+arch+'-input-rpm.stdout; '+arch+'-ldd-original-inputs.stdout; '+arch+'-ldd-realpaths-v2.stdout') if kind=='B' else 'raw/adjudication_R4/'+arch+'-variant-list.diff'
        provenance.append(dict(arch=arch,file=path,change='added' if path in added else 'removed',type=kind,evidence=evidence))
    summary.append(dict(arch=arch,build_exit=0,seconds=float((build/'elapsed-seconds.txt').read_text()),required_paths_once=True,no_unpacked_error=True,main_python_source_lists_covered=True,provenance='PASS',result='PASS'))
    if arch=='aarch64':
        dest=T/'adjudication_R4/new-aarch64';dest.mkdir(parents=True,exist_ok=False);p=named['clang-accel']['path']
        run('new-clang-extract',['nice','-n','19','ionice','-c','3','bash','-o','pipefail','-c','cd '+shlex.quote(str(dest))+' && rpm2cpio '+shlex.quote(p)+' | cpio -idm --quiet --no-absolute-filenames "./emul/usr/bin/clang-22" "emul/usr/bin/clang-22"'])
        run('new-rpm-clang-needed',['readelf','-d',dest/'emul/usr/bin/clang-22']);run('new-rpm-clang-sha',['sha256sum',dest/'emul/usr/bin/clang-22'])
(E/'QEMU_RPM_INVENTORY_REVISED_R4.json').write_text(json.dumps(inventory,indent=2)+'\n')
(E/'QEMU_GATE_REVISED_R4.json').write_text(json.dumps(summary,indent=2)+'\n')
with (E/'QEMU_PROVENANCE_REVISED_R4.tsv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=['arch','file','change','type','evidence'],delimiter='\t');w.writeheader();w.writerows(provenance)
print(json.dumps(summary,indent=2))
