# 编译线进度对齐核对

本次只做材料发布、历史记录对照与只读查询。没有构建、修复、安装、板上操作、包仓推送或配置/源码修改。原记录不覆盖；以下明确区分历史实测、本次静态核查和未观测。

## 一、先推送的完成凭据

**已先完成**：`docs/progress/ARM_RPM_DIAG/` 已提交并普通推送至 `codex/r5-ehabi-diagnosis`，远端 SHA 为 **`029157f1431946c6f13e8b5fde594a06fd7a5425`**。然后才开始第二节核对。推送与远端确认原文见本目录 `raw/013*`、`raw/014*`；其他三个远端分支未变。

原 `progress/ARM_RPM_DIAG/` 共 1,522 个文件，包含 FINAL_RESULT.md、INTERIM_RESULT.md、raw、cells、trace、脚本及取证表；另补入原在 `tmp/ARM_RPM_DIAG/` 的 guest trace，共 **1,523 份原始文件、1,821,469,744 字节**。没有静默跳过文件。宿主 trace（1,567,709,470 字节）、guest-error-calls.tsv（26,048,731 字节）和 guest trace（177,511,348 字节）采用无损 xz 压缩、32 MiB 分片交付，保留原名、原大小、SHA256 与解压校验；原件不变。1529 个交付文件包括这些材料及交付说明/清单。完整目录清单、复原办法、逐文件校验在 `../ARM_RPM_DIAG/README_DELIVERY.md`、`DELIVERY_MANIFEST.json`、`DELIVERY_VERIFICATION.json`。

凭据字符串及所列敏感模式扫描通过；报告不宣称扫描能证明任意未知秘密不存在。原始命令未含凭据，没有因安全扫描删减证据；见 `security-before-delivery.json`、`staged-security-1529.json`。本次其他既有工作区修改未纳入提交。

## 二、逐项答复

### 核对项 1：连续两次失败与“间歇性”

**a. 成立。** 两轮都是写出 18 个 RPM，第 7559 行最后写出 clang-devel-debuginfo，第 7560 行同一句错误：

```text
Wrote: /tmp/task/rpm-arm-llvm/RPMS/armv7l/clang-devel-debuginfo-22.1.8-1.armv7l.rpm
error: create archive failed: cpio: write failed - Function not implemented
```

原始出处（仓库根相对路径）：

- 本地原件 `progress/BUILD_STATIC_0917B/resume_0917/cells/llvm-arm-rpm-serial/build.log:7559`、`:7560`；本次公开摘录 `docs/progress/ALIGN_0920/evidence/failure0917-packaging-lines.txt:18`、`:19`。
- 本地原件 `progress/BUILD_WEEKEND_0918/cells/llvm-arm-whole-retry/build.log:7559`、`:7560`；本次公开摘录 `docs/progress/ALIGN_0920/evidence/failure0918-packaging-lines.txt:18`、`:19`。
- `docs/progress/BUILD_WEEKEND_0918/w4/W2_REPORT.md:5`、`:14`–`:16`。

**b. 需修正。** “间歇性”不能作为已确认归因，若回传如此表述，应撤回。准确表述是：**连续两轮在相同日志位置失败；随后一轮在有诊断条件差异的运行中成功。尚未证明严格同条件下的确定性或间歇性。** 两次同点失败也不足以证明所有条件下必然失败。本次不替换原报告，只在此纠正表达边界。

**c. 成立，周末 W2 计入。** 两次分别是 9 月 17 日 `llvm-arm-rpm-serial` 和 9 月 18 日周末 W2 `llvm-arm-whole-retry`，不是另有两次未披露的失败。原诊断报告已经逐项列出：`docs/progress/ARM_RPM_DIAG/FINAL_RESULT.md:17`–`:18`、`:31`–`:32`。

