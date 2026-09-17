import json
from pathlib import Path
p=Path('progress/BUILD_STATIC_0917B')
def refs(name):
    return dict((ref,sha) for sha,ref in (line.split() for line in (p/'raw'/name).read_text().splitlines()))
before=refs('203_remote_checkpoint_before.stdout.txt')
after=refs('208_native_remote_after.stdout.txt')
target='refs/heads/codex/r5-ehabi-diagnosis'
head=(p/'raw/206_native_checkpoint_head.stdout.txt').read_text().strip()
assert set(before)==set(after)
assert after[target]==head
unchanged={ref:before[ref]==after[ref] for ref in before if ref!=target}
assert all(unchanged.values()), unchanged
assert (p/'raw/207_push_native_checkpoint.exitcode').read_text().strip()=='0'
r=dict(status='PASS',commit=head,only_pushed=target,other_branches_unchanged=unchanged,before=before,after=after,package_push=False)
(p/'native_checkpoint_push.json').write_text(json.dumps(r,indent=2))
print(json.dumps(r,indent=2))
