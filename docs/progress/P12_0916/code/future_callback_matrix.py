#!/usr/bin/env python3
"""真实宿主待提供后的驱动协议；不是伪造的 XW 实例/回调实现。"""
import argparse,json,pathlib,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('manifest');p.add_argument('output');a=p.parse_args()
if not pathlib.Path(a.manifest).is_file():print('NOT_AVAILABLE: real registered XW host/driver');sys.exit(77)
c=json.loads(pathlib.Path(a.manifest).read_text())
if not c.get('host_command') or not c.get('real_provider_path'):print('NOT_AVAILABLE: host_command / real_provider_path');sys.exit(77)
out=pathlib.Path(a.output);out.mkdir(parents=True,exist_ok=False)
samples=[[],[0],[255],list(range(256)),[i%251 for i in range(65536)]]
for n in range(1,6):
 for i,sample in enumerate(samples):
  stem=out/('%d_%d'%(n,i));request={'instance_case':i,'bytes':sample}
  stem.with_suffix('.request.json').write_text(json.dumps(request)+'\n')
  stem.with_suffix('.command.json').write_text(json.dumps(c['host_command'])+'\n')
  r=subprocess.run(c['host_command'],input=json.dumps(request).encode(),stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=120)
  stem.with_suffix('.stdout').write_bytes(r.stdout);stem.with_suffix('.stderr').write_bytes(r.stderr);stem.with_suffix('.exitcode').write_text(str(r.returncode)+'\n')
  assert r.returncode==0
  event=json.loads(r.stdout)
  # The supplied real-host driver must obtain these from the actual registered
  # SetSyncBinaryReply callback, not a substitute vector receiver.
  assert event['actual_bytes']==sample and event['actual_length']==len(sample)
  assert event['source_destroyed'] and event['receiver_released'] and event['actual_callback']=='SetSyncBinaryReply'
  maps=event['proc_self_maps'];assert c['real_provider_path'] in maps and 'libc++.so.' in maps and 'libstdc++.so.' in maps
print('25 real-host data observations passed; registration/driver provenance must also be reviewed')
