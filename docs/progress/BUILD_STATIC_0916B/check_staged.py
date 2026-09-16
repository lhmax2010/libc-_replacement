import subprocess
prefix='docs/progress/BUILD_STATIC_0916B/'
assert subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/r5-ehabi-diagnosis'
paths=subprocess.check_output(['git','diff','--cached','--name-only','-z']).decode().split('\0')
assert any(paths)
assert all(not p or p.startswith(prefix) for p in paths), 'Unexpected staged path'
print('Staged scope verified:',sum(bool(p) for p in paths),'task evidence files only.')
