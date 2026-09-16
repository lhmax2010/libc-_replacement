"""371 个冻结 source RPM 身份的限定名词法盘点；不声称已做语义引用解析。"""
from common import *
import os,csv,gzip,re,time,bisect,collections
manifest=ROOT/'docs/progress/T1/extraction_results.tsv'
extractions=list(csv.DictReader(manifest.open(),delimiter='\t'))
frozen=list(csv.DictReader((ROOT/'docs/progress/T1/scan_status.tsv').open(),delimiter='\t'))
names={p['source_rpm'] for p in frozen}
packages=[p for p in extractions if p['source_rpm'] in names]
assert len(packages)==len(names)==371,(len(packages),len(names))
save('platform/MANIFEST_RECONCILIATION.json',{'frozen_scan_identities':len(names),'extraction_rows':len(extractions),'excluded_not_in_frozen_scan':[p for p in extractions if p['source_rpm'] not in names]})
out=OUT/'platform';out.mkdir(exist_ok=True)
root=OLD/'tmp/corpus/extracted'
exts={'.h','.hh','.hpp','.hxx','.h++','.ipp','.tpp','.inc','.c','.cc','.cpp','.cxx','.c++','.C','.m','.mm'}
mask=re.compile(r'//[^\n]*|/\*.*?\*/|(?:u8|[LuU])?R"(?P<delim>[^\s()\\]{0,16})\(.*?\)(?P=delim)"|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\\n])*\'',re.S)
qualified=re.compile(r'(?<!\w)(std|__gnu_cxx|__gnu_parallel)\s*::\s*([A-Za-z_]\w*(?:\s*::\s*[A-Za-z_]\w*)*)')
alias_re=re.compile(r'\bnamespace\s+(\w+)\s*=\s*((?:std|__gnu_cxx|__gnu_parallel)(?:::.*?)*?)\s*;')
using_re=re.compile(r'\busing\s+namespace\s+((?:std|__gnu_cxx|__gnu_parallel)(?:::\w+)*)\s*;')
def stripped(s):return mask.sub(lambda m:re.sub(r'[^\n]',' ',m.group()),s)
def hits(s):
 code=stripped(s)
 result=[(m.start(),re.sub(r'\s+','',m.group()),'QUALIFIED') for m in qualified.finditer(code)]
 for a in alias_re.finditer(code):
  target=re.sub(r'\s+','',a[2])
  for m in re.finditer(r'(?<!\w)'+re.escape(a[1])+r'\s*::\s*([A-Za-z_]\w*(?:\s*::\s*[A-Za-z_]\w*)*)',code):result.append((m.start(),target+'::'+re.sub(r'\s+','',m[1]),'NAMESPACE_ALIAS_CANDIDATE'))
 return code,sorted(result),list(using_re.finditer(code))
controls={'positive':'std::vector<std::string> f(const std::map<int,int>&); namespace s = std::chrono; s::duration<int> d;',
 'negative':'// std::vector\nconst char* p="std::string"; /* std::map */ auto r=R"x(std::deque)x";',
 'multiline':'std /*comment*/ ::\nvector<int> v;'}
assert [x[1] for x in hits(controls['positive'])[1]]==['std::vector','std::string','std::map','std::chrono','std::chrono::duration']
assert not hits(controls['negative'])[1]
assert hits(controls['multiline'])[1][0][1]=='std::vector'
save('platform/CONTROLS.json',{'status':'PASS','fixtures':controls,'kind':'lexical method only; no semantic or build proof'})
def area(path):
 p='/'+path.lower()+'/'
 if any(x in p for x in ['/libstdc++','/libcxx/','/libc++/','/gcc/testsuite/']):return 'BUNDLED_TOOLCHAIN'
 if re.search(r'/(test[^/]*|unittest[^/]*|benchmark[^/]*|examples?|samples?)/',p):return 'TEST_EXAMPLE'
 if any(x in p for x in ['/third_party/','/third-party/','/vendor/','/external/','/deps/']):return 'BUNDLED_DEPENDENCY'
 return 'PRODUCTION_LIKE_UNCONFIRMED'
