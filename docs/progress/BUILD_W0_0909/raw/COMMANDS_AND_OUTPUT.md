# 命令原文、输出与退出码

所有 HTTP 请求均使用 `curl --disable`，不读取 `.curlrc`；没有使用 `-b`、`-c`、认证参数或任何凭据。

## 001 资源门禁

命令原文：

```bash
tools/resource_gate.sh --level light
```

标准输出：

```text
level=light
nproc=20
mem_available_kib=22016172
mem_min_kib=2097152
load1=2.44
load_multiplier=1.5
load_limit=30.000000
disk_path=.
disk_available_kib=68949000
gate_result=PASS
```

退出码：`0`

## 010 六个 URL：任务要求格式

### a

命令原文：

```bash
curl --disable -sS -o /dev/null -w '%{http_code} %{redirect_url} %{content_type} %{size_download}\n' -L --max-time 30 'https://quickbuild.tizen.org/build/1164638'
```

标准输出：

```text
200  text/html;charset=utf-8 19005
```

退出码：`0`

### b

命令原文：

```bash
curl --disable -sS -o /dev/null -w '%{http_code} %{redirect_url} %{content_type} %{size_download}\n' -L --max-time 30 'https://quickbuild.tizen.org/build/1165448'
```

标准输出：

```text
200  text/html;charset=utf-8 19005
```

退出码：`0`

### c

命令原文：

```bash
curl --disable -sS -o /dev/null -w '%{http_code} %{redirect_url} %{content_type} %{size_download}\n' -L --max-time 30 'https://quickbuild.tizen.org/build/1165447/gbs_reports'
```

标准输出：

```text
200  text/html;charset=utf-8 19077
```

退出码：`0`

### d

命令原文：

```bash
curl --disable -sS -o /dev/null -w '%{http_code} %{redirect_url} %{content_type} %{size_download}\n' -L --max-time 30 'https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/'
```

标准输出：

```text
200  text/html; charset=utf-8 874
```

退出码：`0`

### e

命令原文：

```bash
curl --disable -sS -o /dev/null -w '%{http_code} %{redirect_url} %{content_type} %{size_download}\n' -L --max-time 30 'https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/'
```

标准输出：

```text
200  text/html; charset=utf-8 635
```

退出码：`0`

### f

命令原文：

```bash
curl --disable -sS -o /dev/null -w '%{http_code} %{redirect_url} %{content_type} %{size_download}\n' -L --max-time 30 'https://build.tizen.org/'
```

标准输出：

```text
200  text/html;charset=utf-8 18981
```

退出码：`0`

注：`-L` 后 `%{redirect_url}` 在最终响应没有下一跳时为空，因此状态码后出现两个空格。为避免把登录页 `200` 误判为目标资源可读，下面另存响应头和有效 URL。

## 020 六个 URL：保存响应头、响应体与有效 URL

临时目录由以下命令创建：

```bash
mktemp -d /tmp/build_w0_0909.XXXXXX
```

标准输出：

```text
/tmp/build_w0_0909.Xw93tz
```

退出码：`0`

### a

命令原文：

```bash
curl --disable -sS -D '/tmp/build_w0_0909.Xw93tz/a.headers' -o '/tmp/build_w0_0909.Xw93tz/a.body' -w '%{http_code} %{url_effective} %{num_redirects} %{content_type} %{size_download}\n' -L --max-time 30 'https://quickbuild.tizen.org/build/1164638'
```

标准输出：

```text
200 https://quickbuild.tizen.org/signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/build/1164638 1 text/html;charset=utf-8 19005
```

退出码：`0`

### b

命令原文：

```bash
curl --disable -sS -D '/tmp/build_w0_0909.Xw93tz/b.headers' -o '/tmp/build_w0_0909.Xw93tz/b.body' -w '%{http_code} %{url_effective} %{num_redirects} %{content_type} %{size_download}\n' -L --max-time 30 'https://quickbuild.tizen.org/build/1165448'
```

标准输出：

```text
200 https://quickbuild.tizen.org/signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/build/1165448 1 text/html;charset=utf-8 19005
```

