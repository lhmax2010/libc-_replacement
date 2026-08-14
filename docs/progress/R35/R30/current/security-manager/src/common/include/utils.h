/*
 * Copyright (c) 2016-2022 Samsung Electronics Co., Ltd. All rights reserved
 *
 * This file is licensed under the terms of MIT License or the Apache License
 * Version 2.0 of your choice. See the LICENSE.MIT file for MIT license details.
 * See the LICENSE file or the notice below for Apache License Version 2.0
 * details.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * @file        utils.h
 * @author      Rafal Krypa <r.krypa@samsung.com>
 * @version     1.0
 * @brief       Utility functions, macros and templates
 */

#pragma once

#include <algorithm>
#include <cxxabi.h>
#include <dpl/exception.h>
#include <dpl/log/log.h>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <time.h>
#include <type_traits>
#include <unistd.h>
#include <vector>
#include <sys/types.h>

#include <security-manager-types.h>

#define SECURITY_MANAGER_API __attribute__((visibility("default")))

namespace SecurityManager {

bool isNumber(const std::string &input);
bool notNumber(const std::string &input);
unsigned toNumber(const std::string &input);

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

/*
 * Decorator function that performs frequently repeated exception handling in
 * SS client API functions. Accepts lambda expression as an argument.
 */
template <class F>
int try_catch(F &&f) {
    try {
        return std::forward<F>(f)();
#if defined(__GLIBCXX__)
    } catch (abi::__forced_unwind &) {
        throw;
#endif
    } catch (const Exception &e) {
        LogError("SecurityManager::Exception " << e.DumpToString());
        std::cerr << "SecurityManager::Exception " << e.DumpToString() << std::endl;
    } catch (const std::bad_alloc &e) {
        LogError("Memory allocation failed: " << e.what());
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return SECURITY_MANAGER_ERROR_MEMORY;
    } catch (const std::system_error &e) {
        LogError("STD system_error: " <<  e.code() << "-" << e.what());
        std::cerr << "STD system_error: " <<  e.code() << "-" << e.what() << std::endl;
    } catch (const std::exception &e) {
        LogError("STD exception " << e.what());
        std::cerr << "STD exception " << e.what() << std::endl;
    } catch (...) {
        LogError("Unknown exception occurred");
        std::cerr << "Unknown exception occurred" << std::endl;
    }
    return SECURITY_MANAGER_ERROR_UNKNOWN;
}

time_t monotonicCoarseNow();

// break include chain that triggers -fno-exceptions in rules-loader
class Credentials;
// Used for measuring function/method/scope execution time
class ScopedTimeStamper {
public:
    ScopedTimeStamper(const std::string & location, const Credentials & creds);
    virtual ~ScopedTimeStamper();

private:
    struct timespec m_start;
    std::string m_locationStr;
};

#ifdef BUILD_TYPE_DEBUG
#define LOG_EXECUTION_TIME(location, creds)    ScopedTimeStamper __stamper(location, creds)
#else
#define LOG_EXECUTION_TIME(location, creds)    do {} while (0)
#endif

// Group operations
void loadGroups(std::vector<gid_t> &vgroups, bool is_nss = false);
int group_vector_to_array(const std::vector<gid_t> &vgroups, gid_t **groups, size_t *groups_count);

// Pointer
template<typename T>
std::unique_ptr<T> makeUnique(T *ptr)
{
    return std::unique_ptr<T>(ptr);
}

// Pointer & deleter func
template<typename T, typename F>
std::unique_ptr<T, F> makeUnique(T *ptr, F func)
{
    return std::unique_ptr<T, F>(ptr, func);
}

// Array - borrowed from C++14
template<typename T>
std::unique_ptr<T> makeUnique(size_t size)
{
    return std::unique_ptr<T>(new typename std::remove_extent<T>::type[size]);
}

template <typename T>
static void vectorRemoveDuplicates(std::vector<T> &vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

template <size_t N>
auto possiblyUnterminatedArrayToString(const char (&array)[N]) {
    return std::string(array, strnlen(array, N));
}

gid_t getGidByName(const std::string& name);
[[maybe_unused]] gid_t getSystemShareGid();

[[maybe_unused]] uid_t getUidByName(const std::string& name);
[[maybe_unused]] uid_t getSystemUid();
} /* namespace SecurityManager */

template <class...T>
bool forkExecWaitpid(const char *prog, const T *...args) {
    const auto pid = fork();
    if (pid < 0)
        return false;
    if (pid) {
        int r;
        return TEMP_FAILURE_RETRY(waitpid(pid, &r, 0)) >= 0 && WIFEXITED(r) && !WEXITSTATUS(r);
    }
    execl(prog, prog, args..., static_cast<char*>(nullptr));
    exit(EXIT_FAILURE);
}

template <class T, size_t S>
constexpr bool allTrue(T (&array)[S]) {
    for (auto &a : array)
        if (!a)
            return false;
    return true;
}

template <class T, size_t S>
constexpr bool allTrue(const std::array<T, S>& array) {
    for (auto &a : array)
        if (!a)
            return false;
    return true;
}

template <class T, class U>
constexpr auto max(const T &a, const U &b) {
    return a < b ? b : a;
}

template <class E>
constexpr auto underlying(const E &e) {
    return std::underlying_type_t<E>(e);
}

template <class T, size_t S>
constexpr size_t arraySize(T (&)[S]) {
    return S;
}
