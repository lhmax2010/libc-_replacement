"""有真实重建 provider 后执行；未提供新产物时明确退出 77，绝不替换为原件。"""
from common import *
import argparse,re
ap=argparse.ArgumentParser();ap.add_argument('--provider',required=True);ap.add_argument('--build-evidence',required=True)
args=ap.parse_args();provider=pathlib.Path(args.provider).resolve();evidence=pathlib.Path(args.build_evidence)
if not provider.is_file() or not evidence.is_file():print('NOT_AVAILABLE: rebuilt real provider and build evidence required');sys.exit(77)
digest=hashlib.sha256(provider.read_bytes()).hexdigest()
if digest==hashlib.sha256(PROVIDER.read_bytes()).hexdigest():print('NOT_AVAILABLE: original provider cannot stand in for rebuilt candidate');sys.exit(77)
manifest=json.loads(evidence.read_text())
assert manifest['provider_sha256']==digest
assert manifest['source_rpm_sha256']=='2ae259fd7f28355b1639d15009a9b66d38de98ab590bd534e29ad1d443a4e121'
assert manifest['patch_sha256']==hashlib.sha256((OUT/'W2/provider-exception-cleanup.CANDIDATE.patch').read_bytes()).hexdigest()
assert manifest['compiler'] and manifest['ninja_commands']
rows=[]
for repeats in [1,10]:
 for n in range(1,6):
  row=execute('W2',f'candidate_4_{repeats}_{n}',TMP/'reproduce',[provider,4,repeats,1]);s=(ROOT/(row['raw']+'.stdout')).read_text()
  assert re.search(r'failures='+str(repeats)+r' live=0 bytes=0',s),row
  rows.append(row)
for n in range(1,6):
 row=execute('W2',f'normal_candidate_{n}',TMP/'normal_app',[provider,provider]);s=(ROOT/(row['raw']+'.stdout')).read_text()
 assert 'SUMMARY EXACT=12/12 DESTROYED=12/12' in s
 rows.append(row)
save('W2','VERIFIED_CANDIDATE_RESULTS.json',rows)
