// Copyright 2019 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cynara-creds-self.h>
#include <dirent.h>
#include <fcntl.h>
#include <glib.h>
#include <lwipc.h>
#include <privileged-service/PS_API.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vconf/vconf.h>
#include <vconf/vconf_rtc.h>
#include <iostream>
#include <string>

#include "base/memory/free_deleter.h"
#include "chromium_impl/build/tizen_version.h"

#define FILE_LOG_OUT
#include "dlog_util.h"

// Check if path is real path(not symbolic link) or does not exist.
static bool is_real_path(const char* path) {
  struct stat ls;
  if (lstat(path, &ls) == -1) {
    // not exist
    if (errno == ENOENT)
      return true;
    LOG(ERROR) << "Failed lstat(" << path << ") : " << errno;
  } else {
    // not symbolic link
    if (!S_ISLNK(ls.st_mode))
      return true;
    LOG(ERROR) << path << " is symbolic link.";
  }
  return false;
}

#ifdef FILE_LOG_OUT
std::ofstream TizenDlogWrapper::fstream_ = []() {
  const char* file_log_path = "/tmp/chromium-efl-install.log";
  return is_real_path(file_log_path)
             ? std::ofstream(file_log_path, std::ios::out | std::ios::app)
             : std::ofstream();
}();
__attribute__((destructor)) void close_log_file() {
  TizenDlogWrapper::fstream_.close();
}
#endif

#define PATH_PRELOAD_CHROMIUM_EFL_IMG APP_RO_ROOT_DIR "/res/chromium-efl.img"
#define PATH_UPDATED_CHROMIUM_EFL_IMG APP_RW_ROOT_DIR "/res/chromium-efl.img"

// tv has 3-package 2-lib policy. Upgraded engine is used optionally.
// 1) preload package /ro/... -> /usr/share/.../lib
// 2) updated package /rw/... -> /usr/share/.../lib
// 3) upgrade package /rw/...-upgrade ->  /usr/share/.../lib/upgrade
#define PATH_UPGRADE_CHROMIUM_EFL_IMG \
  APP_UPGRADE_ROOT_DIR "/res/chromium-efl.img"

#define MOUNT_ARGS_PRELOAD_CHROMIUM \
  PATH_PRELOAD_CHROMIUM_EFL_IMG " " LIB_RO_ROOT_DIR " -t squashfs -o loop,ro"
#define MOUNT_ARGS_UPDATED_CHROMIUM \
  PATH_UPDATED_CHROMIUM_EFL_IMG " " LIB_RO_ROOT_DIR " -t squashfs -o loop,ro"
#define MOUNT_ARGS_UPGRADE_CHROMIUM                      \
  PATH_UPGRADE_CHROMIUM_EFL_IMG " " LIB_UPGRADE_ROOT_DIR \
                                " -t squashfs -o loop,ro"

#define UMOUNT_ARGS_PREFIX "-dl "
#define SUCCESS 0
#define FAIL -1

const char* kChromiumUpdateOrPreloadMountReady =
    "/tmp/.chromium_update_or_preload_mount.ready";
const char* kChromiumUpgradeMountReady = "/tmp/.chromium_upgrade_mount.ready";
const char* kChromiumUpgradeTPKExists = "/tmp/.chromium_upgrade_tpk.exists";
const char* kColdBootRequester = "chromium-efl";
const char* kLaunchFail = "rtc/memory/WebApp/LaunchFail";
const char* kPSAgentReady = "/run/ps_agent.pid";
const char* kSetColdBoot = "memory/boot/cold_poweroff_request_pkg";
std::string kTargetTPK;
const unsigned int kPSAgentTimeout = 30000;

static bool ps_agent_ready = false;
static bool WaitPSAgentReady() {
  if (ps_agent_ready)
    return true;

  int maxRetry = 20;
  LOG(INFO) << "Waiting the ps_agent ready";
  while (maxRetry != 0) {
    int ret = LwipcWaitEvent(kPSAgentReady, kPSAgentTimeout);
    if (ret == 0) {
      LOG(INFO) << "ps_agent is ready";
      ps_agent_ready = true;
      return true;
    }

    --maxRetry;
    LOG(ERROR) << "ps_agent is not ready : " << (ret > 0 ? "timeout" : "fail")
               << ", remain retry [" << maxRetry << "]";
  }

  return false;
}

