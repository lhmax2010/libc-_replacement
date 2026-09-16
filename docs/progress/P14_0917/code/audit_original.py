from common import *
import re
rows=json.loads((OUT/'W2/ORIGINAL_RESULTS.json').read_text())
verified=[]
for row in rows:
 p=ROOT/(row['raw']+'.stdout');s=p.read_text();assert hashlib.sha256(p.read_bytes()).hexdigest()==row['stdout_sha256']
 line=re.search(r'^SUMMARY (.+)$',s,re.M)[1]
 values={k:int(v) for k,v in re.findall(r'(\w+)=(\d+)',line)}
 assert 'MAP_ASSERT gnu=1 cxx=0 provider=1' in s and 'TRACE_OVERFLOW=0' in s
 n=values['repeats']
 if values['nth']==0:assert values['success']==n and values['exact']==1 and values['live']==values['bytes']==values['failures']==0
 else:assert values['caught']==values['failures']==n and values['live']==3*n and values['bytes']==834*n and values['unchanged']==1
 verified.append({'raw':row['raw'],**values})
save('W2','ASSERTED_ORIGINAL_RESULTS.json',verified)
print('SPECIFIC_VALUE_ASSERTIONS_PASS',len(verified))
