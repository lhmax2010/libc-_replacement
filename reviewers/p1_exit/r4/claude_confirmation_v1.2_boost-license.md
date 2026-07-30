# gates v1.2 重冻 + boost-license ADMIT 单点确认(评审方:Claude,2026-07-28)

输入:`p1_exit_review_v5.tar.gz`,SHA256 实测
`d379012baf8206d976a6f5e121caf72709fd2914f5d6ae34f392cbdbb97e541c`(吻合);
MANIFEST 全量校验非 OK 行 = 0。

## 裁定:**CONFIRM**

### 1. v1.2 解冻重冻:合规(经本地实测)

- **全矩阵非增量**:33 = 旧 27 + 新 6,verdict_matrix 独立计数
  4 GREEN + 29 RED 与冻结页一致;
- **旧三工具 SHA 不变**:edge_diff 1.1.0 SHA
  `3e06d9e5…f74` 与本方 v2 复核时亲录的值**逐字节相同**(独立可证);
  G6/G7 版本号未动,一级 diff 门(49 路径白名单)断言旧工具零差异;
- **108 项冻结清单自洽**:FREEZE_MANIFEST 实数 108 行;
- **新门六 fixture 全部亲测**:clean GREEN/0;五红逐一命中
  `LEDGER_MISSING_OUTPUT / HOLD_AUTHORITY_DRIFT / ADMIT_CLOSURE_BROKEN /
  EXACT_LOCK_SPLIT / LEDGER_DUPLICATE_ROW`(exit 2);本方初次传错文件名
  时工具以 exit 3 拒绝——顺带实证了 fail-closed 输入路径;
- **真实台账实例过门**:Base-first 三架构实例本方亲跑
  `GATE=GREEN blocking=0`(与报告三实例 GREEN 声明一致)。

GPT r3 的 noarch 盲区修正(31/28/31 → 33/30/33)连同"验证器断言必须与
数据同源生成,禁止硬编码预期"的教训落账,方向正确。

### 2. boost-license 三场景 ADMIT:论证成立,建议一处措辞升格

重分类证据链完整:NEVRA/SOURCERPM/SHA 双源独立复验(下载 SHA 与
repodata SHA 一致)、文件清单仅 License 文本、非 ELF、无 C++ 面;且它
是其余 boost 输出的 Requires 提供者(R3 依赖配对中 15 行),三场景不
ADMIT 反而会阻塞 solver。台账实现也没有偷懒沿用闭包语义——分量字段
用了独立标记 `SHARED_NOARCH_NO_CPP:boost-license`。

**措辞建议(非 OBJECT)**:该标记目前是实例层写法,建议在 D-G4 条款中
升格为命名判据,例如 `ADMIT_STDLIB_NEUTRAL`,三条准入要件写死:
(a) 非 ELF(文件清单机械证明);(b) 无 C++ 面;(c) 内容 SHA 双源复验
一致。适用语义 = 可随任意批次 ADMIT、不占用也不豁免 TIER1 分量闭包
断言;同源文档类输出(boost-doc-html)按同判据反向 HOLD。这样第四支
门禁对该类行的放行依据是显式规则而非逐例注记,后续 source(如
含 noarch 数据包的其他 source)可直接复用。

### 3. note 1 落账:完整,且强于原建议

decision_record :101-104 与 :140-141 实核:跨批 authority 交接二选一
("重验闭包再 ADMIT" / "前批晋级 NEVRA+SHA 为新 authority 保持
HOLD_SIBLING")逐字在案;未选择、未登记、镜像实选 SHA 不符**均阻塞**;
外加"禁止两批次对同一输出各自宣称不同 authority"与 26 分支生效时
同步回写 Base-first 台账的义务——比本方 note 1 的原始表述多出阻塞
语义与双批一致性禁令,更严。boost-license 候选仓可用性与
dotnet-launcher→boost-filesystem 的 D1a 核查也已入 Base-first 开工
检查单(note 2 同批闭合)。

## 结论

**CONFIRM**——v1.2 重冻合规(全矩阵、旧工具字节不变、108 清单自洽,
关键断言全部本地实测);boost-license ADMIT 论证成立,附一条判据命名
升格建议供 P1 后采纳;note 1 落账完整。P1_EXIT_PASS 总裁定继续成立。
