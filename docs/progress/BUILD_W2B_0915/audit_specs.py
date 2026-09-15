import csv
import hashlib
import subprocess
from pathlib import Path

workspace = Path('/home/toolchain/development/libc++_replacement')
output = workspace / 'progress/BUILD_W2B_0915/spec_audit'
output.mkdir(exist_ok=True)
rows = list(csv.DictReader((workspace / 'docs/progress/BUILD_W2_0912/PACKAGE_STATUS.tsv').open(), delimiter='\t'))
with (output / 'inputs.tsv').open('w') as table, (output / 'guard_context.txt').open('w') as context:
    table.write('package\trevision\tspec\tsha256\n')
    for row in rows:
        package = row['package']
        repo = workspace / 'codes/R104' / package
        revision = row['adaptation_commit']
        if package == 'llvm':
            repo = workspace / 'tmp/BUILD_W1BC_0911/target-fetch'
        if package in ('bcc-tools', 'bpftrace'):
            revision = subprocess.check_output(['git', '-C', str(repo), 'rev-parse', 'HEAD'], text=True).strip()
            paths = sorted((repo / 'packaging').glob('*.spec'))
            blobs = [(str(p.relative_to(repo)), p.read_bytes()) for p in paths]
            revision += '+WORKTREE' if package == 'bcc-tools' else ''
        else:
            paths = subprocess.check_output(['git', '-C', str(repo), 'ls-tree', '-r', '--name-only', revision, 'packaging'], text=True).splitlines()
            paths = [p for p in paths if p.endswith('.spec')]
            if package == 'llvm':
                paths = ['packaging/llvm.spec']
            blobs = [(p, subprocess.check_output(['git', '-C', str(repo), 'show', f'{revision}:{p}'])) for p in paths]
        for path, blob in blobs:
            name = package + '__' + Path(path).name
            (output / name).write_bytes(blob)
            table.write(f'{package}\t{revision}\t{path}\t{hashlib.sha256(blob).hexdigest()}\n')
            lines = blob.decode().splitlines()
            selected = set()
            terms = ('_toolchain', 'build_with_libcxx', 'stdlib=libc++', 'lc++abi', 'LLVM_ENABLE_LIBCXX', 'LIBCXX', 'libc++-devel', 'libc++abi-devel')
            for i, line in enumerate(lines):
                if any(term in line for term in terms):
                    selected.update(range(max(0, i-3), min(len(lines), i+5)))
            context.write(f'PACKAGE={package} REVISION={revision} FILE={path}\n')
            for i in sorted(selected):
                context.write(f'{i+1}: {lines[i]}\n')
            context.write('\n')
print('package_count=' + str(len(rows)))
print('spec snapshots and SHA256 recorded; semantic review pending')
