#include "browser_app_interface.h"

#include <appfw/app.h>
#include <capi-system-info/system_info.h>
#include <vconf.h>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include <Ecore.h>
#include <smart-deadlock.h>
#include "browser_app_control.h"
#include "dlog_util.h"
#include "ewk_interface_main.h"

constexpr char B_VCONFKEY_SHOULD_PRELOAD[] =
    "memory/browser_service/should_preload";

namespace samsung_next_browser {

static bool setShouldPreload(int state);
static int getLowMemState();

static bool browser_window_shown = false;
const int watchdogTime = 200;
const float smdlNotifyInterval = 16.0;
static Ecore_Timer* smdlNotifyTimer = nullptr;

struct serviceChecker {
  std::string requestURI;
  std::string chromiumArguments;
  std::string caller;
  bool preLoading;
  bool bg_launch_enabled;

  explicit serviceChecker(app_control_h service)
      : requestURI(""),
        chromiumArguments(""),
        preLoading(false),
        bg_launch_enabled(false) {
    AppControl app_control(service);
    std::string data;

    data = app_control.getUri();
    if (!data.empty()) {
      requestURI = std::move(data);
      LOG(INFO) << requestURI;
    }

    data = app_control.getExtraData("ChromiumArguments");
    if (!data.empty()) {
      chromiumArguments = std::move(data);
      LOG(INFO) << chromiumArguments;
    }

    data = app_control.getExtraData("Preloading");
    if (!data.empty() && strcmp(data.c_str(), "true") == 0) {
      preLoading = true;
    }

    data = app_control.getExtraData("PAYLOAD");
    if (!data.empty()) {
      requestURI = std::move(data);
      LOG(INFO) << "payload: " << requestURI;
    }

    data = app_control.getCaller();
    if (!data.empty()) {
      caller = std::move(data);
      LOG(INFO) << "Caller: " << caller;
    }

    data = app_control.getExtraData("__K_BG_LAUNCH");
    if (!data.empty() && strcmp(data.c_str(), "enable") == 0) {
      LOG(INFO) << "BG LAUNCH ENABLED!";
      bg_launch_enabled = true;
      preLoading = true;
    }
  }
};

enum class DDR_MEMORY_SIZE {
  DDR_1_25G,
  DDR_1_5G,
  DDR_2G,
  DDR_2_5G,
  DDR_3G,
  DDR_1G,
  DDR_5G,
  DDR_4_5G,
  DDR_3_5G,
  DDR_4G,
  DDR_6G
};

int GetDDRInfo(bool* returnCode) {
  int value = 0;
  if (system_info_get_custom_int("com.samsung/featureconf/ddr_memory_size",
                                 &value) != SYSTEM_INFO_ERROR_NONE) {
    LOG(INFO) << ("Failed to get ddr value");
    if (returnCode) {
      (*returnCode) = false;
    }
    return -1;
  }
  LOG(INFO) << "DDR value = " << value;
  if (returnCode) {
    (*returnCode) = true;
  }
  return value;
}

std::string getTVLanguage() {
  char* langset = vconf_get_str(VCONFKEY_LANGSET);
  if (!langset) {
    return "";
  }
  std::string lang = langset;
  free(langset);

  // xy_ZW.UTF-8 -> xy-ZW
  lang = lang.substr(0, 5);
  if (lang.length() >= 3 && lang[2] == '_')
    lang[2] = '-';

  // xy-ZW -> xy
  if (!(lang.starts_with("en") || lang.starts_with("fr") ||
        lang.starts_with("pt") || lang.starts_with("zh"))) {
    lang = lang.substr(0, 2);
  }

  return lang;
}

void passDefaultArguments(std::vector<std::string>& defaultArgumentsList) {
  defaultArgumentsList.push_back(
      "/opt/usr/apps/org.tizen.next-browser/bin/browser");
  defaultArgumentsList.push_back("--ignore-gpu-blocklist");
  defaultArgumentsList.push_back("--ozone-platform=efl");
  defaultArgumentsList.push_back("--in-process-gpu");
  defaultArgumentsList.push_back("--enable-logging=stderr");
  defaultArgumentsList.push_back("--no-sandbox");
  defaultArgumentsList.push_back("--use-internal-popup-menu=true");
  defaultArgumentsList.push_back(
      "--user-data-dir=/opt/usr/home/owner/apps_rw/org.tizen.next-browser/"
      "data");
  defaultArgumentsList.push_back("--disk-cache-size=52428800");
  defaultArgumentsList.push_back(
      "--disk-cache-dir=/opt/usr/home/owner/apps_rw/org.tizen.next-browser/"
      "data");
  defaultArgumentsList.push_back(
      "--enable-features=StorageQuotaSettings:PoolSizeRatio/0.0.3");
  defaultArgumentsList.push_back("--renderer-process-limit=1");
  defaultArgumentsList.push_back("--disable-site-isolation-trials");
  defaultArgumentsList.push_back("--widget-id=org.tizen.next-browser");
  defaultArgumentsList.push_back(
      "--disable-features="
      "ScreenAIOCREnabled,"
      "ScreenAIMainContentExtractionEnabled,"
      "OptimizationHints,"
      "OptimizationGuideService,"
      "OptimizationHintsFetching,"
      "OptimizationHintsFetchingAnonymousDataConsent,"
      "OptimizationTargetPrediction,"
      "OptimizationGuideModelDownloading");
  defaultArgumentsList.push_back("--disable-component-update");
  std::string lang = getTVLanguage();
  if (!lang.empty())
    defaultArgumentsList.push_back("--lang=" + lang);
}

void passBrowserLiteArguments(std::vector<std::string>& defaultArgumentsList) {
  defaultArgumentsList.push_back("--window-size=1280,720");
  defaultArgumentsList.push_back(
      "--user-agent=Mozilla/5.0 (SMART-TV; Linux; Tizen 8.0) "
      "AppleWebKit/537.36 (KHTML, like Gecko) SamsungBrowser/7.0 "
      "Chrome/120.0.0.0  Mobile Safari/537.36");
  defaultArgumentsList.push_back("--browser-lite");
}

void passNextBrowserArguments(std::vector<std::string>& defaultArgumentsList) {
  defaultArgumentsList.push_back("--window-size=1920,1080");
  defaultArgumentsList.push_back(
      "--user-agent=Mozilla/5.0 (SMART-TV; Linux; Tizen 10.1) "
      "AppleWebKit/537.36 (KHTML, "
      "like Gecko) SamsungBrowser/11.0 Chrome/142.0.7444.175 TV Safari/537.36");
}

void parseChromiumArguments(const std::string& args,
                            std::vector<std::string>& chromiumArgumentsList) {
  std::istringstream iss(args);
  std::string token;
  while (iss >> token) {
    chromiumArgumentsList.push_back(token);
  }
}

static Eina_Bool watch_dog_notify(void* data) {
  LOG(INFO) << "Send watchdog notify";
  if (smart_deadlock_watchdog_notify() < 0) {
    LOG(INFO) << "watchdog notify failed!!";
  }
  return ECORE_CALLBACK_RENEW;
}

static bool browser_create(void* user_data) {
  LOG(INFO) << "browser_create starts";
  typedef void (*func_app_create)();
  func_app_create chromeAppCreatePtr =
      reinterpret_cast<func_app_create>(ewk_dlsym("ChromeAppCreate"));
  // should_preload set to 0
  setShouldPreload(0);
  if (chromeAppCreatePtr) {
    chromeAppCreatePtr();
  } else {
    LOG(ERROR) << "ChromeAppCreate function not found";
  }
  int wd = smart_deadlock_register_watchdog_service(watchdogTime);
  LOG(INFO) << "Register wd " << wd;
  if (!smdlNotifyTimer) {
    LOG(INFO) << "Adding watchdog timer...";
    smdlNotifyTimer =
        ecore_timer_add(smdlNotifyInterval, watch_dog_notify, NULL);
  }
  LOG(INFO) << "browser_create ends";
  return 1;
}

static void browser_control(app_control_h app_control, void* user_data) {
  LOG(INFO) << "browser_control starts";

  static bool browser_received_control = false;
  static bool has_browser_launched_without_preloading = false;

  typedef void (*func_app_control)(
      const std::map<std::string, std::string>& keyValuePairs);

  static func_app_control chromeAppControlPtr = nullptr;
  if (chromeAppControlPtr == nullptr) {
    chromeAppControlPtr =
        reinterpret_cast<func_app_control>(ewk_dlsym("ChromeAppControl"));
    if (!chromeAppControlPtr) {
      LOG(ERROR) << "Failed to load ChromeAppControl";
    }
  }

  serviceChecker checker(app_control);
  if (!checker.preLoading && !checker.bg_launch_enabled) {
    LOG(INFO) << "Browser control without preloading";
    has_browser_launched_without_preloading = true;
  }

  if (browser_received_control) {
    LOG(INFO) << "Browser control already received";
    if (checker.requestURI.length()) {
      std::map<std::string, std::string> keyValuePairs = {
          {"URL", checker.requestURI}};
      chromeAppControlPtr(keyValuePairs);
    }
    if (checker.chromiumArguments == "EnableAutomationHelper") {
      LOG(INFO) << "Enable Automation Helper";
      std::map<std::string, std::string> keyValuePairs = {
          {"EnableAutomationHelper", "true"}};
      chromeAppControlPtr(keyValuePairs);
      checker.chromiumArguments = "";
    }
    if (!browser_window_shown) {
      LOG(INFO) << "Showing Preloaded browser";
      browser_window_shown = true;
      std::map<std::string, std::string> keyValuePairs = {
          {"show_preloaded_browser", "true"}};
      chromeAppControlPtr(keyValuePairs);
    }
    if (checker.caller.compare("org.tizen.voice-client") == 0 ||
        checker.caller.compare("com.samsung.tv.alexa-client") == 0 ||
        (checker.bg_launch_enabled &&
         has_browser_launched_without_preloading)) {
      LOG(INFO) << "Self launching browser";
      AppControl appControl(app_control);
      appControl.SelfLaunchBrowser("org.tizen.next-browser");
    }
    LOG(INFO) << "browser already launched or preloaded";
    return;
  }
  std::vector<std::string> argumentsList;
  passDefaultArguments(argumentsList);
  browser_received_control = true;
  if (checker.bg_launch_enabled) {
    argumentsList.emplace_back("--bg-launch-enabled");
  }
  if (checker.preLoading) {
    watch_dog_notify(nullptr);
    argumentsList.emplace_back("--preloading");
    std::map<std::string, std::string> keyValuePairs = {
        {"show_preloaded_browser", "false"}};
    chromeAppControlPtr(keyValuePairs);
  } else
    browser_window_shown = true;

  bool is_browser_lite = false;
  bool error = false;
  DDR_MEMORY_SIZE DDRMemory = static_cast<DDR_MEMORY_SIZE>(GetDDRInfo(&error));

  if (DDRMemory == DDR_MEMORY_SIZE::DDR_1G)
    is_browser_lite = true;

  if (is_browser_lite) {
    passBrowserLiteArguments(argumentsList);
  } else {
    passNextBrowserArguments(argumentsList);
  }

  if (!checker.chromiumArguments.empty()) {
    parseChromiumArguments(checker.chromiumArguments, argumentsList);
  }

  if (!checker.requestURI.empty()) {
    argumentsList.push_back(checker.requestURI);
  }

  int totalBrowserArguments = argumentsList.size();
  LOG(INFO) << totalBrowserArguments;

  const char** argv =
      (const char**)malloc(totalBrowserArguments * sizeof(const char*));
  if (argv == NULL) {
    LOG(ERROR) << "failed to allocate memory";
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < totalBrowserArguments; i++) {
    argv[i] = argumentsList[i].c_str();
    LOG(INFO) << argv[i];
  }

  typedef int (*func_ptr_t)(int totalBrowserArguments, const char** argv);
  func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("ChromeMain"));
  if (fp) {
    int chromeMain = fp(totalBrowserArguments, argv);
    LOG(INFO) << chromeMain;
    ui_app_exit();
    LOG(INFO) << "Exiting browser application...ui_app_exit() called";
  } else {
    LOG(ERROR) << "ChromeMain function not found";
    ui_app_exit();
    LOG(INFO) << "Exiting browser application...ui_app_exit() called";
  }
  free(argv);
}

