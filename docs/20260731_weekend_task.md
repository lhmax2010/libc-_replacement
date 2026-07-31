# 周末任务规格（完整版，可直接执行）

> 致 Codex：本文件为完整任务规格。请先通读第〇至二节（上下文、纪律、目录契约），再按第三节任务队列顺序执行。全程中文输出。

---

## 〇、上下文（必读）

### 项目
Tizen 平台 C++ 标准库迁移：libstdc++ → LLVM libc++。

平台工具链已由 LLVM **21.1.1** 切换至 **22.1.8**，21.1.1 的平台镜像已不可得，因此 21.1.1 线的全部**对象级证据**作废，本次在 22.1.8 上重开。**设计与治理结论存活，不需重做。**

- 波 1 范围：cynara 相关 **5 个源码包 / 19 个二进制包**，架构 **armv7l**
- workspace（下称「工作根」）：`/home/toolchain/development/libc++_replacement/`
- 仓：`git@github.com:lhmax2010/libc-_replacement.git`
- 21.1.1 线备份（**只读**）：`/home/toolchain/development/libc++_replacement_bak/`
- 只读参考目录：`/home/toolchain/plan_evaluation/`
- staging 克隆（本次处理后转只读）：`/home/toolchain/wave1_repo_staging/`
- 开发板（本次仅只读取基线，不做任何部署或修改）：
  - `192.168.108.26` —— **21.1.1 镜像**（易失资产，可能被重刷，优先取基线）
  - `192.168.108.25` —— **22.1.8 镜像**（新平台基线）
- 磁盘可用约 1.1 TB

### 波 1 五个源码包
`codes/` 下目录名与源码包 Name **不一致**，D5 机制按**源码包 Name**匹配，凡涉及匹配一律以 spec 中的 `Name:` 为准，不得使用目录名：

| codes/ 目录 | 源码包 Name（以 spec 为准，需实测确认） |
|---|---|
| `libcynara-commons` | libcynara-commons |
| `libcynara-dbus` | libcynara-creds-dbus（需确认） |
| `cynara` | cynara |
| `askuser` | askuser-notification（需确认） |
| `security-manager` | security-manager |

### D5 注入机制（核心机制，跨版本存活）
在集中式 buildconf 中定义 allowlist 宏，按**源码包 Name** 匹配后向 `%{optflags}` 追加 ` -stdlib=libc++`、向 `%{build_ldflags}` 追加 ` -lc++ -lc++abi`。仅 allowlist 内的源码包被注入，其余包零影响。该机制已在 21.1.1 线经三层证据验证（rpmspec 展开层 / 宏求值层 / 真实构建层），机制本身无缺陷，本次沿用。

### 已知的首例真实迁移缺陷（21.1.1 线实测，跨版本有效）
`libcynara-commons` 的 `src/common/exceptions/TryCatch.h:65` 使用 `__cxxabiv1::__forced_unwind`——该类型为 libstdc++ 专有，libc++ 21/22 均不提供。**本次不修复，只清点（见 T2）。**

---

## 一、全局纪律（不可协商）

1. **fail-closed**：任一断言不成立即停，**不自行修复、不放宽判据、不跳过断言**；
2. **严禁自裁**：不得自行裁决红项性质。红项一律落盘证据后留给人工裁决；
3. **无自证**：任何结论必须有命令原文与退出码；**不得以推断补全证据**（例如不得从 loader 路径推断执行方式、不得从"看起来一致"推断字节一致）；
4. **证据可回链**：file:line / SHA256 / MANIFEST；
5. **变更不继承 PASS**：对象字节一变，其上全部结论重取；
6. **任务内 fail-closed，任务间继续**：某任务红停 → 记录 → **进入下一个无依赖任务**（不空转）；上游红停时下游记 `NOT_RUN_UPSTREAM_RED`，**不得以替代路径继续**；
7. **状态外置**：每任务在 `progress/state/<任务号>.tsv` 记 `STARTED/DONE/RED_STOP/NOT_RUN` 与时间戳；每任务产物自带 `MANIFEST.sha256`。目的：会话中断后新会话可从 `progress/state/` 恢复，无需人工重述；
8. **只读边界**：`plan_evaluation/`、`libc++_replacement_bak/`、**旧位置的全部 `GBS-ROOT*`**、两台开发板，一律只读，零写入零元数据修改（含 chmod/touch）。**旧 buildroot 不得删除**——T0 需从其缓存打捞；
9. **一根一用**：任何 buildroot 产出制品后立即退役，禁止跨 recipe 复用；产物构建完立即复制到独立归档并计 SHA；构建时所用 buildconf/gbs conf 副本随产物一并归档；
10. **磁盘守卫**：每任务开始前检查可用空间，低于 **80 GB** 即停记 `RED_STOP_DISK`，**不清理任何既有数据**；
11. **全部报告与日志说明使用中文。**

