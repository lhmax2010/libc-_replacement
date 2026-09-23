# LLVM W4 R3：两笔本地提交与 prep 完成；历史 spec 差异门禁停报

**不能宣告“QuickBuild 所需的每一处本地配方改动都已在 sandbox 中”。** 本轮两笔仍未推送；按“所有历史实际命令记录”扫描，还观察到本轮两项之外的 spec 差异。按指令列出完整差异后停止，不自行提交、豁免或排除历史试验配方。没有构建、上板、包仓推送或 QuickBuild。

## 一、runtime 等价性与两笔提交

人工已批准：先忽略纯空行差异，再对其他字符做严格比较。没有忽略空行之外的缩进、参数、注释、Release 等内容。R2原始diff、FAIL、比较器以及ALL_INPUTS_R2.tsv等整个inputs_R2目录均未变（raw/R3_030退出0）。

| ARM真实宏组合 | 四个静态开关 | 新增归档文件项 | 与基线比较 | 结论 |
| --- | --- | --- | --- | --- |
| 默认 | ON | libc++.a、libc++abi.a | 仅获准四开关与两条文件记录 | PASS |
| _toolchain_override gcc | OFF | 无 | 忽略纯空行后相同 | PASS |
| _toolchain gcc | OFF | 无 | 忽略纯空行后相同 | PASS |
| --undefine _toolchain | OFF | 无 | 忽略纯空行后相同 | PASS |

四组各有12个新增空行，来自新增条件宏定义。原始与去空行diff分别保存，见 `prep-check/runtime_R3/`。新比较器 `runtime_equivalence_R3.py` 不覆盖R2比较器。LLVM自身四组PASS沿用R2，不改其既有强制Clang行为。

| 提交 | 内容 | SHA |
| --- | --- | --- |
| 1 | libclang.a；正文准确限定既有build_with_libcxx条件 | 617a210064c4559fe1152728f7c9cf5e1ab99a9f |
| 2 | 条件开启/安装/打包libc++.a与libc++abi.a | db5b49afa7836db0414140208b2d3ab4f4aa7293 |

- 第二笔spec与指定 `tmp/NIGHT_0917/specs/libcxx-runtimes.spec` **逐字节相同**，SHA256 `c535823726bb813510cf02b1ca01ceee6fc74309944d77f26ce45f27bd0427fe`；没有需要保留不同值的VCS行。
- 作者均Hao Lin <hao.lin@samsung.com>；无Signed-off-by。committer沿用克隆现有配置，留人工签字时处理。
- 远端仍为 `f203923a1508c9344f5fc6b17bd8822f011655c4`；本地干净，ahead 2 / behind 0。没有包仓push。
- 两笔完整git show：`SANDBOX_COMMITS_R3.diff`；独立format-patch：`0001-local-recipe_R3.patch`、`0002-local-recipe_R3.patch`；身份见 `LOCAL_COMMITS_R3.json`。
- ARM `rpmbuild -bp` 退出0，用时31.98秒：setup、两个Source复制、退出0，未进入build。原根和packaging只读绑定，独立输出在 `tmp/LLVM_W4_0923/runtime-prep_R3/`。
- 资源实测：nice19、ionice idle、cgroup memory.max=16536457216（机器内存50%按页下取整），磁盘261GiB可用。资源证据、命令原文、退出码见 `prep-check/runtime_R3/prep-*.json` 及stdout/stderr。

## 二、本地 spec 清单扫描

全根RPM溯源已按裁决停止，不再补查其未分类项。改扫 `docs/progress/` 的 cell_commands、argv JSON、build.log；为覆盖旧轮次真实命令命名，补入command JSON、command.txt、构建stdout与.log。自身本轮审计目录排除，避免把读历史材料的动作算成新构建。总扫描9,533份记录，244,445,689字节；逐文件输入SHA在 `spec-audit_R3/scan_inputs_sha256_R3.json`。

解析保留命令原文、bwrap路径映射、_topdir/_specdir宏和来源行号；GBS使用同一执行日志的 `processing recipe` 路径。没有以同名spec猜宿主路径。共500条匹配记录、112个可定位且现存的去重路径；另111条无法定位宿主路径的命令记录保留在 `occurrences_R3.json`（可能与已定位记录重合，不称111个新spec）。命令记录只证明记录了调用，不等于构建成功；argv比较副本不当作对账轮新执行。

抽取器自校准7例通过（3个rpmbuild/绑定解析阳性、4个只读检索/打印/rpmspec阴性），见 `extractor_calibration_R3.json`。它不证明任意Python包装器、动态shell求值和未保存日志均可覆盖。

