# 重放与后续输入

## 本轮是什么

仅 x86_64 本机、GNU provider + libstdc++ 消费方。23 条登记边的选择及原符号由 EDGES.tsv/NEXT_STAGE.tsv 固定，不从本轮能跑的函数反向挑选边。19 个独立夹具编译成功；16 个具备五轮有效控制。其余详见 FINAL_RESULT.md。原应用未启动，不等于原消费方包端到端测试。

## 文件入口

- `RESULTS.tsv`：23 条逐边版本、符号、声明、夹具、最终运行记录。
- `LIBCXX_INPUTS.tsv`：逐边确切二进制包名、开发包、辅助 C++ 包和条件性插件；架构 x86_64。`gnu_reference_versions` 是本轮对照版本，不能代称未来 libc++ 已有产物。
- `RPMS.json` / `RPM_PROVENANCE.tsv`：262 个主运行输入及固定快照 URL/SHA。补查数据四包在 `EXTRA_DATA_RPMS.json`，未混入主运行根。
- `fixtures/`：实际公开 API 消费方、身份与生命周期断言。`fixtures/attempts/` 是失效尝试的留档，不是有效夹具。
- `runs/`、`raw/`：包含失败、修正和成功的完整证据；`SELECTED_RUNS.json` 是每条最后一次采用的记录，不从同一轮中挑成功次数。
- `DECLARATIONS.md`：安装头声明位置、原文、文件 SHA 与所属开发包；内部 helper 的证据声明不表示夹具直接 include 内部头。SpinLockWait、GoogleTest helper 从公开 API 触发。

## 取得输入（只解包）

在仓库根目录执行；需 Python 3、curl、rpm 查询工具、rpm2cpio、cpio、nm，以及已确认的编译器。下述命令用于仍在本次授权截止内的重放。截止后应先取得下一任务授权，再显式设置其截止时间 `EDGE_FIXTURE_DEADLINE_UTC`；脚本默认的本次截止不可绕过。默认脚本在截止前五分钟停止新命令以便交接。

```bash
nice -n 19 ionice -c 3 python3 docs/progress/EDGE_FIXTURE_0927/code/common.py gate
nice -n 19 ionice -c 3 python3 docs/progress/EDGE_FIXTURE_0927/code/stage_rpms.py \
  --manifest docs/progress/EDGE_FIXTURE_0927/RPMS.json \
  --root tmp/EDGE_FIXTURE_0927/replay-gnu-root
```

目标必须是本任务 tmp 下**新的空目录**。校验 RPM SHA，记录包文件列表，再解包；不执行安装脚本、不改系统配置。完整输入重放没有在第二个根重复下载运行；本轮实际获取由 acquire/packages 完成，同一 stage 脚本另以一个 bundle RPM 验证过解包路径（`staging/staging-smoke.json`）。测试数据四包也由 stage 脚本实际解包。实际主运行 root 为 `tmp/EDGE_FIXTURE_0927/root`。

## GNU 五轮

```bash
nice -n 19 ionice -c 3 python3 docs/progress/EDGE_FIXTURE_0927/code/run_fixtures.py \
  --root tmp/EDGE_FIXTURE_0927/replay-gnu-root \
  --rpm-manifest docs/progress/EDGE_FIXTURE_0927/RPMS.json \
  --config docs/progress/EDGE_FIXTURE_0927/fixtures.json \
  --compiler /home/toolchain/development/libc++_replacement/progress/R33/tools/tizen-clang++ \
  --stdlib libstdc++ \
  --edges 1 2 3 4 5 6 9 13 14 15 17 19 20 21 22 23
```

`--edges` 可单选或全选；无夹具条目只记缺口，不造通过。边 8/12 会因当前图形前置失败，边 10 故意退出 77，不能用于成功演示。程序路径与 provider 路径均由参数拼入当前 root。编译器前端是 Clang 22.1.8，GNU 指 `-stdlib=libstdc++`。显式 sysroot/GNU toolchain 取当前 RPM 根，编译命令包括 C++17、O0、fno-inline、pthread；不能使用旧包装器默认根冒充当前输入。

每轮运行前执行 `nm -D --undefined-only`，精确匹配所登记 UND；缺失则该轮无效。启动 maps 必须出现 provider 规范路径，runner 启动前/退出后 SHA 一致；GNU 库必须加载且不能有 libc++。lazy PLT 的 `LD_DEBUG=bindings` 提供目标调用的附加证据。没有用 LD_PRELOAD/adaptor 模拟 provider。断言打印具体值、状态和生命周期，失败保留原输出。

## 等待 libc++ 输入后怎么切换

1. 编译线按 `LIBCXX_INPUTS.tsv` 提供 provider、同批 -devel、C++ 依赖及构建身份；准备相同格式的 RPM manifest（name、arch、url、SHA256），解到另一新的 root，不能覆盖 GNU 根。
2. 复制 `fixtures.json` 为新配置，更新 provider 的真实文件名与必要依赖；仅更换输入不修改目标公开 API/断言。头签名或数据契约发生变化时，先记不匹配，不悄悄替换登记边。
3. 用两列 TSV `edge` / `symbol` 提供从**该构建**独立确认的精确符号映射。libc++ 名称不能从 GNU 字面猜出；`--stdlib libc++` 未给映射会拒绝。
4. 设置新 `--root`、`--rpm-manifest`、`--config`、`--compiler`、`--stdlib libc++`、`--expected-symbols <映射>`。要做 GNU 消费方对 libc++ provider，也须显式传对应映射并让链接/UND 检查实际决定是否可运行；符号名不同导致链接失败是结果，不做隐式桥接。
5. 新组合先建立相应同侧控制，再解释混合结果。脚本有此参数入口，但本轮没有运行任何 libc++ 组合，未来编译/链接/运行仍可能失败。图形、信任环境、模型等缺口不会因换库自动消失。

## 资源与外部条件

所有分析、编译、运行串行，nice 19、ionice 3，common.py 按 /proc/meminfo 的实际内存设置 RLIMIT_AS 30%。本机实际为 9,921,873,100 字节。light 闸门失败会落盘 GATE_STATE 后退出，让调度等待十分钟；脚本不会自行后台重试。连续六次失败停止。Git 保存操作普通优先级。

主运行使用隔离目录的动态加载器和库搜索路径，不是 chroot。Dali 工厂动态加载的三份宿主 C 图形库在 HOST_DEPENDENCIES.json 列路径/SHA；不能由此声称平台图形环境完整。没有使用开发板、构建平台包或更改 codes。两个争用/通知夹具内部有两个线程以产生真实 API 行为；任务构建和运行调度仍严格串行。

生命周期计数只断言夹具拥有的实际对象构造/析构完成及对应共享/回调状态；不声称 provider 内部所有分配均无泄漏，不覆盖并发压力、取消、任意输入或生产发布。
