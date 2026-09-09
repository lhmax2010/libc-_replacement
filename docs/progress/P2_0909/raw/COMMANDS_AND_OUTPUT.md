# 命令与输出记录

本文件只记录不含凭据、cookie、session token 和认证请求头的命令与输出。实际用户名和密码从未打印。

## 资源门禁

```bash
tools/resource_gate.sh --level light
```

```text
level=light
nproc=20
mem_available_kib=21900892
mem_min_kib=2097152
load1=1.02
load_multiplier=1.5
load_limit=30.000000
disk_path=.
disk_available_kib=68972020
gate_result=PASS
resource_gate_exit=0
```

退出码：`0`

## 凭据元数据检查

检查仅涉及存在性、文件类型、权限和行数，不输出内容。结果：

```text
credentials_exists=yes
credentials_mode=600
credentials_type=regular file
credentials_lines=2
```

退出码：`0`

## 离线测试

```bash
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest -v tools.tests.test_quickbuild_logs
```

```text
test_credential_byte_redaction_preserves_length (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_credentials_require_exact_mode_and_two_lines (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_gbs_report_parser (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_iframe_parser (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_link_classification (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_login_form_parser (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_missing_credentials_are_actionable (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_output_directory_permissions (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok
test_url_normalization_and_host_restriction (tools.tests.test_quickbuild_logs.QuickBuildLogsTests) ... ok

----------------------------------------------------------------------
Ran 9 tests in 0.001s

OK
```

退出码：`0`

## 构建 1165448 的 GBS 内容列表

```bash
python3 -B tools/quickbuild_logs.py 1165448 --max-pages 20 list --scope gbs-reports
```

```text
kind	architecture	status	package	build_ref	label	url	source_url
# discovered=119 selected=0
```

退出码：`0`

## 构建 1165448 的 lapack 下载尝试

```bash
python3 -B tools/quickbuild_logs.py 1165448 --max-pages 20 download --package lapack --yes
```

```text
ERROR: 没有发现符合条件的可下载项；请先使用 list --scope all 检查页面结构。
kind	architecture	status	package	build_ref	label	url	source_url
```

工具退出码：`2`

## 构建 1165447 的 lapack 实际下载

```bash
python3 -B tools/quickbuild_logs.py 1165447 --max-pages 20 download --package lapack --yes
```

```text
kind	architecture	status	package	build_ref	label	url	source_url
package_log	standard-aarch64	failed	lapack	tizen_unified_toolchain_standard/lapack-3.7.1-1	lapack failed	https://quickbuild.tizen.org/download/1165447/html/GBS%20Reports@%5E@standard-aarch64/logs/fail/lapack-3.7.1-1/log.txt	https://quickbuild.tizen.org/download/1165447/html/GBS%20Reports@%5E@standard-aarch64/index.html
package_log	standard-x86_64	failed	lapack	tizen_unified_toolchain_standard/lapack-3.7.1-1	lapack failed	https://quickbuild.tizen.org/download/1165447/html/GBS%20Reports@%5E@standard-x86_64/logs/fail/lapack-3.7.1-1/log.txt	https://quickbuild.tizen.org/download/1165447/html/GBS%20Reports@%5E@standard-x86_64/index.html
OK 17489 56c71f99488dcc13702ffa12f1fa0fbf1cf0b59e51ef30fba22aa1679a0d5464 mode=fresh redactions=0 /home/toolchain/development/libc++_replacement/logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt
OK 18092 60d77a1fab08c822578a393a1565eb16c02b74bde8aa2b02ce8f097152471ec8 mode=fresh redactions=0 /home/toolchain/development/libc++_replacement/logs/quickbuild/build_1165447/0002_standard-x86_64_lapack_failed_log.txt
```

退出码：`0`

注：表格在代码中先打印并显式 flush；上方按程序逻辑顺序归档。工具开发中一次调用的标准错误先于缓冲的标准输出显示，促使增加了该 flush。

## 断点恢复测试

准备 5,000 字节 `.part`：

```bash
dd if=logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt of=logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt.part bs=1000 count=5 status=none
chmod 600 logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt.part
unlink logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt
stat -c '%a %s %n' logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt.part
```

```text
600 5000 logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt.part
```

退出码：`0`

```bash
python3 -B tools/quickbuild_logs.py 1165447 --max-pages 20 download --package lapack --resume --yes
```

关键输出：

```text
OK 17489 56c71f99488dcc13702ffa12f1fa0fbf1cf0b59e51ef30fba22aa1679a0d5464 mode=restarted_range_unsupported redactions=0 /home/toolchain/development/libc++_replacement/logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt
SKIP /home/toolchain/development/libc++_replacement/logs/quickbuild/build_1165447/0002_standard-x86_64_lapack_failed_log.txt
```

退出码：`0`

## 最终文件检查

```bash
stat -c '%a %s %n' logs/quickbuild/build_1165447 logs/quickbuild/build_1165447/*
python3 -B - <<'PY'
from tools.quickbuild_logs import QuickBuildSession, read_credentials, repo_root
root=repo_root(); user,password=read_credentials(root/'.quickbuild-credentials'); s=QuickBuildSession(user,password,30)
paths=sorted((root/'logs'/'quickbuild'/'build_1165447').iterdir())
print('downloaded_files=',len(paths))
print('files_with_credential_strings=',sum(s.file_contains_credentials(p) for p in paths if p.is_file()))
PY
```

```text
700 4096 logs/quickbuild/build_1165447
600 17489 logs/quickbuild/build_1165447/0001_standard-aarch64_lapack_failed_log.txt
600 18092 logs/quickbuild/build_1165447/0002_standard-x86_64_lapack_failed_log.txt
600 1766 logs/quickbuild/build_1165447/manifest.json
downloaded_files= 3
files_with_credential_strings= 0
```

退出码：`0`

## 工具与测试哈希、忽略规则

```bash
sha256sum tools/quickbuild_logs.py tools/tests/test_quickbuild_logs.py
git check-ignore -v .quickbuild-credentials logs/quickbuild/build_1165447/manifest.json
```

```text
f4bc663a002dd03cac057fee1eb925799cfd50f5eb0792b9bf14ed9cb22302f9  tools/quickbuild_logs.py
59601ef638b44a553d13cb646a5d3f75e63b496685da01663d86139f4bd28ef3  tools/tests/test_quickbuild_logs.py
.gitignore:9:/.quickbuild-credentials	.quickbuild-credentials
.gitignore:10:/logs/quickbuild/	logs/quickbuild/build_1165447/manifest.json
```

退出码：`0`

## 暂存内容凭据安全扫描

该脚本从凭据文件读取两条待查字节串，但只输出扫描文件数和命中数，不输出待查内容：

```bash
python3 -B docs/progress/P2_0909/code/check_staged_credentials.py
```

```text
staged_files_scanned=11
credential_or_session_hits=0
```

退出码：`0`

同时核对暂存路径，不包含 `.quickbuild-credentials` 或 `logs/quickbuild/` 下的文件。