counts=collections.defaultdict(lambda:collections.defaultdict(set));status=[];issues=[];start=time.monotonic()
with gzip.open(out/'hits.tsv.gz','wt',encoding='utf-8',newline='') as hf,gzip.open(out/'files.tsv.gz','wt',encoding='utf-8',newline='') as ff,gzip.open(out/'unqualified_contexts.tsv.gz','wt',encoding='utf-8',newline='') as uf:
 hw=csv.writer(hf,delimiter='\t');fw=csv.writer(ff,delimiter='\t');uw=csv.writer(uf,delimiter='\t')
 hw.writerow(['source_rpm','path','line','qualified_name','detection','area','shape_candidate','source_text','sha256'])
 fw.writerow(['source_rpm','path','size','status','sha256'])
 uw.writerow(['source_rpm','path','line','using_namespace','sha256'])
 for number,pkg in enumerate(packages,1):
  name=pkg['source_rpm'];base=root/name
  item={'source_rpm':name,'extraction_status':pkg['status'],'root':str(base),'eligible_files':0,'read_files':0,'bytes':0,'hit_files':0,'token_hits':0,'using_namespace_files':0,'errors':0}
  if not base.is_dir():item['status']='NOT_OBSERVED_ROOT_MISSING';status.append(item);continue
  def error(e):issues.append({'package':name,'path':str(e.filename),'error':str(e)});item['errors']+=1
  for directory,dirs,files in os.walk(base,followlinks=False,onerror=error):
   dirs[:]=sorted(d for d in dirs if d not in ['.git','.svn','.hg'])
   for filename in sorted(files):
    path=pathlib.Path(directory)/filename
    if path.suffix not in exts and not any(filename.endswith(x+'.in') for x in exts):continue
    item['eligible_files']+=1;rel=str(path.relative_to(base))
    try:
     if path.is_symlink():fw.writerow([name,rel,0,'NOT_OBSERVED_SYMLINK_NOT_FOLLOWED','']);item['errors']+=1;continue
     size=path.stat().st_size
     if size>256*1024*1024:fw.writerow([name,rel,size,'NOT_OBSERVED_SIZE_LIMIT','']);item['errors']+=1;continue
     data=path.read_bytes();digest=hashlib.sha256(data).hexdigest();item['bytes']+=len(data)
     if b'\0' in data:fw.writerow([name,rel,len(data),'NOT_OBSERVED_NUL_ENCODING',digest]);item['errors']+=1;continue
     text=data.decode('utf-8',errors='replace');item['read_files']+=1;fw.writerow([name,rel,len(data),'READ',digest])
     if not any(x in text for x in ['std','__gnu_']):continue
     code,found,using=hits(text)
     if not found and not using:continue
     newlines=[-1]+[m.start() for m in re.finditer('\n',text)];lines=text.splitlines();tag=area(rel)
     if using:item['using_namespace_files']+=1
     for m in using:uw.writerow([name,rel,bisect.bisect_left(newlines,m.start()),m[1],digest])
     if found:item['hit_files']+=1
     for offset,symbol,kind in found:
      line=bisect.bisect_left(newlines,offset);snippet=lines[line-1].strip() if line<=len(lines) else ''
      context=' '.join(lines[max(0,line-2):min(len(lines),line+2)])
      shape='DIRECT_OR_DECLARATION_UNRESOLVED'
      if '(' in context and path.suffix in ['.h','.hh','.hpp','.hxx','.inc']:shape='HEADER_SIGNATURE_CANDIDATE'
      elif ';' in snippet and '(' not in snippet:shape='MEMBER_OR_LOCAL_VARIABLE_UNRESOLVED'
      hw.writerow([name,rel,line,symbol,kind,tag,shape,snippet[:1600],digest]);item['token_hits']+=1
      counts[symbol][tag].add(name)
    except (OSError,UnicodeError) as e:error(e);fw.writerow([name,rel,0,'READ_ERROR',''])
  item['status']='SCANNED_WITH_GAPS' if item['errors'] or pkg['status']!='EXTRACTED_OK' else 'SCANNED_SELECTED_EXTENSIONS'
  status.append(item);save('platform/STATUS.json',status)
  if number%5==0:print('PACKAGES',number,'/',len(packages),'seconds',round(time.monotonic()-start),flush=True)
  if time.monotonic()-start>4*3600:save('platform/STOP.json',{'reason':'FOUR_HOUR_SCAN_BUDGET','last':name});break
save('platform/COUNTS.json',{k:{tag:sorted(v) for tag,v in areas.items()} for k,areas in counts.items()})
save('platform/ISSUES.json',issues)
print('DONE',len(status),'files',sum(p['read_files'] for p in status),'hits',sum(p['token_hits'] for p in status),flush=True)
