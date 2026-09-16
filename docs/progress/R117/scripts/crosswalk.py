from common import *
import csv,re,collections
rows=list(csv.DictReader((OUT/'SKELETON.tsv').open(),delimiter='\t'));byname={r['类型/函数']:r for r in rows}
def refs(names):return ';'.join(byname[n]['ID']+':'+n for n in names if n in byname)
def sources(names):return ';'.join(sorted({r[k] for n in names if (r:=byname.get(n)) for k in ['libcxx源码','libstdcxx源码'] if r[k]!='NOT_OBSERVED'}))
def write(name,rows):
 with (OUT/name).open('w',newline='') as f:
  w=csv.DictWriter(f,rows[0].keys(),delimiter='\t');w.writeheader();w.writerows(rows)
cross=[];risks=list(csv.DictReader((ROOT/'docs/progress/R66/tables/risk_inventory.tsv').open(),delimiter='\t'))
mapping={
1:('取消标记/异常展开/personality','exception;exception_ptr','codes/gcc/libstdc++-v3/libsupc++/cxxabi_forced.h:48'),
2:('无异常配置与防护条件','terminate;exception','codes/llvm/libcxx/include/__config:1'),
3:('异常对象应急分配','bad_alloc;exception_ptr','codes/gcc/libstdc++-v3/libsupc++/eh_alloc.cc:45'),
4:('glibc 与运行库组合','getenv;filesystem::path','codes/gcc/libstdc++-v3/libsupc++/eh_alloc.cc:195'),
5:('异步结果/流状态机','future;promise;packaged_task;async;basic_ios;basic_ostream;exception_ptr','codes/llvm/libcxx/include/future:1'),
6:('扩展/私有头与宏/成员引用','', 'codes/gcc/libstdc++-v3/include/ext/vstring.h:59'),
7:('导出符号集合','', 'codes/gcc/libstdc++-v3/config/abi/pre/gnu.ver:1'),
8:('版本化符号和旧 ELF','condition_variable', 'codes/gcc/libstdc++-v3/config/abi/pre/gnu.ver:1'),
9:('同名 ABI 与错误配对','logic_error;runtime_error;nested_exception;exception_ptr;uncaught_exceptions','codes/gcc/libstdc++-v3/libsupc++/exception_ptr.h:1'),
10:('类型身份/RTTI/异常匹配','type_info;type_index;exception_ptr', 'codes/llvm/libcxxabi/src/cxa_personality.cpp:1'),
11:('对象/节点/迭代器/内联布局','vector;deque;list;map;set;unordered_map;atomic;string;shared_ptr', 'codes/llvm/libcxx/include/__vector/vector.h:87'),
12:('GNU 双 ABI 与配置条件','basic_string;basic_stringbuf;list','codes/gcc/libstdc++-v3/include/bits/c++config:336'),
13:('分配器与跨边界释放','allocator;allocator_traits;shared_ptr;unique_ptr;exception_ptr','codes/llvm/libcxx/include/memory:1'),
14:('静态复制/全局状态与 DSO','basic_ios;exception_ptr','codes/llvm/libcxxabi/src/cxa_exception.cpp:1'),
15:('第三方 C++ 边界与 provider/consumer 图','string;vector;function','docs/progress/P7_0909/stage2/snapshots/evidence/webapi-plugins/353da9cbd3a2_XW_Extension_SyncMessage.h:36'),
16:('共享测试框架的标准对象接口','string;basic_ostream','docs/progress/R109/tables/w3_edges_90.tsv:2'),
17:('C 名称下的 C++ 对象与生命周期','string;vector','docs/progress/P7_0909/stage2/tables/boundary_signal_index.tsv:2'),
18:('不可重编的预制产物','', 'docs/progress/R66/VENDOR_AUDIT.md:1'),
19:('供应商源码/构建可得性','', 'docs/progress/R66/VENDOR_AUDIT.md:1'),
20:('driver 搜索与实际加载身份','', 'docs/progress/R66/DEFENSIVE_AND_ABI.md:1'),
21:('实现允许的顺序/缓冲/区域与时区行为','deque;basic_filebuf;basic_stringbuf;locale;chrono::tzdb;unordered_map;unordered_set;hash','codes/llvm/libcxx/include/deque:1'),
22:('ARM EHABI、展开器与架构矩阵','exception_ptr;exception', 'codes/llvm/libcxxabi/src/cxa_personality.cpp:1'),
23:('插件加载/卸载后的对象寿命','exception_ptr;shared_ptr;function', 'docs/progress/R66/THIRD_PARTY.md:1'),
24:('配置/架构/语言模式/测试发现覆盖','', 'codes/llvm/libcxx/utils/libcxx/header_information.py:1'),
25:('构建硬编码链接库与运行库组合','', 'docs/progress/R66/SOURCE_COMPAT.md:1')}
for row in risks:
 num=int(row['id'].split('-')[1]);title,short,anchor=mapping[num];names=['std::'+n for n in short.split(';') if n];cid='CROSS-'+str(num).zfill(2)
 cross.append({'chapter_id':cid,'scope':title,'related_entities':refs(names),'source_anchor':anchor,'existing_material':'docs/progress/R66/tables/risk_inventory.tsv:'+str(risks.index(row)+2),'former_item':row['id'],'former_text':row['item'],'unit':'横切维度，不是新增公开类型/函数','remaining':'下一阶段验证；保留既有结论的版本/配置边界，不在本轮复验'})
write('CROSSCUTTING_SCOPE.tsv',cross)
links=[]
def link(group,item,old,names,extra='',status='MAPPED'):
 links.append({'group':group,'item':item,'original':old,'skeleton_rows':refs(names),'crosscutting_rows':extra,'header_sources':sources(names),'status':status,'limits':'只映射范围，不重申旧数字为当前事实；重叠项不重复计数'})
