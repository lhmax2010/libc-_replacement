# R94：以实际 GCC 环境验证 toolchain 覆盖机制

## 1. 结论

`%define _toolchain_override clang` 对人工提供的 Tizen-Base / Tizen-Unified
标准仓构建环境 **不生效**。R91 的修复方向对该环境无效。

实际 armv7l GBS 结果如下：

| 测试对象 | spec 身份 | 结果 | CMake 实际编译器 |
| --- | --- | --- | --- |
| 不含 R91 一行 | `48fc3a299ba20...` | 失败，退出 1 | GCC 14.2.0 |
| 含 `%define _toolchain_override clang` | `dd4beffc30be...` | 失败，退出 1 | GCC 14.2.0 |
| 不改 spec、命令行直接定义 `__cc/__cxx` 为目标前缀 Clang（替代路径探针） | `dd4beffc30be...` | 完整构建成功，退出 0 | Clang 22.1.8 |

前两格都在 CMake 编译器检查阶段因 GCC 不识别
`--rtlib=libgcc` 失败。第三格不仅越过配置，而且完成全部构建并生成
armv7l RPM；生成的 `libc++.so.1.0` 与 `libc++abi.so.1.0` 都直接依赖
`libgcc_s.so.1`。

因此，总部日志并非偶发差异。标准平台环境里 `%{__cc}` / `%{__cxx}`
由目标 RPM 宏直接定义为 GCC；`_toolchain_override` 虽在 spec 中存在，
但没有任何宏消费它。

## 2. 测试对象与资源纪律

- 资源门禁：`tools/resource_gate.sh --level medium`，退出 0，`PASS`。
- GBS：2.0.8。
- 架构：armv7l。
- 调度：`nice -n 15`、`ionice -c 3`、GBS worker 1、编译 `-j2`。
- 实测配置：工作区根目录 `gbs.conf`。
- 该配置的 profile 是 `profile.tizen_unified_standard`，仓库为：
  - `Tizen/Tizen-Base/reference/repos/standard/packages/`
  - `Tizen/Tizen-Unified/reference/repos/standard/packages/`
- 测试期间采样到的可用内存约 20–21 GiB，未触发资源停止条件。

两份测试树直接从平台 Git 对象检出，未编辑 spec：

| 测试树 | commit | spec SHA256 | tracked diff |
| --- | --- | --- | --- |
| no override | `48fc3a299ba20dd321a7d4a37c4e13ce095176cd` | `a8437cca668ee785906ba5349efcd5b4b91a049ffd4b53e5234a6c2c6066af74` | 无 |
| with override | `dd4beffc30be18841818651756f4dbe1f54b0016` | `49caee5d58d737fc5a6809cd1abd19afa389c51598f174d5b9968545a7b4c8e8` | 无 |

两提交间对 `packaging/libcxx-runtimes.spec` 的差异只有：

```diff
+%define _toolchain_override clang
```

对象内容 SHA256 与检出文件 SHA256 分别一致，见
`raw/007_platform_commit_evidence.stdout` 与
`raw/009_verify_worktrees_and_resources.stdout`。

## 3. `_toolchain_override` 在哪里定义和消费

### 3.1 Toolchain 项目

机制位于：

`codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config:118-140`

关键内容为：

```spec
%__cc_clang %{_host}-clang
%__cxx_clang %{_host}-clang++
%__cc_gcc %{_host}-gcc
%__cxx_gcc %{_host}-g++
%_toolchain %{?_toolchain_override}%{!?_toolchain_override:clang}
%__cc %{expand:%%{__cc_%{_toolchain}}}
%__cxx %{expand:%%{__cxx_%{_toolchain}}}
```

这里 `_toolchain_override` 是 RPM 项目配置宏，不是 GBS 自身选项，也不是
libc++ 的构建脚本选项。Base-Toolchain 配置先根据它生成 `_toolchain`，
再由 `_toolchain` 间接选择 `__cc/__cxx`。Unified-Toolchain 配置本身没有
重复定义该段，使用 Base-Toolchain 层提供的机制。

### 3.2 Tizen-Base / Tizen-Unified 标准平台项目

以下两个实际平台配置均没有 `_toolchain_override`、`_toolchain`、
`__cc` 或 `__cxx` 的定义/消费：

