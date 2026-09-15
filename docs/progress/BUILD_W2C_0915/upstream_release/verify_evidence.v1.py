"""Verify the recorded checks, without rerunning binaries or changing package sources."""
import hashlib
import json
from pathlib import Path

p = Path('progress/BUILD_W2C_0915/upstream_release')
j = json.loads((p / 'comparison.json').read_text())
assert len(j['assets']) == 5
assert sum(x['downloaded'] for x in j['assets']) == 3
for a in j['assets']:
    if a['downloaded']:
        assert a['github_digest'] == 'sha256:' + a['sha256']
assert len(j['bundles']) == 2
assert all(x['member_count'] == 83 for x in j['bundles'])
assert all(len(x['binary_members']) == 1 for x in j['bundles'])
rows = {x['object']: x for x in j['elf_comparison']}
assert 'libstdc++.so.6' in rows['official_inner_bpftrace']['needed']
assert 'libc.so.6' in rows['official_inner_bpftrace']['needed']
assert rows['official_inner_bpftrace']['stdcxx11_dynamic_lines'] == 260
for label in ('source1002_armv7l', 'source1002_aarch64'):
    assert not rows[label]['matches_official_outer']
    assert not rows[label]['matches_official_inner']
for label in ('015_official_version', '016_arm_asset_version', '027_aarch64_private_library_path'):
    assert (p / 'raw' / (label + '.exitcode')).read_text().strip() == '0'
    assert (p / 'raw' / (label + '.stdout.txt')).read_text().strip() == 'bpftrace v0.24.2'
for command in sorted((p / 'raw').glob('*.command.txt')):
    name = command.name.removesuffix('.command.txt')
    for suffix in ('.stdout.txt', '.stderr.txt', '.exitcode'):
        assert (command.parent / (name + suffix)).exists(), (name, suffix)
    int((command.parent / (name + '.exitcode')).read_text().strip())
expected = {
    'codes/R104/bpftrace/packaging/prebuilt-static-bpftrace.tar.gz':
    '3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123',
    'codes/R104/bpftrace/packaging/bpftrace.spec':
    'f471f98b0d48dd769de286fbeaa8d7292230fbf316c95b06e03e2b631a86cdd3',
}
for name, value in expected.items():
    assert hashlib.file_digest(Path(name).open('rb'), 'sha256').hexdigest() == value
print('PASS: 3 official downloads verified; 2 bundles checked; 2 Source1002 mismatches; 3 version runs exit 0.')
print('PASS: raw command records have outputs and exit codes; package input hashes unchanged.')
