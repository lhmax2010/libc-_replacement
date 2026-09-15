// Copyright 2019 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __BROWSER_APP_CONTROL__
#define __BROWSER_APP_CONTROL__

#include <appfw/app.h>
#include <string>
namespace samsung_next_browser {
class AppControl {
 public:
  explicit AppControl(app_control_h handle);
  std::string getUri();
  void uriFromFirstScreenCaller(std::string& uri);
  std::string getExtraData(const std::string& key);
  std::string getCaller();
  void SelfLaunchBrowser(std::string appId);

 private:
  app_control_h m_app_control_handle;
};
}  // namespace samsung_next_browser
#endif  //__BROWSER_APP_CONTROL__
