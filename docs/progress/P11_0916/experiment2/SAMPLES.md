# 产物身份与符号读取结果

本表由 summarize.py 从实测 JSON 生成。5 轮是静态读取，不是执行目标函数。

| 样本 | 包 / 架构 | RPM 内路径 | ELF SHA256 | 动态符号序号 / 版本索引 |
| --- | --- | --- | --- | --- |
| elf_01 | chromium-efl / aarch64 | `/usr/lib64/libchromium-ewk.so` | `bea1a289188c7bffd1c60aa7c046530b9eb4e084cfb8578d4ad8b875141cb721` | 409 / 1 |
| elf_02 | chromium-efl / aarch64 | `/usr/share/chromium-efl/lib/libchromium-impl.so` | `16a8497f79c3c0e5b8a9cd0ff8987cfa3d78539dc0facfb9c301ecc0bacf4c22` | 4874 / 1 |
| elf_03 | app-rootstrap-devel / x86_64 | `/opt/data/app-rootstrap/usr/lib64/libchromium-ewk.so` | `33e4dd3a48a528c554c331b6e76a48f15470fc83c2a7ad18061d14518c3f962e` | 409 / 1 |
| elf_04 | app-rootstrap-devel / x86_64 | `/opt/data/app-rootstrap/usr/lib64/libewebkit2.so` | `33e4dd3a48a528c554c331b6e76a48f15470fc83c2a7ad18061d14518c3f962e` | 409 / 1 |
| elf_05 | app-rootstrap-devel / x86_64 | `/opt/data/app-rootstrap/usr/lib64/libewebkit2.so.0` | `33e4dd3a48a528c554c331b6e76a48f15470fc83c2a7ad18061d14518c3f962e` | 409 / 1 |
| elf_06 | app-rootstrap-devel / x86_64 | `/opt/data/app-rootstrap/usr/share/chromium-efl/lib/libchromium-impl.so` | `c4896af8a327cd10431b548b7fd8185a9f7313f2849ee69ed01d3968245a3091` | 4916 / 1 |
| elf_07 | chromium-efl / x86_64 | `/usr/lib64/libchromium-ewk.so` | `30d20865d8657f0a3b1d37693e3521a4660cdc3f4b88d21f5c0991dd15d9c0a5` | 409 / 1 |
| elf_08 | chromium-efl / x86_64 | `/usr/share/chromium-efl/lib/libchromium-impl.so` | `18f4ea7cf5962c8d9ecb2436343a881852c5e730326a98c18029a3d5883b27b7` | 4916 / 1 |
| elf_09 | chromium-efl / armv7l | `/usr/lib/libchromium-ewk.so` | `f1c7b9f77897a6d252b554f3cdb9a60bdfc6a65e77b2de24bf183b4b50bf10ff` | 414 / 1 |
| elf_10 | chromium-efl / armv7l | `/usr/share/chromium-efl/lib/libchromium-impl.so` | `6c755dcb6db1afadab28e616816afa9ab336c7f1193f204ce4c9d42324bc1d59` | 3519 / 1 |
| elf_11 | app-rootstrap-devel / aarch64 | `/opt/data/app-rootstrap/usr/lib64/libchromium-ewk.so` | `be7e5307800110e983b22a3857eeeba3aa919405ecb2f03decc96d59b62434de` | 409 / 1 |
| elf_12 | app-rootstrap-devel / aarch64 | `/opt/data/app-rootstrap/usr/lib64/libewebkit2.so` | `be7e5307800110e983b22a3857eeeba3aa919405ecb2f03decc96d59b62434de` | 409 / 1 |
| elf_13 | app-rootstrap-devel / aarch64 | `/opt/data/app-rootstrap/usr/lib64/libewebkit2.so.0` | `be7e5307800110e983b22a3857eeeba3aa919405ecb2f03decc96d59b62434de` | 409 / 1 |
| elf_14 | app-rootstrap-devel / aarch64 | `/opt/data/app-rootstrap/usr/share/chromium-efl/lib/libchromium-impl.so` | `e7f199275299b4a4106e8c6ea633c61600c1c14495fe419ae18033225e5f813d` | 4874 / 1 |
| elf_15 | app-rootstrap-devel / armv7l | `/opt/data/app-rootstrap/usr/lib/libchromium-ewk.so` | `09776bb987abcb49567e521d10451d838ed3f039f377a54e0067315084f77596` | 414 / 1 |
| elf_16 | app-rootstrap-devel / armv7l | `/opt/data/app-rootstrap/usr/lib/libewebkit2.so` | `09776bb987abcb49567e521d10451d838ed3f039f377a54e0067315084f77596` | 414 / 1 |
| elf_17 | app-rootstrap-devel / armv7l | `/opt/data/app-rootstrap/usr/lib/libewebkit2.so.0` | `09776bb987abcb49567e521d10451d838ed3f039f377a54e0067315084f77596` | 414 / 1 |
| elf_18 | app-rootstrap-devel / armv7l | `/opt/data/app-rootstrap/usr/share/chromium-efl/lib/libchromium-impl.so` | `20398c5e7503c180e65a94ec44d7620caafdb57821190424105ad5dce81a5eeb` | 3519 / 1 |

## 来源 RPM

