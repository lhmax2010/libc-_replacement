# Cookie 解析接口在分配失败后的资源遗留

真实 GNU 构建的 `chromium-efl` 中，cookie 解析过程中 vector 扩容分配失败并抛出 `bad_alloc` 后，已存入 vector 的名称、值及 vector 存储没有释放。固定输入下每次遗留 3 块、834 字节；同进程连续失败 10 次累计 30 块、8,340 字节，两组各 5 次一致。独立 GNU 应用直接调用原始 EWK，不经过 adaptor。源码已用 RAII；发布库相关帧没有异常清理元数据，指向 provider 的异常清理构建问题。

## 可执行复现

在材料对应工作区，保留真实 GNU EWK 及 SDK，运行：

```bash
python3 tmp/P14/w2_trace.py
python3 tmp/P14/audit_original.py
python3 tmp/P14/normal.py
```

上述脚本逐条归档完整编译/运行命令和退出码；编译命令分别见 `raw/004_build_observer.command.txt`、`raw/005_build_reproducer.command.txt`。单次运行命令见 `raw/original_4_1_1.command.txt`，它使用真实 SDK loader、明确 library path、独立复现程序和真实 provider，不依赖 adaptor 或 LD_PRELOAD。

预期：单次第 4 次分配失败输出 `caught=1 failures=1 live=3 bytes=834 unchanged=1`；连续 10 次输出 `caught=10 failures=10 live=30 bytes=8340`；正常调用在输出销毁后 `live=0 bytes=0`。每进程 maps 确认 GNU 库和 provider 存在、libc++ 不在。

## 定位和候选

`net/cookies/parsed_cookie.cc:576,585` 构造名称和值；634 行将二者移入 `pairs_`。第 4 次扩容请求 128 字节失败，留下先前的 257、513、64 字节三块。通过堆内容、指针关系与调用栈核对，不只是观察进程内存总量。

[候选 patch](provider-exception-cleanup.CANDIDATE.patch) 基于 `chromium-efl-1.1.144-1.src.rpm`，将解析与公开入口两个翻译单元的异常清理单独启用，沿用现有 RAII，不改接口或解析规则。基线及文件 SHA 见 [补丁身份](PATCH_IDENTITY.json)。

**候选尚未构建验证，不能报为修复完成。** 当前缺产品生成头、GN 参数与可用构建闭包；对真实源码的编译尝试已记录。补丁应用检查通过不等于修后泄漏归零。取得真实重建库后，使用 `verify_candidate.py` 验证分配失败归零及 12 场景不退化，各 5 次。源码仓提交号未观测，以 SRPM 身份和逐文件 SHA 定位基线。

该问题归类为 provider 外部缺陷；不据此认定 adaptor 新引入泄漏。覆盖限于 x86_64 原生固定输入与指定失败点，不是完整浏览器或并发场景的无泄漏保证。
