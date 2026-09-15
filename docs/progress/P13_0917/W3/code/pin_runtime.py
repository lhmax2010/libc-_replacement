#!/usr/bin/env python3
"""给 adaptor 单个 TU 的未定义引用绑定当前运行库真实版本。

这是固定 ELF 产物配置的构建步骤，不读写运行库，不靠加载顺序。
同时隔离 TU 内定义的 weak/COMDAT 辅助符号，避免两套 personality
的 DW.ref 和 __clang_call_terminate 在同 DSO 合并。
"""
import argparse,hashlib,json,pathlib,subprocess
p=argparse.ArgumentParser();p.add_argument('--side',choices=['gnu','cxx'],required=True);p.add_argument('--object',required=True);p.add_argument('--runtime',action='append',required=True);p.add_argument('--record',required=True);a=p.parse_args()
obj=pathlib.Path(a.object);before=hashlib.sha256(obj.read_bytes()).hexdigest();versions={}
header=subprocess.check_output(['readelf','-h',str(obj)],text=True)
assert 'Advanced Micro Devices X86-64' in header,'Only the measured x86_64 object profile is enabled'
for library in a.runtime:
 text=subprocess.check_output(['readelf','--dyn-syms','--wide',library],text=True)
 for line in text.splitlines():
  fields=line.split()
  if len(fields)<8 or fields[6]=='UND' or '@@' not in fields[7]:continue
  name,version=fields[7].split('@@',1)
  if name in versions:assert versions[name]==version,(name,versions[name],version)
  versions[name]=version
symbols=subprocess.check_output(['nm','--format=posix',str(obj)],text=True)
changes={}
for line in symbols.splitlines():
 fields=line.split()
 if len(fields)<2:continue
 name,kind=fields[:2]
 if kind in ['U','w','v'] and name in versions:
  changes[name]=name+'@'+versions[name]
 elif kind in ['U','w','v'] and (name in {'__gxx_personality_v0','__cxa_throw','__cxa_allocate_exception','__cxa_free_exception','__cxa_begin_catch','__cxa_end_catch','__cxa_rethrow','__cxa_get_exception_ptr','_ZSt9terminatev'} or name.startswith(('_ZTISt','_ZTVSt','_Zn','_Zd'))):
  raise RuntimeError('Unversioned/unsupported exception or allocation binding: '+name)
 elif kind in ['W','V']:
  changes[name]=name+'.p13_'+a.side
assert '__gxx_personality_v0' in changes or '__gxx_personality_v0' not in symbols
cmd=['objcopy',*[arg for old,new in sorted(changes.items()) for arg in ['--redefine-sym',old+'='+new]],str(obj)]
r=subprocess.run(cmd);assert r.returncode==0
record={'side':a.side,'object':str(obj),'before_sha256':before,'after_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'runtime_sha256':{x:hashlib.sha256(pathlib.Path(x).read_bytes()).hexdigest()for x in a.runtime},'mapping':changes,'command':cmd,'exit':r.returncode}
pathlib.Path(a.record).write_text(json.dumps(record,indent=2)+'\n')
print(json.dumps(record,indent=2))