| 包 / 架构 | 发布路径 | 完整 RPM SHA256 |
| --- | --- | --- |
| chromium-efl / aarch64 | `aarch64/chromium-efl-1.1.144-1.aarch64.rpm` | `0a0320ded6fd45a3c56edc042a69a59bcabe49bdab84d54472d5b79867e724a9` |
| app-rootstrap-devel / x86_64 | `x86_64/app-rootstrap-devel-10.0.0-1.x86_64.rpm` | `30727d544680277e23f5d3ff70ad9d6a93656c616f39063bc8d9dd46dd6f4b81` |
| chromium-efl / x86_64 | `x86_64/chromium-efl-1.1.144-1.x86_64.rpm` | `4e803cedd52c682c1f90ffbbfc76153a5124f64f502710aaf5a59b478b431e49` |
| chromium-efl / armv7l | `armv7l/chromium-efl-1.1.144-1.armv7l.rpm` | `97a53174c2c090ddc497eb893193adbff8e2a7f9b797d7fd2dcf3e05226fcba6` |
| app-rootstrap-devel / aarch64 | `aarch64/app-rootstrap-devel-10.0.0-1.aarch64.rpm` | `d30fb9e0ac642f1e436cf8a1f9dc4bc2a7a3726fb047f8897805f2cf66767b2e` |
| app-rootstrap-devel / armv7l | `armv7l/app-rootstrap-devel-10.0.0-1.armv7l.rpm` | `e5179e4b7796654bd537518890d146f7b2f160093772fe17ee4fe3eb5bd04cc1` |

## readelf -Ws 原文摘录

### elf_01

完整 -Ws 与 -V：[符号表](raw/elf_01_r1_Ws.stdout) / [版本表](raw/elf_01_r1_V.stdout)。

```text
   409: 0000000000034194   104 FUNC    GLOBAL DEFAULT   12 ewk_parse_cookie
```

### elf_02

完整 -Ws 与 -V：[符号表](raw/elf_02_r1_Ws.stdout) / [版本表](raw/elf_02_r1_V.stdout)。

```text
  4874: 00000000052486f4   492 FUNC    GLOBAL DEFAULT   13 ewk_parse_cookie
```

### elf_03

完整 -Ws 与 -V：[符号表](raw/elf_03_r1_Ws.stdout) / [版本表](raw/elf_03_r1_V.stdout)。

```text
   409: 000000000002bd78    77 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_04

完整 -Ws 与 -V：[符号表](raw/elf_04_r1_Ws.stdout) / [版本表](raw/elf_04_r1_V.stdout)。

```text
   409: 000000000002bd78    77 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_05

完整 -Ws 与 -V：[符号表](raw/elf_05_r1_Ws.stdout) / [版本表](raw/elf_05_r1_V.stdout)。

```text
   409: 000000000002bd78    77 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_06

完整 -Ws 与 -V：[符号表](raw/elf_06_r1_Ws.stdout) / [版本表](raw/elf_06_r1_V.stdout)。

```text
  4916: 000000000603a1e6   619 FUNC    GLOBAL DEFAULT   15 ewk_parse_cookie
```

### elf_07

完整 -Ws 与 -V：[符号表](raw/elf_07_r1_Ws.stdout) / [版本表](raw/elf_07_r1_V.stdout)。

```text
   409: 000000000002bd78    77 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_08

完整 -Ws 与 -V：[符号表](raw/elf_08_r1_Ws.stdout) / [版本表](raw/elf_08_r1_V.stdout)。

```text
  4916: 000000000603a1e6   619 FUNC    GLOBAL DEFAULT   15 ewk_parse_cookie
```

### elf_09

完整 -Ws 与 -V：[符号表](raw/elf_09_r1_Ws.stdout) / [版本表](raw/elf_09_r1_V.stdout)。

```text
   414: 00029c51    60 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_10

完整 -Ws 与 -V：[符号表](raw/elf_10_r1_Ws.stdout) / [版本表](raw/elf_10_r1_V.stdout)。

```text
  3519: 01a15d8d   380 FUNC    GLOBAL DEFAULT   15 ewk_parse_cookie
```

### elf_11

完整 -Ws 与 -V：[符号表](raw/elf_11_r1_Ws.stdout) / [版本表](raw/elf_11_r1_V.stdout)。

```text
   409: 0000000000034194   104 FUNC    GLOBAL DEFAULT   12 ewk_parse_cookie
```

### elf_12

完整 -Ws 与 -V：[符号表](raw/elf_12_r1_Ws.stdout) / [版本表](raw/elf_12_r1_V.stdout)。

```text
   409: 0000000000034194   104 FUNC    GLOBAL DEFAULT   12 ewk_parse_cookie
```

### elf_13

完整 -Ws 与 -V：[符号表](raw/elf_13_r1_Ws.stdout) / [版本表](raw/elf_13_r1_V.stdout)。

```text
   409: 0000000000034194   104 FUNC    GLOBAL DEFAULT   12 ewk_parse_cookie
```

### elf_14

完整 -Ws 与 -V：[符号表](raw/elf_14_r1_Ws.stdout) / [版本表](raw/elf_14_r1_V.stdout)。

```text
  4874: 00000000052486f4   492 FUNC    GLOBAL DEFAULT   13 ewk_parse_cookie
```

### elf_15

完整 -Ws 与 -V：[符号表](raw/elf_15_r1_Ws.stdout) / [版本表](raw/elf_15_r1_V.stdout)。

```text
   414: 00029c51    60 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_16

完整 -Ws 与 -V：[符号表](raw/elf_16_r1_Ws.stdout) / [版本表](raw/elf_16_r1_V.stdout)。

```text
   414: 00029c51    60 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_17

完整 -Ws 与 -V：[符号表](raw/elf_17_r1_Ws.stdout) / [版本表](raw/elf_17_r1_V.stdout)。

```text
   414: 00029c51    60 FUNC    GLOBAL DEFAULT   14 ewk_parse_cookie
```

### elf_18

完整 -Ws 与 -V：[符号表](raw/elf_18_r1_Ws.stdout) / [版本表](raw/elf_18_r1_V.stdout)。

```text
  3519: 01a15d8d   380 FUNC    GLOBAL DEFAULT   15 ewk_parse_cookie
```

