"""Read-only archive payload comparison before considering an old build tree."""
import hashlib
import json
import tarfile
import time
from pathlib import Path

out = Path('progress/BUILD_STATIC_0917B/resume_0917')
paths = {
    'old_arm': Path('tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0/home/abuild/rpmbuild/SOURCES/llvm-22.1.8.tar.gz'),
    'current': Path('tmp/NIGHT_0917/finish_validation/rpm/SOURCES/llvm-22.1.8.tar.gz'),
}
start = time.monotonic()
maps = {}
for label, path in paths.items():
    rows = {}
    with tarfile.open(path, 'r|gz') as archive:
        for entry in archive:
            key = entry.name.removeprefix('./')
            if key in rows:
                raise RuntimeError('duplicate archive member: ' + key)
            row = {'type': entry.type.decode('ascii'), 'mode': entry.mode,
                   'size': entry.size, 'linkname': entry.linkname}
            if entry.isfile():
                digest = hashlib.sha256()
                stream = archive.extractfile(entry)
                while chunk := stream.read(1024 * 1024):
                    digest.update(chunk)
                row['sha256'] = digest.hexdigest()
            rows[key] = row
    maps[label] = rows
    (out / ('source_payload_' + label + '.json')).write_text(json.dumps(rows, indent=2))
    print(label, len(rows), flush=True)
old, new = maps['old_arm'], maps['current']
result = {
    'inputs': {label: str(path) for label, path in paths.items()},
    'ignored_metadata': ['mtime', 'uid', 'gid', 'uname', 'gname', 'archive order', 'compression'],
    'only_old': sorted(old.keys() - new.keys()),
    'only_current': sorted(new.keys() - old.keys()),
    'different': [{'path': key, 'old': old[key], 'current': new[key]}
                  for key in sorted(old.keys() & new.keys()) if old[key] != new[key]],
    'seconds': time.monotonic() - start,
}
(out / 'source_payload_comparison.json').write_text(json.dumps(result, indent=2))
print(json.dumps({k: len(result[k]) for k in ['only_old', 'only_current', 'different']}), flush=True)
print('seconds', result['seconds'], flush=True)
