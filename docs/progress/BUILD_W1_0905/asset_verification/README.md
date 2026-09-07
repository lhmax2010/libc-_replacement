# 资产比对证据说明

`summary.tsv` 按架构比较现有三包合并资产、重新生成的默认 libstdc++
资产和 libc++ 资产。符号集合取每个 `.o`/`.a` 的全局已定义及未定义
符号并去重；路径集合只计普通文件。`std___1` 与 `std___cxx11` 计数基于
`c++filt` 后的唯一符号。

- `*_path_missing.txt` / `*_path_extra.txt`：相对现有资产的完整路径差异；
- `*_nonbinary_hash_diff.txt`：共同的非二进制、非 manifest 文件中内容哈希
  不同的完整清单；
- `*_{defined,undefined}_{missing,extra}_vs_old.txt.gz`：相对现有资产的完整
  符号差异，gzip 压缩；
- `*_std___{1,cxx11}.samples.tsv`：ABI 命名空间的前 50 个符号样本；完整
  数量见 `summary.tsv`；
- `default_switch_equivalence.tsv`：显式 `--stdlib libstdc++` 与完全省略
  `--stdlib` 的逐文件内容对照（排除 manifest）；
- `default_archive_member_equivalence.tsv`：上述两次运行的 runtime 静态归档
  逐成员对照；
- `stdlib_variant_payload.tsv` / `stdlib_variant_archive_members.tsv`：同一
  TensorFlow 2.18.0 输入下 libstdc++ 与 libc++ 两条路径的载荷对照。

现有资产使用 TensorFlow 2.15.1，而新材料使用 2.18.0；所以
`summary.tsv` 中默认路径相对现有资产的大量差异不是标准库开关造成的，
也不能视为等价产物。
