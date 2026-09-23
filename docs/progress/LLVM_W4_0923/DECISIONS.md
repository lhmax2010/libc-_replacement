# 自行判断、边界与技术性记录

## R2 追加（当前状态见 FINAL_RESULT_R2.md）

- 旧GCC override门禁按裁决撤销；LLVM四组全PASS。第一笔只改正文为617a210064c4559fe1152728f7c9cf5e1ab99a9f，与旧2cba97e5的tree相同。
- runtime默认组多12个开头空行，严格比较未通过；已询问能否登记为非功能差异，未自行忽略。第二笔、其余三组与runtime prep未做。
- 38份追加RPM重算SHA并核对当前rpmdb包头；全根为133/134/126/127项，19份不代表全量，当前查询不冒充历史瞬间身份。
- A/B/C与未能归类分开，RPM重算SHA与仓元数据SHA分开，不据VCS单独推定来源。全量门禁未闭合。
- raw/R2_006等路径查询技术性非零保留；R2_016跨全GBS树索引主动停止，转查明确历史路径，未运行或中止构建。
- 报告apply_patch首次因同一补丁对W4_SIGNOFF同时Delete/Add被工具拒绝，未应用；改为分别更新文档，未涉及平台文件。
- 仅提交本目录与LINE_STATUS，保留无关脏工作树；包仓不推，无Signed-off-by。

## R1 历史记录（以下不替代 R2 状态）

- light资源门禁退出0；无构建、无板操作、无QuickBuild、无包仓推送。只允许%prep，独立输出tmp/LLVM_W4_0923/prep，原根与packaging只读绑定。prep为nice19/ionice idle，cgroup MemoryMax=16536457216（总内存50%向页下取整），实际cgroup见prep-resources.json；磁盘264GiB。
- 两份最终写包spec实测同SHA，完整diff只有新增一行；历史VCS/Release说法不强行套入这两份。只读查询34份RPM，未提取或修改原RPM。
- 原可写克隆在本地w1b-push分支、HEAD=f203923a且干净。本轮从该HEAD新建本地目标sandbox同名分支，旧分支未改变。fetch只取目标ref。作者按授权Hao Lin；committer保留克隆现有hao.lin身份，Git提交无法没有该字段，人工amend再确定。提交时仅本次命令禁用hooks，避免自动追加未审文本；未改仓库配置、无Signed-off-by。
- 提交后的三种宏检查使用指定现有ARM根真实宏，GCC严格用_toolchain_override gcc，不以字面_toolchain gcc替代。发现不满足后仅核对远端基线、继续已授权%prep和只读对账，没有修任何条件。命令退出0不代表判据PASS。
- 11包选最近验收轮次的保留导出配方，历史执行证据定位到processing recipe原行；对比的是本轮重算SHA的现存文件。源码tar内部成员和所有架构所有验证夹具不在本轮packaging字节比较范围；tensorflow2的源码修正不能仅由packaging比较重验。
- libsigc++首次脚本误选早期失败日志（带++的标签），未取得recipe路径，raw/030退出1。保留脚本与部分结果于audit_recipes.attempt1.py、recipe-audit-attempt1/；改选实际完成的366 retry日志，raw/034完整11包成功。个别历史路径猜测未命中，原stderr保留，不将其写成不存在。
- LLVM构建SOURCES附带的libcxx-runtimes.spec仅差VCS；它不是静态运行库的实际候选。另沿BUILD_NIGHT_0917/W1与BPF_W1输入清单发现真正候选仍有功能改动未进该sandbox；单列完整diff，不把runtime冒算为11主spec中的第12项，也不擅自提交。
- 没有改变历史验收结论：本轮宏override失败只否定本轮该格，不直接声称历史真实GCC构建没跑；两个静态运行库服务端是否由其它来源提供未观测，不猜。
- 交付仅本目录及LINE_STATUS，原工作树无关修改保留。公开Git身份按人工既有确认保留；秘密、认证配置内容及二进制RPM不入库。最终安全扫描、范围与推送回执随记录。
