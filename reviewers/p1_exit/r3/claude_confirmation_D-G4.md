# D-G4 分层与涉 boost 三轮重写单点确认(评审方:Claude,2026-07-28)

输入:`p1_exit_review_v4.tar.gz`,SHA256 实测
`e83e77aeb032b4893c19038d96447b65f34c305671bf9bae589a5a7b46406fff`(吻合);
MANIFEST 全量校验非 OK 行 = 0;整改 HEAD 与 CORPUS_HEAD 一致。

## 裁定:**CONFIRM**

GPT r2 指出的粒度断层是真实的(boost source 31 输出,二进制粒度批次
声明在 D5 source-Name 判定下不可执行),D-G4 的"构建=source /
晋级=分量闭包 / 逐输出 ADMIT|HOLD"分层是正确的解,且以机械件落地而
非仅文本。

### 与既有治理自洽性(逐项核)

- **D-G1/G2**:正交——D-G4 只加构建/晋级层,不触方法论冻结与
  member_edge_sha256 台账;晋级单元仍 = TIER1 分量闭包,执行单元闭包
  规则原语义保持(decision_record §97-103 勘误逐字核)。
- **D-G3 R2**:base_first_6.tsv 改为三层格式(source 单元 / 逐架构
  source NEVRA / ADMIT / HOLD),并如实记录 libsigc++/taglib 的
  armv7l 1.3 vs aarch64/x86_64 1.1 release 歪斜而非跨架构假定同版——
  诚实度好;Base-first = 3 source/6 ADMIT(boost 四输出含 log
  companion + libsigc++ + taglib),与 T1-0010 分量闭包一致。
- **波 1 sibling 机制**:`wave1_source_admission.tsv` 基线字节不变
  (19 路径五 source),23/26 生效前必须补 boost source 行 + D5-A04
  准入检查——fail-closed 形态与卷宗原机制同构。
- **T1-0008 两分支**:两分支下四包均整分量晋级,boost sibling 全 HOLD;
  r2 确认的"不得拆分四成员"约束在 source 层继续成立。

### 关键技术风险已被机械化处置(非宣称)

- **ADMIT 新版 与 HOLD 存量旧版同源共存的依赖歪斜**——这是 D-G4 的
  要害,已实测审计:90 个架构/输出实例中 boost 精确 EVR 锁 = 0,
  36 行 Requires/Provides 配对可解析(12 行由 HOLD_SIBLING 存量承接),
  boost-license 外部提供者单列告警;并写死执行时补救(漂移/新增精确锁
  → 同批扩张或阻塞,禁以 HOLD 绕过)。
- **promotion_ledger_template 的 RULE 行**把断言编码为机器可查:ADMIT
  行 TIER1 闭包 MUST_BE_CLOSED、HOLD 行钉存量 NEVRA+SHA 且镜像实选
  MUST_EQUAL_LEGACY、精确锁列、`.a`/devel 列(HOLD 覆盖 devel 与静态
  归档,libc++ .a 不得对 libstdc++ 消费者可见)。
- **gates 冻结载荷零差异**(一级 diff 门 15 路径白名单)——未触发
  解冻,v1.1 矩阵效力延续。

### r2"S4 回写义务"在 D-G4 语境下的完整性:**仍完整且已扩展**

decision_record 勘误段明文:"波 1 三条件裁决结果必须回写台账与
D-G3/**D-G4 晋级台账**"——回写对象从(台账标签 + D-G3 名义名单 +
排期)扩到(+ wave1_source_admission 的 boost 行(23/26 时)+ 晋级
台账 ADMIT/HOLD 行),覆盖闭合。

## notes(不影响裁定)

1. **S4 PASS 时 boost source 会在两个批次各构建一次**(9 月 Base-first、
   其后 T1-0008 批次)。第二次构建产生全输出新 NEVRA,其台账必须对
   已 ADMIT 的四个输出重新记账(随分量闭包重验后再 ADMIT,或以已晋级
   NEVRA 作为新的 legacy authority 保持 HOLD)。模板语义支持,执行时
   写清批次间的 legacy authority 交接即可。
2. `boost-license` 作为 census 外部提供者,候选仓缺失/漂移会阻塞
   solver——报告已告警,建议纳入 Base-first 批次的开工检查单一行。

**结论:CONFIRM——D-G4 与 D-G1/G2/G3、执行单元闭包规则、波 1 sibling
机制自洽;三处重写无新矛盾;S4 回写义务完整并已扩展至晋级台账。
P1_EXIT_PASS 总裁定在此修订之上继续成立。**