- `codes/qb/TIZEN/Tizen/Tizen-Base/project_config`
- `codes/qb/TIZEN/Tizen/Tizen-Unified/project_config`

Base 配置反而明确要求 GCC：

```text
207 Required: binutils gcc glibc rpm-build libtool
208 Required: gcc-c++
```

实际 GBS 构建根中的目标 RPM 宏进一步给出直接证据：

`.../usr/lib/rpm/macros:74-76`

```spec
%__cc  armv7l-tizen-linux-gnueabi-gcc
%__cpp armv7l-tizen-linux-gnueabi-gcc -E
%__cxx armv7l-tizen-linux-gnueabi-g++
```

该文件中不存在 `_toolchain_override` 或 `_toolchain`。因此 spec 中定义
`_toolchain_override=clang` 只是产生一个无人引用的宏，不会改写上述值。

人工提供的 `gbs.conf` 同时叠加 Base 与 Unified 标准仓；GBS 实际下载的
有效配置 `/var/tmp/toolchain-gbs/tizen_unified_standard.conf` 同样没有上述
覆盖机制。两次真实构建都把 `%{__cc}` 展开为目标前缀 GCC，验证了最终
组合环境的行为，而非仅根据文本缺失作推断。

### 3.3 分环境回答

- **Tizen-Base 标准环境：不支持该覆盖机制。** 项目配置没有消费者；
  实际目标宏直接指向 GCC；实际 armv7l 构建选择 GCC。
- **Tizen-Unified 标准环境：自身也不支持。** Unified 是叠加在 Base
  之上的配置，本身未增加消费者；人工提供的 Base+Unified 实际组合仍
  选择 GCC。
- **Base-Toolchain / Unified-Toolchain 组合：支持。** 消费逻辑来自
  Base-Toolchain 的上述项目宏。R91 的 x86_64 实构建和此前 Toolchain
  QuickBuild 日志反映的是这一环境，不能外推到 Tizen-Base 标准环境。

## 4. 两次要求内的实际复现

### 4.1 不含 R91 改动

完整命令：`raw/011_build_armv7l_no_override.command.txt`。

退出码：1。

关键原始证据：

```text
-DCMAKE_C_COMPILER=armv7l-tizen-linux-gnueabi-gcc
-DCMAKE_CXX_COMPILER=armv7l-tizen-linux-gnueabi-g++
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
armv7l-tizen-linux-gnueabi-gcc: error: unrecognized command-line option '--rtlib=libgcc'
-- Configuring incomplete, errors occurred!
```

完整 GBS 输出：`raw/011_build_armv7l_no_override.stdout`。

### 4.2 含 R91 改动

第一次尝试 `raw/013_*` 在下载 `filesystem` RPM 时网络超时，尚未到
CMake，不能用于判定；保留其原始记录。缓存后重试命令见
`raw/015_build_armv7l_with_override_retry1.command.txt`。

重试退出码：1。

关键原始证据：

```text
-DCMAKE_C_COMPILER=armv7l-tizen-linux-gnueabi-gcc
-DCMAKE_CXX_COMPILER=armv7l-tizen-linux-gnueabi-g++
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
armv7l-tizen-linux-gnueabi-gcc: error: unrecognized command-line option '--rtlib=libgcc'
-- Configuring incomplete, errors occurred!
```

完整 GBS 输出：`raw/015_build_armv7l_with_override_retry1.stdout`。

两格的编译器、版本、失败选项和失败阶段完全一致。由此明确判定：

> R91 的一行修复在 Tizen-Base / Unified 标准环境无效；即使构建包含
> `dd4beffc...`，仍会复现总部的 GCC 失败。

## 5. 替代路径实测与选项（不作选择）

### 选项 A：在 CMake 参数中明确写目标前缀 Clang

可能改法：

```spec
-DCMAKE_C_COMPILER=%{_host}-clang
-DCMAKE_CXX_COMPILER=%{_host}-clang++
-DCMAKE_ASM_COMPILER=%{_host}-clang
```