static void browser_pause(void* user_data) {
  LOG(INFO) << "browser_pause call";
  typedef void (*func_app_pause)();
  func_app_pause chromeAppPausePtr =
      reinterpret_cast<func_app_pause>(ewk_dlsym("ChromeAppPause"));
  if (chromeAppPausePtr) {
    chromeAppPausePtr();
  } else {
    LOG(ERROR) << "ChromeAppPause function not found";
  }
}

static void browser_resume(void* user_data) {
  LOG(INFO) << "browser_resume starts";
  typedef void (*func_app_resume)();
  func_app_resume chromeAppResumePtr =
      reinterpret_cast<func_app_resume>(ewk_dlsym("ChromeAppResume"));
  if (chromeAppResumePtr) {
    chromeAppResumePtr();
  } else {
    LOG(ERROR) << "ChromeAppResume function not found";
  }
  LOG(INFO) << "browser_resume ends";
}

static void browser_terminate(void* user_data) {
  LOG(INFO) << "browser_terminate starts";
  typedef void (*func_app_terminate)();
  func_app_terminate chromeAppTerminatePtr =
      reinterpret_cast<func_app_terminate>(ewk_dlsym("ChromeAppTerminate"));
  int memState = getLowMemState();
  if (memState < 4 && browser_window_shown) {
    setShouldPreload(1);
    LOG(INFO) << "should_preload set to 1";
  }

  if (chromeAppTerminatePtr) {
    chromeAppTerminatePtr();
  } else {
    LOG(ERROR) << "ChromeAppTerminate function not found";
  }
  int ret = smart_deadlock_unregister_watchdog_service();
  LOG(INFO) << "Unregister wd " << ret;
  if (smdlNotifyTimer) {
    LOG(INFO) << "Deleting watchdog timer...";
    ecore_timer_del(smdlNotifyTimer);
    smdlNotifyTimer = nullptr;
  }
  LOG(INFO) << "browser_terminate ends";
}

