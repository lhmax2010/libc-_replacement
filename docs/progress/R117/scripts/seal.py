from common import *
import csv
def digest(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for chunk in iter(lambda:f.read(1024*1024),b''):h.update(chunk)
 return h.hexdigest()
inputs=[ROOT/'docs/progress/T1/extraction_results.tsv',ROOT/'docs/progress/T1/scan_status.tsv',ROOT/'docs/progress/R32/tables/scan_status.tsv',ROOT/'docs/progress/R66/tables/risk_inventory.tsv',ROOT/'docs/progress/P9_0911/baseline/abi_layout_comparison.tsv',ROOT/'HQ_files/abi_comparison_preliminary.md',ROOT/'review_libc++_libstdc++_adaptor.txt',LLVM/'__config',GCC.parent.parent/'gcc/BASE-VER',pathlib.Path(CC),gnu_config/'bits/c++config.h',ROOT/'codes/llvm/clang/bindings/python/clang/cindex.py']
with (OUT/'INPUTS.tsv').open('w',newline='') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['path','bytes','sha256','status'])
 for p in inputs:w.writerow([str(p),p.stat().st_size if p.is_file() else '',digest(p) if p.is_file() else '', 'PRESENT' if p.is_file() else 'NOT_OBSERVED'])
with (OUT/'SCRIPT_SHA256.tsv').open('w',newline='') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['path','bytes','sha256'])
 for p in sorted((OUT/'scripts').rglob('*')):
  if p.is_file():w.writerow([str(p.relative_to(OUT)),p.stat().st_size,digest(p)])
with (OUT/'SHA256SUMS.tsv').open('w',newline='') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['path','bytes','sha256'])
 for p in sorted(OUT.rglob('*')):
  if p.is_file() and p!=OUT/'SHA256SUMS.tsv':w.writerow([str(p.relative_to(OUT)),p.stat().st_size,digest(p)])
print('SEALED',len(list(OUT.rglob('*'))),'paths')
