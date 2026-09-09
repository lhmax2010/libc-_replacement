# QuickBuild 日志拉取工具使用说明

## 前置条件

- Python 3.10 或兼容版本；只使用标准库。
- 能访问 `https://quickbuild.tizen.org`。
- 有权读取目标构建的 QuickBuild 账号。
- 在仓库根目录执行命令。

## 创建凭据文件

以下命令不会把密码字面量写入 shell 历史或进程参数：

```bash
cd /home/toolchain/development/libc++_replacement
umask 077
read -r -p 'QuickBuild username: ' QB_USER
read -r -s -p 'QuickBuild password: ' QB_PASSWORD
printf '\n'
printf '%s\n%s\n' "$QB_USER" "$QB_PASSWORD" > .quickbuild-credentials
chmod 600 .quickbuild-credentials
unset QB_USER QB_PASSWORD
```

文件必须恰好两行：第一行用户名，第二行密码。不要把实际内容贴到终端日志、报告或命令参数中。

## 只列出，不下载

列出某个包的日志：

```bash
python3 -B tools/quickbuild_logs.py 1165447 list --package lapack
```

列出全部日志候选：

```bash
python3 -B tools/quickbuild_logs.py 1165447 list --scope logs
```

列出 GBS 报告索引及其中的逐包日志：

```bash
python3 -B tools/quickbuild_logs.py 1165447 list --scope gbs-reports
```

`list` 输出为 TSV：类型、架构、状态、包名、构建引用、标签、下载 URL 和来源页 URL。

## 单包日志

先看下载计划，不写文件：

```bash
python3 -B tools/quickbuild_logs.py 1165447 download --package lapack
```

确认后下载：

```bash
python3 -B tools/quickbuild_logs.py 1165447 download --package lapack --yes
```

## 全部失败包日志

先看计划：

```bash
python3 -B tools/quickbuild_logs.py 1165447 download --failed
```

确认后下载：

```bash
python3 -B tools/quickbuild_logs.py 1165447 download --failed --yes
```

## 完整 GBS reports

这可能包含大量逐包日志，务必先看计划：

```bash
python3 -B tools/quickbuild_logs.py 1165447 download --gbs-reports
python3 -B tools/quickbuild_logs.py 1165447 download --gbs-reports --yes
```

## 全部发现到的日志

```bash
python3 -B tools/quickbuild_logs.py 1165447 download --all
python3 -B tools/quickbuild_logs.py 1165447 download --all --yes
```

## 断点与已有文件

最终文件已存在时默认跳过。只有 `.part` 文件存在时，可要求尝试续传：

```bash
python3 -B tools/quickbuild_logs.py 1165447 download --package lapack --resume --yes
```

服务器返回 `206 Partial Content` 时显示 `mode=resumed`；不支持 Range 时显示 `mode=restarted_range_unsupported` 并安全地从头下载。

## 输出位置

```text
logs/quickbuild/build_<构建号>/
├── 下载的日志
└── manifest.json
```

目录已被 `.gitignore` 排除。目录权限为 `700`，文件权限为 `600`。需要提交证据时，只摘录必要片段并注明构建号、架构、原始 URL、文件大小和 SHA256，不提交整份日志。

## 出错提示

- `凭据文件不存在`：按本说明创建 `.quickbuild-credentials`。
- `权限必须为 600`：执行 `chmod 600 .quickbuild-credentials`。
- `登录失败`：核对账号密码及访问权限；工具不会记录登录响应体。
- `没有发现符合条件的可下载项`：先运行 `list --scope all`，确认构建是否实际生成对应报告。
- `页面数量超过安全上限`：确认页面结构正常后，在子命令之前增加 `--max-pages`，例如 `... 1165447 --max-pages 400 list --scope all`。
