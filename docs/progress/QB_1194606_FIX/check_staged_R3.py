"""Check staged scope, evidence size, and common credential formats without printing matches."""
import json,pathlib,re,subprocess
E=pathlib.Path(__file__).resolve().parent
paths=subprocess.check_output(['git','diff','--cached','--name-only','-z']).decode().strip('\0').split('\0')
allowed=['docs/progress/QB_1194606_FIX/','docs/LINE_STATUS.md','docs/progress/R101/tables/base_all_source_packages.tsv','docs/progress/BASE_LEDGER_0921/CEREAL_CORRECTION_1194606_R3.md']
bad=[];large=[];secrets=[]
patterns=[rb'-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----',rb'Authorization:\s*(?:Basic|Bearer)\s+\S+',rb'gh[pousr]_[A-Za-z0-9]{30,}',rb'(?im)^Set-Cookie:\s*\S+',rb'(?im)^Cookie:\s*\S+',rb'(?im)^(?:password|passwd|token)\s*=\s*[^\s#]+']
for p in paths:
    if not any(p.startswith(a) if a.endswith('/') else p==a for a in allowed):bad.append(p)
    data=subprocess.check_output(['git','show',':'+p])
    if len(data)>10*1024**2:large.append(p)
    if any(re.search(pattern,data) for pattern in patterns):secrets.append(p)
result=dict(staged_files=len(paths),out_of_scope=bad,over_10MB=large,credential_pattern_paths=secrets,public_identifiers_not_credentials=True,scope='Only staged files; no credential files read; pattern scan is not a proof against arbitrary secrets')
(E/'STAGED_CHECK_R3.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2));assert not bad and not large and not secrets
