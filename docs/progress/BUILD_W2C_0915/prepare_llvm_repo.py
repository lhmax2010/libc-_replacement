"""Verify finished armv7l LLVM RPMs, then index only explicit local inputs."""
import csv
import json
import shlex
import subprocess
import tempfile
from pathlib import Path

root = Path('/home/toolchain/development/libc++_replacement')
out = root / 'progress/BUILD_W2C_0915'
tmp = root / 'tmp/BUILD_W2C_0915'
tmp.mkdir(exist_ok=True)
journal = (out/'prepare_llvm_repo.commands.jsonl').open('x', buffering=1)

def run(args, cwd=None):
    result = subprocess.run(args, cwd=cwd, text=True, capture_output=True)
    journal.write(json.dumps(dict(command=shlex.join(map(str, args)),
        cwd=str(cwd or root), exitcode=result.returncode,
        stdout=result.stdout, stderr=result.stderr), ensure_ascii=False)+'\n')
    result.check_returncode()
    return result.stdout

assert (out/'raw/048_llvm_confirmed_resume.exitcode').read_text().strip() == '0'
assert 'W2C_INNER_EXIT=0' in (out/'build-w2c-llvm-confirmed.build.log').read_text()
rpm_dir = root/'tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/RPMS/armv7l'
rpms = sorted(rpm_dir.glob('*-22.1.8-115.1.armv7l.rpm'))
assert rpms
records = []
selected = {}
for path in rpms:
    fields = run(['rpm', '-qp', '--qf', '%{NAME}\t%{VERSION}\t%{RELEASE}\t%{ARCH}\n', str(path)]).strip().split('\t')
    assert fields[1:] == ['22.1.8', '115.1', 'armv7l'], fields
    digest = run(['sha256sum', str(path)]).split()[0]
    records.append([*fields, str(path), digest, path.stat().st_size])
    if '-debuginfo' not in fields[0] and '-debugsource' not in fields[0]:
        selected[fields[0]] = path
assert {'clang', 'clang-devel', 'libllvm', 'llvm-devel', 'llvm-static-devel', 'libomp', 'libomp-devel'} <= selected.keys()
with (out/'llvm_armv7l_rpms.tsv').open('w') as stream:
    writer = csv.writer(stream, delimiter='\t')
    writer.writerow(['name','version','release','arch','path','sha256','bytes'])
    writer.writerows(records)

extract = Path(tempfile.mkdtemp(prefix='llvm_verified_', dir=tmp))
run(['bash', '-euo', 'pipefail', '-c', 'rpm2cpio "$1" | cpio -idm --quiet', 'extract', str(selected['libllvm'])], cwd=extract)
dsos = list(extract.rglob('libLLVM.so.22.1'))
assert len(dsos) == 1
needed = run(['readelf', '-d', str(dsos[0])])
(out/'llvm_armv7l_rpm_readelf.txt').write_text(needed)
for name in ['libc++.so.1', 'libc++abi.so.1', 'libgcc_s.so.1']:
    assert f'[{name}]' in needed, name
assert '[libstdc++.so.6]' not in needed
symbols = run(['nm', '-D', '-C', str(dsos[0])])
std1 = [line for line in symbols.splitlines() if 'std::__1::' in line]
stdcxx11 = [line for line in symbols.splitlines() if 'std::__cxx11::' in line]
assert std1 and not stdcxx11
(out/'llvm_armv7l_symbol_sample.txt').write_text('\n'.join(std1[:20])+'\n')

repo = tmp/'llvm-local-repo'
assert not repo.exists(), 'Do not overwrite an existing repository'
(repo/'armv7l').mkdir(parents=True)
for path in selected.values():
    (repo/'armv7l'/path.name).symlink_to(path)
# Existing aarch64 input is reused, not rebuilt in this task.
old_aarch = root/'tmp/BUILD_W2_0912/llvm-local-repo/aarch64'
(repo/'aarch64').mkdir()
aarch_paths = sorted(old_aarch.glob('*.rpm'))
assert aarch_paths
for path in aarch_paths:
    assert path.is_file(), path
    (repo/'aarch64'/path.name).symlink_to(path.resolve())
run(['createrepo_c', '--workers', '1', str(repo)])
for name in ['bcc-libcxx-repo', 'bcc-gcc-repo']:
    empty_repo = tmp/name
    assert not empty_repo.exists(), empty_repo
    empty_repo.mkdir()
    run(['createrepo_c', '--workers', '1', str(empty_repo)])
summary = dict(status='PASS', armv7l_rpm_count=len(rpms),
    armv7l_build_input_rpms=len(selected), std1_dynamic_symbol_lines=len(std1),
    stdcxx11_dynamic_symbol_lines=len(stdcxx11), reused_aarch64_rpms=len(aarch_paths),
    verified_extract=str(extract), libllvm_rpm=str(selected['libllvm']))
(out/'llvm_input_verification.json').write_text(json.dumps(summary, indent=2)+'\n')
print(json.dumps(summary))