static void SetErrMsg(const std::string& mount_error) {
  std::string vconfValue = kTargetTPK + mount_error;
  LOG(ERROR) << "vconf : " << kLaunchFail << " value : " << vconfValue;
  vconf_set_str_rtc(kLaunchFail, vconfValue.c_str());
}

static void RequestColdBoot() {
  LOG(INFO) << "Request Cold Boot";
  if (vconf_set_str(kSetColdBoot, kColdBootRequester) != 0)
    LOG(ERROR) << "Request Cold Boot Fail";
}

// tv has different mount method and no tpk self-installation
static int mount(bool mount, const char* args, bool use_system = false) {
#define DEBUG_LOG_SMACK_LABEL 0
#if DEBUG_LOG_SMACK_LABEL
  char* smack_label = nullptr;
  int ret = cynara_creds_self_get_client(CLIENT_METHOD_SMACK, &smack_label);
  std::unique_ptr<char, base::FreeDeleter> smack_label_holder(smack_label);
  if (ret != CYNARA_API_SUCCESS) {
    LOG(ERROR) << "failed to get smack label";
  } else {
    LOG(INFO) << "SMACK Label:" << smack_label;
  }
#endif

  if (WaitPSAgentReady() == false)
    LOG(ERROR) << "Chromium-efl mount may not work.";

  LOG(INFO) << "mount:" << mount << ", args: \"" << args << "\"";
  int rv = SUCCESS;
  int result = SUCCESS;

  if (mount) {
#if TIZEN_VERSION_AT_LEAST(7, 0, 0)
    rv = PS_Mount_Fast(args, strlen(args) + 1, 1, &result);
#else
    rv = PS_Mount(args, strlen(args) + 1, 1, &result);
#endif
  }
  else
    rv = PS_Umount(args, strlen(args) + 1, 1, &result);
  LOG(INFO) << "result = " << rv << ", " << result;
  if (rv != SUCCESS || result != SUCCESS) {
    std::string mount_error = "mount failed. (" + std::to_string(rv) + ", " +
                              std::to_string(result) + ")";
    SetErrMsg(mount_error);
    return FAIL;
  }

  return SUCCESS;
}

static int MountPatchedTPK() {
  int result = FAIL;
  if (!access(LIB_RO_ROOT_DIR "/version", F_OK)) {
    LOG(INFO) << LIB_RO_ROOT_DIR "/version file already exists.";
    if (LwipcEventDone(kChromiumUpdateOrPreloadMountReady) < 0) {
      // The Lwipc api simply calls the ioctl() inside the function.
      // Failure of this API means that there are some kernel issues.
      LOG(ERROR) << kChromiumUpdateOrPreloadMountReady << " Event Failed.";
    }
    return SUCCESS;
  }

  if (!access(PATH_UPDATED_CHROMIUM_EFL_IMG, F_OK)) {
    LOG(INFO) << PATH_UPDATED_CHROMIUM_EFL_IMG << " exists.";
    kTargetTPK = "WebEngine Updated TPK ";
    result = mount(true, MOUNT_ARGS_UPDATED_CHROMIUM);

    if (LwipcEventDone(kChromiumUpdateOrPreloadMountReady) < 0) {
      // The Lwipc api simply calls the ioctl() inside the function.
      // Failure of this API means that there are some kernel issues.
      LOG(ERROR) << kChromiumUpdateOrPreloadMountReady << " Event Failed.";
    }
  }

  if (result != SUCCESS && !access(PATH_PRELOAD_CHROMIUM_EFL_IMG, F_OK)) {
    LOG(INFO) << PATH_PRELOAD_CHROMIUM_EFL_IMG << " exists.";
    kTargetTPK = "WebEngine Preload TPK ";
    result = mount(true, MOUNT_ARGS_PRELOAD_CHROMIUM);

    if (LwipcEventDone(kChromiumUpdateOrPreloadMountReady) < 0) {
      // The Lwipc api simply calls the ioctl() inside the function.
      // Failure of this API means that there are some kernel issues.
      LOG(ERROR) << kChromiumUpdateOrPreloadMountReady << " Event Failed.";
    }
  }

  return result;
}

