# 项目材料推送记录

- 推送前远端：`28235a85814a9b61b71133151fc6477a738de3d2`。
- 材料与 LINE_STATUS 同提交：`04d6626955d57b7e69245d72db4a7e793cc8a7bd`。
- 普通 push：`git push origin HEAD:refs/heads/codex/r5-ehabi-diagnosis`，退出0；随后 ls-remote 核对上述 SHA。证据 raw/116、117、118。
- 暂存区范围与凭据扫描均 PASS，见 raw/109、114；只提交本轮 docs/progress 与 LINE_STATUS，未提交原有无关工作区改动。
- 本记录随收尾证据补充提交；不修改本轮候选、原 spec 或包仓。补充提交本身的命令与最终远端 SHA 保存在本地 raw 并回传，避免声称某次提交包含其自身完成后的回执。
- 所有大 ELF/构建树留在 tmp，未进入仓库；没有清理任何原有证据，没有上板。
