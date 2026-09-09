#!/usr/bin/env python3
"""续测前核对；运行库只读取/部署，绝不构建。"""
import csv, hashlib, pathlib, re, subprocess, sys
r=pathlib.Path.cwd(); out=r/'docs/progress/IMPL_0909/W2'; raw=out/'raw'
def run(label, args):
    stem=raw/('resume_preflight_'+label)
    rc=subprocess.call([sys.executable,str(r/'docs/progress/R114/code/record.py'),str(stem),'timeout','60',*map(str,args)])
    assert rc==0,(label,rc)
    return pathlib.Path(str(stem)+'.stdout').read_text().replace('\r\n','\n')
release=run('release',['sed','-n','1,130p','/home/toolchain/development/libc++_replacement/docs/progress/BUILD_STATUS_0909.md'])
assert '板子状态：**板子已释放**' in release
old=(raw/'previous_cleanup_04_runtime_hashes.stdout').read_text()
hashes=dict((p,h) for h,p in re.findall(r'^([0-9a-f]{64})\s+(\S+)',old,re.M))
rows=[]
for variant,build in [('runtime','build-armv7l'),('baseline','build-baseline-armv7l')]:
    for name in ['libc++.so.1','libc++abi.so.1']:
        remote=f'/opt/usr/impl_0908_lit/{variant}/{name}'
        local=r/'tmp/IMPL_0908'/build/'lib'/name
        digest=hashlib.sha256(local.read_bytes()).hexdigest()
        assert digest==hashes[remote],('RUNTIME_MISMATCH_STOP',local,digest,hashes[remote])
        rows.append([str(local),remote,digest,'MATCHES_PREVIOUS_BOARD'])
        if variant=='runtime':
            for suite in ['libcxx','libcxxabi']:
                installed=r/'tmp/IMPL_0908'/build/suite/'test-suite-install/lib'/name
                if installed.exists():
                    assert hashlib.sha256(installed.read_bytes()).hexdigest()==digest,installed
with (out/'RESUME_RUNTIME_IDENTITIES.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['local','board','sha256','prior_comparison']);w.writerows(rows)
print('LOCAL_RUNTIME_IDENTITY=PASS four libraries; test install copies checked',flush=True)
compiler='/home/toolchain/development/libc++_replacement/progress/R68/tools/armv7l_lit_host_clangxx.sh'
version=run('compiler_version',[compiler,'--version']);assert '22.1.8' in version
for batch in ['full','resume1','resume2']:
    text=(r/f'docs/progress/IMPL_0908/W3/raw/lit_armv7l_new_{batch}/stderr').read_text()
    assert 'clang-22.1.8' in text and 'optimization=none' in text
    substitutions=dict(re.findall(r'\(([^)]+)\) Using (%\{[^}]+\}) substitution: (.*)',text)) if False else re.findall(r'\(([^)]+)\) Using (%\{[^}]+\}) substitution: (.*)',text)
    selected=[x for x in substitutions if x[1] in ('%{cxx}','%{flags}','%{compile_flags}','%{link_flags}')]
    if batch=='full': reference=selected
    else: assert selected==reference,('HISTORICAL_CONFIG_DIFF',batch)
with (out/'RESUME_HISTORICAL_CONFIG.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['suite','substitution','value']);w.writerows(reference)
paths=[pathlib.Path(compiler),r/'docs/progress/IMPL_0908/code/lit_executor.py']
for suite in ['libcxx','libcxxabi']:
    paths.extend([r/f'tmp/IMPL_0908/build-armv7l/{suite}/test/lit.site.cfg',r/f'tmp/IMPL_0908/build-armv7l/{suite}/test/cmake-bridge.cfg'])
run('config_snapshot',['sha256sum',*paths])
print('HISTORICAL_COMPILER_FLAGS=PASS full/resume1/resume2 identical; c++26 optimization=none',flush=True)
sdb=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
text=run('board_status',sdb+['shell','uname -a; test ! -e /opt/usr/impl_0908_lit && test ! -L /opt/usr/impl_0908_lit; rc=$?; printf "PREDEPLOY_ABSENCE=%d\\n" "$rc"; ps -eo pid,user,comm'])
assert 'armv7l' in text and 'PREDEPLOY_ABSENCE=0' in text
print('BOARD_PREDEPLOY=PASS; no deployment performed by this script',flush=True)
