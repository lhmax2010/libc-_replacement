from common import *
import csv,collections
gate()
edges=list(csv.DictReader((OUT/'EDGES.tsv').open(),delimiter='\t'))
assert len(edges)==23
for name in ('EDGES.tsv','NEXT_STAGE.tsv'):
 assert sha(OUT/name)==sha(ROOT/'docs/progress/RUNTIME_PHASE_SUMMARY_0921'/name)
details={r['edge']:r for r in json.loads((OUT/'DETAILS.json').read_text())}
inv={r['edge']:r for r in json.loads((OUT/'INVENTORY.json').read_text())}
rpms={r['name']:r for r in json.loads((OUT/'RPMS.json').read_text())}
decl=json.loads((OUT/'DECLARATIONS.json').read_text());latest={}
for path in sorted((OUT/'runs').glob('*/results.json')):
 for r in json.loads(path.read_text()):latest[r['edge']]=dict(r,result_file=str(path.relative_to(OUT)))
assert set(latest)==set(range(1,24))
def tsv(name,rows):
 with (OUT/name).open('w') as f:
  w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
def public_packages(e):
 names=set(inv[e]['runtime_packages'])
 for h in decl:
  if h['edge']==e:names.update(h['owners'])
 source=inv[e]['source_package']
 if source+'-devel' in rpms:names.add(source+'-devel')
 return sorted(names)
combined=[];inputs=[];declarations_md=['# 公开声明与调用路径的头文件证据','', '路径相对于隔离 sysroot；每段带完整文件 SHA256 与来源开发包。内部 helper 的声明只用于取证，夹具从公开入口到达，不直接 include 内部头。','']
for h in decl:
 declarations_md += [f"## 边 {h['edge']}：{h['path']}",'',f"SHA256 `{h['sha256']}`；RPM：`{', '.join(h['owners'])}`。",'']
 for x in h['excerpts']:declarations_md += ['```cpp',x['text'],'```','']
(OUT/'DECLARATIONS.md').write_text('\n'.join(declarations_md))
for edge in edges:
 e=int(edge['edge']);r=latest[e];d=details[e];v=inv[e]
 if r.get('fixture'):
  assert sha(OUT/r['fixture'])==r['fixture_sha256'],('stale fixture',e)
  assert r['support_sha256']=={p.name:sha(p) for p in (OUT/'fixtures').glob('*.h')}
  assert r['driver_sha256']==sha(OUT/'code/run_fixtures.py') and r['common_sha256']==sha(OUT/'code/common.py')
  assert r['rpm_manifest_sha256']==sha(OUT/'RPMS.json')
 assert r['stdlib']=='libstdc++'
 if r['rounds']:assert len(r['rounds'])==5
 for x in r['rounds']:
  assert x['und_exact'],('invalid UND',e,x['round'])
  if x['valid']:assert x['exit']==0 and x['provider_mapped'] and x['gnu_mapped'] and not x['cxx_mapped'] and x['target_bindings']
 success=sum(x['valid'] for x in r['rounds']);assert (success==5)==(r['status']=='GNU_BASELINE_5_OF_5')
 primary=public_packages(e);extra=d['extra_cpp']
 for n in primary+extra:assert n in rpms,n
 versions=';'.join(n+' '+rpms[n]['version']['ver']+'-'+rpms[n]['version']['rel'] for n in v['runtime_packages'])
 locations=';'.join(h['path']+':'+','.join(str(x['line']) for x in h['excerpts']) for h in decl if h['edge']==e)
 inputs.append(dict(edge=e,consumer=edge['consumer'],provider_source=edge['provider'],arch='x86_64',libcxx_primary_packages=';'.join(primary),libcxx_fixture_support_packages=';'.join(extra),conditional_plugin_choices=';'.join(d.get('conditional_cpp',[])),gnu_reference_versions=versions,condition=d['prerequisites'],missing=d['missing']))
 combined.append(dict(edge=e,consumer=edge['consumer'],provider_source=edge['provider'],provider_versions=versions,provider_path=v['provider'],provider_sha256=v['provider_sha256'],mangled_symbol=edge['x86_raw_symbol'],declarations=locations,fixture=r.get('fixture') or 'NOT_AVAILABLE',trigger=d['sequence'],status=r['status'],valid_rounds=success,attempts=len(r['rounds']),exit_codes=','.join(str(x.get('exit','INVALID')) for x in r['rounds']),reason=d['missing'],result_file=r['result_file'],libcxx_inputs=';'.join(primary+extra)))
