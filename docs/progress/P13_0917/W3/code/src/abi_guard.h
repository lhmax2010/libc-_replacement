#pragma once
#include <string>
#include <vector>
#include <type_traits>
#include "bridge.h"
#if defined(P12_BUILD_GNU)
#if !defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
#error "GNU-facing compilation did not select GNU headers"
#endif
#if !defined(_GLIBCXX_USE_CXX11_ABI) || _GLIBCXX_USE_CXX11_ABI != 1
#error "This measured GNU interface is CXX11 ABI=1; other ABI needs separate validation"
#endif
#elif defined(P12_BUILD_LIBCXX)
#if !defined(_LIBCPP_VERSION) || defined(__GLIBCXX__)
#error "LLVM-facing compilation did not select libc++ headers"
#endif
#else
#error "Select an explicit standard-library side for each translation unit"
#endif
static_assert(std::is_standard_layout<P12Span>::value && std::is_trivially_copyable<P12Span>::value,"POD span required");
static_assert(std::is_standard_layout<P12CookieState>::value && std::is_trivially_copyable<P12CookieState>::value,"POD state required");
#if defined(__x86_64__)
static_assert(sizeof(void*)==8 && sizeof(P12Span)==16,"x86_64 protocol drift");
static_assert(sizeof(std::vector<unsigned char>)==24,"measured vector baseline drift, not an ABI proof");
#ifdef P12_BUILD_GNU
static_assert(sizeof(std::string)==32,"GNU string baseline drift");
#else
static_assert(sizeof(std::string)==24,"LLVM string baseline drift");
#endif
#endif
