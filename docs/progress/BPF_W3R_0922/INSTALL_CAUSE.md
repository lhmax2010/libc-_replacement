# 一、正常安装预检阻断归因

**已确认的环境条件**：上传目录 `/var/tmp/bpf_static_0922/` 实际为 `/opt/var/tmp/bpf_static_0922/`，位于 `/dev/mmcblk0p3` 的 **rw ext4**；安装目标 `/usr/bin` 与策略文件 `/etc/device-sec-policy` 则位于 `/dev/mmcblk0p2` 的 **ro ext4 根分区**。上传成功与安装预检失败并不矛盾。

| 核对项 | 观测与结论 | 依据 |
|---|---|---|
| 三重身份 | rpi4内核、armv7l、Tizen ID均匹配；不按IP识别 | board-read/IDENTITY.json；board-test/kernel、machine、os.transport.out |
| 根分区空闲 | df仍报1,393,876 KiB；只读并不使df物理空闲变零 | board-root-read/df.out、mounts.out |
| RPM可用块数 | 固定源码在ST_RDONLY时将bavail设为0 | upstream-transaction.c:164 |
| 报错条件 | adj_fs_blocks(bneeded)>bavail，增加RPMPROB_DISKSPACE | upstream-transaction.c:269–272 |
| 显示消息 | needs…filesystem来自RPMPROB_DISKSPACE，向上取整到MiB，文案为MB | upstream-rpmprob.c:143–151 |
| 92MB与Size | Size=90,882,270字节，约86.672MiB；不等于92MiB=96,468,992字节 | board-root-read/rpm-info.out、rpm-size.out |
| 原因匹配 | 已确认只读挂载符合固定源码“按0可用块计算”的条件；不是df所示物理空间不足 | 上述观测+源码 |
| MSM附带报错 | /etc同属ro根分区，不能写入；本轮未追踪MSM内部返回路径，不另行断言插件缺陷 | board-root-read/resolve.out；前轮INSTALL_PREFLIGHT.json |

`adj_fs_blocks` 在 transaction.c:68 对预计用量乘21/20（5%余量），再按文件系统块计数；消息再向上取整。因而92MB不是包头Size的原样打印。按普通文件向上取4096字节块、加5%及MiB向上取整可得到92的量级；这是算术旁证，未声称读取到当时DSI内存值。

## 版本与源码身份边界

任务描述中的“同一份rpm 4.14.1.1源码”需修正：ARM_RPM_DIAG实际固定的是上游 **rpm-4.14.1** 提交 `bfee1410af51c1cc9724791fb8d985260a62102b`，旧INTERIM_RESULT.md:49已注明并非部署Tizen源码的完整身份证明。板上 `rpm --version` 实测也是4.14.1。本轮继续用同一提交，不冒充另外的4.14.1.1 tag；未证明部署二进制与该源码逐字节对应。归因的“已确认”指实际ro条件与指定源码条件对上，不是本轮插桩读到了dsi->bavail。没有重跑安装预检。

## 挂载映射

| 路径 | 实际路径 | 挂载源 / 挂载点 | 类型 | ro/rw |
|---|---|---|---|---|
| / | / | /dev/mmcblk0p2 / / | ext4 | ro |
| /etc | /etc | /dev/mmcblk0p2 / / | ext4 | ro |
| /usr | /usr | /dev/mmcblk0p2 / / | ext4 | ro |
| /opt | /opt | /dev/mmcblk0p3 / /opt | ext4 | rw |
| /tmp | /tmp | tmpfs / /tmp | tmpfs | rw |
| /var/tmp | /opt/var/tmp | /dev/mmcblk0p3 / /opt | ext4 | rw |

### DSI初始化

