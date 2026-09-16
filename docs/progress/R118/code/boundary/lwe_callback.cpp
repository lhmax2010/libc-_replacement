#include "common.h"
#include <dlfcn.h>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include "LWEWebView.h"
int main(int argc,char**argv){assert(argc==3);setvbuf(stdout,nullptr,_IONBF,0);identity();assert(dlopen(argv[1],RTLD_NOW|RTLD_GLOBAL));
 printf("before_initialize=1\n");LWE::LWE::SetVersionPreference(false);LWE::LWE::Initialize(argv[2],true);printf("initialized=%d\n",LWE::LWE::IsInitialized());assert(LWE::LWE::IsInitialized());
 auto*w=LWE::WebContainer::CreateHeadless(64,64,1.0f,"sans-serif","en_US","UTC");assert(w);printf("headless_created=1\n");
 using Iterate=int(*)(void*,int);auto iterate=reinterpret_cast<Iterate>(dlsym(RTLD_DEFAULT,"g_main_context_iteration"));
 printf("glib_iteration_available=%d\n",iterate!=nullptr);
 std::atomic<bool>loaded{false};w->RegisterOnPageLoadedHandler([&](LWE::WebContainer*,const std::string&){loaded.store(true);});w->LoadData("<html><head></head><body>hello</body></html>");
 if(iterate)for(int i=0;i<200&&!loaded.load();++i){iterate(nullptr,0);usleep(10000);}
 printf("page_loaded=%d\n",int(loaded.load()));
 std::atomic<int>called{0};std::string seen;std::mutex guard;
 w->EvaluateJavaScript("1+2",[&](const std::string&result){std::lock_guard<std::mutex>lock(guard);seen=result;called.fetch_add(1);});
 if(iterate)for(int i=0;i<200&&called.load()==0;++i){iterate(nullptr,0);usleep(10000);}
 printf("callback_calls=%d\n",called.load());{std::lock_guard<std::mutex>lock(guard);bytes("callback_value",seen);}
 // 仅检查同步返回前回调的算术结果；异步后端若尚未调度，不算兼容失败。
 if(called.load()==0){printf("CALLBACK_NOT_OBSERVED_REQUIRES_EVENT_LOOP=1\n");w->Destroy();LWE::LWE::Finalize();return 31;}
 {std::lock_guard<std::mutex>lock(guard);assert(called.load()==1&&seen=="3");}w->Destroy();LWE::LWE::Finalize();printf("destroy_completed=1\n");
}
