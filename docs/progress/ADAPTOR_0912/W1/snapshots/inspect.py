#!/usr/bin/env python3
"""只读定界搜索，输出实际路径、行号及匹配上下文。"""
import pathlib, subprocess, sys
r=pathlib.Path.cwd(); ext=pathlib.Path('/home/toolchain/development/libc++_replacement')
def run(tag,cmd):
    return subprocess.run([sys.executable,str(r/'docs/progress/API_0911/record.py'),str(r/'docs/progress/ADAPTOR_0912/W1/raw'/tag),*cmd]).returncode
run('003_board', ['timeout','20','sdb','-s','192.168.108.26:26101','shell',
    'uname -a; test ! -e /opt/usr/home/owner/share/tmp/api_w1_0911_1050; echo PREVIOUS_DIR_ABSENT_RC=$?; ls -ld /opt/usr/home/owner/share/tmp/*api* /opt/usr/home/owner/share/tmp/*adaptor* 2>/dev/null; ps -ef; ls -la /run/user /run/wayland* /tmp/.X11-unix 2>/dev/null; echo READ_ONLY_CHECK_END'])
run('004_source_paths',['bash','-c', 'rg --files --no-ignore '+str(ext/'tmp/corpus/extracted')+' | rg "(chromium-efl.*spec$|dali2-adaptor.*(offscreen|headless|CMakeLists|spec$)|webapi-plugins.*(test|spec$|extension.h$|extension.cc$))" | head -180'])
run('005_cookie_source',['bash','-c', 'nl -ba tmp/API_0911/W1_CONT/chromium_source/chromium-efl-1.1.144/tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc; for f in tmp/API_0911/W1_CONT/chromium_extension_source/chromium-efl-1.1.144/net/cookies/{parsed_cookie,cookie_util}.{cc,h}; do echo "$f"; sed -n "1,100p" "$f"; done'])
run('006_dali_offscreen',['rg','-n','--no-ignore','OffscreenApplication|OFFSCREEN|HEADLESS|headless|GraphicsBackend|GetEnvironmentVariable',str(ext/'tmp/corpus/extracted/dali2-adaptor-2.5.26-1.src.rpm/unpacked/dali2-adaptor-2.5.26.tar.gz/dali2-adaptor-2.5.26/build'),str(ext/'tmp/corpus/extracted/dali2-adaptor-2.5.26-1.src.rpm/unpacked/dali2-adaptor-2.5.26.tar.gz/dali2-adaptor-2.5.26/dali/devel-api/adaptor-framework/offscreen-application.h')])
