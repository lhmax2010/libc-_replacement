# QuickBuild #1194606 状态（2026-10-13）

**ANALYSIS_COMPLETE / BUILD_FAILED，等待人工审阅。** 已完成登录后抓取与分析：258包×3架构，752 succeeded、9 failed、13 excluded。九格失败涉及cereal、icu、三个qemu-accel变体；原始17份日志下载及SHA核对完成。详见FINAL_RESULT.md、BUILD_STATUS.tsv、FAILURES.tsv。候选diff未实施；项目推送回执见PUSH_RECEIPT.md。

下文为开工登录准备的历史记录，不再表示尚未登录。收尾关闭浏览器并保留profile，状态见BROWSER_ACTIONS.json最后操作。

已用 Playwright persistent context 打开可见 Chromium（headless=false），页面为 https://quickbuild.tizen.org/build/1194606/gbs_reports 。桌面终端标题 `QB1194606-Manual-Login`，提示人工在浏览器以 lhmax2025 登录，然后在该终端按回车。不会读取登录表单值或记录终端输入；最多检查 3 次。

profile：`tmp/QB_1194606/browser-profile/`，已由 git ignore 排除，保留供后续复用；未调用 cookies/storageState 等导出接口。浏览器脚本 `browser_session.cjs`，状态文件 `tmp/QB_1194606/browser-status.json`。登录确认后自动执行已排队的只读 gbs_reports 抓取，下载原文先留在 `tmp/QB_1194606/downloads/`，未审查的 HTML/链接不会自动入库。

资源 light 门禁退出 0，磁盘可用 227 GiB。依赖全部安装在本轮 tmp：Node 18 下最新 Playwright 要求 Node 20，已记录警告，改用实查支持 Node >=18 的 playwright-core 1.56.1 与其 Chromium 141.0.7390.37。未变更系统 Node 或配置。

桌面终端首次启动遇到 Snap 库符号冲突，去除两个 LD 变量仍失败；改为只继承必要桌面/用户环境的 gnome-terminal.real 后退出 0，浏览器正常显示并进入人工等待。命令与退出码见 raw/001–009。

全程没有构建、上板、包仓/sandbox推送、新QuickBuild或codes修改。仅项目报告按授权提交推送，原始大日志及profile留tmp、不入库。