| 比较结果 | 路径数 | 说明 |
| --- | ---: | --- |
| 与sandbox一致 | 38 | 11个远端HEAD本轮重新只读核对；仅忽略VCS与纯空行 |
| 与原仓一致（未改过） | 15 | libcynara-commons/security-manager；codes对应spec干净，HEAD见raw/R3_031 |
| 有功能差异未推送（获准两项的副本） | 6 | 与本轮llvm/runtime候选功能内容相同，等待两笔推送 |
| 有功能差异未推送（其它历史副本） | 26 | 触发停报，逐项完整diff保留；不能直接解读为26个当前QuickBuild漏推包 |
| NOT_AVAILABLE，未判定 | 27 | 无唯一原仓spec参照，不能硬塞进三种结论之一 |

**逐路径完整表**：`spec-audit_R3/LOCAL_SPECS_R3.tsv` / `.md` / `.json`，含包名、路径、SHA、最早/最近轮次、全部出现轮次、原文证据与差异路径。SHA为当前保留文件；不能证明一个多次复用路径在所有历史调用时字节均未变化。若缺足以排序的原始时间戳，最早/最近记NOT_OBSERVED并保留全部轮次，不按mtime猜日期。

“有功能差异未推送”是依本轮要求以当前参照做出的差异标签，**不证明该历史配方仍被当前验收采用、不证明它的差异都是新改动，也不证明QuickBuild需要推该旧版本**。本轮没有把这层差别抹平。需要人工明确历史旧配方、GCC测试开关、自建测试包的处置口径后才能闭合目标。

## 三、额外差异与停止点

全部26项完整原文：`spec-audit_R3/EXTRA_DIFFERENCES_R3.diff`；各自另有原始diff与去VCS/空行diff，不省略参数和注释。

| 项目ID | 可直接观察到的差异 |
| --- | --- |
| S003、S110 | 早期bpftrace候选比已推7831fb34少Patch0与prep应用；当前验收S001与sandbox一致 |
| S019–S022 | 更早bpftrace导出spec与当前已推源码static配方不同 |
| S029、S035 | 旧boost配方缺少后来保存/恢复Boost.Build flags并传给b2 install的代码；S048–S050与已推配方一致 |
| S051、S053、S054 | tensorflow2多出明确标注validation-only的 `r105_validation_force_gcc` 开关；不能把测试开关自动定为发布遗漏 |
| S056 | tensorflow2 GCC基线副本与当前适配spec不同 |
| S069–S071 | 早期pcre验证副本与当前sandbox不同 |
| S008、S026 | 旧LLVM配方与当前f203923a不同，不等于本轮单行libclang改动 |
| S023、S097–S104 | 旧runtime配方含历史patch展开、编译器选择/版本化条件等差异，不等于本轮静态库候选 |

27个无唯一参照的路径涉及：bpftrace-undefined-harness 2、tensorflow2-aot模板3、r29-consumer 6、flatbuffers 4、lottie-player 2、r30-pthread-cancel-probe 5、r31-cancel-matrix 2、r32-forced-unwind-candidates 3。已试直接对应 `codes/<包名>/packaging/<spec>` 与 `codes/R104/<包名>/packaging/<spec>`；不因包名是模板或测试名而虚构原仓。发现额外差异后没有继续扩大参照查找、没有提交其它配方。

## 四、交接与自行判断

- 两笔本地提交可供审阅签字，但本轮扫描门禁尚未闭合；签字推送命令见W4_SIGNOFF.md，**本轮不执行**。
- QuickBuild前置：人工裁决额外历史/测试配方差异的适用范围并闭合清单 → 人工签字推LLVM两笔、核远端SHA → 人工批准QuickBuild。不能声称本轮已实现“所有所需改动都在sandbox”。
- 不再把R2全根逐包未归类项当本轮继续任务；其历史表及NOT_OBSERVED全部保留。
- 自行判断：扩展扫描旧命名以覆盖R104等真实构建记录；仅通过明确bwrap绑定/同一执行日志定位路径；无法定位或找参照就保留不可核项；不给历史差异自动豁免；保持非功能忽略范围只限VCS与纯空行。
- 技术性记录：起始过宽AGENTS索引主动结束，改查明确父级目录；未发现适用额外指令。`systemctl --user is-system-running`为degraded（退出1），实际scope创建及memory.max检查均通过；不把该查询退出1当prep失败。比较器/抽取器为修正覆盖旧stdout命名作了有记录的迭代，未改旧证据。
- 项目提交只含本材料目录和LINE_STATUS；包仓不推，codes只读，没有Signed-off-by或QuickBuild操作。最终项目推送SHA见交付回执。
- 收尾自检已执行：暂存材料凭据扫描PASS、两笔作者/无签字/工作树干净/ahead2检查PASS、R2原始FAIL与inputs_R2未变。`SELFCHECK_R2.json`已有的上一轮暂存计数变化未纳入本次提交；本轮另写`SELFCHECK_R3.json`与脚本SHA清单。
- 收尾查询曾误读不存在的`prep-result.json`（raw/R3_033保留报错；该复合shell最终退出0，子命令退出码未单独采集），随后只读列出目录并读取真实`prep-command_R3.json`，其中rpmbuild退出码为0（raw/R3_034、035）。未因此重跑prep或改变证据。
