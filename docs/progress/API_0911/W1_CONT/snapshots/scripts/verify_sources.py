#!/usr/bin/env python3
"""以冻结源码归档为原始对照，逐文件校验真实 Dali 输入；不写源码树。"""
import pathlib,tarfile,hashlib,json
r=pathlib.Path.cwd();o=r/'docs/progress/API_0911/W1_CONT';corpus=pathlib.Path('/home/toolchain/development/libc++_replacement/tmp/corpus/extracted');rows=[];summaries=[]
for pkg,ext in [('dali2','gz'),('dali2-csharp-binder','xz')]:
 parent=corpus/(pkg+'-2.5.26-1.src.rpm');archive=parent/'payload'/(pkg+'-2.5.26.tar.'+ext);source=parent/'unpacked'/archive.name
 total=0;bad=[];positive=False
 with tarfile.open(archive,'r:*') as f:
  for m in f:
   if not m.isfile():continue
   p=pathlib.PurePosixPath(m.name);assert not p.is_absolute() and '..' not in p.parts
   original=f.extractfile(m).read();actual=source/p;expect=hashlib.sha256(original).hexdigest();got=hashlib.sha256(actual.read_bytes()).hexdigest() if actual.is_file() else None
   rows.append(dict(package=pkg,member=m.name,expected_sha256=expect,actual_sha256=got,equal=got==expect));total+=1
   if got!=expect:bad.append(m.name)
   if not positive and original:positive=hashlib.sha256(bytes([original[0]^1])+original[1:]).hexdigest()!=expect
 summaries.append(dict(package=pkg,archive=str(archive),archive_sha256=hashlib.sha256(archive.read_bytes()).hexdigest(),regular_files=total,mismatches=bad,in_memory_changed_byte_positive_control=positive))
(o/'source_integrity.json').write_text(json.dumps(dict(summary=summaries,files=rows),indent=2)+'\n')
print(json.dumps(summaries,indent=2));assert all(not x['mismatches'] and x['in_memory_changed_byte_positive_control'] for x in summaries)
