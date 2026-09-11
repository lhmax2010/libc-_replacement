# 候选输入中的临时 manifest 副本

aarch64 候选导出时，GBS 报告工作树根目录的未跟踪 `llvm.manifest`
也被收入源码归档。它与受版本控制的 `packaging/llvm.manifest` 逐字节
相同：

```text
CMP_EXIT=0
55449a170ea90537532d2fd52a2e2f7c9a53dbcb6b1894c8a321c53a8685524d  llvm.manifest
55449a170ea90537532d2fd52a2e2f7c9a53dbcb6b1894c8a321c53a8685524d  packaging/llvm.manifest
```

它是 GBS 导出留下的生成副本，不是拟提交内容，也不由 spec 安装。
aarch64 的同环境 RPM 对照五类差异均为 0，因而实测未见产品级影响。
在 x86_64 干净构建前虽已删除一次，该文件被失败的增量导出重新生成，
所以 x86_64 干净构建的源码归档也包含同一副本；这将在输入局限中保留，
不会隐去。
