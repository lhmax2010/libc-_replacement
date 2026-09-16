#include "common.h"
#include "LWEWebView.h"
#include <dlfcn.h>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <chrono>
#include <cstring>

static void maps() {
    FILE* f=fopen("/proc/self/maps","r"); assert(f);
    char line[4096]; bool provider=false,gnu=false;
    while(fgets(line,sizeof line,f)) {
        if(strstr(line,"liblightweight-web-engine") || strstr(line,"/libstdc++.") || strstr(line,"/libc++.")) {
            printf("LOADED %s",line);
            provider |= strstr(line,"mobile-impl.so") != nullptr;
            gnu |= strstr(line,"/libstdc++.so") != nullptr;
        }
    }
    fclose(f); printf("provider_mapped=%d gnu_mapped=%d\n",provider,gnu);
    assert(provider && gnu);
}

int main(int argc,char**argv) {
    assert(argc==4); setvbuf(stdout,nullptr,_IONBF,0); identity();
    if(!dlopen(argv[1],RTLD_NOW|RTLD_GLOBAL)) {
        const char* error=dlerror();
        fprintf(stderr,"provider_load_error=%s\n",error?error:"NOT_OBSERVED");
        return 42; // 环境前置失败，不用断言制造额外崩溃诊断。
    }
    LWE::LWE::SetVersionPreference(false);
    LWE::LWE::Initialize(argv[2],true);
    assert(LWE::LWE::IsInitialized());
    auto*w=LWE::WebContainer::CreateHeadless(64,64,1.0f,"sans-serif","en_US","UTC");
    assert(w); maps();
    using Iterate=int(*)(void*,int);
    auto iterate=reinterpret_cast<Iterate>(dlsym(RTLD_DEFAULT,"g_main_context_iteration")); assert(iterate);
    std::atomic<int> render_queries{0};
    if(std::strcmp(argv[3],"no-render")==0) {
        // Renderer.cpp:418/464 gate rendering; headless creation does not install a drawing buffer.
        w->RegisterCanRenderingHandler([&](LWE::WebContainer* view){assert(view==w); ++render_queries; return false;});
    }
    std::atomic<bool> loaded{false};
    w->RegisterOnPageLoadedHandler([&](LWE::WebContainer* view,const std::string&){assert(view==w);loaded=true;});
    w->LoadData("<html><head></head><body>hello</body></html>");
    auto pump=[&](auto ready){
        auto end=std::chrono::steady_clock::now()+std::chrono::seconds(8);
        while(!ready() && std::chrono::steady_clock::now()<end){iterate(nullptr,0);usleep(1000);}
        return ready();
    };
    bool page=pump([&]{return loaded.load();}); printf("page_loaded=%d render_queries=%d\n",page,render_queries.load());
    assert(page);
    std::atomic<int> called{0}; std::string seen; std::mutex lock;
    w->EvaluateJavaScript("1+2",[&](const std::string& value){std::lock_guard<std::mutex> guard(lock);seen=value;++called;});
    bool completed=pump([&]{return called.load()!=0;});
    {std::lock_guard<std::mutex> guard(lock);printf("callback_calls=%d\n",called.load());bytes("callback_value",seen);assert(completed&&called==1&&seen=="3");}
    w->Destroy(); LWE::LWE::Finalize(); printf("destroy_completed=1\n");
}
