import csv,gzip,hashlib,json,re,subprocess
from pathlib import Path

p=Path('progress/BUILD_STATIC_0917'); d=Path('docs/progress/BUILD_STATIC_0917')
checks={}
s=json.loads((p/'gnu-gdb-registers.json').read_text())['stdout']
vals=[]
for line in s.splitlines():
    if re.search(r'<dwarf_reg_size_table(?:\+\d+)?>:',line):
        vals += [int(x) for x in line.split(':',1)[1].split()]
checks['table_all_17_bytes_zero']=vals==[0]*17
checks['register_index_zero']=bool(re.search(r'\nrsi\s+0x0\s+0\b',s))
checks['actual_pc_comparison_and_abort_branch']=all(x in s for x in ['cmp    $0x8,%al','_Unwind_SetGR.cold','libgcc_s.so.1'])
for mode in ['wait','system_clock','steady_clock','custom_clock']:
    f='gnu-gdb-abort.json' if mode=='wait' else 'gnu-gdb-abort-'+mode+'.json'
    s=json.loads((p/f).read_text())['stdout']
    checks['backtrace_'+mode]=all(x in s for x in ['SIGABRT','_Unwind_SetGR.cold','__gxx_personality_v0','_Unwind_ForcedUnwind','libgcc_s.so.1'])
checks['main_selfcheck']=json.loads((p/'selfcheck.json').read_text())['all_pass']
staged=subprocess.check_output(['git','diff','--cached','--name-only','-z']).decode().split('\0')
checks['staged_only_task_material']=all(not x or x.startswith('docs/progress/BUILD_STATIC_0917/') for x in staged)
checks['no_large_binary_staged']=all(not x or Path(x).stat().st_size<10_000_000 for x in staged)
bad=[]
if (d/'SHA256SUMS.tsv').exists():
    for row in csv.DictReader((d/'SHA256SUMS.tsv').open(),delimiter='\t'):
        f=d/row['path']
        if not f.exists() or hashlib.sha256(f.read_bytes()).hexdigest()!=row['sha256']: bad.append(row['path'])
checks['archived_hashes_match']=not bad
for name,item in json.loads((d/'COMPRESSED_FILES.json').read_text()).items():
    checks['compressed_'+name]=hashlib.sha256(gzip.decompress((d/item['archive']).read_bytes())).hexdigest()==item['original_sha256']
print(json.dumps(dict(checks=checks,all_pass=all(checks.values()),bad_hashes=bad),indent=2))
raise SystemExit(0 if all(checks.values()) else 1)
