#include "identity.h"
#include "headless.h"
#include <dali/devel-api/adaptor-framework/offscreen-window.h>
#include <dali/integration-api/scene.h>
#include <dali/public-api/actors/layer.h>
#include <dali/public-api/signals/callback.h>
#include <glib.h>
#include <chrono>
#include <thread>
#include <atomic>
std::atomic<int> delivered{-1},deliveries{0};
void deliver(int32_t frame){delivered=frame;++deliveries;}
struct CallbackLife{std::atomic<int> constructed{0},destroyed{0};};
struct TrackedCallback final:Dali::CallbackBase{
 CallbackLife&life;
 explicit TrackedCallback(CallbackLife&l):CallbackBase(reinterpret_cast<StaticFunction>(&deliver)),life(l){++life.constructed;}
 ~TrackedCallback()override{++life.destroyed;}
};
int main(int argc,char**argv){identity(argc,argv);Lifetime appLife;CallbackLife callbackLife;
 {Owned<Dali::OffscreenApplication> app(appLife,unstarted_application());identity(argc,argv);
 app->Start(); // Requires a real working Tizen EGL/platform context.
 auto scene=Dali::Integration::Scene::Get(app->GetWindow().GetRootLayer());assert(scene);
 std::unique_ptr<Dali::CallbackBase> callback(new TrackedCallback(callbackLife));
 scene.AddFrameRenderedCallback(std::move(callback),4242);assert(!callback);assert(callbackLife.destroyed==0);
 app->RenderOnce();auto end=std::chrono::steady_clock::now()+std::chrono::seconds(5);
 while(deliveries.load()==0 && std::chrono::steady_clock::now()<end){g_main_context_iteration(nullptr,false);std::this_thread::sleep_for(std::chrono::milliseconds(10));}
 assert(delivered.load()==4242 && deliveries.load()==1);
 std::cout<<"VALUES delivered="<<delivered.load()<<" deliveries="<<deliveries.load()<<'\n';app->Terminate();}
 std::cout<<"LIFETIME callback_constructed="<<callbackLife.constructed.load()<<" callback_destroyed="<<callbackLife.destroyed.load()<<'\n';assert(callbackLife.constructed.load()==1 && callbackLife.destroyed.load()==1);lifecycle(appLife,1);}
