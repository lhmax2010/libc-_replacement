"""Check source-built programs and providers; report prebuilt asset separately."""
import hashlib
import json
import re
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

arch, mode, release = sys.argv[1:]
assert arch in ['armv7l','aarch64'] and mode in ['libcxx','gcc']
root = Path('/home/toolchain/development/libc++_replacement')
p = root/'progress/BUILD_W2C_0915'
out = p/f'checks/bpftrace_{arch}_{mode}'
out.mkdir(parents=True, exist_ok=False)
journal = (out/'commands.jsonl').open('w', buffering=1)
def run(command, cwd=None):
    result = subprocess.run(command, cwd=cwd, capture_output=True, text=True)
    journal.write(json.dumps(dict(command=shlex.join(map(str,command)), cwd=str(cwd or root),
        exitcode=result.returncode, stdout=result.stdout, stderr=result.stderr))+'\n')
    result.check_returncode()
    return result.stdout

build_root = root/f'tmp/GBS-ROOT/BUILD-W2C-bpftrace-{arch}-{mode}'
log = p/f'build-w2c-bpftrace-{arch}-{mode}.build.log'
events = [json.loads(line) for line in (p/f'build-w2c-bpftrace-{arch}-{mode}.events.jsonl').read_text().splitlines()]
assert events[-1]['event'] == 'finished' and events[-1]['result'] == 0
text = log.read_text()
if mode == 'libcxx':
    assert 'The CXX compiler identification is Clang 22.1.8' in text
    assert '-stdlib=libc++' in text and '-lc++abi' in text
else:
    assert 'The CXX compiler identification is GNU 14.2.0' in text
    assert '-stdlib=libc++' not in text and '-lc++abi' not in text
rpms = sorted(path for path in (build_root/'local/repos').glob(f'*/{arch}/RPMS/bpftrace*-{release}.{arch}.rpm')
              if '-debuginfo-' not in path.name and '-debugsource-' not in path.name)
assert len(rpms) == 3, rpms
extract = Path(tempfile.mkdtemp(prefix=f'bpf_{arch}_{mode}_', dir=root/'tmp/BUILD_W2C_0915'))
with (out/'rpm_sha256.txt').open('w') as sums:
    for rpm in rpms:
        sums.write(run(['sha256sum',str(rpm)]))
        run(['bash','-euo','pipefail','-c','rpm2cpio "$1" | cpio -idm --quiet','extract',str(rpm)], cwd=extract)

expected = 'libc++.so.1' if mode == 'libcxx' else 'libstdc++.so.6'
forbidden = 'libstdc++.so.6' if mode == 'libcxx' else 'libc++.so.1'
programs = []
for name in ['bpftrace','bpftrace-aotrt']:
    program = extract/'usr/bin'/name
    assert program.is_file()
    header = run(['readelf','-h',str(program)])
    assert ('AArch64' if arch=='aarch64' else 'ARM') in header
    needed = run(['readelf','-d',str(program)])
    (out/f'{name}.dynamic.txt').write_text(needed)
    assert f'[{expected}]' in needed, name
    assert f'[{forbidden}]' not in needed, name
    symbols = run(['nm','-D','--undefined-only','-C',str(program)])
    programs.append(dict(name=name, needed=re.findall(r'Shared library: \[(.*?)\]',needed),
        undefined_std1_lines=sum('std::__1::' in line for line in symbols.splitlines()),
        undefined_stdcxx11_lines=sum('std::__cxx11::' in line for line in symbols.splitlines())))

scratch = build_root/f'local/BUILD-ROOTS/scratch.{arch}.0'
providers = []
for pattern in ['libLLVM.so.22.1','libbcc.so.*']:
    files = [path for path in scratch.glob(f'usr/lib*/{pattern}') if path.is_file() and not path.is_symlink()]
    assert len(files)==1, (pattern,files)
    needed = run(['readelf','-d',str(files[0])])
    assert f'[{expected}]' in needed and f'[{forbidden}]' not in needed
    providers.append(dict(path=str(files[0]), needed=re.findall(r'Shared library: \[(.*?)\]',needed)))

prebuilt = extract/'usr/bin/bpftrace-static'
assert prebuilt.is_file()
prebuilt_dynamic = run(['readelf','-d',str(prebuilt)])
(out/'prebuilt.dynamic.txt').write_text(prebuilt_dynamic)
asset_arch = 'arm' if arch=='armv7l' else 'aarch64'
original = root/f'tmp/BUILD_W2C_0915/bpf-prebuilt.vuDDCU/prebuilt-static-bpftrace/{asset_arch}/usr/bin/bpftrace'
original_sha = run(['sha256sum',str(original)]).split()[0]
packaged_sha = run(['sha256sum',str(prebuilt)]).split()[0]
summary = dict(status='SOURCE_PROGRAMS_AND_PROVIDERS_PASS', arch=arch, mode=mode,
    compiler='Clang 22.1.8' if mode=='libcxx' else 'GNU 14.2.0', rpm_count=len(rpms),
    source_programs=programs, build_root_providers=providers, extract=str(extract),
    prebuilt=dict(original_sha256=original_sha, packaged_sha256=packaged_sha,
        byte_identical=original_sha==packaged_sha,
        needed=re.findall(r'Shared library: \[(.*?)\]',prebuilt_dynamic),
        embedded_cxx_standard_library='NOT_OBSERVED',
        package_push_gate='WAITING_FOR_HUMAN_SCOPE_CONFIRMATION'))
(out/'summary.json').write_text(json.dumps(summary, ensure_ascii=False, indent=2)+'\n')
print(json.dumps(summary, ensure_ascii=False))
