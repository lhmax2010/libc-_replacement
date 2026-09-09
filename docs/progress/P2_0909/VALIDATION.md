# 在线验证记录

## 指定构建号 1165448

命令：

```bash
python3 -B tools/quickbuild_logs.py 1165448 --max-pages 20 list --scope gbs-reports
```

结果：退出码 `0`，`discovered=119 selected=0`。表格只有表头，没有 GBS 报告或逐包日志。

命令：

```bash
python3 -B tools/quickbuild_logs.py 1165448 --max-pages 20 download --package lapack --yes
```

结果：工具退出码 `2`，提示没有符合条件的可下载项。因此指定的 `lapack` 样本在该构建号下为 `NOT_AVAILABLE`。

## 实际下载样本 1165447

命令：

```bash
python3 -B tools/quickbuild_logs.py 1165447 --max-pages 20 download --package lapack --yes
```

结果：退出码 `0`，识别并下载两个失败日志：

| 架构 | 本地文件 | 字节 | SHA256 | 脱敏次数 |
|---|---|---:|---|---:|
| standard-aarch64 | `0001_standard-aarch64_lapack_failed_log.txt` | 17,489 | `56c71f99488dcc13702ffa12f1fa0fbf1cf0b59e51ef30fba22aa1679a0d5464` | 0 |
| standard-x86_64 | `0002_standard-x86_64_lapack_failed_log.txt` | 18,092 | `60d77a1fab08c822578a393a1565eb16c02b74bde8aa2b02ce8f097152471ec8` | 0 |

两者均为 ASCII 文本，能直接定位失败行。完整文件未提交。

## 已有文件跳过

同一命令再次执行，两个文件均显示 `SKIP`；工具重新计算并在 manifest 中记录既有文件的大小和 SHA256。

## `.part` 恢复

使用 aarch64 日志前 5,000 字节构造 `.part`，移除最终文件，然后执行：

```bash
python3 -B tools/quickbuild_logs.py 1165447 --max-pages 20 download --package lapack --resume --yes
```

服务器未接受 Range，工具显示：

```text
mode=restarted_range_unsupported
```

随后从头下载，最终恢复为 17,489 字节，SHA256 与原文件一致。另一个完整文件被跳过。

## 权限与凭据扫描

最终本地状态：

- `.quickbuild-credentials`：`600`；
- `logs/quickbuild/build_1165447/`：`700`；
- 两份日志和 `manifest.json`：`600`；
- 三个下载目录文件中实际凭据字符串命中数：`0`；
- `.quickbuild-credentials` 与 `logs/quickbuild/` 均由 `.gitignore` 命中。
