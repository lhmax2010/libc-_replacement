import subprocess
branch=subprocess.check_output(['git','branch','--show-current'],text=True).strip()
paths=subprocess.check_output(['git','diff','--cached','--name-only'],text=True).splitlines()
assert branch=='codex/r5-ehabi-diagnosis',branch
assert paths and all(x.startswith('docs/progress/BUILD_NIGHT_0917/') for x in paths),paths
print('PASS staged scope',len(paths),'files',branch)