另需统一日期：用户所称“09-20 回传的成功轮”，执行记录实际写的是 **09-18 19:58:55 至 20:53:41 +08**，不是 09-20 执行。证据 `docs/progress/ARM_RPM_DIAG/raw/045_reproduce.started.txt:1`、`045_reproduce.finished.txt:1`；这里只按记录申报，不推测回传时间与执行时间为何不同。

### 核对项 2：成功轮与周末 W2 的完整命令差异

**a–c. “完全一致”不成立。** 已逐字节比较 JSON 原文件、外层 scope 命令、实际运行事件中的 argv、guard 脚本及 deadline 文件。实际运行事件的 argv 与相应 JSON 一致，脚本 SHA 与运行时记录一致，见 `launch-and-payload-verification.json:1`、两轮 `cells/*/events.jsonl:5`。

完整命令而非摘录在 `comparison/old-full-command.txt:1`、`new-full-command.txt:1`；原始 JSON 原样副本在 `argv-old.json`、`argv-new.json`。全部逐字差异分别在 `argv.diff:1`、`scope.diff:1`、`guard.diff:1`、`deadlines.diff:1`；机器可读差异见 `argv-all-differences.json`。以下只列差异，不赋予因果解释：

| 项 | 周末 W2 | 成功诊断轮 |
| --- | --- | --- |
| 外层 scope unit | build-weekend-0918 | arm-rpm-diag |
| guard/阶段/标签 | progress/BUILD_WEEKEND_0918/guard.py W2 llvm-arm-whole-retry | progress/ARM_RPM_DIAG/guard.py W1 llvm-arm-repro |
| guard 输出根/scope 常量 | BUILD_WEEKEND_0918 / build-weekend-0918.scope | ARM_RPM_DIAG / arm-rpm-diag.scope |
| guard 额外逻辑 | 无自动失败目录快照分支 | 增加失败时 ls -la 分支（本轮成功，未触发）；文件末尾换行亦不同 |
| 截止记录 | 2026-09-19 00:21:21.129884+08 | 2026-09-19 01:46:15+08 |
| host strace 时间/字符串 | 无 -tt；-s 160 | 增加 -tt；-s 256 |
| host strace 调用集合 | %file,read,pread64,write,pwrite64,lseek,ioctl,copy_file_range,sendfile | write,mknod,mknodat,link,linkat,setxattr,lsetxattr,fsetxattr,fallocate,openat,close,read,pread64,pwrite64 |
| host strace 过滤 | -e status=failed | -e raw=write,read,pread64,pwrite64；没有 status=failed |
| host trace 输出 | progress/BUILD_WEEKEND_0918/llvm-arm-rpm-failed-syscalls.strace | progress/ARM_RPM_DIAG/host.strace |
| tmp/WEEKEND_0918 → /tmp/task | --bind | --ro-bind |
| 新增绑定 | 无 | --bind tmp/ARM_RPM_DIAG /tmp/diag（实际完整绝对路径在 argv） |
| rpmbuild 入口 | rpmbuild | /usr/bin/qemu-arm -strace -D /tmp/diag/guest-rpmbuild.strace /usr/bin/rpmbuild -vv |
| _rpmdir | /tmp/task/rpm-arm-llvm/RPMS | /tmp/diag/rpm-arm-llvm/RPMS |
| _srcrpmdir | /tmp/task/rpm-arm-llvm/SRPMS | /tmp/diag/rpm-arm-llvm/SRPMS |
| _buildrootdir | /tmp/task/rpm-arm-llvm/BUILDROOT | /tmp/diag/rpm-arm-llvm/BUILDROOT |
| _tmppath | /tmp/task/rpm-arm-llvm/TMP | /tmp/diag/rpm-arm-llvm/TMP |
| JSON 字节格式 | 文件尾无换行 | 文件尾有换行 |

