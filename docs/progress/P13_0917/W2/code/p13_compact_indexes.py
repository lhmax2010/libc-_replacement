#!/usr/bin/env python3
"""索引不重复存 maps；原始 stdout/stderr 保持逐字不动。"""
import hashlib,json,pathlib,sys
root=pathlib.Path(sys.argv[1]);names=sys.argv[2:]
for name in names:
 p=root/name;rows=json.loads(p.read_text());before=hashlib.sha256(p.read_bytes()).hexdigest()
 for row in rows:
  for stream in ['stdout','stderr']:
   raw=root/'raw'/(row['raw']+'.'+stream)
   assert row[stream]==raw.read_text(),raw
   row[stream+'_sha256']=hashlib.sha256(raw.read_bytes()).hexdigest()
  row['value_lines']=[s for s in row['stdout'].splitlines()if s.startswith(('MAP_ASSERT','ALLOC','ERROR','RECOVERY','EXPECTED','ACTUAL'))]
  del row['stdout'];del row['stderr']
 p.write_text(json.dumps(rows,indent=2)+'\n')
 print(name,'rows',len(rows),'prior_index_sha256',before,'raw_streams_unchanged=YES')
