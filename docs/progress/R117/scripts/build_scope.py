"""汇总范围；不把词法命中、解析差集或使用场景候选当成已验证差异。"""
from common import *
import csv,gzip,re,collections
def tsv(path,rows,fields=None):
 p=OUT/path;p.parent.mkdir(exist_ok=True,parents=True)
 with (gzip.open(p,'wt',encoding='utf-8',newline='') if str(p).endswith('.gz') else p.open('w',newline='')) as f:
  w=csv.DictWriter(f,fieldnames=fields or list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
registry={};availability=collections.defaultdict(set);memberdata=collections.defaultdict(dict);parses=[];inventories=[]
for batch in ['headers','headers_supplement','headers_repair','headers_extra','headers_wrapper','headers_legacy_variables']:
 inventories+=json.loads((OUT/batch/'INVENTORY.json').read_text());parses+=json.loads((OUT/batch/'PARSE_STATUS.json').read_text())
 for line in gzip.open(OUT/batch/'declarations.jsonl.gz','rt'):
  d=json.loads(line);name=d['entity']
  if d['kind']=='NAMESPACE_ALIAS':continue
  e=registry.setdefault(name,{'kinds':set(),'libraries':set(),'locations':collections.defaultdict(set),'headers':collections.defaultdict(set),'modes':collections.defaultdict(set),'parsed':set(),'aliases':set()})
  e['kinds'].add(d['kind']);e['libraries'].add(d['library']);e['locations'][d['library']].add(tuple(d['location']));e['headers'][d['library']].add(d['header']);e['modes'][d['library']].add(d['mode'])
  if d['parse_status']=='PARSED':e['parsed'].add(d['library'])
  if d['original']!=name:e['aliases'].add(d['original'])
  availability[(d['library'],d['header'])].add(name)
 for line in gzip.open(OUT/batch/'members.jsonl.gz','rt'):
  d=json.loads(line);key=(d['library'],d['kind'],d['name'],d['signature'],tuple(d['location']))
  memberdata[d['owner']][key]=d
# Direct public nested types are entities too; member functions remain in the member index.
for owner,entries in list(memberdata.items()):
 for d in entries.values():
  if d['kind'] not in ['CLASS_TEMPLATE','CLASS_TEMPLATE_PARTIAL_SPECIALIZATION','STRUCT_DECL','CLASS_DECL','UNION_DECL','ENUM_DECL','TYPEDEF_DECL','TYPE_ALIAS_DECL','TYPE_ALIAS_TEMPLATE_DECL'] or d['access'] not in ['PUBLIC','INVALID']:continue
  name=owner+'::'+d['name'];e=registry.setdefault(name,{'kinds':set(),'libraries':set(),'locations':collections.defaultdict(set),'headers':collections.defaultdict(set),'modes':collections.defaultdict(set),'parsed':set(),'aliases':set()})
  lib=d['library'];e['kinds'].add('TYPE');e['libraries'].add(lib);e['locations'][lib].add(tuple(d['location']));e['headers'][lib].add(d['header']);e['modes'][lib].add(d['mode'])
  if lib in registry.get(owner,{}).get('parsed',set()):e['parsed'].add(lib)
  availability[(lib,d['header'])].add(name)
print('registry',len(registry),flush=True)
def norm(n):return n.replace('std::__1::','std::').replace('std::__cxx11::','std::').replace('std::views::','std::ranges::views::')
def match(n):
 n=norm(n)
 while n:
  if n in registry:return n
  n=n.rpartition('::')[0]
 return None
counts=json.loads((OUT/'platform/COUNTS.json').read_text());packages={r['source_rpm']:r['package_name'] for r in csv.DictReader((ROOT/'docs/progress/R32/tables/scan_status.tsv').open(),delimiter='\t')}
usage=collections.defaultdict(lambda:collections.defaultdict(set));unmatched=[]
for token,areas in counts.items():
 name=match(token)
 if not name:unmatched.append({'token':token,'source_rpm_count':len(set().union(*(set(v) for v in areas.values()))),'status':'NOT_MATCHED_TO_PARSED_PUBLIC_ENTITY'});continue
 for area,rpms in areas.items():usage[name][area].update(rpms)
examples=collections.defaultdict(dict);shapes=collections.defaultdict(collections.Counter);signature_packages=collections.defaultdict(set)
with gzip.open(OUT/'platform/hits.tsv.gz','rt') as f:
 for d in csv.DictReader(f,delimiter='\t'):
  name=match(d['qualified_name'])
  if not name:continue
  shapes[name][d['shape_candidate']]+=1
  if d['shape_candidate']=='HEADER_SIGNATURE_CANDIDATE' and d['area']=='PRODUCTION_LIKE_UNCONFIRMED':signature_packages[name].add(d['source_rpm'])
  k=d['area']+':'+d['shape_candidate']
  if k not in examples[name]:examples[name][k]=d
print('usage',len(usage),'unmatched_tokens',len(unmatched),flush=True)
tsv('platform/unmatched_tokens.tsv',sorted(unmatched,key=lambda r:-r['source_rpm_count']))
tsv('platform/entity_examples.tsv',[dict(entity=n,**d) for n,ex in sorted(examples.items()) for d in ex.values()])
high={
 'std::string':('docs/progress/P7_0909/stage2/snapshots/evidence/lightweight-web-engine/db4f11ee2748_LWEWebView.h:335;docs/progress/P7_0909/stage2/tables/boundary_signal_index.tsv:2','EWK 输出结构四字段与输入、LWE 参数/返回值；真实消费端范围另核'),
 'std::vector':('docs/progress/P7_0909/stage2/snapshots/evidence/webapi-plugins/353da9cbd3a2_XW_Extension_SyncMessage.h:36;docs/progress/P7_0909/stage2/snapshots/evidence/webapi-plugins/e4597e6f32ca_extension.cc:348','引擎/插件函数表参数与调用；不是普通 JS 直接接触'),
 'std::function':('docs/progress/P7_0909/stage2/snapshots/evidence/lightweight-web-engine/db4f11ee2748_LWEWebView.h:344','LWE 回调签名；SDK 暴露与每个实际调用方未确认')}
layout=list(csv.DictReader((ROOT/'docs/progress/P9_0911/baseline/abi_layout_comparison.tsv').open(),delimiter='\t'))
p9={ 'std::'+r['type'].split('<')[0]:r for r in layout if r.get('same_layout_metrics')=='NO'}
prior=collections.defaultdict(list)
for name in p9:prior[name].append('docs/progress/P9_0911/MEMBER_LAYOUT_ZH.md')
for name in ['std::condition_variable','std::condition_variable_any','std::wbuffer_convert','std::shared_mutex','std::shared_timed_mutex']:prior[name].append('HQ_files/abi_comparison_preliminary.md:四、线程取消相关的差异')
for name in ['std::unordered_map','std::unordered_set','std::hash']:prior[name].append('本轮任务书：评审待查项（原评审逐字出处未定位）')
rows=[];scenarios=[];source_rows=[]
def source_ref(loc):return loc[0]+':'+str(loc[1])
def chapter(name,refs,extension):
 if extension.startswith(('供应商','TS')):return '扩展、实验与兼容设施'
 text=name+' '+' '.join(refs.values())
 # Reading order only, inferred from actual namespace/declaration file; not ISO chapter numbering.
 groups=[('并发、同步与异步结果',r'atomic|mutex|condition_variable|thread|future|promise|packaged_task|latch|barrier|semaphore|stop_token'),
 ('时间与文件系统',r'chrono|filesystem|fs_path|fs_ops|fs_dir'),
 ('异常、类型身份与错误',r'exception|typeinfo|type_info|typeindex|type_index|stdexcept|system_error|error_code|error_condition'),
 ('字符、字符串与编码',r'string|charconv|char_traits|codecvt|text_encoding'),
 ('内存、分配器与所有权',r'memory|alloc|shared_ptr|weak_ptr|unique_ptr|auto_ptr|pointer_traits|/new:'),
 ('容器与迭代器',r'vector|deque|list|map|set|queue|stack|array|span|iterator|hashtable|tree'),
 ('流、格式化与区域设置',r'iostream|ios_base|basic_ios|ostream|istream|fstream|sstream|streambuf|syncstream|spanstream|locale|format|print|iomanip'),
 ('类型特征、概念与通用工具',r'type_traits|concept|utility|tuple|pair|optional|variant|any|compare'),
 ('算法、范围与可调用对象',r'algorithm|functional|ranges|function|bind|invoke|search|sort|execution|coroutine|generator'),
 ('数值、随机与位操作',r'numeric|numbers|random|ratio|complex|valarray|cmath|limits|bit|cfenv')]
 for title,pattern in groups:
  if re.search(pattern,text):return title
 return 'C 兼容入口及其他公开声明'
for i,(name,e) in enumerate(sorted(registry.items()),1):
 sid='API-'+str(i).zfill(5);e['id']=sid
 libs=e['libraries'];uses=usage[name];allrpms=set().union(*uses.values()) if uses else set();prod=uses.get('PRODUCTION_LIKE_UNCONFIRMED',set());families={packages.get(p,p) for p in allrpms};prodfamilies={packages.get(p,p) for p in prod}
 scan_eligible=name.startswith(('std::','__gnu_cxx::','__gnu_parallel::')) and 'operator' not in name
 priority='高（公开跨包签名证据）' if name in high else '未分级（不在限定名扫描覆盖）' if not scan_eligible else '中候选（≥10 包族生产样式命中；包内性未证）' if len(prodfamilies)>=10 else '低候选（有限词法命中）' if allrpms else '零限定名命中（非未使用证明）'
 locs={lib:sorted(e['locations'][lib],key=lambda x:('/__cxx03/' in x[0],x[0],x[1])) for lib in sorted(libs)}
 refs={lib:source_ref(v[0]) for lib,v in locs.items()}
 extension='供应商扩展命名空间/头文件' if name.startswith(('__gnu_','std::__debug')) or any('/ext/' in x[0] or '/parallel/' in x[0] or '/debug/' in x[0] for xs in locs.values() for x in xs) else 'TS/TR/experimental（非当前标准主体）' if '::experimental::' in name or '::tr1::' in name or '::tr2::' in name else '标准入口的公开声明候选；标准归属逐项未规范核验'
 libstate='两侧有声明（非等价证明）' if len(libs)==2 else '仅 '+next(iter(libs))+' 解析命中；另一侧 NOT_OBSERVED，非已证缺失'
 if len(e['parsed'])<len(libs):libstate+='；含仅错误恢复 AST 的声明'
 membernames={d['name'] for d in memberdata[name].values()};kind=';'.join(sorted(e['kinds']))
 scene=[];points=[]
 if 'TYPE' in e['kinds']:
  scene=['构造/析构、赋值、拷贝/移动（可用性及删除操作待按实例核验）','作为成员/容器元素/map key（需求约束待验）','跨线程/DSO 生命周期与所有权（非安全承诺）']
  points=['表示/对齐/内联 ABI、分配与释放归属可能不同（未查证）','异常/错误状态及强制展开路径待查（不得一概适用）']
  if membernames & {'begin','end','cbegin','rbegin'}:scene.append('迭代、失效、顺序');points.append('迭代器表示/失效、顺序约束待查；不得默认未规定顺序')
  if membernames & {'data','c_str','size'}:scene.append('C API 交互：'+','.join(sorted(membernames & {'data','c_str','size'})));points.append('指针生命周期、长度单位/编码、终止符待查')
  if membernames & {'compare','operator==','operator<=>','operator()'} or name in ['std::string','std::hash']:scene.append('比较/hash/调用（按实际重载和特化）');points.append('相等性、排序、hash 值可重复性及持久化假设待查')
  if 'unordered_' in name:scene.append('插入/擦除/rehash 后迭代顺序与 bucket 分布');points.append('迭代顺序、hash 值、桶/增长策略可能不同，未验证')
  if name in ['std::string','std::basic_string']:scene.append('空串/短串/长串、嵌入 NUL、reserve/shrink_to_fit');points.append('短串容量、容量增长、调用前状态、分配失败回滚待查')
 else:
  scene=['声明可用模式/重载或模板替换/调用及返回（按实体类别适用）','显式限定名与 ADL/宏/别名（未解析调用图）']
  points=['约束、可用宏/模式、返回/错误与异常规格可能不同，尚未查证']
 if name in high:
  scene += ['同侧/双向跨库、旧对象与新头、分配失败/越界、销毁','比较/hash（无对应操作时核验为不适用）、容器嵌套、map key 约束']
  scenarios.append({'id':sid,'entity':name,'basis':high[name][0],'construct_assign_copy_move':'逐重载及 deleted/allocator 传播；只拟定待查场景','iteration_order':'按 begin/end 声明确认；std::function 非容器，迭代不适用；string/vector 按序内容核验','compare_hash':'比较操作与 std::hash 特化是否存在先核验；不臆造 vector/function hash','nested_element_map_key':'对象作为元素、移动限制；作为 key 所需 Compare/Hash/Equal 单列，不预设可用','thread_dso':'共享读/写条件、同步、回调重入、跨 DSO 创建/使用/销毁、卸载后寿命','c_api':'string: data/c_str/size 与 NUL/编码；vector: data/size 与空缓冲、vector<bool> 另列；function: 回调+上下文桥，不存在 data/c_str','errors':'分配失败、长度/边界错误、空 function 调用；取消单列，非统一异常类型','additional':'自定义 allocator/traits、短长数据、旧输出复用、内联副本、编译/链接/运行时身份','possible_differences':'全部是候选：布局/容量/分配、缓冲、错误/回滚、回调销毁及符号版本；本轮不验证','operation_sources':';'.join(refs.values()),'member_index':'public_members.tsv.gz'})
 r={'章':('扩展与移植入口' if extension.startswith(('供应商','TS')) else '公开类型' if 'TYPE' in e['kinds'] else '函数/变量/概念'), 'ID':sid,'类型/函数':name,'实体类别':kind,'两边是否都有':libstate,'平台使用包数':str(len(allrpms))+'（限定名命中 source RPM；语义实际使用数 NOT_OBSERVED）' if scan_eligible else 'NOT_OBSERVED（扫描命名空间/操作符覆盖之外）','去重包族数':len(families) if scan_eligible else 'NOT_OBSERVED','生产样式包族数':len(prodfamilies) if scan_eligible else 'NOT_OBSERVED','优先级':priority,'用法场景':'；'.join(scene),'可能差异点':'；'.join(points),'对应既有材料':';'.join(prior[name]),'libcxx源码':refs.get('libcxx','NOT_OBSERVED'),'libstdcxx源码':refs.get('libstdcxx','NOT_OBSERVED'),'扩展标注':extension,'用法形态证据':';'.join(k+'='+str(v) for k,v in sorted(shapes[name].items())) or 'NOT_OBSERVED','跨包证据':high.get(name,('NOT_OBSERVED：词法头签名不证明跨包',''))[0]}
 r['章']=chapter(name,refs,extension)
 rows.append(r)
 for lib,xs in locs.items():
  for loc in xs:source_rows.append({'id':sid,'entity':name,'library':lib,'path':loc[0],'line':loc[1],'column':loc[2],'headers':';'.join(sorted(e['headers'][lib])),'modes':';'.join(sorted(e['modes'][lib]))})
tsv('SKELETON.tsv',rows);tsv('HIGH_PRIORITY_SCENARIOS.tsv',scenarios);tsv('entity_sources.tsv.gz',source_rows)
tsv('public_members.tsv.gz',[dict(owner=n,library=d['library'],kind=d['kind'],name=d['name'],signature=d['signature'],type=d['type'],path=d['location'][0],line=d['location'][1],access=d['access']) for n,entries in sorted(memberdata.items()) for d in entries.values()])
tsv('header_entities.tsv.gz',[{'library':lib,'header':h,'entity':n,'id':registry[n]['id'],'kinds':';'.join(sorted(registry[n]['kinds']))} for (lib,h),names in sorted(availability.items()) for n in sorted(names)])
tsv('platform/entity_package_counts.tsv',[{'id':registry[n]['id'],'entity':n,'area':a,'source_rpm_count':len(p),'package_family_count':len({packages.get(x,x) for x in p}),'source_rpms':';'.join(sorted(p))} for n,v in sorted(usage.items()) for a,p in v.items()])
tsv('platform/INTERFACE_CANDIDATES.tsv',[{'id':registry[n]['id'],'entity':n,'source_rpm_count':len(p),'package_family_count':len({packages.get(x,x) for x in p}),'source_rpms':';'.join(sorted(p)),'classification':'HEADER_SIGNATURE_HEURISTIC_NOT_CROSS_PACKAGE_PROOF','example_index':'entity_examples.tsv','actual_cross_package_count':'NOT_OBSERVED'} for n,p in sorted(signature_packages.items()) if 'TYPE' in registry[n]['kinds']])
# Full per-header presence is an observed filesystem fact, not proof of API absence.
headerlibs=collections.defaultdict(set);entrysources=collections.defaultdict(set)
for h in inventories:headerlibs[h['header']].add(h['library']);entrysources[h['header']].add(h['path'])
tsv('HEADER_SETS.tsv',[{'header':h,'presence':'BOTH' if len(l)==2 else next(iter(l))+'_ONLY_IN_INVENTORY','paths':';'.join(sorted(entrysources[h])),'entity_absence_inference':'禁止由入口文件差集推导实体不存在'} for h,l in sorted(headerlibs.items())])
save('SUMMARY.json',{'entities':len(rows),'header_names':len(headerlibs),'header_names_by_library':{lib:sum(lib in l for l in headerlibs.values()) for lib in ['libcxx','libstdcxx']},'both_entities':sum(len(e['libraries'])==2 for e in registry.values()),'only_observed_one_side':sum(len(e['libraries'])==1 for e in registry.values()),'priority':dict(collections.Counter(r['优先级'] for r in rows)),'parse_status':dict(collections.Counter(p['status'] for p in parses)),'parse_operations':len(parses),'matched_usage_entities':sum(bool(v) for v in usage.values()),'unmatched_tokens':len(unmatched),'high_entities':list(high),'interface_candidate_type_names':sum('TYPE' in registry[n]['kinds'] for n in signature_packages)})
md=['# 两套标准库差异清单骨架（范围候审，不是差异结论）','','全表逐行由实际声明位置生成；重载合并，详细声明/成员/模式另见索引。包数只表示已扫文件的限定名命中，不等于实际构建使用。单侧解析命中不等于另一侧不存在。所有“可能差异”均未在本轮查证。','', '| 章 | ID／类型或函数 | 两侧声明 | 包数（source RPM 词法） | 优先级 | 用法场景 | 可能差异点 | 既有材料／源码 |','|---|---|---|---|---|---|---|---|']
def esc(x):return str(x).replace('|','\\|').replace('\n',' ')
for r in rows:md.append('| '+' | '.join(esc(x) for x in [r['章'],r['ID']+' '+r['类型/函数'],r['两边是否都有'],r['平台使用包数'],r['优先级'],r['用法场景'],r['可能差异点'],r['对应既有材料']+';'+r['libcxx源码']+';'+r['libstdcxx源码']])+' |')
(OUT/'SKELETON.md').write_text('\n'.join(md)+'\n')
print('DONE',len(rows),flush=True)
