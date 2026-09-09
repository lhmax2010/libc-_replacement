# QuickBuild 与相关站点匿名访问验证

## 结论

在本次明确测试的六个 URL 中，QuickBuild 构建页、`gbs_reports` 目录和 `build.tizen.org` 均不能匿名读取；它们的原始响应是重定向，跟随重定向后得到的 `200 OK` 是 QuickBuild 登录页，不是构建内容。登录页是用户名/密码 HTML 表单；未显示 SSO、客户端证书或 OAuth 入口。未尝试登录，后端实际接受哪一类账号凭据为 `NOT_OBSERVED`。

Tizen 下载站的 Base Toolchain 快照根目录以及指定快照目录均可匿名读取，返回公开的 HTML 目录索引。

因此，在当前匿名访问条件下：

- QuickBuild 构建日志不能直接取得；没有执行 `lapack` 日志定位，因为其前置条件“构建页匿名可读”不成立。
- `gbs_reports` 目录结构不能列出；请求在到达目录内容前被重定向至登录页。
- 后续 W4 应维持本地复现路径。
- W3 的 QuickBuild 输入应标注“未验证匹配”。

以上结论只覆盖表中六个 URL 在本次请求时的匿名 HTTP 行为，不外推到已登录会话、API、其他端点或其他时刻。

## 资源门禁

执行 `tools/resource_gate.sh --level light`，退出码为 `0`，结果为 `gate_result=PASS`。完整命令、输出和退出码见 `raw/COMMANDS_AND_OUTPUT.md`。

## 访问结果

| # | URL | 原始响应与重定向 | 最终内容 | 匿名结论 |
|---|---|---|---|---|
| a | `https://quickbuild.tizen.org/build/1164638` | `302 Found`，1 次重定向 | `200 OK`，登录页，`text/html;charset=utf-8`，19,005 字节 | 需要登录 |
| b | `https://quickbuild.tizen.org/build/1165448` | `302 Found`，1 次重定向 | `200 OK`，登录页，`text/html;charset=utf-8`，19,005 字节 | 需要登录 |
| c | `https://quickbuild.tizen.org/build/1165447/gbs_reports` | `302 Found`，1 次重定向 | `200 OK`，登录页，`text/html;charset=utf-8`，19,077 字节 | 需要登录 |
| d | `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/` | `200 OK`，无重定向 | 公开目录索引，`text/html; charset=utf-8`，874 字节 | 可匿名读取 |
| e | `https://download.tizen.org/snapshots/TIZEN/Tizen/Tizen-Base-Toolchain/tizen-base-toolchain_20260828.101647/` | `200 OK`，无重定向 | 公开目录索引，`text/html; charset=utf-8`，635 字节 | 可匿名读取 |
| f | `https://build.tizen.org/` | `301` 后两次 `302`，共 3 次重定向 | QuickBuild 登录页，`text/html;charset=utf-8`，18,981 字节 | 需要登录 |

机器可读结果见 `ACCESS_RESULTS.tsv`。这里将“原始响应”和“跟随重定向后的最终响应”分开记录，以免把登录页的最终 `200` 误判为目标资源可读。

## 可读页面内容

### Base Toolchain 快照根目录

匿名目录索引列出：

- `latest/`
- `reference/`
- `tizen-base-toolchain_20260813.050338/`
- `tizen-base-toolchain_20260828.101647/`
- `tizen-base-toolchain_20260901.103155/`

原始响应体见 `samples/d_base_toolchain_index.html`。

### 指定快照目录

匿名目录索引列出：

- `builddata/`
- `repos/`
- `build.xml`（目录页显示 658 字节）

原始响应体见 `samples/e_verified_snapshot_index.html`。本任务没有继续进入子目录或下载构建产物，因为任务目标仅为访问验证。

## 登录页形式

构建页样本响应明确包含：

```html
<h1 class="text-2xl font-bold">Login to QuickBuild</h1>
<form id="id1" method="post" action="signin?...">
<input type="text" ... name="userName"></input>
<input type="password" ... name="password"></input>
```

