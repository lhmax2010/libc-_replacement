# 其余 10 个包的 sandbox 分支创建记录

目标分支统一为 `sandbox/lhmax2025/libcxx-migration`。本轮不包含
`tensorflow2`。

## 结果

| 包 | 基线及新分支 HEAD | 创建结果 | 原有远端 refs |
| --- | --- | --- | --- |
| `abseil-cpp` | `9c39c516dca54c786ddc5da45a5a15acf41ef75e` | 成功 | 未变 |
| `bcc-tools` | `dd26ba03fc8109396bf841f1bbf7b4df0a8ed9d2` | 成功 | 未变 |
| `boost` | `69203710b8582c489f420ef53a769cc6077c250b` | 成功 | 未变 |
| `bpftrace` | `30e51cd665360f85b665308dc13ba27df0c5f739` | 成功 | 未变 |
| `icu` | `e4a4d7411b0f288581cf020a176b5dddbb504ae7` | 成功 | 未变 |
| `jsoncpp` | `96bf0c36eed62a6d7bfc9bdb30f0e41d877aa6f0` | 成功 | 未变 |
| `libsigc++` | `5f177f7c98ea29833989b5d0d6a79fece9701545` | 成功 | 未变 |
| `llvm` | `2d23367d74afbf2bb1e9e4013fce072b3a154109` | 成功 | 未变 |
| `pcre` | `feba6a19627f5d6e74d551ef2fc8b33af0547780` | 成功 | 未变 |
| `taglib` | `4e7de646d897ed5a92111799058b21705a3bd34c` | 成功 | 未变 |

执行顺序为：先对全部 10 仓读取完整 heads，确认 `tizen_base` 等于本地 HEAD、
目标分支不存在、工作树干净；全部通过后才逐仓执行普通 `git branch` 和
普通 `git push`；最后再次读取完整 heads，逐仓移除新分支行后与推送前做
字节比较。10 仓比较全部通过。

没有使用 `--force`、`--force-with-lease`、正号 refspec 或 Gerrit
`refs/for/*`；没有推送正式分支；没有提交或修改任何 spec。

第一次预检在 `boost` 的只读 `ls-remote` 上于 60 秒超时并退出 124。由于
脚本当时尚处于“全部预检”阶段，没有执行任何建分支或推送命令。核对本地
目标分支数为 0 后，将单仓只读查询上限改为 90 秒重试；第二次全流程成功。

完整命令、stdout、stderr 和退出码见 `raw/038_*`、`raw/039_*`、
`raw/040_*`；逐仓前后 refs 见 `branch_ops/` 和 `tables/ten_*`。
