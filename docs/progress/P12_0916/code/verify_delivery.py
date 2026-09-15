#!/usr/bin/env python3
import hashlib,json,pathlib,shutil,subprocess,sys,tempfile
import build as b
patch=b.OUT/'patches';source=b.TMP/'upstream/chromium-efl-1.1.144'
for mode in ['separate','embedded']:
 work=pathlib.Path(tempfile.mkdtemp(prefix='patch_'+mode+'_',dir=b.TMP))
 shutil.copytree(source,work,dirs_exist_ok=True)
 for name in ['0001-route-cookie-to-explicit-adaptor.patch','0002-gn-explicit-link-inputs.patch','0003-spec-'+mode+'-CANDIDATE.patch']:
  b.run('patch_check_'+mode+'_'+name,['git','apply','--check',patch/name],cwd=work)
  b.run('patch_apply_'+mode+'_'+name,['git','apply',patch/name],cwd=work)
config={'compiler':str(b.CC),'gnu_flags':b.flags['gnu'],'libcxx_flags':b.flags['cxx'],'includes':b.inc,'link_flags':b.link,'provider_path':str(b.PROVIDER),'provider_abi_evidence':'SOURCE_INDEX.json + provider SHA256; measured GNU runtime','consumer_abi_evidence':'BUILD_CONFIG.json exact libc++ headers','direction':'reverse-mechanism'}
f=b.TMP/'package_reverse.json';f.write_text(json.dumps(config,indent=2)+'\n')
for mode in ['separate','embedded']:
 b.run('package_candidate_'+mode,[sys.executable,b.TMP/'package_build.py',f,b.TMP/('package_'+mode),'--mode',mode])
for script in ['future_ewk_matrix.py','future_callback_matrix.py']:
 stem=b.OUT/'raw'/('pending_'+script)
 cmd=[sys.executable,str(b.REC),str(stem),sys.executable,str(b.TMP/script),str(b.TMP/'missing-real-artifact-manifest.json'),str(b.TMP/'must-not-create')]
 subprocess.run(cmd)
 assert int(pathlib.Path(str(stem)+'.exitcode').read_text())==77
inputs=[b.PROVIDER,b.CC,b.LIBCXX/'lib/libc++.so.1',b.LIBCXX/'lib/libc++abi.so.1',b.SDK/'closure_ewk/libstdc++.so.6',b.SDK/'closure_ewk/ld-linux-x86-64.so.2']
(b.OUT/'INPUT_ARTIFACTS.json').write_text(json.dumps([{'path':str(p),'resolved':str(p.resolve()),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}for p in inputs],indent=2)+'\n')
print('PATCH_DRY_APPLY=PASS; BOTH_CANDIDATE_BUILDS=PASS; RPM_BUILD=NOT_OBSERVED; PENDING_RUNNERS_EXIT=77')
