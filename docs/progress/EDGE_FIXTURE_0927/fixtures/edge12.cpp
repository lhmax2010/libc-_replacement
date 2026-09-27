#include "identity.h"
#include "headless.h"
#include <dali-toolkit/devel-api/controls/web-view/web-view.h>
#include <glib.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
struct Token{std::atomic<int>&destroyed;explicit Token(std::atomic<int>&d):destroyed(d){}~Token(){++destroyed;}};
int main(int argc,char**argv){identity(argc,argv);Lifetime appLife;std::atomic<int> calls{0},destroyed{0};std::mutex lock;std::string actual;std::weak_ptr<Token> weak;
 const std::string expected="data:text/html,%3Chtml%3E%3Cbody%3E42%3C/body%3E%3C/html%3E";
 {Owned<Dali::OffscreenApplication> app(appLife,unstarted_application());identity(argc,argv);app->Start();
 auto view=Dali::Toolkit::WebView::New();assert(view);auto token=std::make_shared<Token>(destroyed);weak=token;
 Dali::WebEnginePlugin::WebEnginePageLoadCallback callback=[token,&lock,&actual,&calls](const std::string&url){std::lock_guard<std::mutex>guard(lock);actual=url;++calls;};
 token.reset();view.RegisterPageLoadStartedCallback(std::move(callback));callback=nullptr;assert(!weak.expired());
 view.LoadUrl(expected);auto end=std::chrono::steady_clock::now()+std::chrono::seconds(5);
 while(calls.load()==0 && std::chrono::steady_clock::now()<end){g_main_context_iteration(nullptr,false);std::this_thread::sleep_for(std::chrono::milliseconds(10));}
 {std::lock_guard<std::mutex>guard(lock);assert(calls.load()==1 && actual==expected);std::cout<<"VALUES calls="<<calls.load()<<" url="<<actual<<'\n';}
 view.RegisterPageLoadStartedCallback({});view.Reset();app->Terminate();}
 assert(weak.expired() && destroyed.load()==1);std::cout<<"LIFETIME callback_token_constructed=1 callback_token_destroyed="<<destroyed.load()<<'\n';lifecycle(appLife,1);}