全部显式 `--setenv` **无差异**：PATH=/bin:/usr/bin:/sbin:/usr/sbin，QEMU_RESERVED_VA=0x100000000，CMAKE_BUILD_PARALLEL_LEVEL=1，RPM_BUILD_NCPUS=1，MAKEFLAGS=-j1，LC_ALL=C。guard 固定的宿主 PATH 也相同。**没有两轮完整继承环境的历史快照，其他继承变量是否完全相同为 NOT_OBSERVED**，不以显式项一致冒充整个环境一致。

资源参数无差异：MemoryMax=16536457216、nice19、ionice idle、串行限制；两轮实际 scope 核验见各自 `events.jsonl:1`–`:4`。根与 BUILD 绑定的源路径相同：`tmp/GBS-ROOT/BUILD-W2B-llvm-armv7l/local/BUILD-ROOTS/scratch.armv7l.0` 及其中 `home/abuild/rpmbuild/BUILD`。spec 路径相同，SHA256 为 `cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68`。其余宏、目标、rc/macros 路径、--noprep/--noclean/-bb 均无 argv 差异。**同一路径不等于两次启动时整棵输入树逐字节一致；没有完整历史树哈希，记 NOT_OBSERVED。**

另有并行只读取证观察器，是成功轮新增的运行条件，不属于上述 rpmbuild argv：观察 RPM 文件描述符/创建事件并保留头部前缀。完整启动命令为 `nice -n 19 ionice -c 3 python3 progress/ARM_RPM_DIAG/watch_headers.py`，退出 0；原记录见 `docs/progress/ARM_RPM_DIAG/raw/119_observe_headers.command.txt:1`、`119_observe_headers.exitcode:1` 和 `FINAL_RESULT.md:50`；不能省略此差异。前缀不是完整 RPM。

**d–e. 需修正证据时间。** 本次在两套原宏参数和绑定路径下，仅执行只读 `rpm --eval`，不是构建。完整命令/退出码/原文在 `comparison/old-actual-eval.json:1`、`new-actual-eval.json:1`、raw/021、022。两次都实际输出：

```text
QEMU_RESERVED_VA=0x100000000
_binary_payload=w5T1.xzdio
_target=armv7l-linux
_build_create_debug=1
```

这是 **09-20 按两轮参数复核的实测展开值**。两轮历史启动事件确实设置相同 QEMU_RESERVED_VA，但没有找到当时保存的 rpm --eval 或完整 /proc/environ 内容，历史瞬间的独立实测快照记 NOT_OBSERVED。不能将今天的查询倒填为历史采样。作为独立产物核对，周末 W2 和诊断轮 llvm-static-devel 的 RPM 头均实测 PAYLOADCOMPRESSOR=xz、PAYLOADFLAGS=5T1（`launch-and-payload-verification.json`），不支持“这一轮改了压缩算法”的说法。

**f. 成立。** 成功轮写出 22 个，历史两轮各 18 个；前 18 个名称和 Wrote 顺序完全相同。逐个对照（含三轮各自行号）见 `rpm-order-comparison.tsv:2`–`:23`。新增四个为：

1. lldb-debuginfo-22.1.8-1.armv7l.rpm；成功日志第 57586 行。
2. lldb-devel-debuginfo-22.1.8-1.armv7l.rpm；第 57595 行。
3. compiler-rt-debuginfo-22.1.8-1.armv7l.rpm；第 57605 行。
4. libomp-debuginfo-22.1.8-1.armv7l.rpm；第 57612 行。

### 核对项 3：第 19 个包身份

**需修正限定，而不是撤回成功轮实测。**

