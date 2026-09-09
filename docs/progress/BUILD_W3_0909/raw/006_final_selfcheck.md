# 最终自检

第一次调用使用临时目录并在 trap 中写了清理命令；执行环境在进程创建前
拒绝该调用，故无退出码、没有产生或删除文件。修正为使用 Bash 进程替换，
不创建临时文件；校验内容和判据没有变化。

成功命令原文：

```bash
set -euo pipefail
for lang in zh en; do
  src="docs/progress/BUILD_W3_0909/source/base_packages_${lang}.tsv"
  out="docs/progress/BUILD_W3_0909/updated/base_packages_${lang}.tsv"
  md="docs/progress/BUILD_W3_0909/updated/base_packages_${lang}.md"
  test "$(wc -l < "$src")" -eq 255
  test "$(wc -l < "$out")" -eq 255
  cmp <(cut -f1-8 "$src") <(cut -f1-8 "$out")
  changed=$(paste "$src" "$out" | awk -F '\t' '$9 != $18 {n++} END {print n+0}')
  test "$changed" -eq 1
  markdown_rows=$(awk '/^\| / {n++} END {print n-2}' "$md")
  test "$markdown_rows" -eq 254
  printf '%s rows=254 first8=IDENTICAL status_changes=%s markdown_rows=%s\n' "$lang" "$changed" "$markdown_rows"
done
awk -F '\t' 'NR>1 {n[$4]++} END {for(k in n) print k "=" n[k]}' docs/progress/BUILD_W3_0909/updated/base_packages_zh.tsv | sort
docs/progress/BUILD_W3_0909/code/audit_libcxx_guards.sh /home/toolchain/development/libc++_replacement | awk -F '\t' 'NR>1 && ($4!="PASS" || $5!="PASS") {bad++} END {print "spec_guard_failures=" bad+0; exit(bad?1:0)}'
awk -F '\t' 'END {print "package_status_rows=" NR-1; exit(NR==12?0:1)}' docs/progress/BUILD_W3_0909/PACKAGE_STATUS.tsv
awk -F '\t' 'END {print "compatibility_rows=" NR-1; exit(NR==12?0:1)}' docs/progress/BUILD_W3_0909/COMPATIBILITY_AUDIT.tsv
rg -n '^> \*\*本文档按既有理解编写' docs/progress/BUILD_W3_0909/QUICKBUILD_INPUT.md
git diff --check -- docs/progress/BUILD_W3_0909 docs/progress/BUILD_STATUS_0909.md
printf 'selfcheck_exit=0\n'
```

输出：

```text
zh rows=254 first8=IDENTICAL status_changes=1 markdown_rows=254
en rows=254 first8=IDENTICAL status_changes=1 markdown_rows=254
不需要改=237
当前不能改=6
需要改=11
spec_guard_failures=0
package_status_rows=11
compatibility_rows=11
1:> **本文档按既有理解编写，未与 QuickBuild 实际界面核对；若格式与实际不符，请告知我们调整。**
selfcheck_exit=0
```

退出码：`0`。