来源：[固定源码](https://github.com/rpm-software-management/rpm/blob/bfee1410af51c1cc9724791fb8d985260a62102b/lib/transaction.c#L110)；本地SHA `d4c3742e34a6a4613795b997672c2376b4c21dcd27dcf57271cc197a47841019`。

```c
110: static int rpmtsInitDSI(const rpmts ts)
111: {
112:     if (rpmtsFilterFlags(ts) & RPMPROB_FILTER_DISKSPACE)
113: 	return 0;
114:     ts->dsi = _free(ts->dsi);
115:     ts->dsi = xcalloc(1, sizeof(*ts->dsi));
116:     return 0;
117: }
```

### 统计与只读处理

来源：[固定源码](https://github.com/rpm-software-management/rpm/blob/bfee1410af51c1cc9724791fb8d985260a62102b/lib/transaction.c#L157)；本地SHA `d4c3742e34a6a4613795b997672c2376b4c21dcd27dcf57271cc197a47841019`。

```c
157:     dsi->dev = sb.st_dev;
158:     dsi->bsize = sfb.f_bsize;
159:     if (!dsi->bsize)
160: 	dsi->bsize = 512;       /* we need a bsize */
161:     dsi->bneeded = 0;
162:     dsi->ineeded = 0;
163: #ifdef STATFS_HAS_F_BAVAIL
164:     dsi->bavail = (sfb.f_flag & ST_RDONLY) ? 0 : sfb.f_bavail;
165: #else
166: /* FIXME: the statfs struct doesn't have a member to tell how many blocks are
167:  * available for non-superusers.  f_blocks - f_bfree is probably too big, but
168:  * it's about all we can do.
169:  */
170:     dsi->bavail = sfb.f_blocks - sfb.f_bfree;
```

### 问题判定

来源：[固定源码](https://github.com/rpm-software-management/rpm/blob/bfee1410af51c1cc9724791fb8d985260a62102b/lib/transaction.c#L260)；本地SHA `d4c3742e34a6a4613795b997672c2376b4c21dcd27dcf57271cc197a47841019`。

```c
260: static void rpmtsCheckDSIProblems(const rpmts ts, const rpmte te)
261: {
262:     rpmDiskSpaceInfo dsi = ts->dsi;
263: 
264:     if (dsi == NULL || !dsi->bsize)
265: 	return;
266: 
267:     for (; dsi->bsize; dsi++) {
268: 
269: 	if (dsi->bavail >= 0 && adj_fs_blocks(dsi->bneeded) > dsi->bavail) {
270: 	    if (dsi->bneeded > dsi->obneeded) {
271: 		rpmteAddProblem(te, RPMPROB_DISKSPACE, NULL, dsi->mntPoint,
272: 		   (adj_fs_blocks(dsi->bneeded) - dsi->bavail) * dsi->bsize);
273: 		dsi->obneeded = dsi->bneeded;
274: 	    }
```

### 消息类型与显示单位

来源：[固定源码](https://github.com/rpm-software-management/rpm/blob/bfee1410af51c1cc9724791fb8d985260a62102b/lib/rpmprob.c#L143)；本地SHA `c63a8c2f8760c599fc6ccf88247c27494e065874a7a1d5ae2fa42f555b60f03e`。

```c
143:     case RPMPROB_DISKSPACE:
144: 	rasprintf(&buf,
145: 	    _("installing package %s needs %" PRIu64 "%cB on the %s filesystem"),
146: 		pkgNEVR,
147: 		prob->num1 > (1024*1024)
148: 		    ? (prob->num1 + 1024 * 1024 - 1) / (1024 * 1024)
149: 		    : (prob->num1 + 1023) / 1024,
150: 		prob->num1 > (1024*1024) ? 'M' : 'K',
151: 		str1);
```

### 额外5%余量

来源：[固定源码](https://github.com/rpm-software-management/rpm/blob/bfee1410af51c1cc9724791fb8d985260a62102b/lib/transaction.c#L68)；本地SHA `d4c3742e34a6a4613795b997672c2376b4c21dcd27dcf57271cc197a47841019`。

```c
68: #define	adj_fs_blocks(_nb)	(((_nb) * 21) / 20)
```
