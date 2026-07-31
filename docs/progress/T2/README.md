# T2 策展说明

产物包括五包实测 `Name`、静态命中、缺陷全量表、传播面和注入有效性预检。结论为 `DONE`：确认两处 `__forced_unwind` 声明面缺陷，另有 4 个 optflags 与 5 个 build_ldflags `NO_OP` 疑点；整包编译因缺 `libc++-devel` 为 `NOT_AVAILABLE`。

全量本地证据位于 `/home/toolchain/development/libc++_replacement/progress/T2/`；源码在本地 `codes/`，诊断根在本地 `tmp/GBS-ROOT/DIAG-20260731/`。源码行和构建全程日志按契约标为 `LOCAL_ONLY`，详见 `EXCLUDED.tsv`。
