import datetime,hashlib,json,subprocess
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); t=Path('tmp/NIGHT_0917'); rows=[]
for arch in ['x86_64','aarch64']:
    data=json.loads((p/f'{arch}_runtime_comparison.json').read_text()); assert len(data)==2 and all(r['passed'] for r in data)
    for mode in ['libcxx','baseline','gcc']:
        events=[json.loads(x) for x in (p/f'runtime-{arch}-{mode}.events.jsonl').read_text().splitlines()]
        assert events[-1]['event']=='finished' and events[-1]['exitcode']==0
        rows.append(dict(arch=arch,mode=mode,start=events[0]['time'],finish=events[-1]['time'],compiler='Clang 22.1.8 (runtime spec unchanged)',result='PASS_TARGET_BUILD'))
    norm=lambda s:'\n'.join(l for l in s.splitlines() if l.strip())
    assert norm((p/f'{arch}_baseline.expanded.spec').read_text())==norm((p/f'{arch}_gcc.expanded.spec').read_text())==norm((p/f'{arch}_undefined.expanded.spec').read_text())
now=datetime.datetime.now().astimezone(); start=datetime.datetime.fromisoformat((p/'raw/001_resource_gate.started.txt').read_text().strip())
(p/'w1_result.json').write_text(json.dumps(dict(status='COMPLETE_TARGET_BUILDS',start=start.isoformat(),finish=now.isoformat(),seconds=(now-start).total_seconds(),cells=rows,rpm_build='NOT_OBSERVED',armv7l='NOT_OBSERVED_BOARD_OFFLINE'),indent=2))
body='''# W1：条件启用 libc++ / libc++abi 静态归档

## 结论

x86_64、aarch64 两架构均完成：候选 libc++ 路径目标构建、改前同环境基线构建、
候选 GCC 条件路径构建。两种新增静态归档均已生成。共享库动态符号表及 DT_NEEDED
与同环境基线一致；GCC 条件路径不生成新增归档，符号和依赖同样一致。

这是 **CMake 目标构建与产物核对完成**，不是新 RPM 全包写包验证，也不是
新静态展开器/线程取消行为验证。armv7l 未构建，板上项目 NOT_OBSERVED_BOARD_OFFLINE。

## 基线与改动

来源 tmp/BUILD_W1BC_0911/target-fetch 的干净 f203923a1508c9344f5fc6b17bd8822f011655c4。
原源码及旧构建根通过 bwrap 只读挂载；只写 tmp/NIGHT_0917。
候选 specs/libcxx-runtimes.spec 只处理静态构建/安装开关与对应两个条件 %files 项。
完整 diff：raw/035_w1_diff.stdout.txt（diff 退出 1 表示存在预期改动）。

四个静态开关仅在 _toolchain 已定义且 toolchain_is clang 时 ON；否则 OFF。
%files 中新增 libc++.a/libc++abi.a 使用同一条件，避免影响 GCC 路径或遗漏安装文件。
原共享库、展开器、ABI 合并选项均不改变。

## 实测方式与边界

读取旧根的实际架构编译 flags；从候选 spec 通过 rpmspec -P 得到 CMake 命令，
只替换源码/构建/版本脚本的临时路径，以 -j1 实际运行 cxxabi、cxx、cxx_experimental。
平台 project_config 的 toolchain_is 宏原文作为 rpmspec 输入，不用常量模拟条件。
八份展开结果和八份执行脚本（未定义路径仅展开，未构建）均归档。

重要：该运行时包的原 spec 本来就直接选择目标前缀 Clang。
因此 GCC 条件路径实跑的编译器仍是 Clang 22.1.8，这是**保留原行为**，
不是“GCC driver 编译成功”。没有擅自把本来强制 Clang 的包改为 GCC 编译。
两架构 baseline/GCC/undefined 展开 spec 忽略空行完全一致。

readelf 动态符号比较包含大小、类型、绑定、可见性、节索引、符号及版本名，
排除装载地址；完整原始表及差集保留。x86_64 的 libc++/libc++abi 分别
2203/415 条；aarch64 分别 2209/419 条，所有比较差集为空。
DT_NEEDED 同样完全一致。文件 SHA256 不同，本轮未对所有字节差异归因，
不称逐字节一致，也不把符号表一致扩展为全部行为已验证。

archive_*.json 记录新归档哈希、ar 成员、编译器注记和精确字符串检查。
x86_64 两归档 _ZNSt3__1 命中行数 17486/315，aarch64 17411/315；
四份 _ZNSt7__cxx11 均为 0。数字是 strings 命中行，不是去重接口数。
静态库并不通过 DT_NEEDED 判定；最终静态程序的依赖与功能留给 W3。

## 资源与说明

并行 1、nice19、ionice3、ulimit -v 16148878 KiB；每个构建都有进程组监控、
500 目标 I/O 探测、三次暂停策略和 W1 固定截止时间。没有放宽门禁。
编译脚本固定 PATH。技术性路径/PATH 重跑见 STATUS.md，原始失败日志未改。
raw/024 的全构建目录 du 为减少元数据 I/O 已中止；后续改查精确文件，
不是构建任务失败，也未把缺失对象当作完整构建缓存可复用。

只提交候选 spec 副本、脚本、差异及证据到项目仓；不提交/推送平台包仓。
'''
(p/'W1_REPORT.md').write_text(body)
s=(p/'STATUS.md').read_text().replace('| W1 libcxx 静态库 | RUNNING | 以 raw/001 开始计时，限 3 小时 |',f'| W1 libcxx 静态库 | COMPLETE_TARGET_BUILDS | {(now-start).total_seconds():.0f} 秒 |')
(p/'STATUS.md').write_text(s)
print(json.dumps(dict(w1='COMPLETE_TARGET_BUILDS',seconds=(now-start).total_seconds(),cells=6)))
