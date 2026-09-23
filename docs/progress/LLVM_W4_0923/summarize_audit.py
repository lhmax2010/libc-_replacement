import csv,difflib,json
from pathlib import Path
P=Path.cwd();E=P/'docs/progress/LLVM_W4_0923';rows=json.loads((E/'recipe-audit/RESULTS.json').read_text())
with (E/'RECIPE_AUDIT.tsv').open('w',newline='') as f:
 w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(['package','sandbox_sha','validated_evidence','validated_spec','classification','patch_files','note'])
 for r in rows:
  w.writerow([r['package'],r['remote_sha'],r['evidence'],r['validated_spec'],r['classification'],sum(x['path'].endswith(('.patch','.diff')) for x in r['packaging_files']),'LLVM本地已补一行但GCC override门禁不通过' if r['package']=='llvm' else '源码tar内部改动不在本次packaging文件比对范围'])
base=(P/'tmp/BUILD_W1BC_0911/target-fetch/packaging/libcxx-runtimes.spec').read_text();candidate=(P/'tmp/NIGHT_0917/specs/libcxx-runtimes.spec').read_text()
(E/'UNPUSHED_LIBCXX_RUNTIME.diff').write_text(''.join(difflib.unified_diff(base.splitlines(True),candidate.splitlines(True),fromfile='f203923a/packaging/libcxx-runtimes.spec',tofile='validated-local/libcxx-runtimes.spec')))
parts=['# 11 包已验证配方与 sandbox 对账','', '| 包 | sandbox SHA | 最近验收依据中选取的配方记录 | 结论 |','| --- | --- | --- | --- |']
for r in rows:parts.append(f"| {r['package']} | `{r['remote_sha']}` | `{r['evidence']}` | {r['classification']} |")
parts+=['','九包的唯一spec差异均是GBS导出的VCS字段；原文逐包见recipe-audit/<包>/SPEC.diff，行内容同时收录RESULTS.json。此类字段记录源码身份，不改变构建命令；不纳入LLVM提交。bpftrace spec与patch完全一致。LLVM主spec唯一功能差异为LIBCLANG_BUILD_STATIC=ON，已创建本地未推提交。','',
'共核对9份packaging中的.patch/.diff文件（boost 1、icu 6、pcre 1、bpftrace 1），与保存的实际导出输入SHA完全一致；未发现导出目录额外patch。其余包packaging下没有独立.patch/.diff文件，不代表源码tar内没有已合入补丁。全目录各文件哈希和缺项见各result.json；LLVM的附带libcxx-runtimes.spec与LLVM构建SOURCES中的那份仅差VCS（raw/035），但那份不是bpftrace静态运行库输入实际用的候选！后者另见UNPUSHED_LIBCXX_RUNTIME.diff。','',
'范围：按最近作为验收依据的轮次选取其保留构建根的导出spec/patch（通常最后x86_64 libc++格；bcc取最后ARM libc++格；LLVM与bpftrace取最新完整RPM轮）。R105沿用R104 libsigc++构建，R108的tensorflow2判据沿用R105 libc++候选、不拿GCC基线夹具冒充候选。不是本轮重建，也不是逐个源码tar全部成员与Git树相等的证明；导出路径由历史日志定位，本轮SHA为保留文件现值。Release在所选副本中未产生差异。','',
'本轮11个远端均重新ls-remote确认，远端未变；全部只读git show，不fetch到codes。当前不能据此关闭QuickBuild前置：LLVM本地主spec补齐未推，指定GCC override检查失败，libcxx-runtimes静态配方还未进入该sandbox。']
(E/'RECIPE_AUDIT.md').write_text('\n'.join(parts)+'\n')
print('11 rows; 9 metadata-only, 1 identical, 1 authorized LLVM functional delta; runtime dependency gap separately recorded')