### 21.1.1 线的教训（本次必须防住的两族败因）
七月共十一次红停，其中八处是任务书规格错：

**A 族——证据回链缺失**（凭构想写环境细节）：曾把未经验证的传输方式写进任务书；曾假设某配置项承载 repo 信息而实际不承载；曾凭记忆申报差异集而实测完全不同。
→ **对策**：任何环境细节必须从冻结证据逐字取用并标出处；继承/接续类断言的差异集必须先对留存证据实测提取。

**B 族——构想性断言**（断言包含机制未承诺的性质）：曾断言 diff 的**呈现顺序**（实际 diff 对连续变更区按先删后增输出）；曾断言注入结果**以后缀结尾**（实际平台组装 `%{optflags}` 时会在其后追加架构旗标）；曾断言成员 spec **必然有 diff**（实际 spec 不引用该宏则展开层不可见）；曾断言某文件**存在于**构建工作区。
→ **对策**：断言不得包含机制未承诺的位置/顺序/存在性假设；判据优先采用**剔除等价、多重集等价**等形态无关形式。

**另两条硬教训**：
- **EOF 归一化**：「冻结原件 + 枚举增量」型副本必须以**字节拼接**构造，禁止用会整文件重写的行式工具（sed/awk/编辑器整存）；交付自检含末尾 16 字节 hexdump 与原件比对；
- **跨 recipe 复用 buildroot** 是七月四条污染阳性的共同病因。

---

## 二、目录契约（全部路径以此为准）

工作根 `/home/toolchain/development/libc++_replacement/` **即 git 工作树**。

```
libc++_replacement/
├── codes/        波 1 五个源码包                    [gitignore，不上传]
├── tmp/          临时文件、语料、全部 buildroot
│   ├── GBS-ROOT/<根名>/                            [gitignore，不上传]
│   └── corpus/   T1 语料（临时扫描材料）             [gitignore，不上传]
├── artifacts/    权威制品、打捞归档、隔离旧仓        [gitignore，不上传]
├── progress/     全量过程文档、日志、state/、MANIFEST
│                 [gitignore，不上传；本地备份]
├── docs/         定稿文档                            [上传]
│   └── progress/<任务号>/  策展证据副本              [上传，≤5 MB/文件]
└── gbs_llvm.conf
```

**规则**：
1. **`codes/` 仅放波 1 五个源码包**，不得放入其他源码；T1 的语料是临时扫描材料，落 `tmp/corpus/`；
2. **全部 buildroot 落 `tmp/GBS-ROOT/<根名>/`**，一根一用，产出即退役；**退役根在波次收官前不得清理**——它是证据，`tmp` 这个命名不代表可弃；
3. **权威制品必须离开 buildroot**：构建完成立即复制到 `artifacts/` 并计 SHA256；`local/repos/` 内副本不作权威；制品 SHA 清单与 MANIFEST 落 `progress/`；
4. **`codes/`、`tmp/`、`artifacts/`、`progress/` 一律不上传**。`progress/` 承载全量过程证据，仅作本地备份——整体入仓会使仓体积迅速膨胀到 GB 级；
5. **上传经由 `docs/progress/<任务号>/` 的策展副本进行。选取为机械规则，不是主观判断：**
   - **必传**：该任务报告 `.md`、结论类 `.tsv`（矩阵/台账/清单）、`MANIFEST.sha256`、该任务 state 行；
   - **必传（关键规则）**：**报告中以 `file:line` 引用过的每一个文件**——引用即必传，否则引用断链；
   - **不传**：逐包/逐 spec 的批量原始输出、构建全程日志、RPM/tarball/解包树、`tmp/` 与 `artifacts/` 内任何内容、任何单文件 ≥ 5 MB；
   - **未传或超限者一律登记**于 `docs/progress/<任务号>/EXCLUDED.tsv`（文件名 / 大小 / SHA256 / `progress/` 内本地路径 / 排除原因）；报告中对应引用标 `LOCAL_ONLY` 并附 SHA256；
   - **总量守卫**：`docs/progress/` 总体积超过 200 MB 即停并报告，**不得自行删减已按上述规则必传的内容**；
