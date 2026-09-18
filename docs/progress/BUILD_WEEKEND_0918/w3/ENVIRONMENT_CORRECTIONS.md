# 输入环境修正与失败留痕

以下均是本轮实际观测，不以修复脚本代替验证通过。

| 原始记录 | 观测 | 处理与边界 |
| --- | --- | --- |
| 004、010、011 | cp 无法读取原根 usr/sbin 下 root:root/0700 的 glibc_post_upgrade、unix_update | 整个 usr/sbin 保留原根只读挂载，不读取受限文件，不声称该目录完成 SHA256 核验；不新增 sudo |
| 012、014 | 复制保留了 usr/bin、usr/lib64 等 0555 模式；再次复制不能替换已有链接 | 新建 input-aarch64-v3，保留旧失败目录；015 完成 21,273 条核验 |
| 016 | RPM --test 发现 libffi-devel-static 3.4.7 缺匹配 libffi-devel | 019 从已观测仓库目录下载两架构 libffi/libffi-devel，记录 URL、目录哈希、RPM 摘要和依赖 |
| 017、018 | 固化 PATH 内没有 rg，检索实际未执行；shell 最后的 date 返回0 | 不把空输出视为不存在；日志记录器 PATH 加入实际 rg 所在目录。017 的检索结果不可用 |
| 022、024、026 | RPM 输入安装报 cpio ENOTTY；独立写探针 mkdirat 在 usr/lib64 返回 EACCES | 028 仅让 17 个当前用户所有的临时输入目录增加所有者写权限；032 对明确列出的输入重装成功。重装保留依赖/冲突检查，不改来源权限。ENOTTY 与 EACCES 的关联以本次前后结果为限 |
| 025 | 资源监控在 CMake try_compile 的 Make 调用时停止；旧监控未记录该次完整 argv | 修正为 GNU Make 没有 -j/并行 jobserver 请求时按其默认单任务处理；实际参数纳入日志。Ninja 仍要求显式 -j1；不将本次停止写成包回归 |
| 030、033、034、035 | GCC 14.2.0 的配置链接失败：AArch64 ld 读取现有 x86_64 liblto_plugin.so | 仅在调用挂载中选用原 GBS emul 的匹配 x86_64 linker；不改旧根。原 GCC driver 实测生成 AArch64 ELF。完整包是否通过另看构建结果 |
| 037、039 | 命名空间 root 下 tar 恢复归档 UID 1000 失败（未映射 UID） | RPM 输入安装使用私有命名空间 root；包构建恢复映射普通 UID/GID 1000。不改原 tar 与源码 |
| 040、047、049–051 | 主程序构建完成，static 配置前 `realpath: command not found` | 仅在候选 spec 的 libc++ 分支，将相对路径计算改为 build 目录内指向同一 libclang.a 的符号链接；完整 diff/新 SHA256 已保存，GCC 与未定义分支不改 |
| 051、056–061 | 主程序与 static 均链接完成，但 %install 找不到原平台 HAL 检查脚本 | 补充原 etc/hal 的只读挂载与三个文件的身份；不删除/跳过该钩子。另准备针对实际候选 spec 的补充检查，避免混用 SOURCES 中旧 spec 的结论 |

其中目录复制重试、构建用户映射、未预检 realpath、遗漏 HAL 输入以及 Make 默认串行误判，属于本轮准备/监控脚本的问题，不作为 bpftrace 或 libc++ 的回归证据。

历史 armv7l LLVM 整轮失败是 `cpio: write failed - Function not implemented`。旧 rsync 错误是读取 llvm-opt-report 时 EPERM。二者不是同一条错误，不能合并归因；本轮 W2 分开核查。

## 执行脚本对应

每个构建/复制格记录执行参数、脚本 SHA256、实际 cgroup 内存值、优先级和 I/O 探测。前后版本的 guard 在 `guard-version-preservation.json` 中逐一与启动时 SHA256 对上：部分快照是在启动后保存，明确写为后保存的 SHA 匹配副本，而不是冒称启动前已落盘。命令、技术性失败退出码保留，不回填成功。
# 追加：安装后检查输入遗漏