- **a/c：成功轮实测，成立。** `docs/progress/ARM_RPM_DIAG/cells/llvm-arm-repro/build.log:57586` 原文为 `Wrote: /tmp/diag/rpm-arm-llvm/RPMS/armv7l/lldb-debuginfo-22.1.8-1.armv7l.rpm`。guest trace 第 1817734 行成功 `openat(...lldb-debuginfo...,O_RDWR|O_CREAT|O_LARGEFILE|O_TRUNC,0666) = 4`，见 `trace-counts.json:105`–`:107`。不是按 spec 顺序推断。
- **b：若把这个身份直接用于“旧轮最终失败的归档包/成员已确认”，不成立。** 旧失败 trace 第 3171491–3171492 行有此路径，但仅路径出现不足以定位归档内部失败操作。旧轮日志确实没有下一条 Wrote，也没有保留下来的失败 RPM 头部。`docs/progress/BUILD_WEEKEND_0918/w1/W2_FAILURE_PROVENANCE.md:7`–`:21` 和 `docs/progress/BUILD_STATIC_0917B/resume_0917/resume_stop_1909/ARM_RPM_PARTIAL.md:11` 的限制仍有效。

建议统一读作：“成功诊断轮第 19 个写出的包是 lldb-debuginfo；旧失败轮内部失败的具体归档操作仍 NOT_OBSERVED。”成功轮该包的五个普通文件与保留旧 BUILDROOT 中同路径 SHA 相同，也不能证明整棵输入树相同。

### 核对项 4：strace 层次与 ENOSYS 分布

**a. 成立。** 周末 W2 strace 在 bwrap 外层，是宿主调用记录；ARM rpmbuild 是 ELF32 ARM EABI5、动态解释器 /lib/ld-linux.so.3，经 QEMU 用户态入口运行。当前 binfmt 和二进制核对见 raw/038；原执行入口及子进程见两轮 events.jsonl。仅 QEMU_RESERVED_VA 的存在本身不是执行架构证明。

**b. 需限定。** 宿主 strace 可以看到 QEMU 为 guest 请求实际发出的宿主系统调用及其返回，不能等同于 guest 全部系统调用视图。QEMU 内部模拟/拒绝的 guest 请求未必对应同名宿主调用；RPM 库内部错误也不是一条宿主系统调用。故所选宿主 trace 中没有 ENOSYS，不等于 guest 没有 ENOSYS；也不证明报错文本必然对应 write 系统调用的 ENOSYS。

**c. 需修正“同一层”的概括。** 成功轮同时有外层 host strace 和显式 QEMU `-strace` guest trace。外层挂接位置相同，但调用过滤不同；guest 层是新增。旧轮没有同等 guest trace。证据为 `comparison/argv.diff:1`。

**d. 实测分布成立：不是只在第 19 包附近。** 成功轮本来没有写包失败点。guest trace 共 1,926,506 行；用落盘脚本 `count_trace.py` 重算，完整单调用行中 statx→errno38 有 **101,895 条**，另有 **11 条交错/不完整匹配行**不混入完整调用计数。其中 **101,821** 条紧邻下一行、同 PID、完整 fstatat64 返回 0。其余不因此视为回退失败。完整位置清单在 `statx-enosys-locations.tsv`，原始 trace 复原见 ARM_RPM_DIAG 的交付说明。

| guest 行号区间（包含两端） | 完整 statx/ENOSYS 条数 | 交错/不完整匹配行 |
| --- | ---: | ---: |
| 1–813697，首个 RPM open 前 | 101776 | 2 |
| 813698–1817733，第 1–18 个 RPM 区间 | 44 | 8 |
| 1817734–1882404，第 19 个 RPM 区间 | 2 | 0 |
| 1882405–1926506，其后 | 73 | 1 |

首条完整匹配在第 6 行，最后一条在第 1926500 行；第 19 包区间的两条在 1817775、1882323 行。前一条之后的 fstatat 行有其他线程输出交错，不计入严格相邻完整配对；后一条第 1882324 行明确成功。依据 `trace-counts.json:3`–`:18`、`:122` 起及位置清单。这里只陈述分布，不归因于旧轮致命错误。

### 核对项 5：libclang.a 身份分歧

**a. 成立，尚未归因。** `docs/progress/BUILD_WEEKEND_0918/w4/W2_REPORT.md:28`–`:34` 的差异仍未解决。本次只读解出 RPM 成员计算 SHA，不比较代码/归因、不替换输入。