6. **路径隔离不等于污染免疫**：防线仍是「一根一用、产出即退役、构建时配置副本随产物归档」这三条纪律。

---

## 三、任务队列

**依赖关系**：T-1 → （T0、T0-B、T1、T2 互不依赖，且**不依赖任何新制品**）→ T3 → T4 → T6；T5 独立（依赖 T-1）；T7 → T8；T9 最后。

**即使 T3 全线失败，T0/T0-B/T1/T2 仍能跑完，周末不空转。**

**执行顺序**：T-1 → T0 → T0-B → T1 → T2 → T3 → T4 → T5 → T6 → T7 → T8 → T9。

---

### T-1　发车前置：D5 文档漂移提交 + 推送验证 + 建立工作树

**背景**：`wave1_repo_staging` 存在两个未提交修改，其中 D5 宏片段的**工作树版含一处已裁定的缺陷修复（宏胶合空格）**，HEAD 版仍是缺陷形态。若不提交，本次重开时从 HEAD 取宏块会把缺陷种回。

1. **漂移复核（提交前必做，任一不通过即停，不提交）**：
   a. `cd /home/toolchain/wave1_repo_staging`；`git status --porcelain` 原文落盘；确认漂移**恰为** `d5/d5_mechanism.md` 与 `d5/macro_definition_snippet.conf` 两项，出现第三项即停报；
   b. 两文件的 HEAD 版与工作树版各计 SHA256；
   c. 对 `macro_definition_snippet.conf`：将**工作树版**与已验证的正式 buildconf（SHA `7181723c90079f88de345c261a57c01b480ce72d65cce12672f61859e419fe86`，位于 `libc++_replacement_bak/` 下）内的 D5 宏块做机制比对——剔除注释行与 allowlist 取值行后逐字 diff，**退出 0** 即确认工作树版为正确胶合形态；非 0 即停报；
   d. 对 `d5_mechanism.md` 出 HEAD→工作树完整 diff 原文落盘；
2. **提交**：仅提交这两个文件，不得 `add` 任何其他路径；commit message 中文，注明「D5 宏胶合修复补提交：工作树版与已验证正式 buildconf 的宏块机制比对退出 0；HEAD 版为缺陷形态（宏命中值缺前导分隔符、注入点保留宏外空格）」，并附两版 SHA256；
3. **线界 tag**：在提交后的 HEAD 上打 tag `line-21.1.1-final`，message 注明「LLVM 21.1.1 平台镜像已不可得，本线对象级证据整体作废，设计与治理结论存活」；
4. **推送验证**：push 提交与 tag，输出与退出码原文落盘；失败即停报，保留本地提交，**不强推、不改写历史**；
5. **建立工作树（根目录即工作树，不用子目录 clone）**：
   a. `cd /home/toolchain/development/libc++_replacement`；确认无 `.git`；列出当前顶层内容原文落盘；除 `codes/`、`tmp/`、`gbs_llvm.conf` 外，**允许存在人工预先放置的 `docs/` 及其下文档**，其余意外内容即停报；
   b. `git init`；`git remote add origin git@github.com:lhmax2010/libc-_replacement.git`；`git fetch origin`；`git checkout -b main origin/main`；若 checkout 因既有文件冲突失败，**列出冲突文件原文后停报，不得强制覆盖** `codes/` 或 `gbs_llvm.conf`；
   c. 创建目录 `docs/`、`docs/progress/`、`progress/`、`progress/state/`、`artifacts/`；写入 `.gitignore`：
      ```
      /codes/
      /tmp/
      /artifacts/
      /progress/
      *.rpm
      *.tar.gz
      *.tar.xz
      ```
   d. 提交 `.gitignore`、目录骨架，以及 `docs/` 下人工预先放置的全部文档（**含本任务规格**），push；**本任务规格必须在 T0 开跑前入仓，并记录其 commit SHA 与文件 SHA256**——周末的全部结论只有对着这份冻结规格才有意义；
   e. 落盘：`git log -1` 原文、`git status` 原文、两级目录树；
   此后本工作树为唯一工作克隆，`wave1_repo_staging` **转只读**。

