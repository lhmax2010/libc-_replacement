from common import *
for lib,header in [('libcxx',LLVM/'vector'),('libstdcxx',GCC/'std/vector')]:
 row=run('ast_'+lib,[CC,*FLAGS[lib],'-std=c++23','-x','c++-header','-fsyntax-only','-Xclang','-ast-dump=json',header],allowed=(1,))
 print(lib,row)
 if not row['exit']:
  p=ROOT/(row['raw']+'.stdout');data=json.loads(p.read_text());print('bytes',p.stat().st_size)
  def visit(n,ns=[]):
   if n.get('kind')=='NamespaceDecl':ns=ns+[n.get('name','')]
   if n.get('name')=='vector' and n.get('kind') in ['ClassTemplateDecl','CXXRecordDecl']:print(ns,n['kind'],n.get('loc'))
   for x in n.get('inner',[]):visit(x,ns)
  visit(data)
