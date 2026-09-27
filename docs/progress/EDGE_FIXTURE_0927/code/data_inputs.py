from common import *
catalog=json.loads((TMP/'catalog.json').read_text())
names=['manifest-parser-examples','manifest-parser-tests','cert-svc-test','cert-svc-test-binaries']
rows=[]
for name in names:
 matches=[r for r in catalog if r['name']==name and r['arch']=='x86_64'];assert len(matches)==1
 rows.append(matches[0])
save('EXTRA_DATA_RPMS.json',rows)
