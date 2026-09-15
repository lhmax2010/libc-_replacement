import datetime,hashlib,json
from pathlib import Path
p=Path('progress/BUILD_NIGHT_0917'); data=json.loads((p/'old_libclang_result.json').read_text())
paths=[Path(x) for x in (p/'raw/189_other_source_scope.stdout.txt').read_text().splitlines() if x.endswith('/packaging')]
files=[]
for d in paths:
    for f in sorted(d.rglob('*')):
        if f.is_file() and f.suffix in ['.spec','.cmake','.sh','.md']:
            files.append((str(f),hashlib.sha256(f.read_bytes()).hexdigest()))
(p/'w4_searched_files.tsv').write_text('path\tsha256\n'+''.join(f'{f}\t{h}\n' for f,h in files))
now=datetime.datetime.now().astimezone(); start=datetime.datetime.fromisoformat((p/'raw/180_w4_resource_gate.started.txt').read_text().strip()); seconds=(now-start).total_seconds(); assert seconds<3600
(p/'w4_result.json').write_text(json.dumps(dict(status='PARTIAL_ORIGIN',start=start.isoformat(),finish=now.isoformat(),seconds=seconds,consumer_revision='624f68d5f67a673083e5ad08ea205742613d6e53',introduction='84440faa935cc2650bf84e3204550016d7d21db8',producer_recipe_candidate='46df8cd0d7cb564af6e1e482f3c48d04dba3079b',searched_packaging_directories=len(paths),searched_files=len(files),unresolved=['remote branch creator and creation time','exact producer build to payload hash chain','all-platform other consumers']),indent=2))
body='''# W4：旧 libclang 21.1.1 归档来源

## 结论：引入者与编译器已确认，原始生成批次尚未闭合

- 消费分支：bpftrace 的 sandbox/hoegeun/static-build，远端本次仍为
  624f68d5f67a673083e5ad08ea205742613d6e53。
- 归档由提交 84440faa935cc2650bf84e3204550016d7d21db8 引入，作者/提交者
  Hoegeun Kwon <hoegeun.kwon@samsung.com>；作者时间 2026-02-04 13:03:38 +0900，
  提交时间 13:04:04 +0900。提交说明明确称 prebuilt libclang v21.1.1。
- 两份归档各 23 个对象，所有对象的 .comment 都是
  `GCC: (Tizen GCC 14.2.0 20240801 2.1) 14.2.0`。这是直接编译器注记，
  不是从 std::__cxx11 猜 GCC。未定义 __cxx11 符号行分别 31/30，St3__1 为 0。
- 找到生产端候选配方：LLVM 的 sandbox/hoegeun/llvm-static @
  46df8cd0d7cb564af6e1e482f3c48d04dba3079b，spec 版本 21.1.1，明确打开静态
  libclang。但没有构建日志/产物哈希证明这份配方产出了本次两份归档。

因此不能写“原生成配方已完全确认”，也不能继续写“完全没有任何生产端线索”。
远端分支是谁创建、何时创建为 NOT_OBSERVED；Git commit 的作者/时间不能证明
服务器分支创建事件。真正运行原构建的人同样 NOT_OBSERVED，提交者不等于构建者。

## 输入及哈希链

完整仓库 URL：

- ssh://lhmax2025@review.tizen.org:29418/platform/upstream/bpftrace
- ssh://lhmax2025@review.tizen.org:29418/platform/upstream/llvm

引入提交与本次消费分支 HEAD 的 packaging/prebuilt.tar.gz blob 完全相同，
SHA256 均为 0ac7bac842a0cbfa64a8227bd554a0a9c3d85d54d3795a44791603917359264a。
本轮重新从 Git blob 计算，而非只照抄上一轮元数据。

| 包内路径 | 架构 | SHA256 |
| --- | --- | --- |
| prebuilt/usr/lib64/libclang.a.21.1.1 | AArch64 | cb5958fa3c575430a9e033d9d24dc49311836693f6f9f295c93b5a18963fa9b8 |
| prebuilt/usr/lib/libclang.a.21.1.1 | ARM | bceccaf5ef0d5fd44478b93888dd5630f9389408474651159d114a2fccb317d5 |

版本 21.1.1 的依据为文件名与引入提交说明；没有把归档当程序执行 --version。
本轮重新核查 ar 成员、readelf 机器类型/.comment、nm 未定义符号和完整文件哈希。
详见 old_libclang_result.json、old_libclang_commands.json、raw/181、187、200。
两份归档的字节来自前一夜已保存 payload；旧元数据来源和新核查均有记录。

## 消费侧与生产端代码记录

消费分支 packaging/bpftrace.spec:9 为 Source1002: prebuilt.tar.gz；%prep 解包，
第 71 行传入 `-DLIBCLANG_STATIC_PATH:PATH=prebuilt%{_libdir}/libclang.a`。
src/ast/CMakeLists.txt:83–86 将该路径转为源码相对绝对路径并加入 ast 的链接库：

```cmake
if(LIBCLANG_STATIC_PATH)
  get_filename_component(LIBCLANG_STATIC_ABS_PATH "${CMAKE_SOURCE_DIR}/${LIBCLANG_STATIC_PATH}" ABSOLUTE)
  message(STATUS "Using custom libclang absolute path: ${LIBCLANG_STATIC_ABS_PATH}")
  target_link_libraries(ast PUBLIC ${LIBCLANG_STATIC_ABS_PATH} bpf clangIndex clangDriver clangFrontend clangCodeGen)
```

这是静态构建时消费归档的直接配置证据，不是对所有已发布 bpftrace 二进制的归属证明。
引入提交只修改 spec 并添加二进制 tar（raw/187），未附生成脚本。

生产端候选提交 46df8cd… 的作者 Hoegeun Kwon；作者时间
2025-12-17 18:11:00 +0900，提交时间 2025-12-23 14:52:21 +0900。
提交摘要 `packaging: test static build for llvm and clang`。
原始 diff 与完整 spec 在 raw/197、198，带行号实文在 raw/199：

```text
# packaging/llvm.spec at 46df8cd…, lines 161–168
-DCMAKE_C_COMPILER=%__cc
-DCMAKE_CXX_COMPILER=%__cxx
-DLLVM_HOST_TRIPLE=%{_host}
-DCMAKE_ASM_FLAGS="$CFLAGS"
-DCMAKE_BUILD_TYPE=Release
-DLLVM_ENABLE_ASSERTIONS=No
-DLLVM_ENABLE_RTTI=ON
-DLLVM_TARGETS_TO_BUILD='X86;ARM;AArch64;BPF;RISCV'
# lines 178–180
-DLLVM_BUILD_STATIC=ON
-DLIBCLANG_BUILD_STATIC=ON
-DLLVM_ENABLE_PROJECTS="clang;lldb;clang-tools-extra;lld"
```

还加入静态依赖，启用 CMake trace，原记录并行参数为 6。本轮只读该配方，
**未执行其并行 6 参数，也未把整分支套入候选**。该旧配方本身没有本任务要求的
libc++ 条件保护，不能直接替代 W1/W2 已做的条件化改动。
真实原构建中的 RPM 宏值、输入 RPM、完整 CMake trace/命令及产物哈希仍缺。
需原构建记录或开发人员提供可验证的“此配方→这两个 SHA256”对应关系；同作者、
同版本及静态开关一致只是候选线索，不构成完整来源证明。

## 是否还有其他消费者

已证实的消费者是上述 bpftrace 静态构建分支。
对本地可得 20 个 packaging 目录的 .spec/.cmake/.sh/.md 用字面量检索
libclang.a.21.1.1、LIBCLANG_STATIC_PATH、prebuilt.tar.gz，结果为 0；
另对 qemu-accel@2737f3bf78ed33be4333114c0e5c741a1deca467 的已跟踪文本做
同类及 bpftrace 检索，也为 0。具体文件和 SHA256 在 w4_searched_files.tsv；
这些是本次工作区文件，不假装是全部源码仓的同一固定快照。
raw/201 用已知候选 spec 验证相关文本模式确实能命中，旧分支 raw/190 也有阳性。

这些检索不能排除改名/复制后的资产、外部 CI 配方、未下载源码、已经静态链入
且剥离符号的产物。**全平台是否另有消费者仍为 NOT_OBSERVED**，不能写“没有”。
当前 f895f8c… 的 Source1002 是完整 bpftrace 程序 tar，和这里的 libclang tar
不是同一文件；未证明那两个完整程序由本分支生成，其来源未知登记仍然保留。
W3 新候选未使用这份 21.1.1 归档。

## 方法与技术性重试

--deepen=30 被服务端以 internal server error 拒绝（raw/183）；改为明确
--depth=30 后读取到引入提交及父提交（raw/184–187）。未推送远端、未改分支。
历史仍是浅库，不声称遍历全部历史。
raw/188 把 21.1.1 当正则，误命中 SVG 数字；raw/190 改字面量 -F，保留原记录。
raw/192、193 的退出 1 是检索零匹配，不是读取成功的替代证据或全平台无使用证明。
raw/194–195 的本地对象定位错误已改为 codes/llvm 的只读对象缓存（raw/196）。

此处说明的是 **Clang/libclang 本身作为 C++ 程序/库时携带的 ABI**，不是
Clang driver 不能使用 libc++。旧归档为 GCC + libstdc++ ABI、标称 21.1.1，
不能作为本轮 LLVM 22.1.8 + libc++ 静态组件的匹配输入。
登记见 KNOWN_ITEMS.md；未修改旧资产或发布配置。
'''
(p/'W4_REPORT.md').write_text(body)
s=(p/'STATUS.md').read_text().replace('| W4 21.1.1 归档来源 | RUNNING | raw/180 起，限 1 小时 |',f'| W4 21.1.1 归档来源 | PARTIAL_ORIGIN | {seconds:.0f} 秒 |')
(p/'STATUS.md').write_text(s)
print(json.dumps(dict(status='PARTIAL_ORIGIN',seconds=seconds,packaging_dirs=len(paths),files=len(files))))
