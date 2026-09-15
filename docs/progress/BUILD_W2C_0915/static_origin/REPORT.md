# bpftrace 预编译资产：来源与镜像归属核查

## 结论先行

1. **未证实来自qemu-accel，来源仍未知**。提供的仓库当前树、可达本地
   分支及已检查的实际qemu-accel RPM清单中，均未观测到bpftrace构建/收集
   记录或对应二进制。因此SHA256对应比对为NOT_AVAILABLE，不是“哈希不同”。
2. **qemu-accel定位是QEMU交叉构建环境的宿主原生工具加速**，spec明确
   声明不用于系统安装。这个定位不能外推给尚未证明与它有关的bpftrace-static。
3. **六份指定历史镜像清单均没有bpftrace-static，也没有bpftrace或qemu-accel
   包族**。这是固定快照内的结论，不排除测试时加装、新镜像或其他清单。
4. 两架构资产中指定的`_ZNSt3__1`/`_ZNSt7__cxx11`字符串均NOT_OBSERVED。
   找到`v0.24.2`、`Clang 21.1.1`、`LLVM version 21.1.1`，但这些不能证明
   生成工具链身份或内嵌标准库。它们并非完全静态ELF，仍动态依赖C库。
5. “来自qemu-accel且能重建”的前提尚未成立，不能给出可靠重建耗时。
   未实施、未重建、未执行资产、未推送bpftrace；其余10包状态不变，门禁交人工决定。

## 一、输入身份与来源查证

提供仓库：`codes/qemu-accel`，远端配置
`git://review.tizen.org/git/platform/upstream/qemu-accel`；本地`tizen_base`
HEAD为`2737f3bf78ed33be4333114c0e5c741a1deca467`，工作树干净。
当前树只有6个文本打包文件，无内置ELF或tar；逐文件SHA256见qemu_source_files.tsv。

执行过的检索（原命令、输出、退出码见raw/002–006及commands.jsonl）：

- 当前packaging目录大小写不敏感检索`bpftrace|libbpf|prebuilt-static|bpf`，0命中，
  rg退出1表示未匹配，不是脚本失败。
- 本地147条分支/远端跟踪引用去重为97个tip，逐棵树git grep
  `bpftrace|prebuilt-static|libbpf`，0命中；没有联网补取未提供的分支。
- 仓库不是浅克隆；本地可达350个提交，提交消息bpf检索及历史
  `bpftrace|prebuilt-static`变动检索未命中。仅声明已可达历史的检索结果，
  不声称排除了别的仓库、未公开分支或外部生成流程。

进一步核查实际构建根的两个RPM（raw/011–018）：

| RPM | VCS | 内容核对 |
| --- | --- | --- |
| qemu-accel-x86_64-armv7l-0.4-1.1.armv7l | platform/upstream/qemu-accel#e01aa7250a1a73aa8f88ba9ac4a05cbc954d1c9f | 文件清单不含bpftrace/libbpf/bcc |
| qemu-accel-x86_64-aarch64-0.4-1.1.aarch64 | 同上 | 文件清单不含bpftrace/libbpf/bcc |

两者来自此前本轮构建使用的Base-Toolchain reference仓，不冒充六镜像的固定快照
产物，也不与提供仓库HEAD混为同一revision。实际RPM的VCS revision亦在本地
97棵tip检索范围内，其spec另存于raw/015。
armv7l构建根`/emul/usr/bin/ls`实测为x86-64 ELF，印证其“目标架构包容纳
宿主原生程序”的机制；仅抽查该ELF，不声称逐个验证了所有载荷架构。

### tar与二进制的SHA256必须分开

人工给出的`3785365b…`是**整个Source1002 tar**的哈希，不是任一内含ELF。
本轮重新从tar读取成员并与此前解出的文件逐字节对照：

| 对象 | SHA256 |
| --- | --- |
| packaging/prebuilt-static-bpftrace.tar.gz | 3785365b768e29a2f6a31a54b5f1843b3fc64d974ecc7dd588197644d3514123 |
| prebuilt-static-bpftrace/arm/usr/bin/bpftrace | ade7530b3824dd83eebba837928d3919dcb4cd7708212a8607c3629cb5c55f5b |
| prebuilt-static-bpftrace/aarch64/usr/bin/bpftrace | 4d12f98487f70addfeaa1602f91463458a738a4dd437241a70f77ab54680cab1 |

