from common import *
import csv,re
def table(headers,rows):
 def cell(s):return str(s).replace('|','\\|').replace('\n','<br>')
 return '| '+' | '.join(headers)+' |\n| '+' | '.join(['---']*len(headers))+' |\n'+''.join('| '+' | '.join(cell(c) for c in r)+' |\n' for r in rows)
def ref(name):return f'[{name}]({name})（SHA256 `{sha(OUT/name)}`）'
ms=json.loads((OUT/'MEASUREMENTS.json').read_text());assert len(ms)==16
typ=list(csv.DictReader((OUT/'TYPES.tsv').open(),delimiter='\t'));assert len(typ)==8 and all(r['result']=='DIVERGENT' for r in typ)
qs=json.loads((OUT/'BOUNDARY_QUERIES.json').read_text());sy=json.loads((OUT/'SEEK_SYMBOLS.json').read_text())
exp=json.loads((OUT/'prior_DIVERGENT_EXPOSURE.json').read_text());byid={r['id']:r for r in exp}
assert sum(int(r['exact_spelling_positions']) for r in exp)==245
defs=[]
for lib,config in [(c['library'],c) for c in json.loads((OUT/'CONFIGURATIONS.json').read_text()) if c['arch']=='armv7l']:
 headers=config['header_sha256']
 path=next(p for p in headers if p.endswith('/bits/ios_base.h')) if lib=='gnu' else next(p for p in headers if p.endswith('/v1/ios'))
 assert sha(path)==headers[path]
 original=pathlib.Path(path).read_text();lines=original.splitlines();selected=[]
 clean=re.sub(r'/\*.*?\*/',lambda m:'\n'*m[0].count('\n'),original,flags=re.S)
 clean=re.sub(r'//[^\n]*','',clean)
 for i,line in enumerate(clean.splitlines()):
  if ('enum ' in line or 'typedef ' in line) and any(t in line for t in ['Fmtflags','Iostate','Openmode','Seekdir','fmtflags','iostate','openmode','seekdir']):
   selected.append(dict(line=i+1,context='\n'.join(f'{j+1}: {lines[j]}' for j in range(max(0,i-1),min(len(lines),i+10)))))
 defs.append(dict(library=lib,path=path,sha256=headers[path],excerpts=selected))