**输出**：漂移比对结论、commit SHA、tag、push 退出码、工作树 HEAD SHA。

---

### T0　21.1.1 制品与输入本地打捞

**目的**：21.1.1 平台镜像已不可得，制品成为不可复现对象。本任务不为复现，为**审计留痕与将来 A/B 对照**保住证据可读性。机会窗口只在旧 buildroot 被删除之前。

1. 先 `find` 全列**旧位置**的全部 `GBS-ROOT*`（workspace 之外，如 `/home/toolchain/tmp/GBS-ROOT*`、`~/GBS-ROOT*` 等），路径与大小落盘；
2. **只读复制**下列对象到 `artifacts/archive_2111_salvage/`，逐一计 SHA256：
   - 既有 candidate repo 的 21.1.1-2 制品（三架构共 24 个 RPM）+ repodata；
   - 旧 `GBS-ROOT*` 内 zypp / rpm 缓存中残留的 **21.1.1 工具链 RPM**（clang / llvm / lld / binutils 及其 devel 包）——逐根搜索缓存目录，命中即复制；
   - 各旧根内可确认的 buildconf / gbs conf 副本；
   - 21.1.1 线的冻结构建日志（含 `init_buildsystem --repository` 行）；
3. 输出 `INVENTORY.tsv`（对象 / 来源路径 / SHA256 / 取得时间 / 可复现性说明）+ MANIFEST；
4. 缺失对象记 `NOT_FOUND` 并列明已搜索范围，**本任务允许部分缺失，不因此停止**；
5. **严禁修改或删除任何源位置文件。**

---

### T0-B　板级基线打捞（只读，两台板）

**目的**：`192.168.108.26` 跑 21.1.1 镜像，是该平台形态的最后实物，随时可能被重刷。趁在把基线取下来；`.25` 顺带取 22.1.8 新平台基线用于对照。

1. **全程只读**：不安装、不部署、不修改任何文件、不重启、不运行本项目任何制品；
2. 对两台板分别采集（命令与原文全部落盘，标注板 IP 与镜像版本）：
   - 镜像标识：`/etc/tizen-release`、build id 相关文件、`uname -a`；
   - 已装包全量：`rpm -qa`（排序后落盘）；
   - C++ 运行时形态：`/usr/lib*` 下 `libstdc++.so*`、`libc++*.so*`、`libc++abi*.so*`、`libatomic.so*`、`libgcc_s.so*` 的 `ls -l` 与 `sha256sum`；
   - 若板上存在 clang：`clang --version` 原文；
   - 选取 2–3 个代表性 C++ 二进制（如 cynara 相关服务），`ldd -r` 输出与退出码；
3. 两板结果做对照表：包集差异计数、C++ 运行时差异、版本差异；**不判良恶，不给建议**；
4. **若任一板 ssh 不可达**：记 `NOT_AVAILABLE` 与失败原文，**继续另一台**；两台都不可达则本任务记 `RED_STOP_BOARD_UNREACHABLE` 并**继续下一任务**。

---

### T1　平台级 libstdc++ 专有用法语料扫描

**目的**：波 1 首包即撞上源码级不兼容。「大批量包可纯机器时间迁移」这一排期假设，**唯一能证实或证伪的测量就是本任务**。

1. **语料构建**：
   a. 从平台仓 repodata 取出依赖 libstdc++ 的二进制包集，映射到 `SOURCERPM`，去重得到**源码包集**；清单与快照 ID 落盘；
   b. 下载对应 src.rpm 到 `tmp/corpus/srpms/`，解包到 `tmp/corpus/extracted/`；**不得放入 `codes/`**；
   c. 下载或解包失败者逐一登记，**不停止**；
2. **扫描**（只读，不构建），逐包统计下列模式，每命中给 `包名 / file:line / 上下文三行`：
   - `__forced_unwind`、`__cxxabiv1`、`abi::`、`<cxxabi.h>`
   - `__gnu_cxx`、`#include <ext/`、`#include <bits/`
   - `__GLIBCXX__`、`_GLIBCXX_`、`std::__cxx11`
   - `-lstdc++`、构建系统文件内的 `libstdc++`
   - `pthread_cancel`、`pthread_setcancelstate`、`pthread_testcancel`
