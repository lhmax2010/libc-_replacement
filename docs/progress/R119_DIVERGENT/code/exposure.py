#!/usr/bin/env python3
from common import *
import collections,re
gate('exposure_gate')
rows=json.loads((OUT/'TYPE_RESULTS.json').read_text())
hits=json.load(gzip.open(OUT/'HEADER_HITS.json.gz','rt'))
byid=collections.defaultdict(list)
for h in hits:
 for id in h['type_candidates']:byid[id].append(h)
selected={}
tokens=['ChecksumStream','PdfInputDevice','Coordinates','FromChrono','getLastModified','DictValue']
for h in hits:
 if not any(t in h['context'] or any(t in x['path'] for x in h['references']) for t in tokens):continue
 ref=h['references'][0];p=pathlib.Path(ref['cache_path'])
 if str(p) in selected:continue
 assert sha(p)==ref['file_sha256']
 selected[str(p)]=dict(path=str(p),sha256=sha(p),references=h['references'],text='\n'.join(f'{i+1}: {s}' for i,s in enumerate(p.read_text(errors='replace').splitlines())))
save(OUT/'EXPOSURE_SELECTED_HEADERS.json',selected)
for v in selected.values():
 print(v['references'][0]['path'])
 for line in v['text'].splitlines():
  if any(t in line for t in ['streamoff','fmtflags','openmode','seekdir','Tell(','Tell (','Seek(','Seek (','const_iterator','FromChrono','getLastModified']):print(line)
print('selected_headers',len(selected))
direct=[]
for r in rows:
 if r['result']!='DIVERGENT':continue
 expression=re.sub(r'\s+','',r['expression'])
 matches=[h for h in byid[r['id']] if expression in re.sub(r'\s+','',h['text'])]
 direct.append(dict(id=r['id'],expression=r['expression'],qualified_root_positions=len(byid[r['id']]),exact_spelling_positions=len(matches),matches=matches))
save(OUT/'DIRECT_EXPOSURE_HITS.json',direct)
print('DIRECT_COUNTS')
for r in direct:
 if r['exact_spelling_positions']:print(r['id'],r['expression'],r['exact_spelling_positions'])
