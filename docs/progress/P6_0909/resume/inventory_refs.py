#!/usr/bin/env python3
"""只读扫描本材料仓的跟踪文本；辨识完整及 7–40 位旧 SHA，输出候选，不改文件。"""
import collections
import csv
import pathlib
import re
import subprocess
import sys
base=pathlib.Path('docs/progress/P6_0909/resume')
mapping=list(csv.DictReader((base/'SHA_MAPPING.tsv').open(),delimiter='\t'))
old=[r['old_sha'] for r in mapping]
pattern=re.compile(r'(?<![0-9a-fA-F])([0-9a-f]{7,40})(?![0-9a-fA-F])')
assert pattern.findall('x '+old[0][:8]+' / '+old[0])==[old[0][:8],old[0]]
paths=subprocess.check_output(['git','ls-files','-z']).decode().split('\0')
rows=[];scanned=0;excluded=[]
for name in filter(None,paths):
    path=pathlib.Path(name)
    if not path.is_file():excluded.append([name,'NOT_FILE']);continue
    data=path.read_bytes()
    if b'\0' in data:excluded.append([name,'BINARY_NUL']);continue
    try:text=data.decode('utf-8')
    except UnicodeDecodeError:excluded.append([name,'NOT_UTF8']);continue
    scanned+=1
    if not any(sha[:7] in text for sha in old):continue
    for number,line in enumerate(text.splitlines(),1):
        for match in pattern.finditer(line):
            matches=[r for r in mapping if r['old_sha'].startswith(match[1])]
            if not matches:continue
            assert len(matches)==1
            r=matches[0]
            parts=path.parts
            historical=any(x in parts for x in ['raw','before','snapshots','original_commits','code','delivery','delivery_resume']) or path.suffix in ['.py','.sh','.patch','.cpp','.h','.commit','.message'] or 'SHA256' in path.name or path.name in ['EVIDENCE_INDEX.tsv','SOURCE_PROVENANCE.tsv'] or name.startswith('docs/progress/P6_0909/')
            context=line[max(0,match.start()-160):match.end()+240]
            rows.append([name,number,match[1],r['old_sha'],r['new_sha'][:len(match[1])],'HISTORICAL_OR_EXECUTABLE_REVIEW' if historical else 'CURRENT_TEXT_CANDIDATE',context])
suffix=sys.argv[1] if len(sys.argv)>1 else 'before'
for filename,header,values in [(f'REFERENCES_{suffix}.tsv',['file','line','old_token','old_sha','new_token','classification','context'],rows),(f'SCAN_EXCLUSIONS_{suffix}.tsv',['file','reason'],excluded)]:
    with (base/filename).open('w') as f:
        w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(header);w.writerows(values)
print('TRACKED_TEXT_FILES',scanned,'MATCH_OCCURRENCES',len(rows),'MATCH_FILES',len({r[0] for r in rows}),'POSITIVE_CONTROL=PASS')
for category in ['CURRENT_TEXT_CANDIDATE','HISTORICAL_OR_EXECUTABLE_REVIEW']:
    subset=[r for r in rows if r[5]==category]
    print(category,'FILES',len({r[0] for r in subset}),'OCCURRENCES',len(subset))
    if category=='CURRENT_TEXT_CANDIDATE':
        for name,count in sorted(collections.Counter(r[0] for r in subset).items()):print(count,name)