3. **输出**：
   - `corpus_hits.tsv`（逐命中）；
   - `per_package_summary.tsv`（每包命中模式数、是否命中「确定需改」类）；
   - **发生率**：命中 ≥1 个确定需改模式的包数 / **实际扫描包数**；
   - **覆盖率申报（必须）**：应扫包数、实际扫描包数、失败包数及原因——**发生率必须以实际扫描数为分母并显式注明覆盖率**，不得以应扫数充当分母；
4. **不做修复建议、不判良恶。**

---

### T2　波 1 五源包源码缺陷清点

**目的**：避免「修一个 → 重建 → 撞下一个」的串行发现，一次收齐全部缺陷。

1. **先确认五个源码包的 Name**：从各包 spec 的 `Name:` 字段逐字提取并落盘（目录名不可信，见第〇节）；
2. **诊断专用**：允许在 `tmp/GBS-ROOT/DIAG-<日期>/` 内以 keep-going 方式编译（`make -k` 或等价），**不在首个错误处停**；该根标 `RETIRED_DIAGNOSTIC_ONLY`，产物一律标 `NON_AUTHORITATIVE`，不进入任何台账；
3. 五源包逐一收集全部诊断（源包 / TU / file:line / 原文），汇总「错误类别 × 次数 × 涉及文件」；
4. 对五源包全树（含 `packaging/`）执行 T1 同一模式集的静态清点；
5. **传播面**：`TryCatch.h` 是否安装为对外头文件（spec `%files` 与 install 规则原文）；五源包中哪些 include 了它；其余命中项的跨包传播关系同样列明；
6. **注入有效性预检**：各包构建系统是否实际消费 `%{optflags}` / `%{build_ldflags}`，是否存在 `CXXFLAGS`/`LDFLAGS` 覆盖或硬编码 flag。不消费者单列 `INJECTION_NO_OP_SUSPECTED`——**此类「静默不生效」比编译失败更危险**；
7. **不起草补丁、不修改任何源码。**

---

### T3　LLVM 22.1.8 libc++ / libc++abi 三架构构建

#### T3-0　开工断言：版本一致性（不通过即红停，不得进入构建）

1. 从 LLVM 源码树提取版本：`llvm/CMakeLists.txt` 的 `LLVM_VERSION_MAJOR/MINOR/PATCH`，以及 `git describe` / tag / commit，**原文落盘**；
2. 从当前平台镜像**实际解析到的 clang RPM** 提取 NEVRA 与其 `--version` 输出，**原文落盘**；
3. **断言两者主次修版本一致**；不一致即 `RED_STOP_VERSION_SKEW`，落盘双方原文后停止本任务；
4. 若源码为开发分支（版本号带 `git`/`dev` 后缀，或 tag 非 release），**一并申报该事实并停止，交人工裁决——不得自行判定「差别不大」继续**。

#### T3-1　构建

1. **输入申报（动手前先落盘，缺失即停）**：LLVM 源码身份（tag/commit + SHA）；packaging spec commit + spec SHA256（spec 取自工作树 `packaging/`）；gbs 命令行原文（从 `libc++_replacement_bak/` 下 21.1.1 线的冻结构建记录**逐字取用**，仅替换必要的版本与路径，替换处逐项申报）；构建期 repo 集原文；所用 buildconf / gbs conf 路径 + SHA256；**实际解析到的工具链 RPM NEVRA 全列**；
2. 每架构一个全新 buildroot：`tmp/GBS-ROOT/LIBCXX-2218-<arch>-<日期>`，`--clean`，禁止复用任何既有根；
3. 构建全程日志逐字落盘（含 `init_buildsystem --repository` 行）；
4. 产物**立即**复制到 `artifacts/rebuild_2218/<arch>/` 并计逐 RPM SHA256；构建时所用 buildconf / gbs conf 副本一并归档；归档后 buildroot 标 `RETIRED_SINGLE_USE`（留痕不删）；
5. **架构顺序：armv7l 优先**（先导架构、唯一阻塞项），完成即归档；再 aarch64、x86_64。**任一架构失败不影响其余架构继续。**