退出码：`0`

### c

命令原文：

```bash
curl --disable -sS -D '/tmp/build_w0_0909.Xw93tz/c.headers' -o '/tmp/build_w0_0909.Xw93tz/c.body' -w '%{http_code} %{url_effective} %{num_redirects} %{content_type} %{size_download}\n' -L --max-time 30 'https://quickbuild.tizen.org/build/1165447/gbs_reports'
```

标准输出：

```text
200 https://quickbuild.tizen.org/signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/build/1165447/gbs_reports 1 text/html;charset=utf-8 19077
```

退出码：`0`

### d

命令原文：

```bash
curl --disable -sS -D '/tmp/build_w0_0909.Xw93tz/d.headers' -o '/tmp/build_w0_0909.Xw93tz/d.body' -w '%{http_code} %{url_effective} %{num_redirects} %{content_type} %{size_download}\n' -L --max-time 30 'https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/'
```

标准输出：

```text
200 https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/ 0 text/html; charset=utf-8 874
```

退出码：`0`

### e

命令原文：

```bash
curl --disable -sS -D '/tmp/build_w0_0909.Xw93tz/e.headers' -o '/tmp/build_w0_0909.Xw93tz/e.body' -w '%{http_code} %{url_effective} %{num_redirects} %{content_type} %{size_download}\n' -L --max-time 30 'https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/'
```

标准输出：

```text
200 https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/ 0 text/html; charset=utf-8 635
```

退出码：`0`

### f

命令原文：

```bash
curl --disable -sS -D '/tmp/build_w0_0909.Xw93tz/f.headers' -o '/tmp/build_w0_0909.Xw93tz/f.body' -w '%{http_code} %{url_effective} %{num_redirects} %{content_type} %{size_download}\n' -L --max-time 30 'https://build.tizen.org/'
```

标准输出：

```text
200 https://quickbuild.tizen.org/signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/dashboard 3 text/html;charset=utf-8 18981
```

退出码：`0`

## 030 重定向链与响应类型

命令原文：

```bash
for id in a b c d e f; do printf '=== %s ===\n' "$id"; rg -n -i '^(HTTP/|location:|content-type:|content-length:)' "/tmp/build_w0_0909.Xw93tz/$id.headers"; done
```

标准输出：

```text
=== a ===
1:HTTP/1.1 302 Found
11:Location: ../signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/build/1164638
14:HTTP/1.1 200 OK
17:Content-Type: text/html;charset=utf-8
=== b ===
1:HTTP/1.1 302 Found
11:Location: ../signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/build/1165448
14:HTTP/1.1 200 OK
17:Content-Type: text/html;charset=utf-8
=== c ===
1:HTTP/1.1 302 Found
11:Location: ../../signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/build/1165447/gbs_reports
14:HTTP/1.1 200 OK
17:Content-Type: text/html;charset=utf-8
=== d ===
1:HTTP/1.1 200 OK
4:Content-Type: text/html; charset=utf-8
=== e ===
1:HTTP/1.1 200 OK
4:Content-Type: text/html; charset=utf-8
=== f ===
1:HTTP/2 301
2:content-length: 0
3:location: https://quickbuild.tizen.org/
12:HTTP/1.1 302 Found
22:Location: dashboard
25:HTTP/1.1 302 Found
35:Location: signin?redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/dashboard
38:HTTP/1.1 200 OK
41:Content-Type: text/html;charset=utf-8
```

其中原始输出的 `1:HTTP/2 301` 在 `301` 后含一个行尾空格；Markdown 归档无法在不触发仓库行尾检查的情况下直观呈现该空格，因此上方去掉了该单个行尾空格，并在此明确记录。其余字符逐字保持。

退出码：`0`

## 040 登录表单检查

命令原文：

```bash
rg -n -i '<title|<form|action=|type="(text|password)|name="|sso|certificate|oauth|sign.?in|login' /tmp/build_w0_0909.Xw93tz/a.body | head -80
```

退出码：`0`

与判定有关的输出原文：

