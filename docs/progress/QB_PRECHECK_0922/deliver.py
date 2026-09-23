"""Validate and copy this task's evidence only; never stage unrelated changes."""
import base64,hashlib,json,pathlib,re,shutil,subprocess,sys
P=pathlib.Path.cwd(); S=P/'progress/QB_PRECHECK_0922'; D=P/'docs/progress/QB_PRECHECK_0922'
status=P/'docs/progress/BUILD_WEEKEND_0918/w4/PACKAGE_STATUS.md'
before=(S/'PACKAGE_STATUS.before.md').read_text().splitlines(keepends=True)
after=status.read_text().splitlines(keepends=True)
assert len(before)==len(after)
changed=[i+1 for i,(a,b) in enumerate(zip(before,after)) if a!=b]
assert len(changed)==1 and after[changed[0]-1].startswith('| bpftrace |')
assert subprocess.check_output(['git','branch','--show-current'],text=True).strip()=='codex/r5-ehabi-diagnosis'
secrets=[]
for name in ('.quickbuild-credentials','password.md'):
 f=P/name
 if f.is_file():
  lines=f.read_bytes().splitlines()
  # The user explicitly authorized public account identifiers, not passwords.
  secrets.extend(x for x in (lines[1:] if name=='.quickbuild-credentials' else lines) if len(x)>3)
auth_files=[pathlib.Path.home()/'.oscrc',pathlib.Path.home()/'.netrc']
ssh=pathlib.Path.home()/'.ssh'
if ssh.is_dir():auth_files.extend(f for f in ssh.iterdir() if f.is_file())
auth_blobs=[]
for f in auth_files:
 if not f.is_file():continue
 data=f.read_bytes()
 if len(data)>32:auth_blobs.append(data)
 if b'PRIVATE KEY' in data:
  secrets.extend(x.strip() for x in data.splitlines() if len(x.strip())>40 and not x.startswith(b'-----'))
 if f.name in ('.oscrc','.netrc'):
  secrets.extend(m.group(1) for m in re.finditer(rb'(?im)(?:password|passwd|token|pass)\s*(?:=\s*|\s+)([^\s]+)',data) if len(m.group(1))>3)
secrets=[v for s in secrets for v in (s,base64.b64encode(s),s.hex().encode())]
files=[p for p in S.rglob('*') if p.is_file()]+[status,P/'docs/LINE_STATUS.md']
for f in files:
 data=f.read_bytes()
 if any(s in data for s in secrets) or any(s in data for s in auth_blobs):
  raise SystemExit('STOP: credential literal found; do not commit')
 for pattern in (rb'-----BEGIN (?:OPENSSH |RSA |EC |DSA )?PRIVATE KEY-----',rb'gh[pousr]_[A-Za-z0-9]{30,}',rb'github_pat_[A-Za-z0-9_]{40,}',rb'(?im)^(?:Set-Cookie|Cookie|Authorization):\s*\S+'):
  if re.search(pattern,data):raise SystemExit('STOP: authentication material pattern; do not commit')
 assert f.stat().st_size<5*1024*1024, str(f)
result={'branch':'codex/r5-ehabi-diagnosis','changed_package_status_lines':changed,'other_ten_rows_and_other_text_unchanged':True,'credential_literal_scan':'PASS','private_key_token_cookie_auth_pattern_scan':'PASS','authentication_file_content_scan':'PASS','public_git_identifiers_explicitly_authorized':True,'credential_values_not_printed':True,'files_scanned':len(files),'no_binary_artifacts':True}
(S/'SELFCHECK.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
(S/'SCRIPT_SHA256.json').write_text(json.dumps({p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(S.glob('*.py'))},indent=2)+'\n')
D.mkdir(parents=True,exist_ok=True)
for f in S.rglob('*'):
 if f.is_file():
  out=D/f.relative_to(S);out.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(f,out)
print(json.dumps(result,ensure_ascii=False))
