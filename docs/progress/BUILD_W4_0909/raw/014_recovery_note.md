# 凭据扫描恢复记录

首次凭据扫描以完整认证 header 名作为待查字面量，因而扫描器源码命中自己
定义的三个规则，退出码为 `1`；没有命中用户名或密码。原始结果见
`014_credential_scan.*`。

修正只把三个 header 名在扫描器源码中拆分后运行时拼接，避免规则自命中；
凭据值仍从 `.quickbuild-credentials` 读取且不输出。重跑记录见
`015_credential_scan_retry.*`。
