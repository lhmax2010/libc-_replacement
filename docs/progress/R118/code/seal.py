#!/usr/bin/env python3
"""封存已完成证据；不把正在运行的 recorder 文件当成最终内容。"""
import hashlib
import json
import pathlib
import sys

root = pathlib.Path(__file__).resolve().parents[1]
manifest = root / 'SHA256.json'
suffixes = ('.command.txt', '.stdout', '.stderr', '.exitcode', '.time.json')

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

if '--verify' in sys.argv:
    rows = json.loads(manifest.read_text())
    failures = [r['path'] for r in rows
                if not (root/r['path']).is_file()
                or digest(root/r['path']) != r['sha256']]
    print(json.dumps({'checked_files':len(rows), 'failures':failures}))
    raise SystemExit(bool(failures))

rows = []
for path in sorted(root.rglob('*')):
    if not path.is_file() or path == manifest or '__pycache__' in path.parts:
        continue
    if 'raw' in path.relative_to(root).parts:
        suffix = next((s for s in suffixes if path.name.endswith(s)), None)
        if suffix:
            stem = str(path)[:-len(suffix)]
            if not all(pathlib.Path(stem+s).is_file() for s in suffixes):
                continue
    rows.append({'path':str(path.relative_to(root)), 'sha256':digest(path)})
manifest.write_text(json.dumps(rows, indent=2, ensure_ascii=False)+'\n')
print(json.dumps({'sealed_files':len(rows)}))
