# 操作与判断记录

1. 用户“好了，你试试呢”解除登录等待，未授权构建或实施修复。源码操作仅git show与tmp fetch，codes只读。
2. 人工登录后专用终端仍等待回车；仅向QB1194606-Manual-Login发送Return（raw/011），未输入/读取密码。确认无登录表单后同一context下载；未导出cookie/storageState/认证头。
3. Node18不满足最新Playwright要求，选用实查支持Node>=18的1.56.1；桌面终端Snap库冲突，最小桌面环境启动gnome-terminal.real成功。原错误/退出码保留，未改系统配置。
4. HTML与可读文本逐格交叉检查258×3，不用旧254表替代。只读额外下载本次Boost RPM确认提供方ABI；没有原位objcopy/strip。
5. raw/020_sources的cereal git show失败（该批bash末条退出0，stderr保留），单分支克隆未含目标对象；按manifest精确SHA fetch后raw/023、024、027成功。没有替换版本。
6. 三类候选diff只保存在材料目录，未应用。cereal两个互斥方向由人工选；qemu具体漏项生成环节未观测，不把/../假说写事实。
7. ICU manifest与实际checkout不符单列，不修改原数据。分支列只为预期，实际checkout SHA匹配不等于观测到运行分支名。
8. 17份日志整批>10MB，保守全部原文留tmp，入库首错前后40行、摘要、URL/SHA。原认证UI HTML留tmp，不自动入库。
9. 文档首次批量补丁因同一文件同时Delete/Add而被工具原子拒绝；无文件变更，改为独立操作后重做。此为文档编辑技术错误，不涉及构建或平台文件。
10. 仅暂存本轮目录与LINE_STATUS，保留其它既有变更，.gitignore既有编辑不纳入。仅推项目分支，包仓/sandbox/Gerrit不推。
11. 首次收尾自检与浏览器异步关闭同时发生，browser_closed尚为false导致退出1（raw/039）；其余计数、17份SHA、忽略规则与凭据形态扫描均通过。等待原关闭操作完成后重跑，不改检查判据，不读取profile内容；原FAIL记录保留。