save('SELECTED_RUNS.json',[latest[e] for e in range(1,24)])
save('FINAL_ROWS.json',combined);tsv('RESULTS.tsv',combined);tsv('LIBCXX_INPUTS.tsv',inputs)
tsv('RPM_PROVENANCE.tsv',[dict(name=r['name'],version=r['version']['ver']+'-'+r['version']['rel'],arch=r['arch'],source=r['source_package'],snapshot=r['snapshot'],url=r['url'],sha256=r['checksum'],identity=r['identity'],file_list=r['file_list_record']) for r in rpms.values()])
save('STAGING_SMOKE_INPUT.json',[rpms['bundle']])
passed=[r['edge'] for r in combined if r['valid_rounds']==5];blocked=[r['edge'] for r in combined if r['valid_rounds']!=5]
assert passed==[1,2,3,4,5,6,9,13,14,15,17,19,20,21,22,23]
for e in (8,12):assert all(x['exit']==-6 and not x['target_bindings'] for x in latest[e]['rounds'])
assert all(x['exit']==77 and x['target_bindings'] for x in latest[10]['rounds'])
host={}
for r in latest.values():
 for x in r['rounds']:
  for path,digest in x.get('maps',{}).items():
   if not pathlib.Path(path).is_relative_to(TMP):host[path]=digest
