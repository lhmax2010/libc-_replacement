from common import *
src=TMP/'chromium-efl'
includes=[x for x in CONFIG['common'] if x.startswith('-I')]
flags=[*GNU,'-std=c++20','-O2','-g','-fPIC','-ffunction-sections','-fdata-sections','-fexceptions','-DNDEBUG','-DNET_IMPLEMENTATION','-DBASE_IMPLEMENTATION','-I'+str(src),'-I'+str(src/'tizen_src/ewk/efl_integration'),*includes]
for name,relative in [('parsed_cookie','net/cookies/parsed_cookie.cc'),('ewk_cookie_parser','tizen_src/ewk/efl_integration/public/ewk_cookie_parser.cc')]:
 row=run('W2','compile_source_'+name,[CC,*flags,'-c',src/relative,'-o',TMP/(name+'.o')],allow=(1,))
 print(name,row['exit'])
