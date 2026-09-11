#!/usr/bin/env python3
"""复核失效链接的真实来源地址，重算已有符号表；不运行原实验。"""
import base64,collections,csv,hashlib,importlib.util,pathlib,re,subprocess,sys
b=pathlib.Path('docs/progress/P4_0909'); r=pathlib.Path('docs/progress/R116')
rev='97b436da4c33663581d394f4ee0a5977fc38c2f4'; results=[]
for name,path in [('pinned_shared','src/shared_mutex.cpp'),('pinned_shared_header','include/shared_mutex'),('pinned_cv','src/condition_variable.cpp'),('pinned_cv_header','include/__condition_variable/condition_variable.h'),('pinned_wbuffer','include/__locale_dir/wbuffer_convert.h')]:
    url=f'https://chromium.googlesource.com/external/github.com/llvm/llvm-project/libcxx/+/{rev}/{path}'
    stem=b/f'raw/corrected_urls/{name}'
    rc=subprocess.run([sys.executable,'docs/progress/R114/code/record.py',str(stem),'nice','-n','15','ionice','-c','3','curl','--fail','--location','--max-time','25',url+'?format=TEXT']).returncode
    if rc==0:
        data=base64.b64decode(pathlib.Path(str(stem)+'.stdout').read_bytes()); prior=(r/f'sources/{name}.txt').read_bytes()
        match=data==prior
        print(name,'SHA_MATCH',match,hashlib.sha256(data).hexdigest());assert match
    else:match='NOT_OBSERVED'
    results.append([name,url,rc,match])
with (b/'CORRECTED_URLS.tsv').open('w',newline='') as f:
    w=csv.writer(f,delimiter='\t');w.writerow(['snapshot','replacement_url','curl_exit','decoded_matches_original_snapshot']);w.writerows(results)
for file in ['036_platform_symbols.stdout','037_experiment_symbols.stdout']:
    pairs=set(); nodes=set()
    for line in (r/'raw'/file).read_text().splitlines():
        x=line.split()
        if len(x)>=8 and x[0].rstrip(':').isdigit() and x[3] in ['FUNC','IFUNC','OBJECT'] and x[6]!='UND':
            parts=re.split(r'@@?',x[7]); pair=(parts[0],parts[1] if len(parts)>1 else 'UNVERSIONED');pairs.add(pair)
            if x[6]=='ABS' and parts[0].startswith(('GCC_','LLVM_')):nodes.add(pair)
    print(file,'PAIRS',len(pairs),'ABS_VERSION_NODE_PAIRS',len(nodes))
    if file.startswith('036'):platform=pairs
    else:experiment=pairs
print('PLATFORM_PAIRS_NOT_IN_EXPERIMENT',len(platform-experiment))
spec=importlib.util.spec_from_file_location('old','docs/progress/R66/code/compare_abi_manifests.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
left=m.stdcxx('codes/gcc/libstdc++-v3/config/abi/post/x86_64-linux-gnu/baseline_symbols.txt')
print('GCC_BASELINE_DISTINCT_UNVERSIONED_KEYS',len(left),'KIND_COUNTS',dict(collections.Counter(v[0] for v in left.values())))
print('SOURCE_SCRIPT_SHA256',hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest())