实际依据：未修改 spec，而由 GBS 命令行把 `__cc/__cxx/__cpp` 直接定义为
对应目标前缀 Clang，得到与上述 CMake 展开相同的结果。armv7l 完整构建
成功，CMake 识别 Clang 22.1.8，生成七个二进制 RPM 和一个 SRPM；两个
共享库均 `NEEDED libgcc_s.so.1`。

完整命令与输出见 `raw/022_*`，产物/依赖核验见 `raw/023_*`、
`raw/027_*`。

- Tizen-Base：armv7l 已完整实测有效。
- Tizen-Unified：人工提供的 Base+Unified 组合已完整实测有效。
- Toolchain 项目：宏原本就会展开成相同驱动名；R91 的 Toolchain
  x86_64 构建已有实际成功记录。
- aarch64、x86_64 在本次 R94 未重跑：`NOT_OBSERVED`。

后果：明确贯彻“该包只用 Clang 构建”的方向；不允许平台的默认 GCC
选择渗入。代价是三处 CMake 编译器参数直接绑定目标前缀 Clang。

### 选项 B：在 spec 中直接重定义 `__cc/__cxx`

可能改法：

```spec
%define __cc %{_host}-clang
%define __cxx %{_host}-clang++
```

GBS 命令行定义同名宏的 armv7l 完整构建已经成功，因此其最终宏值与
驱动行为有实测依据。但“由 spec 内 `%define` 覆盖平台宏”的优先级形式
没有在 R94 中另改 spec 实测，记为 `NOT_OBSERVED`；若采用该选项，仍应
在两套实际环境各做一次完整构建。该做法还会改变 RPM 通用编译器宏，
作用面比只改三项 CMake 参数更广。

### 选项 C：按总部建议，仅对 Clang 添加 `--rtlib=libgcc`

可在 `%build` 的 shell 中执行当前 driver 的 `--version`，以输出中稳定的
`clang version` 识别 Clang；不要依赖标准平台里不存在的 `_toolchain`。
示意（不是已实施补丁）：

```sh
rtlib_flag=
case "$(%{__cc} --version 2>&1)" in
  *[Cc]lang*) rtlib_flag=--rtlib=libgcc ;;
esac
```

然后在 C、C++、共享链接和可执行链接四处统一使用 `$rtlib_flag`。

实测依据：实际 armv7l 构建根中 GCC 输出
`Tizen GCC 14.2.0...`，Clang 输出 `clang version 22.1.8`；QEMU 驱动探针
能把二者正确分档。Clang 接受该选项，GCC 明确拒绝。

- 标准 Base/Unified：会识别 GCC并省略该选项。
- Toolchain：会识别实际 Clang并保留该选项；若有人确实切成 GCC，
  同样会省略。

未验证点：R94 没有修改 spec，因此没有完整实测“GCC 省略该选项后”
libc++ 全包是否还能构建；该项为 `NOT_OBSERVED`。这一路线也改变了原先
“包必须由 Clang 构建”的约束，允许 GCC 路径继续，需由人工裁决。

### 选项 D：无条件删除 `--rtlib=libgcc`

armv7l 实际 Clang 22.1.8 的 `-###` 链接计划显示：有无该选项都选择
`-lgcc` 与 `-lgcc_s`，见 `raw/028_*`。因此在当前 armv7l 驱动配置上，
该标志是冗余的；GCC 也将不再遇到未知选项。

但是该事实只在本次 armv7l 构建根实测。其他架构、未来 Clang 配置和
完整 GCC 构建均为 `NOT_OBSERVED`；无条件删除还会弱化 spec 对展开器的
显式约束。

### 不可靠的判断方式

- 用 `_toolchain` / `_toolchain_override` 条件：标准平台配置没有消费者，
  不可靠。
- 仅凭 `BuildRequires: clang`：它只保证安装 Clang；此次构建根同时安装
  GCC 与 Clang，实际仍选择 GCC。
- 在 spec 配置前直接判断 CMake 的 `CMAKE_*_COMPILER_ID`：ID 要在 CMake
  `project()` 后才产生，无法直接解决传给首次 compiler check 的 flag。

本报告只列选项及证据，不推荐、不排序、不实施。

## 6. R91 第一阶段为什么查错

R91 的方法链有四个具体问题：

1. **环境名称混同。** 查的是 `Tizen-Base-Toolchain` 和
   `Tizen-Unified-Toolchain` 的 project_config；总部失败发生在
   `Tizen-Base` 标准平台环境。这是不同项目配置。
