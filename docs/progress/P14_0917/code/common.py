import hashlib,json,pathlib,subprocess,sys
ROOT=pathlib.Path(__file__).resolve().parents[2]
TMP=ROOT/'tmp/P14'
OUT=ROOT/'docs/progress/P14_0917'
REC=ROOT/'docs/progress/API_0911/record.py'
CONFIG=json.loads((ROOT/'docs/progress/P13_0917/W1/BUILD_CONFIG.json').read_text())
CC=CONFIG['compiler']
GNU=CONFIG['flags']['gnu']
PROVIDER=pathlib.Path(CONFIG['real_gnu_provider'])
LOADER=CONFIG['loader']
LIBPATH=CONFIG['library_path']+':'+str(TMP)
GNU_LIB=ROOT/'tmp/API_0911/W1_CONT/closure_ewk/libstdc++.so.6'
LINK=['-nostdlib++',str(GNU_LIB),'-ldl','-pthread','-fuse-ld=/usr/bin/ld.gold']
def run(stage,name,args,allow=()):
    stem=OUT/stage/'raw'/name
    attempt=1
    while pathlib.Path(str(stem)+'.command.txt').exists():
        attempt+=1;stem=OUT/stage/'raw'/(name+'_try'+str(attempt))
    p=subprocess.run([sys.executable,str(REC),str(stem),'bash',str(TMP/'limit.sh'),*map(str,args)],cwd=ROOT)
    result={'name':name,'exit':int(pathlib.Path(str(stem)+'.exitcode').read_text()),'raw':str(stem.relative_to(ROOT))}
    result['stdout_sha256']=hashlib.sha256(pathlib.Path(str(stem)+'.stdout').read_bytes()).hexdigest()
    if p.returncode and result['exit'] not in allow:raise RuntimeError(result)
    return result
def execute(stage,name,exe,args=(),allow=()):
    return run(stage,name,[LOADER,'--library-path',LIBPATH,exe,*args],allow)
def save(stage,name,obj):
    (OUT/stage).mkdir(parents=True,exist_ok=True)
    (OUT/stage/name).write_text(json.dumps(obj,indent=2)+'\n')
