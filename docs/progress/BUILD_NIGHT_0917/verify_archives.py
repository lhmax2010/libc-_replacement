import hashlib,json,subprocess,sys
from pathlib import Path
out=Path('progress/BUILD_NIGHT_0917'); rows=[]
for filename in sys.argv[1:]:
    p=Path(filename); commands=[]; texts={}
    for tool in [['strings',str(p)],['ar','t',str(p)],['readelf','-p','.comment',str(p)]]:
        r=subprocess.run(tool,capture_output=True,text=True); commands.append(dict(command=tool,exitcode=r.returncode,stderr=r.stderr)); texts[tool[0]]=r.stdout
    row=dict(path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest(),bytes=p.stat().st_size,members=texts['ar'].splitlines(),exact_std1_lines=sum('_ZNSt3__1' in l for l in texts['strings'].splitlines()),exact_cxx11_lines=sum('_ZNSt7__cxx11' in l for l in texts['strings'].splitlines()),std1_sample=[l for l in texts['strings'].splitlines() if '_ZNSt3__1' in l][:10],cxx11_sample=[l for l in texts['strings'].splitlines() if '_ZNSt7__cxx11' in l][:10],comments=texts['readelf'],commands=commands)
    rows.append(row)
name='archive_'+hashlib.sha256('|'.join(sys.argv[1:]).encode()).hexdigest()[:12]+'.json'
(out/name).write_text(json.dumps(rows,indent=2)); print(name)
for r in rows: print(r['path'],r['bytes'],r['exact_std1_lines'],r['exact_cxx11_lines'])