2. **三架构证据来自 Toolchain 快照。** R85/R91 引用的 QuickBuild
   armv7l、aarch64、x86_64 日志属于 Base Toolchain 构建，只证明那个
   项目选择 Clang。
3. **本地实构建只覆盖 Toolchain 仓的 x86_64。** R91 使用的
   `progress/T3R/config/gbs_llvm.conf` 指向 `Tizen-Base-Toolchain` 与
   `Tizen-Unified-Toolchain`；没有用这次人工提供的标准仓 `gbs.conf`
   实测 armv7l。
4. **宏探针预先注入了支持机制。** R91 的合成 `rpmspec` 探针主动定义
   `_toolchain`、`__cc_clang`、`__cc_gcc` 等，证明的是“机制存在时宏如何
   展开”，不能证明标准平台环境提供了该机制。

由这些局部事实外推出“Base、Unified 当前三架构均实际使用 Clang”是不
成立的。准确表述应限缩为：此前被检查的 Toolchain 项目三架构使用
Clang；Tizen-Base / Unified 标准平台需另测。

今后同类问题的可靠顺序应是：

1. 固定人工实际使用的 `gbs.conf`、profile、仓 URL 与架构；
2. 用目标 spec 做真实 GBS 构建，至少走到 CMake compiler check；
3. 从日志同时记录完整 CMake 命令和 `compiler identification`；
4. 读取实际构建根的 `/usr/lib/rpm/macros`，确认最终 `__cc/__cxx`；
5. 对每个 profile/架构分别建矩阵，不把 Toolchain 项目结果外推到平台
   标准项目；
6. 合成宏探针只用于解释机制，不能代替环境判定。

## 7. 技术性非零与恢复记录

以下不是判据失败，均保留原始记录：

- `004`：日志包装器参数误传重复目录，核对命令未执行；随后按接口重跑。
- `006`：平台大仓 `git status` 长时间阻塞，SIGTERM 后恢复记退出 143；
  退出码为恢复记录，不是包装器自动写入，未改文件。
- `008`：两工作树成功检出，末尾 `git diff` 把文件路径当 revision，退出
  128；随后用对象 SHA256 与文件 SHA256 重验通过。
- `010`：从测试树使用相对包装器路径，shell 返回 127，GBS 未启动。
- `013`：含改动首试下载 RPM 网络超时；重试 `015` 才到达判据点。
- `017`：直接 sudo 只读探针需要交互密码，退出 1；未绕过权限，改用
  rootless 文件读取。
- `020`：主机不能直接执行 ARM 目标程序，返回 255；改用 QEMU 用户态
  探针 `021`。
- `026`：GCC 的主机侧 QEMU `-###` 探针因脱离 chroot 找不到 LTO plugin；
  不用于构建成败判定。

## 8. 自行判断、疑问与边界

自行判断：

1. 为避免改变任一 spec，使用两个精确提交的 detached worktree 做 A/B。
2. 含改动首试网络超时不属于判据失败，复用缓存重试一次。
3. 额外执行“命令行直接定义 `__cc/__cxx`”探针，只用于验证替代路径，
   不代表选择该方案。
4. 人工 `gbs.conf` 是 Base+Unified 组合，故实际复现结论适用于该组合；
   分别对两个 source project 的“支持情况”还结合了各自实际
   project_config 与构建根最终宏。

尚存疑问 / 未覆盖：

1. 总部具体失败构建使用的源码提交是否包含 `dd4beffc...` 未取得其完整
   provenance；但本次已证明，即使包含该提交也仍失败。
2. 未分别创建 Base-only 与 Unified-only 的独立 GBS profile；人工提供的
   实际 profile 正是 Base+Unified 组合。两份配置均无覆盖机制，组合实测
   也无效。
3. aarch64、x86_64 标准平台 profile 未在 R94 重跑：`NOT_OBSERVED`。
4. 条件省略 flag 后的 GCC 全包构建：`NOT_OBSERVED`。

本任务没有修改 `packaging/libcxx-runtimes.spec` 或任何平台源码，没有推送
Gerrit/外部源码仓，也没有选定替代方案。
