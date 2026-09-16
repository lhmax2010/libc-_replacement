from common import *
import re, shlex
REMOTE='/opt/usr/home/owner/share/tmp/r119'
SDB=['/home/toolchain/.local/bin/sdb','-s','192.168.108.26:26101']
def remote(tag,command):
    rc,out,raw=record(tag,['timeout','45',*SDB,'shell','{ '+command+'; }; r119_rc=$?; printf "\nR119_TARGET_EXIT=%s\n" "$r119_rc"'])
    marker=re.findall(r'R119_TARGET_EXIT=(\d+)',out)
    if rc or not marker:raise RuntimeError('BOARD_CONNECTION_INTERRUPTED: '+raw+'；立即停报，不重试')
    return int(marker[-1]),out,raw
def must(tag,command):
    rc,out,raw=remote(tag,command)
    if rc:raise RuntimeError(f'BOARD_COMMAND_FAILED {raw} exit={rc}')
    return out

base=OLD/'tmp/R11/extracted/3e/3e88844a0970832494dfb24067c7b8d52276413b200f44885ce43bb8b401f674/usr/lib/lwe/mobile'
provider=base/'liblightweight-web-engine.mobile-impl.so';front=base/'liblightweight-web-engine.mobile.so'
remote_provider='/usr/lib/lwe/mobile/'+provider.name;remote_front='/usr/lib/lwe/mobile/'+front.name
out=must('arm_provider_identity','sha256sum '+remote_provider+' '+remote_front+' /usr/lib/libstdc++.so.6')
_,_,_,gnu=configuration('armv7l','gnu')
save(OUT/'W0/arm_system_identity.json',dict(observed_system=out,expected_provider=digest(provider),expected_front=digest(front),expected_gnu=digest(gnu),policy='系统产物与冻结产物不同：独立上传，不替换系统文件'))
remote_provider=REMOTE+'/'+provider.name;remote_front=REMOTE+'/'+front.name
dest=TMP/'W0/armv7l';dest.mkdir(parents=True,exist_ok=True)
headers=ROOT/'tmp/R118/function/lwe/include';source=OUT/'code/lwe_callback.cpp';rows=[]
for lib in ['gnu','cxx']:
    flags,links,b,g=configuration('armv7l',lib);exe=dest/('callback_'+lib)
    rc,_,raw=record('arm_lwe_'+lib+'_build',['nice','-n','19','ionice','-c','3',CC,*flags,'-I'+str(headers),source,front,*links,'-ldl','-pthread','-o',exe])
    row=dict(library=lib,build_exit=rc,build_raw=raw,provider_sha256=digest(provider),source_sha256=digest(source),runs=[]);rows.append(row)
    if rc:continue
    assert lib=='gnu', 'libc++ 混合自然链接意外成功，需重新确认部署依赖'
    must('arm_prepare','test ! -e '+REMOTE+' && mkdir '+REMOTE+' && mkdir '+REMOTE+'/storage && ln -s '+remote_front+' '+REMOTE+'/liblightweight-web-engine.so.1')
    uploads=[(p,p.name) for p in sorted(base.glob('*.so'))]+[(gnu,'libstdc++.so.6')]
    coregl=OLD/'tmp/R11/extracted/07/074712eff9b92d24038bf6b19e5c4ea5e5c508908bae6924bc441345a3b6df09/usr/lib'
    uploads += [(coregl/name,name) for name in ['libGLESv2.so.2','libEGL.so.1','libCOREGL.so.4']]
    save(OUT/'W0/arm_uploads.json',[dict(source=str(p),target=REMOTE+'/'+name,sha256=digest(p)) for p,name in uploads])
    for n,(p,name) in enumerate(uploads):checked('arm_lib_upload'+str(n),['timeout','45',*SDB,'push',p,REMOTE+'/'+name])
    libhashes=must('arm_uploaded_library_hashes','sha256sum '+' '.join(REMOTE+'/'+name for _,name in uploads))
    for p,name in uploads:assert digest(p)+'  '+REMOTE+'/'+name in libhashes
    row['exe_sha256']=digest(exe)
    checked('arm_upload',['timeout','45',*SDB,'push',exe,REMOTE+'/callback_gnu'])
    out=must('arm_exe_identity','sha256sum '+REMOTE+'/callback_gnu && chmod 755 '+REMOTE+'/callback_gnu')
    assert digest(exe)+'  '+REMOTE+'/callback_gnu' in out
    for n in range(1,6):
        rc,out,raw=remote('arm_lwe_gnu_run'+str(n),'cd '+REMOTE+' && ulimit -c 0 && LD_LIBRARY_PATH='+REMOTE+':/usr/lib/lwe/mobile nice -n 19 ionice -c 3 ./callback_gnu '+remote_provider+' '+REMOTE+'/storage no-render')
        row['runs'].append(dict(exit=rc,raw=raw,concrete_value='callback_value size=1 hex=33' in out,destroyed='destroy_completed=1' in out))
        save(OUT/'W0/armv7l.json',rows)
        if rc:break
    # Only enumerate and remove this task's independently created directory.
    out=must('arm_cleanup_inventory','find '+REMOTE+' -mindepth 1 -printf "%y %p\n"')
    files=[];dirs=[]
    for line in out.replace('\r','').splitlines():
        if line.startswith(('f ','l ','d ')):
            kind,p=line.split(' ',1);assert p.startswith(REMOTE+'/') and '/../' not in p
            (dirs if kind=='d' else files).append(p)
    if files:must('arm_cleanup_files','rm -- '+' '.join(map(shlex.quote,files)))
    for n,p in enumerate(sorted(dirs,key=len,reverse=True)+[REMOTE]):must('arm_cleanup_directory'+str(n),'rmdir -- '+shlex.quote(p))
    must('arm_released','test ! -e '+REMOTE+' && printf "BOARD_RELEASED=1\n"')
    save(OUT/'W0/board_cleanup.json',dict(removed_files=files,removed_directories=dirs+[REMOTE],absent=True,released=True))
save(OUT/'W0/armv7l.json',rows)