058 中主程序与 static 完成链接，HAL 检查通过后因 `/etc/app-rootstrap/app-rootstrap-checker.sh` 不存在而失败。私有根的白名单复制遗漏了该原根目录；这是本轮准备失误。074 核对实际安装后脚本，app-rootstrap 为最后一项；补挂原目录只读、记录文件 SHA256，不改检查器、不禁用检查、不修改原根。再次完整 `rpmbuild -bb`。

## armv7l GCC 的既有 GBS 链接器/插件配对

147 首次完整 GCC 格在 CMake 编译器链接检查失败：`liblto_plugin.so: wrong ELF class: ELFCLASS64`。151 记录原目标链接器为 ARM ELF32、插件绝对链接指向 guest `/emul/` 的宿主文件；在 host 上直接 `file -L` 不会自动按 guest 根解析该绝对链接，故其“无法打开”不能写为 guest 插件缺失。152 对真实 emul 插件检查并使用原有 emul GNU linker，GCC 14.2 驱动已编译/链接 armv7-a、softfp、thumb 的 ARM ELF32 小探针。仅在 bwrap 调用内只读绑定匹配链接器，原根不改、不新增工具链。随后 153 重跑完整格。该 GNU linker 修正不是 armv7l 原生 Clang 许可的替代。
# 会话恢复后的记录补充

## 补充 HAL 检查的实际 BUILDROOT

复核旧 helper（278）发现它将 ARM 路径拼成 `bpftrace-0.24.2-1.armv7l`，实际 rpmbuild 使用 `.arm`。因此不以原 ARM 补充检查退出 0 证明检查过实际载荷。RPM 解包核查、标准库依赖、SHA256 和完整编译命令审计均使用正确路径，结果不因这项 helper 错误而改写。新增 v2 helper：先要求存在唯一包含 `/usr/bin/bpftrace` 的实际 BUILDROOT，再记录该文件 SHA256、真实入口 spec 和检查器输出；原记录保留，待当前构建结束后重跑补充检查。

## 未定义工具链的测试入口

- raw/269：ARM 未定义格在 BuildRequires 阶段退出 1，仍要求仅 libc++ 分支使用的四个静态开发包；没有执行编译。
- raw/273：同样宏参数下 `rpmspec -P/--eval` 显示 `_toolchain` 未定义，展开后没有这些依赖。这个查询不能代替实际 `rpmbuild` 的宏状态。
- raw/274、275：测试入口在包含候选前打印实际状态。`rpmbuild` 显示 `HARNESS_BEFORE_UNDEFINE=1|clang`，入口执行 `%undefine _toolchain` 后及包含原样候选后均为 0，C/C++ compiler 宏为目标前缀 GCC。这里只确认接口之间的实际差异，不声称已追踪 RPM 内部每次重新加载配置的源码调用。
- 修正只在 `tmp/WEEKEND_0918/specs/bpftrace-undefined-harness.spec` 测试入口，不改候选本身、不改原构建根配置、不补装 libc++ 路径依赖来掩盖错误。两架构的 dry parse 均通过（275），完整构建另记；ARM 从 276 开始。
- 274 的首次预检误用 `-bb --nobuild`：它仍检查尚未生成的 `%files`，因此报 manifest/文件缺失。改为 `-bp --nobuild`（275）；旧脚本与失败输出保留，不能把 dry parse 通过写成实际构建通过。

raw/198 查询旧材料中不存在的 `BUILD_STATIC_0917/FINAL_RESULT.md`，退出 2；raw/199 定位实际的分阶段报告，raw/200 读取 `W1_REPORT.md`、`W3_REPORT.md`、`W4_REPORT.md` 成功。该文件名错误不代表旧实验材料不可得，也不计为构建或验收失败。
