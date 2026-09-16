from common import *
sys.dont_write_bytecode=True
sys.path.insert(0,str(ROOT/'codes/llvm/clang/bindings/python'))
from clang import cindex as ci
sysroot=OLD/'tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0'
ci.Config.set_library_file(str(sysroot/'usr/lib64/libclang.so.22.1.8'))
index=ci.Index.create()
for lib,header in [('libcxx',LLVM/'vector'),('libstdcxx',GCC/'std/vector')]:
 args=[*FLAGS[lib],'-std=c++23','-x','c++','--sysroot='+str(sysroot),'-resource-dir',str(sysroot/'usr/lib64/clang/22')]
 tu=index.parse(str(header),args=args,options=ci.TranslationUnit.PARSE_SKIP_FUNCTION_BODIES)
 print(lib,'diagnostics',[(d.severity,str(d)) for d in tu.diagnostics])
 count=0
 def visit(c):
  global count
  if c.location.file:
   if c.spelling=='vector':print(c.kind,c.spelling,c.location.file.name,c.location.line,c.semantic_parent.spelling)
   count+=1
  if c.kind in [ci.CursorKind.TRANSLATION_UNIT,ci.CursorKind.NAMESPACE,ci.CursorKind.CLASS_TEMPLATE,ci.CursorKind.STRUCT_DECL,ci.CursorKind.CLASS_DECL,ci.CursorKind.UNION_DECL]:
   for x in c.get_children():visit(x)
 visit(tu.cursor);print('cursors',count)
