#!/usr/bin/env python3
"""对低命中候选追加不要求 std:: 前缀的检索，处理 using/namespace 别名漏检。"""
import collections,csv,gzip,hashlib,json,pathlib,re,time
root=pathlib.Path(__file__).resolve().parents[4];base=root/'docs/progress/R119_batch2'
start=time.monotonic();words=['smatch','match_results','future_status','directory_iterator','fstream','basic_fstream','wstring_convert']
pattern=re.compile(r'\b(?:'+'|'.join(words)+r')\b')
assert pattern.search('using std::smatch;') and not pattern.search('smatches')
with (root/'docs/progress/P7_0909/tables/packages.tsv').open() as f:
    devel={r['checksum'] for r in csv.DictReader(f,delimiter='\t') if r['devel_name']=='YES'}
stats=collections.Counter();seen=set();rows=collections.defaultdict(list);issues=[]
with gzip.open(root/'docs/progress/P7_0909/tables/headers.tsv.gz','rt') as f:
    for r in csv.DictReader(f,delimiter='\t'):
        if r['rpm_sha256'] not in devel or r['scope']!='INCLUDE_TREE':continue
        stats['selected_records']+=1
        if r['status']!='PASS':issues.append(dict(path=r['path'],reason=r['status']));continue
        key=r['file_sha256']
        if key in seen:continue
        seen.add(key);p=pathlib.Path(r['cache_path']);data=p.read_bytes();assert hashlib.sha256(data).hexdigest()==key
        stats['read_unique_contents']+=1;lines=data.decode(errors='replace').splitlines()
        for i,line in enumerate(lines):
            for name in set(pattern.findall(line)):
                rows[name].append(dict(metadata=r,line=i+1,context='\n'.join(f'{j+1}: {lines[j]}' for j in range(max(0,i-3),min(len(lines),i+4)))))
for name,items in rows.items():
    path=base/'W1/unqualified';path.mkdir(exist_ok=True)
    (path/(name+'.json')).write_text(json.dumps(items,ensure_ascii=False,indent=2)+'\n')
    print('\nTOKEN',name,'count',len(items))
    for item in items:
        m=item['metadata']
        if '/c++/' in m['path']:continue
        print(m['sourcerpm'],m['path'],item['context'],sep='\n')
summary=dict(stats=stats,issues=issues,counts={n:len(rows[n]) for n in words},seconds=time.monotonic()-start,positive_control='using std::smatch;',negative_control='smatches',limit='词面检索含注释，逐条需人工判断；同内容去重保留首份来源，不代表只有一个包。仅冻结安装头，不作全平台所有条件编译的断言。')
(base/'W1/unqualified/SUMMARY.json').write_text(json.dumps(summary,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(summary,ensure_ascii=False))
