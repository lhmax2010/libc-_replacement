#include "identity.h"
#include "headless.h"
#include <dali/devel-api/adaptor-framework/offscreen-window.h>
#include <dali/integration-api/scene.h>
#include <dali/public-api/actors/layer.h>
#include <dali/public-api/signals/callback.h>
int delivered=-1,deliveries=0;
void deliver(int32_t frame){delivered=frame;++deliveries;}
struct TrackedCallback final:Dali::CallbackBase{
 Lifetime&life;
 explicit TrackedCallback(Lifetime&l):CallbackBase(reinterpret_cast<StaticFunction>(&deliver)),life(l){++life.constructed;}
 ~TrackedCallback()override{++life.destroyed;}
};
int main(int argc,char**argv){identity(argc,argv);Lifetime appLife,callbackLife;
 {Owned<Dali::OffscreenApplication> app(appLife,unstarted_application());identity(argc,argv);
 auto scene=Dali::Integration::Scene::Get(app->GetWindow().GetRootLayer());assert(scene);
 std::unique_ptr<Dali::CallbackBase> callback(new TrackedCallback(callbackLife));auto original=callback.get();
 scene.AddFrameRenderedCallback(std::move(callback),4242);assert(!callback);assert(callbackLife.destroyed==0);
 Dali::Integration::Scene::FrameCallbackContainer out;scene.GetFrameRenderedCallback(out);assert(out.size()==1);assert(out[0].first.get()==original && out[0].second==4242);
 // Public extraction/dispatch validates the stored callback and ownership. This
 // is NOT a claim that a graphics driver generated a completed frame.
 Dali::CallbackBase::Execute(*out[0].first,out[0].second);assert(delivered==4242 && deliveries==1);
 std::cout<<"VALUES registered_count="<<out.size()<<" frame="<<out[0].second<<" delivered="<<delivered<<" deliveries="<<deliveries<<'\n';
 out.clear();assert(callbackLife.destroyed==1);Dali::Integration::Scene::FrameCallbackContainer again;scene.GetFrameRenderedCallback(again);assert(again.empty());}
 lifecycle(callbackLife,1);lifecycle(appLife,1);}
