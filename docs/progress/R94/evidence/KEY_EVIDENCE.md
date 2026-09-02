# 关键证据索引

## 无覆盖行：实际 GCC 失败

原始文件：`../raw/011_build_armv7l_no_override.stdout`

```text
-DCMAKE_C_COMPILER=armv7l-tizen-linux-gnueabi-gcc
-- The C compiler identification is GNU 14.2.0
armv7l-tizen-linux-gnueabi-gcc: error: unrecognized command-line option '--rtlib=libgcc'
```

退出码：`../raw/011_build_armv7l_no_override.exitcode`（1）。

## 含覆盖行：仍为 GCC、同点失败

原始文件：`../raw/015_build_armv7l_with_override_retry1.stdout`

```text
-DCMAKE_C_COMPILER=armv7l-tizen-linux-gnueabi-gcc
-- The C compiler identification is GNU 14.2.0
armv7l-tizen-linux-gnueabi-gcc: error: unrecognized command-line option '--rtlib=libgcc'
```

退出码：`../raw/015_build_armv7l_with_override_retry1.exitcode`（1）。

## 直接指定 Clang 探针：完整成功

原始文件：`../raw/022_build_armv7l_direct_cc_clang_probe.stdout`

```text
-DCMAKE_C_COMPILER=armv7l-tizen-linux-gnueabi-clang
-- The C compiler identification is Clang 22.1.8
-- Build files have been written to: /home/abuild/rpmbuild/BUILD/llvm-22.1.8/build
=== Total succeeded built packages: (1) ===
```

退出码：`../raw/022_build_armv7l_direct_cc_clang_probe.exitcode`（0）。

`../raw/027_inspect_actual_clang_driver_config.stdout` 记录：

```text
libc++.so.1.0    NEEDED libgcc_s.so.1
libc++abi.so.1.0 NEEDED libgcc_s.so.1
```

## 实际目标宏

原始文件：`../raw/019_compare_environment_compiler_mechanisms.stdout`

```spec
%__cc  armv7l-tizen-linux-gnueabi-gcc
%__cpp armv7l-tizen-linux-gnueabi-gcc -E
%__cxx armv7l-tizen-linux-gnueabi-g++
```

Toolchain 项目中的覆盖消费者与标准平台项目的缺失情况，见同一原始文件
及 `../raw/004_locate_toolchain_override.stdout`。
