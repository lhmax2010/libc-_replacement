"""Use verified RPM hashes; reuse aarch64 packages without rebuilding them."""
import json
import shlex
import subprocess
from pathlib import Path

root = Path('/home/toolchain/development/libc++_replacement')
p = root/'progress/BUILD_W2C_0915'
journal = (p/'prepare_bcc_repos.commands.jsonl').open('x', buffering=1)
manifest = []
for mode in ['libcxx','gcc']:
    repo = root/f'tmp/BUILD_W2C_0915/bcc-{mode}-repo'
    assert repo.is_dir()
    for arch in ['armv7l','aarch64']:
        if arch == 'armv7l':
            record = p/f'checks/bcc_armv7l_{mode}/check_1/rpm_sha256.txt'
        else:
            filename = 'preserved_rpm_sha256.txt' if mode == 'libcxx' else 'rpm_sha256.txt'
            record = root/f'docs/progress/BUILD_W2_0912/checks/bcc_aarch64_{mode}/{filename}'
        target_dir = repo/arch
        target_dir.mkdir(exist_ok=False)
        rows = record.read_text().splitlines()
        assert len(rows) == 3
        for row in rows:
            digest, original = row.split(None,1)
            source = Path(original.strip())
            if not source.is_absolute():
                source = root/source
            assert source.is_file(), source
            command = ['sha256sum', str(source)]
            result = subprocess.run(command, capture_output=True, text=True)
            journal.write(json.dumps(dict(command=shlex.join(command), exitcode=result.returncode,
                stdout=result.stdout, stderr=result.stderr))+'\n')
            result.check_returncode()
            assert result.stdout.split()[0] == digest
            (target_dir/source.name).symlink_to(source.resolve())
            manifest.append(dict(mode=mode, arch=arch, path=str(source), sha256=digest,
                                 prior_hash_record=str(record)))
    command = ['createrepo_c','--workers','1','--update',str(repo)]
    result = subprocess.run(command, capture_output=True, text=True)
    journal.write(json.dumps(dict(command=shlex.join(command), exitcode=result.returncode,
        stdout=result.stdout, stderr=result.stderr))+'\n')
    result.check_returncode()
(p/'bcc_inputs_for_bpftrace.json').write_text(json.dumps(manifest, indent=2)+'\n')
print(f'PASS verified_input_rpms={len(manifest)}')