qemu-accel没有可供配对的bpftrace成员，无法进行两个来源之间的同文件SHA256
比对。不能把当前目录“没有对象”写成“对象不一致”或“确定不可能来自它”。

bpftrace本地历史可见该tar于`d63c8f0dae915021e9497981f257a89244582314`
处出现，提交摘要为“Convert shell scripts from bash to dash compatibility”。
但**该仓库是浅克隆**，这个提交显示整树导入形态，不足以确认为资产最初引入
或生成提交，更不能根据提交作者推断谁编译了二进制。未fetch或修改浅克隆边界。

## 二、qemu-accel是怎样工作的

当前`packaging/qemu-accel.spec.in`实际代码（raw/005保留带行号全文）：

```spec
# 31–32
# default path in qemu
%define emul_path /emul
# 81–89
Summary:        Native binaries for speeding up cross compile
License:        GPL-2.0
Group:          Development/Cross Compilation
ExclusiveArch:  x86_64

%description
This package is used in %{cross} architecture builds using qemu to speed up builds
with native binaries.
This should not be installed on systems, it is just intended for qemu environments.
```

该spec的`%prep`为空，`%build`只有sanitizer条件下的gcc_unforce_options，
不是bpftrace编译流程（121–128行）。`%install`枚举构建根中既有工具、
收集动态依赖（167–358行），复制到/emul并修改解释器/运行路径（361–395行）：

```sh
# 307–308：已有可执行文件及其依赖
for executable in $executables; do
  bins="$bins $executable `ldd $executable 2>/dev/null | sed -n 's,.*=> \(/[^ ]*\) .*,\1,p'`"
# 379–386：复制、重定位已有程序
mkdir -p ${outfile%/*}
cp -aL $binary $outfile
[ "$binary" = "$rtld" ] && return
[ "$binary" = "%{emul_path}$rtld" ] && return

# not all binaries have an .interp section
if patchelf --print-interpreter $outfile 1>/dev/null 2>/dev/null; then
  patchelf --set-interpreter %{emul_path}$rtld $outfile
```

上述是阅读源码，**本轮没有执行ldd/patchelf或打包脚本**。
BuildRequires同时有gcc、clang、libstdc++（38、53、68行），只能证明
收集/打包所需输入，不能据此认定未知bpftrace资产的生产编译器或链接方式。

所以：qemu-accel按设计是构建环境工具包，不是产品运行时组件；但来源关联
未建立，不能据这个设计结论把bpftrace-static自动归类为“仅构建机使用”。

## 三、六份镜像清单的精确核对

复用的是`docs/progress/R98/inputs/image_packages/`六份清单，不使用GCC世界
的R95清单代替。身份为：
`Tizen-Unified-Toolchain/tizen-unified-toolchain_20260829.015247`。
完整下载URL、文件SHA256和条目数见image_membership.tsv，逐一与原下载记录
`docs/progress/R98/raw/010_fetch_llvm_snapshot_metadata_final.stdout`比对，全部一致。

| 镜像 | 条目数 | bpftrace-static | bpftrace | qemu-accel包族 |
| --- | ---: | ---: | ---: | ---: |
| tizen-boot-aarch64-rpi | 194 | 0 | 0 | 0 |
| tizen-boot-armv7l-rpi | 194 | 0 | 0 | 0 |
| tizen-headed-aarch64 | 1262 | 0 | 0 | 0 |
| tizen-headed-armv7l | 1265 | 0 | 0 | 0 |
| tizen-headless-aarch64 | 742 | 0 | 0 | 0 |
| tizen-headless-armv7l | 742 | 0 | 0 | 0 |
| 合计（镜像间不去重） | 4399 | 0 | 0 | 0 |

格式为`NAME.ARCH VERSION-RELEASE VCS`，按最后一个点分离NAME与ARCH后
精确匹配bpftrace-static/bpftrace；qemu-accel包族匹配精确名或`qemu-accel-`
前缀，非任意子串。以4个含近似名的正反样本校准匹配方法，未靠包名经验判断。

结论仅为**这些已固定清单中没有相应RPM条目**；未对镜像根文件系统逐文件
查找，更不能排除未由这些RPM记录的同名文件、后续安装、测试工具注入、
其他镜像或迁移后新快照。该批快照也不是全平台已改用libc++的镜像。

## 四、二进制字符串与可确认的链接性质

用`strings -a`读取全部可打印ASCII字符串，readelf/file核对ELF，未运行
二进制。完整输出及命令在commands.jsonl；额外检索保持独立记录，不覆盖初次输出。

