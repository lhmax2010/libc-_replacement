from common import *
sys.dont_write_bytecode=True
sys.path.insert(0,str(ROOT/'codes/llvm/clang/bindings/python'))
from clang import cindex as ci
s=OLD/'tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0'
ci.Config.set_library_file(str(s/'usr/lib64/libclang.so.22.1.8'))
index=ci.Index.create()
for lib,head in [('libcxx',LLVM/'type_traits'),('libcxx',LLVM/'algorithm'),('libstdcxx',GCC/'ext/vstring.h')]:
 tu=index.parse(str(head),args=[*FLAGS[lib],'-std=c++23','-x','c++','--sysroot='+str(s),'-resource-dir',str(s/'usr/lib64/clang/22')],options=ci.TranslationUnit.PARSE_SKIP_FUNCTION_BODIES)
 def v(c):
  if c.spelling in ['is_same_v','sort','__versa_string']:
   p=c.semantic_parent;ns=[]
   while p and p.kind!=ci.CursorKind.TRANSLATION_UNIT:ns.append((p.kind.name,p.spelling));p=p.semantic_parent
   print(lib,c.kind.name,c.spelling,c.location,ns,flush=True)
  if c.kind in [ci.CursorKind.TRANSLATION_UNIT,ci.CursorKind.NAMESPACE,ci.CursorKind.UNEXPOSED_DECL,ci.CursorKind.LINKAGE_SPEC]:
   for x in c.get_children():v(x)
 v(tu.cursor)
