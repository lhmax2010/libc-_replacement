"""Strict per-cell artifact checks; original output and all differences are retained."""
import difflib, hashlib, json, pathlib, shlex, subprocess, sys
R=pathlib.Path(__file__).resolve().parents[3]; E=R/'docs/progress/QB_1194606_FIX';T=R/'tmp/QB_1194606_FIX'
cell=sys.argv[1];O=E/('verify-'+cell+'_R4');O.mkdir(exist_ok=False)
B=T/'GBS-R4'/cell;root=B/'local/BUILD-ROOTS/scratch.x86_64.0'
n=0;checks=[]
def run(argv,ok=(0,)):
    global n
    n+=1;p=O/f'{n:03d}'
    p.with_suffix('.command.txt').write_text(shlex.join(argv)+'\n')
    q=subprocess.run(argv,capture_output=True)
    p.with_suffix('.stdout').write_bytes(q.stdout);p.with_suffix('.stderr').write_bytes(q.stderr);p.with_suffix('.exitcode').write_text(str(q.returncode)+'\n')
    assert q.returncode in ok,(argv,q.returncode)
    return q.stdout.decode(errors='replace')
def check(name,passed,details=None):
    checks.append(dict(check=name,result='PASS' if passed else 'FAIL',details=details))
    (O/'result.json').write_text(json.dumps(dict(cell=cell,result='PASS' if all(c['result']=='PASS' for c in checks) else 'FAIL',checks=checks),indent=2)+'\n')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def extract(p,kind):
    dest=T/'verified_R4'/cell/kind;dest.mkdir(parents=True,exist_ok=False)
    run(['bash','-o','pipefail','-c','cd '+shlex.quote(str(dest))+' && rpm2cpio '+shlex.quote(str(p))+' | cpio -idm --quiet --no-absolute-filenames'])
    return dest
def compare(label,before,after):
    diff=list(difflib.unified_diff(before,after,fromfile='previous',tofile='candidate',lineterm=''))
    (O/(label+'.diff')).write_text('\n'.join(diff)+('\n' if diff else ''))
    return not diff
assert (E/('build-'+cell+'_R4')/'exitcode').read_text().strip()=='0'
log=(E/('build-'+cell+'_R4')/'build.log').read_text(errors='replace')
rpms=sorted((B/'local/repos/qb1194606/x86_64/RPMS').rglob('*.rpm'))
assert rpms
inventory=[];named={}
for p in rpms:
    run(['rpm','-K','--nosignature',str(p)])
    name=run(['rpm','-qp','--qf','%{NAME}',str(p)])
    files=run(['rpm','-qlp',str(p)]).splitlines()
    inventory.append(dict(name=name,path=str(p),sha256=sha(p),bytes=p.stat().st_size,files=files));named[name]=inventory[-1]
(O/'RPM_INVENTORY.json').write_text(json.dumps(inventory,indent=2)+'\n')
check('gbs_exit_zero_and_RPM_digest',True)
if cell=='cereal':
    check('no_uniform_int_distribution_error','error:' not in '\n'.join(x for x in log.splitlines() if 'uniform_int_distribution' in x))
    bad=[x for x in log.splitlines() if ('basic_binary_oprimitive' in x or 'basic_binary_iprimitive' in x) and ('undefined' in x.lower() or 'error' in x.lower())]
    linked='Built target performance' in log and 'Linking CXX executable performance' in log
    check('original_undefined_references_measured_absent',linked and not bad,{'linked':linked,'bad_lines':bad})
    perf=root/'home/abuild/rpmbuild/BUILD/cereal-1.3.2/build/sandbox/performance'
    dyn=run(['readelf','-d',str(perf)])
    check('performance_NEEDED','[libc++.so.1]' in dyn and '[libstdc++.so.6]' not in dyn,{'path':str(perf),'sha256':sha(perf)})
    old=T/'preQB-cereal-devel.rpm';assert sha(old)=='8b3b0b2555528178d5c8f33b0713861d63ecb0aae685db831c7e4397c73ca7e3'
    oldfiles=run(['rpm','-qlp',str(old)]).splitlines();new=named['cereal-devel']
    check('complete_RPM_file_list_equal',compare('file-list',oldfiles,new['files']))
    a=extract(old,'previous');b=extract(pathlib.Path(new['path']),'candidate')
    cmd=['diff','-r','--no-dereference',str(a/'usr/include'),str(b/'usr/include')]
    diff=run(cmd,ok=(0,1));(O/'headers-content.diff').write_text(diff)
    ma={str(p.relative_to(a)):sha(p) for p in (a/'usr/include').rglob('*') if p.is_file()}
    mb={str(p.relative_to(b)):sha(p) for p in (b/'usr/include').rglob('*') if p.is_file()}
    (O/'headers-sha256.json').write_text(json.dumps({'previous':ma,'candidate':mb},indent=2)+'\n')
    check('headers_byte_identical',ma==mb and bool(ma),{'header_files':len(ma)})
