# 新配方写包与 ARM 上板结果

本轮状态：**W2R_COMPLETE / W3=BLOCKED_AT_NORMAL_INSTALL_PREFLIGHT**。两架构写包与RPM内ELF验收通过；ARM正常安装预检失败，功能对照和取消探针未执行，尚不能确认候选可替换发布资产。不推包仓、不覆盖Source1002、不起QuickBuild。

## 一张表看门禁

| 项 | 结果 | 证据/边界 |
|---|---|---|
| armv7l 整轮/三RPM | PASS | 退出0；三份RPM核验见verify-armv7l/commands.json |
| armv7l RPM内static ELF | PASS | verify-armv7l/result.json；不是旧ELF |
| aarch64 整轮/三RPM | PASS | 退出0；三份RPM核验见verify-aarch64/commands.json |
| aarch64 RPM内static ELF | PASS | verify-aarch64/result.json；不是旧ELF |
| ARM static正常安装预检 | FAIL（已停止） | board/INSTALL_PREFLIGHT.json；不绕过、不重试 |
| ARM正常安装static与%post | NOT_OBSERVED | board/INSTALL_PREFLIGHT.json；正式安装未执行时无INSTALL_RESULT.json |
| ARM候选/原件成对功能 | NOT_OBSERVED | board/pairs.json仅实际执行后才生成；本轮未执行 |
| ARM主包安装预检 | FAIL（非static失败） | 仅预检，未安装主包；board/INSTALL_PREFLIGHT.json |
| ARM取消 shared-gcc | NOT_OBSERVED | 物理板；共享为主证据，静态为对照；不预设崩溃 |
| ARM取消 static-gcc | NOT_OBSERVED | 物理板；共享为主证据，静态为对照；不预设崩溃 |
| 板端恢复与清理 | PRESERVED_FAILURE_SCENE | board/before.json、after.json、CLEANUP.json |

aarch64 不上板，运行功能本轮 NOT_OBSERVED（按任务范围）。两格从源码tar重新执行prep/build/install/write，不复用上轮重链接ELF。六份RPM（每格main/common/static三份）身份、大小、payload和文件数量见 [RPM_INVENTORY.tsv](RPM_INVENTORY.tsv)，依赖见 [RPM_REQUIRES.tsv](RPM_REQUIRES.tsv)。逐份 `rpm -K --nosignature`、文件清单、脚本与提取命令在 verify-*/commands.json。

## 配方与兼容性范围

临时spec只增加Patch0和prep无条件应用，完整差异为 [SPEC_DIFF.patch](SPEC_DIFF.patch)。配方全文与补丁在 recipe/，SHA在recipe/IDENTITY.json。补丁只改变 STATIC_LINKING 且 LIBCLANG_STATIC_PATH 分支的一行，将裸 bpf 改为 `${LIBBPF_LIBRARIES}`。主包关闭STATIC_LINKING，GCC/未定义路径不构建static；未添加新开关，未改原spec。GCC/未定义本轮未重构，不把分支核查当作重测。

两架构沿用BPF_W1_0921隔离入口及输入根，只换本轮tmp与SOURCES副本；宏预检均为w5.xzdio，实际RPM payload需与清单相符。j1、nice19、ionice idle、cgroup 16536457216字节均保留；实际PID/cgroup/memory.max在cells事件。开始磁盘143GiB、I/O0.104秒，资源门禁0；每500目标探测和小时状态由guard保留。未并行构建两架构。

## RPM中 ELF 身份

### armv7l

- 新RPM内ELF：`6e110b9c61a592539e99f5fbfb619211a261552d4bff0de2a7040a39cb4bc1ba`，90870912字节。
- 上轮仅重链接ELF：`a68f5c8c44051769e1e4718935fc3c5e9e147c5ea56e1a91b86ab902414346d5`，90870376字节。两者只登记，不解释SHA差异。
- DT_NEEDED：`libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux.so.3`。
- 命名空间计数：`{"std1": 22952, "cxx11": 0, "cxx11_broad": 0}`；libbpf已定义符号交集770，包含bpf_object__open。
- ELF类别/架构、压缩包提取后.text/.rodata与本轮构建ELF一致、精确四依赖均见验收JSON。

### aarch64

- 新RPM内ELF：`26ecab8874f842db053a227a92b93e000fcf818ded8cd957daa0e716f36e0079`，136139424字节。
- 上轮仅重链接ELF：`26ecab8874f842db053a227a92b93e000fcf818ded8cd957daa0e716f36e0079`，136139424字节。两者只登记，不解释SHA差异。
- DT_NEEDED：`libm.so.6, libgcc_s.so.1, libc.so.6, ld-linux-aarch64.so.1`。
- 命名空间计数：`{"std1": 16480, "cxx11": 0, "cxx11_broad": 0}`；libbpf已定义符号交集764，包含bpf_object__open。
- ELF类别/架构、压缩包提取后.text/.rodata与本轮构建ELF一致、精确四依赖均见验收JSON。

两个新RPM内ELF与上轮重链接ELF的对比：ARM SHA不同；aarch64 SHA相同。只登记本次身份关系，不推广为可重现性证明。

## -lstdc++fs 待办（不改）

来自 `src/CMakeLists.txt:188-191` 的Clang/旧GCC文件系统分支，不是spec直接加入。另起独立输出文件，以相同链接参数附加 `-Wl,--trace` 实测解析，未覆盖RPM、构建ELF或原dependency文件；不据此推断所有成员均被拉入。

- armv7l：`/usr/bin/../lib/gcc/armv7l-tizen-linux-gnueabi/14.2.0/libstdc++fs.a`，静态归档530116字节，SHA `18de93a14b6b648e2ab41acedc4aca625d7fa1f172b894c09edb3670fb258cb2`。
- aarch64：`/usr/bin/../lib64/gcc/aarch64-tizen-linux-gnu/14.2.0/libstdc++fs.a`，静态归档642798字节，SHA `53002b96dde2c2c1810f36f65664f75e789398345bdc999d665f9d22be13fc61`。

W4需审阅这项链接选择；本轮不删除、不替换它。原始trace与命令在link-audit-*/。

## ARM成对对照

正常 static 预检实际失败，未正式安装，故 `--version`、`--info`、`-l`、三种脚本各3次以及两组取消各20次全部 NOT_OBSERVED（实际执行0次，不是0/20通过或失败）。不得把尚未运行的成对测试说成等价。见 [BOARD_ANALYSIS.md](BOARD_ANALYSIS.md) 的原文与现场状态。

| 调用 | 候选退出码 | 原件退出码 | 机械分类（不是功能结论） | stdout / stderr |
|---|---:|---:|---|---|
| 全部 | NOT_OBSERVED | NOT_OBSERVED | 未取得板测结果 | 未观测 |

原计划是每项候选后原件、脚本各3对、取消各20次，并保存-l完整输出/前50行和--info逐行diff；**此次未执行这些步骤，因此不存在这些输出文件，也未选择内核探针**。准备了脚本不等于已执行。

## 失败保留、工具问题与下一步

ARM第一次核验器正则双重转义误判ELF类别；实际readelf已是ELF32/ARM。首次目录、脚本、退出1记录完整保留；修复核验器后对同一RPM在新解包目录核验通过，不是重跑构建或替换候选。符号交集原文本分隔符问题同样保留原件并提供可读副本，不影响集合与门禁计算。详见 DECISIONS.md。

W4所需spec/patch、RPM SHA与板测门禁见 W4_INPUTS.md。只有人工审阅签字后才能推进sandbox配方与QuickBuild；本轮不做这两项。
