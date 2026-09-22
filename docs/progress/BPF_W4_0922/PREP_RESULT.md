# 提交后 ARM prep 检查

本地提交 `72fda9941031fc35d8825e73446ca43153c4b69b`，在原有 ARM 隔离根及输入基础上 **只执行 rpmbuild -bp**，退出 0；未执行 %build。

| 检查 | 结果 | 原始位置 |
|---|---|---|
| 读取提交后的 spec | SHA `ccb538d077451c8be004948115ae6a20c68bb4269459a12d4507b450253c0db2` | prep-check/result.json；spec 从包仓 packaging 只读绑定 |
| payload 预解析 | w5.xzdio；toolchain clang | prep-check/macro-eval.stdout.txt |
| %setup | 解包后 STATUS=0 | prep-check/rpmbuild-bp.stderr.txt:5–8 |
| Patch0 | 打到 src/ast/CMakeLists.txt，成功 | 同 stderr:11–12 与 stdout |
| Source1002 解包 | 两架构资产完整列出；命令通过 | 同 stderr:14–23；result.json 中文件大小和 SHA |
| sed 前计数 | **`+ test 1 = 1`** | 同 stderr:24–25 |
| sed | 执行后退出 0；新 shared-libgcc 行 1 处、原 static-libgcc 行 0 处 | 同 stderr:26–27；result.json |
| %build | 未执行 | stdout 只有 Executing(%prep)，脚本显式 -bp |

命令原文、argv、各退出码及耗时在 `prep-check/commands.json`；完整输出均保存。使用全新 `tmp/BPF_W4_0922/prep-check-verified/` 作为写目录，旧构建根与输入全部只读绑定，不复用旧 BUILD 目录。%setup 中的清理仅作用于新独立输出目录，并未清理旧失败现场。spec/CMake 的动作只发生在新解包副本。

资源：nice 19、ionice idle、单进程准备步骤；cgroup 实测 memory.max **16536457216 字节**（总内存 32297772 KiB 的 50% 向下按 4096 字节页对齐）；PID/cgroup 原文在 resources.json。首次资源自检把页对齐值与数学上的精确半值比较，差 2048 字节，误报退出 1；**尚未执行 rpm/prep**。修正检查为向下页对齐后在新独立目录执行，没有放宽上限。

本轮没有实际重跑六格构建或板测；提交信息中的成品与板测数字引用已有 BPF_STATIC_0922、BPF_W3R_0922 验证。本轮新增证据仅为文件身份、提交差异、宏条件核查和 prep。
