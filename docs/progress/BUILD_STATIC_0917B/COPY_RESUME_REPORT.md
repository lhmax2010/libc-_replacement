# 缩小复制范围的执行记录

## 结果

人工批准的停止旧复制、缩小范围复制、恢复所有权与删除密码文件四步已完成。新根是 `tmp/STATIC_0917B/llvm-root-minimal`；旧的未完成副本 `tmp/STATIC_0917B/llvm-root` 未删除，也不作为构建输入。

| 项目 | 实测结果 | 证据 |
| --- | --- | --- |
| 停止旧 cp | sudo kill 退出 0；原 cp 最终退出 143 | raw/058、059、060；raw/037 |
| 复制前大小 | 1,070,743,552 字节，小于 3 GiB | raw/064 stdout 中逐目录 du |
| 新复制 | 退出 0，17,949 条输出，约 106 秒 | raw/064 |
| 复制进程内存上限 | 地址空间软、硬上限均 16,536,451,072 字节 | raw/064 中 helper 与 cp 的 limits |
| 优先级 | nice 19，ionice idle | raw/064 |
| 所有权恢复 | 两份副本均恢复为 1000:1000，退出 0；逐项检查无其他 UID | raw/065、071 |
| 密码销毁 | shred 退出 0，之后确认文件不存在 | raw/066、071、097 |

第一次复制的内存限制被 sudo 重置为 unlimited，且误带入旧 GCC 构建目录；这两项错误没有隐去。第二次使用 sudo 后执行的 `prlimit --as`，并读取实际 cp 的 `/proc/.../limits` 验证。地址空间限制并非 RSS 或整个进程组内存之和限制。

## 复制范围

源根：`tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0`。

| 目录 | du 字节数 | 用途 |
| --- | ---: | --- |
| usr | 760025088 | 原有构建工具、头文件、库、RPM 工具 |
| etc | 503808 | 根内工具配置与 RPM 宏 |
| var | 4874240 | 根内 RPM 数据库等状态 |
| .build | 4096 | 原构建环境元数据 |
| emul | 305336320 | 交叉构建根已有的宿主工具运行环境 |
| bin、sbin、lib、lib64 | 0 | 指向 usr 下目录的符号链接 |

未复制原 home 下约 11 GiB 的旧 GCC 构建树，也未复制其他历史构建产物。新根只额外建立空的工作目录与挂载点。后续自行生成的源码展开与构建文件不属于此次复制集合。

复制辅助脚本逐 500 条复制输出进行小规模读写探测；此次没有超过 30 秒，无需暂停。原始探测命令、输出、退出码在 raw/064。复制脚本快照及 SHA256 在本目录与 raw/063。

## 全部 sudo 调用

以下命令均使用 `< password.md` 从标准输入读取，未展开密码。各调用的外层记录器命令原文也已保存。

1. raw/037：原整根复制，最终退出 143。

```sh
sudo -S -p "" -- cp -a --reflink=auto --one-file-system -- /home/toolchain/development/libc++_replacement/tmp/GBS-ROOT/BUILD-W1B-llvm-aarch64/local/BUILD-ROOTS/scratch.aarch64.0 /home/toolchain/development/libc++_replacement/tmp/STATIC_0917B/llvm-root < password.md
```

2. raw/059：核对 PID、命令与进程开始标识后停止原 cp，退出 0。

```sh
sudo -S -p "" -- /usr/bin/kill --signal TERM 605945 < password.md
```

3. raw/064：受限的最小复制，退出 0。`copy_memory_bytes` 由 MemTotal 计算，实际展开为 16536451072；不是密码或其编码。

```sh
copy_memory_bytes=$(awk "/^MemTotal:/ {printf \"%.0f\", int(\$2/2)*1024}" /proc/meminfo)
sudo -S -p "" -- /usr/bin/prlimit --as="$copy_memory_bytes:$copy_memory_bytes" -- /usr/bin/python3 /home/toolchain/development/libc++_replacement/progress/BUILD_STATIC_0917B/copy_minimal_root.py < password.md
```