---

### T4　三架构打包期门禁（依赖 T3）

1. 在新制品上完整重跑打包期全部门项，**结论不继承旧制品**；门项清单从 `libc++_replacement_bak/` 下 21.1.1 线打包报告逐字取用并申报；
2. 含：符号版本断言（**注意 `@@LLVM_21` → `@@LLVM_22`，见 T5**）、闭包 / 强 UND、红绿绑定 fixture、头文件搜索、`ldd -r` 解析；
3. **`ldd -r` 类检查须申报执行方式**（原生 / QEMU / chroot / 其他）——21.1.1 线曾出现「有结果无命令、执行方式 NOT_FOUND」的证据缺口，本次必须消除；宿主为 x86_64，x86_64 可原生执行，aarch64 若无 QEMU 则记 `NOT_AVAILABLE`，**不得推断**；
4. 逐架构出报告；某架构红停不影响其余架构。

---

### T5　门禁工具版本断言修订 + 解冻重跑（独立任务，依赖 T-1）

**背景**：冻结门禁中存在 `@@LLVM_21` 版本字面量断言。升级到 22 必须改断言；按冻结纪律，**任何工具或 fixture 变更须解冻并重跑全部 42 项矩阵**。纯机器时间。

1. 定位全部含版本字面量的断言处（file:line 全列），逐处申报修订前后文本；
2. 修订版本字面量；**除版本字面量外不得改动任何其他逻辑**——逐行 diff 落盘，出现非版本字面量的差异即红停；
3. 重跑 42 项矩阵全量，逐项结果落盘；
4. 出新冻结登记（工具 SHA + 版本号 + fixture 数）；旧登记标 `SUPERSEDED_BY_LLVM_22` 保留。

---

### T6　candidate repo 组建（依赖 T4 全绿）

1. 以新制品组建 candidate repo + repodata 生成 + repo 级 SHA；
2. **明确申报 HTTP 服务目录路径**——21.1.1 线曾出现服务目录绑定 `NOT_FOUND` 的记录缺口，本次必须消除；
3. 21.1.1-2 旧仓整体移入 `artifacts/quarantine/<日期>_2111/`（**只移不删**）；
4. 出《环境门重锚申报》：环境门 SHA 比对按新值、server 数据目录指向新仓、HTTP 前置检查需重跑——**仅申报，不越界改动执行侧配置**。

---

### T7　配置层重建

#### T7-1　冻结原件取用
1. 工作根下的 `gbs_llvm.conf`（已为 22.1.8 镜像）计 SHA256 并冻结为新原件；
2. **重新提取 effective unified standard buildconf**（21.1.1 线的提取产物 `9eac0e23…` 已作废）；提取方式与命令原文落盘；新原件计 SHA256 冻结；
3. 旧原件 SHA（`a32c1aba…` / `9eac0e23…`）标 `SUPERSEDED_BY_PLATFORM_2218` 记录保留。

#### T7-2　D5 宏块来源约束【防回归，必读】
D5 宏块**必须**取自下列之一，并申报取用来源：
- 21.1.1 线已验证的正式 buildconf（SHA `7181723c…`）中的 D5 宏块；或
- 仓内 `d5/macro_definition_snippet.conf` 的 **T-1 提交后版本**。

**严禁**取自 T-1 提交前的 HEAD 版本——该版本宏命中值缺前导分隔符、注入点保留宏外空格，是已裁定的缺陷形态。取用后逐行比对并申报一致性。

#### T7-3　正式副本生成与两级 diff 门
1. **必须以字节拼接方式生成**，禁止使用会整文件重写的行式工具；生成后自检末尾 16 字节 hexdump 与原件比对；
2. 两级 diff 门：
   - gbs conf 副本剔除 candidate repo 段后与原件 `cmp` 退出 0；
   - buildconf 副本剔除 D5 宏块后与原件 `cmp` 退出 0；
3. 变更行集断言采用**顺序无关**形式（sort 后 cmp / 多重集等价）；**禁止硬编码 diff 呈现顺序**；
4. `OFF_PROVEN` 机械重取证：新 buildconf 对既有禁用项的 grep 退出 1；
5. 两副本 SHA256 冻结。

---

### T8　注入机制三层证据重取（依赖 T7 全绿）

