# 当前状态

更新：2026-09-22T17:43:30.086909+08:00

| 项 | 状态 | 证据 |
|---|---|---|
| armv7l 整轮/三RPM | PASS | 退出0；三份RPM核验见verify-armv7l/commands.json |
| armv7l RPM内static ELF | PASS | verify-armv7l/result.json；不是旧ELF |
| aarch64 整轮/三RPM | PASS | 退出0；三份RPM核验见verify-aarch64/commands.json |
| aarch64 RPM内static ELF | PASS | verify-aarch64/result.json；不是旧ELF |
| ARM static正常安装预检 | FAIL（已停止） | board/INSTALL_PREFLIGHT.json；不绕过、不重试 |
| ARM正常安装static与%post | NOT_OBSERVED | board/INSTALL_PREFLIGHT.json；正式安装未执行时无INSTALL_RESULT.json |
| ARM候选/原件成对功能 | NOT_OBSERVED | board/pairs.json仅实际执行后才生成；本轮未执行 |
| ARM主包安装预检 | FAIL（非static失败） | 仅预检，未安装主包；board/INSTALL_PREFLIGHT.json |
| ARM取消 shared-gcc | NOT_OBSERVED | 物理板；共享为主证据，静态为对照；不预设崩溃 |
| ARM取消 static-gcc | NOT_OBSERVED | 物理板；共享为主证据，静态为对照；不预设崩溃 |
| 板端恢复与清理 | PRESERVED_FAILURE_SCENE | board/before.json、after.json、CLEANUP.json |

全表与限制见FINAL_RESULT.md。
