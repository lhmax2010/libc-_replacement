#!/usr/bin/env python3
import csv
import hashlib
import os
import shutil
from collections import Counter
from datetime import datetime
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
P = ROOT / "progress/R22"
T = P / "tables"
B = P / "board"
PD = ROOT / "docs/patches"

def rows(path):
    with (ROOT / path).open(encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def write_tsv(path, fields, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        w.writeheader(); w.writerows(data)

def sha(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            h.update(block)
    return h.hexdigest()

# Existing local download files: filesystem mtimes are explicitly only a proxy,
# because R11 did not record per-download wall-clock timestamps.
time_rows = []
for kind, rel in [("binary", "progress/R11/tables/binary_download_status.tsv"), ("source", "progress/R11/tables/source_download_status.tsv")]:
    ts = []
    missing = 0
    for r in rows(rel):
        path = Path(r["target"])
        if not path.is_absolute(): path = ROOT / path
        if path.exists(): ts.append(path.stat().st_mtime)
        else: missing += 1
    time_rows.append({
        "object_kind": kind,
        "local_objects_with_mtime": len(ts),
        "local_objects_missing": missing,
        "earliest_local_mtime_utc8": datetime.fromtimestamp(min(ts)).astimezone().isoformat() if ts else "NOT_AVAILABLE",
        "latest_local_mtime_utc8": datetime.fromtimestamp(max(ts)).astimezone().isoformat() if ts else "NOT_AVAILABLE",
        "status": "FILESYSTEM_MTIME_PROXY_NOT_DOWNLOAD_LOG_TIME",
    })
write_tsv(T / "download_time_ranges.tsv", list(time_rows[0]), time_rows)

source_rows = [
    {"measurement": "R22 ABI graph and staging", "repository": "Base+Unified binary and source reference/standard", "revision_and_repomd_sha256": "base_binary 1785866115 / 1789528921ae67d70ba657c787899ec580ab0b17a8368a0245ec09440710cf16; unified_binary 1785904307 / e425b6a91a3cfb2f67cbd73160ac48e2bb776e92866de81d5b97bb25e5ab39c3; base_source 1785866095 / cf7fca47147304ec114e8e0c19f6f43353dbf973ee623d3eb5188919c509fe72; unified_source 1785904240 / f15c6854277fb975d6d5e52d9a33c120eb85bb5cdc726acdf4243026098fde59", "object_form": "发布形态：仓内 RPM 解包所得 ELF；不是板上已安装对象", "download_time": "见 download_time_ranges.tsv；仅本地 mtime 代理"},
    {"measurement": "R1 source/build classification annotations", "repository": "T1 Base+Unified reference/standard", "revision_and_repomd_sha256": "逐项见 progress/T1/repo_identity.tsv 与 source_repo_identity.tsv；该快照早于 R11", "object_form": "发布形态：T1 SRPM 语料，精确 source RPM 身份匹配时才复用", "download_time": "T1 state: 2026-07-31T19:14:49+08:00–2026-07-31T20:37:14+08:00"},
    {"measurement": "R22 board path probe", "repository": "NOT_APPLICABLE", "revision_and_repomd_sha256": "Tizen 11.0 board 192.168.108.25:26101", "object_form": "运行形态：板上挂载与执行策略；不参与迁移清单计数", "download_time": "NOT_APPLICABLE"},
]
write_tsv(T / "data_sources.tsv", list(source_rows[0]), source_rows)

edge_summary = rows("progress/R22/tables/edge_attribution_summary.tsv")
summary = {(r["scope"], r["classification"]): r for r in edge_summary}
stage = rows("progress/R22/tables/stage_summary.tsv")
checks = rows("progress/R22/tables/cross_checks.tsv")
unknown = rows("progress/R22/tables/unknown_attribution_summary.tsv")

report = f"""# R22 第一部分：分阶段迁移清单修正

## 输入、对象与口径

本轮没有重新扫描平台仓。输入文件及 SHA256 在 `inputs/input_identity.tsv`；仓快照、对象形态与时间口径在 `tables/data_sources.tsv` 和 `tables/download_time_ranges.tsv`。阶段清单单位是精确 NEVRA 形态的源码 RPM 身份：适配集合 483/1,450 个仓内源码 RPM 记录；对应二进制 Name 数按源码身份内去重后合计 1,768。图和阶段分析对象均为 R11 下载的发布形态 RPM，不是开发板已安装的运行形态。

R11 运行时图分母为 36,829 个 ET_EXEC/ET_DYN ELF、11,332 个二进制 RPM 记录。151,403 条 DT_NEEDED 边中 138,236 条唯一解析（91.30%），13,167 条不可唯一解析（8.70%）；不可唯一解析边没有推断补全。R22 针对 1,705 条源码包候选边涉及的 9,234 个本地缓存 ELF 执行定向动态符号提取，9,234/9,234 成功；这不是平台仓重扫。

## 逆向边归因

对每条 A→B 候选边，以具体消费者 ELF 的 UND 动态符号与其 DT_NEEDED 实际解析到的 provider ELF 的 DEF 动态符号求交：交集中至少一个符号以 `_Z` 开头，才记真正 C++ ABI 耦合；交集全为非 `_Z` 则记纯 C 接口；无交集单列，不冒充 C++ ABI 边。逐 ELF/DT_NEEDED/符号证据见大表，逐源码边汇总见 `tables/source_edge_classification.tsv`。

- 全部 1,705 条候选源码边：1,234 条真 C++ ABI 耦合，445 条纯 C 接口，26 条没有符号交集，NOT_AVAILABLE 0。
- R20 的 462 条逆向边：452 条真 C++ ABI 耦合，8 条纯 C 接口误判，2 条没有符号交集，NOT_AVAILABLE 0。完整逐条归因见 `tables/reverse_edge_attribution.tsv`。

## UNKNOWN 归因

R20 的 121/121 个 UNKNOWN 均不是“依赖不可解析、无 ELF或分类文件缺失”，而是同一旧规则缺口：单节点、非插件包只要存在第一阶段之外的 ABI 依赖，就既不满足旧第二阶段条款，也未命中旧第三阶段条款。逐包原理由 `unknown_121_attribution.tsv` 保留；R22 在 SCC 凝聚图上按 provider-first 深度机械分层后，121/121 均获得阶段，保留 UNKNOWN 归属为 0。源码/构建修改需求仍可为 UNKNOWN，未被阶段归属替代。

## 修正规则与结果

1. 从候选图剔除 445 条纯 C 接口边和 26 条无动态符号交集边，仅保留 1,234 条实证 C++ ABI 耦合边。
2. 厂商预编译对象仍以 5 个二进制 Name / 4 个源码 RPM 身份单列；没有把它们塞进迁移阶段。
3. 对其余 479 个源码身份重算 SCC。实测所有 SCC 均为单节点，因此 `coupling_components.tsv` 只有表头。
4. 第一阶段以首批五包和测试框架 gtest 为硬种子，并闭包纳入它们传递依赖的所有 provider SCC，共 13 个源码 RPM。
5. 剩余 SCC 以 A→B 表示消费者依赖 provider，从 provider 端计算拓扑深度；实测最大深度 5。深度 0–2 为第二阶段（450 个），深度 3–5 为第三阶段（16 个）。该规则确保任何较早阶段都不依赖较晚阶段。
6. 厂商单列 4 个；三阶段加单列为 483/483。

汇总见 `tables/stage_summary.tsv`，全清单见 `tables/migration_packages_full.tsv`。修正后真 C++ ABI 逆向边为 0/1,234；`tables/corrected_inverse_edges.tsv` 只有表头。多包 SCC 为 0；插件宿主、测试框架、厂商对象和 Chromium 在 `tables/special_cases.tsv` 单列。

## 交叉校验

`tables/cross_checks.tsv` 的 13 项均 PASS：总数 483/483；首批五包全在第一阶段；`libcynara-commons` 与 `security-manager` 均为源码修改 YES；gtest 在第一阶段；阶段间逆向 ABI 边 0；Chromium 正常归入第二阶段且源码/构建修改需求保持 UNKNOWN；旧 121 个阶段 UNKNOWN 全部闭合。

Chromium 使用 R19 实测事实：42/42 个运行时 ELF 动态依赖 libstdc++，libc++ 依赖 0，`std::__1` 命中 0。它没有因 T1 源码解包缺口被排除；只把源码修改需求和构建系统修改需求标 UNKNOWN。

## 483 与 372 的口径差异

372 是 T1 快照中“二进制 RPM 元数据直接依赖 libstdc++”映射并去重得到的源码包集合；371/372 成功解包扫描，Chromium 1 包失败。483 是较后 R11 快照中“至少一个运行时 ELF 含 Itanium 修饰符号或 C++ 运行时 DT_NEEDED”的源码 RPM 身份集合。前者是直接 libstdc++ 依赖驱动的源码语料分母，后者还纳入静态含 C++ 特征、间接形态以及精确 release 身份，因此两数不能互换。本迁移清单以运行时形态定义的 483 为分母；R1 修改标注只在精确源码 RPM 身份匹配时复用，否则保持 UNKNOWN。

本报告不含排期、人日、优先级建议或风险评级。
"""
(P / "R22_migration_report.md").write_text(report, encoding="utf-8")

# Board facts. First-pass SDB carrier results are intentionally retained but invalid.
probe = rows("progress/R22/board/path_probe_results_v2.tsv")
mounts = {
    "/root": ("/", "ext4", "rw,relatime,mb_optimize_scan=0", 1781416),
    "/opt/usr": ("/opt/usr", "f2fs", "rw,lazytime,relatime,background_gc=on,nogc_merge,discard,discard_unit=block,user_xattr,inline_xattr,acl,inline_data,inline_dentry,flush_merge,barrier,extent_cache,mode=adaptive,active_logs=6,alloc_mode=default,checkpoint_merge,fsync_mode=posix,memory=normal,errors=continue", 113541492),
    "/opt/usr/home/owner/share/tmp": ("NOT_FOUND", "NOT_AVAILABLE", "NOT_AVAILABLE", "NOT_AVAILABLE"),
    "/opt/home": ("/opt", "ext4", "rw,relatime", 1263900),
    "/home/owner": ("/opt/usr", "f2fs", "rw,lazytime,relatime,background_gc=on,nogc_merge,discard,discard_unit=block,user_xattr,inline_xattr,acl,inline_data,inline_dentry,flush_merge,barrier,extent_cache,mode=adaptive,active_logs=6,alloc_mode=default,checkpoint_merge,fsync_mode=posix,memory=normal,errors=continue", 113541492),
    "/var/tmp": ("/opt", "ext4", "rw,relatime", 1263900),
    "/opt/var/tmp": ("/opt", "ext4", "rw,relatime", 1263900),
}
ranked = []
for r in probe:
    mp, fs, opts, free = mounts[r["candidate"]]
    ranked.append({**r, "resolved_mountpoint": mp, "filesystem": fs, "mount_options": opts, "noexec_observed": "NOT_AVAILABLE" if mp == "NOT_FOUND" else ("YES" if "noexec" in opts else "NO"), "available_kib": free})
ranked.sort(key=lambda r: (r["status"] != "WRITABLE_AND_EXECUTABLE_CONFIRMED", -(int(r["available_kib"]) if isinstance(r["available_kib"], int) else -1), r["candidate"]))
for i, r in enumerate(ranked, 1): r["availability_order"] = i
fields = ["availability_order", "candidate", "status", "resolved_mountpoint", "filesystem", "mount_options", "noexec_observed", "available_kib", "mkdir_carrier_exit_code", "mkdir_remote_exit_code", "push_exit_code", "execute_exit_code", "cleanup_carrier_exit_code", "cleanup_remote_exit_code", "deletion_verify_carrier_exit_code", "deletion_verify_remote_exit_code"]
write_tsv(B / "board_path_ranked.tsv", fields, ranked)
security = [
    {"item": "SDB identity", "observed": "192.168.108.25:26101 device rpi4; Linux 6.12.80-arm-rpi4-v7l; armv7l; uid=0; context=User::Shell", "evidence": "commands/012_board_connect_identity.log"},
    {"item": "Smack", "observed": "smackfs mounted rw,nosuid,nodev,noexec; process label User::Shell; ambient System::Privileged", "evidence": "commands/016_board_security_mount_space.log"},
    {"item": "securityfs", "observed": "/sys/kernel/security NOT_FOUND; no securityfs mount output", "evidence": "commands/016_board_security_mount_space.log"},
    {"item": "label tooling", "observed": "/usr/bin/chsmack and /usr/bin/smackload present; chsmack help exposes label-setting operations; authorized policy workflow NOT_AVAILABLE", "evidence": "commands/016_board_security_mount_space.log"},
    {"item": "execution denials", "observed": "0/6 existing candidates denied; therefore no noexec-vs-Smack denial case was observed", "evidence": "board/path_probe_results_v2.tsv"},
    {"item": "invalid first pass", "observed": "INVALID_CARRIER_RC: sdb shell did not propagate remote rc; not used for conclusions", "evidence": "board/path_probe_results.tsv; commands/013_board_path_probes.log"},
]
write_tsv(B / "security_policy_facts.tsv", list(security[0]), security)

board_report = """# R22 第二部分：板上可执行路径探测

## 载体与探针

SDB `192.168.108.25:26101` 实测可达，设备为 rpi4，`uname -m` 为 armv7l。探针是 556 字节、ELF32 ARM EABI5、静态链接、入口 `_start` 直接以 syscall 退出 0 的最小文件；源码、完整 LLVM 22.1.8 汇编/链接命令、ELF 头和 SHA256 分别见 `board/exec_probe_armv7l.S` 与 `commands/011_compile_exec_probe_direct_llvm.log`。这不是平台包构建。

首次循环发现 `sdb shell` 的宿主退出码不会传递远端命令退出码，故 `board/path_probe_results.tsv` 全部标为 INVALID_CARRIER_RC，不用于结论。该首轮还因载体误判让 `sdb push` 自动创建了原本不存在的 `/opt/usr/home/owner/share/tmp`；已以精确路径 `rmdir` 删除并确认不存在，原文见 `commands/014_cleanup_sdb_created_candidate.log`。第二轮在每条远端命令输出显式 `__R22_REMOTE_RC` 后重测。

## 实测结果

6/7 个候选路径原本存在，且 6/6 都完成临时目录创建、探针写入、实际执行退出 0、删除退出 0、删除后不存在核验退出 0：`/opt/usr`、`/home/owner`、`/opt/home`、`/var/tmp`、`/opt/var/tmp`、`/root`。按可用空间降序的机械排列及每步 carrier/remote 退出码见 `board/board_path_ranked.tsv`。`/opt/usr/home/owner/share/tmp` 原本不存在，第二轮没有创建，状态 NOT_FOUND。

`/tmp` 挂载为 `rw,nosuid,nodev,noexec`；上述 6 个成功路径实际归属 `/`、`/opt` 或 `/opt/usr`，对应挂载项均未见 noexec。`/home/owner` 实际解析为 `/opt/usr/home/owner`，`/var/tmp` 解析为 `/opt/var/tmp`。最大剩余空间是 `/opt/usr` 挂载的 113,541,492 KiB；`/opt` 为约 1,263,900 KiB，根分区为 1,781,416 KiB。

Smack 已启用，执行会话标签为 `User::Shell`；SDB 推入的探针观测标签为 `User::App::Shared`，6/6 均实际执行成功。本轮没有执行拒绝，因此不存在可据以区分 noexec 与 Smack 拒绝的失败样本。板上存在 `chsmack` 和 `smackload`，但合法授权流程/审批依据不可得，记 NOT_AVAILABLE；只读取帮助，未实施任何放宽。

所有临时测试目录均已删除并核验。未安装包、未写持久文件、未改配置、未重启或启停服务、未部署项目制品。
"""
(P / "R22_board_path_report.md").write_text(board_report, encoding="utf-8")

# Patch evidence copies and snippets.
patch_evidence = P / "patches"
patch_evidence.mkdir(exist_ok=True)
shutil.copyfile(ROOT / "progress/T2/forced_unwind_propagation.tsv", patch_evidence / "forced_unwind_propagation.tsv")
lib_log = Path("/home/toolchain/development/libc++_replacement_bak/wave1_execution/stageA/candidate_build/logs/01_libcynara-commons.raw.log")
sec_log = Path("/home/toolchain/development/libc++_replacement_bak/wave1_execution/stageA/diagnostic_resume11/logs/19_security-manager_bc_then_keepgoing.raw.log")
lib_lines = lib_log.read_text(encoding="utf-8", errors="replace").splitlines()
sec_lines = sec_log.read_text(encoding="utf-8", errors="replace").splitlines()
(patch_evidence / "libcynara-commons_error_context.raw.log").write_text("\n".join(lib_lines[692:699]) + "\n", encoding="utf-8")
(patch_evidence / "security-manager_error_context.raw.log").write_text("\n".join(sec_lines[546:556]) + "\n", encoding="utf-8")

PD.mkdir(parents=True, exist_ok=True)
for name in ["libcynara-commons", "security-manager"]:
    (PD / name / "patch").mkdir(parents=True, exist_ok=True)
    (PD / name / "patch/.gitkeep").write_text("", encoding="utf-8")

index = [
    {"包名": "libcynara-commons", "缺陷类型": "LIBSTDCXX_FORCED_UNWIND_TYPE", "状态": "待开发", "目标仓": "platform/core/security/libcynara-commons", "备注": "TryCatch.h:65；公开安装头；本任务未编写补丁"},
    {"包名": "security-manager", "缺陷类型": "LIBSTDCXX_FORCED_UNWIND_TYPE", "状态": "待开发", "目标仓": "platform/core/security/security-manager", "备注": "utils.h:70；内部头；本任务未编写补丁"},
]
write_tsv(PD / "INDEX.tsv", list(index[0]), index)

lib_readme = """# libcynara-commons：libc++ 迁移缺陷记录

## 错误现象

armv7l 候选构建的完整诊断原文：

```text
[   83s] In file included from /home/abuild/rpmbuild/BUILD/cynara-0.26.0/src/helpers/session/session.cpp:34:
[   83s] /home/abuild/rpmbuild/BUILD/cynara-0.26.0/src/common/exceptions/TryCatch.h:65:25: error: no type named '__forced_unwind' in namespace '__cxxabiv1'
[   83s]    65 |     } catch (const abi::__forced_unwind &) {
[   83s]       |                    ~~~~~^
```

证据：`progress/R22/patches/libcynara-commons_error_context.raw.log`；只读来源为 bak 下 `01_libcynara-commons.raw.log:695`。

## 触发条件

- 标准库：`-stdlib=libc++`；运行时包 `libc++-21.1.1-2`、`libc++abi-21.1.1-2`。
- 架构：armv7l，`-march=armv7-a -mthumb -mfloat-abi=softfp`。
- 编译器：`clang-22.1.8-18.1`，驱动 `/bin/armv7l-tizen-linux-gnueabi-clang++`。
- 原命令见 `progress/R22/commands/021_defect_evidence_extract.log`。

## 错误分析

`TryCatch.h:32` 包含 `<cxxabi.h>`，第 65 行捕获 `abi::__forced_unwind` 并立即重抛。现有源码注释明确该分支用于保持 `pthread_cancel` 的强制展开不被 catch-all 吞掉。候选构建实测 libc++abi 的 `__cxxabiv1` 不声明该类型；R1 将 `__forced_unwind` 以两包真实编译失败为锚归入 T1_HARD。原平台发布包使用 libstdc++，该类型由其 C++ ABI 头提供。

## 影响面

该头由 cynara-devel 安装，是对外公开头。既有 44 条传播记录中，本头占 14 条：libcynara-commons 自身 13 条，libcynara-creds-dbus 1 条；逐条文件:行号见 `progress/R22/patches/forced_unwind_propagation.tsv`。已实测受影响源码包：`libcynara-commons`、`libcynara-creds-dbus`。

## 解决方案

NOT_AVAILABLE：本任务按纪律只建立目录与文档，尚未开发或裁定具体源码修改；不得以未经双标准库构建验证的改写代替方案。

## 兼容性

NOT_AVAILABLE：尚无补丁，因此不存在“修改后在 libstdc++ 与 libc++ 下均可构建”的实测记录。

## 验证记录

- 已验证：原代码在 libc++ 候选构建中于 10 个本包 TU 报同一错误；安装头还阻断 libcynara-dbus 的 TU，详细逻辑诊断位于既有 bak 只读证据。
- 已验证：R1 交叉验证把本包标为 T1_HARD，和 T2 构建失败 MATCH。
- 未验证：补丁内容、libstdc++ 回归构建、libc++ 修复后构建、线程取消运行时语义，均 NOT_AVAILABLE。

## 提交时机

暂不提交：该修改只因 libc++ 迁移需要；待平台 libc++ 上下文建立后依次开发、双标准库验证并提交评审。
"""
(PD / "libcynara-commons/README.md").write_text(lib_readme, encoding="utf-8")

sec_readme = """# security-manager：libc++ 迁移缺陷记录

## 错误现象

armv7l 候选构建的完整诊断原文：

```text
[   78s] In file included from /home/abuild/rpmbuild/BUILD/security-manager-2.4.0/src/common/check-proper-drop.cpp:31:
[   78s] In file included from /home/abuild/rpmbuild/BUILD/security-manager-2.4.0/src/common/include/check-proper-drop.h:28:
[   78s] /home/abuild/rpmbuild/BUILD/security-manager-2.4.0/src/common/include/utils.h:70:19: error: no type named '__forced_unwind' in namespace '__cxxabiv1'
[   78s]    70 |     } catch (abi::__forced_unwind &) {
[   78s]       |              ~~~~~^
[   78s] 1 error generated.
[   78s] make[2]: *** [src/common/CMakeFiles/security-manager-commons.dir/build.make:278: src/common/CMakeFiles/security-manager-commons.dir/check-proper-drop.cpp.o] Error 1
```

证据：`progress/R22/patches/security-manager_error_context.raw.log`；只读来源为 bak 下 `19_security-manager_bc_then_keepgoing.raw.log:549`。

## 触发条件

- 标准库：`-stdlib=libc++`；运行时包 `libc++-21.1.1-2`、`libc++abi-21.1.1-2`。
- 架构：armv7l，`-march=armv7-a -mthumb -mfloat-abi=softfp`。
- 编译器：`clang-22.1.8-18.1`，驱动 `/bin/armv7l-tizen-linux-gnueabi-clang++`。
- 原命令见 `progress/R22/commands/021_defect_evidence_extract.log`。

## 错误分析

`utils.h:32` 包含 `<cxxabi.h>`，第 70 行捕获 `abi::__forced_unwind` 后立即重抛，使强制展开不被后续异常处理吞掉。候选构建实测 libc++abi 的 `__cxxabiv1` 不声明该 libstdc++ C++ ABI 专有类型；原平台发布形态使用 libstdc++。

## 影响面

该文件是 security-manager 内部头，不是已观测到的跨源码包安装头。既有 44 条传播记录中，本头占 30 条，消费者全部是 `security-manager`；逐条文件:行号见 `progress/R22/patches/forced_unwind_propagation.tsv`。

## 解决方案

NOT_AVAILABLE：本任务未开发或裁定具体源码修改，且没有可据以证明线程取消语义保持的补丁实测。

## 兼容性

NOT_AVAILABLE：尚无补丁，因此不存在修改后 libstdc++ 与 libc++ 双构建实测。

## 验证记录

- 已验证：libc++ armv7l keep-going 构建在该头产生多处同根错误；首条完整诊断见上文。
- 已验证：R1 交叉验证把本包标为 T1_HARD，和 T2 构建失败 MATCH。
- 未验证：补丁内容、两套标准库回归构建和线程取消运行时语义，均 NOT_AVAILABLE。

## 提交时机

暂不提交：该修改只因 libc++ 迁移需要；待平台 libc++ 上下文建立后依次开发、双标准库验证并提交评审。
"""
(PD / "security-manager/README.md").write_text(sec_readme, encoding="utf-8")

root_readme = """# libc++ 迁移补丁候选目录

每个源码包一个子目录；`patch/` 只接收 `git format-patch` 形态文件，状态由 `INDEX.tsv` 维护。本次首批两项均为“待开发”，所以 `patch/` 仅含用于保留空目录的 `.gitkeep`，没有伪造补丁。
"""
(PD / "README.md").write_text(root_readme, encoding="utf-8")

missing = [
    {"package": "libcynara-commons", "field": "解决方案", "status": "NOT_AVAILABLE", "reason": "未开发/未裁定补丁"},
    {"package": "libcynara-commons", "field": "修改后双标准库兼容性", "status": "NOT_AVAILABLE", "reason": "无补丁可构建"},
    {"package": "libcynara-commons", "field": "线程取消运行时验证", "status": "NOT_AVAILABLE", "reason": "未实施"},
    {"package": "security-manager", "field": "解决方案", "status": "NOT_AVAILABLE", "reason": "未开发/未裁定补丁"},
    {"package": "security-manager", "field": "修改后双标准库兼容性", "status": "NOT_AVAILABLE", "reason": "无补丁可构建"},
    {"package": "security-manager", "field": "线程取消运行时验证", "status": "NOT_AVAILABLE", "reason": "未实施"},
]
write_tsv(P / "patches/missing_fields.tsv", list(missing[0]), missing)

patch_report = """# R22 第三部分：补丁目录建立

已建立 `docs/patches/INDEX.tsv`、`docs/patches/libcynara-commons/{README.md,patch/}` 和 `docs/patches/security-manager/{README.md,patch/}`。两项状态均为“待开发”；目标仓由本地代码仓 remote 原文确认。没有编写补丁代码，也没有修改任何源码。

两份 README 均按固定模板填写。真实 armv7l libc++ 编译诊断、触发配置、源码上下文和 44/44 条传播记录均来自既有 T2/bak 证据。传播分布是：libcynara-commons 公开头 14 条（本包 13、libcynara-creds-dbus 1），security-manager 内部头 30 条（全部本包）。

缺项全部显式登记在 `patches/missing_fields.tsv`：具体修复方案、修改后双标准库构建兼容性、线程取消运行时验证。因为补丁尚未开发，这些字段均为 NOT_AVAILABLE，而不是推断补全。
"""
(P / "R22_patch_directory_report.md").write_text(patch_report, encoding="utf-8")

print("reports=3")
print("migration_rows=", len(rows("progress/R22/tables/migration_packages_full.tsv")))
print("board_candidates=", len(probe))
print("patch_index_rows=", len(index))