static void system_low_memory(app_event_info_h, void* user_data) {
  LOG(INFO) << "system_low_memory call";
  typedef void (*func_system_low_memory)();
  func_system_low_memory systemLowMemoryHandlerPtr =
      reinterpret_cast<func_system_low_memory>(
          ewk_dlsym("SystemLowMemoryHandler"));
  if (systemLowMemoryHandlerPtr) {
    systemLowMemoryHandlerPtr();
  } else {
    LOG(ERROR) << "SystemLowMemoryHandler function not found";
  }
}

static int getLowMemState() {
  int lowMemory;
  if (vconf_get_int(VCONFKEY_SYSMAN_LOW_MEMORY, &lowMemory)) {
    LOG(INFO) << "fail to get lowMemory state";
    return -1;
  }
  LOG(INFO) << "lowMemory = " << lowMemory;
  return lowMemory;
}

static bool setShouldPreload(int state) {
  LOG(INFO) << "state= " << state;
  if (vconf_set_int(B_VCONFKEY_SHOULD_PRELOAD, state)) {
    LOG(INFO) << "set should preload state fail";
    return false;
  }
  return true;
}

int BrowserMain(int argc, const char** argv) {
  LOG(INFO) << "BROWSER-APP-INTERFACE : BrowserMain called..";
  for (int i = 0; i < argc; i++)
    LOG(INFO) << argv[i];
  bool isSubprocess = false;
  for (int i = 0; i < argc; i++) {
    std::string arg(argv[i]);
    isSubprocess = arg.find("--type") != std::string::npos;
    if (isSubprocess)
      break;
  }
  if (isSubprocess) {
    LOG(INFO) << "Subprocess";
    typedef int (*func_ptr_t)(int argc, const char** argv);
    func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("ChromeMain"));
    if (fp)
      return fp(argc, argv);
    return 0;
  } else {
    LOG(INFO) << "Main process";
    ui_app_lifecycle_callback_s event_callback;
    memset(&event_callback, 0x00, sizeof(ui_app_lifecycle_callback_s));
    app_event_handler_h low_memory_handler = NULL;
    event_callback.create = browser_create;
    event_callback.terminate = browser_terminate;
    event_callback.pause = browser_pause;
    event_callback.resume = browser_resume;
    event_callback.app_control = browser_control;
    ui_app_add_event_handler(&low_memory_handler, APP_EVENT_LOW_MEMORY,
                             system_low_memory, NULL);
    LOG(INFO) << "ui_app_main begins";
    int browser_init = ui_app_main(argc, (char**)argv, &event_callback, NULL);
    LOG(INFO) << "Main process ends";
    return browser_init;
  }
}

}  // namespace samsung_next_browser
