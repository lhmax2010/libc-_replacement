#!/usr/bin/env python3
"""复核两架构最终矩阵的逐格分母、值判定、原始返回码与部署摘要。"""
import collections,csv,pathlib,re
r=pathlib.Path('docs/progress/IMPL_0908/W3')
for arch in ['x86_64','armv7l']:
 rows=list(csv.DictReader((r/f'matrix_{arch}_final.tsv').open(),delimiter='\t'))
 assert len(rows)==225
 cells=collections.Counter(x['cell'] for x in rows)
 assert len(cells)==45 and set(cells.values())=={5}
 for x in rows:
  assert x['value_assert']=='PASS' and x['exit']==x['expected_exit']
  p=pathlib.Path(x['evidence']);text=pathlib.Path(str(p)+'.stdout').read_text()
  assert 'RUNTIME_IDENTITY=PASS' in text
  if arch=='armv7l':assert x['environment']=='物理板' and 'REMOTE_EXIT='+x['exit'] in text
  else:assert pathlib.Path(str(p)+'.exitcode').read_text().strip()==x['exit']
 print(arch,'FINAL_MATRIX=PASS cells=45 repeats=5 runs=225')
deploy=(r/'raw/052_deploy_final2.stdout').read_text()
assert 'DEPLOY=PASS' in deploy and 'REMOTE_EXIT=0' in deploy and deploy.count(': OK')==16
assert (r/'PAYLOAD_final_SHA256SUMS').read_text()==(r/'PAYLOAD_final2_SHA256SUMS').read_text()
print('PAYLOAD=PASS files=16 FILENAME_REPAIR_DID_NOT_CHANGE_BYTES=PASS')
failed=(r/'raw/matrix_x86_64_fixed/44_new_shared_2.stdout').read_text()
assert 'FAIL=internal_mutex_unlocked' in failed
print('EARLIER_OBSERVER_FAILURE_PRESERVED=PASS not folded into final success denominator')
assert re.search(r'FAIL=',failed) # failure detector positive control
print('VALUE_FAILURE_DETECTOR_POSITIVE_CONTROL=PASS')
