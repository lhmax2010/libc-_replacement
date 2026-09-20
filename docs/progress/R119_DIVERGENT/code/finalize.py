#!/usr/bin/env python3
"""汇总已测量/已逐条核查的证据；不把词法命中提升为公开签名。"""
from common import *
import collections,re
gate('finalize_gate')
rows=json.loads((OUT/'TYPE_RESULTS.json').read_text());lookup={r['id']:r for r in rows}
direct=json.loads((OUT/'DIRECT_EXPOSURE_HITS.json').read_text())
headers=json.load(gzip.open(OUT/'HEADER_HITS.json.gz','rt'))
selected=json.loads((OUT/'EXPOSURE_SELECTED_HEADERS.json').read_text())
# 下面是人工逐条阅读的声明位置，不是按名称自动断言公开。
specs=[
 ('T0003','/dali/devel-api/adaptor-framework/file-loader.h',[75,87,95],'Dali::FileLoader::ReadFile / GetFileSize','参数引用/返回值','ReadFile 消费者有匹配；三种 adaptor provider 同 SONAME，精确部署绑定未定'),
 ('T0008','/cynara/storage/ChecksumStream.h',[53,54],'Cynara::ChecksumStream::flags','参数/返回值','S 内指定函数身份无异包 UND；非平台级阴性'),
 ('T0010','/cynara/storage/ChecksumStream.h',[51],'Cynara::ChecksumStream::open','参数','S 内指定函数身份无异包 UND；非平台级阴性'),
 ('T0001','/podofo/base/PdfInputDevice.h',[103,121,132],'PoDoFo::PdfInputDevice::Tell / Seek / Read','参数/返回值','Tell/Seek 查询有定义，无异包 UND；Read 尚未按身份核查'),
 ('T0011','/podofo/base/PdfInputDevice.h',[121],'PoDoFo::PdfInputDevice::Seek','参数','S 内指定函数身份无异包 UND；虚调用不由该阴性排除'),
 ('T0009','/podofo/base/PdfInputDevice.h',[148],'PoDoFo::PdfInputDevice::Clear','参数','内联虚方法；S 内指定函数身份无异包 UND，不排除虚调用'),
 ('T0072','/glibmm-2.4/glibmm/ustring.h',[229],'Glib::get_unichar_from_std_iterator','参数','S 内指定函数身份无异包 UND；迭代器包装另需结构链'),
 ('T0416','/pxr/base/arch/threads.h',[51],'ArchGetMainThreadId','返回值','S 内指定函数身份无异包 UND；返回类型不由修饰名确认'),
 ('T0576','/libxml++-5.0/libxml++/exceptions/wrapped_exception.h',[41],'xmlpp::wrapped_exception::wrapped_exception','参数','S 内指定函数身份无异包 UND'),
 ('T0576','/opencv4/opencv2/core/detail/async_promise.hpp',[47],'cv::AsyncPromise::setException','参数','存在重载；命中的异包链接为 cv::Exception const&，不能继承给 exception_ptr'),
 ('T0368','/usr/include/absl/time/time.h',[1031],'absl::FromChrono','参数引用','S 内 FromChrono 查询无异包 UND；内联/模板途径未排除'),
 ('T0377','/usr/include/absl/time/time.h',[661,662],'absl::FromChrono','参数','S 内 FromChrono 查询无异包 UND'),
 ('T0379','/usr/include/absl/time/time.h',[663,664],'absl::FromChrono','参数','S 内 FromChrono 查询无异包 UND'),
 ('T0381','/usr/include/absl/time/time.h',[665,666],'absl::FromChrono','参数','S 内 FromChrono 查询无异包 UND'),
 ('T0383','/usr/include/absl/time/time.h',[667,668],'absl::FromChrono','参数','S 内 FromChrono 查询无异包 UND'),
 ('T0385','/usr/include/absl/time/time.h',[669,670],'absl::FromChrono','参数','S 内 FromChrono 查询无异包 UND'),
 ('T0387','/usr/include/absl/time/time.h',[671,672],'absl::FromChrono','参数','S 内 FromChrono 查询无异包 UND'),
]
public=[]
for id,suffix,lines,identity,form,note in specs:
 refs=[ref for h in headers for ref in h['references'] if ref['cache_path'].endswith(suffix)]
 refs={r['cache_path']:r for r in refs};assert refs,(id,suffix)
 # 完整、未截断的所有匹配路径；相同内容的架构变体不作测量外推。
 for p,ref in refs.items():
  assert sha(p)==ref['file_sha256'];text=pathlib.Path(p).read_text(errors='replace').splitlines()
  snippet='\n'.join(f'{n}: {text[n-1]}' for n in lines)
  assert lookup[id]['expression'] in snippet,(id,snippet)
  selected[p]=dict(path=p,sha256=sha(p),references=[ref],text='\n'.join(f'{i+1}: {s}' for i,s in enumerate(text)))
  public.append(dict(id=id,expression=lookup[id]['expression'],package=ref['sourcerpm'],header=ref['path'],lines=','.join(map(str,lines)),identity=identity,form=form,status='PUBLIC_DECLARATION_CONFIRMED',boundary_note=note,source_sha256=sha(p),source_path=p,snippet=snippet))