save('HOST_DEPENDENCIES.json',host)
assert all(any(s in p for s in ['libEGL.so','libGLESv2.so','libGLdispatch.so']) for p in host),host
summary=dict(architecture='x86_64 native',edges=23,baseline_edges=len(passed),baseline_rounds=sum(r['valid_rounds'] for r in combined),not_available_edges=len(blocked),passed=passed,not_available=blocked,compiled_fixtures=sum(bool(r.get('fixture')) for r in latest.values()),selected_attempts=sum(len(r['rounds']) for r in latest.values()),rpm_count=len(rpms),rlimit_as_bytes=MEM*30//100,deadline_utc=datetime.datetime.fromtimestamp(DEADLINE,datetime.timezone.utc).isoformat(),generated_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
save('COUNTS.json',summary)
lines=['# 23 条跨包边：真实调用夹具与 GNU 基线','',
 '## 结论','',
 f"**PARTIAL：{len(passed)}/23 条完成 x86_64 本机 GNU/GNU 五轮有效基线，共 80 个有效轮次；7 条 NOT_AVAILABLE。** 已编译 19 个边夹具，最终采用 95 轮尝试（80 次有效、10 次初始化终止、5 次仅错误路径）。四条未造运行夹具。没有测 libc++ 组合、没有用板子、没有构建平台包，也没有把这 16 条称为原消费方产品端到端通过。",'',
 '边集合只来自冻结的 EDGES.tsv 和 NEXT_STAGE.tsv；原消费方包用于说明这条登记边的来源，本轮新建最小消费方，不冒称启动原应用。历史 18 包对/23 符号边不因此增减。', '',
 '## 来源、环境与验收条件','',
 '- Unified 固定快照：`tizen-unified-toolchain_20260917.132101`；Base：`tizen-base-toolchain_20260914.073422`。先从 reference/build.xml 取得当前身份，再固定 URL。详见 [快照](SNAPSHOTS.json)、[逐 RPM URL/SHA/版本](RPM_PROVENANCE.tsv)。',
 f"- 下载/解包 {len(rpms)} 个 RPM（目标库、开发头与依赖，包括开发包带入的 Boost 运行组件）；仅放 `tmp/EDGE_FIXTURE_0927/`。不安装 RPM，不执行安装脚本，不修改系统库/配置。",
 '- 另取四个示例/测试数据 RPM，解至独立 test-data 目录；未混入上述运行库集合、未运行其测试二进制。URL/SHA 及文件清单见 [EXTRA_DATA_RPMS.json](EXTRA_DATA_RPMS.json)、[数据补查](DATA_FOLLOWUP.md)。',
 '- Clang 22.1.8、`-std=c++17 -O0 -fno-inline -stdlib=libstdc++`；GNU 指标准库配置，不冒称 GCC 前端。显式采用上述快照的 GNU 14.2.0 开发头/sysroot；每轮打印 GNU release/date/CXX11_ABI，完整编译命令在 raw。',
 '- 每轮先 nm -D 核对**指定精确 UND**，启动时 maps 核对提供方规范路径、打印预计算 SHA，进程退出后再核 SHA。LD_DEBUG=bindings 配合 lazy PLT 记录目标真实绑定；通过轮必须同时满足退出 0、值/状态/生命周期断言、目标绑定、GNU 运行库已加载且无 libc++。',
 '- 普通目标的依赖从隔离目录加载。单例夹具在真实离屏工厂构造后还装载了本机三份 C 图形库：EGL、GLESv2、GLdispatch，逐轮 maps/SHA 见 [HOST_DEPENDENCIES.json](HOST_DEPENDENCIES.json)。隔离目录不是 chroot；不宣称整套产品镜像环境。没有启动渲染，也不据此承诺图形链或产品环境通过。',
 f"- 串行、nice 19、ionice 3，RLIMIT_AS={MEM*30//100} 字节（实际内存 30%），light 闸门；失败调度按十分钟/连续六次停止。当前闸门均通过，原始状态在 raw。硬截止为 2026-09-28 08:30 +08。",'',
 '## 逐边结果','',
 '| 边 | 原消费方 → 提供方 | 夹具 | 真实触发与断言摘要 | GNU 五轮 | 缺口 | libc++ 输入 |','| --- | --- | --- | --- | --- | --- | --- |']
for row in combined:
 e=row['edge'];d=details[e];fixture=f"[{pathlib.Path(row['fixture']).name}]({row['fixture']})" if row['fixture']!='NOT_AVAILABLE' else 'NOT_AVAILABLE'
 result='5/5 有效' if row['valid_rounds']==5 else ('0/5；'+row['exit_codes'] if row['attempts'] else '未运行')
 lines.append(f"| {e} | {row['consumer']} → {row['provider_source']} | {fixture} | {d['sequence']}；{d['assertions']} | {result} | {d['missing'] or '仅本样本 GNU 基线；其他组合未测'} | `{row['libcxx_inputs']}` |")
lines+=['','完整 mangled 名、提供库版本、声明位置和证据入口在 [RESULTS.tsv](RESULTS.tsv)；头文件 SHA/原文/归属见 [DECLARATIONS.md](DECLARATIONS.md) / [JSON](DECLARATIONS.json)。逐边 libc++ 目标和辅助包、架构、条件在 [LIBCXX_INPUTS.tsv](LIBCXX_INPUTS.tsv)。','',
 '## 未完成项与口径','',
 '- **边 7**：精确旧符号缺失，不是查询零命中就停。当前同名方法的导出是正向对照；当前角色数组编码为 `Lm5E`，登记为 `Lm4E`。见 [符号新旧原文](EDGE7_SYMBOL_CHANGE.json)。本轮没有改登记边或把新接口当替代样本。',
 '- **边 8、12**：最终候选都编译且具备指定 UND，但在 Application::Start 前置阶段发生 EGL/DaliException 终止；目标绑定未发生。初版边 8 使用更新线程专用读取方法属于夹具错误，不算 provider 失败。纠正后保持真实帧/页面回调断言，等待正确运行环境。',
 '- **边 10**：实际调用已发生，五次验证缺失文件 false/非空诊断及析构；缺有效文档的正向对照，因此仍 NOT_AVAILABLE，退出 77，不能列入 80 次有效运行。',
 '- **边 11、16、18**：只完成声明/依赖/装载与前置梳理，未伪造签名验证、推理后端、安全服务或无数据空操作。逐项已试方法及具体需求见 [DETAILS.json](DETAILS.json)。',
 '- **边 15**：无 Application 的初版五次退出 77。通过公开 OffscreenApplication 工厂（GLIB、MANUAL）构造而不 Start，完成真实类型键检索、值与销毁断言后，才纳入当前 5/5；不是降低前置检查。',
 '- 异常范围：XML Reader 畸形输入验证公开文档的 zypp::Exception 类型族并打印动态类型；JSON/Delta 用返回值报告解析失败。未统一注入分配失败/取消，也未把不崩溃当作全异常安全。生命周期是被测对象/共享引用/回调捕获的断言，不是内部分配器全泄漏证明。',
 '- 没有 armv7l/aarch64、混合标准库、原应用完整上下文、并发压力、任意输入覆盖。后续换输入仅适用于已经有效的夹具；缺上下文的七条仍须补齐条件并验证夹具，不能承诺只换库就全部可跑。','',
 '## 原始证据与复现','',
 '[最终选用的逐轮 JSON](SELECTED_RUNS.json) 保留每轮 command/stdout/stderr/exitcode 路径、UND 原文、绑定原文、maps/SHA。`runs/` 与 `raw/` 保留所有失败和修正尝试，不只保留成功轮。',
 '重放方式及 libc++ 输入约束见 [README.md](README.md)；判断与尚存问题见 [DECISIONS.md](DECISIONS.md)；统计见 [COUNTS.json](COUNTS.json)；自检与源码 SHA 见 [AUDIT.json](AUDIT.json) 和 [SHA256SUMS](SHA256SUMS)。',
 '', '完成后停止，交人工审阅；推送回执另见 DELIVERY.md。','']
(OUT/'FINAL_RESULT.md').write_text('\n'.join(lines))
print(json.dumps(summary,ensure_ascii=False,indent=2))
