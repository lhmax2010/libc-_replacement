# 同环境基线身份

基线由现有 LLVM 仓的已提交对象本地共享克隆到临时目录，未写入 `codes/`。

命令原文：

```bash
git clone --shared --no-hardlinks codes/R104/llvm tmp/BUILD_W1B_0909/llvm-baseline
git -C tmp/BUILD_W1B_0909/llvm-baseline checkout --detach 2d23367d74afbf2bb1e9e4013fce072b3a154109
```

退出码：`0`

身份：

```text
BASELINE_HEAD=2d23367d74afbf2bb1e9e4013fce072b3a154109
BASELINE_SPEC_SHA256=7962d176484920de6138d70c7e8a8e1764dfada6655abf0332e9748167e889fd
CANDIDATE_HEAD=2d23367d74afbf2bb1e9e4013fce072b3a154109
CANDIDATE_SPEC_SHA256=e6b52c4391c605741dadada1faa9f5376e592bb5b3f05949f13d3eaebfd08e67
```

两侧 Git HEAD 相同；候选侧只有既定的未提交打包/CMake 扩展接入改动和
三份 TF 2.18 libc++ 资产，基线侧为该 HEAD 的干净已提交内容。
