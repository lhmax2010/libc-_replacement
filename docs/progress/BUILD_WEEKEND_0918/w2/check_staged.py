"""Check exact staged scope and common secret patterns; never read credentials."""
import gzip,hashlib,json,re,subprocess
from pathlib import Path
paths=[x for x in subprocess.check_output(['git','diff','--cached','--name-only','-z']).decode().split('\0') if x]
assert paths and all(x.startswith('docs/progress/BUILD_WEEKEND_0918/') for x in paths)
delivery=Path('docs/progress/BUILD_WEEKEND_0918')
indexed={x for x in subprocess.check_output(['git','ls-files','--cached','-z','--',str(delivery)]).decode().split('\0') if x}
present={str(x) for x in delivery.rglob('*') if x.is_file()}
assert present<=indexed,('Delivery files missing from Git index',sorted(present-indexed))
manifest_checks=[]
for manifest in sorted(delivery.rglob('MANIFEST.json')):
    for entry in json.loads(manifest.read_text())['files']:
        data=Path(entry['delivery']).read_bytes()
        original=gzip.decompress(data) if entry['gzip'] else data
        assert len(original)==entry['bytes_uncompressed'],entry['delivery']
        assert hashlib.sha256(original).hexdigest()==entry['sha256_uncompressed'],entry['delivery']
    manifest_checks.append(str(manifest))
rows=[]
for name in paths:
    data=subprocess.check_output(['git','show',':'+name]); assert data==Path(name).read_bytes()
    decoded=gzip.decompress(data) if name.endswith('.gz') else data
    for pattern in (rb'-----BEGIN (?:RSA |OPENSSH |EC )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'curl\s+-u\s+[^\s]+:[^\s]+'):
        assert not re.search(pattern,decoded),name
    rows.append({'path':name,'sha256':hashlib.sha256(data).hexdigest()})
assert not subprocess.check_output(['git','status','--porcelain','--','password.md','.quickbuild-credentials']).strip()
result={'status':'PASS','scope':'docs/progress/BUILD_WEEKEND_0918 only','files':rows,
        'all_delivery_files_indexed':True,'verified_archive_manifests':manifest_checks,
        'credential_contents_read':False,'exact_secret_string_scan':'NOT_OBSERVED; no credential contents read'}
Path('progress/BUILD_WEEKEND_0918/STAGED_CHECK.json').write_text(json.dumps(result,indent=2))
print('PASS staged files',len(rows))
