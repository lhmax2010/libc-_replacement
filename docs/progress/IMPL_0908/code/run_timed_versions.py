#!/usr/bin/env python3
"""两架构真实新运行库中的旧/新 system-clock 包装，每格五次。"""
import csv,pathlib,re,subprocess,sys
r=pathlib.Path.cwd();rows=[]
for arch in ['x86_64','armv7l']:
 for binary,expected,version in [('old_timed',86,'LLVM_22'),('wait',0,'LLVM_22_TIZEN_1')]:
  symbol_stem=r/f'docs/progress/IMPL_0908/W3/raw/timed_versions/{arch}_{binary}_symbols'
  subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(symbol_stem),'readelf','--dyn-syms','--wide',str(r/'tmp/IMPL_0908'/('probes-'+arch)/binary)],check=True)
  symbols=pathlib.Path(str(symbol_stem)+'.stdout').read_text()
  assert any('condition_variable15__do_timed_wait' in line and '@'+version+' ' in line for line in symbols.splitlines())
  for repeat in range(1,6):
   stem=r/f'docs/progress/IMPL_0908/W3/raw/timed_versions/{arch}_{binary}_{repeat}'
   if arch=='armv7l':
    leaf=f'env LD_LIBRARY_PATH=/var/tmp/impl_0908/new EXPECTED_RUNTIME=/var/tmp/impl_0908/new nice -n 15 ionice -c 3 /var/tmp/impl_0908/probes/{binary} system_clock'
    cmd=['timeout','40','/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101','shell','ulimit -c 0; '+leaf+'; rc=$?; printf "REMOTE_EXIT=%d\\n" "$rc"']
   else:
    lib=r/'tmp/IMPL_0908/build-native/lib'
    cmd=['nice','-n','15','ionice','-c','3','timeout','35','env','LD_LIBRARY_PATH='+str(lib),'EXPECTED_RUNTIME='+str(lib),str(r/'tmp/IMPL_0908/probes-x86_64'/binary),'system_clock']
   subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),*cmd])
   host=int(pathlib.Path(str(stem)+'.exitcode').read_text());text=pathlib.Path(str(stem)+'.stdout').read_text()
   if arch=='armv7l':
    m=re.search(r'REMOTE_EXIT=(\d+)',text)
    if host or not m:raise SystemExit('CONNECTION_FAILURE: STOP')
    rc=int(m[1])
   else:rc=host
   assert rc==expected and 'RUNTIME_IDENTITY=PASS' in text
   assert ('event=terminate' if expected else 'canceled=1 cleanup_count=1') in text
   rows.append([arch,'物理板' if arch=='armv7l' else '原生',binary,version,repeat,rc,'PASS',str(stem)])
   with (r/'docs/progress/IMPL_0908/W3/timed_versions.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['arch','environment','binary','import_version','repeat','exit','value_assert','evidence']);w.writerows(rows)
print('TIMED_VERSION_MATRIX=PASS cells=4 runs=20')