因此观测到的是普通用户名/密码表单。页面中没有观测到 SSO、证书或 OAuth 入口；这不能证明服务端不存在其他登录机制。为减少无关页面内容，归档只保留相关表单摘录，见 `samples/a_quickbuild_login_form_excerpt.html`；完整响应体的原始 SHA256 和字节数保留在命令记录中。请求未携带 cookie 文件、浏览器会话或凭据，并用 `curl --disable` 禁止读取 `.curlrc`。

## 错误响应

六个 URL 均未产生终态 HTTP 4xx/5xx，因此“记录错误响应体前 200 字节”不适用。`build.tizen.org` 根页面本次没有复现背景中提到的 REST `401`：它重定向到 QuickBuild 登录页；由于本任务没有给出此前 REST 端点，未对该 `401` 作外推或复现。

## 样本与校验

| 文件 | SHA256 |
|---|---|
| `samples/a_quickbuild_login_form_excerpt.html` | `381b2af84108aec1da557aacaeb43c60c69d080d4fe876b767d067069e2fb401` |
| `samples/a_response_headers.txt` | `02c2bfb606e73ed164471bf7af9926398af216ffd424b7fc93003b415c1772f5` |
| `samples/b_response_headers.txt` | `9997bb7b60fb6471578911d6c9335dd9eabc3475df831cbd945ff086f47b093a` |
| `samples/c_response_headers.txt` | `d5be9c8948fcf61f324ca26e952adbc0f5b35391780b79f26c2d82cb0acf328d` |
| `samples/d_base_toolchain_index.html` | `390dbc48afce5b3e507dc73f27dbd0ca67d98a8dbaf75765720f257cdcfac476` |
| `samples/d_response_headers.txt` | `6d2aed87c7eadcce4c74f0bddca2a939fa8977833566f6fdcc16e3e79ba3b88c` |
| `samples/e_response_headers.txt` | `6d2aed87c7eadcce4c74f0bddca2a939fa8977833566f6fdcc16e3e79ba3b88c` |
| `samples/e_verified_snapshot_index.html` | `b297a5905433b2e700854be1bc013dd4e388855cd999c076f91c0a9bb7f53f5c` |
| `samples/f_response_headers.txt` | `426e5caaab07dd05fd0fa2283481cf44033382311e349ea2be322b9b32787bc7` |

六份响应头归档副本删除了服务器下发的 `Set-Cookie` 行，避免保存没有复核价值的匿名临时会话标识；HTTP 状态行、`Location`、`Content-Type` 及其他响应头内容保持不变。所有样本另统一将 CRLF 转为 LF 并删除行尾空白，方便纳入 Git；原始响应体及响应头在这些规范化操作之前的 SHA256 也保留在命令记录中。请求过程没有向后续请求复用这些 cookie。最终归档 SHA256 如上。

## 自行判断与尚存疑问

- 自行判断：把最终 `200` 按有效 URL 和页面内容进一步分类；登录页的 `200` 不等同于目标资源匿名可读。这是 HTTP 重定向链的直接事实判断。
- 自行判断：只保留登录表单摘录以及六份经 `Set-Cookie` 删除处理的响应头，避免归档四份近似登录 HTML、无关页面代码及匿名临时会话标识；每个 URL 的独立状态、有效 URL、大小和重定向证据仍已保留。
- 尚存疑问：QuickBuild 所需账号的来源、权限范围以及是否另有 SSO/证书入口均未观测，需要人工决定是否提供合法凭据后才能确认。
- 尚存疑问：已登录后是否能访问这三个具体构建资源，以及 `lapack` 日志和 `gbs_reports` 的具体格式，均为 `NOT_OBSERVED`。

## 操作边界确认

本任务未尝试登录，未猜测凭据，未复用 cookie，未修改构建、源码、配置或远端 Gerrit 状态；只生成了本项目内的验证材料并将其提交到项目文档分支。
