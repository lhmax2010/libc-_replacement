#!/usr/bin/env python3
import pathlib, subprocess, sys, json, hashlib, re
root=pathlib.Path.cwd();out=root/'docs/progress/API_0911/W2';tmp=root/'tmp/API_0911/W2';tmp.mkdir(parents=True,exist_ok=True)
base=pathlib.Path('/home/toolchain/development/libc++_replacement/progress/R105/artifacts')
versions={'gnu':('boost_x86_64_gcc','105.10.8'),'cxx':('boost_x86_64_libcxx','105.10.7')}
providers={k:base/d/f'boost-program-options-1.83.0-{v}.x86_64/usr/lib64/libboost_program_options.so.1.83.0' for k,(d,v) in versions.items()}
headers=base/'boost_x86_64_libcxx/boost-devel-1.83.0-105.10.7.x86_64/usr/include'
runtime=root/'tmp/IMPL_0908/build-native/lib';cxxinc=root/'tmp/IMPL_0908/build-native/include/c++/v1'
compiler='/home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++'
def run(stem,cmd):
    rc=subprocess.run([sys.executable,str(root/'docs/progress/API_0911/record.py'),str(out/'raw'/stem),*cmd]).returncode
    if rc:raise RuntimeError((stem,rc))
def compile(kind,src,dest):
    cmd=['g++'] if kind=='gnu' else [compiler,'-nostdinc++','-I'+str(cxxinc)]
    run(dest.name+'_compile',['nice','-n','15','ionice','-c','3',*cmd,'-std=c++17','-O0','-g','-fPIC','-fvisibility=hidden','-DBOOST_ALL_DYN_LINK','-I'+str(headers),'-c',str(out/src),'-o',str(dest)])
run('003_versions',['bash','-lc','g++ --version; '+compiler+' --version'])
manifest=[];matrix=[]
for k,p in providers.items():
    run('provider_'+k+'_dynamic',['readelf','-dW',str(p)])
    run('provider_'+k+'_symbols',['nm','-D','-C',str(p)])
    manifest.append(dict(role='real_provider_'+k,path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
for caller,prov in [('gnu','cxx'),('cxx','gnu')]:
    provider=providers[prov];cell=caller+'_to_'+prov
    a=tmp/(cell+'_caller.o');b=tmp/(cell+'_provider.o');app=tmp/(caller+'_app.o');shim=tmp/('lib'+cell+'.so')
    compile(caller,'caller.cpp',a);compile(prov,'provider_side.cpp',b);compile(caller,'app.cpp',app)
    run(cell+'_link',['nice','-n','15','ionice','-c','3','g++','-shared','-Wl,-z,defs',str(a),str(b),str(provider),'-L'+str(runtime),'-Wl,-rpath,'+str(runtime),'-Wl,-rpath,'+str(provider.parent),'-lc++','-lc++abi','-lstdc++','-ldl','-o',str(shim)])
    run(cell+'_dynamic',['readelf','-dW',str(shim)]);run(cell+'_symbols',['nm','-DC',str(shim)])
    for mode in ['shim','direct_control']:
        p=provider if mode=='shim' else providers[caller]
        dep=shim if mode=='shim' else p
        exe=tmp/(cell+'_'+mode)
        run(exe.name+'_link',['nice','-n','15','ionice','-c','3','g++',str(app),str(dep),'-L'+str(runtime),'-Wl,-rpath,'+str(tmp),'-Wl,-rpath,'+str(p.parent),'-Wl,-rpath,'+str(runtime),'-Wl,--no-as-needed','-lc++','-lc++abi','-lstdc++','-ldl','-o',str(exe)])
        for repeat in range(1,6):
            name=f'{cell}_{mode}_{repeat}'
            run(name,['nice','-n','15','ionice','-c','3','timeout','20','env','LD_LIBRARY_PATH='+str(runtime)+':'+str(p.parent)+':'+str(tmp),str(exe),str(p),mode])
            text=(out/'raw'/(name+'.stdout')).read_text()
            assert 'ASSERT values=1 maps_cxx=1 maps_gnu=1 maps_provider=1' in text
            assert len(re.findall(r'VALUE .*exact=1',text))==5
            if mode=='shim':
                assert text.count('REAL_CALLEE='+str(provider))==5,text[:1000]
                assert 'PROVIDER_SCOPES_COMPLETED=5' in text
            matrix.append(dict(cell=cell,mode=mode,repeat=repeat,architecture='x86_64 native',exact_payloads=5,maps='PASS',exit=0,raw=name))
        manifest.append(dict(role=exe.name,path=str(exe),sha256=hashlib.sha256(exe.read_bytes()).hexdigest()))
    manifest.append(dict(role=shim.name,path=str(shim),sha256=hashlib.sha256(shim.read_bytes()).hexdigest()))
for p in [runtime/'libc++.so.1',runtime/'libc++abi.so.1',pathlib.Path(subprocess.check_output(['g++','-print-file-name=libstdc++.so.6'],text=True).strip()).resolve()]:
    manifest.append(dict(role='runtime',path=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
(out/'matrix.json').write_text(json.dumps(matrix,indent=2)+'\n');(out/'identity.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('PASS',len(matrix),'runs',sum(x['exact_payloads'] for x in matrix),'payload checks')
