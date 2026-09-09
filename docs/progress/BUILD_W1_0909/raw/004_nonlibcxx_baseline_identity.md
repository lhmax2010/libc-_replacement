# 不启用 libc++ 路径的固定基线

用于“与改动前现状逐项对照”的固定输入是此前 GCC 标准仓调查使用的公开
快照：

```text
https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base/tizen-base_20260827.214809/
```

只下载了三架构各一份 `libllvm` RPM，没有镜像整个仓库。实际执行的完整
命令原文为：

```bash
mkdir -p tmp/BUILD_W1_0909/baseline_gcc_rpms; for arch in armv7l aarch64 x86_64; do url="https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base/tizen-base_20260827.214809/repos/standard/packages/$arch/libllvm-22.1.8-2.3.$arch.rpm"; nice -n 15 ionice -c 3 curl -fsSL --retry 3 --connect-timeout 30 -o "tmp/BUILD_W1_0909/baseline_gcc_rpms/libllvm-22.1.8-2.3.$arch.rpm" "$url" || exit $?; done; sha256sum tmp/BUILD_W1_0909/baseline_gcc_rpms/*.rpm
```

整体及三次 curl 退出码均为 `0`。

SHA256 与此前包清单记录完全相符：

```text
095010ef887376c28727fc23746eee1fc3ea28778c725e25729774eba1b8a094  libllvm-22.1.8-2.3.armv7l.rpm
9cad0f673aa681ea8ae191f6de0a4d0d42f9f756d7d28f4a2f96c2ad85a0ae3e  libllvm-22.1.8-2.3.aarch64.rpm
7627add20f1dd299212361aadd091cc5a7f057059f493b280ba230f4f3c29d38  libllvm-22.1.8-2.3.x86_64.rpm
```

三份基线 RPM 的依赖元数据都包含 `libstdc++.so.6` 与 `libgcc_s.so.1`，
均不包含 `libc++.so.1` 或 `libc++abi.so.1`。后续逐架构对照至少比较：

1. 实际编译器和 CMake 的 `LLVM_ENABLE_LIBCXX` 状态；
2. `libLLVM.so.22.1` 的标准库动态依赖；
3. AOT runtime 接口、`std::__cxx11` ABI 身份和原有模型资产选择；
4. RPM 子包集合及 `libllvm` 文件清单。

该固定快照属于 `Tizen-Base` GCC 标准仓，和待测的“不定义
`_toolchain`”路径同属 GCC + libstdc++ 口径。即使编译器家族相同，构建
时间、Release 和构建环境仍会使 RPM 或 ELF 字节哈希变化，因此不把整包
逐字节一致作为等价判据；对照覆盖上述接口、ABI、资产来源、动态依赖和
打包结构。

候选分支保留的原始 TensorFlow 2.15/libstdc++ 模型资产 SHA256：

```text
dd468cce897c7f578b38668e9d869c6785f3fdbcf97879fd7775b430bd0a3a9a  mlgo_arm_model.tar.gz
76f896dfdce3fa19d0ed467554ed3a643f062f52b4a4db81f576d04c469014a1  mlgo_aarch_model.tar.gz
6c221a08aa63f80c3d82713ba968125797f16351c258a174fc0eec6fdffdbf43  mlgo_x86_model.tar.gz
```

候选 spec 在 `build_with_libcxx` 未定义时仍按架构直接解开以上原文件，
没有复制、转换或重新打包。完整构建后的检查会再次从构建根证明实际选择，
此处只记录输入身份，不提前把设计当作结果。
