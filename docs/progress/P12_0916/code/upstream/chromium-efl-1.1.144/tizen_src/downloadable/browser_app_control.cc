#include "browser_app_control.h"

#include <vconf.h>

#include "dlog_util.h"

namespace samsung_next_browser {

#define csfsAppId "com.samsung.tv.csfs"
#define litewebappserviceAppId "org.tizen.litewebappservice"

AppControl::AppControl(app_control_h handle) : m_app_control_handle(handle) {}

std::string AppControl::getUri() {
  char* arg = nullptr;
  std::string uri;
  if (app_control_get_uri(m_app_control_handle, &arg) ==
      APP_CONTROL_ERROR_NONE) {
    if (arg) {
      LOG(INFO) << "Requested URI: " << arg;
      uri = arg;
      free(arg);
      arg = nullptr;
    }
  }
  uriFromFirstScreenCaller(uri);
  return uri;
}
void AppControl::uriFromFirstScreenCaller(std::string& uri) {
  std::string caller = getCaller();
  char* firstScreenPkgNamePtr = vconf_get_str("db/first_screen/type");
  std::string firstScreenPkgName;

  if (firstScreenPkgNamePtr) {
    firstScreenPkgName = firstScreenPkgNamePtr;
    free(firstScreenPkgNamePtr);
  }
  if (!firstScreenPkgName.compare(csfsAppId) &&
      !caller.compare(litewebappserviceAppId)) {
    if (!uri.empty() && uri.at(0) == '{' &&
        uri.find("{\"values\":") != std::string::npos) {
      uri = uri.substr(11, uri.length() - 13);
    }
  }
}
std::string AppControl::getExtraData(const std::string& key) {
  char* arg = nullptr;
  std::string data;
  if (app_control_get_extra_data(m_app_control_handle, key.c_str(), &arg) ==
      APP_CONTROL_ERROR_NONE) {
    if (arg) {
      LOG(INFO) << "Extradata key: " << key << " Value: " << arg;
      data = arg;
      free(arg);
      arg = nullptr;
    }
  }
  return data;
}

std::string AppControl::getCaller() {
  char* arg = nullptr;
  std::string caller;
  if (app_control_get_caller(m_app_control_handle, &arg) ==
      APP_CONTROL_ERROR_NONE) {
    if (arg) {
      caller = arg;
      free(arg);
      arg = nullptr;
    }
  }
  return caller;
}

void AppControl::SelfLaunchBrowser(std::string appId) {
  LOG(INFO) << "";
  app_control_h app_control = NULL;
  int ret = app_control_create(&app_control);
  if (ret != APP_CONTROL_ERROR_NONE) {
    LOG(INFO) << "Error creating app control";
    return;
  }

  app_control_set_operation(app_control, APP_CONTROL_OPERATION_DEFAULT);
  app_control_set_app_id(app_control, appId.c_str());
  app_control_add_extra_data(app_control, "caller_id",
                             "org.tizen.next-browser");

  app_control_add_extra_data(app_control, "pin_launched", "1");

  ret = app_control_send_launch_request(app_control, NULL, NULL);
  if (ret == APP_CONTROL_ERROR_NONE) {
    LOG(INFO) << "Successfully launched the app";
  } else {
    LOG(INFO) << "Launch fail";
  }
  app_control_destroy(app_control);
}

}  // namespace samsung_next_browser
