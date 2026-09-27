from common import *
resource.setrlimit(resource.RLIMIT_CORE,(0,0));gate()
root=TMP/'root';build=TMP/'context';build.mkdir(exist_ok=True)
libs=[root/x for x in ('usr/lib64','lib64','usr/lib64/hal')];lp=':'.join(map(str,libs))
compiler='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
exe=build/'dali_context';p=root/'usr/lib64/libdali2-adaptor.so.2.0.0'
r=run('context_compile',[compiler,'--sysroot='+str(root),'--gcc-toolchain='+str(root/'usr'),'-std=c++17','-O0','-stdlib=libstdc++','-I'+str(root/'usr/include'),OUT/'fixtures/dali_context.cpp','-o',exe,*['-L'+str(d) for d in libs],'-Wl,-rpath-link,'+lp,'-ldali2-adaptor','-ldali2-core'],check=False)
result=dict(compile=r,source_sha256=sha(OUT/'fixtures/dali_context.cpp'),scope='Prerequisite only, not one of the 23 edge baselines')
if not r['exit']:
 gate();result['run']=run('context_offscreen',['env','-u','DISPLAY','-u','WAYLAND_DISPLAY','XDG_CACHE_HOME='+str(build/'cache'),root/'lib64/ld-linux-x86-64.so.2','--library-path',lp,exe,p,sha(p)],cwd=build,timeout=30,check=False)
if (OUT/'CONTEXT_PREFLIGHT.json').exists():save('context_history/'+str(time.time_ns())+'.json',json.loads((OUT/'CONTEXT_PREFLIGHT.json').read_text()))
save('CONTEXT_PREFLIGHT.json',result)
print(json.dumps(result,ensure_ascii=False)[-5000:])