**b/c. 需补全：不止原先两种身份。** 本次在相关任务目录定位到 **4 种不同的 RPM 内 libclang.a 字节内容**，均为 2,465,418 字节；另有 1 份构建中间归档。四个单独 RPM 的 digest 检查都通过，但不能把这一点当作它们所在整轮都成功。完整路径、命令、退出码、RPM SHA 与成员 SHA 在 `static-identities/summary.json:1` 及对应单项 JSON。

| 来源 | libclang.a SHA256 | 状态/用途 |
| --- | --- | --- |
| STATIC_0917B/rpm-arm-llvm/RPMS | 9884348bb092f3d550d2485de9a83dcb130f8b1540e0c4f115b82686e42bc374 | 已批准旧 18 包输入，USABLE_BUT_BUILD_INCOMPLETE；bpftrace 当前使用 |
| WEEKEND_0918/rpm-arm-llvm/RPMS | 6b5e306dc7a575cb4a1b2e7ee54f9cda999af4ebe16cbdf935840c1fbbe8e96f | 周末 W2 整轮失败后 18 包之一；未替换下游；差异未归因 |
| ARM_RPM_DIAG/rpm-arm-llvm/RPMS | 9d8d9bc44d3080a39d0af03afbf6bc59a6d10c9875390e3b60b65d2fc2c267b7 | 成功诊断轮 22 包之一；诊断产物，未批准替换下游；新差异未归因 |
| STATIC_0917B/rpm-arm-llvm/RPMS_stopped_parallel_1459 | 22361ff3e78b830df37c225e12e072f1a8f58de27538d23ef57f1299f97665df | 更早中止的并行压缩轮隔离产物，不是获准输入；不升级其整轮状态 |

上表目录均在 `tmp/` 下，具体 RPM 名均为 `armv7l/llvm-static-devel-22.1.8-1.armv7l.rpm`。最后一轮隔离状态出处 `docs/progress/BUILD_STATIC_0917B/resume_0917/resume_stop_1909/STATUS.md:68`–`:69`，它的 RPM PAYLOADFLAGS 为 5T，另外三份为 5T1；此处不据此解释成员 SHA 差异。

下游实际绑定 `tmp/WEEKEND_0918/input-armv7l-v3/usr`，其中 `lib/libclang.a` 本次实测仍为 **9884348b…2bc374**，不是 6b5e306d，也不是 9d8d9bc4；见 raw/027、029。当前 LLVM BUILD 中间归档 `.../BUILD/llvm-22.1.8/build/lib/libclang.a` 为 21,239,890 字节，SHA **a9853fedbf393608aae081a03481bd4c300455465549287aea68bc226da27a04**（raw/040）。它不计入“4 份 RPM 内产物”，也不据大小不同自行认定差异原因。

此数量只覆盖相关已定位目录，不宣称全工作区不存在其他静态归档版本。

## 三、当前真实状态表

详见本目录 `CURRENT_STATUS.md` 与同内容 TSV。核心结论：**有一次 LLVM armv7l 诊断条件下的整轮增量写包成功，但不是历史故障已修复，也不是 bpftrace 候选已完成。**

## 四、主动纠正容易产生的误读

