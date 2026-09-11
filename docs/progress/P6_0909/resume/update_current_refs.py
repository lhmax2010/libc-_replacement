#!/usr/bin/env python3
"""仅机械更新明确的说明文档 SHA；保留历史原始记录和校验清单，逐处记录前后文本。"""
import csv
import hashlib
import pathlib
import re
import sys
base=pathlib.Path('docs/progress/P6_0909/resume')
csv.field_size_limit(sys.maxsize)
mapping=list(csv.DictReader((base/'SHA_MAPPING.tsv').open(),delimiter='\t'))
candidates=list(csv.DictReader((base/'REFERENCES_before.tsv').open(),delimiter='\t'))
names=sorted({r['file'] for r in candidates if r['classification']=='CURRENT_TEXT_CANDIDATE' and r['file'].endswith('.md')})
assert len(names)==12
pattern=re.compile(r'(?<![0-9a-fA-F])([0-9a-f]{7,40})(?![0-9a-fA-F])')
changes=[];files=[]
for name in names:
    path=pathlib.Path(name);original=path.read_bytes();text=original.decode()
    snap=base/'before_current'/name;snap.parent.mkdir(parents=True,exist_ok=True);snap.write_bytes(original)
    updated=[]
    for number,line in enumerate(text.splitlines(keepends=True),1):
        def replace(m):
            hits=[r for r in mapping if r['old_sha'].startswith(m[1])]
            if not hits:return m[1]
            assert len(hits)==1
            new=hits[0]['new_sha'][:len(m[1])]
            changes.append([name,number,m[1],new,line.rstrip('\n')])
            return new
        updated.append(pattern.sub(replace,line))
    result=''.join(updated).encode()
    assert result!=original
    path.write_bytes(result)
    files.append([name,hashlib.sha256(original).hexdigest(),hashlib.sha256(result).hexdigest()])
for name,header,rows in [('REFERENCE_EDITS.tsv',['file','old_line','old_token','new_token','before_context'],changes),('REFERENCE_FILES.tsv',['file','before_sha256','after_sha256_at_replacement'],files)]:
    with (base/name).open('w') as f:
        w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(header);w.writerows(rows)
print('CURRENT_MARKDOWN_FILES',len(files),'SHA_OCCURRENCES_CHANGED',len(changes))