```text
6:<title>QuickBuild</title>
378:<body id="sign-in-page" data-bs-theme="light">
423:				<h1 class="text-2xl font-bold">Login to QuickBuild</h1>
430:			<form id="id1" method="post" action="signin?0-1.IFormSubmitListener-content-form&amp;redirect-url-after-sign-in=https%3A//quickbuild.tizen.org/build/1164638"><div style="width:0px;height:0px;position:absolute;left:-100px;top:-100px;overflow:hidden"><input type="hidden" name="id1_hf_0" id="id1_hf_0" /></div>
438:									<input type="text" class="form-control form-control-sm auto-focus" value="" name="userName"></input>
447:									<input type="password" class="form-control form-control-sm" autocomplete="off" value="" name="password"></input>
455:									<input id="remember" type="checkbox" class="form-check-input mr-2" name="remember"/>
468:						 Login
```

没有执行表单提交或登录尝试。

## 050 公开目录索引

命令原文：

```bash
sed -n '1,120p' /tmp/build_w0_0909.Xw93tz/d.body
sed -n '1,120p' /tmp/build_w0_0909.Xw93tz/e.body
```

标准输出：

```html
<html>
<head><title>Index of /snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/</title></head>
<body>
<h1>Index of /snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/</h1><hr><pre><a href="../">../</a>
<a href="latest/">latest/</a>                                            02-Sep-2026 00:52                   -
<a href="reference/">reference/</a>                                         29-Aug-2026 01:29                   -
<a href="tizen-base-toolchain_20260813.050338/">tizen-base-toolchain_20260813.050338/</a>              14-Aug-2026 09:04                   -
<a href="tizen-base-toolchain_20260828.101647/">tizen-base-toolchain_20260828.101647/</a>              29-Aug-2026 01:29                   -
<a href="tizen-base-toolchain_20260901.103155/">tizen-base-toolchain_20260901.103155/</a>              02-Sep-2026 00:52                   -
</pre><hr></body>
</html>
<html>
<head><title>Index of /snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/</title></head>
<body>
<h1>Index of /snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/</h1><hr><pre><a href="../">../</a>
<a href="builddata/">builddata/</a>                                         29-Aug-2026 01:29                   -
<a href="repos/">repos/</a>                                             29-Aug-2026 01:29                   -
<a href="build.xml">build.xml</a>                                          28-Aug-2026 17:05                 658
</pre><hr></body>
</html>
```

退出码：`0`

## 060 样本复制与 SHA256

命令原文：

```bash
cp /tmp/build_w0_0909.Xw93tz/a.body docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html
cp /tmp/build_w0_0909.Xw93tz/d.body docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html
cp /tmp/build_w0_0909.Xw93tz/e.body docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
cp /tmp/build_w0_0909.Xw93tz/a.headers docs/progress/BUILD_W0_0909/samples/a_response_headers.txt
cp /tmp/build_w0_0909.Xw93tz/b.headers docs/progress/BUILD_W0_0909/samples/b_response_headers.txt
cp /tmp/build_w0_0909.Xw93tz/c.headers docs/progress/BUILD_W0_0909/samples/c_response_headers.txt
cp /tmp/build_w0_0909.Xw93tz/d.headers docs/progress/BUILD_W0_0909/samples/d_response_headers.txt
cp /tmp/build_w0_0909.Xw93tz/e.headers docs/progress/BUILD_W0_0909/samples/e_response_headers.txt
cp /tmp/build_w0_0909.Xw93tz/f.headers docs/progress/BUILD_W0_0909/samples/f_response_headers.txt
sha256sum docs/progress/BUILD_W0_0909/samples/*
```

标准输出：

