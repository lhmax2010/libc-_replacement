from common import *
gate()
root=TMP/'root';inv=json.loads((OUT/'INVENTORY.json').read_text());rpms=json.loads((OUT/'RPMS.json').read_text())
owner={}
for r in rpms:
 for p in (ROOT/(r['file_list_record']+'.stdout')).read_text().splitlines():owner.setdefault(p,[]).append(r['name'])
headers={
1:[('appcore_cpp/app_core_base.hh','AddEvent'),('appcore_cpp/interface_main_loop.hh','OnLoop')],
2:[('bundle_cpp.h','int Add(')],3:[('bundle_cpp.h','initializer_list')],
4:[('scim-1.0/scim_utility.h','utf8_wcstombs')],5:[('jsoncpp/json/reader.h','bool parse(')],
6:[('absl/base/internal/spinlock_wait.h','SpinLockWait('),('absl/base/call_once.h','SpinLockWait(')],
7:[('dali/devel-api/adaptor-framework/actor-accessible.h','GetMatches('),('dali/devel-api/atspi-interfaces/collection.h','using MatchRule')],
8:[('dali/integration-api/scene.h','AddFrameRenderedCallback'),('dali/integration-api/scene.h','static Scene New(')],
9:[('gtest/internal/gtest-string.h','StringStreamToString'),('gtest/gtest.h','StringStreamToString')],
10:[('delta/delta_parser.h','ParseManifest'),('delta/delta_handler.h','added()')],
11:[('cert-svc/vcore/SignatureValidator.h','checkList(')],
12:[('dali-toolkit/devel-api/controls/web-view/web-view.h','RegisterPageLoadStartedCallback')],
13:[('dali/devel-api/common/hash.h','CalculateHash')],14:[('dali/devel-api/threading/conditional-wait.h','WaitUntil')],
15:[('dali/devel-api/common/singleton-service.h','GetSingleton'),('dali/devel-api/common/singleton-service.h','Application')],
16:[('media/inference_engine_common.h','GetInputTensorBuffers'),('media/inference_engine_common_impl.h','BindBackend'),('media/inference_engine_common_impl.h','GetInputTensorBuffers')],
17:[('gtest/gtest-printers.h','PrintStringTo')],18:[('notification-ex/shared_file.h','SetPrivateSharing')],
19:[('zypp/ResPool.h','setRequestedLocales')],20:[('zypp/ZConfig.h','multiversionSpec')],21:[('zypp/CheckSum.h','std::istream & input_r')],
22:[('dali/devel-api/threading/conditional-wait.h','WaitUntil')],23:[('zypp/parser/xml/Reader.h','Reader( const InputStream'),('zypp/base/InputStream.h','InputStream(')]}
out=[]
for e,items in headers.items():
 for path,needle in items:
  p=root/'usr/include'/path;lines=p.read_text().splitlines();hits=[n for n,s in enumerate(lines) if needle in s];assert hits,(path,needle)
  excerpts=[dict(line=n+1,text='\n'.join(f'{k+1}: {lines[k]}' for k in range(max(0,n-4),min(len(lines),n+12)))) for n in hits]
  out.append(dict(edge=e,path='usr/include/'+path,sha256=sha(p),owners=owner.get('/usr/include/'+path,[]),needle=needle,excerpts=excerpts))
save('DECLARATIONS.json',out)
loader=root/'lib64/ld-linux-x86-64.so.2';libpath=':'.join(str(root/p) for p in ('usr/lib64','lib64','usr/lib','lib','usr/lib64/hal','usr/lib/hal'))
checks=[]
for e in [7,8,11,12,16,18]:
 gate();r=inv[e-1];p=ROOT/r['provider']
 x=run('provider_loader_'+str(e),[loader,'--library-path',libpath,'--list',p],check=False)
 checks.append(dict(edge=e,provider=r['provider'],sha256=sha(p),loader_record=x['record'],exit=x['exit'],stdout=x['stdout'],stderr=x['stderr']))
save('LOADER_PREFLIGHT.json',checks)
r=inv[6];nm=(ROOT/(r['symbol_record']+'.stdout')).read_text()
actual=[s for s in nm.splitlines() if 'ActorAccessible10GetMatchesE' in s]
assert actual,'Positive control for current GetMatches export must exist'
assert not r['symbol_present']
save('EDGE7_SYMBOL_CHANGE.json',dict(expected=r['expected_symbol'],current_definitions=actual,provider=r['provider'],sha256=r['provider_sha256'],record=r['symbol_record'],conclusion='Current provider has a different GetMatches export; no substitution for the registered symbol'))
print('DECLARATIONS',len(out),'LOADER',[(r['edge'],r['exit']) for r in checks]);print('EDGE7',actual)
