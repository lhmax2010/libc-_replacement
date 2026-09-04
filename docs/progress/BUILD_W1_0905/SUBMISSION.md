# 项目仓提交记录

## 主材料提交

- 分支：`codex/r5-ehabi-diagnosis`
- 提交：`dd36a692eeebab051565821e343fab75777bc896`
- 提交说明：`Document MLGO AOT generation prerequisites`
- 推送：成功，见 `raw/105_push_main.*`
- 推送后核对：本地与远端均为 `dd36a692eeebab051565821e343fab75777bc896`，见 `raw/106_verify_main_remote.*`

## 提交命令记录说明

实际提交命令为：

```bash
git commit -m 'Document MLGO AOT generation prerequisites' -m '记录 MLGO/XLA AOT 生成指南、输入身份及材料不一致导致的第一阶段阻断；尚未生成或修改资产。'
```

退出码为 `0`。该命令误未通过日志包装器执行，终端回显又被调用层截断；因此这里是根据 shell 历史动作和 `git show -s --format=fuller` 作出的**恢复记录**，不冒充包装器自动捕获的原始输出。提交对象本身是结果的可复核证据。

推送和远端核对均由日志包装器执行，命令、stdout、stderr、退出码完整保存在相应的 105、106 记录中。