4. raw/065：复制完成后恢复两份副本的所有权，退出 0。

```sh
sudo -S -p "" -- /usr/bin/chown -hR -- 1000:1000 /home/toolchain/development/libc++_replacement/tmp/STATIC_0917B/llvm-root /home/toolchain/development/libc++_replacement/tmp/STATIC_0917B/llvm-root-minimal < password.md
```

随后 raw/066 执行 `shred -u password.md`，退出 0。没有用于安装软件、系统配置或构建的 sudo 调用；后续准备使用普通用户的私有 user namespace。

## 续跑中的技术性修正

- RPM 输入目录中存在绝对符号链接，第一次容器挂载无法解析目标：改为解析到真实文件后逐个只读挂载，保留失败记录与执行过的脚本版本。
- 依赖预检先后要求同版本 llvm、lldb：补入实际已有的 22.1.8-108.2 aarch64 RPM，不使用 `--nodeps`。
- 宿主为 x86_64，目标 RPM 为 aarch64：逐包核对架构后，仅在私有交叉根安装时使用 `--ignorearch`，仍执行完整依赖校验。
- 私有根保留了部分 555 目录，导致普通用户安装写入失败：raw/092 逐项记录并增加这些目录的所有者写权限，不触碰源根。
- raw/095 最终安装及编译器版本检查均退出 0；实际输出 `clang version 22.1.8`、`Target: aarch64-tizen-linux-gnu`。
- raw/096 诊断命令中误写一个日志文件名，cat 报缺失；raw/097 使用真实文件名、启用错误即退出后复核三个退出码均为 0。没有把外层命令最后退出 0 当作前一个 cat 成功。
- raw/100 完整 rpmbuild 首次在解包模型归档时退出 1：安装阶段 namespace UID 0 不具备恢复归档内 UID 1000 的映射。改为以当前普通用户 UID/GID 1000 运行 rpmbuild；raw/105 重试已通过 `%prep` 并进入 CMake。没有更改归档或源码。
- raw/107 核对实际 rpmbuild 进程的地址空间上限、nice 与 I/O 等级。Ninja 命令固定 `-j1`；spec 原有 compile/link pool 的 6/2 是各池上界，不会把全局 `-j1` 提升为并行 6。

上述只证明复制和环境准备完成，不等同于完整 RPM、安装测试或物理板验证通过。

## 本次自行判断与尚待确认

1. 环境准备的技术性修正均限定在私有副本：补齐同版本依赖、目标架构安装、目录所有者可写、普通用户 rpmbuild。没有修改平台源码、旧根或原资产。
2. 完整构建实际跑到 181/7546，尚无编译错误；观察到编译器通过 QEMU 执行，耗时风险明显。另一个已验证根有 515 MiB 的原生加速工具集，版本和目标只读探测通过，尚未用于此次构建。
3. 为避免等待构建路线确认期间继续消耗资源，主动以普通用户 SIGINT 保存当前断点并释放进程。**这是本次自行判断，未达到原四小时时限，也不是 I/O 超时导致停止。** 没有将这次停止误写为验证失败。
4. 尚待人工选择：是否允许复用 `tmp/GBS-ROOT/BUILD-W2C-bpftrace-aarch64-libcxx/local/BUILD-ROOTS/scratch.aarch64.0/emul`（只读挂载，不用 sudo），重新确认 CMake 后继续完整 rpmbuild；否则按原 QEMU 路线续跑。两种选择都不能把缺失的 RPM 或板上验证标成通过。没有自行延长截止时间。

当前结论为 PARTIAL；新 libclang 完整开发 RPM、bpftrace 两架构三路径 RPM 与安装测试、armv7l 物理板验证未完成，候选还不能替换发布资产。