save(OUT/'EXPOSURE_SELECTED_HEADERS.json',selected);tsv(OUT/'PUBLIC_DECLARATIONS.tsv',public)
assoc=list(csv.DictReader((OUT/'CONFIRMED_ASSOCIATIONS.tsv').open(),delimiter='\t'))
cross=collections.defaultdict(list)
for r in assoc:
 for id in r['ranks'].split(';'):cross[id].append(r['id'])
pub=collections.defaultdict(list)
for r in public:pub[r['id']].append(r)
exposure=[]
for r in direct:
 id=r['id'];bound=cross[id];p=pub[id]
 exposure.append(dict(id=id,expression=r['expression'],result='CROSS_PACKAGE' if bound else ('PUBLIC_DECLARATION_CONFIRMED_CONSUMER_NOT_CLOSED' if p else 'NOT_OBSERVED'),qualified_root_positions=r['qualified_root_positions'],exact_spelling_positions=r['exact_spelling_positions'],confirmed_associations=';'.join(bound),public_identities=';'.join(sorted({x['identity'] for x in p})),evidence='CONFIRMED_ASSOCIATIONS.tsv;PUBLIC_DECLARATIONS.tsv;DIRECT_EXPOSURE_HITS.json#'+id,missing=('其它消费者覆盖仍不穷尽' if bound else ('具体函数查询及洞见 PUBLIC_DECLARATIONS.tsv，不能把重载阳性或返回类型名字交集继承过来' if p else '词法命中未闭合为该固定实例的公开签名；别名、模板实参、成员结构及函数身份绑定待定；不据零命中判不存在'))))
assert len(exposure)==242;tsv(OUT/'DIVERGENT_EXPOSURE.tsv',exposure)
tsv(OUT/'ARMV7L_RETEST.tsv',[dict(id=r['id'],expression=r['expression'],x86_64=r['result'],armv7l='NOT_OBSERVED',reason='底层类型、指针宽度、库配置、枚举及模板实现均需在目标配置重测；不预测两侧是否相同') for r in rows])
old=list(csv.DictReader((ROOT/'docs/progress/R119_RECLASSIFY/CROSS_PACKAGE_PAIRS.tsv').open(),delimiter='\t'))
edges=list(csv.DictReader((OUT/'CROSS_SYMBOL_EDGES_UPDATED.tsv').open(),delimiter='\t'))
pairs={(r['consumer_package'],r['provider_package']):dict(r) for r in old}
for e in assoc:
 key=(e['consumer_package'],e['provider_package'])
 if key not in pairs:pairs[key]={k:e[k] for k in old[0]};pairs[key]['id']='divergent_pair_'+str(len(pairs)+1)
 else:
  p=pairs[key];p['candidates']=';'.join(sorted(set(p['candidates'].split(';'))|set(e['candidates'].split(';'))));p['candidate_count']=len(p['candidates'].split(';'));p['ranks']+=';'+e['ranks'];p['evidence']+=';'+e['evidence']
assert len(pairs)==18 and len(edges)==23;tsv(OUT/'CROSS_PACKAGE_PAIRS_UPDATED.tsv',list(pairs.values()))
# 改进不可得的解释不改变原始测量输出，也不修改旧任务材料。
annotated=0
for p in (OUT/'measurements').glob('*.json'):
 m=json.loads(p.read_text())
 if '所请求实例中无此类型成员' in m.get('reason',''):
  m['reason']=m['reason'].replace('所请求实例中无此类型成员','请求成员类型在本次访问上下文不可形成（可能未声明或不可访问），不推断不存在');save(p,m);annotated+=1
versions=[]
for proof in json.loads((OUT/'CONFIRMED_EDGE_PROOFS.json').read_text()):
 if proof['kind']=='zypp_storage':
  assert all('@@ZYPP_plain' in s for s in proof['provider_lines']);assert all('@ZYPP_plain' in s for s in proof['consumer_lines'])
  versions.append(dict(raw_symbol=proof['raw_symbol'],required='ZYPP_plain',provided='ZYPP_plain',matched=True))
save(OUT/'VERSION_CHECK.json',versions)
save(OUT/'EXPOSURE_SUMMARY.json',dict(status_counts=dict(collections.Counter(r['result'] for r in exposure)),public_declaration_rows=len(public),public_expression_count=sum(bool(v) for v in pub.values()),cross_expression_count=sum(bool(v) for v in cross.values()),notes_reworded=annotated,queries=len(json.loads((OUT/'BOUNDARY_FUNCTION_QUERIES.json').read_text()))))
print((OUT/'EXPOSURE_SUMMARY.json').read_text())
