import csv,hashlib,subprocess
from pathlib import Path
root=Path('docs/progress/BUILD_STATIC_0916B')
rows=list(csv.DictReader((root/'SHA256SUMS.tsv').open(),delimiter='\t'))
for r in rows:
    data=(root/r['path']).read_bytes()
    assert len(data)==int(r['bytes']) and hashlib.sha256(data).hexdigest()==r['sha256'],r['path']
head=subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()
branch='refs/heads/codex/r5-ehabi-diagnosis'
remote=subprocess.check_output(['git','ls-remote','--heads','origin',branch],text=True).strip()
assert remote==head+'\t'+branch
print('Archive hashes verified:',len(rows))
print('Local/remote HEAD:',head)
print('Task result remains PARTIAL with static cancellation FAIL.')
