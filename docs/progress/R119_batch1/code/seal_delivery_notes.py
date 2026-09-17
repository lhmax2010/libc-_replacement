"""只更新封存后本助手明确修改的交付说明，追加新记录摘要；不重复读大证据。"""
from common import *
import json
manifest=OUT/'SHA256.json'
rows={r['path']:r for r in json.loads(manifest.read_text())}
changed={'STATUS.md','W1/RESOURCE_STOP.md','code/seal_delivery_notes.py'}
for p in sorted(OUT.rglob('*')):
    if not p.is_file()or p==manifest or '__pycache__'in p.parts:continue
    rel=str(p.relative_to(OUT))
    if rel in rows and rel not in changed:continue
    if '/raw/'in str(p):
        suffix=next((s for s in ['.command.txt','.stdout','.stderr','.exitcode','.time.json']if p.name.endswith(s)),None)
        if suffix and not pathlib.Path(str(p)[:-len(suffix)]+'.time.json').is_file():continue
    rows[rel]={'path':rel,'sha256':digest(p)}
save(manifest,[rows[k]for k in sorted(rows)])
print('DELIVERY_NOTES_UPDATED',sorted(changed),'TOTAL_MANIFEST_ENTRIES',len(rows))
