# R84：可直接使用的 project_config 补丁

## 1. 结论

已生成两份独立补丁，并在仅包含实际文件副本的 `tmp/R84/apply-test/` 中完成
严格应用、与 R83 建议逐字节比对及反向恢复。最终应用和反向操作均使用
`--fuzz=0`，退出码均为 0；反向后 SHA256 与原件完全相同。

`codes/` 下两份原始 `project_config` 没有修改；`git diff --exit-code --
codes/...` 退出 0。未执行 QuickBuild 或任何全量编译，未向 Gerrit 或其他
外部仓推送。

产物：

| 产物 | SHA256 |
|---|---|
| `docs/progress/R84/base_project_config.patch` | `9b2eedde28547ba0702e39c6a4cd29637dbcd8cebe096a53ce1f2ce504700c20` |
| `docs/progress/R84/unified_project_config.patch` | `22ed0a3f6b75bff9b9bd3924e81c8eecc0434c4ff2e3495c3e632484de374547` |

两份补丁的 `---`/`+++` 路径分别就是：

- `codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config`
- `codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config`

因此应在项目仓根目录 `/home/toolchain/development/libc++_replacement` 下用
`-p0` 执行。

## 2. 人工直接执行命令

先核对未修改原件的已验证身份，再依次应用：

```bash
cd /home/toolchain/development/libc++_replacement
sha256sum \
  codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config \
  codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
patch --batch --fuzz=0 -p0 < docs/progress/R84/base_project_config.patch
patch --batch --fuzz=0 -p0 < docs/progress/R84/unified_project_config.patch
sha256sum \
  codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config \
  codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
```

输入 SHA256 应为：

```text
aa7e0ad85c9decbb5b53f822b2414c1ed4569a2b2b39930c08c3f69e011fa209  codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086  codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
```

应用后的 SHA256 应为：

```text
353df9624c94588005ab5340220a02d58b37ab9d0f0810bdf8ef69dfa078b410  codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
06d975ac3e3400f14102b0402db6610044f5296fcc73000e5a4cae873b5739bb  codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
```

若输入 SHA256 不同，应停止，不要强行应用。

## 3. 实际应用验证

实际验证目录是：

```text
/home/toolchain/development/libc++_replacement/tmp/R84/apply-test
```

最终两条打补丁命令及完整输出如下；命令均退出 0：

```bash
cd tmp/R84/apply-test && patch --batch --fuzz=0 -p0 < ../../../docs/progress/R84/base_project_config.patch
```

```text
patching file codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
```

```bash
cd tmp/R84/apply-test && patch --batch --fuzz=0 -p0 < ../../../docs/progress/R84/unified_project_config.patch
```

```text
patching file codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
```

验证方法不是只搜索新增行：另建 `tmp/R84/r83-expected/`，把原件分别复制为
R83 旧合并 diff 所用的文件名，实际应用
`docs/progress/R83/artifacts/PROJECT_CONFIG_CHANGES.diff`，再与本任务应用结果
分别执行 `cmp -s`。结果为：

```text
base_cmp_r83_rc=0
unified_cmp_r83_rc=0
```

这证明两份新补丁应用后的完整文件与 R83 建议逐字节一致。应用后完整 diff、
命令、stdout、stderr 和退出码见 `raw/022_*` 至 `raw/025_*`。

验证过程中，三版手工组织的 Base hunk 在严格测试中未匹配；它们只作用于
临时副本，失败输出保留在 `raw/005_*`、`raw/010_*`、`raw/014_*`。最终版本
改用机械 `diff -u` 给出的 hunk 边界，并从原始 SHA256 副本重新开始，最终
验证见 `raw/022_apply_base_verified.*`。这属于产物生成阶段的技术性修正，
没有修改实际 `codes/` 文件，也没有放宽 fuzz。

## 4. 撤销及恢复验证

人工撤销时在同一仓根目录执行，顺序与应用相反：

```bash
cd /home/toolchain/development/libc++_replacement
patch --batch --fuzz=0 -R -p0 < docs/progress/R84/unified_project_config.patch
patch --batch --fuzz=0 -R -p0 < docs/progress/R84/base_project_config.patch
sha256sum \
  codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config \
  codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
```

实际反向验证的命令和完整输出如下，两条均退出 0：

```bash
cd tmp/R84/apply-test && patch --batch --fuzz=0 -R -p0 < ../../../docs/progress/R84/unified_project_config.patch
```

```text
patching file codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
```