p9=list(csv.DictReader((ROOT/'docs/progress/P9_0911/baseline/abi_layout_comparison.tsv').open(),delimiter='\t'))
for i,p in enumerate(p9,2):
 if p.get('same_layout_metrics')=='NO':link('P9 十个类型',p['type'],'docs/progress/P9_0911/baseline/abi_layout_comparison.tsv:'+str(i),['std::'+p['type'].split('<')[0]],'CROSS-11')
for r in cross:link('R66 二十五项',r['former_item'],r['existing_material'],[],r['chapter_id'],'MAPPED_CROSSCUTTING_NOT_A_SINGLE_ENTITY')
for item,names,c in [
 ('标记类型',['std::exception'],'CROSS-01'),('typed catch 防护位置',['std::basic_ostream','std::future'],'CROSS-01;CROSS-05'),('wait 异常规格',['std::condition_variable'],'CROSS-01;CROSS-08'),('析构同步',['std::wbuffer_convert'],'CROSS-21'),('读写锁后端/状态回滚',['std::shared_mutex','std::shared_timed_mutex'],'CROSS-01;CROSS-24')]:link('ABI 报告第四节',item,'HQ_files/abi_comparison_preliminary.md:四、线程取消相关的差异',names,c)
seven=[
 ('应用 catch-all 吞强制展开',['std::exception','std::exception_ptr'],'CROSS-01;CROSS-10;CROSS-22',34),
 ('标准库内部 catch-all 防护',['std::basic_ios','std::basic_ostream','std::future','std::packaged_task'],'CROSS-01;CROSS-05',214),
 ('noexcept/锁状态/内联重编边界',['std::condition_variable','std::condition_variable_any','std::wbuffer_convert','std::shared_mutex'],'CROSS-01;CROSS-08;CROSS-11',308),
 ('八处对应关系与扩展',['std::condition_variable_any','__gnu_cxx::__versa_string','std::tr2::dynamic_bitset'],'CROSS-06',492),
 ('string 收缩与 ostream sentry 析构',['std::basic_string','std::basic_ostream'],'CROSS-01;CROSS-13',525),
 ('异步取消可能绕过防护',['std::atomic'],'CROSS-01;CROSS-22;CROSS-24',656),
 ('其他二十五项风险，不再新增二十五个类型',[],'CROSS-01 至 CROSS-25',686)]
for i,(title,names,c,line) in enumerate(seven,1):link('七个问题',str(i)+' '+title,'headers/prior_seven_issues.md:'+str(line),names,c)
for title,names,c,origin,status in [
 ('无序容器迭代顺序',['std::unordered_map','std::unordered_set'],'CROSS-21','本轮用户任务书第四步 8e','MAPPED_ORIGINAL_REVIEW_QUOTE_NOT_OBSERVED'),
 ('hash 值稳定性/跨库/持久化',['std::hash','std::unordered_map','std::unordered_set'],'CROSS-21','本轮用户任务书第四步 8e','MAPPED_ORIGINAL_REVIEW_QUOTE_NOT_OBSERVED'),
 ('类型信息比较策略',['std::type_info','std::type_index'],'CROSS-10','review_libc++_libstdc++_adaptor.txt','MAPPED_NOT_REASSESSING_OLD_EXPERIMENT_STATUS'),
 ('GNU 空字符串存储释放',['std::basic_string'],'CROSS-12;CROSS-13','review_libc++_libstdc++_adaptor.txt','MAPPED_NOT_REASSESSING_OLD_EXPERIMENT_STATUS'),
 ('libsupc++ 与展开器组合',['std::exception_ptr'],'CROSS-01;CROSS-10;CROSS-22','docs/progress/P11_0916/','MAPPED_CONFIGURATION_AXIS'),
 ('veneer/薄转发层路由',['std::string','std::vector'],'CROSS-08;CROSS-17;CROSS-23','review_libc++_libstdc++_adaptor.txt','MAPPED_DEPLOYMENT_AXIS_NOT_PUBLIC_ENTITY')]:link('评审议题',title,origin,names,c,status)
write('EXISTING_MATERIAL_CROSSWALK.tsv',links)
write('UNMATCHED_OR_NONENTITY.tsv',[r for r in links if not r['skeleton_rows'] or r['status']!='MAPPED'])
md=['# 既有材料对照','','这里核对的是范围落点，不重复验证旧实验，也不把旧版报告的统计复制为当前统计。','', '| 原材料组 | 项 | 对应实体 | 横切维度 | 状态 |','|---|---|---|---|---|']
for r in links:md.append('| '+' | '.join(r[k].replace('|','\\|') for k in ['group','item','skeleton_rows','crosscutting_rows','status'])+' |')
md+=['','## 无法放进单一类型/函数行的项目','','运行库、符号版本、编译模式、预制二进制、实际加载身份与测试发现是横切维度。它们已在 CROSSCUTTING_SCOPE.tsv 单列，不伪造为公开标准库类型。','无序容器顺序/hash 值已依本轮任务要求纳入；在已读 adaptor 评审原文中未定位其逐字引文，因此未冒称找到了原评审出处。','七个问题的原件不在当前分支 docs/hq；从同一工作区旧材料目录读取并冻结到 headers/prior_seven_issues.md。范围映射保留原件版本，并引用后续修正记录 R74/R75；不据旧件宣称当前补丁状态。']
(OUT/'EXISTING_MATERIAL_CROSSWALK.md').write_text('\n'.join(md)+'\n')
print('crosswalk',len(links),'risk_items',len(cross))
