from common import *
import gzip,csv,re,collections
out=OUT/'source_index';out.mkdir(exist_ok=True)
roots=[LLVM,GCC,GCC.parent/'libsupc++']
files=[];count=collections.Counter()
with gzip.open(out/'conditional_declarations.tsv.gz','wt') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['path','line','kind','source_text','sha256'])
 for root in roots:
  for p in sorted(root.rglob('*')):
   if not p.is_file() or p.is_symlink() or p.name in ['Makefile.in'] or p.suffix in ['.po','.gmo']:continue
   data=p.read_bytes()
   if b'\0' in data:continue
   digest=hashlib.sha256(data).hexdigest();rel=str(p.relative_to(ROOT));files.append([rel,len(data),digest])
   for n,line in enumerate(data.decode('utf-8',errors='replace').splitlines(),1):
    kind=''
    if re.match(r'\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b',line):kind='CONDITIONAL_BRANCH'
    elif re.match(r'\s*#\s*define\b',line):kind='MACRO'
    elif re.search(r'\b(class|struct|union|enum|typedef|using|concept)\s+\w|\btemplate\s*<|\b(?:constexpr|inline)\b.*\b\w+\s*=',line):kind='DECLARATION_CANDIDATE_NOT_SEMANTIC'
    if kind:w.writerow([rel,n,kind,line,digest]);count[kind]+=1
with (out/'files.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['path','bytes','sha256']);w.writerows(files)
save('source_index/SUMMARY.json',{'files':len(files),'rows':dict(count),'scope':'原文索引包括非活动条件、内部实现与注释；不是公开 API 计数；代码无改动'})
print(len(files),dict(count))