save('ENUM_DEFINITIONS.json',defs)
types_table=table(['架构','表达式','GNU 实际类型 / 编码','libc++ 实际类型 / 编码','GNU size/align','libc++ size/align'],[[r['arch'],r['expression'],r['gnu_type']+' / `'+r['gnu_encoding']+'`',r['cxx_type']+' / `'+r['cxx_encoding']+'`',r['gnu_size']+'/'+r['gnu_align'],r['cxx_size']+'/'+r['cxx_align']] for r in typ])
exposure_table=table(['类型','旧索引位置','已确认身份 / 签名条数','头文件位置','本轮定义 / UND / 异包配对'],[[r['expression'],byid[r['id']]['exact_spelling_positions'],r['query'].rstrip('(')+' / '+str(len(r['declaration']['lines'].split(','))),r['declaration']['header']+':'+r['declaration']['lines'],f"{len(r['definitions'])} / {len(r['all_references'])} / {len(r['other_source_links'])}"] for r in qs])
body=f'''# 四项流状态类型的目标架构复测与公开暴露核查

状态：实验与限定暴露核查完成，待人工审阅；**不下产品兼容性结论**。

## 一、结论

- armv7l：4/4 分歧；aarch64：4/4 分歧。两库各五轮一致；两架构均使用 QEMU 用户态，没有占用开发板。
- fmtflags / iostate / openmode：GNU 为各自具名枚举，libc++ 为 unsigned int；seekdir 是两个不同枚举。全部 size=4、align=4，类型身份和实际编码仍不同。
- 真 ARM podofo devel 头编译所得 Seek 名称不同；普通虚调用编译端选中的虚表索引却均为 6。该组合不能由符号差异推导槽位不同，更不能推导业务安全。
- 复用既有公开暴露记录：四种类型、四个函数身份、五条签名；其旧索引位置分别为 61 / 85 / 87 / 12，共 245。位置数量不等于公开签名数。
- 对上述已确认身份的限定查询未发现异包 UND，新增确认包对/符号边均为 0；x86_64 仍为 18/23。先前已证明的两个 ARM 包对/符号边不增不减，ARM 全集仍未查。不是平台级“无消费者”。

## 二、探针、配置与具体数据

{types_table}

数据：{ref('TYPES.tsv')}；完整 TypeToken 编码、每格源/可执行文件 SHA、五次原输出与编译命令索引：{ref('MEASUREMENTS.json')}。

直接复用原测量器的 PRE/source 生成逻辑，emit、TypeToken、四个正向编码控制不改；仅选取这四个表达式。原 ARM types.cpp 的版本与目标尺寸打印也原样保留。两套库版本实测为 `_GLIBCXX_RELEASE=14` 和 `_LIBCPP_VERSION=220108`。{ref('PROBE_REUSE.json')}

配置的 --target、sysroot、标准模式、优化、链接选项逐项复用既有 JSON；原头文件摘要先硬校验，再编译。armv7l 为 `armv7l-tizen-linux-gnueabi`，编译产生 Thumb ARM 目标；aarch64 为 `aarch64-tizen-linux-gnu`。前者 `/usr/bin/qemu-arm-static`，后者 `/usr/bin/qemu-arm64-static`。QEMU 只执行结果打印，类型选择由编译时目标头/宏决定。{ref('CONFIGURATIONS.json')}

单架构为 4 表达式 × 2 库 × 5 轮 = 40 条目标结果观察，装在 10 次进程运行中；两架构合计 16 个类型/库/架构格、80 条目标结果观察、20 次进程运行。正向控制每次执行并断言 int/long/long long/const long& 的编码；无不可得格被替换实参。实际定义摘录和头文件 SHA：{ref('ENUM_DEFINITIONS.json')}。

## 三、真实 ARM Seek 声明及虚调用

真实头：`podofo-devel-0.9.7-0.armv7l.rpm`，RPM SHA256 `6d7c622f861f505dc2b16558bf7b633effbe87d2d835b23a9ebf0509b37cc81e`；来源 revision `platform/upstream/podofo#96f17d0ba63484f444d003128280c370f9476ce3`。`/usr/include/podofo/base/PdfInputDevice.h:121`：

```cpp
virtual void Seek(std::streamoff off, std::ios_base::seekdir dir = std::ios_base::beg);
```

头文件 SHA256 `d92f794449a30db68e2f100d30c4275aa78884daf1bc3751d196b62dfdd9d467`。已将实际使用的 podofo 头与 RPM 内文件摘要逐项绑定，不凭提取目录名推定来源。{ref('RPM_BINDING.json')} {ref('SEEK_HEADER.json')}

探针有两种调用：显式类限定调用只为抑制虚派发、显露真实声明的 UND 名称；普通 `d.Seek(off, dir)` 保留虚派发。两库各编译一次对象文件，另生成 LLVM IR。没有手写替代声明，也没有构建或运行 podofo 提供方。

GNU：

```text
{sy[0]['symbol']}
```

libc++：

```text
{sy[1]['symbol']}
```

来源及对象 SHA：{ref('SEEK_SYMBOLS.json')}；程序：[seek.cpp](probes/seek.cpp)。两边 streamoff 已同为 long long（`x`），剩下的差异来自 seekdir 类型身份；这正是 ARM 上仍存在、不同于先前整数别名差异的因素。

**编译观察**：两份普通虚调用 IR 都先读取对象虚表指针，再用 `getelementptr ... i64 6` 取函数指针，最后 `call void %...`。索引 6 是相对所用虚表 address point 的从零开始索引，不是“第六个声明”，也不是完整虚表符号起点偏移。这个虚调用块没有按 Seek 的修饰名直接调用。两份 IR、提取段落及 SHA：{ref('VIRTUAL_CALL.json')}。

**机制说明**：函数名参与按名链接；虚调用则从对象所指虚表的选定槽位取得入口。声明/继承/覆盖关系决定槽位组织，不是按修饰名排序。因此名称不同不必导致槽位移动；同一槽位仍不足以证明对象、参数值约定、真实提供方表项、派生覆盖及销毁兼容。通用机制参考 [C++ ABI 虚表组织](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-components) 与 [调用方规则](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vcall)；目标行为的直接证据是本次 ARM 编译输出，不把通用文档当成完整 Tizen 产品证明。

**反驳边界**：若要反驳“本配置编译调用方选择同一槽位”，需给出不同的实际编译输出/配置；若要证明混合产品兼容或不兼容，仍需真实跨包动态对象、提供方、调用路径和运行值。该运行结果为 NOT_OBSERVED。

## 四、公开签名与限定消费方核查

{exposure_table}

原已确认的完整五条声明逐行保留于 {ref('PUBLIC_DECLARATIONS.json')}，原文含参数、返回、inline/virtual 限定和头 SHA。fmtflags 的 `flags()` 返回值与 `flags(fmtflags)` 参数重载分别计一条；另外各一条。{ref('prior_DIVERGENT_EXPOSURE.json')}

245 是旧索引的命中位置，包含尚未逐一完成签名判定的上下文，**本轮没有将其全部升级为公开签名**。本轮按任务要求直接复用已确认清单，只查询这些函数身份，不重开全部命中语义分析。

正向对照先定位真实 `tizen_base::Bundle::Add`，两端索引 SHA 与原提供/消费方记录匹配且来源源码包不同，确认查询能找到异包配对。随后按完整函数身份筛出定义，并用完整规范化函数名找相等的 UND，再比较源码 RPM，而不是查四个类型名字的字面交集。{ref('BOUNDARY_CONTROL.json')} {ref('BOUNDARY_QUERIES.json')}

范围仍为 `S_R119_20260918` 的 3,085 个 x86_64 ELF，不是所有平台产物。四个身份分别找到 2/1/1/1 个定义、均未找到对应 UND。没有构成需要进一步升级的新符号边。内联、虚调用、函数指针、静态链接、生成头、同名不同配置、未扫描架构/产物均不由该阴性排除；尤其 Seek/Clear 不能用无 UND 证明无实际调用。

**既有成员边保留**：fmtflags 已随 `app-core → gtest` 的 ostream 内部状态出现；openmode 已随 `enlightenment → gtest` 的 stringstream 内部状态出现。它们与本轮 ChecksumStream 等直接签名查询不是同一条证据，不能因本轮阴性而抹掉，也不新增重复包边。{ref('EXISTING_MEMBER_EDGES.json')} {ref('EDGE_COUNTS.json')}

## 五、可直接并入汇编的差异说明

先前的 13 项是同名整数别名在两库中选择 long 或 long long；它们在已测 armv7l 配置下恰好选择相同类型。本轮四项则是流状态类型的身份分歧：前三项在 GNU 中是具名枚举，在 libc++ 中是 unsigned int，seekdir 在两库中是不同枚举。实测 armv7l 与 aarch64 均保留分歧，虽然各项两侧都占 4 字节、按 4 字节对齐。相同宽度不能让它们形成相同的 C++ 链接名称；Seek 已展示这个差异，但虚调用可以通过表中入口而不按函数名查找，因此不能把符号分歧直接写成虚调用必失败或产品不兼容。

## 六、范围与交付

四项从原“不测”集合独立列出：类型差异已确认；已登记直接公开签名的异包消费仍未闭合。其余 568 项不重开。未测真实混合库 podofo/Cynara 调用、真实提供方虚表内容、派生类覆盖与对象销毁、枚举值组合的转换语义；没有以测量宽度替这些问题作答。

汇编仅更新枚举条目、相关挂账与不测集合构成，并标版本。改前原件保留在本目录 `summary_v1/`，对应旧提交 `bf35a916d9a9d058e91e851a74129a4efb76f9c8`；变更清单与 SHA 见 SUMMARY_UPDATE.json，逐行差异见 SUMMARY.diff。没有覆盖旧类型测量、定界或投影产物。

自行判断：用显式限定调用观察函数名、普通虚调用观察编译端槽位；不将无 UND 升级为无调用；将既有两条成员关联与本轮直接签名阴性分开；不重开 245 个词法位置的全量语义分析。没有板上操作，没有修改平台源码或配置，没有新包部署，未推 Gerrit。
'''
(OUT/'FINAL.md').write_text(body)
save('RESULT.json',dict(armv7l_divergent=4,aarch64_divergent=4,all_size=4,all_align=4,type_cells=16,target_observations=80,process_runs=20,qualified_positions=245,public_identities=4,public_signatures=5,query_definition_counts={r['query']:len(r['definitions']) for r in qs},new_edges=0,x86_pairs=18,x86_edges=23,arm_full_scope='NOT_OBSERVED',remaining_no_test=568))
