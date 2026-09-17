"""手工核对后的确切两步包含，不使用 basename 推导包含链。"""
from common import *
import json
p=OUT/'W1/positive/absl_atomic.json'
obj=json.loads(p.read_text())
src=OLD/'tmp/corpus/extracted/grpc-1.76.0-0.src.rpm/unpacked/v1.76.0.tar.gz/grpc-1.76.0/src/core/util/sync_abseil.cc'
header=pathlib.Path(obj['header_matches'][0]['metadata']['cache_path'])
include_root=next(x for x in header.parents if x.name=='include')
call_once=include_root/'absl/base/call_once.h'
records=[]
for file,needle in [(src,'#include "absl/base/call_once.h"'),(call_once,'#include "absl/base/internal/spinlock_wait.h"')]:
    lines=file.read_text().splitlines()
    hits=[i for i,s in enumerate(lines)if s.strip()==needle]
    assert len(hits)==1,(file,hits)
    i=hits[0]
    records.append({'path':str(file),'sha256':digest(file),'line':i+1,'text':lines[i], 'context':'\n'.join(f'{j+1}: {lines[j]}'for j in range(max(0,i-3),min(len(lines),i+4)))})
records.append({'path':str(header),'sha256':digest(header),'declaration':obj['header_matches'][0],'elf_record':'W1/positive/absl_atomic.json','limit':'静态源码包含链与真实 ELF UND/DEF 相交；不是预处理器执行跟踪，也不是运行覆盖。'})
save(OUT/'W1/positive/absl_atomic_include_chain.json',records)
index=[]
for file in sorted(p.parent.glob('*.json')):
    x=json.loads(file.read_text())
    if isinstance(x,dict)and'spec'in x:index.append({'key':x['spec']['key'],'headers':len(x['header_matches']),'elfs':len(x['elf_matches']),'includes':len(x['source_includes'])})
save(p.parent/'INDEX.json',index)
print(json.dumps(records,ensure_ascii=False,indent=2))
