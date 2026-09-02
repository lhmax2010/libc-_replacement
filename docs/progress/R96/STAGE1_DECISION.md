# R96 第一阶段：方案选择（等待人工确认）

## 结论

选择 R94 **选项 A：在 CMake 编译器参数中直接指定目标前缀 Clang**。

拟议形式为：

```spec
-DCMAKE_C_COMPILER=%{_host}-clang
-DCMAKE_CXX_COMPILER=%{_host}-clang++
-DCMAKE_ASM_COMPILER=%{_host}-clang
```

同时删除已经证实无效的：

```spec
%define _toolchain_override clang
```

本阶段只选择方案；尚未修改 `packaging/libcxx-runtimes.spec`。

## 选择理由

1. 该包的既定设计是只用 Clang 构建；直接指定 CMake driver 与设计一致，
   不把 GCC 变成另一条支持路径。
2. 它绕过了两套环境不一致的 RPM 编译器选择机制：不再读取
   `%{__cc}` / `%{__cxx}`，因此也不依赖 `_toolchain_override` 是否被
   project_config 消费。
3. 改动作用域窄：只改变本 spec 的三个 CMake compiler 参数，不重定义
   RPM 通用宏，不触碰现有 target triple、编译 flags 或链接 flags。
4. R94 已有强实测依据：armv7l Base+Unified 标准仓环境中，把最终
   `__cc/__cxx/__cpp` 直接设为相同的目标前缀 Clang 后，CMake 收到的正是
   上述三个值，完整构建成功，且两个共享库都依赖 `libgcc_s.so.1`。

## 在两种环境中的生效机制

### GCC 环境（工作区 `gbs.conf`）

该配置使用 Tizen-Base 与 Tizen-Unified 标准仓。环境自己的
`%{__cc}` / `%{__cxx}` 仍可保持目标前缀 GCC；修改后的 spec 不再引用
它们。RPM 展开 `%{_host}` 后，CMake 三个 compiler 参数直接得到目标前缀
Clang。`BuildRequires: clang = %{version}` 负责把对应 Clang 包装入构建根。

因此该格的预先判据是：**构建应成功，但实际编译器必须是 Clang，而不是
GCC**。若仍出现 GCC、Clang 不存在或构建失败，均判该格未通过，不推送。

### LLVM 环境（工作区 `gbs_llvm.conf`）

该配置使用 Tizen-Base-Toolchain 与 Tizen-Unified-Toolchain 仓。
Base-Toolchain 原本就把 `%{__cc}` 展开为 `%{_host}-clang`，但新方案不再
依赖该间接机制；CMake 同样直接取得 `%{_host}-clang` / `clang++`。

因此 GCC 与 LLVM 两种环境最终进入 CMake 的 compiler 参数相同，差异只
在项目配置与软件仓，不会再因 `_toolchain_override` 支持情况而分叉。

两份 LLVM 配置 `gbs_llvm.conf` 与 R91 使用的
`progress/T3R/config/gbs_llvm.conf` 已逐字节确认相同。四格实测仍是最终
判据；aarch64 的直接形式在 R94 中为 `NOT_OBSERVED`，不会用机制说明替代
后续实测。

## R94 实测依据

采用的证据是 R94 矩阵的：

```text
direct_cc_clang_probe
commit: dd4beffc30be18841818651756f4dbe1f54b0016
arch: armv7l
environment: Tizen-Base + Tizen-Unified standard repositories
compiler: Clang 22.1.8
result: full build succeeded, exit 0
runtime dependency: libc++.so.1.0 and libc++abi.so.1.0 both NEEDED libgcc_s.so.1
```

该探针通过 GBS 命令行定义 `__cc/__cxx/__cpp`，使现有 CMake 参数最终
展开为目标前缀 Clang。选项 A 直接写入同一组最终 CMake compiler 值，
因此已验证的是它的最终构建行为；spec 文本形式本身仍须由第二阶段四格
验证确认。

## 为什么不选其余三种

### 不选选项 B：在 spec 中重定义 `__cc/__cxx`

R94 完整成功的是 **GBS 命令行**定义同名宏，不是 spec 内 `%define` 的
形式。后者的宏优先级形式没有单独实测，而且会覆盖本 spec 中所有对 RPM
通用编译器宏的使用，作用域比直接改三个 CMake 参数更广。

也就是说，没有忽略已经验证过的做法：选项 A 复用的正是该探针最终得到
的 CMake driver 值；只是不把平台通用宏作为中转层，从而消除新的优先级
依赖。

### 不选选项 C：仅在 Clang 下添加 `--rtlib=libgcc`

该方案会让标准环境继续选择 GCC，只是为 GCC 省略选项。这与“该包只应
使用 Clang 构建”的既定判断不同，并新增 driver 识别和四处 flag 同步的
shell 逻辑。R94 只验证了版本识别，没有完整验证 GCC 省略该选项后的包
构建，关键路径仍为 `NOT_OBSERVED`。

### 不选选项 D：无条件删除 `--rtlib=libgcc`

R94 仅证明当前 armv7l Clang 的默认链接计划仍选择 `libgcc_s`；aarch64、
x86_64 和未来驱动配置没有相同实测。删除它会弱化 spec 对展开器选择的
显式约束，而直接指定 Clang 可以保留该约束。

## 第二阶段四格的预先判据

若人工确认选项 A，四格统一按以下标准判定，不能事后调整：

| 架构 | 环境 | 预期编译器 | 预期构建结果 |
| --- | --- | --- | --- |
| armv7l | GCC 标准仓 | Clang 22.1.8 | 成功 |
| armv7l | LLVM Toolchain 仓 | Clang 22.1.8 | 成功 |
| aarch64 | GCC 标准仓 | Clang 22.1.8 | 成功 |
| aarch64 | LLVM Toolchain 仓 | Clang 22.1.8 | 成功 |

每格还必须同时满足：

1. 成功产物中的 libc++ 与 libc++abi 直接依赖 `libgcc_s.so.1`；
2. 每个可用架构的 `libc++-devel` 五处头文件检查全部命中；
3. 命令、完整输出和退出码落盘；
4. 任一格不满足即停止，不提交、不推送 sandbox。

## 自行判断与疑问

自行判断：选择作用域更窄、最终 driver 行为已由 R94 armv7l 完整构建
验证的选项 A；GCC 环境两格预先申报为“强制 Clang 后必须成功”。

尚存疑问：无。第二阶段的 GCC/LLVM 配置已分别定位为工作区根目录的
`gbs.conf` 与 `gbs_llvm.conf`；后者与 R91 的配置 SHA256 相同。
