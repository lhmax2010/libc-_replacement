# lzma 返回值取证准备与待确认项

## 当前状态

**正式复现尚未启动，实际失败轮的lzma_ret为NOT_OBSERVED。**

原argv有`-e status=failed`，它不能记录第2c条要求的成功open/read。已提出两个明确选项，等待人工确认：

- 允许去掉该过滤器：保留原跟踪调用集合，另加包装器；把过滤变化单独申报，不称严格只改一个变量。
- 不允许：原过滤保持不变；照常抓返回值和VA快照，无法取得的成功文件访问记录标NOT_OBSERVED。

未自行选择额外扩大host跟踪，没有启动写包、没有-vv、没有新增guest-strace。

## 已做的准备

1. 磁盘初始210675826688字节可用（约197GiB），medium资源门禁0，I/O探针0.06551325699547306秒。最后检查约192GiB，仍高于20GiB。见raw/001–003、019。
2. guest的`command -v ltrace`及文件检查未找到ltrace，`test -x /usr/bin/ltrace`返回1，见raw/005、016；未安装软件或改构建根。选择独立LD_PRELOAD包装器方案。
3. 包装器源码`tmp/ARM_LZMA_0921/lzma_tap.c`，在guest内编译为ARM ELF32共享库。直接用GCC第一次因既有LTO插件为ELFCLASS64而链接失败；仅诊断DSO编译追加`-fno-use-linker-plugin`后成功（raw/006、008）。正式rpmbuild参数不因此改变。
4. 包装器记录每次真实调用的ret、action、avail_in/out前后值、total_in/out、errno前后；原样返回并恢复真实调用后的errno。首次非OK/非STREAM_END时，在真实调用返回后、调用方继续前保存status/maps/limits。STREAM_END是正常结束，不能记为错误。它未写入lzma_stream字段、未改变算法或压缩选项。记录动作和加载DSO仍可能扰动布局/时序，不能声称零侵入。
5. 构造正常压缩、分配器主动拒绝分配、非法action三个小型校准案例，各跑无包装/有包装一次。返回序列、errno、流计数与输出SHA逐项一致；实际记录到0、1、5、11。正常压缩SHA为`13bfc0ec491bc165f444cc4068b0b663a8aa37965d2a00f44ccd85b4e79e75e5`。**这些是人工构造的校准，不是LLVM写包结果**。见CALIBRATION.json、raw/011及tmp下*-tap.calls.tsv。
6. `rpmbuild --version`用同一包装器加载成功，输出RPM version 4.14.1（raw/018）；不是正式写包。
7. 上轮22包成功输出树另存`tmp/ARM_LZMA_0921/previous-vv-success/`，16993项校验通过，原树未删除。cp用时81.624秒，含逐文件校验共177.660秒；见previous-preserved-manifest.json和raw/012。此前失败树备份未动。共享BUILD增量树没有回滚，限制仍然存在。
8. 控制器和capture_failure复用上一轮版本，任务目录与scope改成本轮；正式启动时会核验实际构建进程的nice19、idle、串行和memory.max=16536457216。原文来源及退出码见raw/013–014。准备脚本prepare.py默认保留原过滤器，只有显式参数才会删除过滤；尚未运行该脚本。

## 仪表身份与VA解释限制

- lzma_tap.c SHA256：`db938fde145fb4069a172692a97ed1834141f6fc101ed65d1001f6d56a39eff1`
- lzma_tap.so SHA256：`f7cb2a46c2ea63859b2cceec38327b3da8715386064a270d04331f1c7024412d`
- guest rpmbuild SHA256：`1b3dbf19fed47cd7d30f6ed68ebf8a050f6b09204f086180d04bc43f91976578`
- guest liblzma.so.5.8.1 SHA256：`d29fa8718e3a5ab66836df7553328e3241b7221aeaf2f8b151b97ed4423bc10e`
- spec SHA256：`cde49c78e71ed52f99cb9c7691b2cc04c407a98ae4a5b353b5d58efcc4882c68`

小型校准中pointer_bits=32，实际QEMU_RESERVED_VA=0x100000000；guest读到的maps为32位客体地址，但status的VmSize/VmPeak为4404504kB，超过4GiB，不能当成纯客体已占用地址空间。正式轮需保留原始status，并单独按guest maps统计条目数、区间与最大空洞；它们不是可分配成功的保证。即使拿到MEM_ERROR，也不能仅凭该返回值或VmSize高于4GiB自动证明碎片，更不能直接断言-vv的机制已被证明。

此次未修改平台源码、spec、project_config、根内RPM/liblzma；未推Gerrit或包仓。没有将校准返回码代替目标失败。命令及技术性非零均保留在raw/。
