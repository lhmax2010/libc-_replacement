"""资源中止后的证据归档：串行、固定 1 MiB 缓冲，不重扫源码。"""
from common import *
import gzip,json,shutil,time
d=OUT/'W1/entity_full';backup=TMP/'full_context_cache';backup.mkdir(exist_ok=True,parents=True)
rc,txt,raw=record('tracked_context_files',['git','ls-files','--','docs/progress/R119_batch1/W1/entity_full']);assert rc==0
tracked=set(txt.splitlines());names=json.loads((d/'COUNTS.json').read_text());items=[]
for name in names:
    p=d/(name.replace('::','_')+'.json')
    if not p.exists():continue
    dest=p.with_suffix('.json.gz')
    with p.open('rb')as src,gzip.open(dest,'wb',compresslevel=1)as out:shutil.copyfileobj(src,out,1024*1024)
    h=digest(p);item={'entity':name,'path':str(dest.relative_to(OUT)),'sha256':digest(dest),'uncompressed_sha256':h,'rows':names[name]}
    if str(p.relative_to(ROOT))not in tracked:
        target=backup/p.name
        assert not target.exists(),target
        p.rename(target);item['local_uncompressed_backup']=str(target)
    items.append(item)
save(d/'ARCHIVE.json',{'scope':'已收集的限定名上下文；归档不代表已逐条完成语义定界。','items':items})
print('ARCHIVED',len(items),'BUFFER_BYTES',1024*1024)
