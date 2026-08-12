# R29 双运行时 provider 打包形态（实现前冻结）

## 不变量

1. 单一 spec 在 `%build` 内构建两遍；原有 libstdc++ 构建、原子包名和默认开发接口均保持不变。
2. libc++ 变体子包统一采用 `<原子包>-libcxx` / `<源包>-libcxx-devel` 命名；静态库另入 `<源包>-libcxx-devel-static`（原包本就拆静态子包时）。
3. 运行时共享库仍置于 `%{_libdir}`，但文件名和 `DT_SONAME` 同时加 `-libcxx`，例如 `libfoo-libcxx.so.1`。仅改路径而不改 SONAME 不满足同进程共载约束。
4. libc++ 头文件置于 `%{_includedir}/<源包>-libcxx/`；静态库重命名为 `libfoo-libcxx.a`。两套包不得拥有任何同路径普通文件。
5. libc++ 的 pkg-config 文件保留消费者使用的逻辑模块名（如 `flatbuffers.pc`），但置于 `%{_libdir}/libcxx-pkgconfig/<源包>/`；它只显式 `Provides: pkgconfig(<模块>-libcxx)`，并用 `%__provides_exclude_from` 阻止私有目录生成同名自动 Provides。中央 `PKG_CONFIG_PATH` 选择该覆盖层。
6. CMake config 置于 `%{_libdir}/libcxx-prefix/<源包>/lib/cmake/<包>/`，保留原 `find_package` 名和 target 名，内部绝对路径改指向 `-libcxx` SONAME 与私有头目录。中央 `CMAKE_PREFIX_PATH` 选择该前缀。
7. provider 的第二遍构建由 spec 显式设置 `CXX=clang++`、`-stdlib=libc++` 和 `-lc++abi`；这是产出变体本身的包内职责。消费者选择变体只允许通过中央 CXXFLAGS/LDFLAGS、`PKG_CONFIG_PATH`、`CMAKE_PREFIX_PATH`，消费者 spec 与源码在 A/B 两次构建间逐字不变。
8. 原构建先安装到 `%{buildroot}`；libc++ 构建先安装到独立 staging 根，随后仅复制、重命名所需文件。`patchelf --set-soname` 修改变体 SONAME，并对变体内部依赖执行 `--replace-needed`。

## 样本特化

| 样本 | 构建系统 | libc++ 运行时 | 私有开发接口 | 静态库 |
|---|---|---|---|---|
| pcre | autotools | `libpcrecpp-libcxx.so.0`（C 组件继续复用 `libpcre.so.1`） | `pcre-libcxx` 头目录；私有 `libpcrecpp.pc` | `libpcrecpp-libcxx.a` |
| flatbuffers | CMake | `libflatbuffers-libcxx.so.24.3.25` | 私有头、pkg-config、CMake config | `libflatbuffers-libcxx.a` |
| lottie-player | meson | `librlottie-libcxx.so.0`；image-loader 置于私有模块目录 | 私有头与 `rlottie.pc` | 不提供 |

## 红停条件

- 任一变体 ELF 仍 `DT_NEEDED libstdc++.so.6`；
- 两套共享库 SONAME 相同；
- 原 devel 与 libc++ devel 包存在同路径普通文件；
- A/B 消费者需要改 spec 或源码才能切换；
- pkg-config 或 CMake 实际解析路径未随中央注入切换。
