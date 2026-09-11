/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WIDGET_BASE_HH_
#define WIDGET_BASE_HH_

#include <app_common.h>

#include <memory>
#include <string>

#include <app_core_multi_window_base.hh>

#ifndef EXPORT_API
#define EXPORT_API __attribute__((visibility("default")))
#endif

namespace tizen_cpp {

class EXPORT_API WidgetBase : public AppCoreMultiWindowBase {
 public:
  WidgetBase();
  virtual ~WidgetBase();

  std::string GetViewerEndpoint();
  void Run(int argc, char** argv) override;
  void Exit() override;
  int OnControl(tizen_base::Bundle b) override;
  int OnCreate() override;
  int OnTerminate() override;
  int OnReceive(aul_type type, tizen_base::Bundle b) override;
  void ExitContext(std::shared_ptr<Context> context) override;

 protected:
  void Dispose() override;

 private:
  friend class WidgetContext;
  class Impl;
  std::unique_ptr<Impl> impl_;
};

class EXPORT_API WidgetContext : public AppCoreMultiWindowBase::Context {
 public:
  enum class DestroyType {
    PERMANENT,
    TEMPORARY
  };

  WidgetContext(std::string context_id, std::string inst_id,
      AppCoreMultiWindowBase* app);
  virtual ~WidgetContext();
  virtual bool OnCreate(const tizen_base::Bundle& contents, int w, int h) = 0;
  virtual void OnDestroy(DestroyType reason,
      const tizen_base::Bundle& contents) = 0;
  virtual void OnPause();
  virtual void OnResume();
  virtual void OnResize(int w, int h);
  virtual void OnUpdate(const tizen_base::Bundle& contents, bool force);
  void ExitAsync();
  int SetContents(const tizen_base::Bundle& contents);
  int WindowBind(std::string id, void* wl_win);

 private:
  using AppCoreMultiWindowBase::Context::WindowBind;
  void OnCreate() override;
  void OnTerminate() override;

 private:
  friend class WidgetBase;
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace tizen_cpp

#endif  // WIDGET_BASE_HH_
