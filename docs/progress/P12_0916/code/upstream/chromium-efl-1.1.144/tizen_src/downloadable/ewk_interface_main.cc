// Copyright 2019 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ewk_interface_main.h"

#include "build/build_config.h"

#define TIZEN_VERSION                                        \
  (TIZEN_VERSION_MAJOR * 10000 + TIZEN_VERSION_MINOR * 100 + \
   TIZEN_VERSION_PATCH)
#define TIZEN_VERSION_AT_LEAST(major, minor, patch) \
  (TIZEN_VERSION >= (major * 10000 + minor * 100 + patch))

#define CHROMIUM_PRELOAD_LIB_PATH LIB_RO_ROOT_DIR "/lib/" CHROMIUM_IMPL_LIB_FILE

#if BUILDFLAG(IS_TIZEN_TV)
#define PATH_PRELOAD_CHROMIUM_EFL_IMG APP_RO_ROOT_DIR "/res/chromium-efl.img"
#define MOUNT_ARGS_PRELOAD_CHROMIUM \
  PATH_PRELOAD_CHROMIUM_EFL_IMG " " LIB_RO_ROOT_DIR " -t squashfs -o loop,ro"

#define CHROMIUM_UPGRADE_LIB_PATH \
  LIB_UPGRADE_ROOT_DIR "/lib/" CHROMIUM_IMPL_LIB_FILE

#include <lwipc.h>
#include <ttrace.h>

#include <sys/resource.h>
#endif  // BUILDFLAG(IS_TIZEN_TV)

#include <EWebKit.h>
#include <EWebKit_internal.h>
#include <EWebKit_product.h>

#include <dlfcn.h>
#include <unistd.h>

#include "dlog_util.h"

#include <sys/stat.h>
#include <sys/time.h>

#if defined(ENABLE_WRT_JS)
#if BUILDFLAG(IS_TIZEN_TV)
#include <linux/loop.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/statfs.h>
#include <system_info.h>
#include <vconf.h>

#if TIZEN_VERSION_AT_LEAST(8, 0, 0) && defined(TIZEN_VD_ENTERPRISE_FEATURE)
#include <launchpad.h>
#endif
#endif  // BUILDFLAG(IS_TIZEN_TV)
#endif

#if defined(SAMSUNG_NEXT_BROWSER)
#include "browser_app_interface.h"
#endif  // SAMSUNG_NEXT_BROWSER

