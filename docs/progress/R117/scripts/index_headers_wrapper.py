"""只解析声明，不生成目标文件。保留配置、诊断、公开入口与实际声明位置。"""
from common import *
import csv,gzip,re,time,gc,os
sys.dont_write_bytecode=True
sys.path.insert(0,str(ROOT/'codes/llvm/clang/bindings/python'))
from clang import cindex as ci
SYSROOT=OLD/'tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0'
ci.Config.set_library_file(str(SYSROOT/'usr/lib64/libclang.so.22.1.8'))
supplement=os.environ.get('R117_SUPPLEMENT')=='1'
repair=os.environ.get('R117_REPAIR')=='1'
extra=os.environ.get('R117_EXTRA')=='1'
wrapper=os.environ.get('R117_WRAPPER')=='1'
prefix='headers_wrapper' if wrapper else 'headers_extra' if extra else 'headers_repair' if repair else 'headers_supplement' if supplement else 'headers'
out=OUT/prefix;out.mkdir(exist_ok=True)
headers=[]
for p in sorted(LLVM.rglob('*')):
 if not p.is_file():continue
 name=str(p.relative_to(LLVM))
 if '__' in name or p.name in ['CMakeLists.txt','module.modulemap.in','libcxx.imp']:continue
 category='TS_EXPERIMENTAL' if name.startswith('experimental/') else 'VENDOR_EXTENSION' if name.startswith('ext/') else 'STANDARD_HEADER_CANDIDATE'
 headers.append(dict(library='libcxx',header=name,path=str(p.relative_to(ROOT)),category=category,inventory_evidence='codes/llvm/libcxx/utils/libcxx/header_information.py:38',sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
make=(GCC/'Makefile.am').read_text()
for folder in ['std','c_global','c_std','c_compatibility','c','backward','debug','decimal','experimental','ext','parallel','tr1','tr2']:
 for p in sorted((GCC/folder).rglob('*')):
  if not p.is_file() or p.name.startswith('_') or p.suffix in ['.tcc','.in'] or '/bits/' in str(p):continue
  rel=str(p.relative_to(GCC/folder));name=rel if folder in ['std','c_global','c_std','c_compatibility','c'] else folder+'/'+rel
  # Nested implementation trees are not advertised as independent public API entries.
  if folder in ['ext','parallel','experimental'] and '/' in rel and not (rel.startswith('pb_ds/') and '/detail/' not in rel):continue
  install_tail=rel[len('pb_ds/'):] if rel.startswith('pb_ds/') else rel
  mentions=[i for i,line in enumerate(make.splitlines(),1) if ('/'+install_tail) in line]
  if not mentions:continue
  category='STANDARD_HEADER_CANDIDATE' if folder in ['std','c_global','c_std','c_compatibility','c'] else 'TS_EXPERIMENTAL' if folder in ['experimental','tr1','tr2'] else 'VENDOR_EXTENSION'
  headers.append(dict(library='libstdcxx',header=name,path=str(p.relative_to(ROOT)),category=category,inventory_evidence='codes/gcc/libstdc++-v3/include/Makefile.am:'+','.join(map(str,mentions)),sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
for name in ['exception','new','typeinfo','initializer_list','compare','cxxabi.h']:
 p=GCC.parent/'libsupc++'/name
 if p.is_file():headers.append(dict(library='libstdcxx',header=name,path=str(p.relative_to(ROOT)),category='VENDOR_ABI_EXTENSION' if name=='cxxabi.h' else 'STANDARD_HEADER_CANDIDATE',inventory_evidence='codes/gcc/libstdc++-v3/libsupc++/Makefile.am',sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
if supplement:
 headers=[h for h in headers if h['category']=='STANDARD_HEADER_CANDIDATE' or h['header'].startswith('ext/pb_ds/')]
if extra:headers=[h for h in headers if h['header'].startswith(('ext/pb_ds/','debug/'))]
if wrapper:headers=[h for h in headers if h['library']=='libcxx' and h['header']=='string.h']
with (out/'inventory.tsv').open('w') as f:
 w=csv.DictWriter(f,headers[0].keys(),delimiter='\t');w.writeheader();w.writerows(headers)
save(prefix+'/INVENTORY.json',headers)
K=ci.CursorKind
kindmap={K.CLASS_TEMPLATE:'TYPE',K.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION:'TYPE',K.STRUCT_DECL:'TYPE',K.CLASS_DECL:'TYPE',K.UNION_DECL:'TYPE',K.ENUM_DECL:'TYPE',K.TYPEDEF_DECL:'TYPE',K.TYPE_ALIAS_DECL:'TYPE',K.TYPE_ALIAS_TEMPLATE_DECL:'TYPE',K.FUNCTION_DECL:'FUNCTION',K.FUNCTION_TEMPLATE:'FUNCTION',K.VAR_DECL:'VARIABLE',K.ENUM_CONSTANT_DECL:'VARIABLE',K.USING_DECLARATION:'IMPORTED_DECLARATION'}
if hasattr(K,'CONCEPT_DECL'):kindmap[K.CONCEPT_DECL]='CONCEPT'
kindmap[K.UNEXPOSED_DECL]='VARIABLE_TEMPLATE_CANDIDATE'
kindmap[K.NAMESPACE_ALIAS]='NAMESPACE_ALIAS'
containers={K.NAMESPACE,K.LINKAGE_SPEC,K.UNEXPOSED_DECL,K.TRANSLATION_UNIT}
classkinds={K.CLASS_TEMPLATE,K.CLASS_TEMPLATE_PARTIAL_SPECIALIZATION,K.STRUCT_DECL,K.CLASS_DECL,K.UNION_DECL,K.ENUM_DECL}
memberkinds=classkinds|{K.CXX_METHOD,K.CONSTRUCTOR,K.DESTRUCTOR,K.CONVERSION_FUNCTION,K.FUNCTION_TEMPLATE,K.FIELD_DECL,K.TYPE_ALIAS_DECL,K.TYPEDEF_DECL,K.VAR_DECL,K.ENUM_CONSTANT_DECL,K.TYPE_ALIAS_TEMPLATE_DECL}
def reserved(s):return s.startswith('__') or bool(re.match(r'_[A-Z]',s))
def namespace(c):
 parts=[];p=c.semantic_parent
 while p and p.kind!=K.TRANSLATION_UNIT:
  if p.kind==K.NAMESPACE and p.spelling:parts.append(p.spelling)
  elif p.kind not in [K.NAMESPACE,K.LINKAGE_SPEC,K.UNEXPOSED_DECL]:return None
  p=p.semantic_parent
 return list(reversed(parts))
def normalize(parts):
 return [x for x in parts if x not in ['__1','__cxx11','__8','_V2','_V1','__cxx1998','__cpo']]
def public_namespace(parts):
 return all(not reserved(s) or s in ['__gnu_cxx','__gnu_pbds','__gnu_parallel','__gnu_debug','__debug'] for s in normalize(parts))
def location(c):
 if not c.location.file:return None
 p=pathlib.Path(c.location.file.name)
 try:rel=str(p.relative_to(ROOT))
 except ValueError:return None
 if not rel.startswith('codes/'):return None
 if not (rel.startswith('codes/llvm/libcxx/include/') or rel.startswith('codes/gcc/libstdc++-v3/')):return None
 return [rel,c.location.line,c.location.column]
index=ci.Index.create();status=[];started=time.monotonic()
with gzip.open(out/'declarations.jsonl.gz','wt') as df,gzip.open(out/'members.jsonl.gz','wt') as mf:
 for hnum,h in enumerate(headers,1):
  lib=h['library'];header=h['header']
  modes=(['c++23','c++26'] if repair else ['c++11','c++14','c++17','c++20'] if supplement else ['c++23','c++26','c++03']) if h['category']=='STANDARD_HEADER_CANDIDATE' else ['c++23']
  for mode in modes:
   args=[*FLAGS[lib],'-I'+str(GCC.parent/'libsupc++'),'-std='+mode,'-x','c++','--sysroot='+str(SYSROOT),'-resource-dir',str(SYSROOT/'usr/lib64/clang/22'),'-Wno-pragma-system-header-outside-header']
   t=time.monotonic();row={'library':lib,'header':header,'path':h['path'],'mode':mode,'args':args,'declarations':0,'members':0}
   try:
    if wrapper:
     virtual=str(TMP/'header_wrapper.cpp')
     row['unsaved_wrapper']='#include <'+header+'>\n'
     tu=index.parse(virtual,args=args,unsaved_files=[(virtual,row['unsaved_wrapper'])],options=ci.TranslationUnit.PARSE_SKIP_FUNCTION_BODIES)
    else:tu=index.parse(str(ROOT/h['path']),args=args,options=ci.TranslationUnit.PARSE_SKIP_FUNCTION_BODIES)
    row['diagnostics']=[{'severity':d.severity,'text':str(d)} for d in tu.diagnostics]
    row['status']='PARTIAL_PARSE_ERRORS' if any(d.severity>=3 for d in tu.diagnostics) else 'PARSED'
    def write_member(c,owner):
     for child in c.get_children():
      if child.kind not in memberkinds or child.access_specifier==ci.AccessSpecifier.PRIVATE or reserved(child.spelling):continue
      loc=location(child)
      if loc:
       mf.write(json.dumps({'library':lib,'header':header,'mode':mode,'owner':owner,'kind':child.kind.name,'name':child.spelling,'signature':child.displayname,'type':child.type.spelling,'location':loc,'access':child.access_specifier.name},ensure_ascii=False)+'\n');row['members']+=1
    def visit(c):
     if c.kind in containers and not (c.kind==K.UNEXPOSED_DECL and c.spelling):
      for x in c.get_children():visit(x)
      return
     kind=kindmap.get(c.kind)
     # libclang exposes variable templates as UNEXPOSED_DECL on some versions;
     # those are separately covered by the source declaration candidate index.
     if not kind or not c.spelling:return
     ns=namespace(c);loc=location(c)
     if ns is None or not public_namespace(ns) or not loc:return
     if reserved(c.spelling) and not any(x in ns for x in ['__gnu_cxx','__gnu_pbds','__gnu_parallel']):return
     canonical='::'.join(normalize(ns)+[c.spelling])
     original='::'.join(ns+[c.spelling])
     df.write(json.dumps({'library':lib,'header':header,'mode':mode,'entity':canonical,'original':original,'kind':kind,'cursor_kind':c.kind.name,'signature':c.displayname,'type':c.type.spelling,'usr':c.get_usr(),'location':loc,'parse_status':row['status']},ensure_ascii=False)+'\n');row['declarations']+=1
     if c.kind in classkinds:write_member(c,canonical)
    visit(tu.cursor)
    del tu;gc.collect()
   except Exception as e:row.update(status='NOT_OBSERVED_PARSE_EXCEPTION',error=repr(e))
   row['seconds']=round(time.monotonic()-t,3);status.append(row)
   save(prefix+'/PARSE_STATUS.json',status)
  if hnum%10==0:print('HEADERS',hnum,'/',len(headers),'seconds',round(time.monotonic()-started),flush=True)
print('DONE',len(headers),'entries',len(status),'parses',flush=True)
