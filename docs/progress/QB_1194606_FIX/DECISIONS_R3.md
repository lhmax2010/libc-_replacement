# R3 实施判断与技术性修正

- cereal 使用人工新授权的 28aedc95 精确基线；tizen 是空树占位，后续不再用 tizen HEAD 代表 QuickBuild 输入。
- ICU 现有六个补丁登记为 Source2–7，没有现有 PatchN。本次新增 Patch0，采用与现有补丁一致的 `%{__patch} -p1 < %{PATCH0}`，在 Source7 后执行。原候选补丁对原始 Makefile 的 apply --check 通过；打包补丁上下文保留 Source4 已加的 LINKFLAGS，仅将 tz2icu 的 -I$(srcdir) 换成 -iquote $(srcdir)，不去掉 PIE。没有直接改上游 C++ 文件。
- 四组宏比较仅忽略纯空行，严格验证差异为已批准的新增选项/补丁接入/清单块；cereal 默认 ON，其余三组 OFF。ICU 和 qemu 补丁跨工具链应用是本次已批准内容，不宣称 GCC 展开逐字节相同。
- raw/058 是 bwrap 尝试在只读根创建 /task 的技术错误；改为 tmpfs 下 /tmp/task。raw/059 记录旧根的 rpmspec 对 `%patch0` 保持字面形式，比较器 FAIL；改成现有 spec 使用的明确 patch 调用。原始记录保留，最终 raw/060 五份 spec × 四组全部 PASS。
- 首次 GBS 从项目目录启动时忽略了期望的包工作目录，报告 No source package found；尚未编译。第二次在包克隆内作为 cwd 启动，输入和配方未变。两次日志各自保留，不把第一次入口错误计为 cereal 编译失败。
- 本轮 RBS 固定 URL 的 repomd SHA256：e568cd09a390bf727d9800f80fc9757e9843fe66f45ecd2b875dbc7e51c2b754。项目配置复用 QuickBuild 日志提取副本，不修改 config/ 原文件；payload 通过命令行指定 w5.xzdio。
- MemoryMax 实际为 16536461312 字节（本机总内存 50%，按页向下取整），nice 19、ionice idle，实际 cgroup 见 RESOURCE-*_R3.json。GBS 打印的 “no limit mem size” 不代表外层 cgroup 无上限。
