import pathlib,json,subprocess,sys,hashlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
TMP=ROOT/'tmp/R117';OUT=ROOT/'docs/progress/R117';REC=ROOT/'docs/progress/API_0911/record.py'
CC='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
SDK=ROOT/'tmp/API_0911/W1_CONT/gnu14_sdk'
gnu_config=next(SDK.rglob('c++config.h')).parent.parent
LLVM=ROOT/'codes/llvm/libcxx/include';GCC=ROOT/'codes/gcc/libstdc++-v3/include'
gnu_public=next(SDK.rglob('vector')).parent
FLAGS={'libcxx':['-nostdinc++','-I'+str(LLVM),'-I'+str(TMP/'config/libcxx')],
       'libstdcxx':['-nostdinc++','-I'+str(GCC/'std'),'-I'+str(GCC/'c_global'),'-I'+str(GCC),'-I'+str(gnu_config),'-I'+str(gnu_public)]}
def run(tag,args,allowed=()):
 stem=OUT/'raw'/tag;i=1
 while pathlib.Path(str(stem)+'.command.txt').exists():i+=1;stem=OUT/'raw'/(tag+'_try'+str(i))
 subprocess.run([sys.executable,str(REC),str(stem),'bash',str(TMP/'limit.sh'),*map(str,args)],cwd=ROOT)
 code=int(pathlib.Path(str(stem)+'.exitcode').read_text())
 if code and code not in allowed:raise RuntimeError((tag,code))
 return {'raw':str(stem.relative_to(ROOT)),'exit':code}
def save(name,data):
 p=OUT/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(data,ensure_ascii=False,indent=2)+'\n')
