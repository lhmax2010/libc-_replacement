#!/usr/bin/env python3
from common import *
from measure import CC,flags
import re
gate('config_audit_gate');reports=[]
for lib in ['gnu','cxx']:
 m=json.loads((OUT/'measurements'/f'T0001_{lib}.json').read_text());src=ROOT/m['source'];opts,_=flags(lib)
 rc,out,ref=record('config_macros_'+lib,[CC,*opts,'-E','-dM',src]);assert rc==0
 macros=[s for s in out.splitlines() if re.match(r'#define (?:_LIBCPP_VERSION |__GLIBCXX__ |__GNUC__ |__GNUC_MINOR__ |_GLIBCXX_RELEASE |_GLIBCXX_USE_CXX11_ABI |_LIBCPP_ABI_NAMESPACE |__SIZEOF_LONG__ |__SIZEOF_POINTER__ |__INT64_TYPE__ |__cplusplus )',s)]
 rc,out,href=record('config_headers_'+lib,[CC,*opts,'-H','-fsyntax-only',src]);assert rc==0
 used={}
 for line in (ROOT/(href+'.stderr')).read_text().splitlines():
  match=re.match(r'^\.+ (/.*)$',line)
  if match:
   p=pathlib.Path(match[1])
   if p.is_file():used[str(p)]=sha(p)
 assert used
 reports.append(dict(library=lib,macros=macros,macro_record=ref,header_record=href,actual_header_sha256=used))
save(OUT/'ACTIVE_CONFIG.json',reports)
for r in reports:print(r['library'],r['macros'],'headers',len(r['actual_header_sha256']))
# 比较本次文字读取与旧 S 的读入异常，不隐去一条新增工具正文。
old=json.loads((ROOT/'docs/progress/R119_batch2/W1/HEADER_COVERAGE.json').read_text())
issues=[r for r in old['issues'] if r['path'].endswith('desc_gentool')]
print('prior extra-file issue',issues)
save(OUT/'HEADER_SCOPE_DELTA.json',dict(old_read=223533,new_raw_read=223534,extra=issues,effective_original_scope_read=223533,note='额外读入的工具正文不作为公开 C++ 签名证据；不把读入数 +1 解释为新增平台 API'))
# Dali 实际定义与存储链。
header=json.loads((OUT/'EDGE_STRUCTURAL_HEADERS.json').read_text())['dali_time'][0]
lines=header['text'].splitlines();print('DALI_ALIASES',[s for s in lines if 'TimePoint' in s or 'std::chrono' in s])
assert any('TimePoint' in s and 'std::chrono' in s for s in lines)
for rel,tokens in [('codes/gcc/libstdc++-v3/include/bits/chrono.h',['rep __r;','duration __d;']),('codes/llvm/libcxx/include/__chrono/duration.h',['rep __rep_;']),('codes/llvm/libcxx/include/__chrono/time_point.h',['duration __d_;'])]:
 p=ROOT/rel;content=p.read_text();assert any(t in content for t in tokens)
 excerpt=[dict(line=i+1,text=s) for i,s in enumerate(content.splitlines()) if any(t in s for t in tokens)]
 save(OUT/'structure'/pathlib.Path(rel).name.replace('.','_').__add__('.json'),dict(path=rel,sha256=sha(p),member_lines=excerpt))