namespace ewk_interface {

void* g_impl_lib_handle = nullptr;
int dlopen_mode = RTLD_LAZY | RTLD_GLOBAL;

inline unsigned long long ConvertMilliseconds(timespec ts) {
  return ((unsigned long long)ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

#if BUILDFLAG(IS_TIZEN_TV)
bool g_upgrade_lib = false;  // conservative policy
const char* kChromiumMountReady = "/tmp/.chromium_mount.ready";
const char* kChromiumUpdateOrPreloadMountReady =
    "/tmp/.chromium_update_or_preload_mount.ready";
const char* kChromiumUpgradeMountReady = "/tmp/.chromium_upgrade_mount.ready";
const char* kChromiumUpgradeTPKExists = "/tmp/.chromium_upgrade_tpk.exists";
const unsigned int kUpgradeMountTimeout = 10000;

inline bool upgradeTPKExists() {
  if (LwipcIsDone(kChromiumUpgradeTPKExists) > 0) {
    LOG(INFO) << "Upgrade TPK exists";
    return true;
  }

  LOG(INFO) << "Upgrade TPK does not exist";
  return false;
}

inline bool upgradeTPKMounted() {
  bool result = !access(CHROMIUM_UPGRADE_LIB_PATH, F_OK);
  LOG(INFO) << "path: " << CHROMIUM_UPGRADE_LIB_PATH << ", result: " << result;

  if (!result) {
    LOG(INFO) << "error reason: " << strerror(errno);
  }

  return result;
}

#if defined(ENABLE_WRT_JS)
#define SQUASHFS_MAGIC 0x73717368

const char* kPrivilegeServiceLibPath = "libprivileged-service-client.so";
const char* kPSAgentReady = "/run/ps_agent.pid";
const char* kUWETag = "USE_UWE";
const char* kWrtLoaderCompleted = "/tmp/.wrt_loader.completed";
const unsigned int kPSAgentTimeout = 30000;

#if TIZEN_VERSION_AT_LEAST(8, 0, 0) && defined(TIZEN_VD_ENTERPRISE_FEATURE)
const int kBooTypeLastModeUrlLauncher = 2;
const char* kEPAppbootType = "db/ep-common/last_mode";
#endif

char app_define_base_path[128] = {0};
bool try_mounted = false;

bool IsFirstWrtLoader() {
  return try_mounted || access(kWrtLoaderCompleted, F_OK) != 0;
}

void DisposeLoaderIfNecessary() {
#if TIZEN_VERSION_AT_LEAST(8, 0, 0) && defined(TIZEN_VD_ENTERPRISE_FEATURE)
  LOG(INFO) << "DisposeLoaderIfNecessary for LFD";
  if (!IsFirstWrtLoader()) {
    // skip if this is not first wrt-loader
    return;
  }

  LOG(INFO) << "wrt-loader first launched!!";

  int boot_type = 0;
  if (vconf_get_int(kEPAppbootType, &boot_type) != 0) {
    LOG(ERROR) << "vconf error : " << kEPAppbootType;
    return;
  }

  LOG(INFO) << "boot type == " << boot_type;
  if (boot_type != kBooTypeLastModeUrlLauncher) {
    LOG(INFO) << "wrt-loader not needed!";
    launchpad_loader_dispose();
  }
#endif  // TIZEN_VERSION_AT_LEAST(8, 0, 0) && defined(TIZEN_VD_ENTERPRISE_FEATURE)
}

void SetPriority(int absolute) {
  setpriority(PRIO_PROCESS, 0, absolute);
  LOG(INFO) << "Set process priority : " << absolute;
}

void LowerPriorityIfNecessary() {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  auto current_time = ConvertMilliseconds(now);
  if (current_time > 120000 || IsFirstWrtLoader()) {
    // skip if kerneltime is over 120 or if this is first wrt-loader
    return;
  }

  char* foreground_app = vconf_get_str("memory/wrt/most_recent_foreground");
  if (!foreground_app || strlen(foreground_app) < 10) {
    LOG(INFO) << "There is no FG webapp";
    if (foreground_app) free(foreground_app);
    return;
  }
  LOG(INFO) << "most_recent_foreground : " << foreground_app
            << ", current_time : " << current_time;
  free(foreground_app);
  SetPriority(10);
}

bool WaitPSAgentReady() {
  static bool ps_agent_ready = false;
  if (ps_agent_ready)
    return true;

  int max_retry = 20;
  LOG(INFO) << "Waiting the ps_agent ready";
  while (max_retry != 0) {
    int ret = LwipcWaitEvent(kPSAgentReady, kPSAgentTimeout);
    if (ret == 0) {
      LOG(INFO) << "ps_agent is ready";
      ps_agent_ready = true;
      return true;
    }

    --max_retry;
    LOG(ERROR) << "ps_agent is not ready : " << (ret > 0 ? "timeout" : "fail")
               << ", remain retry [" << max_retry << "]";
  }

  return false;
}

bool PSAgentMount(const char* image_path) {
  if (access(image_path, F_OK) != 0) {
    LOG(INFO) << "no chromium-efl.img";
    return false;
  }

  if (WaitPSAgentReady() == false)
    LOG(ERROR) << "Chromium-efl mount may not work.";

  int rv = 0;
  int result = 0;
  const char* args = MOUNT_ARGS_PRELOAD_CHROMIUM;

  LOG(INFO) << "mount args: \"" << args << "\"";

  auto ps_handle_ = dlopen(kPrivilegeServiceLibPath, RTLD_LAZY);
  if (!ps_handle_) {
    LOG(ERROR) << "dlopen() fail : " << dlerror();
    return false;
  }
  typedef int (*ps_mount)(const char*, size_t, int, int*);
  ps_mount PS_Mount = nullptr;
#if TIZEN_VERSION_AT_LEAST(7, 0, 0)
  PS_Mount = (ps_mount)dlsym(ps_handle_, "PS_Mount_Fast");
#else
  PS_Mount = (ps_mount)dlsym(ps_handle_, "PS_Mount");
#endif
  if (!PS_Mount) {
    LOG(ERROR) << "dlsym() fail : " << dlerror();
    return false;
  }
  rv = PS_Mount(args, strlen(args) + 1, 1, &result);
  LOG(INFO) << "result = " << rv << ", " << result;
  if (rv != 0 || result != 0) {
    LOG(ERROR) << "mount failed. (" << rv << ", " << result << ")";
    return false;
  }
  return true;
}

bool IsAppzoneSupport() {
  bool web_isolation_support = false;
  LOG(INFO) << "fms get start";
  if (SYSTEM_INFO_ERROR_NONE ==
      system_info_get_custom_bool(
          "com.samsung/featureconf/security.container.web_isolation",
          &web_isolation_support)) {
  LOG(INFO) << "fms get end";
    return web_isolation_support;
  } else {
    return false;
  }
}

bool DirectMount(const char* image_path, const char* point_point) {
  const char* dev_loop_control = "/dev/loop-control";
  const char* dev_loop_prefix = "/dev/loop";

  int ret;
  struct statfs _statfs;
  ret = statfs(point_point, &_statfs);
  if (ret < 0) {
    LOG(ERROR) << "Error getting statfs";
    return false;
  }
  if (_statfs.f_type == SQUASHFS_MAGIC) {
    LOG(INFO) << "Already mounted";
    return true;
  }

  const auto loop_control = std::fopen(dev_loop_control, "r");
  if (!loop_control) {
    LOG(ERROR) << "Cannot open : " << dev_loop_control;
    return false;
  }

  LOG(INFO) << "Direct mount start";
  const auto loop_control_fd = fileno(loop_control);
  const auto devnr = ioctl(loop_control_fd, LOOP_CTL_GET_FREE);
  std::stringstream loopname;
  loopname << dev_loop_prefix << devnr;
  const auto loop_device_name = loopname.str();
  const auto loop_device = std::fopen(loop_device_name.c_str(), "r");
  if (!loop_device) {
    LOG(ERROR) << "Cannot open : " << loop_device_name;
    std::fclose(loop_control);
    return false;
  }
  const auto loop_device_fd = fileno(loop_device);
  const auto image = std::fopen(image_path, "r");
  if (!image) {
    LOG(ERROR) << "Cannot open : " << image_path;
    std::fclose(loop_device);
    std::fclose(loop_control);
    return false;
  }

  const auto image_fd = fileno(image);
  ioctl(loop_device_fd, LOOP_SET_FD, image_fd);
  const auto result = mount(loop_device_name.c_str(), point_point, "squashfs",
                            MS_RDONLY, nullptr);
  ioctl(loop_device_fd, LOOP_CLR_FD, 0);
  std::fclose(image);
  std::fclose(loop_device);
  std::fclose(loop_control);
  LOG(INFO) << "Direct mount end";
  return result == 0;
}

bool MountDefaultChromiumImage(bool default_psagent_mount = false) {
  if (access(CHROMIUM_PRELOAD_LIB_PATH, F_OK) == 0) {
    LOG(INFO) << "Already libchromium-impl.so exists";
    return true;
  }

  if (default_psagent_mount || IsAppzoneSupport()) {
    if (!PSAgentMount(PATH_PRELOAD_CHROMIUM_EFL_IMG)) {
      LOG(ERROR) << "pa-agent mount has failed, will wait efl-install-service";
      return false;
    }
  } else {
    if (!DirectMount(PATH_PRELOAD_CHROMIUM_EFL_IMG, LIB_RO_ROOT_DIR)) {
      LOG(ERROR) << "Direct mount has failed, will wait efl-install-service";
      return false;
    }
  }
  try_mounted = true;
  return true;
}

void SetIndependentUWEPath(const char* base_path) {
  // base_path: /opt/usr/apps/{pkgid}/bin/{appid}
  // get pkgid by parsing base_path, concatenate $app_define_base_path
  const static size_t prefix_len = strlen("/opt/usr/apps/");
  auto p = strchr(base_path + prefix_len, '/');
  char pkgid[64] = {0};
  strncpy(pkgid, base_path + prefix_len, p - base_path - prefix_len);
  snprintf(app_define_base_path, sizeof(app_define_base_path),
           "/home/owner/apps_rw/%s/shared/res/", pkgid);
}

void TryOpenAppDefinedLib() {
  if (g_impl_lib_handle)
    return;
  if (strlen(app_define_base_path) == 0)
    return;
  if (access(app_define_base_path, F_OK) != 0)
    return;

  char app_defined_lib[256] = {0};
  snprintf(app_defined_lib, sizeof(app_defined_lib),
           "%s/lib/libchromium-impl.so", app_define_base_path);
  LOG(INFO) << "app_defined_lib : " << app_defined_lib;

  if (access(app_defined_lib, F_OK) != 0)
    return;
  g_impl_lib_handle = dlopen(app_defined_lib, RTLD_LAZY | RTLD_GLOBAL);
  if (!g_impl_lib_handle) {
    LOG(ERROR) << "dlopen error : " << dlerror();
    return;
  }
}
#endif  // defined(ENABLE_WRT_JS)

#endif  // BUILDFLAG(IS_TIZEN_TV)

void* open_library() {
  if (g_impl_lib_handle)
    return g_impl_lib_handle;

#if BUILDFLAG(IS_TIZEN_TV)
  bool apply_upgrade = false;
  if (g_upgrade_lib && upgradeTPKExists()) {
    LOG(INFO) << "policy: " << g_upgrade_lib
              << ", RW TPK found, attempt to mount RW TPK";
    if (upgradeTPKMounted()) {
      LOG(INFO) << "RW TPK mount already done. Load RW package.";
      apply_upgrade = true;
    } else {
      LOG(INFO) << "Waiting for RW TPK mount done. Max wait: "
                << kUpgradeMountTimeout << "ms";
      int ret =
          LwipcWaitEvent(kChromiumUpgradeMountReady, kUpgradeMountTimeout);
      if (ret != 0) {
        LOG(INFO) << "RW TPK exists, but mount failed. Load RO package.";
        apply_upgrade = false;
      } else {
        LOG(INFO) << "RW TPK mount done. Load RW package.";
        apply_upgrade = true;
      }
    }
  }
  const char* path =
      apply_upgrade ? CHROMIUM_UPGRADE_LIB_PATH : CHROMIUM_PRELOAD_LIB_PATH;
#else
  const char* path = CHROMIUM_PRELOAD_LIB_PATH;
#endif

#if defined(USE_TTRACE)
  traceBegin(TTRACE_TAG_WEB, "dlopen start");
#endif

  unsigned long long diff;
  struct timespec begin, end;
  struct stat file_stat;

  stat(path, &file_stat);
  clock_gettime(CLOCK_MONOTONIC, &begin);
  g_impl_lib_handle = dlopen(path, dlopen_mode);
  clock_gettime(CLOCK_MONOTONIC, &end);
  if (!g_impl_lib_handle)
    LOG(ERROR) << "dlopen error : " << dlerror();
  diff = ConvertMilliseconds(end) - ConvertMilliseconds(begin);
  LOG(ERROR) << "Open library done. cost:" << diff << "ms\npath:" << path
             << " size:" << file_stat.st_size / (1024 * 1024) << "MB"
             << ", dlopen mode:" << dlopen_mode;

#if defined(USE_TTRACE)
  traceEnd(TTRACE_TAG_WEB);
#endif
  // FIXME:Need to call dlclose(), to make sure that the opened chromium-impl
  // library joins all threads forked by chromium
  return g_impl_lib_handle;
}

}  // namespace ewk_interface

using namespace ewk_interface;

static void _ewk_force_acceleration() __attribute__((constructor));

void _ewk_force_acceleration() {
  // Chromium-efl port does not support s/w mode. So we need to set h/w mode
  // before creating elm_window. To do this, make constructor function which is
  // called at library loading time and set "ELM_ACCEL=hw" here. If not, native
  // app which does not call elm_config_accel_preference_set() function will
  // fail to execute.
  setenv("ELM_ACCEL", "hw", 1);

  // Chromium-efl does not support evasgl render thread feature yet.
  setenv("EVAS_GL_DISABLE_RENDER_THREAD", "1", 1);
}

void* ewk_dlsym(const char* function_name) {
#if defined(ENABLE_WRT_JS) && BUILDFLAG(IS_TIZEN_TV)
  TryOpenAppDefinedLib();
#endif

  if (!g_impl_lib_handle && !open_library())
    return nullptr;

  void* function_addr = dlsym(g_impl_lib_handle, function_name);
  if (!function_addr)
    LOG(ERROR) << "No " << function_name << " symbol found! " << dlerror();

  return function_addr;
}

Eina_Bool ewk_wait_chromium_ready(unsigned int timeout_msec) {
#if BUILDFLAG(IS_TIZEN_TV)
  LOG(INFO) << "ewk_wait_chromium_ready called. timeout : " << timeout_msec;

  int ret = LwipcWaitEvent(kChromiumUpdateOrPreloadMountReady, timeout_msec);
  if (ret == 0) {
    LOG(INFO) << "update or preload chromium mount is done.";
    return true;
  }

  LOG(ERROR) << "chromium mount is not ready yet: "
             << (ret > 0 ? "timeout" : "fail");
#else
  LOG(WARN) << "Not supported function";
#endif
  return false;
}

Eina_Bool ewk_check_chromium_ready() {
#if BUILDFLAG(IS_TIZEN_TV)
  LOG(INFO) << "ewk_check_chromium_ready called.";

  if (LwipcIsDone(kChromiumUpdateOrPreloadMountReady) > 0) {
    LOG(INFO) << "update or preload chromium mount is done.";
    return true;
  }

  LOG(ERROR) << "chromium mount is not ready yet.";
#else
  LOG(WARN) << "Not supported function";
#endif
  return false;
}

int ewk_set_version_policy(int preference) {
#if BUILDFLAG(IS_TIZEN_TV)
  int ret = preference;
  if (0 == preference)
    g_upgrade_lib = false;  // conservative
  else if (1 == preference)
    g_upgrade_lib = true;  // progressive
  else {
    // todo : we can add the exact version policy like 63, 69 in the future.
    ret = g_upgrade_lib ? 1 : 0;
    LOG(ERROR) << "Unknown preference:" << preference << " current:" << ret;
  }
  LOG(INFO) << "ewk_set_version_policy : " << ret;
  typedef int (*func_ptr_t)(int);
  func_ptr_t fp =
      reinterpret_cast<func_ptr_t>(ewk_dlsym("ewk_set_version_policy"));
  if (fp) {
    return fp(preference);
  } else if (g_upgrade_lib) {
    // ignore request
    LOG(ERROR) << "ewk_set_version_policy is not found. UWE request is ignored";
    g_upgrade_lib = false;
    return 0;
  }
  return ret;
#else
  LOG(WARN) << "Not supported function";
  return 1;
#endif
}

int ewk_init(void) {
  LOG(INFO) << "EWK-INTERFACE : ewk_init called..";
  typedef int (*func_ptr_t)(void);
  func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("ewk_init"));
  if (fp)
    return fp();
  return 0;
}

int ewk_shutdown(void) {
  LOG(INFO) << "EWK-INTERFACE : ewk_shutdown called..";
  typedef int (*func_ptr_t)(void);
  func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("ewk_shutdown"));
  if (fp)
    return fp();
  return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((visibility("default"))) int efl_webprocess_main(
    int argc,
    const char** argv) {
  LOG(INFO) << "EWK-INTERFACE : efl_webprocess_main called..";

#if BUILDFLAG(IS_TIZEN_TV)
  if (!g_impl_lib_handle && !access(CHROMIUM_UPGRADE_LIB_PATH, F_OK)) {
    // If library is not loaded and upgrade package is installed,
    // we need to decide upgrade library by command line flag --impl-library
    // appended in browser process.
    for (int i = 1; i < argc; ++i) {
      if (!strcmp(argv[i], "--impl-library-upgrade")) {
        g_upgrade_lib = true;
        break;
      }
    }
    LOG(INFO) << "Version policy : " << g_upgrade_lib;
  }
#endif

  typedef int (*func_ptr_t)(int argc, const char** argv);
  func_ptr_t fp =
      reinterpret_cast<func_ptr_t>(ewk_dlsym("efl_webprocess_main"));
  if (fp)
    return fp(argc, argv);
  return 0;
}

#if defined(ENABLE_WRT_JS)
__attribute__((visibility("default")))
int WRTMain(int argc, char** argv) {
  LOG(INFO) << "EWK-INTERFACE : WRTMain called..";
#if BUILDFLAG(IS_TIZEN_TV)
  if (strcmp(argv[0], "/usr/bin/wrt-loader") == 0) {
    DisposeLoaderIfNecessary();
    LowerPriorityIfNecessary();
    dlopen_mode = RTLD_NOW | RTLD_GLOBAL;
    LOG(INFO) << "for wrt-loader, dlopen with RTLD_NOW | RTLD_GLOBAL";
  }
  if (!MountDefaultChromiumImage() && !ewk_wait_chromium_ready(20000)) {
    return 0;
  }

  if (IsFirstWrtLoader())
    dlopen_mode = RTLD_NOW | RTLD_GLOBAL;

  for (int i = 1; i < argc; ++i) {
    if (strncmp(argv[i], kUWETag, strlen(kUWETag)) == 0) {
      LOG(INFO) << "USE UWE";
      SetIndependentUWEPath(argv[0]);
      ewk_set_version_policy(1);  // use latest engine
      break;
    }
  }
#endif

  typedef int (*func_ptr_t)(int argc, char** argv);
  func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("WRTMain"));
  if (fp)
    return fp(argc, argv);
  return 0;
}

