#!/usr/bin/env python3
"""只读源码/已发布头文件查找；选读文件留全文快照、行号与 SHA256。"""
import csv,hashlib,pathlib,subprocess,sys,json
O=pathlib.Path('docs/progress/P7_0909/stage2');C=list(csv.DictReader((O/'tables/source_locations.tsv').open(),delimiter='\t'))
mode,family,*args=sys.argv[1:]
if family=='csapi-tizenfx' and pathlib.Path('tmp/P7_0909/csapi').exists():roots=[pathlib.Path('tmp/P7_0909/csapi')]
else:roots=[pathlib.Path(p) for r in C if r['family']==family for p in r['source_roots'].split(';') if p]
if mode.startswith('header'):
    H=list(csv.DictReader((O/'tables/headers.tsv').open(),delimiter='\t'))
    paths=[pathlib.Path(r['local_path']) for r in H if r['family']==family]
else:
    paths=[]
    for root in roots:
        paths+=list(map(pathlib.Path,subprocess.check_output(['rg','--files','--hidden',str(root)]).decode().splitlines()))
assert paths,(family,'NOT_AVAILABLE')
if mode.endswith('files'):
    for p in paths:
        if not args or args[0] in str(p):print(p)
elif mode.endswith('search'):
    term=args[0];suffix=args[1] if len(args)>1 else ''
    selected=[str(p) for p in paths if not suffix or str(p).endswith(suffix)]
    assert selected,(family,suffix)
    # 输出保留 rg 本来的逐文件行号。每块 100 个文件，限制命令行长度。
    codes=[]
    for i in range(0,len(selected),100):
        cmd=['rg','-n','--no-heading','--color','never',term,*selected[i:i+100]]
        print('SEARCH_COMMAND '+json.dumps(cmd,ensure_ascii=False),flush=True)
        rc=subprocess.run(cmd).returncode;codes.append(rc)
    print('SEARCH_EXIT_CODES '+json.dumps(codes))
    assert all(x in (0,1) for x in codes)
elif mode.endswith('show'):
    suffix=args[0];selected=[p for p in paths if str(p).endswith(suffix)];assert len(selected)==1,(suffix,list(map(str,selected)))
    p=selected[0];raw=p.read_bytes();sha=hashlib.sha256(raw).hexdigest()
    dest=O/'snapshots/evidence'/family/(sha[:12]+'_'+p.name);dest.parent.mkdir(parents=True,exist_ok=True)
    if dest.exists():assert dest.read_bytes()==raw
    else:dest.write_bytes(raw)
    start=int(args[1]) if len(args)>1 else 1;end=int(args[2]) if len(args)>2 else 1000000
    print(json.dumps(dict(source=str(p),snapshot=str(dest),sha256=sha,range=[start,end]),ensure_ascii=False))
    for n,line in enumerate(raw.decode(errors='replace').splitlines(),1):
        if start<=n<=end:print(f'{n}: {line}')
else:raise ValueError(mode)
