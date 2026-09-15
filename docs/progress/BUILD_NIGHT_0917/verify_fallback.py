import hashlib,json,re,subprocess
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); t=Path('tmp/NIGHT_0917'); rows=[]; logs=[]
original=t/'original_assets/prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace'
digest=hashlib.sha256(original.read_bytes()).hexdigest()
for mode in ['gcc','undefined']:
    for action in ['configure','build','install']:
        ev=[json.loads(s) for s in (p/f'bpftrace-aarch64-{mode}-{action}.events.jsonl').read_text().splitlines()]
        assert ev[-1]['event']=='finished' and ev[-1]['exitcode']==0
    d=t/'builds'/f'bpftrace-aarch64-{mode}'
    text=(d/'build.ninja').read_text(); assert '-stdlib=libc++' not in text and '-lc++abi' not in text
    compiler=next((d/'CMakeFiles').glob('*/CMakeCXXCompiler.cmake')).read_text()
    assert 'CMAKE_CXX_COMPILER_ID "GNU"' in compiler and 'CMAKE_CXX_COMPILER_VERSION "14.2.0"' in compiler
    f=d/'src/bpftrace'; command=['readelf','-d',str(f)]
    r=subprocess.run(command,capture_output=True,text=True); logs.append(dict(command=command,exitcode=r.returncode,stdout=r.stdout,stderr=r.stderr)); assert r.returncode==0
    needed=re.findall(r'\(NEEDED\).*?\[(.*?)\]',r.stdout)
    assert 'libstdc++.so.6' in needed and 'libc++.so.1' not in needed and 'libc++abi.so.1' not in needed
    asset=t/f'fallback-{mode}/usr/bin/bpftrace-static'; assert asset.read_bytes()==original.read_bytes()
    rows.append(dict(mode=mode,compiler='GNU 14.2.0',build='PASS_ALL_CMAKE_TARGETS',needed=needed,forbidden_libcxx_flags=0,installed_asset_sha256=hashlib.sha256(asset.read_bytes()).hexdigest(),original_sha256=digest,asset_byte_equal=True,rpm_build='NOT_OBSERVED'))
(p/'fallback_verification_commands.json').write_text(json.dumps(logs,indent=2))
(p/'fallback_result.json').write_text(json.dumps(rows,indent=2)); print(json.dumps(rows,indent=2))
