#!/usr/bin/env python3
import hashlib,pathlib,shutil,subprocess,json
import p13_driver as d
b=d.b
for mode in ['separate','embedded']:
 source=b.TMP/('package_P13_'+mode);target=b.OUT/'package_evidence'/mode;target.mkdir(parents=True,exist_ok=True)
 for p in source.iterdir():
  if p.is_file() and (p.suffix in ['.json','.stdout','.stderr','.exitcode','.args']):shutil.copyfile(p,target/p.name)
test=subprocess.check_output(['nm','-D','--defined-only',str(b.BUILD/'ewk_adaptor_reverse.so')],text=True)
assert 'p13_test_configure' in test and 'p12_test_fail_field' in test
result={'positive_control':test,'release':{}}
for name in ['ewk_adaptor_reverse.so','forwarder_separate.so','forwarder_embedded.so']:
 p=b.BUILD/'release'/name;symbols=subprocess.check_output(['nm','-D','--defined-only',str(p)],text=True)
 assert 'p13_test_' not in symbols and 'p12_test_' not in symbols
 result['release'][name]={'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'exports':symbols}
(b.OUT/'RELEASE_EXPORT_CHECK.json').write_text(json.dumps(result,indent=2)+'\n')
dirty={'stderr':'94f75a90010d49a676335c1caca88ff04cb1b0a1294bdbb655f38d865eb7a18c','stdout':'dde4c0b37000f6e3d01bd5096bba7ee331a31b216bd43dcbee24f7f2a9a9cf7c'}
for extension,h in dirty.items():assert hashlib.sha256((b.ROOT/'docs/progress/R115/W3/raw'/('033_angle_files.'+extension)).read_bytes()).hexdigest()==h
print('PACKAGE_TEXT_EVIDENCE_COPIED; RELEASE_EXPORT_POSITIVE_CONTROL_PASS; PREEXISTING_DIRTY_FILES_UNCHANGED')
