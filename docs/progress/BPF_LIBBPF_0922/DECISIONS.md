# 自行判断、技术性返回与边界

- 开工固定为2026-09-22 13:04，硬截止17:04，不因准备/询问延长。资源门禁退出0；内存上限16536457216字节（总内存50%）、nice19、ionice idle、单任务串行。无遗留构建进程匹配（raw/024退出1表示无匹配）。
- 两架构均已有libbpf.a，先验证归档身份和RPM归属，不另建libbpf，不改其spec。旧输入根在隔离命令中改为只读绑定；只复制和写入本轮tmp。
- 复制前记录大小1.4GiB/1.8GiB；逐文件校验。沿用旧对象，只改变static分支一行CMake并重链接，不执行主包构建。实际复制/构建脚本快照与SHA在cells。
- 修正采用既有`${LIBBPF_LIBRARIES}`，而非另造开关；缓存已为.a。修正位置位于STATIC_LINKING和自定义LIBCLANG_STATIC_PATH条件内。现有spec只有`_toolchain`已定义且clang时才构建这个static目标；GCC与未定义分支仍取Source1002。本轮未编辑原spec，也不声称已重新执行回退路径（其已有证据在上一轮）。W4若纳入补丁，仍须放在现有libc++条件内。
- 改前链接不是把link.txt当作执行证据：在独立副本中实际调用cmake_link_script，verbose输出完整driver行并成功退出。改后为CMake --build --verbose，命令原文保留于cells及verify/link-before/after.txt。
- CMake生成的链接行除消除-lbpf外，还减少重复的libbpf.a/libbcc.a/libbcc_bpf.a项，完整token计数差异单列；不是人工额外删库，仍保留这些归档的链接项。
- 探针源码原样复用，采用同一套x86_64 libc++/libc++abi静态库与-shared-libgcc；额外用--whole-archive强制保留libbpf，防止按需链接未抽取库成员而形成空对照。使用原四种等待形态各5次。探针自身仍有libatomic/libpthread依赖，不能冒充候选bpftrace的精确DT_NEEDED契约，也不能冒充ARM板测。
- raw/052在链接尚未完成时读取输出，readelf退出1并报“Not an ELF file”；这是读到正在生成的文件，不是链接失败。等after-armv7l退出0后重新核验，raw/055及verify-armv7l记录实际结果。
- ARM最初执行脚本用了strip默认全剥离；随后读正式brp-strip确认其实际用strip -g。保留中间全剥离产物为bpftrace-static.fully-stripped，再从未剥离产物按-g生成最终候选；aarch64在启动前改用-g。没有重编代码、没有覆盖历史产物。最终SHA/大小只指-g版本。
- raw/004无AGENTS文件返回2、raw/010首次筛选无命中、diff退出1均如实留存，非构建失败。未改平台配置。
- 包装口径已异步询问：是否本轮生成static-only RPM，还是只交ELF留待W4。确认前先做独立可执行的链接/探针/身份核查；不擅自宣称旧RPM含本轮新ELF。