```bash
cd tmp/R84/apply-test && patch --batch --fuzz=0 -R -p0 < ../../../docs/progress/R84/base_project_config.patch
```

```text
patching file codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
```

恢复后结果：

```text
aa7e0ad85c9decbb5b53f822b2414c1ed4569a2b2b39930c08c3f69e011fa209  tmp/R84/apply-test/codes/qb/TIZEN/Tizen/Tizen-Base-Toolchain/project_config
a5abe9c7a6dcf2909799e6bbc6cca6349c2fe94d9cf74132db74eebc30f34086  tmp/R84/apply-test/codes/qb/TIZEN/Tizen/Tizen-Unified-Toolchain/project_config
base_restore_cmp_rc=0
unified_restore_cmp_rc=0
```

既校验了 SHA256，也对恢复文件与保存的原始副本执行了逐字节比较。

## 5. QuickBuild 构建顺序

### 5.1 是否会由 Support 自动排序

**不能确认 QuickBuild 服务端会自动排序，且本机证据表明不能把 `Support:`
当作可靠的调度依赖。** 平台调度器版本、任务图、buildroot 求解日志当前均
`NOT_OBSERVED`。

本机 `/usr/lib/build/Build.pm` 的 `get_build` 把 `support` 加进实际 buildroot
包集合；但 `get_deps` 明确是“影响其他包构建”的集合，并在第 641–647 行
过滤 `support`。所以本机语义是“安装到 buildroot”，不是“建立 source
package 的构建先后边”。`gbs 2.0.8 qbbuild --help` 也没有承诺这种调度行为。

### 5.2 人工应如何做

在平台侧给出相反的任务图证据前，建议两阶段：

1. 使用未加入本次 `Support/Keep` 的 bootstrap 配置，先构建 sandbox
   LLVM/Clang，再构建 `libcxx-runtimes.spec`，发布四个 provider 二进制包；
2. 确认第二阶段仓库/snapshot 已能解析准确的新 NEVRA 后，应用本任务两份
   project_config 补丁，再构建其余全部包。

尤其不要直接用已打 Base 补丁的配置自举 `libcxx-runtimes`：它同时在
`Support:` 要求两个 devel 包、在 `Keep:` 保留四个 libc++/libc++abi 包；
按本机 build core 逻辑，provider 构建存在依赖自身尚未生成子包的风险。

若 QuickBuild 支持显式 staged job，一次平台提交也可以包含上述两个逻辑
阶段，但必须证明 provider 已成功发布后消费者才启动；不能只凭 `Support:`
推断。完整检查点见 `QUICKBUILD_ORDERING.md`。

### 5.3 平台检查点

人工需保存并确认：第二份 spec 被识别为独立 source package；任务图含
`llvm -> libcxx-runtimes -> consumers`；provider RPM 已发布；消费者
buildroot 日志显示新 devel 包的准确 NEVRA/来源；没有 `nothing provides`、
`unresolvable` 或旧包选择；消费者头文件含层 C 防护。

## 6. 自行判断、疑问与边界

1. 自行判断：补丁以仓根相对路径生成，统一使用 `-p0`；这直接对应任务给出的
   实际文件位置，避免自定文件名。
2. 自行判断：验证强制 `--fuzz=0`，并用 R83 旧 diff 的完整结果做 `cmp`，
   以免“能打上”掩盖偏移或内容差异。
3. 自行判断：根据本机 build core 的 `get_build/get_deps` 语义，推荐两阶段
   构建，并让 provider 阶段使用 bootstrap 配置。
4. 尚存疑问：QuickBuild 服务端是否采用相同 OBS build core、是否支持 LLVM
   仓中第二份独立 spec、是否有隐藏的 staged/bootstrap 调度机制，均
   `NOT_OBSERVED`；需按平台检查点人工确认。
5. 任务边界：没有修改 `codes/` 原件，没有执行全量编译或 QuickBuild，
   没有推送 Gerrit；R84 任务未要求提交本项目 Git，因此本轮也未 commit/push。

## 7. 原始记录

所有执行命令逐字保存在 `raw/*.command.txt`，stdout、stderr、退出码分别保存为
同前缀的 `.stdout`、`.stderr`、`.exitcode`。关键记录：

- `001`–`004`：输入身份、R83 建议与临时副本；
- `005`–`021`：草稿 hunk 失败、诊断和机械 diff；
- `022`–`025`：最终应用及与 R83 的逐字节验证；
- `026`–`028`：反向恢复和 SHA256/cmp；
- `030`–`035`：本地 QuickBuild/OBS build 语义与 provider 证据。