```text
a72985db74f44105860ce5f635ef8880945ee2526b597e92b73dec323c9a97f9  docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html
b8ef82cc6bc5e232e5dbf2cbc118fc0804e4895b38d6a2174bad6b56339cf05d  docs/progress/BUILD_W0_0909/samples/a_response_headers.txt
9028d452179f6ae3290b2d3e9e44cb45c6ddb63f460d824b015056e593afbd39  docs/progress/BUILD_W0_0909/samples/b_response_headers.txt
e2dfe575c23ff0cb004ec56e834097d862ef3dc6aa0102ea8c8761b24f4b72ae  docs/progress/BUILD_W0_0909/samples/c_response_headers.txt
6ef4cfa3874262017d1c299f88dfafe4b76b048a0ed24e979bdd67e3badfcfd4  docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html
b12efd567330e6cda469cba68191c8521080028d3d8004fdd29db085f7a18d11  docs/progress/BUILD_W0_0909/samples/d_response_headers.txt
b12efd567330e6cda469cba68191c8521080028d3d8004fdd29db085f7a18d11  docs/progress/BUILD_W0_0909/samples/e_response_headers.txt
919fc5fd9f9c0aae7eb820edc4f0657073ef6f8bd6cf5fd2696f65e5a7500d71  docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
c67867f497897e9b2e1b9a577c7f454f2b874fdc2864ae59e265863a3f437630  docs/progress/BUILD_W0_0909/samples/f_response_headers.txt
```

退出码：`0`

## 070 离线一致性自检

命令原文：

