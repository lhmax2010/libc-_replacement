#!/usr/bin/env python3
"""单并行小目标构建；无 Chromium 全量构建，无全局安装。"""
import hashlib,json,pathlib,subprocess,sys
ROOT=pathlib.Path(__file__).resolve().parents[2]
TMP=pathlib.Path(__file__).resolve().parent
OUT=ROOT/'docs/progress/P12_0916'
OLD=pathlib.Path('/home/toolchain/development/libc++_replacement')
REC=ROOT/'docs/progress/API_0911/record.py'
BUILD=TMP/'build';BUILD.mkdir(exist_ok=True)
SRC=TMP/'src';SDK=ROOT/'tmp/API_0911/W1_CONT'
LIBCXX=ROOT/'tmp/IMPL_0908/build-native'
PROVIDER=ROOT/'tmp/API_0911/W1/chromium_image/lib/libchromium-impl.so'
CC=OLD/'progress/R33/tools/tizen-clang++'
gh=list((SDK/'gnu14_sdk').rglob('vector'));gh=[p.parent for p in gh if p.parent.name=='c++'];assert len(gh)==1
cfg=list((SDK/'gnu14_sdk').rglob('c++config.h'));assert len(cfg)==1
flags={'gnu':['-nostdinc++','-I'+str(gh[0]),'-I'+str(cfg[0].parent.parent)],
       'cxx':['-nostdinc++','-I'+str(LIBCXX/'include/c++/v1')]}
headers=json.loads((ROOT/'docs/progress/API_0911/W1_CONT/ewk_header_identity.json').read_text())
inc=['-I'+p for p in sorted({str(pathlib.Path(x['path']).parent) for x in headers})]
inc+=['-I'+str(OLD/'tmp/R11/extracted/8f/8f8016c7c6a6bfbf8db05bf8e0f5ce9f1b840a1f6e1f834793554e491879a0ff/usr/include/evas-1'),
      '-I'+str(OLD/'tmp/R11/extracted/dc/dc477fb3be2ce74ab5af99eb32157a7b82d33dbc7d589a34808168feac979942/usr/include')]
common=['-std=c++17','-O2','-g','-fPIC','-fvisibility=hidden','-fexceptions','-D_GNU_SOURCE','-I'+str(SRC),*inc]
link=['-nostdlib++','-L'+str(LIBCXX/'lib'),'-Wl,--no-as-needed','-lc++','-lc++abi',str(SDK/'closure_ewk/libstdc++.so.6'),'-ldl','-pthread']
steps=[]
def run(tag,args,cwd=ROOT):
    stem=OUT/'raw'/tag
    cmd=[sys.executable,str(REC),str(stem),'bash',str(SRC/'limit_build.sh'),*map(str,args)]
    result=subprocess.run(cmd,cwd=cwd)
    rc=int(pathlib.Path(str(stem)+'.exitcode').read_text())
    steps.append({'tag':tag,'exit':rc})
    (OUT/'BUILD_STEPS.json').write_text(json.dumps(steps,indent=2)+'\n')
    if rc:raise SystemExit('BUILD STOP: '+tag+'; inspect stderr')
def compile(tag,source,flavor,extra=()):
    obj=BUILD/(tag+'.o')
    run('build_'+tag,[CC,*flags[flavor],*common,*extra,'-c',source,'-o',obj])
    return obj
def shared(tag,objects,export,extra=()):
    out=BUILD/(tag+'.so')
    run('link_'+tag,[CC,'-shared',*objects,*link,'-Wl,-z,defs,-z,relro,-z,now',
        '-Wl,--version-script='+str(export),*extra,'-o',out])
    return out