1. “成功轮”和“失败轮”不是完全相同命令；不能以先失败后成功直接称间歇性，更不能宣布根因已排除。
2. 本地执行时间是 09-18，不能把 09-20 的回传/核对日替换成构建日期。
3. “第 19 个 lldb-debuginfo”明确限定为成功轮观测；旧轮内部出错成员/操作仍未知。
4. 同名同版本 RPM 不是同一输入。新诊断轮的 libclang.a 又是第三种主线身份；另有更早隔离版本。下游没有切换到新 22 包。
5. LLVM 的 22 包整轮写包成功不等于 clean-room 完整重编；使用 --noprep、原 BUILD 树、96 个增量目标，也不等于功能等价/可替换认证。
6. “ARM libc++ 主包未写出”上下文指 **bpftrace 的 libc++ 构建路径主包**；若按字面理解成 libcxx-runtimes 的 libc++ RPM，则错误，后者已有 4 包。为避免再次混淆，状态表把两者分列；已向人工提问确认用词，不据歧义删去任一项。
7. bpftrace armv7l/libc++ 尚在编译阶段超时终止，不属于“重复写包失败”；aarch64/libc++ 是写包成功但多出动态 libbpf 依赖，不能两者混称同类失败。
8. armv7l 板上取消已做，静态 libgcc 对照 **也通过 20/20**，没有复现 x86_64 的崩溃。仅安装与功能等价未闭合。不能写“两架构都实证静态取消必崩”，也不能写“板上验证全没做”。
9. 原资产仅做了安装依赖预检，缺依赖后未安装、未运行同组功能测试。按推测链接形态编的探针不能证明已 strip 原资产必崩。
10. “11/11 已推送”是原源码适配工作状态，并不包含这份来源未知 static 资产已经被替换；当前 static 候选仍未通过替换门禁，Source1002 未覆盖。
11. SOURCE_PROVENANCE 的“旧 HEAD、6 个补丁”在所查现行输入中已不成立，但手工候选 RPM 的 VCS 字段另有缺口：本次查询 libc++ 及四份 llvm-static-devel 均为 `(none)`（raw/039、048），不能把格式文件中的 GBS VCS 声明当成这些 RPM 已自带精确包 revision。身份仍须引用外部输入清单。
12. 原诊断 guest-error 粗匹配行数不是失败系统调用次数；并发 trace 会把一个调用名和另一个调用的错误拼在同一行。本次严格统计不将这种行算作已确认调用，更不把它们当作失败 write。
13. ENOSYS 文本、rsync EPERM、编译超时、额外 libbpf 依赖是不同观测，不因先后出现就合并成一个已确认根因。
14. 本次核对期间，远端 `codex/runtime-validation` 从首轮推送后 raw/014 记录的 `f32e3b8ef4ba0477563d439282aea1907f791b22` 变为 raw/052 记录的 `05230f6cde077b70aa19fdbbecd68b5eae817f52`。本任务没有推送该分支，变化来源 NOT_OBSERVED。首轮推送前后“三个其他分支未变”的局部对照成立，但不能扩展成整个核对期间所有远端分支均不变；最终推送另取当时前后状态核对。

## 五、自行判断、技术性重跑与未决问题

- 本次判断限于证据等级、计数边界和明确的状态区分；没有替人工选修法、批准新输入或改变迁移判定。
- 历史进程已退出，不能重新取得当时完整环境。只读宏查询标为本次重放，不伪装历史快照。未定位到致命 guest/RPM 内部操作、归档 SHA 差异原因、ARM/x86 取消机制差异原因，均 NOT_OBSERVED。
- 静态归档检查脚本首次用到本机 Python 3.10 不提供的 hashlib.file_digest，技术性失败后改为分块 SHA 并只读重跑；首次脚本、输出保留（raw/032、035、036）。未修改任何被查对象。
- trace 配对扫描初版把 18 条交错输出算作完整相邻配对（101839）；发现后收紧为单调用完整行，最终 101821。执行过的 v1 与最终脚本均保留，raw/025 与042/043可核对；原始 trace 未改。
- raw/044 的展示命令因 head 提前退出产生 141，属于管道展示非零，不是构建失败；完整源文件未受影响。目录通配未匹配等技术性信息保留原 stderr，不隐去。
- 交付自检发现两处历史 build.log 的本地路径被误加 docs/ 前缀（历史交付并无该展开目录）；已改为准确本地原件路径并增加本次已发布摘录的行号引用。仅修正本次草稿引用，不修改历史材料。首次自检失败记录 raw/054 保留。
- 未获取任何新编译/功能结果，也不新检验包修复。下一步待人工对齐审阅后再定。
