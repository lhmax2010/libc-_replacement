# R3 精确源码 ref 核对

| 包 / ref | HEAD / 构建基线 | 包含指定提交 | packaging spec |
|---|---|---|---|
| cereal / tizen | 896a21f0cbcb34835bba8d39f2a4254d691ea98f | 否；空仓占位 Initial empty repository | 不存在 |
| cereal / tizen_base | 28aedc95fdc600568c57babf28ff717e0b2bca7d | 是，HEAD 就是 manifest SHA | cereal.spec 存在 |
| cereal / accepted/tizen_base、accepted/tizen_base_dev、accepted/tizen_base_toolchain、accepted/tizen_base_x | 28aedc95fdc600568c57babf28ff717e0b2bca7d | 是 | 存在 |
| icu / sandbox/lhmax2025/libcxx-migration | dcbb3fb476b4d57c4c7e5770f1ed55d43f6f6330 | 是 | icu.spec 存在 |
| qemu-accel / tizen_base | HEAD=419d8296b4073bbabab7123bec1abe4146087126；以 manifest 的 2737f3bf78ed33be4333114c0e5c741a1deca467 为本地基线，不追 ref 最新值 | 是，HEAD 更新但本轮不用 | armv7l/aarch64/riscv64 三份 spec 存在 |

完整远端 ref/SHA（含 qemu 各 ref 的 HEAD）、contains 与 tags 原始输出：raw/040–050、052。cereal 指定提交可达，按人工 R3 裁决将尚未提交改动的本地指定分支由空占位移动到精确 SHA；未导入旧源码、未追随其它 HEAD、未创建其它分支。三个基线均不需追加按 SHA fetch。

manifest 原文保存在 `../QB_1194606/evidence/manifest.xml`，检索为 raw/048：

```xml
<project name="platform/upstream/cereal" path="platform/upstream/cereal" revision="28aedc95fdc600568c57babf28ff717e0b2bca7d"/>
<project name="platform/upstream/icu" path="platform/upstream/icu" revision="e4a4d7411b0f288581cf020a176b5dddbb504ae7"/>
<project name="platform/upstream/qemu-accel" path="platform/upstream/qemu-accel" revision="2737f3bf78ed33be4333114c0e5c741a1deca467"/>
```

这三行没有 upstream 字段，记 NOT_OBSERVED，不能由远端分支名补造。ICU manifest 仍是旧 e4a4，与 QuickBuild 实际 checkout 的 dcbb 不一致；沿用已查明的实际日志和本轮人工指定 dcbb，不回退 manifest 旧版本。其它两包按 manifest 精确 revision。
