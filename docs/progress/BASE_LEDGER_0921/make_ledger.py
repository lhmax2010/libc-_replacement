import collections,csv,hashlib,json,pathlib,re
base=pathlib.Path(__file__).resolve().parent;root=base.parent.parent
items=json.loads((base/'inventory.json').read_text())
rem={r['package']:r for r in json.loads((base/'remote_results.json').read_text())}
hist={r['package']:r for r in json.loads((base/'history_results.json').read_text())}
scope='（沿用固定快照、armv7l 已覆盖符号图的原判定；不外推到所有架构/插件/动态加载）'
reason_text={
 'NO_BUILD_EXPOSURE_WITHOUT_SHIPPED_CPP_BOUNDARY':'候选来自实际 C++ 构建等信号；原判定记录没有发布包 C++ 运行时依赖信号、没有 C++ 静态归档，已覆盖图无 Unified 运行期消费边，归入构建暴露但无已交付 C++ 边界类。',
 'NO_BASE_ONLY_OR_INDEPENDENT_PRODUCT_NO_UNIFIED_RUNTIME_EDGE':'自身有 C++ 运行时依赖，但原判定图没有 Unified 运行期消费边，按 Base 内使用或独立程序的既定规则无需新增标准库适配。',
 'NO_HEADER_PAYLOAD_COMPILED_BY_CONSUMER':'记录的 C++ 载体是由使用方编译的头文件；没有发布包 C++ 运行时依赖信号、没有 C++ 静态归档且不提供共享库，因此载体继承使用方标准库。',
 'NO_EXACT_EDGES_ONLY_C_OR_NO_INTERSECTION':'已有 armv7l 精确符号交集将跨仓运行期边全部归为纯 C 接口或无符号交集，原判定无需新增适配；并非声称包内不含 C++。',
 'NO_CPP_STATIC_ARCHIVE_WITHOUT_UNIFIED_CONSUMER':'有含 C++ 的静态归档，但原判定中 Unified 运行期消费方与构建依赖消费方均为 0，按既定规则无需改；不声称任意未来消费方均安全。',
 'NO_ALREADY_EXPLICIT_LIBCXX_RUNTIME_BUILD':'本包自身就是显式构建 libc++/libc++abi 的运行时提供者；原判定为无需再靠额外标准库 flags 适配，不等于其从未修改或不需要供给运行时。',
 'C1_CURRENT_GCC_OVERRIDE_OR_GCC_ROLE':'原判定确认当前 GCC 构建配置/工具链提供者角色，归为本轮保留 GCC 的设计例外；这是当前状态，不是永久技术限制。'
}
changes={
 'abseil-cpp':'spec：Clang 条件、BuildRequires、C++/链接 flags；无源码修改',
 'bcc-tools':'spec：Clang 条件、BuildRequires、C++/链接 flags；无源码修改',
 'boost':'spec：条件选择标准库及 devel 依赖、保存并传递 b2 install 参数；不只是加 flag',
 'bpftrace':'spec：条件依赖与 flags；源码构建适配，Source1002 static 原资产未替换',
 'icu':'spec：条件依赖与 C++/链接 flags；无源码修改',
 'jsoncpp':'spec：条件依赖与 flags，另清除末尾空行；无源码修改',
 'libsigc++':'spec：条件依赖与 flags；无源码修改',
 'pcre':'spec：条件依赖、devel 运行库开发依赖与 flags；无源码修改',
 'taglib':'spec：条件依赖与 flags；无源码修改',
 'tensorflow2':'spec + 源码：条件标准库选择，elementwise.cc 的 std::abs 重载选择修正',
 'llvm':'HEAD：llvm.spec 条件选择/依赖/LLVM_ENABLE_LIBCXX，新增三架构 TF2.18 libc++ AOT 资产；本 HEAD 无上游 C++ 修改；不能将整个 backport 分支说成仅 flags',
 'libcxx-runtimes':'历史 HEAD：libcxx-runtimes.spec 显式选择 Clang、aarch64 调整；相对 R101 已观测 2d23367d 基线新增 0，不属于 11 包新增适配'
}
ledger=[];outside=[];remote_table=[];eleven=[]
for item in items:
 name=item['package'];old=item['r100'];remote=rem[name]
 refs=[f"docs/progress/R100/tables/base_cpp_package_decisions.tsv:{item['r100_line']}",f"docs/progress/R101/tables/base_all_source_packages.tsv:{item['r101_line']}",f"progress/BASE_LEDGER_0921/remote/{name}.json"]
 branch=remote['ref']+(' @ '+remote['head'] if remote['state']=='EXISTS' else ' — '+remote['state'])
 if item['published']:
  if remote['state']=='EXISTS' and remote['head']==item['published']['commit']:
   status='ADAPTED_PUSHED';why='指定 sandbox 远端 HEAD 与既有已发布状态记录一致；'+changes[name]+'。'
  else:status='NOT_EVALUATED';why='远端身份未与既有实施记录一致，不能自动认定当前 HEAD 已验收。'
  refs.append(f"docs/progress/BUILD_WEEKEND_0918/w4/PACKAGE_STATUS.tsv:{item['published_line']}")
  if name=='tensorflow2':refs.extend(['docs/progress/R103/REPORT.md:10',f"docs/progress/R109/tables/base_all_source_packages_updated.tsv:{item['updated_line']}"])
  eleven.append(dict(package=name,expected=item['published']['commit'],observed=remote['head'],match=remote['head']==item['published']['commit'],validation_reused=item['published']['prior_validation_reused'],note=item['published']['candidate_notes']))
 elif old['decision']=='CANNOT_SWITCH_CURRENTLY':
  status='EXCLUDED_BY_DESIGN';why=reason_text[old['matched_rules']]
 elif old['decision']=='NO_LIBCXX_NEEDED':
  status='NO_CHANGE_NEEDED';why=reason_text[old['matched_rules']]
 elif name=='gmp' and item['updated']['判定结果']=='不需要改':
  status='NO_CHANGE_NEEDED';why='R103 后续收口：eigen 最终仅头文件/配置且无 ELF；python-pycrypto 三架构实际链接 -lgmp，最终产物不依赖 libgmpxx/libstdc++，与 libgmpxx.a 强定义符号交集为 0。限定这两个消费方，不外推。'
  refs.extend(['docs/progress/R103/REPORT.md:9',f"docs/progress/R109/tables/base_all_source_packages_updated.tsv:{item['updated_line']}"])
 else:status='NOT_EVALUATED';why='既有记录不足以映射到本任务要求的闭合状态，等待人工。'
 if not item['published']:
  why+=' 原表事实：运行期 C++ 信号='+old['dynamic_cpp_runtime_signal']+'；C++ 静态归档条目='+old['cpp_static_archive_count']+'；Unified 边分类='+old['unified_runtime_edge_classes']+'；Base 内部作为消费方边分类='+old['base_internal_edge_classes_as_consumer']+'；构建依赖消费方='+old['unified_buildrequire_consumers']+'。'+scope
 basis=why+' 依据：'+'；'.join(refs)
 row={'源码包':name,'R100 判定':old['decision'],'sandbox 分支':branch,'状态':status,'依据':basis}
 ledger.append(row)
 if not item['published']:outside.append(row)
 if remote['state']=='EXISTS':
  h=hist[name]
  remote_table.append(dict(package=name,url=item['url'],branch=item['branch'],head=remote['head'],committer_date=h['head_metadata'][3],author_date=h['head_metadata'][2],head_commit_count=h['head_commit_count'],head_parent=h['head_parent'],r101_snapshot_revision=h['r101_baseline'],commits_since_r101_snapshot=h.get('commits_since_r101','NOT_APPLICABLE_NONANCESTRAL' if h.get('baseline_ancestry_exitcode')==1 else 'NOT_AVAILABLE_SHALLOW'),implementation_baseline=h.get('observed_implementation_baseline',h['r101_baseline']),commits_since_implementation_baseline=h['commits_since_implementation_baseline'],visible_history_count=h['visible_history_count'],complete_history_count=h['complete_history_count'],change_nature=changes[name],head_changed_files=';'.join(h['head_changed_files']),evidence=f'progress/BASE_LEDGER_0921/history/{name}.json'))