elif cell=='icu':
    commands=[x for x in log.splitlines() if 'tz2icu.cpp' in x and '-iquote' in x and 'clang' in x]
    (O/'tz2icu-command.txt').write_text('\n'.join(commands)+'\n')
    check('tz2icu_iquote_command',bool(commands))
    check('no_version_shadowing_error',not any('error:' in x and ('version:' in x or '/version' in x) for x in log.splitlines()))
    extracted=extract(pathlib.Path(named['libicu']['path']),'candidate')
    libs=sorted(p for p in (extracted/'usr/lib64').glob('libicu*.so.*') if not p.is_symlink())
    assert libs
    for p in libs:
        dyn=run(['readelf','-d',str(p)])
        check('NEEDED:'+p.name,'[libc++.so.1]' in dyn,{'sha256':sha(p),'libstdcxx':'[libstdc++.so.6]' in dyn})
    for name,item in named.items():
        old=next((T/'previous_R4').glob(name+'-[0-9]*.rpm'),None)
        if old:
            previous=run(['rpm','-qlp',str(old)]).splitlines()
            equal=compare(name+'-file-list',previous,item['files'])
            check('file_list_comparison_recorded:'+name,True,{'identical':equal,'previous':str(old),'sha256':sha(old)})
else:
    new=named['clang-accel'];required={'/emul/usr/lib64/libc++.so.1','/emul/usr/lib64/libc++abi.so.1'}
    check('clang_accel_libcxx_files',required.issubset(new['files']))
    check('no_unpacked_files_error','Installed (but unpackaged) file(s) found' not in log)
    old=T/'previous_R4/clang-accel-0.4-2.1.x86_64.rpm'
    previous=run(['rpm','-qlp',str(old)]).splitlines()
    compare('clang-accel-file-list',previous,new['files'])
    added=sorted(set(new['files'])-set(previous));removed=sorted(set(previous)-set(new['files']))
    details={'added':added,'removed':removed,'previous_sha256':sha(old)}
    if cell != 'qemu-accel-aarch64':
        # The only retrieved published RPM has aarch64 compiler entry points.
        # Preserve the raw difference, but do not treat a different variant as
        # a valid baseline for either PASS or a candidate regression verdict.
        details['reason']='NOT_OBSERVED: matching previous-variant RPM unavailable; retrieved baseline is aarch64'
        checks.append(dict(check='file_list_only_two_additions',result='NOT_OBSERVED',details=details))
        (O/'result.json').write_text(json.dumps(dict(cell=cell,result='NOT_OBSERVED' if all(c['result']!='FAIL' for c in checks) else 'FAIL',checks=checks),indent=2)+'\n')
    else:
        check('file_list_only_two_additions',set(added)==required and not removed,details)
print((O/'result.json').read_text())
sys.exit(1 if any(x['result']=='FAIL' for x in checks) else 2 if any(x['result']=='NOT_OBSERVED' for x in checks) else 0)
