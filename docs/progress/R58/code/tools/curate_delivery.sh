#!/usr/bin/env bash
set -euo pipefail
workspace=/home/toolchain/development/libc++_replacement
dst="$workspace/docs/progress/R58"
rm -rf "$dst"
mkdir -p "$dst/code"/{patches,current,baseline,tests,tools,build_config,evidence} "$dst/review_package"/{patches,current,baseline,comparison,tests,commands,raw/current,raw/prior,tables/prior,tables/current,todo} "$dst/tables"
cp "$workspace/progress/R58/REPORT.md" "$workspace/progress/R58/RESULT.txt" "$workspace/progress/R58/KNOWN_GAPS.md" "$dst/"
cp "$workspace/progress/R58/tables/"* "$dst/tables/"
cp "$workspace/docs/progress/R51/patches/0001-Layer-A-libc-abi-preserve-foreign-forced-unwind-stat.patch" "$dst/code/patches/"
cp "$workspace/docs/progress/R51/patches/0002-Layer-B-libc-abi-expose-GNU-forced-unwind-marker-typ.patch" "$dst/code/patches/"
cp "$workspace/docs/progress/R55/patches/0001-Layer-C-header-preserve-forced-unwind-and-ready-async.patch" "$dst/code/patches/"
cp "$workspace/docs/progress/R53/patches/0001-libcxx-preserve-forced-unwind-in-library-catch-sites.patch" "$dst/code/patches/"
files=(libcxxabi/include/cxxabi.h libcxxabi/src/cxa_exception.cpp libcxxabi/src/cxa_personality.cpp libcxx/include/future libcxx/include/istream libcxx/include/string libcxx/include/__ostream/basic_ostream.h libcxx/src/ios.cpp)
for f in "${files[@]}"; do
  mkdir -p "$dst/code/current/$(dirname "$f")" "$dst/code/baseline/$(dirname "$f")"
  cp "$workspace/tmp/R58/source-full/$f" "$dst/code/current/$f"
  cp "$workspace/tmp/R58/source-base/$f" "$dst/code/baseline/$f"
done
cp -a "$workspace/progress/R58/src/." "$dst/code/tests/"
cp -a "$workspace/progress/R54/src/." "$dst/code/tests/R54" 2>/dev/null || true
cp -a "$workspace/progress/R55/src/." "$dst/code/tests/R55" 2>/dev/null || true
cp -a "$workspace/progress/R58/tools/." "$dst/code/tools/"
cp "$workspace/tmp/R58/build-full-x86_64/CMakeCache.txt" "$dst/code/build_config/CMakeCache.x86_64.txt"
cp "$workspace/tmp/R58/build-full-armv7l/CMakeCache.txt" "$dst/code/build_config/CMakeCache.armv7l.txt"
cp -a "$workspace/progress/R58/commands" "$dst/code/evidence/"
cp -a "$workspace/progress/R58/raw" "$dst/code/evidence/"
cp -a "$dst/code/patches/." "$dst/review_package/patches/"
cp -a "$dst/code/current/." "$dst/review_package/current/"
cp -a "$dst/code/baseline/." "$dst/review_package/baseline/"
cp -a "$dst/code/tests/." "$dst/review_package/tests/"
cp -a "$workspace/docs/progress/R54/review_package/comparison/." "$dst/review_package/comparison/"
cp -a "$workspace/progress/R58/commands/." "$dst/review_package/commands/"
cp -a "$workspace/progress/R58/raw/." "$dst/review_package/raw/current/"
for r in R51 R52 R53 R54 R55; do
  test -d "$workspace/docs/progress/R54/review_package/raw/$r" && cp -a "$workspace/docs/progress/R54/review_package/raw/$r" "$dst/review_package/raw/prior/" || true
  test -d "$workspace/docs/progress/$r/tables" && cp -a "$workspace/docs/progress/$r/tables" "$dst/review_package/tables/prior/$r" || true
done
cp -a "$dst/tables/." "$dst/review_package/tables/current/"
cp "$workspace/docs/progress/R56/REPORT.md" "$dst/review_package/todo/R56_FORWARD_DECL_NOT_VIABLE.md"
cat > "$dst/review_package/todo/CONDITIONAL_COMPILATION_TODO.md" <<'EOF'
# 已登记待办：层 C 条件编译改造

本轮不处理该项。R56 已实测仅以前置声明替代 `cxxabi.h` 不可行：catch handler 参数类型必须完整。仓内未发现 `docs/progress/R57/`，因此 R57 交付材料记 `NOT_AVAILABLE`，不得推断其内容。
EOF
cp "$dst/REPORT.md" "$dst/RESULT.txt" "$dst/KNOWN_GAPS.md" "$dst/review_package/"
cat > "$dst/review_package/README.md" <<'EOF'
# R58 多方代码评审材料

本目录包含四个最终补丁、最终与基线完整文件、libstdc++ 并排对照、测试源码、逐选项命令、当前和既往矩阵/原始输出、未覆盖边界及条件编译待办。结论见 `REPORT.md`，逐文件校验见 `INDEX.tsv`。
EOF
find "$dst" -type f -size +5M -print > "$dst/OVER_5MB.txt"
test ! -s "$dst/OVER_5MB.txt"
python3 - "$dst" <<'PY'
from pathlib import Path
import hashlib,sys
d=Path(sys.argv[1]); rows=[]
for p in sorted(d.rglob('*')):
    if p.is_file() and p.name != 'INDEX.tsv':
        rel=p.relative_to(d)
        h=hashlib.sha256(p.read_bytes()).hexdigest()
        rows.append((str(rel),str(rel),h,str(p.stat().st_size)))
text='source_or_role\tupload_path\tsha256\tsize_bytes\n'+''.join('\t'.join(r)+'\n' for r in rows)
(d/'INDEX.tsv').write_text(text)
rp=d/'review_package'
rows=[]
for p in sorted(rp.rglob('*')):
    if p.is_file() and p.name != 'INDEX.tsv':
        rel=p.relative_to(rp); rows.append((str(rel),hashlib.sha256(p.read_bytes()).hexdigest(),str(p.stat().st_size)))
(rp/'INDEX.tsv').write_text('path\tsha256\tsize_bytes\n'+''.join('\t'.join(r)+'\n' for r in rows))
PY