**为什么重跑**：D5 机制跨版本存活，但正式 buildconf 字节已变；按「变更不继承 PASS」，其上结论全部重取。机器时间，非设计时间。

1. **rpmspec 展开层**：非成员 spec 全量抽样（清单口径与清单文件 SHA 申报）+ 负对照复杂包，两侧 `rpmspec -P` 逐字节 diff 退出 0；解析失败计数两侧相等、零新增失败；
2. **条件式成员门**：五成员 spec ——
   - diff 为空：**仅当**该 spec 的 `%optflags`/`%build_ldflags` 文本引用 grep 退出 1 时判 PASS（grep 原文作为该行证据）；spec 有引用而 diff 为空 → RED；
   - diff 非空：变更行剥除两获批后缀后与原行集 sort 后 cmp 退出 0；
3. **宏求值层**：五成员 + ≥3 非成员 × 两宏，断言为「**恰一次 + 剔除等价**」：
   - formal 求值中获批后缀恰出现一次（计数落盘）；
   - 剔除该唯一后缀后与 baseline 同名同宏求值逐字节 `cmp` 退出 0；
   - baseline 两后缀 grep 退出 1；
   - **不得断言「以后缀结尾」**——平台组装 `%{optflags}` 时会在其后追加架构旗标；
4. 三层结论收束成证据链报告。

---

### T9　汇总、策展与上传

1. 出《周末执行汇总报告》（置于 `docs/`）：逐任务状态、红停清单及证据路径、未运行任务及原因、磁盘与时间统计；
2. **策展与上传**（严格按第二节规则 5，不得自行加减）：
   - 逐任务在 `docs/progress/<任务号>/` 下生成策展副本；每个任务目录附 `README.md`，说明该任务产物、结论、红停状态，以及 `progress/` 内对应全量证据的本地路径；
   - 先生成每任务报告，再据报告中出现的全部 `file:line` 引用**反查**应传文件清单——**引用即必传**；
   - 生成 `EXCLUDED.tsv` 登记未传与超限文件；
   - 校验：`docs/progress/` 内不得出现 ≥ 5 MB 文件，不得出现 RPM/tarball/解包树；总体积 > 200 MB 即停报；
3. commit message 含任务号与状态；push；**不强推、不改写历史**；推送失败记 `RED_STOP_PUSH`，保留本地全部产物，重试不超过 3 次；
4. **归档整理（把 21.1.1 线旧证据移入 `archive/line_2111/`）不在本次范围**，留待人工执行。

---

## 四、明确不在本包内（必须人工裁决）

| 事项 | 原因 |
|---|---|
| 正式构建命令冻结 | 合法差异集的枚举是人工裁决，非机械推导 |
| 波 1 候选构建 | 必然撞上源码缺陷（T2 给全量清单），补丁方案需人工定 |
| 源码补丁的载体形态与上游归属 | 改安全线代码需对方 owner 评审 + Gerrit |
| 板上部署与运行时验证 | 属 Stage B；本次对两台板**只读取基线** |
| 人工签核、Q3 二选一、G7 三 rider | 人工签署与二值门验收 |
| 晋级台账 ADMIT / 发波 | 终裁权在人 |
| 制品状态转正 | 需在正式 wave buildroot 复验 |

---

## 五、交付清单（结束时输出）

1. `progress/state/` 全量任务状态；
2. **T3-0 版本一致性断言结果**——若红停，构建类任务全部无效，须最优先呈现；
3. **T1 发生率数字 + 覆盖率**；
4. **T2 缺陷全量清单 + 五个源码包 Name 实测值**；
5. T0 打捞清单（含 `NOT_FOUND` 项）与 T0-B 两板基线对照表；
6. T3 / T4 三架构构建与门禁结果，含**实际解析到的工具链 NEVRA**；
7. T5 的 42 项矩阵重跑结果与新冻结登记；
8. git commit 列表、tag、push 退出码、`docs/progress/` 总体积。

---

## 六、现实预期

本次是**首次在 22.1.8 上构建**，T3 出红的概率不低。本任务包的设计要点在于：**T0 / T0-B / T1 / T2 不依赖任何新制品**，即使 T3 全线失败，仍能拿到打捞归档、两板基线、平台级发生率数字与波 1 缺陷全量清单——这四者足以支撑下一步全部决策。
