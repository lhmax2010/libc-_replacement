"""一次串行读取冻结源码文件清单，索引显式 include；不声称解析宏或构建。"""
from common import *
import csv,gzip,re,hashlib,time,collections
csv.field_size_limit(20_000_000)
manifest=ROOT/'docs/progress/R117/platform/files.tsv.gz'
dest=OUT/'W1/source_includes';dest.mkdir(exist_ok=True)
pattern=re.compile(rb'^\s*#\s*include\s*[<"]([^>"\r\n]+)[>"]',re.M)
positive=b'#include "bundle_cpp.h"\n # include <absl/base/call_once.h>\n'
negative=b'// #include "not_a_header.h"\n'
assert [m[1]for m in pattern.finditer(positive)]==[b'bundle_cpp.h',b'absl/base/call_once.h']
assert not list(pattern.finditer(negative))
save(dest/'CONTROLS.json',{'positive':positive.decode(),'negative':negative.decode(),'status':'PASS','limit':'词法显式 include 候选；未剔除块注释或 #if 停用分支，不能单独作为构建采用证明。'})
start=time.monotonic();counts=collections.Counter();packages=collections.Counter();issues=[]
with gzip.open(manifest,'rt')as f,gzip.open(dest/'index.tsv.gz','wt',newline='')as of:
    w=csv.writer(of,delimiter='\t',lineterminator='\n');w.writerow(['source_rpm','path','line','include','sha256'])
    for r in csv.DictReader(f,delimiter='\t'):
        if r['status']!='READ':counts['manifest_not_read']+=1;continue
        path=OLD/'tmp/corpus/extracted'/r['source_rpm']/r['path']
        try:
            data=path.read_bytes();actual=hashlib.sha256(data).hexdigest()
        except OSError as e:
            issues.append({'path':str(path),'error':str(e)});continue
        if actual!=r['sha256']:
            issues.append({'path':str(path),'error':'SHA256_CHANGED','expected':r['sha256'],'actual':actual});continue
        counts['files_read']+=1;counts['bytes_read']+=len(data);packages[r['source_rpm']]+=1
        for m in pattern.finditer(data):
            w.writerow([r['source_rpm'],r['path'],data.count(b'\n',0,m.start())+1,m[1].decode('utf-8','replace'),actual]);counts['includes']+=1
        if counts['files_read']%10000==0:
            save(dest/'STATUS.json',{'counts':counts,'packages_seen':len(packages),'seconds':time.monotonic()-start,'complete':False})
            print('READ',counts['files_read'],'PACKAGES',len(packages),'SECONDS',round(time.monotonic()-start),flush=True)
        if time.monotonic()-start>3*3600:
            issues.append({'error':'THREE_HOUR_INDEX_CAP','last_path':str(path)});break
    else:counts['manifest_exhausted']=1
save(dest/'STATUS.json',{'manifest':str(manifest),'manifest_sha256':digest(manifest),'counts':counts,'packages':packages,'issues':issues,'seconds':time.monotonic()-start,'complete':bool(counts['manifest_exhausted'])})
print('DONE',dict(counts),'ISSUES',len(issues),flush=True)
