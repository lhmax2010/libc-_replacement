# R23 方法与判据

## 第一部分

1. 冻结 R11/R13/R16/R17 输入的路径、SHA256 和字节数。
2. 符号成员只取 `R17 family=FIXED_POINT` 与 R16 实测 libgcc_s 导出集的
   逐名交集；不在本任务内凭前缀新增成员。
3. 对 R11 `elf_inventory.tsv` 中 `runtime_elf=YES` 的 36,829 行逐文件
   打开缓存 ELF。按 EI_CLASS/EI_DATA 解析 section table，读取
   `SHT_DYNSYM` 与其 `sh_link` 指向的字符串表；`st_shndx==0` 是 UND。
4. 动态符号基名与本架构成员集做精确相交。零命中的成立条件是
   36,829/36,829 均取得确定结果。
5. 无 section table 的一个对象改由 GNU `readelf --dyn-syms -W` 实测；
   仅因退出 0 且输出为空才闭合为无动态符号表。
6. 36 个样本以 GNU readelf 复核。ELF 第 0 号 null symbol 计入解析器的
   dynsym entry 数，但不出现在要求名称的 readelf 行匹配中；验证时显式
   加一归一化，UND 名称集合不受影响。
7. `.comment` 中 GCC/clang 标记只用于编译器生产者形态；不由此推断
   第三方所有权。

## 第二部分

测试框架目录是明确的源码包名称正则
`gtest|gmock|googletest|catch2|cppunit|doctest`。阶段来自 R22 全量表；
打包形态来自既有下载 RPM 的 `rpm -qlp`；BuildRequires 来自 R11 source
RPM `requires` 元数据，关系名集合为 `gtest`、`gtest-devel`、`gmock`、
`gmock-devel`、`pkgconfig(gtest)`、`pkgconfig(gmock)`。同一源码 NEVRA
有多个关系时只计一个源码包实例。

## 第三部分

历史证据不回写。对 R15/R15b 报告增加“后续证据已取代当前阻塞”的
状态注记，并在 R22 正向报告集中登记当前状态与剩余能力边界。

## 禁止项遵守

未访问或重扫平台仓，未下载新数据，未安装、构建或部署，未修改既有
RPM/ELF。所有读操作及脚本总命令的原文和退出码保留在 `commands/`。
