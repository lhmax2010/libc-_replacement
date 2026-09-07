#!/usr/bin/env python3
"""流式扫描固定版本源码归档，不展开整个 Chromium 工作树。"""
import collections,csv,hashlib,pathlib,re,sys,tarfile
archive=pathlib.Path(sys.argv[1]);root=pathlib.Path('docs/progress/R116');rows=[];counts=collections.Counter();positive=[]
exts={'.c','.cc','.cpp','.cxx','.h','.hh','.hpp','.hxx','.m','.mm','.gn','.gni','.md','.txt','.py','.sh','.bzl'}
terms=['pthread_cancel','pthread_setcanceltype','PTHREAD_CANCEL_ASYNCHRONOUS','std::shared_mutex','std::shared_timed_mutex','wbuffer_convert']
dest=root/'sources/corpus_matches';dest.mkdir(exist_ok=True)
with tarfile.open(archive,'r|gz') as tar:
 for member in tar:
  counts['archive_members']+=1
  if not member.isfile():continue
  counts['regular_files']+=1
  if pathlib.PurePosixPath(member.name).suffix not in exts and pathlib.PurePosixPath(member.name).name not in ['DEPS','LICENSE','README']:
   counts['not_selected_extension']+=1;continue
  if member.size>16*1024*1024:counts['oversize_text_files']+=1;continue
  data=tar.extractfile(member).read()
  if b'\0' in data:counts['binary_selected_extension']+=1;continue
  counts['scanned_text_files']+=1;text=data.decode('utf-8',errors='replace');matched=False
  if member.name=='base/threading/thread.cc':
   positive=[x for x in ['StopSoon','Stop','PostTask'] if x in text]
  for number,line in enumerate(text.splitlines(),1):
   for term in terms:
    if term in line:rows.append([term,member.name,number,line.strip()]);matched=True
   if re.search(r'#\s*include\s*[<"]boost/',line):rows.append(['boost_include',member.name,number,line.strip()]);matched=True
  if matched:
   key=hashlib.sha256(member.name.encode()).hexdigest()[:16];(dest/(key+'.txt')).write_bytes(data)
   (dest/(key+'.path')).write_text(member.name+'\nsha256='+hashlib.sha256(data).hexdigest()+'\n')
with (root/'chromium_usage.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['term','path','line','text']);w.writerows(rows)
for key,value in sorted(counts.items()):print(key+'='+str(value))
for term in terms+['boost_include']:print('hits',term,sum(x[0]==term for x in rows),'files',len({x[1] for x in rows if x[0]==term}))
assert len(positive)==3;print('positive_control_thread_source=PASS')