def vectors():
    for side in ['gnu','cxx']:
        obj=compile('vector_provider_'+side,SRC/'vector_provider.cpp',side)
        shared('vector_provider_'+side,[obj],SRC/'vector.exports')
        run('build_vector_app_'+side,[CC,*flags[side],*common,SRC/'vector_app.cpp',*link,'-o',BUILD/('vector_app_'+side)])
    for front,back in [('gnu','cxx'),('cxx','gnu')]:
        f=compile('vector_front_'+front,SRC/'vector_front.cpp',front)
        for bad in ([False,True] if front=='gnu' else [False]):
            name='vector_'+front+'_to_'+back+('_bad' if bad else '')
            b=compile(name+'_back',SRC/'vector_back.cpp',back,
                ['-DP12_VECTOR_PROVIDER_PATH="'+str(BUILD/('vector_provider_'+back+'.so'))+'"',*(['-DP12_CORRUPT'] if bad else [])])
            shared(name,[f,b],SRC/'vector.exports')
def cookies():
    for front,back,kind in [('cxx','gnu','reverse'),('gnu','cxx','production_pending')]:
        # Production-direction code builds, but no corresponding real provider
        # is available. Do not execute it against a GNU artifact.
        provider=PROVIDER if kind=='reverse' else pathlib.Path('/usr/share/chromium-efl/lib/libchromium-impl.so')
        b=compile('ewk_back_'+kind,SRC/'ewk_back.cpp',back,['-DP12_PROVIDER_PATH="'+str(provider)+'"'])
        f=compile('ewk_front_'+kind,SRC/'ewk_front.cpp',front,['-DP12_TESTING'])
        shared('ewk_adaptor_'+kind,[f,b],SRC/'ewk.exports',
            ['-Wl,-soname,libewk-cookie-adaptor.so.1'])
        if kind=='reverse':
            compile('ewk_front_embedded',SRC/'ewk_front.cpp',front,['-DP12_TESTING','-DP12_EMBEDDED'])
def forwarder():
    work=TMP/'forwarder_source';down=work/'tizen_src/downloadable'
    extra=['-I'+str(down),'-I'+str(TMP/'testconfig'),
        '-DLIB_RO_ROOT_DIR="'+str(PROVIDER.parent.parent)+'"','-DCHROMIUM_IMPL_LIB_FILE="libchromium-impl.so"']
    main=compile('forwarder_main',down/'ewk_interface_main.cc','gnu',extra)
    for kind in ['control','separate','embedded']:
        obj=compile('forwarder_'+kind,work/'ewk_api_wrapper_generated.cc','gnu',
                    [*extra,*(['-DEWK_COOKIE_ADAPTOR'] if kind!='control' else [])])
        objects=[main,obj]
        more=[str(SDK/'closure_ewk/libdlog.so.0')]
        if kind=='separate':
            # A specific dependency, not LD_PRELOAD or global lookup order.
            objects+=[BUILD/'ewk_adaptor_reverse.so']
            more+=['-Wl,-rpath,$ORIGIN']
            alias=BUILD/'libewk-cookie-adaptor.so.1'
            if not alias.exists():alias.symlink_to('ewk_adaptor_reverse.so')
        elif kind=='embedded':objects+=[BUILD/'ewk_front_embedded.o',BUILD/'ewk_back_reverse.o']
        shared('forwarder_'+kind,objects,SRC/'forwarder.exports',more)
    config={'compiler':str(CC),'flags':flags,'common':common,'link':link,
            'loader':str(SDK/'closure_ewk/ld-linux-x86-64.so.2'),
            'library_path':str(SDK/'closure_ewk')+':'+str(LIBCXX/'lib')+':'+str(BUILD),
            'real_gnu_provider':str(PROVIDER),'build':str(BUILD),
            'memory_limit_virtual_kib':4194304,'parallelism':1,'nice':19,'ionice_class':3,
            'test_forwarder_profile':'standalone non-TV, generated test build_config.h, not full GN/RPM build'}
    (OUT/'BUILD_CONFIG.json').write_text(json.dumps(config,indent=2)+'\n')
if __name__=='__main__':
    for stage in sys.argv[1:] or ['vectors','cookies','forwarder']:globals()[stage]()