static void AlertUpgradeTPK() {
  if (!access(PATH_UPGRADE_CHROMIUM_EFL_IMG, F_OK)) {
    LOG(INFO) << PATH_UPGRADE_CHROMIUM_EFL_IMG << " exists.";

    if (LwipcEventDone(kChromiumUpgradeTPKExists) < 0) {
      // The Lwipc api simply calls the ioctl() inside the function.
      // Failure of this API means that there are some kernel issues.
      LOG(ERROR) << kChromiumUpgradeTPKExists << " Event Failed.";
    }
  } else {
    LOG(INFO) << "No WebEngine Upgrade TPK found";
  }
}

static void MountUpgradeTPK() {
  if (!access(LIB_UPGRADE_ROOT_DIR "/version", F_OK)) {
    RequestColdBoot();
    LOG(INFO) << LIB_UPGRADE_ROOT_DIR "/version file already exists.";
  }

  if (!access(PATH_UPGRADE_CHROMIUM_EFL_IMG, F_OK)) {
    LOG(INFO) << PATH_UPGRADE_CHROMIUM_EFL_IMG << " exists.";
    kTargetTPK = "WebEngine Upgrade TPK ";
    mount(true, MOUNT_ARGS_UPGRADE_CHROMIUM);
    if (LwipcEventDone(kChromiumUpgradeMountReady) < 0) {
      // The Lwipc api simply calls the ioctl() inside the function.
      // Failure of this API means that there are some kernel issues.
      LOG(ERROR) << kChromiumUpgradeMountReady << " Event Failed.";
    }
  } else {
    LOG(INFO) << "No WebEngine Upgrade TPK found";
  }
}

enum Options {
  InstallTpk,
  Mount,
  Umount,
  UmountAll,
};

int main(int argc, char* argv[]) {
  LOG(INFO) << argv[0] << " started.";
  bool result = 0;
  bool is_remove_tpk = false;
  std::string args;
  Options option = InstallTpk;

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--mount")) {
      option = Mount;
    } else if (!strcmp(argv[i], "--umount")) {
      option = Umount;
    } else if (!strcmp(argv[i], "--umount-all")) {
      option = UmountAll;
    } else if (option == InstallTpk) {
      if (!strcmp(argv[i], "-d")) {
        is_remove_tpk = true;
      } else {
        args.append(argv[i]);
      }
    } else if (option == Mount || option == Umount) {
      if (args.length() > 0)
        args.append(" ");
      args.append(argv[i]);
    } else {
      LOG(WARN) << "Unknown Argument : " << argv[i];
    }
  }

  if (option == InstallTpk) {  // InstallTpk
    AlertUpgradeTPK();
    // TV does not install tpk. Built-in installer installs.
    // To avoid concurrency problem, a lock must be shared with installer.
    //
    // Mount patched/preload chromium-efl.img (Mandatory)
    result = MountPatchedTPK();
    if (result != SUCCESS)
      return FAIL;

    // Mount upgrade chromium-efl.img (Optional)
    MountUpgradeTPK();
    if (is_remove_tpk) {
      LOG(WARN) << "'-d' option is not supported for tv";
    }
  } else {
    if (option == Mount || option == Umount) {
      result = mount(option == Mount, args.c_str());
    } else if (option == UmountAll) {
      if (!access(LIB_UPGRADE_ROOT_DIR "/version", F_OK)) {
        mount(false, UMOUNT_ARGS_PREFIX LIB_UPGRADE_ROOT_DIR);
      }
      if (!access(LIB_RO_ROOT_DIR "/version", F_OK)) {
        mount(false, UMOUNT_ARGS_PREFIX LIB_RO_ROOT_DIR);
      }
    }
  }

  LOG(INFO) << argv[0] << " ended. result:" << result;
  return result;
}
