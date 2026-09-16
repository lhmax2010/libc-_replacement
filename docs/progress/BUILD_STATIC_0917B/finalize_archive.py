import hashlib
import json
import shutil
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917B')
d=Path('docs/progress/BUILD_STATIC_0917B')
assert not Path('password.md').exists()
records=[]
for prefix in ['134_archive_resume','135_git_before_publish','136_index_before_stage']:
    row={'label':prefix}
    for suffix in ['command.txt','stdout.txt','stderr.txt','exitcode','started.txt','finished.txt','resources.txt']:
        q=p/'raw'/f'{prefix}.{suffix}'
        assert q.exists(), str(q)
        row[suffix]=q.read_text()
    assert row['exitcode'].strip()=='0'
    records.append(row)
(d/'ARCHIVE_COMPLETION.json').write_text(json.dumps(records,indent=2))
shutil.copy2(Path(__file__),d/Path(__file__).name)
print('Finalized completed archive and pre-push command records; original raw snapshots untouched.')