__attribute__((visibility("default")))
int WRTServiceMain(int argc, char** argv) {
  LOG(INFO) << "EWK-INTERFACE : WRTServiceMain called..";

#if BUILDFLAG(IS_TIZEN_TV)
  MountDefaultChromiumImage(true);
#endif

  typedef int (*func_ptr_t)(int argc, char** argv);
  func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("WRTServiceMain"));
  if (fp)
    return fp(argc, argv);
  return 0;
}
#endif

#if defined(BUILD_CHROME)
#if defined(SAMSUNG_NEXT_BROWSER)
__attribute__((visibility("default"))) int ChromeMain(int argc,
                                                      const char** argv) {
  LOG(INFO) << "EWK-INTERFACE : ChromeMain called..";

  // Check if Chromium is ready before proceeding
  if (ewk_check_chromium_ready() == EINA_FALSE) {
    LOG(INFO) << "Chromium is not mounted yet";
    do {
      LOG(INFO) << "Wait for Chromium mount...";
    } while (ewk_wait_chromium_ready(1000) == EINA_FALSE);
  }

  if (!ewk_set_version_policy(1))  // To support UWE
  {
    LOG(ERROR) << "Failed to find ewk_set_version_policy";
    return 0;
  }
  LOG(INFO) << "ewk_set_version_policy done";
  return samsung_next_browser::BrowserMain(argc, argv);
}
#else
__attribute__((visibility("default"))) int ChromeMain(int argc,
                                                      const char** argv) {
  typedef int (*func_ptr_t)(int argc, const char** argv);
  func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("ChromeMain"));
  if (fp) {
    return fp(argc, argv);
  }
  LOG(ERROR) << "EWK-INTERFACE : ChromeMain not found.";
  return 0;
}

__attribute__((visibility("default"))) int ChromeMainTizen(int argc,
                                                           const char** argv) {
  typedef int (*func_ptr_t)(int argc, const char** argv);
  func_ptr_t fp = reinterpret_cast<func_ptr_t>(ewk_dlsym("ChromeMainTizen"));
  if (fp) {
    return fp(argc, argv);
  }
  LOG(ERROR) << "EWK-INTERFACE : ChromeMainTizen not found.";
  return 0;
}
#endif
#endif  // defined(BUILD_CHROME)

#ifdef __cplusplus
}
#endif
