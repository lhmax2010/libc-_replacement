"""把人工选取的声明与冻结的真实 ELF / 源码包含记录关联；不运行 ELF。"""
from common import *
import csv,gzip,json,collections,subprocess,re,time
csv.field_size_limit(20_000_000)
dest=OUT/'W1/positive'; dest.mkdir(exist_ok=True)
specs=json.loads((OUT/'code/positive_specs.json').read_text())
if len(sys.argv)>1:specs=[s for s in specs if s['key']in sys.argv[1:]]
business=json.loads((OUT/'W1/EXPANDED_BUSINESS_QUEUE.json').read_text())
names={}
with (ROOT/'docs/progress/P7_0909/inputs/source_package_records.tsv').open()as f:
    for r in csv.DictReader(f,delimiter='\t'):names[r['location']]=r['name']
inventory=collections.defaultdict(list)
with (OLD/'progress/R11/tables/elf_inventory.tsv').open()as f:
    for r in csv.DictReader(f,delimiter='\t'):
        if r['arch']=='x86_64' and r['runtime_elf']=='YES': inventory[(r['sourcerpm'],r['path'])].append(r)
edges=[]
for p in sorted((ROOT/'docs/progress/R19_21/R20/tables/split').glob('cpp_abi_edges.part*.tsv')):
    with p.open()as f:
        for i,r in enumerate(csv.DictReader(f,delimiter='\t'),2):
            if r['arch']=='x86_64':edges.append({**r,'table':str(p.relative_to(ROOT)),'line':i})
headers=collections.defaultdict(list)
families={s['provider']for s in specs}
with (ROOT/'docs/progress/P7_0909/tables/packages.tsv').open()as f:
    devel={r['checksum']for r in csv.DictReader(f,delimiter='\t')if r['devel_name']=='YES'}
with gzip.open(ROOT/'docs/progress/P7_0909/tables/headers.tsv.gz','rt')as f:
    seen=set()
    for r in csv.DictReader(f,delimiter='\t'):
        family=names.get(r['sourcerpm'])
        if family not in families or r['status']!='PASS' or r['scope']!='INCLUDE_TREE' or r['rpm_sha256']not in devel:continue
        key=(family,r['cache_path'])
        if key in seen:continue
        seen.add(key);headers[family].append(r)
cache={}
def dyn(r):
    key=(r['rpm_sha256'],r['path'])
    if key not in cache:
        p=OLD/'tmp/R11/extracted'/r['rpm_sha256'][:2]/r['rpm_sha256']/r['path'].lstrip('/')
        if not p.is_file():return None
        tag='elf_'+r['rpm_sha256'][:12]+'_'+re.sub(r'[^A-Za-z0-9]','_',p.name)
        rc,txt,raw=record(tag,['readelf','--dyn-syms','--wide',str(p)])
        if rc!=0:return None
        cache[key]={'path':str(p),'sha256':digest(p),'record':raw,'lines':txt.splitlines(),'inventory':r}
    return cache[key]
results=[]
for s in specs:
    start=time.monotonic(); proof={'spec':s,'header_matches':[],'elf_matches':[],'source_includes':[]}
    for h in headers[s['provider']]:
        if s['header'] and not h['path'].endswith('/'+s['header']):continue
        p=pathlib.Path(h['cache_path'])
        if not p.is_file():continue
        txt=p.read_text(errors='replace');lines=txt.splitlines()
        for i,t in enumerate(lines):
            if s['token'] in t:
                proof['header_matches'].append({'metadata':h,'line':i+1,'context':'\n'.join(f'{j+1}: {lines[j]}'for j in range(max(0,i-3),min(len(lines),i+5)))})
    candidates=[];seen=set()
    for n in s['entities']:
        for e in business.get(n,[]):
            if e['provider_source']==s['provider'] and e['consumer_source']==s['consumer'] and e['demangled'].startswith(s['symbol']) and e['raw_symbol']not in seen:
                seen.add(e['raw_symbol']);candidates.append(e)
    for e in candidates:
        for edge in edges:
            if edge['provider_source_rpm']!=e['provider_sourcerpm'] or edge['consumer_source_rpm']!=e['consumer_sourcerpm']:continue
            ps=inventory[edge['provider_source_rpm'],edge['provider_elf']];cs=inventory[edge['consumer_source_rpm'],edge['consumer_elf']]
            for pr in ps:
                pd=dyn(pr)
                if pd is None:continue
                pl=[l for l in pd['lines']if len(l.split())>=8 and l.split()[7].split('@')[0]==e['raw_symbol'] and l.split()[6]!='UND']
                if not pl:continue
                for cr in cs:
                    cd=dyn(cr)
                    if cd is None:continue
                    cl=[l for l in cd['lines']if len(l.split())>=8 and l.split()[7].split('@')[0]==e['raw_symbol'] and l.split()[6]=='UND']
                    if cl:
                        proof['elf_matches'].append({'symbol':e,'edge':edge,'provider':{k:v for k,v in pd.items()if k!='lines'},'consumer':{k:v for k,v in cd.items()if k!='lines'},'provider_lines':pl,'consumer_lines':cl})
                        break
                if proof['elf_matches']:break
            if proof['elf_matches']:break
        if proof['elf_matches']:break
    include_names=sorted({pathlib.Path(h['metadata']['path']).name for h in proof['header_matches']})
    roots=sorted((OLD/'tmp/corpus/extracted').glob(s['consumer']+'-*.src.rpm'))
    # 只按真实包含语句检索；同名头匹配仍待人工核对安装路径。
    for root in roots:
        if not include_names:continue
        pattern=r'^\s*#\s*include\s*[<"](?:[^>"\n]*/)?(?:'+ '|'.join(re.escape(n)for n in include_names)+r')[>"]'
        rc,txt,raw=record('include_'+s['key'],['rg','-n','-uuu','--glob','*.h','--glob','*.hh','--glob','*.hpp','--glob','*.cc','--glob','*.cpp','--glob','*.cxx',pattern,str(root/'unpacked')])
        proof['source_includes'].append({'root':str(root),'record':raw,'exit':rc,'matches':txt.splitlines()})
    proof['verification_seconds']=time.monotonic()-start
    save(dest/(s['key']+'.json'),proof);results.append({'key':s['key'],'headers':len(proof['header_matches']),'elfs':len(proof['elf_matches']),'includes':len(proof['source_includes'])})
    print(results[-1],flush=True)
save(dest/'INDEX.json',results)
