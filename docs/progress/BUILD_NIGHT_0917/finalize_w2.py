import datetime,json
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917')
r=json.loads((p/'libclang_result.json').read_text())
cells=[]
for name in ['x86_64-libcxx','x86_64-gcc','aarch64-libcxx']:
    e=[json.loads(s) for s in (p/f'libclang-{name}-build.events.jsonl').read_text().splitlines()]
    assert e[-1]['event']=='finished' and e[-1]['exitcode']==0
    cells.append(dict(cell=name,start=e[0]['time'],finish=e[-1]['time'],result='PASS_TARGET_BUILD'))
now=datetime.datetime.now().astimezone(); start=datetime.datetime.fromisoformat((p/'raw/043_w2_resource_gate.started.txt').read_text().strip())
seconds=(now-start).total_seconds(); assert seconds<5*3600
(p/'w2_result.json').write_text(json.dumps(dict(status='COMPLETE_TARGET_BUILDS',start=start.isoformat(),finish=now.isoformat(),seconds=seconds,cells=cells,artifacts=r,rpm_build='NOT_OBSERVED'),indent=2))
body='''# W2：条件产出 libclang.a

## 结果及适用范围

x86_64、aarch64 的 libc++ 静态 libclang 目标已构建成功；x86_64 的真实
GCC 14.2.0 路径也完成 libclang 共享目标，未生成 libclang.a，未引入 libc++ 参数。
这是目标构建完成，不是完整 LLVM RPM 写包，也不是 bpftrace 静态功能验证。
后者必须在下一阶段独立核查，不能从归档中的 ABI 标记推出最终程序合格。

## 改动与实际构建

候选 llvm.spec 基于 f203923a1508c9344f5fc6b17bd8822f011655c4。
唯一新增行 -DLIBCLANG_BUILD_STATIC=ON 放在原有 build_with_libcxx 条件内；
该条件仅在 _toolchain 已定义且 toolchain_is clang 时成立。
static-devel 原有 *.a 文件规则已涵盖新增归档。差异见 raw/057，宏展开核查见 raw/052。
GCC、未定义路径与基线展开结果忽略空行一致。

实际使用 Clang 源码自带 standalone CMake 入口，复用已安装 LLVM 开发库，
而非重新构建 LLVM。源码与原根均只读挂载。libc++ 两格各构建 libclang_static
及其所需 Clang 组件；真实 GCC 格构建 libclang。完整参数与固定 PATH 见
libclang_commands.json、candidate_specs/configure-libclang-*、build-libclang-*。
这些 standalone 参数不是 llvm.spec 全部打包参数的逐项复制；没有把目标验证
冒充全包、原 RPM 字节一致或所有 GCC 架构验证。GCC 本轮只实际验证 x86_64。

两份新 libclang.a 的 _ZNSt3__1 字符串检查为阳性，_ZNSt7__cxx11 为零，
并以 nm 的较宽 __cxx11 匹配复核为零。libclang_result.json 保留哈希、大小及计数；
计数是字符串行，不是去重 API 数。GCC 共享产物 DT_NEEDED 含 libstdc++.so.6，
不含 libc++.so.1 / libc++abi.so.1；实际 build.ninja 的两个禁止参数均为零。

## 环境修正与审计限制

- x86_64 根缺 Ninja：从已有 Tizen 根复制到任务临时工具目录；原根未修改。
- aarch64 根缺 Python 解释器：从先前运行时构建根复制 Python 3.14 与 libpython，
  标准库只读挂载；固定 PYTHONHOME、PATH、LD_LIBRARY_PATH，配置重跑通过。
- 首次 libclang 配置失败日志被同名重跑覆盖：如实保存工具返回的恢复记录
  libclang-configure-first-failure.RECOVERED.md，不称原始自动落盘日志。
  从后续重跑起先保留 previous-N 日志；执行过的旧脚本另存并保留哈希。
- raw/109、110 查询 GCC 库目录用了不适用的 usr/lib 路径，技术性退出 2；
  raw/111 重新定位到 usr/lib64/gcc/...，确认 libstdc++fs.a 实际存在。

并行 1、nice19、ionice3、ulimit -v 16148878 KiB，监控每 500 目标探测 I/O。
不修改 codes/、上游 C++ 或原构建根；只交付临时候选与证据，未推包仓。
'''
(p/'W2_REPORT.md').write_text(body)
s=(p/'STATUS.md').read_text().replace('| W2 libclang.a | RUNNING | raw/043 起，限 5 小时 |',f'| W2 libclang.a | COMPLETE_TARGET_BUILDS | {seconds:.0f} 秒 |')
(p/'STATUS.md').write_text(s)
print(json.dumps(dict(status='COMPLETE_TARGET_BUILDS',seconds=seconds,cells=len(cells))))