```bash
set -o pipefail
cmp -s /tmp/build_w0_0909.Xw93tz/a.body docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html
cmp_a=$?
cmp -s /tmp/build_w0_0909.Xw93tz/d.body docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html
cmp_d=$?
cmp -s /tmp/build_w0_0909.Xw93tz/e.body docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
cmp_e=$?
printf 'cmp_a=%s\ncmp_d=%s\ncmp_e=%s\n' "$cmp_a" "$cmp_d" "$cmp_e"
awk -F '\t' 'NR==1 { if (NF != 9) exit 1; next } { if (NF != 9) exit 2; n++ } END { print "data_rows=" n; if (n != 6) exit 3 }' docs/progress/BUILD_W0_0909/ACCESS_RESULTS.tsv
rg -n 'Login to QuickBuild|name="userName"|name="password"' docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html
rg -n 'tizen-base-toolchain_20260828\.101647|builddata/|repos/|build\.xml' docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
sha256sum -c <(awk -F '\`' '/^\| `samples\// {print $4 "  docs/progress/BUILD_W0_0909/" $2}' docs/progress/BUILD_W0_0909/REPORT.md)
```

标准输出：

```text
cmp_a=0
cmp_d=0
cmp_e=0
data_rows=6
423:				<h1 class="text-2xl font-bold">Login to QuickBuild</h1>
438:									<input type="text" class="form-control form-control-sm auto-focus" value="" name="userName"></input>
447:									<input type="password" class="form-control form-control-sm" autocomplete="off" value="" name="password"></input>
docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html:2:<head><title>Index of /snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/</title></head>
docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html:4:<h1>Index of /snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/</h1><hr><pre><a href="../">../</a>
docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html:5:<a href="builddata/">builddata/</a>                                         29-Aug-2026 01:29                   -
docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html:6:<a href="repos/">repos/</a>                                             29-Aug-2026 01:29                   -
docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html:7:<a href="build.xml">build.xml</a>                                          28-Aug-2026 17:05                 658
docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html:8:<a href="tizen-base-toolchain_20260828.101647/">tizen-base-toolchain_20260828.101647/</a>              29-Aug-2026 01:29                   -
docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html: OK
docs/progress/BUILD_W0_0909/samples/a_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/b_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/c_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html: OK
docs/progress/BUILD_W0_0909/samples/d_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/e_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html: OK
docs/progress/BUILD_W0_0909/samples/f_response_headers.txt: OK
```

退出码：`0`

## 080 临时目录清理

命令原文：

```bash
find /tmp/build_w0_0909.Xw93tz -maxdepth 1 -type f -delete && rmdir /tmp/build_w0_0909.Xw93tz && test ! -e /tmp/build_w0_0909.Xw93tz && printf 'temporary_directory_removed=yes\n'
```

标准输出：

```text
temporary_directory_removed=yes
```

退出码：`0`

## 090 响应头归档去除匿名会话标识

命令原文：

```bash
sed -i '/^Set-Cookie:/Id' docs/progress/BUILD_W0_0909/samples/*_response_headers.txt
if rg -n -i '^set-cookie:' docs/progress/BUILD_W0_0909/samples/*_response_headers.txt; then exit 1; else printf 'set_cookie_lines_remaining=0\n'; fi
sha256sum docs/progress/BUILD_W0_0909/samples/*
```

标准输出：

```text
set_cookie_lines_remaining=0
a72985db74f44105860ce5f635ef8880945ee2526b597e92b73dec323c9a97f9  docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html
95a9218794b4b0a66488985ffb64f11c34af3987a30df69838e6b7c9f45c76c9  docs/progress/BUILD_W0_0909/samples/a_response_headers.txt
410fa51a5b477a5c042813bb3f68c50a61b0667fe7f10a53b958c90fab6794e8  docs/progress/BUILD_W0_0909/samples/b_response_headers.txt
4fe38856cad5005b782f7561ded02ad4931cb9739ee5c51693b2674167e1c41f  docs/progress/BUILD_W0_0909/samples/c_response_headers.txt
6ef4cfa3874262017d1c299f88dfafe4b76b048a0ed24e979bdd67e3badfcfd4  docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html
b12efd567330e6cda469cba68191c8521080028d3d8004fdd29db085f7a18d11  docs/progress/BUILD_W0_0909/samples/d_response_headers.txt
b12efd567330e6cda469cba68191c8521080028d3d8004fdd29db085f7a18d11  docs/progress/BUILD_W0_0909/samples/e_response_headers.txt
919fc5fd9f9c0aae7eb820edc4f0657073ef6f8bd6cf5fd2696f65e5a7500d71  docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
86594fd377e71af0a7ceb81e829767dd98d3debe46ec75bc4ea9d1115adeb2a0  docs/progress/BUILD_W0_0909/samples/f_response_headers.txt
```

退出码：`0`

## 100 提交前自检

命令原文：

```bash
set -o pipefail
printf 'branch='; git branch --show-current
printf 'head='; git rev-parse HEAD
printf 'result_rows='; awk 'END {print NR-1}' docs/progress/BUILD_W0_0909/ACCESS_RESULTS.tsv
printf 'login_required='; awk -F '\t' 'NR>1 && $9=="LOGIN_REQUIRED" {n++} END {print n+0}' docs/progress/BUILD_W0_0909/ACCESS_RESULTS.tsv
printf 'public_readable='; awk -F '\t' 'NR>1 && $9=="PUBLIC_READABLE" {n++} END {print n+0}' docs/progress/BUILD_W0_0909/ACCESS_RESULTS.tsv
if rg -n 'JSESSIONID_|^Set-Cookie:' docs/progress/BUILD_W0_0909; then exit 2; else printf 'archived_session_identifiers=0\n'; fi
sha256sum -c <(awk -F '\`' '/^\| `samples\// {print $4 "  docs/progress/BUILD_W0_0909/" $2}' docs/progress/BUILD_W0_0909/REPORT.md)
git diff --check -- docs/progress/BUILD_W0_0909
find docs/progress/BUILD_W0_0909 -maxdepth 2 -type f -printf '%P\n' | sort
```

标准输出：

```text
branch=codex/r5-ehabi-diagnosis
head=4455af0c2ab0707eed6d6a5bf6ddfefdfffecc07
result_rows=6
login_required=4
public_readable=2
archived_session_identifiers=0
docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html: OK
docs/progress/BUILD_W0_0909/samples/a_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/b_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/c_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html: OK
docs/progress/BUILD_W0_0909/samples/d_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/e_response_headers.txt: OK
docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html: OK
docs/progress/BUILD_W0_0909/samples/f_response_headers.txt: OK
ACCESS_RESULTS.tsv
README.md
REPORT.md
raw/COMMANDS_AND_OUTPUT.md
samples/a_quickbuild_login_page.html
samples/a_response_headers.txt
samples/b_response_headers.txt
samples/c_response_headers.txt
samples/d_base_toolchain_index.html
samples/d_response_headers.txt
samples/e_response_headers.txt
samples/e_verified_snapshot_index.html
samples/f_response_headers.txt
```

退出码：`0`

## 110 文本样本行尾规范化

命令原文：

```bash
sed -i 's/\r$//; s/[[:blank:]]*$//' docs/progress/BUILD_W0_0909/samples/*
sha256sum docs/progress/BUILD_W0_0909/samples/*
```

标准输出：

```text
802fc17a2d39bcd53ffa2e359f0c0386cce6b65f2745ba6f8446c114994f99a7  docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_page.html
2809dd9031eaec240587fc8886be38ce238e4feb0bfed9fd0ff49298ff66b786  docs/progress/BUILD_W0_0909/samples/a_response_headers.txt
7c20f706fbda207b7275acc001f986934a06a7d0c92b1759f5ee396d6351c512  docs/progress/BUILD_W0_0909/samples/b_response_headers.txt
4d16cd1ea71431770f1aa4aed003657584fabd864651b96ffd246e3a52c7c99e  docs/progress/BUILD_W0_0909/samples/c_response_headers.txt
390dbc48afce5b3e507dc73f27dbd0ca67d98a8dbaf75765720f257cdcfac476  docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html
acbb37d3a90ce87f2d877dd256a8f20e3ff22386b5f67b45e01ab2b23101d78a  docs/progress/BUILD_W0_0909/samples/d_response_headers.txt
acbb37d3a90ce87f2d877dd256a8f20e3ff22386b5f67b45e01ab2b23101d78a  docs/progress/BUILD_W0_0909/samples/e_response_headers.txt
b297a5905433b2e700854be1bc013dd4e388855cd999c076f91c0a9bb7f53f5c  docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
58451a17004e31367ec8794ab0583f607705fbd0b646e650c1eb9ea1ef2e79e7  docs/progress/BUILD_W0_0909/samples/f_response_headers.txt
```

退出码：`0`

## 120 最终样本精简与校验

登录页从完整页面精简为报告中引用的表单摘录，避免归档与访问判定无关的页面代码；该文本编辑通过补丁工具完成，没有执行 shell 命令。响应头文件仅删除末尾多余空行，随后计算最终 SHA256。

命令原文：

```bash
perl -0pi -e 's/\n+\z/\n/' docs/progress/BUILD_W0_0909/samples/*_response_headers.txt
sha256sum docs/progress/BUILD_W0_0909/samples/*
```

标准输出：

```text
381b2af84108aec1da557aacaeb43c60c69d080d4fe876b767d067069e2fb401  docs/progress/BUILD_W0_0909/samples/a_quickbuild_login_form_excerpt.html
02c2bfb606e73ed164471bf7af9926398af216ffd424b7fc93003b415c1772f5  docs/progress/BUILD_W0_0909/samples/a_response_headers.txt
9997bb7b60fb6471578911d6c9335dd9eabc3475df831cbd945ff086f47b093a  docs/progress/BUILD_W0_0909/samples/b_response_headers.txt
d5be9c8948fcf61f324ca26e952adbc0f5b35391780b79f26c2d82cb0acf328d  docs/progress/BUILD_W0_0909/samples/c_response_headers.txt
390dbc48afce5b3e507dc73f27dbd0ca67d98a8dbaf75765720f257cdcfac476  docs/progress/BUILD_W0_0909/samples/d_base_toolchain_index.html
6d2aed87c7eadcce4c74f0bddca2a939fa8977833566f6fdcc16e3e79ba3b88c  docs/progress/BUILD_W0_0909/samples/d_response_headers.txt
6d2aed87c7eadcce4c74f0bddca2a939fa8977833566f6fdcc16e3e79ba3b88c  docs/progress/BUILD_W0_0909/samples/e_response_headers.txt
b297a5905433b2e700854be1bc013dd4e388855cd999c076f91c0a9bb7f53f5c  docs/progress/BUILD_W0_0909/samples/e_verified_snapshot_index.html
426e5caaab07dd05fd0fa2283481cf44033382311e349ea2be322b9b32787bc7  docs/progress/BUILD_W0_0909/samples/f_response_headers.txt
```

退出码：`0`
