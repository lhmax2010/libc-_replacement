"""Check only deliverables; do not read credential files or browser profile."""
import csv, hashlib, json, pathlib, re, subprocess
out=pathlib.Path(__file__).resolve().parent
checks={}
status=list(csv.DictReader((out/'BUILD_STATUS.tsv').open(),delimiter='\t'))
checks['774_unique_cells']=len(status)==774==len({(x['package'],x['architecture']) for x in status})
checks['258_packages']=len({x['package'] for x in status})==258
checks['9_failures']=sum(x['status']=='failed' for x in status)==9
checks['752_successes']=sum(x['status']=='succeeded' for x in status)==752
checks['13_excluded']=sum(x['status']=='excluded' for x in status)==13
checks['12_expected_inputs']=len(list(csv.DictReader((out/'INPUT_COMPARISON.tsv').open(),delimiter='\t')))==12
for row in csv.DictReader((out/'DOWNLOADS.tsv').open(),delimiter='\t'):
    checks['sha:'+row['package']+'/'+row['architecture']]=hashlib.sha256(pathlib.Path(row['local_path']).read_bytes()).hexdigest()==row['sha256']
checks['browser_closed']=json.loads(pathlib.Path('tmp/QB_1194606/browser-status.json').read_text())['state']=='CLOSED_PROFILE_RETAINED'
result=subprocess.run(['git','check-ignore','tmp/QB_1194606/browser-profile/','tmp/QB_1194606/downloads/logs/'],capture_output=True,text=True)
checks['private_artifacts_ignored']=result.returncode==0 and len(result.stdout.splitlines())==2
checks['project_branch']=subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/r5-ehabi-diagnosis'
# Generic credential-shape checks, not a comparison against secret files.
patterns={
 'private_key':r'-----BEGIN (?:OPENSSH |RSA |EC |DSA )?PRIVATE KEY-----',
 'authorization_header':r'(?im)^\s*(?:authorization|proxy-authorization)\s*:\s*(?:Basic|Bearer)\s+\S+',
 'session_header':r'(?im)^\s*(?:set-cookie|cookie)\s*:',
 'embedded_url_password':r'(?i)https?://[^\s/:]+:[^\s/@]+@',
 'token_literal':r'(?i)\b(?:ghp_|github_pat_|sk-proj-)[a-zA-Z0-9_\-]{20,}',
}
findings=[]
for p in sorted(out.rglob('*'))+[pathlib.Path('docs/LINE_STATUS.md')]:
    if not p.is_file() or p.name=='SELFCHECK.json': continue
    text=p.read_text(errors='replace')
    for label,pattern in patterns.items():
        if re.search(pattern,text): findings.append({'path':str(p),'kind':label})
checks['no_credential_shapes']=not findings
staged=subprocess.check_output(['git','diff','--cached','--name-only','-z']).decode().split('\0')
staged=[p for p in staged if p]
checks['staged_scope_only']=all(p.startswith('docs/progress/QB_1194606/') or p=='docs/LINE_STATUS.md' for p in staged)
for p in staged:
    content=subprocess.check_output(['git','show',':'+p]).decode(errors='replace')
    for label,pattern in patterns.items():
        if re.search(pattern,content): findings.append({'path':'INDEX:'+p,'kind':label})
checks['no_staged_credential_shapes']=not findings
report={'checks':checks,'findings_paths_only':findings,'credential_files_read':False,'scope':'this evidence directory and LINE_STATUS only; generic pattern scan plus manual review'}
(out/'SELFCHECK.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(report,ensure_ascii=False,indent=2))
assert all(checks.values())
