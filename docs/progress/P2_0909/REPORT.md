# QuickBuild 日志拉取工具报告

## 交付结论

已实现可复用工具 `tools/quickbuild_logs.py`。工具能够：

- 从 QuickBuild HTML 登录表单动态取得提交地址和隐藏字段；
- 使用内存 cookie 会话访问构建页，不把 cookie、认证头或 session token 写盘；
- 列出构建级完整日志、步骤日志、GBS 报告索引和逐包日志；
- 按包名选择单包日志、选择全部失败包日志，或选择完整 `gbs_reports` 内容；
- 在实际下载前打印完整计划；没有 `--yes` 时不下载；
- 跳过已存在的最终文件；对 `.part` 文件尝试 HTTP Range 续传，服务器不支持 Range 时从头重下并明确显示 `mode=restarted_range_unsupported`；
- 将日志固定存入 `logs/quickbuild/build_<构建号>/`，并生成不含认证信息的 `manifest.json`。

## 凭据处理

采用任务书允许的两行格式：工作区根目录 `.quickbuild-credentials` 第一行为用户名、第二行为密码。选择保留该格式，是因为现有凭据文件已经符合，且 Python 可以直接在进程内完成表单认证，不需要生成第二份 netrc 文件。

安全约束的实现如下：

- 文件不存在、不是普通文件、是符号链接、所有者不是当前用户、权限不是严格 `600`、不是恰好两个非空行时立即退出；
- 命令行没有用户名、密码或凭据文件路径参数；QuickBuild 主机地址也固定在脚本内，避免把凭据提交给任意主机；
- 登录通过 Python 标准库发起表单 POST，凭据只在进程内存中存在，不出现在 `ps` 命令行；
- 不提交“Remember me”，cookie 只存于内存 `CookieJar`；
- 下载数据若包含与用户名或密码完全相同的字节串，会在写入过程中替换成等长星号，并在下载结果中报告脱敏次数；
- 日志目录权限为 `700`，日志与 manifest 权限为 `600`；
- `.quickbuild-credentials` 和整个 `/logs/quickbuild/` 均加入 `.gitignore`；
- 错误信息不输出登录响应体、POST 数据、cookie 或认证头。

## 在线验证

### 任务指定的构建 1165448

实际认证访问后：

- `list --scope gbs-reports` 返回 `selected=0`；
- `download --package lapack --yes` 以退出码 `2` 明确报告没有符合条件的可下载项；
- `gbs_reports` 页面没有报告 iframe，也没有架构报告链接。

因此，构建 `1165448` 下的 `lapack` 包日志为 `NOT_AVAILABLE`，无法声称已从该构建号拉到该日志。没有用其他构建号冒充此项。

### 可用样本 1165447

相邻且在 W0 中明确给出的 `build/1165447/gbs_reports` 实际含有 `lapack` 日志，因此只将它作为工具功能验证样本：

| 架构 | 大小 | 格式 | SHA256 | 能否看到失败原因 |
|---|---:|---|---|---|
| `standard-aarch64` | 17,489 字节，273 行 | ASCII text | `56c71f99488dcc13702ffa12f1fa0fbf1cf0b59e51ef30fba22aa1679a0d5464` | 能 |
| `standard-x86_64` | 18,092 字节，269 行 | ASCII text | `60d77a1fab08c822578a393a1565eb16c02b74bde8aa2b02ce8f097152471ec8` | 能 |

两份日志均由最终版本工具实际下载，下载时凭据字符串脱敏次数均为 `0`。关键片段见 `samples/lapack_failure_excerpt.txt`，完整日志只保存在被忽略的 `logs/quickbuild/`，未提交。

## 跳过与恢复验证

- 再次执行同一下载命令时，两份最终文件均显示 `SKIP`，哈希不变。
- 将 aarch64 文件的前 5,000 字节作为 `.part` 后执行 `--resume`：服务器没有接受 Range，工具明确显示 `mode=restarted_range_unsupported`，自动从头重新下载；最终大小和 SHA256 与原文件一致。
- 此行为满足“能续传则续传；服务器不支持时安全重下”，不会把部分文件误认为完成文件。

## 自动测试

9 项离线测试全部通过，覆盖：

- 登录表单及 iframe 解析；
- GBS 包名、状态和日志链接解析；
- URL 编码和 QuickBuild 主机限制；
- 构建日志链接分类；
- 凭据文件缺失、权限和两行格式检查；
- 凭据字节串等长脱敏；
- 输出目录权限。

工具 SHA256：`f4bc663a002dd03cac057fee1eb925799cfd50f5eb0792b9bf14ed9cb22302f9`。

## `.gitignore` 改动

新增：

```gitignore
/.quickbuild-credentials
/logs/quickbuild/
```

工作区原有的 `.gitignore` 未提交改动 `/config/` 不属于本任务；提交时将只暂存上述两行，保留该既有改动在工作区。

## 凭据安全自检

已使用随交付材料保存的 `code/check_staged_credentials.py` 对本次 11 个暂存文件逐一检查。脚本从 `.quickbuild-credentials` 读取实际用户名与密码，仅在内存中比较，同时检查认证头、cookie 与 session 标识形态；不会打印待查字符串。结果为 `credential_or_session_hits=0`，退出码 `0`。

暂存路径也已单独核对：不包含 `.quickbuild-credentials`、`logs/quickbuild/` 或任何本任务范围外的已有修改。任一安全扫描命中都会返回退出码 `2`，阻止后续提交。

## 自行判断与尚存疑问

- 自行判断：使用 Python 标准库表单认证和内存 cookie，而不是把现有两行凭据转换为 netrc；这样不产生凭据副本，也不让凭据进入进程参数。
- 自行判断：为保证凭据永不随下载内容落盘，对下载体中与凭据完全相同的字节串做等长星号替换；manifest 记录替换次数。此安全处理优先于日志逐字节原样性。
- 自行判断：对 `1165448` 严格记为 `NOT_AVAILABLE`，仅以 `1165447` 完成工具功能验证，不把两者混为一批。
- 尚存疑问：任务书为何把 `lapack` 日志归于 `1165448`；本次实际页面与报告索引均不支持这一对应关系，需要人工确认是否写错构建号。
- 已知限制：工具依赖当前 QuickBuild/Wicket HTML 结构；若登录表单、GBS 报告表格或链接结构变化，会以可操作错误停止。JavaScript 动态生成且 HTML 中完全没有的链接无法发现。