def write(name,rows):
 with (base/(name+'.tsv')).open('w') as f:
  w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
 def esc(v):return str(v).replace('|','\\|').replace('\n','<br>')
 keys=list(rows[0]);text='| '+' | '.join(keys)+' |\n| '+' | '.join('---' for _ in keys)+' |\n'
 text+=''.join('| '+' | '.join(esc(r[k]) for k in keys)+' |\n' for r in rows)
 (base/(name+'.md')).write_text(text)
write('BASE_73_LEDGER',ledger);write('OUTSIDE_ELEVEN_62',outside);write('REMOTE_BRANCH_DETAILS',remote_table);write('ELEVEN_CROSSCHECK',eleven)
counts=dict(collections.Counter(r['状态'] for r in ledger))
assert len(ledger)==73 and len(outside)==62 and len(eleven)==11
assert len({r['源码包'] for r in ledger})==73
assert all(r['match'] for r in eleven)
assert all(x['state'] in ('EXISTS','ABSENT') for x in rem.values())
checks=dict(rows=73,outside_eleven=62,eleven_all_match=True,status_counts=counts,remote_states=dict(collections.Counter(r['state'] for r in rem.values())),not_evaluated=[r['源码包'] for r in ledger if r['状态']=='NOT_EVALUATED'],no_image_membership_used=True,scope='R101 fixed source universe + R100 fixed C++ candidates; not current reference full rescan')
for entry in json.loads((base/'INPUT_IDENTITIES.json').read_text()):
 assert hashlib.sha256((root/entry['path']).read_bytes()).hexdigest()==entry['sha256']
checks['original_input_hashes_unchanged']=True
(base/'SELFCHECK.json').write_text(json.dumps(checks,ensure_ascii=False,indent=2)+'\n')
print(json.dumps(checks,ensure_ascii=False,indent=2))
