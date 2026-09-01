# 可交人工使用的 Gerrit 留言措辞

本文件只是草稿。R88 没有在 Gerrit 留言、投票或修改 change 状态。

## 必须确认 1：change 尚未实际合入

> 只读复核时，change 352523 的 current patch set 1 为
> `e79f1c3dde12c356142b77231e304c0da67adaf1`，但 Gerrit query 仍返回
> `status=NEW, open=true`；同时远端 `refs/heads/tizen_base` 仍指向父提交
> `8dfebafe1a477b3dcc678ee4cb18a3a4306d5a7c`。请在发布或以 tizen_base
> 触发后续构建前确认该 change 已实际 submit，并再次核对远端 ref。当前
> patch set 的 tree 已核实与 sandbox 验证基准完全相同，但当前分支尚未
> 包含它。

## 必须改 2：随包安装的 SOURCE_PROVENANCE 已过期

> `packaging/SOURCE_PROVENANCE` 与当前 standard GBS export 不一致。实测
> `e79f1c3d...` 导出 7 个源码 patch：既有 Patch0—Patch5，加上整个 squash
> 形成的 Patch6/`0007-libcxx-backport-EH-ABI-forced-unwind-handling.patch`；
> 当前文件仍记录旧 revision `3d3e3da4...`、“6 patches”，第 6 个 patch
> 的旧 commit/SHA，且未记录 forced-unwind squash。该文件会安装到
> `libc++-devel` 的 `/usr/share/doc/libcxx-runtimes/SOURCE_PROVENANCE`，
> 因而会对外发布错误来源信息。请在发布前用一个后续 packaging-only
> 提交修正并重建 `libcxx-runtimes`。不建议直接在当前 commit 中硬编码
> 当前 commit/patch SHA 后 amend，因为 amend 会再次改变这些值。

## 必须确认 3：GNU Build ID 发布政策

> 此前 QuickBuild 产物中，aarch64/armv7l/x86_64 的 `libc++.so.1.0` 和
> `libc++abi.so.1.0` 均缺 GNU Build ID，日志同时报告 `Missing build-id`。
> 当前 squash 的 spec 与该验证树逐字节相同，未包含相应修正。请在发布前
> 明确确认平台政策；如果发布门禁要求 Build ID，请补充链接/打包配置并
> 重建。如果平台允许，也请记录明确的豁免依据。

## 建议 1：changelog

> 当前 `%changelog` 最后一项是 2026-08-04 的打包迁移说明，未记录本次
> forced-unwind 改动。建议在发布前增加实际日期和变更摘要，便于包级追踪。

## 条件性建议 2：Release

> spec 当前为 `22.1.8-1`。如果同 NEVRA 已进入任何正式目标仓，请提高
> `Release`，避免同 NEVRA 对应不同内容；如果它从未正式发布，作为新包
> 首次发布可以保持 Release 1。请按目标仓历史确认。

## 正向核对结果（可一并告知）

> 内容核对通过：`e79f1c3d...` 与 sandbox 验证 HEAD `08a64b6e...` 的完整
> Git tree ID 均为 `e35b4a413b8437d05a89047e09d6bd5ff524237d`；squash
> 的 13 个文件逐字节相同，11 项关键修复全部存在。GBS 2.0.8 standard
> export 会把 squash 生成为一个新的 Patch6，不存在悬空 PatchN；该 patch
> 在父提交内容上的 `--dry-run --fuzz=0` 全部通过。