| 观察 | armv7l | aarch64 |
| --- | --- | --- |
| `_ZNSt3__1` | NOT_OBSERVED（0） | NOT_OBSERVED（0） |
| `_ZNSt7__cxx11` | NOT_OBSERVED（0） | NOT_OBSERVED（0） |
| 可读`std::__1` / `std::__cxx11` | 均0 | 均0 |
| 版本字串 | v0.24.2、Clang 21.1.1、LLVM version 21.1.1 | 同左 |
| 动态依赖 | libm.so.6、libc.so.6、ld-linux.so.3 | libm.so.6、libc.so.6、ld-linux-aarch64.so.1 |
| ELF状态 | ARM EABI5，stripped | AArch64，stripped |

因此它们**不是整个ELF完全静态链接**。C++部分是否/怎样静态链接仍需生成
记录、链接图或未strip符号佐证；没有libstdc++/libc++的DT_NEEDED不能证明
没有C++运行时代码。指定前缀零命中不等于它们不存在于已剥离的代码中。

进一步扩大词形后，两个资产同时含有“libc++”和“libstdc++”，例如：

```text
Use libc++ (rather than libstdc++) [-stdlib=]
libc++,libstdc++,none
include path for libstdc++ headers not found; pass '-stdlib=libc++' on the command line to use the libc++ standard library instead
```

这些是工具支持的选项/诊断文本，不是两套库都实际链入的证明。
初次宽泛版本规则各匹配60行，其中混入`gcc::Linker`和嵌入头文件注释，
不把60当作60个真实版本。后续逐类结果见additional_string_evidence.json。
`Clang/LLVM 21.1.1`可作为资产所含工具代码的线索，不证明生产它的driver版本；
与本轮源码构建使用Clang22.1.8的事实分开陈述。
strings链路用4条已知阳性字串核查，可打印前缀和版本均能检出。

## 五、重建可行性与缺失材料（未实施）

无法从qemu-accel找到生成入口，故“来自该仓且可重建”的条件未成立。
**恢复原资产并切换libc++的可行性与总耗时目前不能可靠判定**。

只读检查发现bpftrace源码另有通用静态构建入口：
`.github/include/static.sh:14–20`用docker/Dockerfile.static配置
`-DSTATIC_LINKING=ON`；`src/CMakeLists.txt:92–94`含
`target_link_options(bpftrace BEFORE PRIVATE "-static-libgcc" "-static-libstdc++")`。
但现有CI入口用Alpine3.22、LLVM18包，脚本26–29行期望musl-x86_64依赖，
与本资产的ARM/AArch64、glibc、嵌入LLVM21.1.1线索不一致。
这不能当作Source1002的可复现配方，本轮没有运行或改动该脚本。

要形成可执行重建评估，至少缺：

- Source1002原生成仓库/分支/revision、完整命令和目标sysroot；
- 对应LLVM/BCC等静态输入的版本、补丁、标准库及来源；
- 两架构生成/打包/strip步骤与ABI、功能验证要求；
- 原流程构建时长，或允许在明确输入后作受控小规模计时。

已有四格记录是普通源码程序构建，不含未知静态依赖重建成本，不用于冒充
本资产重建估时。静态输入需与所选标准库兼容，这是待验证前提，不是已验证结果。
未选择门禁口径，未建议把未确认项按“安全”放行。

## 六、纪律、问题与交付

light资源门禁退出0。本轮只读codes与原材料；未改spec、源码、配置，
未重建、未执行资产、未连接板子、未提交或推送任何包。其他10包状态不变。
报告与证据在static_origin子目录另存，不改上轮构建结论。

技术性修正：首次RPM查询默认dbpath错误，试图读取root内的/home/toolchain/.rpmdb，
退出1；不能将stdout的“not installed”当作包不存在。显式指定/var/lib/rpm后
两架构查询成功，原错误和修正都保留（raw/009、011、016）。

自行判断：选用此前明确的六份Toolchain镜像并校验原下载哈希；扩展检查
本地ref tip、历史及实际RPM文件清单；把版本字符串作为线索而非工具链证明；
将源代码中存在的通用CI静态入口与原资产生成机制区分。未据负面检索排除
其他来源，未替人工改变门禁。

待人工确认：qemu-accel线索是否指另一个分支、包或仓库？能否提供Source1002
的生成记录/资产来源？六清单之外是否存在测试镜像或加装清单？查证到此停下。
