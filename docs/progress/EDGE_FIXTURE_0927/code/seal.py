"""最终文件清单；交付记录另存，避免自引用摘要。"""
from common import *
gate()
paths=[]
for p in sorted(OUT.rglob('*')):
 if not p.is_file() or '__pycache__' in p.parts or p.name=='SHA256SUMS' or p.name=='DELIVERY.md' or 'delivery' in p.relative_to(OUT).parts:continue
 # This wrapper's own stdout/exit/time are not complete until it returns.
 if '_seal_manifest' in p.name:continue
 paths.append(p)
(OUT/'SHA256SUMS').write_text(''.join(sha(p)+'  '+str(p.relative_to(OUT))+'\n' for p in paths))
print('sealed_files',len(paths))
