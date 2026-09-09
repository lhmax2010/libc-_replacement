#!/usr/bin/env python3
"""仅轮询本机编译线文档；不连接开发板。30 分钟间隔，六次。"""
import datetime,json,pathlib,subprocess,sys,time
root=pathlib.Path.cwd()
out=root/'docs/progress/IMPL_0909/W2'
first=json.loads((out/'raw/002_release_check_0.time.json').read_text())
start=datetime.datetime.fromisoformat(first['start']).timestamp()
paths=[root/'docs/progress/BUILD_STATUS_0909.md',
       pathlib.Path('/home/toolchain/development/libc++_replacement/docs/progress/BUILD_STATUS_0909.md')]
rec=root/'docs/progress/R114/code/record.py'
for attempt in range(1,7):
    due=start+1800*attempt
    while time.time()<due:
        remaining=due-time.time()
        print('WAITING_FOR_BOARD next_check_utc='+datetime.datetime.fromtimestamp(due,datetime.timezone.utc).isoformat()+
              ' seconds_remaining='+str(int(remaining)),flush=True)
        time.sleep(min(60,remaining))
    command=[sys.executable,'-c',
        'import pathlib,sys; [(print("PATH="+s),print(pathlib.Path(s).read_text() if pathlib.Path(s).is_file() else "NOT_AVAILABLE")) for s in sys.argv[1:]]',
        *map(str,paths)]
    stem=out/'raw'/f'release_poll_{attempt}'
    result=subprocess.run([sys.executable,str(rec),str(stem),'timeout','20',*command])
    text=pathlib.Path(str(stem)+'.stdout').read_text()
    candidate=result.returncode==0 and ('板子已释放' in text or '板子已清理并释放' in text)
    print('RELEASE_CHECK',attempt,'candidate='+str(candidate),flush=True)
    if candidate:
        print('REVIEW_REQUIRED: 文本可能是引用或待办，须人工阅读确认后才能使用板子。',flush=True)
        sys.exit(0)
print('WAIT_LIMIT_REACHED: 六次重试结束；W2 记 WAITING_FOR_BOARD，转 W3，包尾再查一次。',flush=True)
