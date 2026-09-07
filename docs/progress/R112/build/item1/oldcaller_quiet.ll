; ModuleID = 'docs/progress/R112/code/item1_oldcaller_cancel.cpp'
source_filename = "docs/progress/R112/code/item1_oldcaller_cancel.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-tizen-linux-gnu"

%"class.std::__1::condition_variable" = type { %union.pthread_cond_t }
%union.pthread_cond_t = type { %struct.__pthread_cond_s }
%struct.__pthread_cond_s = type { %union.__atomic_wide_counter, %union.__atomic_wide_counter, [2 x i32], [2 x i32], i32, i32, [2 x i32] }
%union.__atomic_wide_counter = type { i64 }
%"class.std::__1::mutex" = type { %union.pthread_mutex_t }
%union.pthread_mutex_t = type { %struct.__pthread_mutex_s }
%struct.__pthread_mutex_s = type { i32, i32, i32, i32, i32, i16, i16, %struct.__pthread_internal_list }
%struct.__pthread_internal_list = type { ptr, ptr }
%"struct.std::__1::__atomic_base" = type { %"struct.std::__1::__cxx_atomic_impl" }
%"struct.std::__1::__cxx_atomic_impl" = type { %"struct.std::__1::__cxx_atomic_base_impl" }
%"struct.std::__1::__cxx_atomic_base_impl" = type { i8 }
%"struct.std::__1::__atomic_base.2" = type { %"struct.std::__1::__cxx_atomic_impl.3" }
%"struct.std::__1::__cxx_atomic_impl.3" = type { %"struct.std::__1::__cxx_atomic_base_impl.4" }
%"struct.std::__1::__cxx_atomic_base_impl.4" = type { i32 }
%"class.std::__1::unique_lock" = type <{ ptr, i8, [7 x i8] }>
%"struct.(anonymous namespace)::FrameCleanup" = type { i8 }
%struct.timespec = type { i64, i64 }

$_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqe220108ENS_12memory_orderE = comdat any

$_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqe220108EiNS_12memory_orderE = comdat any

$__clang_call_terminate = comdat any

$_ZNSt3__15mutex13native_handleB9nqe220108Ev = comdat any

$_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE = comdat any

$_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE = comdat any

$_ZNSt3__111unique_lockINS_5mutexEEC2B9nqe220108ERS1_ = comdat any

$_ZNSt3__111unique_lockINS_5mutexEED2B9nqe220108Ev = comdat any

$_ZNSt3__118__cxx_atomic_storeB9nqe220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE = comdat any

$_ZNSt3__122__cxx_atomic_fetch_addB9nqe220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE = comdat any

$_ZNSt3__117__cxx_atomic_loadB9nqe220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE = comdat any

$_ZNSt3__117__cxx_atomic_loadB9nqe220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE = comdat any

@_ZN12_GLOBAL__N_19target_cvE = internal global %"class.std::__1::condition_variable" zeroinitializer, align 8, !dbg !0
@__dso_handle = external hidden global i8
@_ZN12_GLOBAL__N_111real_unlockE = internal global ptr null, align 8, !dbg !340
@.str = private unnamed_addr constant [21 x i8] c"pthread_mutex_unlock\00", align 1, !dbg !347
@_ZN12_GLOBAL__N_113target_nativeE = internal global ptr null, align 8, !dbg !353
@stdout = external global ptr, align 8
@.str.1 = private unnamed_addr constant [19 x i8] c"event=dlsym_failed\00", align 1, !dbg !355
@_ZN12_GLOBAL__N_112target_mutexE = internal global %"class.std::__1::mutex" zeroinitializer, align 8, !dbg !360
@.str.2 = private unnamed_addr constant [20 x i8] c"event=create rc=%d\0A\00", align 1, !dbg !362
@.str.3 = private unnamed_addr constant [28 x i8] c"event=worker_ready_observed\00", align 1, !dbg !367
@.str.4 = private unnamed_addr constant [21 x i8] c"event=wait_confirmed\00", align 1, !dbg !372
@.str.5 = private unnamed_addr constant [84 x i8] c"event=joined cancel_rc=%d join_rc=%d canceled=%d frame_cleanup=%d target_unlock=%d\0A\00", align 1, !dbg !374
@.str.6 = private unnamed_addr constant [35 x i8] c"event=trylock rc=%d errno_busy=%d\0A\00", align 1, !dbg !379
@.str.7 = private unnamed_addr constant [62 x i8] c"event=contender_join create_rc=%d join_rc=%d contender_rc=%d\0A\00", align 1, !dbg !384
@.str.8 = private unnamed_addr constant [26 x i8] c"event=assert expected=%d\0A\00", align 1, !dbg !389
@_ZN12_GLOBAL__N_112track_unlockE = internal global { { i8 } } zeroinitializer, align 1, !dbg !394
@_ZN12_GLOBAL__N_119target_unlock_countE = internal global { { i32 } } zeroinitializer, align 4, !dbg !538
@.str.11 = private unnamed_addr constant [51 x i8] c"event=terminate frame_cleanup=%d target_unlock=%d\0A\00", align 1, !dbg !728
@"__const.~FrameCleanup.marker" = private unnamed_addr constant [21 x i8] c"event=frame_cleanup\0A\00", align 16
@_ZN12_GLOBAL__N_15readyE = internal global { { i8 } } zeroinitializer, align 1, !dbg !733
@_ZN12_GLOBAL__N_119frame_cleanup_countE = internal global { { i32 } } zeroinitializer, align 4, !dbg !735
@.str.14 = private unnamed_addr constant [54 x i8] c"event=contender rc=%d errno_busy=%d errno_timeout=%d\0A\00", align 1, !dbg !737
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 65535, ptr @_GLOBAL__sub_I_item1_oldcaller_cancel.cpp, ptr null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" !dbg !2015 {
  %1 = call i32 @__cxa_atexit(ptr @_ZNSt3__118condition_variableD1Ev, ptr @_ZN12_GLOBAL__N_19target_cvE, ptr @__dso_handle) #2, !dbg !2016
  ret void, !dbg !2016
}

; Function Attrs: nounwind
declare void @_ZNSt3__118condition_variableD1Ev(ptr noundef nonnull align 8 dereferenceable(48)) unnamed_addr #1

; Function Attrs: nounwind
declare i32 @__cxa_atexit(ptr, ptr, ptr) #2

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define i32 @pthread_mutex_unlock(ptr noundef nonnull %0) #3 personality ptr @__gxx_personality_v0 !dbg !2017 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2018, !DIExpression(), !2019)
  %3 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2020
  %4 = icmp eq ptr %3, null, !dbg !2022
  br i1 %4, label %5, label %7, !dbg !2022

5:                                                ; preds = %1
  %6 = call ptr @dlsym(ptr noundef inttoptr (i64 -1 to ptr), ptr noundef @.str) #2, !dbg !2023
  store ptr %6, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2024
  br label %7, !dbg !2025

7:                                                ; preds = %5, %1
  %8 = call noundef zeroext i1 @_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_112track_unlockE, i32 noundef 0) #2, !dbg !2026
  br i1 %8, label %9, label %15, !dbg !2028

9:                                                ; preds = %7
  %10 = load ptr, ptr %2, align 8, !dbg !2029
  %11 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2030
  %12 = icmp eq ptr %10, %11, !dbg !2031
  br i1 %12, label %13, label %15, !dbg !2028

13:                                               ; preds = %9
  %14 = call noundef i32 @_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqe220108EiNS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119target_unlock_countE, i32 noundef 1, i32 noundef 0) #2, !dbg !2032
  br label %15, !dbg !2033

15:                                               ; preds = %13, %9, %7
  %16 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2034
  %17 = icmp eq ptr %16, null, !dbg !2036
  br i1 %17, label %18, label %19, !dbg !2036

18:                                               ; preds = %15
  call void @_Exit(i32 noundef 87) #11, !dbg !2037
  unreachable, !dbg !2037

19:                                               ; preds = %15
  %20 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2038
  %21 = load ptr, ptr %2, align 8, !dbg !2039
  %22 = invoke noundef i32 %20(ptr noundef %21)
          to label %23 unwind label %24, !dbg !2038

23:                                               ; preds = %19
  ret i32 %22, !dbg !2040

24:                                               ; preds = %19
  %25 = landingpad { ptr, i32 }
          catch ptr null, !dbg !2038
  %26 = extractvalue { ptr, i32 } %25, 0, !dbg !2038
  call void @__clang_call_terminate(ptr %26) #11, !dbg !2038
  unreachable, !dbg !2038
}

; Function Attrs: nounwind
declare ptr @dlsym(ptr noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef zeroext i1 @_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) %0, i32 noundef %1) #3 comdat align 2 !dbg !2041 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2042, !DIExpression(), !2044)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2045, !DIExpression(), !2046)
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base", ptr %5, i32 0, i32 0, !dbg !2047
  %7 = load i32, ptr %4, align 4, !dbg !2048
  %8 = call noundef zeroext i1 @_ZNSt3__117__cxx_atomic_loadB9nqe220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %6, i32 noundef %7) #2, !dbg !2049
  ret i1 %8, !dbg !2050
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqe220108EiNS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) %0, i32 noundef %1, i32 noundef %2) #3 comdat align 2 !dbg !2051 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2052, !DIExpression(), !2054)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !2055, !DIExpression(), !2056)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2057, !DIExpression(), !2058)
  %7 = load ptr, ptr %4, align 8
  %8 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base.2", ptr %7, i32 0, i32 0, !dbg !2059
  %9 = load i32, ptr %5, align 4, !dbg !2060
  %10 = load i32, ptr %6, align 4, !dbg !2061
  %11 = call noundef i32 @_ZNSt3__122__cxx_atomic_fetch_addB9nqe220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE(ptr noundef %8, i32 noundef %9, i32 noundef %10) #2, !dbg !2062
  ret i32 %11, !dbg !2063
}

; Function Attrs: noreturn nounwind
declare void @_Exit(i32 noundef) #4

declare i32 @__gxx_personality_v0(...)

; Function Attrs: noinline noreturn nounwind uwtable
define linkonce_odr hidden void @__clang_call_terminate(ptr noundef %0) #5 comdat {
  %2 = call ptr @__cxa_begin_catch(ptr %0) #2
  call void @_ZSt9terminatev() #11
  unreachable
}

declare ptr @__cxa_begin_catch(ptr)

declare void @_ZSt9terminatev()

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define noundef i32 @main() #6 !dbg !2064 {
  %1 = alloca i32, align 4
  %2 = alloca i64, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i64, align 8
  %11 = alloca i32, align 4
  %12 = alloca ptr, align 8
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca i8, align 1
  %16 = alloca i8, align 1
  %17 = alloca i8, align 1
  %18 = alloca i8, align 1
  store i32 0, ptr %1, align 4
  %19 = load ptr, ptr @stdout, align 8, !dbg !2065
  %20 = call i32 @setvbuf(ptr noundef %19, ptr noundef null, i32 noundef 2, i64 noundef 0) #2, !dbg !2066
  %21 = call noundef ptr @_ZSt13set_terminatePFvvE(ptr noundef @_ZN12_GLOBAL__N_116terminate_markerEv) #2, !dbg !2067
  %22 = call ptr @dlsym(ptr noundef inttoptr (i64 -1 to ptr), ptr noundef @.str) #2, !dbg !2068
  store ptr %22, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2069
  %23 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2070
  %24 = icmp eq ptr %23, null, !dbg !2072
  br i1 %24, label %25, label %27, !dbg !2072

25:                                               ; preds = %0
  %26 = call i32 @puts(ptr noundef @.str.1), !dbg !2073
  store i32 68, ptr %1, align 4, !dbg !2075
  br label %147, !dbg !2075

27:                                               ; preds = %0
  %28 = call noundef ptr @_ZNSt3__15mutex13native_handleB9nqe220108Ev(ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE), !dbg !2076
  store ptr %28, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2077
    #dbg_declare(ptr %2, !2078, !DIExpression(), !2080)
    #dbg_declare(ptr %3, !2081, !DIExpression(), !2082)
  %29 = call i32 @pthread_create(ptr noundef %2, ptr noundef null, ptr noundef @_ZN12_GLOBAL__N_16workerEPv, ptr noundef null) #2, !dbg !2083
  store i32 %29, ptr %3, align 4, !dbg !2082
  %30 = load i32, ptr %3, align 4, !dbg !2084
  %31 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %30), !dbg !2085
  %32 = load i32, ptr %3, align 4, !dbg !2086
  %33 = icmp ne i32 %32, 0, !dbg !2088
  br i1 %33, label %34, label %35, !dbg !2088

34:                                               ; preds = %27
  store i32 65, ptr %1, align 4, !dbg !2089
  br label %147, !dbg !2089

35:                                               ; preds = %27
  br label %36, !dbg !2090

36:                                               ; preds = %39, %35
  %37 = call noundef zeroext i1 @_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_15readyE, i32 noundef 2) #2, !dbg !2091
  %38 = xor i1 %37, true, !dbg !2092
  br i1 %38, label %39, label %41, !dbg !2090

39:                                               ; preds = %36
  %40 = call i32 @usleep(i32 noundef 1000), !dbg !2093
  br label %36, !dbg !2090, !llvm.loop !2094

41:                                               ; preds = %36
  %42 = call i32 @puts(ptr noundef @.str.3), !dbg !2097
  call void @_ZNSt3__15mutex4lockEv(ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE), !dbg !2098
  call void @_ZNSt3__15mutex6unlockEv(ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE) #2, !dbg !2099
  %43 = call i32 @puts(ptr noundef @.str.4), !dbg !2100
  call void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_112track_unlockE, i1 noundef zeroext true, i32 noundef 3) #2, !dbg !2101
    #dbg_declare(ptr %4, !2102, !DIExpression(), !2103)
  %44 = load i64, ptr %2, align 8, !dbg !2104
  %45 = call i32 @pthread_cancel(i64 noundef %44), !dbg !2105
  store i32 %45, ptr %4, align 4, !dbg !2103
    #dbg_declare(ptr %5, !2106, !DIExpression(), !2107)
  store ptr null, ptr %5, align 8, !dbg !2107
    #dbg_declare(ptr %6, !2108, !DIExpression(), !2109)
  %46 = load i64, ptr %2, align 8, !dbg !2110
  %47 = call i32 @pthread_join(i64 noundef %46, ptr noundef %5), !dbg !2111
  store i32 %47, ptr %6, align 4, !dbg !2109
  call void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_112track_unlockE, i1 noundef zeroext false, i32 noundef 3) #2, !dbg !2112
    #dbg_declare(ptr %7, !2113, !DIExpression(), !2114)
  %48 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119target_unlock_countE, i32 noundef 0) #2, !dbg !2115
  store i32 %48, ptr %7, align 4, !dbg !2114
    #dbg_declare(ptr %8, !2116, !DIExpression(), !2117)
  %49 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119frame_cleanup_countE, i32 noundef 0) #2, !dbg !2118
  store i32 %49, ptr %8, align 4, !dbg !2117
  %50 = load i32, ptr %4, align 4, !dbg !2119
  %51 = load i32, ptr %6, align 4, !dbg !2120
  %52 = load ptr, ptr %5, align 8, !dbg !2121
  %53 = icmp eq ptr %52, inttoptr (i64 -1 to ptr), !dbg !2122
  %54 = zext i1 %53 to i64, !dbg !2121
  %55 = select i1 %53, i32 1, i32 0, !dbg !2121
  %56 = load i32, ptr %8, align 4, !dbg !2123
  %57 = load i32, ptr %7, align 4, !dbg !2124
  %58 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %50, i32 noundef %51, i32 noundef %55, i32 noundef %56, i32 noundef %57), !dbg !2125
    #dbg_declare(ptr %9, !2126, !DIExpression(), !2127)
  %59 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2128
  %60 = call i32 @pthread_mutex_trylock(ptr noundef %59) #2, !dbg !2129
  store i32 %60, ptr %9, align 4, !dbg !2127
  %61 = load i32, ptr %9, align 4, !dbg !2130
  %62 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, i32 noundef %61, i32 noundef 16), !dbg !2131
  %63 = load i32, ptr %9, align 4, !dbg !2132
  %64 = icmp eq i32 %63, 0, !dbg !2134
  br i1 %64, label %65, label %69, !dbg !2134

65:                                               ; preds = %41
  %66 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2135
  %67 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2136
  %68 = call noundef i32 %66(ptr noundef %67), !dbg !2135
  br label %69, !dbg !2135

69:                                               ; preds = %65, %41
    #dbg_declare(ptr %10, !2137, !DIExpression(), !2138)
    #dbg_declare(ptr %11, !2139, !DIExpression(), !2140)
  %70 = call i32 @pthread_create(ptr noundef %10, ptr noundef null, ptr noundef @_ZN12_GLOBAL__N_19contenderEPv, ptr noundef null) #2, !dbg !2141
  store i32 %70, ptr %11, align 4, !dbg !2140
    #dbg_declare(ptr %12, !2142, !DIExpression(), !2143)
  store ptr null, ptr %12, align 8, !dbg !2143
    #dbg_declare(ptr %13, !2144, !DIExpression(), !2145)
  %71 = load i32, ptr %11, align 4, !dbg !2146
  %72 = icmp eq i32 %71, 0, !dbg !2147
  br i1 %72, label %73, label %76, !dbg !2146

73:                                               ; preds = %69
  %74 = load i64, ptr %10, align 8, !dbg !2148
  %75 = call i32 @pthread_join(i64 noundef %74, ptr noundef %12), !dbg !2149
  br label %77, !dbg !2146

76:                                               ; preds = %69
  br label %77, !dbg !2146

77:                                               ; preds = %76, %73
  %78 = phi i32 [ %75, %73 ], [ -1, %76 ], !dbg !2146
  store i32 %78, ptr %13, align 4, !dbg !2145
    #dbg_declare(ptr %14, !2150, !DIExpression(), !2151)
  %79 = load ptr, ptr %12, align 8, !dbg !2152
  %80 = ptrtoint ptr %79 to i64, !dbg !2153
  %81 = trunc i64 %80 to i32, !dbg !2153
  store i32 %81, ptr %14, align 4, !dbg !2151
  %82 = load i32, ptr %11, align 4, !dbg !2154
  %83 = load i32, ptr %13, align 4, !dbg !2155
  %84 = load i32, ptr %14, align 4, !dbg !2156
  %85 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, i32 noundef %82, i32 noundef %83, i32 noundef %84), !dbg !2157
    #dbg_declare(ptr %15, !2158, !DIExpression(), !2159)
  %86 = load i32, ptr %4, align 4, !dbg !2160
  %87 = icmp eq i32 %86, 0, !dbg !2161
  br i1 %87, label %88, label %94, !dbg !2162

88:                                               ; preds = %77
  %89 = load i32, ptr %6, align 4, !dbg !2163
  %90 = icmp eq i32 %89, 0, !dbg !2164
  br i1 %90, label %91, label %94, !dbg !2165

91:                                               ; preds = %88
  %92 = load ptr, ptr %5, align 8, !dbg !2166
  %93 = icmp eq ptr %92, inttoptr (i64 -1 to ptr), !dbg !2167
  br label %94

94:                                               ; preds = %91, %88, %77
  %95 = phi i1 [ false, %88 ], [ false, %77 ], [ %93, %91 ], !dbg !2168
  %96 = zext i1 %95 to i8, !dbg !2159
  store i8 %96, ptr %15, align 1, !dbg !2159
    #dbg_declare(ptr %16, !2169, !DIExpression(), !2170)
  %97 = load i32, ptr %8, align 4, !dbg !2171
  %98 = icmp eq i32 %97, 1, !dbg !2172
  br i1 %98, label %99, label %108, !dbg !2173

99:                                               ; preds = %94
  %100 = load i32, ptr %7, align 4, !dbg !2174
  %101 = icmp eq i32 %100, 1, !dbg !2175
  br i1 %101, label %102, label %108, !dbg !2176

102:                                              ; preds = %99
  %103 = load i32, ptr %9, align 4, !dbg !2177
  %104 = icmp eq i32 %103, 0, !dbg !2178
  br i1 %104, label %105, label %108, !dbg !2179

105:                                              ; preds = %102
  %106 = load i32, ptr %14, align 4, !dbg !2180
  %107 = icmp eq i32 %106, 0, !dbg !2181
  br label %108

108:                                              ; preds = %105, %102, %99, %94
  %109 = phi i1 [ false, %102 ], [ false, %99 ], [ false, %94 ], [ %107, %105 ], !dbg !2168
  %110 = zext i1 %109 to i8, !dbg !2170
  store i8 %110, ptr %16, align 1, !dbg !2170
    #dbg_declare(ptr %17, !2182, !DIExpression(), !2183)
  %111 = load i32, ptr %8, align 4, !dbg !2184
  %112 = icmp eq i32 %111, 0, !dbg !2185
  br i1 %112, label %113, label %122, !dbg !2186

113:                                              ; preds = %108
  %114 = load i32, ptr %7, align 4, !dbg !2187
  %115 = icmp eq i32 %114, 0, !dbg !2188
  br i1 %115, label %116, label %122, !dbg !2189

116:                                              ; preds = %113
  %117 = load i32, ptr %9, align 4, !dbg !2190
  %118 = icmp eq i32 %117, 16, !dbg !2191
  br i1 %118, label %119, label %122, !dbg !2192

119:                                              ; preds = %116
  %120 = load i32, ptr %14, align 4, !dbg !2193
  %121 = icmp eq i32 %120, 110, !dbg !2194
  br label %122

122:                                              ; preds = %119, %116, %113, %108
  %123 = phi i1 [ false, %116 ], [ false, %113 ], [ false, %108 ], [ %121, %119 ], !dbg !2168
  %124 = zext i1 %123 to i8, !dbg !2183
  store i8 %124, ptr %17, align 1, !dbg !2183
    #dbg_declare(ptr %18, !2195, !DIExpression(), !2196)
  %125 = load i8, ptr %15, align 1, !dbg !2197
  %126 = trunc i8 %125 to i1, !dbg !2197
  br i1 %126, label %127, label %135, !dbg !2198

127:                                              ; preds = %122
  %128 = load i8, ptr %16, align 1, !dbg !2199
  %129 = trunc i8 %128 to i1, !dbg !2199
  br i1 %129, label %133, label %130, !dbg !2200

130:                                              ; preds = %127
  %131 = load i8, ptr %17, align 1, !dbg !2201
  %132 = trunc i8 %131 to i1, !dbg !2201
  br label %133, !dbg !2200

133:                                              ; preds = %130, %127
  %134 = phi i1 [ true, %127 ], [ %132, %130 ]
  br label %135

135:                                              ; preds = %133, %122
  %136 = phi i1 [ false, %122 ], [ %134, %133 ], !dbg !2168
  %137 = zext i1 %136 to i8, !dbg !2196
  store i8 %137, ptr %18, align 1, !dbg !2196
  %138 = load i8, ptr %18, align 1, !dbg !2202
  %139 = trunc i8 %138 to i1, !dbg !2202
  %140 = zext i1 %139 to i64, !dbg !2202
  %141 = select i1 %139, i32 1, i32 0, !dbg !2202
  %142 = call i32 (ptr, ...) @printf(ptr noundef @.str.8, i32 noundef %141), !dbg !2203
  %143 = load i8, ptr %18, align 1, !dbg !2204
  %144 = trunc i8 %143 to i1, !dbg !2204
  %145 = zext i1 %144 to i64, !dbg !2204
  %146 = select i1 %144, i32 0, i32 66, !dbg !2204
  store i32 %146, ptr %1, align 4, !dbg !2205
  br label %147, !dbg !2205

147:                                              ; preds = %135, %34, %25
  %148 = load i32, ptr %1, align 4, !dbg !2206
  ret i32 %148, !dbg !2206
}

; Function Attrs: nounwind
declare i32 @setvbuf(ptr noundef, ptr noundef, i32 noundef, i64 noundef) #1

; Function Attrs: nounwind
declare noundef ptr @_ZSt13set_terminatePFvvE(ptr noundef) #1

; Function Attrs: mustprogress noinline optnone uwtable
define internal void @_ZN12_GLOBAL__N_116terminate_markerEv() #7 !dbg !2207 {
  %1 = alloca [128 x i8], align 16
  %2 = alloca i32, align 4
    #dbg_declare(ptr %1, !2208, !DIExpression(), !2212)
    #dbg_declare(ptr %2, !2213, !DIExpression(), !2214)
  %3 = getelementptr inbounds [128 x i8], ptr %1, i64 0, i64 0, !dbg !2215
  %4 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119frame_cleanup_countE, i32 noundef 0) #2, !dbg !2216
  %5 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119target_unlock_countE, i32 noundef 0) #2, !dbg !2217
  %6 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef %3, i64 noundef 128, ptr noundef @.str.11, i32 noundef %4, i32 noundef %5) #2, !dbg !2218
  store i32 %6, ptr %2, align 4, !dbg !2214
  %7 = load i32, ptr %2, align 4, !dbg !2219
  %8 = icmp sgt i32 %7, 0, !dbg !2221
  br i1 %8, label %9, label %16, !dbg !2221

9:                                                ; preds = %0
  %10 = getelementptr inbounds [128 x i8], ptr %1, i64 0, i64 0, !dbg !2222
  %11 = load i32, ptr %2, align 4, !dbg !2223
  %12 = sext i32 %11 to i64, !dbg !2223
  %13 = call i64 @write(i32 noundef 1, ptr noundef %10, i64 noundef %12), !dbg !2224
  %14 = icmp ne i64 %13, 0, !dbg !2224
  %15 = xor i1 %14, true, !dbg !2225
  br label %16, !dbg !2226

16:                                               ; preds = %9, %0
  call void @_Exit(i32 noundef 86) #11, !dbg !2227
  unreachable, !dbg !2227
}

declare i32 @puts(ptr noundef) #8

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef ptr @_ZNSt3__15mutex13native_handleB9nqe220108Ev(ptr noundef nonnull align 8 dereferenceable(40) %0) #3 comdat align 2 !dbg !2228 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2229, !DIExpression(), !2231)
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %"class.std::__1::mutex", ptr %3, i32 0, i32 0, !dbg !2232
  ret ptr %4, !dbg !2233
}

; Function Attrs: nounwind
declare i32 @pthread_create(ptr noundef, ptr noundef, ptr noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline optnone uwtable
define internal noundef ptr @_ZN12_GLOBAL__N_16workerEPv(ptr noundef %0) #7 !dbg !2234 {
  %2 = alloca ptr, align 8
  %3 = alloca %"class.std::__1::unique_lock", align 8
  %4 = alloca %"struct.(anonymous namespace)::FrameCleanup", align 1
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2237, !DIExpression(), !2238)
  %5 = call i32 @pthread_setcancelstate(i32 noundef 0, ptr noundef null), !dbg !2239
  %6 = call i32 @pthread_setcanceltype(i32 noundef 0, ptr noundef null), !dbg !2240
    #dbg_declare(ptr %3, !2241, !DIExpression(), !2242)
  call void @_ZNSt3__111unique_lockINS_5mutexEEC2B9nqe220108ERS1_(ptr noundef nonnull align 8 dereferenceable(9) %3, ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE), !dbg !2242
    #dbg_declare(ptr %4, !2243, !DIExpression(), !2250)
  call void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_15readyE, i1 noundef zeroext true, i32 noundef 3) #2, !dbg !2251
  call void @_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE(ptr noundef nonnull align 8 dereferenceable(48) @_ZN12_GLOBAL__N_19target_cvE, ptr noundef nonnull align 8 dereferenceable(9) %3) #2, !dbg !2252
  call void @_ZN12_GLOBAL__N_112FrameCleanupD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %4) #2, !dbg !2253
  call void @_ZNSt3__111unique_lockINS_5mutexEED2B9nqe220108Ev(ptr noundef nonnull align 8 dereferenceable(9) %3) #2, !dbg !2253
  ret ptr inttoptr (i64 85 to ptr), !dbg !2253
}

declare i32 @printf(ptr noundef, ...) #8

declare i32 @usleep(i32 noundef) #8

declare void @_ZNSt3__15mutex4lockEv(ptr noundef nonnull align 8 dereferenceable(40)) #8

; Function Attrs: nounwind
declare void @_ZNSt3__15mutex6unlockEv(ptr noundef nonnull align 8 dereferenceable(40)) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) %0, i1 noundef zeroext %1, i32 noundef %2) #3 comdat align 2 !dbg !2254 {
  %4 = alloca ptr, align 8
  %5 = alloca i8, align 1
  %6 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2255, !DIExpression(), !2257)
  %7 = zext i1 %1 to i8
  store i8 %7, ptr %5, align 1
    #dbg_declare(ptr %5, !2258, !DIExpression(), !2259)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2260, !DIExpression(), !2261)
  %8 = load ptr, ptr %4, align 8
  %9 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base", ptr %8, i32 0, i32 0, !dbg !2262
  %10 = load i8, ptr %5, align 1, !dbg !2263
  %11 = trunc i8 %10 to i1, !dbg !2263
  %12 = load i32, ptr %6, align 4, !dbg !2264
  call void @_ZNSt3__118__cxx_atomic_storeB9nqe220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE(ptr noundef %9, i1 noundef zeroext %11, i32 noundef %12) #2, !dbg !2265
  ret void, !dbg !2266
}

declare i32 @pthread_cancel(i64 noundef) #8

declare i32 @pthread_join(i64 noundef, ptr noundef) #8

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) %0, i32 noundef %1) #3 comdat align 2 !dbg !2267 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2268, !DIExpression(), !2270)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2271, !DIExpression(), !2272)
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base.2", ptr %5, i32 0, i32 0, !dbg !2273
  %7 = load i32, ptr %4, align 4, !dbg !2274
  %8 = call noundef i32 @_ZNSt3__117__cxx_atomic_loadB9nqe220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %6, i32 noundef %7) #2, !dbg !2275
  ret i32 %8, !dbg !2276
}

; Function Attrs: nounwind
declare i32 @pthread_mutex_trylock(ptr noundef) #1

; Function Attrs: mustprogress noinline optnone uwtable
define internal noundef ptr @_ZN12_GLOBAL__N_19contenderEPv(ptr noundef %0) #7 !dbg !2277 {
  %2 = alloca ptr, align 8
  %3 = alloca %struct.timespec, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2278, !DIExpression(), !2279)
    #dbg_declare(ptr %3, !2280, !DIExpression(), !2281)
  call void @llvm.memset.p0.i64(ptr align 8 %3, i8 0, i64 16, i1 false), !dbg !2281
  %5 = call i32 @clock_gettime(i32 noundef 0, ptr noundef %3) #2, !dbg !2282
  %6 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 1, !dbg !2283
  %7 = load i64, ptr %6, align 8, !dbg !2284
  %8 = add nsw i64 %7, 300000000, !dbg !2284
  store i64 %8, ptr %6, align 8, !dbg !2284
  %9 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 1, !dbg !2285
  %10 = load i64, ptr %9, align 8, !dbg !2285
  %11 = icmp sge i64 %10, 1000000000, !dbg !2287
  br i1 %11, label %12, label %19, !dbg !2287

12:                                               ; preds = %1
  %13 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 0, !dbg !2288
  %14 = load i64, ptr %13, align 8, !dbg !2290
  %15 = add nsw i64 %14, 1, !dbg !2290
  store i64 %15, ptr %13, align 8, !dbg !2290
  %16 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 1, !dbg !2291
  %17 = load i64, ptr %16, align 8, !dbg !2292
  %18 = sub nsw i64 %17, 1000000000, !dbg !2292
  store i64 %18, ptr %16, align 8, !dbg !2292
  br label %19, !dbg !2293

19:                                               ; preds = %12, %1
    #dbg_declare(ptr %4, !2294, !DIExpression(), !2295)
  %20 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2296
  %21 = call i32 @pthread_mutex_timedlock(ptr noundef %20, ptr noundef %3) #2, !dbg !2297
  store i32 %21, ptr %4, align 4, !dbg !2295
  %22 = load i32, ptr %4, align 4, !dbg !2298
  %23 = call i32 (ptr, ...) @printf(ptr noundef @.str.14, i32 noundef %22, i32 noundef 16, i32 noundef 110), !dbg !2299
  %24 = load i32, ptr %4, align 4, !dbg !2300
  %25 = icmp eq i32 %24, 0, !dbg !2302
  br i1 %25, label %26, label %30, !dbg !2302

26:                                               ; preds = %19
  %27 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2303
  %28 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2304
  %29 = call noundef i32 %27(ptr noundef %28), !dbg !2303
  br label %30, !dbg !2303

30:                                               ; preds = %26, %19
  %31 = load i32, ptr %4, align 4, !dbg !2305
  %32 = sext i32 %31 to i64, !dbg !2305
  %33 = inttoptr i64 %32 to ptr, !dbg !2306
  ret ptr %33, !dbg !2307
}

; Function Attrs: nounwind
declare i32 @snprintf(ptr noundef, i64 noundef, ptr noundef, ...) #1

declare i64 @write(i32 noundef, ptr noundef, i64 noundef) #8

declare i32 @pthread_setcancelstate(i32 noundef, ptr noundef) #8

declare i32 @pthread_setcanceltype(i32 noundef, ptr noundef) #8

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr hidden void @_ZNSt3__111unique_lockINS_5mutexEEC2B9nqe220108ERS1_(ptr noundef nonnull align 8 dereferenceable(9) %0, ptr noundef nonnull align 8 dereferenceable(40) %1) unnamed_addr #7 comdat align 2 !dbg !2308 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2309, !DIExpression(), !2311)
  store ptr %1, ptr %4, align 8
    #dbg_declare(ptr %4, !2312, !DIExpression(), !2313)
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %5, i32 0, i32 0, !dbg !2314
  %7 = load ptr, ptr %4, align 8, !dbg !2315, !nonnull !246, !align !2316
  store ptr %7, ptr %6, align 8, !dbg !2314
  %8 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %5, i32 0, i32 1, !dbg !2317
  store i8 1, ptr %8, align 8, !dbg !2317
  %9 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %5, i32 0, i32 0, !dbg !2318
  %10 = load ptr, ptr %9, align 8, !dbg !2318
  call void @_ZNSt3__15mutex4lockEv(ptr noundef nonnull align 8 dereferenceable(40) %10), !dbg !2320
  ret void, !dbg !2321
}

; Function Attrs: nounwind
declare void @_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE(ptr noundef nonnull align 8 dereferenceable(48), ptr noundef nonnull align 8 dereferenceable(9)) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define internal void @_ZN12_GLOBAL__N_112FrameCleanupD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %0) unnamed_addr #3 align 2 personality ptr @__gxx_personality_v0 !dbg !2322 {
  %2 = alloca ptr, align 8
  %3 = alloca [21 x i8], align 16
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2323, !DIExpression(), !2325)
  %4 = load ptr, ptr %2, align 8
  %5 = call noundef i32 @_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqe220108EiNS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119frame_cleanup_countE, i32 noundef 1, i32 noundef 0) #2, !dbg !2326
    #dbg_declare(ptr %3, !2328, !DIExpression(), !2329)
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %3, ptr align 16 @"__const.~FrameCleanup.marker", i64 21, i1 false), !dbg !2329
  %6 = getelementptr inbounds [21 x i8], ptr %3, i64 0, i64 0, !dbg !2330
  %7 = invoke i64 @write(i32 noundef 1, ptr noundef %6, i64 noundef 20)
          to label %8 unwind label %11, !dbg !2331

8:                                                ; preds = %1
  %9 = icmp ne i64 %7, 0, !dbg !2331
  %10 = xor i1 %9, true, !dbg !2332
  ret void, !dbg !2333

11:                                               ; preds = %1
  %12 = landingpad { ptr, i32 }
          catch ptr null, !dbg !2331
  %13 = extractvalue { ptr, i32 } %12, 0, !dbg !2331
  call void @__clang_call_terminate(ptr %13) #11, !dbg !2331
  unreachable, !dbg !2331
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden void @_ZNSt3__111unique_lockINS_5mutexEED2B9nqe220108Ev(ptr noundef nonnull align 8 dereferenceable(9) %0) unnamed_addr #3 comdat align 2 !dbg !2334 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2335, !DIExpression(), !2336)
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %3, i32 0, i32 1, !dbg !2337
  %5 = load i8, ptr %4, align 8, !dbg !2337
  %6 = trunc i8 %5 to i1, !dbg !2337
  br i1 %6, label %7, label %10, !dbg !2337

7:                                                ; preds = %1
  %8 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %3, i32 0, i32 0, !dbg !2340
  %9 = load ptr, ptr %8, align 8, !dbg !2340
  call void @_ZNSt3__15mutex6unlockEv(ptr noundef nonnull align 8 dereferenceable(40) %9) #2, !dbg !2341
  br label %10, !dbg !2340

10:                                               ; preds = %7, %1
  ret void, !dbg !2342
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #9

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #10

; Function Attrs: nounwind
declare i32 @clock_gettime(i32 noundef, ptr noundef) #1

; Function Attrs: nounwind
declare i32 @pthread_mutex_timedlock(ptr noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden void @_ZNSt3__118__cxx_atomic_storeB9nqe220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE(ptr noundef %0, i1 noundef zeroext %1, i32 noundef %2) #3 comdat !dbg !2343 {
  %4 = alloca ptr, align 8
  %5 = alloca i8, align 1
  %6 = alloca i32, align 4
  %7 = alloca i8, align 1
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2347, !DIExpression(), !2348)
  %8 = zext i1 %1 to i8
  store i8 %8, ptr %5, align 1
    #dbg_declare(ptr %5, !2349, !DIExpression(), !2350)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2351, !DIExpression(), !2352)
  %9 = load ptr, ptr %4, align 8, !dbg !2353
  %10 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl", ptr %9, i32 0, i32 0, !dbg !2354
  %11 = load i32, ptr %6, align 4, !dbg !2355
  %12 = load i8, ptr %5, align 1, !dbg !2356
  %13 = trunc i8 %12 to i1, !dbg !2356
  %14 = zext i1 %13 to i8, !dbg !2357
  store i8 %14, ptr %7, align 1, !dbg !2357
  switch i32 %11, label %15 [
    i32 3, label %17
    i32 5, label %19
  ], !dbg !2357

15:                                               ; preds = %3
  %16 = load i8, ptr %7, align 1, !dbg !2357
  store atomic i8 %16, ptr %10 monotonic, align 1, !dbg !2357
  br label %21, !dbg !2357

17:                                               ; preds = %3
  %18 = load i8, ptr %7, align 1, !dbg !2357
  store atomic i8 %18, ptr %10 release, align 1, !dbg !2357
  br label %21, !dbg !2357

19:                                               ; preds = %3
  %20 = load i8, ptr %7, align 1, !dbg !2357
  store atomic i8 %20, ptr %10 seq_cst, align 1, !dbg !2357
  br label %21, !dbg !2357

21:                                               ; preds = %19, %17, %15
  ret void, !dbg !2358
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNSt3__122__cxx_atomic_fetch_addB9nqe220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE(ptr noundef %0, i32 noundef %1, i32 noundef %2) #3 comdat !dbg !2359 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2363, !DIExpression(), !2364)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !2365, !DIExpression(), !2366)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2367, !DIExpression(), !2368)
  %9 = load ptr, ptr %4, align 8, !dbg !2369
  %10 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl.4", ptr %9, i32 0, i32 0, !dbg !2370
  %11 = load i32, ptr %6, align 4, !dbg !2371
  %12 = load i32, ptr %5, align 4, !dbg !2372
  store i32 %12, ptr %7, align 4, !dbg !2373
  switch i32 %11, label %13 [
    i32 1, label %16
    i32 2, label %16
    i32 3, label %19
    i32 4, label %22
    i32 5, label %25
  ], !dbg !2373

13:                                               ; preds = %3
  %14 = load i32, ptr %7, align 4, !dbg !2373
  %15 = atomicrmw add ptr %10, i32 %14 monotonic, align 4, !dbg !2373
  store i32 %15, ptr %8, align 4, !dbg !2373
  br label %28, !dbg !2373

16:                                               ; preds = %3, %3
  %17 = load i32, ptr %7, align 4, !dbg !2373
  %18 = atomicrmw add ptr %10, i32 %17 acquire, align 4, !dbg !2373
  store i32 %18, ptr %8, align 4, !dbg !2373
  br label %28, !dbg !2373

19:                                               ; preds = %3
  %20 = load i32, ptr %7, align 4, !dbg !2373
  %21 = atomicrmw add ptr %10, i32 %20 release, align 4, !dbg !2373
  store i32 %21, ptr %8, align 4, !dbg !2373
  br label %28, !dbg !2373

22:                                               ; preds = %3
  %23 = load i32, ptr %7, align 4, !dbg !2373
  %24 = atomicrmw add ptr %10, i32 %23 acq_rel, align 4, !dbg !2373
  store i32 %24, ptr %8, align 4, !dbg !2373
  br label %28, !dbg !2373

25:                                               ; preds = %3
  %26 = load i32, ptr %7, align 4, !dbg !2373
  %27 = atomicrmw add ptr %10, i32 %26 seq_cst, align 4, !dbg !2373
  store i32 %27, ptr %8, align 4, !dbg !2373
  br label %28, !dbg !2373

28:                                               ; preds = %25, %22, %19, %16, %13
  %29 = load i32, ptr %8, align 4, !dbg !2373
  ret i32 %29, !dbg !2374
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNSt3__117__cxx_atomic_loadB9nqe220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %0, i32 noundef %1) #3 comdat !dbg !760 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2375, !DIExpression(), !2376)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2377, !DIExpression(), !2378)
  %6 = load ptr, ptr %3, align 8, !dbg !2379
  %7 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl.4", ptr %6, i32 0, i32 0, !dbg !2380
  %8 = load i32, ptr %4, align 4, !dbg !2381
  switch i32 %8, label %9 [
    i32 1, label %11
    i32 2, label %11
    i32 5, label %13
  ], !dbg !2382

9:                                                ; preds = %2
  %10 = load atomic i32, ptr %7 monotonic, align 4, !dbg !2382
  store i32 %10, ptr %5, align 4, !dbg !2382
  br label %15, !dbg !2382

11:                                               ; preds = %2, %2
  %12 = load atomic i32, ptr %7 acquire, align 4, !dbg !2382
  store i32 %12, ptr %5, align 4, !dbg !2382
  br label %15, !dbg !2382

13:                                               ; preds = %2
  %14 = load atomic i32, ptr %7 seq_cst, align 4, !dbg !2382
  store i32 %14, ptr %5, align 4, !dbg !2382
  br label %15, !dbg !2382

15:                                               ; preds = %13, %11, %9
  %16 = load i32, ptr %5, align 4, !dbg !2382
  ret i32 %16, !dbg !2383
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef zeroext i1 @_ZNSt3__117__cxx_atomic_loadB9nqe220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %0, i32 noundef %1) #3 comdat !dbg !767 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i8, align 1
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2384, !DIExpression(), !2385)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2386, !DIExpression(), !2387)
  %6 = load ptr, ptr %3, align 8, !dbg !2388
  %7 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl", ptr %6, i32 0, i32 0, !dbg !2389
  %8 = load i32, ptr %4, align 4, !dbg !2390
  switch i32 %8, label %9 [
    i32 1, label %11
    i32 2, label %11
    i32 5, label %13
  ], !dbg !2391

9:                                                ; preds = %2
  %10 = load atomic i8, ptr %7 monotonic, align 1, !dbg !2391
  store i8 %10, ptr %5, align 1, !dbg !2391
  br label %15, !dbg !2391

11:                                               ; preds = %2, %2
  %12 = load atomic i8, ptr %7 acquire, align 1, !dbg !2391
  store i8 %12, ptr %5, align 1, !dbg !2391
  br label %15, !dbg !2391

13:                                               ; preds = %2
  %14 = load atomic i8, ptr %7 seq_cst, align 1, !dbg !2391
  store i8 %14, ptr %5, align 1, !dbg !2391
  br label %15, !dbg !2391

15:                                               ; preds = %13, %11, %9
  %16 = load i8, ptr %5, align 1, !dbg !2391
  %17 = trunc i8 %16 to i1, !dbg !2391
  ret i1 %17, !dbg !2392
}

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_item1_oldcaller_cancel.cpp() #0 section ".text.startup" !dbg !2393 {
  call void @__cxx_global_var_init(), !dbg !2395
  ret void
}

attributes #0 = { noinline uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }
attributes #3 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { noinline noreturn nounwind uwtable "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #9 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #10 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #11 = { noreturn nounwind }

!llvm.dbg.cu = !{!742}
!llvm.module.flags = !{!2008, !2009, !2010, !2011, !2012, !2013}
!llvm.ident = !{!2014}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "target_cv", linkageName: "_ZN12_GLOBAL__N_19target_cvE", scope: !2, file: !3, line: 15, type: !4, isLocal: true, isDefinition: true)
!2 = !DINamespace(scope: null)
!3 = !DIFile(filename: "docs/progress/R112/code/item1_oldcaller_cancel.cpp", directory: "/home/toolchain/development/libcxx_runtime_validation", checksumkind: CSK_MD5, checksum: "f1b61fbe14c257e9dd775daa7fc78efa")
!4 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "condition_variable", scope: !6, file: !5, line: 89, size: 384, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !8, identifier: "_ZTSNSt3__118condition_variableE")
!5 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__condition_variable/condition_variable.h", directory: "/home/toolchain/development")
!6 = !DINamespace(name: "__1", scope: !7, exportSymbols: true)
!7 = !DINamespace(name: "std", scope: null)
!8 = !{!9, !49, !53, !54, !59, !63, !64, !65, !208, !213, !219}
!9 = !DIDerivedType(tag: DW_TAG_member, name: "__cv_", scope: !4, file: !5, line: 90, baseType: !10, size: 384)
!10 = !DIDerivedType(tag: DW_TAG_typedef, name: "__libcpp_condvar_t", scope: !6, file: !11, line: 111, baseType: !12)
!11 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__thread/support/pthread.h", directory: "/home/toolchain/development")
!12 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_cond_t", file: !13, line: 80, baseType: !14)
!13 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/pthreadtypes.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "8a5acdbeec491eca11cf81cb1ef77ea7")
!14 = distinct !DICompositeType(tag: DW_TAG_union_type, file: !13, line: 75, size: 384, flags: DIFlagTypePassByValue, elements: !15, identifier: "_ZTS14pthread_cond_t")
!15 = !{!16, !42, !47}
!16 = !DIDerivedType(tag: DW_TAG_member, name: "__data", scope: !14, file: !13, line: 77, baseType: !17, size: 384)
!17 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__pthread_cond_s", file: !18, line: 94, size: 384, flags: DIFlagTypePassByValue, elements: !19, identifier: "_ZTS16__pthread_cond_s")
!18 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/thread-shared-types.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "b9a7199822bce372686baacd32a9f4f3")
!19 = !{!20, !33, !34, !38, !39, !40, !41}
!20 = !DIDerivedType(tag: DW_TAG_member, name: "__wseq", scope: !17, file: !18, line: 96, baseType: !21, size: 64)
!21 = !DIDerivedType(tag: DW_TAG_typedef, name: "__atomic_wide_counter", file: !22, line: 33, baseType: !23)
!22 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/atomic_wide_counter.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "d1ec99b0f6e36c0a42ac284d459cbd38")
!23 = distinct !DICompositeType(tag: DW_TAG_union_type, file: !22, line: 25, size: 64, flags: DIFlagTypePassByValue, elements: !24, identifier: "_ZTS21__atomic_wide_counter")
!24 = !{!25, !27}
!25 = !DIDerivedType(tag: DW_TAG_member, name: "__value64", scope: !23, file: !22, line: 27, baseType: !26, size: 64)
!26 = !DIBasicType(name: "unsigned long long", size: 64, encoding: DW_ATE_unsigned)
!27 = !DIDerivedType(tag: DW_TAG_member, name: "__value32", scope: !23, file: !22, line: 32, baseType: !28, size: 64)
!28 = distinct !DICompositeType(tag: DW_TAG_structure_type, scope: !23, file: !22, line: 28, size: 64, flags: DIFlagTypePassByValue, elements: !29, identifier: "_ZTSN21__atomic_wide_counterUt_E")
!29 = !{!30, !32}
!30 = !DIDerivedType(tag: DW_TAG_member, name: "__low", scope: !28, file: !22, line: 30, baseType: !31, size: 32)
!31 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "__high", scope: !28, file: !22, line: 31, baseType: !31, size: 32, offset: 32)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "__g1_start", scope: !17, file: !18, line: 97, baseType: !21, size: 64, offset: 64)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "__g_refs", scope: !17, file: !18, line: 98, baseType: !35, size: 64, offset: 128)
!35 = !DICompositeType(tag: DW_TAG_array_type, baseType: !31, size: 64, elements: !36)
!36 = !{!37}
!37 = !DISubrange(count: 2)
!38 = !DIDerivedType(tag: DW_TAG_member, name: "__g_size", scope: !17, file: !18, line: 99, baseType: !35, size: 64, offset: 192)
!39 = !DIDerivedType(tag: DW_TAG_member, name: "__g1_orig_size", scope: !17, file: !18, line: 100, baseType: !31, size: 32, offset: 256)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "__wrefs", scope: !17, file: !18, line: 101, baseType: !31, size: 32, offset: 288)
!41 = !DIDerivedType(tag: DW_TAG_member, name: "__g_signals", scope: !17, file: !18, line: 102, baseType: !35, size: 64, offset: 320)
!42 = !DIDerivedType(tag: DW_TAG_member, name: "__size", scope: !14, file: !13, line: 78, baseType: !43, size: 384)
!43 = !DICompositeType(tag: DW_TAG_array_type, baseType: !44, size: 384, elements: !45)
!44 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!45 = !{!46}
!46 = !DISubrange(count: 48)
!47 = !DIDerivedType(tag: DW_TAG_member, name: "__align", scope: !14, file: !13, line: 79, baseType: !48, size: 64)
!48 = !DIBasicType(name: "long long", size: 64, encoding: DW_ATE_signed)
!49 = !DISubprogram(name: "condition_variable", linkageName: "_ZNSt3__118condition_variableC4B9nqe220108Ev", scope: !4, file: !5, line: 93, type: !50, scopeLine: 93, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!50 = !DISubroutineType(types: !51)
!51 = !{null, !52}
!52 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!53 = !DISubprogram(name: "~condition_variable", linkageName: "_ZNSt3__118condition_variableD4Ev", scope: !4, file: !5, line: 98, type: !50, scopeLine: 98, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!54 = !DISubprogram(name: "condition_variable", linkageName: "_ZNSt3__118condition_variableC4ERKS0_", scope: !4, file: !5, line: 101, type: !55, scopeLine: 101, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!55 = !DISubroutineType(types: !56)
!56 = !{null, !52, !57}
!57 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !58, size: 64)
!58 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !4)
!59 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__118condition_variableaSERKS0_", scope: !4, file: !5, line: 102, type: !60, scopeLine: 102, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!60 = !DISubroutineType(types: !61)
!61 = !{!62, !52, !57}
!62 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !4, size: 64)
!63 = !DISubprogram(name: "notify_one", linkageName: "_ZNSt3__118condition_variable10notify_oneEv", scope: !4, file: !5, line: 104, type: !50, scopeLine: 104, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!64 = !DISubprogram(name: "notify_all", linkageName: "_ZNSt3__118condition_variable10notify_allEv", scope: !4, file: !5, line: 105, type: !50, scopeLine: 105, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!65 = !DISubprogram(name: "wait", linkageName: "_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE", scope: !4, file: !5, line: 107, type: !66, scopeLine: 107, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!66 = !DISubroutineType(types: !67)
!67 = !{null, !52, !68}
!68 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !69, size: 64)
!69 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "unique_lock<std::__1::mutex>", scope: !6, file: !70, line: 32, size: 128, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !71, templateParams: !206, identifier: "_ZTSNSt3__111unique_lockINS_5mutexEEE")
!70 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__mutex/unique_lock.h", directory: "/home/toolchain/development")
!71 = !{!72, !134, !135, !139, !143, !153, !162, !171, !172, !177, !180, !184, !187, !188, !191, !192, !195, !198, !202, !203}
!72 = !DIDerivedType(tag: DW_TAG_member, name: "__m_", scope: !69, file: !70, line: 37, baseType: !73, size: 64)
!73 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !74, size: 64)
!74 = !DIDerivedType(tag: DW_TAG_typedef, name: "mutex_type", scope: !69, file: !70, line: 34, baseType: !75, flags: DIFlagPublic)
!75 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "mutex", scope: !6, file: !76, line: 24, size: 320, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !77, identifier: "_ZTSNSt3__15mutexE")
!76 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__mutex/mutex.h", directory: "/home/toolchain/development")
!77 = !{!78, !109, !113, !118, !122, !123, !124, !128, !129}
!78 = !DIDerivedType(tag: DW_TAG_member, name: "__m_", scope: !75, file: !76, line: 25, baseType: !79, size: 320)
!79 = !DIDerivedType(tag: DW_TAG_typedef, name: "__libcpp_mutex_t", scope: !6, file: !11, line: 47, baseType: !80)
!80 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_mutex_t", file: !13, line: 72, baseType: !81)
!81 = distinct !DICompositeType(tag: DW_TAG_union_type, file: !13, line: 67, size: 320, flags: DIFlagTypePassByValue, elements: !82, identifier: "_ZTS15pthread_mutex_t")
!82 = !{!83, !103, !107}
!83 = !DIDerivedType(tag: DW_TAG_member, name: "__data", scope: !81, file: !13, line: 69, baseType: !84, size: 320)
!84 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__pthread_mutex_s", file: !85, line: 22, size: 320, flags: DIFlagTypePassByValue, elements: !86, identifier: "_ZTS17__pthread_mutex_s")
!85 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/struct_mutex.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "6b075eca9ab0e2d761b2afc4ecfab776")
!86 = !{!87, !89, !90, !91, !92, !93, !95, !96}
!87 = !DIDerivedType(tag: DW_TAG_member, name: "__lock", scope: !84, file: !85, line: 24, baseType: !88, size: 32)
!88 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!89 = !DIDerivedType(tag: DW_TAG_member, name: "__count", scope: !84, file: !85, line: 25, baseType: !31, size: 32, offset: 32)
!90 = !DIDerivedType(tag: DW_TAG_member, name: "__owner", scope: !84, file: !85, line: 26, baseType: !88, size: 32, offset: 64)
!91 = !DIDerivedType(tag: DW_TAG_member, name: "__nusers", scope: !84, file: !85, line: 28, baseType: !31, size: 32, offset: 96)
!92 = !DIDerivedType(tag: DW_TAG_member, name: "__kind", scope: !84, file: !85, line: 32, baseType: !88, size: 32, offset: 128)
!93 = !DIDerivedType(tag: DW_TAG_member, name: "__spins", scope: !84, file: !85, line: 34, baseType: !94, size: 16, offset: 160)
!94 = !DIBasicType(name: "short", size: 16, encoding: DW_ATE_signed)
!95 = !DIDerivedType(tag: DW_TAG_member, name: "__elision", scope: !84, file: !85, line: 35, baseType: !94, size: 16, offset: 176)
!96 = !DIDerivedType(tag: DW_TAG_member, name: "__list", scope: !84, file: !85, line: 36, baseType: !97, size: 128, offset: 192)
!97 = !DIDerivedType(tag: DW_TAG_typedef, name: "__pthread_list_t", file: !18, line: 55, baseType: !98)
!98 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__pthread_internal_list", file: !18, line: 51, size: 128, flags: DIFlagTypePassByValue, elements: !99, identifier: "_ZTS23__pthread_internal_list")
!99 = !{!100, !102}
!100 = !DIDerivedType(tag: DW_TAG_member, name: "__prev", scope: !98, file: !18, line: 53, baseType: !101, size: 64)
!101 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !98, size: 64)
!102 = !DIDerivedType(tag: DW_TAG_member, name: "__next", scope: !98, file: !18, line: 54, baseType: !101, size: 64, offset: 64)
!103 = !DIDerivedType(tag: DW_TAG_member, name: "__size", scope: !81, file: !13, line: 70, baseType: !104, size: 320)
!104 = !DICompositeType(tag: DW_TAG_array_type, baseType: !44, size: 320, elements: !105)
!105 = !{!106}
!106 = !DISubrange(count: 40)
!107 = !DIDerivedType(tag: DW_TAG_member, name: "__align", scope: !81, file: !13, line: 71, baseType: !108, size: 64)
!108 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!109 = !DISubprogram(name: "mutex", linkageName: "_ZNSt3__15mutexC4B9nqe220108Ev", scope: !75, file: !76, line: 28, type: !110, scopeLine: 28, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!110 = !DISubroutineType(types: !111)
!111 = !{null, !112}
!112 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !75, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!113 = !DISubprogram(name: "mutex", linkageName: "_ZNSt3__15mutexC4ERKS0_", scope: !75, file: !76, line: 30, type: !114, scopeLine: 30, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!114 = !DISubroutineType(types: !115)
!115 = !{null, !112, !116}
!116 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !117, size: 64)
!117 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !75)
!118 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__15mutexaSERKS0_", scope: !75, file: !76, line: 31, type: !119, scopeLine: 31, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!119 = !DISubroutineType(types: !120)
!120 = !{!121, !112, !116}
!121 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !75, size: 64)
!122 = !DISubprogram(name: "~mutex", linkageName: "_ZNSt3__15mutexD4B9nqe220108Ev", scope: !75, file: !76, line: 34, type: !110, scopeLine: 34, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!123 = !DISubprogram(name: "lock", linkageName: "_ZNSt3__15mutex4lockEv", scope: !75, file: !76, line: 39, type: !110, scopeLine: 39, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!124 = !DISubprogram(name: "try_lock", linkageName: "_ZNSt3__15mutex8try_lockEv", scope: !75, file: !76, line: 40, type: !125, scopeLine: 40, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!125 = !DISubroutineType(types: !126)
!126 = !{!127, !112}
!127 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!128 = !DISubprogram(name: "unlock", linkageName: "_ZNSt3__15mutex6unlockEv", scope: !75, file: !76, line: 41, type: !110, scopeLine: 41, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!129 = !DISubprogram(name: "native_handle", linkageName: "_ZNSt3__15mutex13native_handleB9nqe220108Ev", scope: !75, file: !76, line: 44, type: !130, scopeLine: 44, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!130 = !DISubroutineType(types: !131)
!131 = !{!132, !112}
!132 = !DIDerivedType(tag: DW_TAG_typedef, name: "native_handle_type", scope: !75, file: !76, line: 43, baseType: !133, flags: DIFlagPublic)
!133 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !79, size: 64)
!134 = !DIDerivedType(tag: DW_TAG_member, name: "__owns_", scope: !69, file: !70, line: 38, baseType: !127, size: 8, offset: 64)
!135 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqe220108Ev", scope: !69, file: !70, line: 41, type: !136, scopeLine: 41, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!136 = !DISubroutineType(types: !137)
!137 = !{null, !138}
!138 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !69, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!139 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqe220108ERS1_", scope: !69, file: !70, line: 42, type: !140, scopeLine: 42, flags: DIFlagPublic | DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!140 = !DISubroutineType(types: !141)
!141 = !{null, !138, !142}
!142 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !74, size: 64)
!143 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqe220108ERS1_NS_12defer_lock_tE", scope: !69, file: !70, line: 47, type: !144, scopeLine: 47, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!144 = !DISubroutineType(types: !145)
!145 = !{null, !138, !142, !146}
!146 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "defer_lock_t", scope: !6, file: !147, line: 20, size: 8, flags: DIFlagTypePassByValue, elements: !148, identifier: "_ZTSNSt3__112defer_lock_tE")
!147 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__mutex/tag_types.h", directory: "/home/toolchain/development")
!148 = !{!149}
!149 = !DISubprogram(name: "defer_lock_t", linkageName: "_ZNSt3__112defer_lock_tC4Ev", scope: !146, file: !147, line: 21, type: !150, scopeLine: 21, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!150 = !DISubroutineType(types: !151)
!151 = !{null, !152}
!152 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !146, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!153 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqe220108ERS1_NS_13try_to_lock_tE", scope: !69, file: !70, line: 51, type: !154, scopeLine: 51, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!154 = !DISubroutineType(types: !155)
!155 = !{null, !138, !142, !156}
!156 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "try_to_lock_t", scope: !6, file: !147, line: 24, size: 8, flags: DIFlagTypePassByValue, elements: !157, identifier: "_ZTSNSt3__113try_to_lock_tE")
!157 = !{!158}
!158 = !DISubprogram(name: "try_to_lock_t", linkageName: "_ZNSt3__113try_to_lock_tC4Ev", scope: !156, file: !147, line: 25, type: !159, scopeLine: 25, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!159 = !DISubroutineType(types: !160)
!160 = !{null, !161}
!161 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !156, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!162 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqe220108ERS1_NS_12adopt_lock_tE", scope: !69, file: !70, line: 54, type: !163, scopeLine: 54, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!163 = !DISubroutineType(types: !164)
!164 = !{null, !138, !142, !165}
!165 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "adopt_lock_t", scope: !6, file: !147, line: 28, size: 8, flags: DIFlagTypePassByValue, elements: !166, identifier: "_ZTSNSt3__112adopt_lock_tE")
!166 = !{!167}
!167 = !DISubprogram(name: "adopt_lock_t", linkageName: "_ZNSt3__112adopt_lock_tC4Ev", scope: !165, file: !147, line: 29, type: !168, scopeLine: 29, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!168 = !DISubroutineType(types: !169)
!169 = !{null, !170}
!170 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !165, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!171 = !DISubprogram(name: "~unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEED4B9nqe220108Ev", scope: !69, file: !70, line: 65, type: !136, scopeLine: 65, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!172 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4ERKS2_", scope: !69, file: !70, line: 70, type: !173, scopeLine: 70, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!173 = !DISubroutineType(types: !174)
!174 = !{null, !138, !175}
!175 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !176, size: 64)
!176 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !69)
!177 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEaSERKS2_", scope: !69, file: !70, line: 71, type: !178, scopeLine: 71, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!178 = !DISubroutineType(types: !179)
!179 = !{!68, !138, !175}
!180 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqe220108EOS2_", scope: !69, file: !70, line: 73, type: !181, scopeLine: 73, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!181 = !DISubroutineType(types: !182)
!182 = !{null, !138, !183}
!183 = !DIDerivedType(tag: DW_TAG_rvalue_reference_type, baseType: !69, size: 64)
!184 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEaSB9nqe220108EOS2_", scope: !69, file: !70, line: 80, type: !185, scopeLine: 80, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!185 = !DISubroutineType(types: !186)
!186 = !{!68, !138, !183}
!187 = !DISubprogram(name: "lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE4lockB9nqe220108Ev", scope: !69, file: !70, line: 86, type: !136, scopeLine: 86, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!188 = !DISubprogram(name: "try_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE8try_lockB9nqe220108Ev", scope: !69, file: !70, line: 87, type: !189, scopeLine: 87, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!189 = !DISubroutineType(types: !190)
!190 = !{!127, !138}
!191 = !DISubprogram(name: "unlock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE6unlockB9nqe220108Ev", scope: !69, file: !70, line: 95, type: !136, scopeLine: 95, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!192 = !DISubprogram(name: "swap", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE4swapB9nqe220108ERS2_", scope: !69, file: !70, line: 97, type: !193, scopeLine: 97, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!193 = !DISubroutineType(types: !194)
!194 = !{null, !138, !68}
!195 = !DISubprogram(name: "release", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE7releaseB9nqe220108Ev", scope: !69, file: !70, line: 102, type: !196, scopeLine: 102, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!196 = !DISubroutineType(types: !197)
!197 = !{!73, !138}
!198 = !DISubprogram(name: "owns_lock", linkageName: "_ZNKSt3__111unique_lockINS_5mutexEE9owns_lockB9nqe220108Ev", scope: !69, file: !70, line: 109, type: !199, scopeLine: 109, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!199 = !DISubroutineType(types: !200)
!200 = !{!127, !201}
!201 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !176, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!202 = !DISubprogram(name: "operator bool", linkageName: "_ZNKSt3__111unique_lockINS_5mutexEEcvbB9nqe220108Ev", scope: !69, file: !70, line: 110, type: !199, scopeLine: 110, flags: DIFlagPublic | DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!203 = !DISubprogram(name: "mutex", linkageName: "_ZNKSt3__111unique_lockINS_5mutexEE5mutexB9nqe220108Ev", scope: !69, file: !70, line: 111, type: !204, scopeLine: 111, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!204 = !DISubroutineType(types: !205)
!205 = !{!73, !201}
!206 = !{!207}
!207 = !DITemplateTypeParameter(name: "_Mutex", type: !75)
!208 = !DISubprogram(name: "native_handle", linkageName: "_ZNSt3__118condition_variable13native_handleB9nqe220108Ev", scope: !4, file: !5, line: 173, type: !209, scopeLine: 173, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!209 = !DISubroutineType(types: !210)
!210 = !{!211, !52}
!211 = !DIDerivedType(tag: DW_TAG_typedef, name: "native_handle_type", scope: !4, file: !5, line: 172, baseType: !212, flags: DIFlagPublic)
!212 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !10, size: 64)
!213 = !DISubprogram(name: "__do_timed_wait", linkageName: "_ZNSt3__118condition_variable15__do_timed_waitERNS_11unique_lockINS_5mutexEEENS_6chrono10time_pointINS5_12system_clockENS5_8durationIxNS_5ratioILl1ELl1000000000EEEEEEE", scope: !4, file: !5, line: 177, type: !214, scopeLine: 177, flags: DIFlagPrototyped, spFlags: 0)
!214 = !DISubroutineType(types: !215)
!215 = !{null, !52, !68, !216}
!216 = !DICompositeType(tag: DW_TAG_class_type, name: "time_point<std::__1::chrono::system_clock, std::__1::chrono::duration<long long, std::__1::ratio<1L, 1000000000L> > >", scope: !218, file: !217, line: 36, size: 64, flags: DIFlagFwdDecl | DIFlagNonTrivial, identifier: "_ZTSNSt3__16chrono10time_pointINS0_12system_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEE")
!217 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__chrono/time_point.h", directory: "/home/toolchain/development")
!218 = !DINamespace(name: "chrono", scope: !6)
!219 = !DISubprogram(name: "__do_timed_wait", linkageName: "_ZNSt3__118condition_variable15__do_timed_waitB9nqe220108ERNS_11unique_lockINS_5mutexEEENS_6chrono10time_pointINS5_12steady_clockENS5_8durationIxNS_5ratioILl1ELl1000000000EEEEEEE", scope: !4, file: !5, line: 180, type: !220, scopeLine: 180, flags: DIFlagPrototyped, spFlags: 0)
!220 = !DISubroutineType(types: !221)
!221 = !{null, !52, !68, !222}
!222 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "time_point<std::__1::chrono::steady_clock, std::__1::chrono::duration<long long, std::__1::ratio<1L, 1000000000L> > >", scope: !218, file: !217, line: 36, size: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !223, templateParams: !328, identifier: "_ZTSNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEE")
!223 = !{!224, !297, !301, !306, !311, !315, !318, !319, !320, !323, !324, !327}
!224 = !DIDerivedType(tag: DW_TAG_member, name: "__d_", scope: !222, file: !217, line: 46, baseType: !225, size: 64)
!225 = !DIDerivedType(tag: DW_TAG_typedef, name: "duration", scope: !222, file: !217, line: 41, baseType: !226, flags: DIFlagPublic)
!226 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "duration<long long, std::__1::ratio<1L, 1000000000L> >", scope: !218, file: !227, line: 166, size: 64, flags: DIFlagTypePassByValue, elements: !228, templateParams: !277, identifier: "_ZTSNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEE")
!227 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__chrono/duration.h", directory: "/home/toolchain/development")
!228 = !{!229, !231, !235, !240, !249, !250, !254, !257, !258, !259, !263, !264, !269, !270, !271, !272, !275, !276}
!229 = !DIDerivedType(tag: DW_TAG_member, name: "__rep_", scope: !226, file: !227, line: 203, baseType: !230, size: 64)
!230 = !DIDerivedType(tag: DW_TAG_typedef, name: "rep", scope: !226, file: !227, line: 199, baseType: !48, flags: DIFlagPublic)
!231 = !DISubprogram(name: "duration", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEC4Ev", scope: !226, file: !227, line: 207, type: !232, scopeLine: 207, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!232 = !DISubroutineType(types: !233)
!233 = !{null, !234}
!234 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !226, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!235 = !DISubprogram(name: "count", linkageName: "_ZNKSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE5countB9nqe220108Ev", scope: !226, file: !227, line: 230, type: !236, scopeLine: 230, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!236 = !DISubroutineType(types: !237)
!237 = !{!230, !238}
!238 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !239, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!239 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !226)
!240 = !DISubprogram(name: "operator+", linkageName: "_ZNKSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEpsB9nqe220108Ev", scope: !226, file: !227, line: 234, type: !241, scopeLine: 234, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!241 = !DISubroutineType(types: !242)
!242 = !{!243, !238}
!243 = !DIDerivedType(tag: DW_TAG_typedef, name: "type", scope: !245, file: !244, line: 22, baseType: !226)
!244 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__type_traits/type_identity.h", directory: "/home/toolchain/development")
!245 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__type_identity<std::__1::chrono::duration<long long, std::__1::ratio<1L, 1000000000L> > >", scope: !6, file: !244, line: 21, size: 8, flags: DIFlagTypePassByValue, elements: !246, templateParams: !247, identifier: "_ZTSNSt3__115__type_identityINS_6chrono8durationIxNS_5ratioILl1ELl1000000000EEEEEEE")
!246 = !{}
!247 = !{!248}
!248 = !DITemplateTypeParameter(name: "_Tp", type: !226)
!249 = !DISubprogram(name: "operator-", linkageName: "_ZNKSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEngB9nqe220108Ev", scope: !226, file: !227, line: 237, type: !241, scopeLine: 237, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!250 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEppB9nqe220108Ev", scope: !226, file: !227, line: 240, type: !251, scopeLine: 240, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!251 = !DISubroutineType(types: !252)
!252 = !{!253, !234}
!253 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !226, size: 64)
!254 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEppB9nqe220108Ei", scope: !226, file: !227, line: 244, type: !255, scopeLine: 244, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!255 = !DISubroutineType(types: !256)
!256 = !{!226, !234, !88}
!257 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmmB9nqe220108Ev", scope: !226, file: !227, line: 245, type: !251, scopeLine: 245, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!258 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmmB9nqe220108Ei", scope: !226, file: !227, line: 249, type: !255, scopeLine: 249, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!259 = !DISubprogram(name: "operator+=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEpLB9nqe220108ERKS4_", scope: !226, file: !227, line: 251, type: !260, scopeLine: 251, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!260 = !DISubroutineType(types: !261)
!261 = !{!253, !234, !262}
!262 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !239, size: 64)
!263 = !DISubprogram(name: "operator-=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmIB9nqe220108ERKS4_", scope: !226, file: !227, line: 255, type: !260, scopeLine: 255, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!264 = !DISubprogram(name: "operator*=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmLB9nqe220108ERKx", scope: !226, file: !227, line: 260, type: !265, scopeLine: 260, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!265 = !DISubroutineType(types: !266)
!266 = !{!253, !234, !267}
!267 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !268, size: 64)
!268 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !230)
!269 = !DISubprogram(name: "operator/=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEdVB9nqe220108ERKx", scope: !226, file: !227, line: 264, type: !265, scopeLine: 264, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!270 = !DISubprogram(name: "operator%=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEErMB9nqe220108ERKx", scope: !226, file: !227, line: 268, type: !265, scopeLine: 268, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!271 = !DISubprogram(name: "operator%=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEErMB9nqe220108ERKS4_", scope: !226, file: !227, line: 272, type: !260, scopeLine: 272, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!272 = !DISubprogram(name: "zero", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE4zeroB9nqe220108Ev", scope: !226, file: !227, line: 279, type: !273, scopeLine: 279, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!273 = !DISubroutineType(types: !274)
!274 = !{!226}
!275 = !DISubprogram(name: "min", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE3minB9nqe220108Ev", scope: !226, file: !227, line: 282, type: !273, scopeLine: 282, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!276 = !DISubprogram(name: "max", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE3maxB9nqe220108Ev", scope: !226, file: !227, line: 285, type: !273, scopeLine: 285, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!277 = !{!278, !279}
!278 = !DITemplateTypeParameter(name: "_Rep", type: !48)
!279 = !DITemplateTypeParameter(name: "_Period", type: !280)
!280 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "ratio<1L, 1000000000L>", scope: !6, file: !281, line: 232, size: 8, flags: DIFlagTypePassByValue, elements: !282, templateParams: !294, identifier: "_ZTSNSt3__15ratioILl1ELl1000000000EEE")
!281 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/ratio", directory: "/home/toolchain/development")
!282 = !{!283, !289, !290, !291, !292, !293}
!283 = !DIDerivedType(tag: DW_TAG_variable, name: "__na", scope: !280, file: !281, line: 236, baseType: !284, flags: DIFlagStaticMember, extraData: i64 1)
!284 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !285)
!285 = !DIDerivedType(tag: DW_TAG_typedef, name: "intmax_t", file: !286, line: 90, baseType: !287)
!286 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/stdint.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "9a48e354f1a21dc4e8a9d7739cbf86a7")
!287 = !DIDerivedType(tag: DW_TAG_typedef, name: "__intmax_t", file: !288, line: 72, baseType: !108)
!288 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "e1865d9fe29fe1b5ced550b7ba458f9e")
!289 = !DIDerivedType(tag: DW_TAG_variable, name: "__da", scope: !280, file: !281, line: 237, baseType: !284, flags: DIFlagStaticMember, extraData: i64 1000000000)
!290 = !DIDerivedType(tag: DW_TAG_variable, name: "__s", scope: !280, file: !281, line: 238, baseType: !284, flags: DIFlagStaticMember, extraData: i64 1)
!291 = !DIDerivedType(tag: DW_TAG_variable, name: "__gcd", scope: !280, file: !281, line: 239, baseType: !284, flags: DIFlagStaticMember, extraData: i64 1)
!292 = !DIDerivedType(tag: DW_TAG_variable, name: "num", scope: !280, file: !281, line: 242, baseType: !284, flags: DIFlagPublic | DIFlagStaticMember, extraData: i64 1)
!293 = !DIDerivedType(tag: DW_TAG_variable, name: "den", scope: !280, file: !281, line: 243, baseType: !284, flags: DIFlagPublic | DIFlagStaticMember, extraData: i64 1000000000)
!294 = !{!295, !296}
!295 = !DITemplateValueParameter(name: "_Num", type: !108, value: i64 1)
!296 = !DITemplateValueParameter(name: "_Den", type: !108, value: i64 1000000000)
!297 = !DISubprogram(name: "time_point", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEC4B9nqe220108Ev", scope: !222, file: !217, line: 49, type: !298, scopeLine: 49, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!298 = !DISubroutineType(types: !299)
!299 = !{null, !300}
!300 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !222, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!301 = !DISubprogram(name: "time_point", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEC4B9nqe220108ERKS6_", scope: !222, file: !217, line: 50, type: !302, scopeLine: 50, flags: DIFlagPublic | DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!302 = !DISubroutineType(types: !303)
!303 = !{null, !300, !304}
!304 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !305, size: 64)
!305 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !225)
!306 = !DISubprogram(name: "time_since_epoch", linkageName: "_ZNKSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEE16time_since_epochB9nqe220108Ev", scope: !222, file: !217, line: 59, type: !307, scopeLine: 59, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!307 = !DISubroutineType(types: !308)
!308 = !{!225, !309}
!309 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !310, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!310 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !222)
!311 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEppB9nqe220108Ev", scope: !222, file: !217, line: 66, type: !312, scopeLine: 66, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!312 = !DISubroutineType(types: !313)
!313 = !{!314, !300}
!314 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !222, size: 64)
!315 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEppB9nqe220108Ei", scope: !222, file: !217, line: 70, type: !316, scopeLine: 70, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!316 = !DISubroutineType(types: !317)
!317 = !{!222, !300, !88}
!318 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEmmB9nqe220108Ev", scope: !222, file: !217, line: 71, type: !312, scopeLine: 71, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!319 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEmmB9nqe220108Ei", scope: !222, file: !217, line: 75, type: !316, scopeLine: 75, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!320 = !DISubprogram(name: "operator+=", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEpLB9nqe220108ERKS6_", scope: !222, file: !217, line: 78, type: !321, scopeLine: 78, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!321 = !DISubroutineType(types: !322)
!322 = !{!314, !300, !304}
!323 = !DISubprogram(name: "operator-=", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEmIB9nqe220108ERKS6_", scope: !222, file: !217, line: 82, type: !321, scopeLine: 82, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!324 = !DISubprogram(name: "min", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEE3minB9nqe220108Ev", scope: !222, file: !217, line: 89, type: !325, scopeLine: 89, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!325 = !DISubroutineType(types: !326)
!326 = !{!222}
!327 = !DISubprogram(name: "max", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEE3maxB9nqe220108Ev", scope: !222, file: !217, line: 92, type: !325, scopeLine: 92, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!328 = !{!329, !339}
!329 = !DITemplateTypeParameter(name: "_Clock", type: !330)
!330 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "steady_clock", scope: !218, file: !331, line: 26, size: 8, flags: DIFlagTypePassByValue, elements: !332, identifier: "_ZTSNSt3__16chrono12steady_clockE")
!331 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__chrono/steady_clock.h", directory: "/home/toolchain/development")
!332 = !{!333, !335}
!333 = !DIDerivedType(tag: DW_TAG_variable, name: "is_steady", scope: !330, file: !331, line: 32, baseType: !334, flags: DIFlagPublic | DIFlagStaticMember, extraData: i1 true)
!334 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !127)
!335 = !DISubprogram(name: "now", linkageName: "_ZNSt3__16chrono12steady_clock3nowEv", scope: !330, file: !331, line: 34, type: !336, scopeLine: 34, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!336 = !DISubroutineType(types: !337)
!337 = !{!338}
!338 = !DIDerivedType(tag: DW_TAG_typedef, name: "time_point", scope: !330, file: !331, line: 31, baseType: !222, flags: DIFlagPublic)
!339 = !DITemplateTypeParameter(name: "_Duration", type: !226)
!340 = !DIGlobalVariableExpression(var: !341, expr: !DIExpression())
!341 = distinct !DIGlobalVariable(name: "real_unlock", linkageName: "_ZN12_GLOBAL__N_111real_unlockE", scope: !2, file: !3, line: 22, type: !342, isLocal: true, isDefinition: true)
!342 = !DIDerivedType(tag: DW_TAG_typedef, name: "UnlockFn", scope: !2, file: !3, line: 21, baseType: !343)
!343 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !344, size: 64)
!344 = !DISubroutineType(types: !345)
!345 = !{!88, !346}
!346 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !80, size: 64)
!347 = !DIGlobalVariableExpression(var: !348, expr: !DIExpression())
!348 = distinct !DIGlobalVariable(scope: null, file: !3, line: 79, type: !349, isLocal: true, isDefinition: true)
!349 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 168, elements: !351)
!350 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !44)
!351 = !{!352}
!352 = !DISubrange(count: 21)
!353 = !DIGlobalVariableExpression(var: !354, expr: !DIExpression())
!354 = distinct !DIGlobalVariable(name: "target_native", linkageName: "_ZN12_GLOBAL__N_113target_nativeE", scope: !2, file: !3, line: 20, type: !346, isLocal: true, isDefinition: true)
!355 = !DIGlobalVariableExpression(var: !356, expr: !DIExpression())
!356 = distinct !DIGlobalVariable(scope: null, file: !3, line: 92, type: !357, isLocal: true, isDefinition: true)
!357 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 152, elements: !358)
!358 = !{!359}
!359 = !DISubrange(count: 19)
!360 = !DIGlobalVariableExpression(var: !361, expr: !DIExpression())
!361 = distinct !DIGlobalVariable(name: "target_mutex", linkageName: "_ZN12_GLOBAL__N_112target_mutexE", scope: !2, file: !3, line: 14, type: !75, isLocal: true, isDefinition: true)
!362 = !DIGlobalVariableExpression(var: !363, expr: !DIExpression())
!363 = distinct !DIGlobalVariable(scope: null, file: !3, line: 99, type: !364, isLocal: true, isDefinition: true)
!364 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 160, elements: !365)
!365 = !{!366}
!366 = !DISubrange(count: 20)
!367 = !DIGlobalVariableExpression(var: !368, expr: !DIExpression())
!368 = distinct !DIGlobalVariable(scope: null, file: !3, line: 105, type: !369, isLocal: true, isDefinition: true)
!369 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 224, elements: !370)
!370 = !{!371}
!371 = !DISubrange(count: 28)
!372 = !DIGlobalVariableExpression(var: !373, expr: !DIExpression())
!373 = distinct !DIGlobalVariable(scope: null, file: !3, line: 109, type: !349, isLocal: true, isDefinition: true)
!374 = !DIGlobalVariableExpression(var: !375, expr: !DIExpression())
!375 = distinct !DIGlobalVariable(scope: null, file: !3, line: 117, type: !376, isLocal: true, isDefinition: true)
!376 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 672, elements: !377)
!377 = !{!378}
!378 = !DISubrange(count: 84)
!379 = !DIGlobalVariableExpression(var: !380, expr: !DIExpression())
!380 = distinct !DIGlobalVariable(scope: null, file: !3, line: 125, type: !381, isLocal: true, isDefinition: true)
!381 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 280, elements: !382)
!382 = !{!383}
!383 = !DISubrange(count: 35)
!384 = !DIGlobalVariableExpression(var: !385, expr: !DIExpression())
!385 = distinct !DIGlobalVariable(scope: null, file: !3, line: 134, type: !386, isLocal: true, isDefinition: true)
!386 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 496, elements: !387)
!387 = !{!388}
!388 = !DISubrange(count: 62)
!389 = !DIGlobalVariableExpression(var: !390, expr: !DIExpression())
!390 = distinct !DIGlobalVariable(scope: null, file: !3, line: 149, type: !391, isLocal: true, isDefinition: true)
!391 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 208, elements: !392)
!392 = !{!393}
!393 = !DISubrange(count: 26)
!394 = !DIGlobalVariableExpression(var: !395, expr: !DIExpression())
!395 = distinct !DIGlobalVariable(name: "track_unlock", linkageName: "_ZN12_GLOBAL__N_112track_unlockE", scope: !2, file: !3, line: 17, type: !396, isLocal: true, isDefinition: true)
!396 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "atomic<bool>", scope: !6, file: !397, line: 242, size: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !398, templateParams: !419, identifier: "_ZTSNSt3__16atomicIbEE")
!397 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/atomic.h", directory: "/home/toolchain/development")
!398 = !{!399, !514, !518, !521, !526, !529, !535}
!399 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !396, baseType: !400, extraData: i32 0)
!400 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__atomic_base<bool, false>", scope: !6, file: !397, line: 42, size: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !401, templateParams: !512, identifier: "_ZTSNSt3__113__atomic_baseIbLb0EEE")
!401 = !{!402, !430, !431, !437, !442, !455, !459, !462, !465, !466, !467, !470, !473, !477, !480, !481, !482, !485, !488, !489, !490, !493, !496, !499, !502, !503, !504, !505, !508}
!402 = !DIDerivedType(tag: DW_TAG_member, name: "__a_", scope: !400, file: !397, line: 44, baseType: !403, size: 8)
!403 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_impl<bool, std::__1::__cxx_atomic_base_impl<bool> >", scope: !6, file: !404, line: 114, size: 8, flags: DIFlagTypePassByValue, elements: !405, templateParams: !428, identifier: "_ZTSNSt3__117__cxx_atomic_implIbNS_22__cxx_atomic_base_implIbEEEE")
!404 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/support.h", directory: "/home/toolchain/development")
!405 = !{!406, !421, !425}
!406 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !403, baseType: !407, extraData: i32 0)
!407 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_base_impl<bool>", scope: !6, file: !408, line: 29, size: 8, flags: DIFlagTypePassByValue, elements: !409, templateParams: !419, identifier: "_ZTSNSt3__122__cxx_atomic_base_implIbEE")
!408 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/support/c11.h", directory: "/home/toolchain/development")
!409 = !{!410, !412, !416}
!410 = !DIDerivedType(tag: DW_TAG_member, name: "__a_value", scope: !407, file: !408, line: 38, baseType: !411, size: 8)
!411 = !DIDerivedType(tag: DW_TAG_atomic_type, baseType: !127)
!412 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIbEC4B9nqe220108Ev", scope: !407, file: !408, line: 32, type: !413, scopeLine: 32, flags: DIFlagPrototyped, spFlags: 0)
!413 = !DISubroutineType(types: !414)
!414 = !{null, !415}
!415 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !407, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!416 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIbEC4Eb", scope: !407, file: !408, line: 37, type: !417, scopeLine: 37, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!417 = !DISubroutineType(types: !418)
!418 = !{null, !415, !127}
!419 = !{!420}
!420 = !DITemplateTypeParameter(name: "_Tp", type: !127)
!421 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIbNS_22__cxx_atomic_base_implIbEEEC4B9nqe220108Ev", scope: !403, file: !404, line: 115, type: !422, scopeLine: 115, flags: DIFlagPrototyped, spFlags: 0)
!422 = !DISubroutineType(types: !423)
!423 = !{null, !424}
!424 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !403, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!425 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIbNS_22__cxx_atomic_base_implIbEEEC4B9nqe220108Eb", scope: !403, file: !404, line: 116, type: !426, scopeLine: 116, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!426 = !DISubroutineType(types: !427)
!427 = !{null, !424, !127}
!428 = !{!420, !429}
!429 = !DITemplateTypeParameter(name: "_Base", type: !407, defaulted: true)
!430 = !DIDerivedType(tag: DW_TAG_variable, name: "is_always_lock_free", scope: !400, file: !397, line: 49, baseType: !334, flags: DIFlagStaticMember)
!431 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EE12is_lock_freeB9nqe220108Ev", scope: !400, file: !397, line: 52, type: !432, scopeLine: 52, flags: DIFlagPrototyped, spFlags: 0)
!432 = !DISubroutineType(types: !433)
!433 = !{!127, !434}
!434 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !435, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!435 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !436)
!436 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !400)
!437 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE12is_lock_freeB9nqe220108Ev", scope: !400, file: !397, line: 55, type: !438, scopeLine: 55, flags: DIFlagPrototyped, spFlags: 0)
!438 = !DISubroutineType(types: !439)
!439 = !{!127, !440}
!440 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !441, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!441 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !400)
!442 = !DISubprogram(name: "store", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE", scope: !400, file: !397, line: 58, type: !443, scopeLine: 58, flags: DIFlagPrototyped, spFlags: 0)
!443 = !DISubroutineType(types: !444)
!444 = !{null, !445, !127, !446}
!445 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !436, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!446 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "memory_order", scope: !6, file: !447, line: 31, baseType: !31, size: 32, flags: DIFlagEnumClass, elements: !448, identifier: "_ZTSNSt3__112memory_orderE")
!447 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/memory_order.h", directory: "/home/toolchain/development")
!448 = !{!449, !450, !451, !452, !453, !454}
!449 = !DIEnumerator(name: "relaxed", value: 0, isUnsigned: true)
!450 = !DIEnumerator(name: "consume", value: 1, isUnsigned: true)
!451 = !DIEnumerator(name: "acquire", value: 2, isUnsigned: true)
!452 = !DIEnumerator(name: "release", value: 3, isUnsigned: true)
!453 = !DIEnumerator(name: "acq_rel", value: 4, isUnsigned: true)
!454 = !DIEnumerator(name: "seq_cst", value: 5, isUnsigned: true)
!455 = !DISubprogram(name: "store", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE", scope: !400, file: !397, line: 62, type: !456, scopeLine: 62, flags: DIFlagPrototyped, spFlags: 0)
!456 = !DISubroutineType(types: !457)
!457 = !{null, !458, !127, !446}
!458 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !400, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!459 = !DISubprogram(name: "load", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EE4loadB9nqe220108ENS_12memory_orderE", scope: !400, file: !397, line: 66, type: !460, scopeLine: 66, flags: DIFlagPrototyped, spFlags: 0)
!460 = !DISubroutineType(types: !461)
!461 = !{!127, !434, !446}
!462 = !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqe220108ENS_12memory_orderE", scope: !400, file: !397, line: 70, type: !463, scopeLine: 70, flags: DIFlagPrototyped, spFlags: 0)
!463 = !DISubroutineType(types: !464)
!464 = !{!127, !440, !446}
!465 = !DISubprogram(name: "operator bool", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EEcvbB9nqe220108Ev", scope: !400, file: !397, line: 74, type: !432, scopeLine: 74, flags: DIFlagPrototyped, spFlags: 0)
!466 = !DISubprogram(name: "operator bool", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EEcvbB9nqe220108Ev", scope: !400, file: !397, line: 75, type: !438, scopeLine: 75, flags: DIFlagPrototyped, spFlags: 0)
!467 = !DISubprogram(name: "exchange", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE8exchangeB9nqe220108EbNS_12memory_orderE", scope: !400, file: !397, line: 76, type: !468, scopeLine: 76, flags: DIFlagPrototyped, spFlags: 0)
!468 = !DISubroutineType(types: !469)
!469 = !{!127, !445, !127, !446}
!470 = !DISubprogram(name: "exchange", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE8exchangeB9nqe220108EbNS_12memory_orderE", scope: !400, file: !397, line: 79, type: !471, scopeLine: 79, flags: DIFlagPrototyped, spFlags: 0)
!471 = !DISubroutineType(types: !472)
!472 = !{!127, !458, !127, !446}
!473 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqe220108ERbbNS_12memory_orderES3_", scope: !400, file: !397, line: 83, type: !474, scopeLine: 83, flags: DIFlagPrototyped, spFlags: 0)
!474 = !DISubroutineType(types: !475)
!475 = !{!127, !445, !476, !127, !446, !446}
!476 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !127, size: 64)
!477 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqe220108ERbbNS_12memory_orderES3_", scope: !400, file: !397, line: 87, type: !478, scopeLine: 87, flags: DIFlagPrototyped, spFlags: 0)
!478 = !DISubroutineType(types: !479)
!479 = !{!127, !458, !476, !127, !446, !446}
!480 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqe220108ERbbNS_12memory_orderES3_", scope: !400, file: !397, line: 92, type: !474, scopeLine: 92, flags: DIFlagPrototyped, spFlags: 0)
!481 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqe220108ERbbNS_12memory_orderES3_", scope: !400, file: !397, line: 96, type: !478, scopeLine: 96, flags: DIFlagPrototyped, spFlags: 0)
!482 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqe220108ERbbNS_12memory_orderE", scope: !400, file: !397, line: 101, type: !483, scopeLine: 101, flags: DIFlagPrototyped, spFlags: 0)
!483 = !DISubroutineType(types: !484)
!484 = !{!127, !445, !476, !127, !446}
!485 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqe220108ERbbNS_12memory_orderE", scope: !400, file: !397, line: 105, type: !486, scopeLine: 105, flags: DIFlagPrototyped, spFlags: 0)
!486 = !DISubroutineType(types: !487)
!487 = !{!127, !458, !476, !127, !446}
!488 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqe220108ERbbNS_12memory_orderE", scope: !400, file: !397, line: 109, type: !483, scopeLine: 109, flags: DIFlagPrototyped, spFlags: 0)
!489 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqe220108ERbbNS_12memory_orderE", scope: !400, file: !397, line: 113, type: !486, scopeLine: 113, flags: DIFlagPrototyped, spFlags: 0)
!490 = !DISubprogram(name: "wait", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EE4waitB9nqe220108EbNS_12memory_orderE", scope: !400, file: !397, line: 118, type: !491, scopeLine: 118, flags: DIFlagPrototyped, spFlags: 0)
!491 = !DISubroutineType(types: !492)
!492 = !{null, !434, !127, !446}
!493 = !DISubprogram(name: "wait", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE4waitB9nqe220108EbNS_12memory_orderE", scope: !400, file: !397, line: 121, type: !494, scopeLine: 121, flags: DIFlagPrototyped, spFlags: 0)
!494 = !DISubroutineType(types: !495)
!495 = !{null, !440, !127, !446}
!496 = !DISubprogram(name: "notify_one", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE10notify_oneB9nqe220108Ev", scope: !400, file: !397, line: 124, type: !497, scopeLine: 124, flags: DIFlagPrototyped, spFlags: 0)
!497 = !DISubroutineType(types: !498)
!498 = !{null, !445}
!499 = !DISubprogram(name: "notify_one", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE10notify_oneB9nqe220108Ev", scope: !400, file: !397, line: 125, type: !500, scopeLine: 125, flags: DIFlagPrototyped, spFlags: 0)
!500 = !DISubroutineType(types: !501)
!501 = !{null, !458}
!502 = !DISubprogram(name: "notify_all", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE10notify_allB9nqe220108Ev", scope: !400, file: !397, line: 126, type: !497, scopeLine: 126, flags: DIFlagPrototyped, spFlags: 0)
!503 = !DISubprogram(name: "notify_all", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE10notify_allB9nqe220108Ev", scope: !400, file: !397, line: 127, type: !500, scopeLine: 127, flags: DIFlagPrototyped, spFlags: 0)
!504 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIbLb0EEC4B9nqe220108Ev", scope: !400, file: !397, line: 131, type: !500, scopeLine: 131, flags: DIFlagPrototyped, spFlags: 0)
!505 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIbLb0EEC4B9nqe220108Eb", scope: !400, file: !397, line: 136, type: !506, scopeLine: 136, flags: DIFlagPrototyped, spFlags: 0)
!506 = !DISubroutineType(types: !507)
!507 = !{null, !458, !127}
!508 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIbLb0EEC4ERKS1_", scope: !400, file: !397, line: 138, type: !509, scopeLine: 138, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!509 = !DISubroutineType(types: !510)
!510 = !{null, !458, !511}
!511 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !441, size: 64)
!512 = !{!420, !513}
!513 = !DITemplateValueParameter(type: !127, defaulted: true, value: i1 false)
!514 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIbEC4B9nqe220108Ev", scope: !396, file: !397, line: 246, type: !515, scopeLine: 246, flags: DIFlagPrototyped, spFlags: 0)
!515 = !DISubroutineType(types: !516)
!516 = !{null, !517}
!517 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !396, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!518 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIbEC4B9nqe220108Eb", scope: !396, file: !397, line: 251, type: !519, scopeLine: 251, flags: DIFlagPrototyped, spFlags: 0)
!519 = !DISubroutineType(types: !520)
!520 = !{null, !517, !127}
!521 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIbEaSB9nqe220108Eb", scope: !396, file: !397, line: 253, type: !522, scopeLine: 253, flags: DIFlagPrototyped, spFlags: 0)
!522 = !DISubroutineType(types: !523)
!523 = !{!127, !524, !127}
!524 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !525, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!525 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !396)
!526 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIbEaSB9nqe220108Eb", scope: !396, file: !397, line: 257, type: !527, scopeLine: 257, flags: DIFlagPrototyped, spFlags: 0)
!527 = !DISubroutineType(types: !528)
!528 = !{!127, !517, !127}
!529 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIbEaSERKS1_", scope: !396, file: !397, line: 262, type: !530, scopeLine: 262, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!530 = !DISubroutineType(types: !531)
!531 = !{!532, !517, !533}
!532 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !396, size: 64)
!533 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !534, size: 64)
!534 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !396)
!535 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIbEaSERKS1_", scope: !396, file: !397, line: 263, type: !536, scopeLine: 263, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!536 = !DISubroutineType(types: !537)
!537 = !{!532, !524, !533}
!538 = !DIGlobalVariableExpression(var: !539, expr: !DIExpression())
!539 = distinct !DIGlobalVariable(name: "target_unlock_count", linkageName: "_ZN12_GLOBAL__N_119target_unlock_countE", scope: !2, file: !3, line: 18, type: !540, isLocal: true, isDefinition: true)
!540 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "atomic<int>", scope: !6, file: !397, line: 242, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !541, templateParams: !563, identifier: "_ZTSNSt3__16atomicIiEE")
!541 = !{!542, !704, !708, !711, !716, !719, !725}
!542 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !540, baseType: !543, extraData: i32 0)
!543 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__atomic_base<int, true>", scope: !6, file: !397, line: 144, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !544, templateParams: !702, identifier: "_ZTSNSt3__113__atomic_baseIiLb1EEE")
!544 = !{!545, !653, !657, !660, !665, !668, !669, !670, !671, !672, !673, !674, !675, !676, !679, !682, !683, !684, !687, !690, !691, !692, !693, !694, !695, !696, !697, !698, !699, !700, !701}
!545 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !543, baseType: !546, extraData: i32 0)
!546 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__atomic_base<int, false>", scope: !6, file: !397, line: 42, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !547, templateParams: !651, identifier: "_ZTSNSt3__113__atomic_baseIiLb0EEE")
!547 = !{!548, !574, !575, !581, !586, !590, !594, !597, !600, !603, !606, !609, !612, !616, !619, !620, !621, !624, !627, !628, !629, !632, !635, !638, !641, !642, !643, !644, !647}
!548 = !DIDerivedType(tag: DW_TAG_member, name: "__a_", scope: !546, file: !397, line: 44, baseType: !549, size: 32)
!549 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_impl<int, std::__1::__cxx_atomic_base_impl<int> >", scope: !6, file: !404, line: 114, size: 32, flags: DIFlagTypePassByValue, elements: !550, templateParams: !572, identifier: "_ZTSNSt3__117__cxx_atomic_implIiNS_22__cxx_atomic_base_implIiEEEE")
!550 = !{!551, !565, !569}
!551 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !549, baseType: !552, extraData: i32 0)
!552 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_base_impl<int>", scope: !6, file: !408, line: 29, size: 32, flags: DIFlagTypePassByValue, elements: !553, templateParams: !563, identifier: "_ZTSNSt3__122__cxx_atomic_base_implIiEE")
!553 = !{!554, !556, !560}
!554 = !DIDerivedType(tag: DW_TAG_member, name: "__a_value", scope: !552, file: !408, line: 38, baseType: !555, size: 32)
!555 = !DIDerivedType(tag: DW_TAG_atomic_type, baseType: !88)
!556 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIiEC4B9nqe220108Ev", scope: !552, file: !408, line: 32, type: !557, scopeLine: 32, flags: DIFlagPrototyped, spFlags: 0)
!557 = !DISubroutineType(types: !558)
!558 = !{null, !559}
!559 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !552, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!560 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIiEC4Ei", scope: !552, file: !408, line: 37, type: !561, scopeLine: 37, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!561 = !DISubroutineType(types: !562)
!562 = !{null, !559, !88}
!563 = !{!564}
!564 = !DITemplateTypeParameter(name: "_Tp", type: !88)
!565 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIiNS_22__cxx_atomic_base_implIiEEEC4B9nqe220108Ev", scope: !549, file: !404, line: 115, type: !566, scopeLine: 115, flags: DIFlagPrototyped, spFlags: 0)
!566 = !DISubroutineType(types: !567)
!567 = !{null, !568}
!568 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !549, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!569 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIiNS_22__cxx_atomic_base_implIiEEEC4B9nqe220108Ei", scope: !549, file: !404, line: 116, type: !570, scopeLine: 116, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!570 = !DISubroutineType(types: !571)
!571 = !{null, !568, !88}
!572 = !{!564, !573}
!573 = !DITemplateTypeParameter(name: "_Base", type: !552, defaulted: true)
!574 = !DIDerivedType(tag: DW_TAG_variable, name: "is_always_lock_free", scope: !546, file: !397, line: 49, baseType: !334, flags: DIFlagStaticMember)
!575 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EE12is_lock_freeB9nqe220108Ev", scope: !546, file: !397, line: 52, type: !576, scopeLine: 52, flags: DIFlagPrototyped, spFlags: 0)
!576 = !DISubroutineType(types: !577)
!577 = !{!127, !578}
!578 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !579, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!579 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !580)
!580 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !546)
!581 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE12is_lock_freeB9nqe220108Ev", scope: !546, file: !397, line: 55, type: !582, scopeLine: 55, flags: DIFlagPrototyped, spFlags: 0)
!582 = !DISubroutineType(types: !583)
!583 = !{!127, !584}
!584 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !585, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!585 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !546)
!586 = !DISubprogram(name: "store", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE5storeB9nqe220108EiNS_12memory_orderE", scope: !546, file: !397, line: 58, type: !587, scopeLine: 58, flags: DIFlagPrototyped, spFlags: 0)
!587 = !DISubroutineType(types: !588)
!588 = !{null, !589, !88, !446}
!589 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !580, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!590 = !DISubprogram(name: "store", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE5storeB9nqe220108EiNS_12memory_orderE", scope: !546, file: !397, line: 62, type: !591, scopeLine: 62, flags: DIFlagPrototyped, spFlags: 0)
!591 = !DISubroutineType(types: !592)
!592 = !{null, !593, !88, !446}
!593 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !546, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!594 = !DISubprogram(name: "load", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE", scope: !546, file: !397, line: 66, type: !595, scopeLine: 66, flags: DIFlagPrototyped, spFlags: 0)
!595 = !DISubroutineType(types: !596)
!596 = !{!88, !578, !446}
!597 = !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE", scope: !546, file: !397, line: 70, type: !598, scopeLine: 70, flags: DIFlagPrototyped, spFlags: 0)
!598 = !DISubroutineType(types: !599)
!599 = !{!88, !584, !446}
!600 = !DISubprogram(name: "operator int", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EEcviB9nqe220108Ev", scope: !546, file: !397, line: 74, type: !601, scopeLine: 74, flags: DIFlagPrototyped, spFlags: 0)
!601 = !DISubroutineType(types: !602)
!602 = !{!88, !578}
!603 = !DISubprogram(name: "operator int", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EEcviB9nqe220108Ev", scope: !546, file: !397, line: 75, type: !604, scopeLine: 75, flags: DIFlagPrototyped, spFlags: 0)
!604 = !DISubroutineType(types: !605)
!605 = !{!88, !584}
!606 = !DISubprogram(name: "exchange", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE8exchangeB9nqe220108EiNS_12memory_orderE", scope: !546, file: !397, line: 76, type: !607, scopeLine: 76, flags: DIFlagPrototyped, spFlags: 0)
!607 = !DISubroutineType(types: !608)
!608 = !{!88, !589, !88, !446}
!609 = !DISubprogram(name: "exchange", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE8exchangeB9nqe220108EiNS_12memory_orderE", scope: !546, file: !397, line: 79, type: !610, scopeLine: 79, flags: DIFlagPrototyped, spFlags: 0)
!610 = !DISubroutineType(types: !611)
!611 = !{!88, !593, !88, !446}
!612 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqe220108ERiiNS_12memory_orderES3_", scope: !546, file: !397, line: 83, type: !613, scopeLine: 83, flags: DIFlagPrototyped, spFlags: 0)
!613 = !DISubroutineType(types: !614)
!614 = !{!127, !589, !615, !88, !446, !446}
!615 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !88, size: 64)
!616 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqe220108ERiiNS_12memory_orderES3_", scope: !546, file: !397, line: 87, type: !617, scopeLine: 87, flags: DIFlagPrototyped, spFlags: 0)
!617 = !DISubroutineType(types: !618)
!618 = !{!127, !593, !615, !88, !446, !446}
!619 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqe220108ERiiNS_12memory_orderES3_", scope: !546, file: !397, line: 92, type: !613, scopeLine: 92, flags: DIFlagPrototyped, spFlags: 0)
!620 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqe220108ERiiNS_12memory_orderES3_", scope: !546, file: !397, line: 96, type: !617, scopeLine: 96, flags: DIFlagPrototyped, spFlags: 0)
!621 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqe220108ERiiNS_12memory_orderE", scope: !546, file: !397, line: 101, type: !622, scopeLine: 101, flags: DIFlagPrototyped, spFlags: 0)
!622 = !DISubroutineType(types: !623)
!623 = !{!127, !589, !615, !88, !446}
!624 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqe220108ERiiNS_12memory_orderE", scope: !546, file: !397, line: 105, type: !625, scopeLine: 105, flags: DIFlagPrototyped, spFlags: 0)
!625 = !DISubroutineType(types: !626)
!626 = !{!127, !593, !615, !88, !446}
!627 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqe220108ERiiNS_12memory_orderE", scope: !546, file: !397, line: 109, type: !622, scopeLine: 109, flags: DIFlagPrototyped, spFlags: 0)
!628 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqe220108ERiiNS_12memory_orderE", scope: !546, file: !397, line: 113, type: !625, scopeLine: 113, flags: DIFlagPrototyped, spFlags: 0)
!629 = !DISubprogram(name: "wait", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EE4waitB9nqe220108EiNS_12memory_orderE", scope: !546, file: !397, line: 118, type: !630, scopeLine: 118, flags: DIFlagPrototyped, spFlags: 0)
!630 = !DISubroutineType(types: !631)
!631 = !{null, !578, !88, !446}
!632 = !DISubprogram(name: "wait", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE4waitB9nqe220108EiNS_12memory_orderE", scope: !546, file: !397, line: 121, type: !633, scopeLine: 121, flags: DIFlagPrototyped, spFlags: 0)
!633 = !DISubroutineType(types: !634)
!634 = !{null, !584, !88, !446}
!635 = !DISubprogram(name: "notify_one", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE10notify_oneB9nqe220108Ev", scope: !546, file: !397, line: 124, type: !636, scopeLine: 124, flags: DIFlagPrototyped, spFlags: 0)
!636 = !DISubroutineType(types: !637)
!637 = !{null, !589}
!638 = !DISubprogram(name: "notify_one", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE10notify_oneB9nqe220108Ev", scope: !546, file: !397, line: 125, type: !639, scopeLine: 125, flags: DIFlagPrototyped, spFlags: 0)
!639 = !DISubroutineType(types: !640)
!640 = !{null, !593}
!641 = !DISubprogram(name: "notify_all", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE10notify_allB9nqe220108Ev", scope: !546, file: !397, line: 126, type: !636, scopeLine: 126, flags: DIFlagPrototyped, spFlags: 0)
!642 = !DISubprogram(name: "notify_all", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE10notify_allB9nqe220108Ev", scope: !546, file: !397, line: 127, type: !639, scopeLine: 127, flags: DIFlagPrototyped, spFlags: 0)
!643 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb0EEC4B9nqe220108Ev", scope: !546, file: !397, line: 131, type: !639, scopeLine: 131, flags: DIFlagPrototyped, spFlags: 0)
!644 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb0EEC4B9nqe220108Ei", scope: !546, file: !397, line: 136, type: !645, scopeLine: 136, flags: DIFlagPrototyped, spFlags: 0)
!645 = !DISubroutineType(types: !646)
!646 = !{null, !593, !88}
!647 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb0EEC4ERKS1_", scope: !546, file: !397, line: 138, type: !648, scopeLine: 138, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!648 = !DISubroutineType(types: !649)
!649 = !{null, !593, !650}
!650 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !585, size: 64)
!651 = !{!564, !652}
!652 = !DITemplateValueParameter(type: !127, value: i1 false)
!653 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEC4B9nqe220108Ev", scope: !543, file: !397, line: 149, type: !654, scopeLine: 149, flags: DIFlagPrototyped, spFlags: 0)
!654 = !DISubroutineType(types: !655)
!655 = !{null, !656}
!656 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !543, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!657 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEC4B9nqe220108Ei", scope: !543, file: !397, line: 151, type: !658, scopeLine: 151, flags: DIFlagPrototyped, spFlags: 0)
!658 = !DISubroutineType(types: !659)
!659 = !{null, !656, !88}
!660 = !DISubprogram(name: "fetch_add", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_addB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 153, type: !661, scopeLine: 153, flags: DIFlagPrototyped, spFlags: 0)
!661 = !DISubroutineType(types: !662)
!662 = !{!88, !663, !88, !446}
!663 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !664, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!664 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !543)
!665 = !DISubprogram(name: "fetch_add", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 156, type: !666, scopeLine: 156, flags: DIFlagPrototyped, spFlags: 0)
!666 = !DISubroutineType(types: !667)
!667 = !{!88, !656, !88, !446}
!668 = !DISubprogram(name: "fetch_sub", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_subB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 159, type: !661, scopeLine: 159, flags: DIFlagPrototyped, spFlags: 0)
!669 = !DISubprogram(name: "fetch_sub", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_subB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 162, type: !666, scopeLine: 162, flags: DIFlagPrototyped, spFlags: 0)
!670 = !DISubprogram(name: "fetch_and", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_andB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 165, type: !661, scopeLine: 165, flags: DIFlagPrototyped, spFlags: 0)
!671 = !DISubprogram(name: "fetch_and", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_andB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 168, type: !666, scopeLine: 168, flags: DIFlagPrototyped, spFlags: 0)
!672 = !DISubprogram(name: "fetch_or", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE8fetch_orB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 171, type: !661, scopeLine: 171, flags: DIFlagPrototyped, spFlags: 0)
!673 = !DISubprogram(name: "fetch_or", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE8fetch_orB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 174, type: !666, scopeLine: 174, flags: DIFlagPrototyped, spFlags: 0)
!674 = !DISubprogram(name: "fetch_xor", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_xorB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 177, type: !661, scopeLine: 177, flags: DIFlagPrototyped, spFlags: 0)
!675 = !DISubprogram(name: "fetch_xor", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_xorB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 180, type: !666, scopeLine: 180, flags: DIFlagPrototyped, spFlags: 0)
!676 = !DISubprogram(name: "operator++", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEppB9nqe220108Ei", scope: !543, file: !397, line: 184, type: !677, scopeLine: 184, flags: DIFlagPrototyped, spFlags: 0)
!677 = !DISubroutineType(types: !678)
!678 = !{!88, !663, !88}
!679 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEppB9nqe220108Ei", scope: !543, file: !397, line: 185, type: !680, scopeLine: 185, flags: DIFlagPrototyped, spFlags: 0)
!680 = !DISubroutineType(types: !681)
!681 = !{!88, !656, !88}
!682 = !DISubprogram(name: "operator--", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEmmB9nqe220108Ei", scope: !543, file: !397, line: 186, type: !677, scopeLine: 186, flags: DIFlagPrototyped, spFlags: 0)
!683 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEmmB9nqe220108Ei", scope: !543, file: !397, line: 187, type: !680, scopeLine: 187, flags: DIFlagPrototyped, spFlags: 0)
!684 = !DISubprogram(name: "operator++", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEppB9nqe220108Ev", scope: !543, file: !397, line: 188, type: !685, scopeLine: 188, flags: DIFlagPrototyped, spFlags: 0)
!685 = !DISubroutineType(types: !686)
!686 = !{!88, !663}
!687 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEppB9nqe220108Ev", scope: !543, file: !397, line: 189, type: !688, scopeLine: 189, flags: DIFlagPrototyped, spFlags: 0)
!688 = !DISubroutineType(types: !689)
!689 = !{!88, !656}
!690 = !DISubprogram(name: "operator--", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEmmB9nqe220108Ev", scope: !543, file: !397, line: 190, type: !685, scopeLine: 190, flags: DIFlagPrototyped, spFlags: 0)
!691 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEmmB9nqe220108Ev", scope: !543, file: !397, line: 191, type: !688, scopeLine: 191, flags: DIFlagPrototyped, spFlags: 0)
!692 = !DISubprogram(name: "operator+=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEpLB9nqe220108Ei", scope: !543, file: !397, line: 192, type: !677, scopeLine: 192, flags: DIFlagPrototyped, spFlags: 0)
!693 = !DISubprogram(name: "operator+=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEpLB9nqe220108Ei", scope: !543, file: !397, line: 193, type: !680, scopeLine: 193, flags: DIFlagPrototyped, spFlags: 0)
!694 = !DISubprogram(name: "operator-=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEmIB9nqe220108Ei", scope: !543, file: !397, line: 194, type: !677, scopeLine: 194, flags: DIFlagPrototyped, spFlags: 0)
!695 = !DISubprogram(name: "operator-=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEmIB9nqe220108Ei", scope: !543, file: !397, line: 195, type: !680, scopeLine: 195, flags: DIFlagPrototyped, spFlags: 0)
!696 = !DISubprogram(name: "operator&=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEaNB9nqe220108Ei", scope: !543, file: !397, line: 196, type: !677, scopeLine: 196, flags: DIFlagPrototyped, spFlags: 0)
!697 = !DISubprogram(name: "operator&=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEaNB9nqe220108Ei", scope: !543, file: !397, line: 197, type: !680, scopeLine: 197, flags: DIFlagPrototyped, spFlags: 0)
!698 = !DISubprogram(name: "operator|=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEoRB9nqe220108Ei", scope: !543, file: !397, line: 198, type: !677, scopeLine: 198, flags: DIFlagPrototyped, spFlags: 0)
!699 = !DISubprogram(name: "operator|=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEoRB9nqe220108Ei", scope: !543, file: !397, line: 199, type: !680, scopeLine: 199, flags: DIFlagPrototyped, spFlags: 0)
!700 = !DISubprogram(name: "operator^=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEeOB9nqe220108Ei", scope: !543, file: !397, line: 200, type: !677, scopeLine: 200, flags: DIFlagPrototyped, spFlags: 0)
!701 = !DISubprogram(name: "operator^=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEeOB9nqe220108Ei", scope: !543, file: !397, line: 201, type: !680, scopeLine: 201, flags: DIFlagPrototyped, spFlags: 0)
!702 = !{!564, !703}
!703 = !DITemplateValueParameter(type: !127, defaulted: true, value: i1 true)
!704 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIiEC4B9nqe220108Ev", scope: !540, file: !397, line: 246, type: !705, scopeLine: 246, flags: DIFlagPrototyped, spFlags: 0)
!705 = !DISubroutineType(types: !706)
!706 = !{null, !707}
!707 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !540, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!708 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIiEC4B9nqe220108Ei", scope: !540, file: !397, line: 251, type: !709, scopeLine: 251, flags: DIFlagPrototyped, spFlags: 0)
!709 = !DISubroutineType(types: !710)
!710 = !{null, !707, !88}
!711 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIiEaSB9nqe220108Ei", scope: !540, file: !397, line: 253, type: !712, scopeLine: 253, flags: DIFlagPrototyped, spFlags: 0)
!712 = !DISubroutineType(types: !713)
!713 = !{!88, !714, !88}
!714 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !715, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!715 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !540)
!716 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIiEaSB9nqe220108Ei", scope: !540, file: !397, line: 257, type: !717, scopeLine: 257, flags: DIFlagPrototyped, spFlags: 0)
!717 = !DISubroutineType(types: !718)
!718 = !{!88, !707, !88}
!719 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIiEaSERKS1_", scope: !540, file: !397, line: 262, type: !720, scopeLine: 262, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!720 = !DISubroutineType(types: !721)
!721 = !{!722, !707, !723}
!722 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !540, size: 64)
!723 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !724, size: 64)
!724 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !540)
!725 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIiEaSERKS1_", scope: !540, file: !397, line: 263, type: !726, scopeLine: 263, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!726 = !DISubroutineType(types: !727)
!727 = !{!722, !714, !723}
!728 = !DIGlobalVariableExpression(var: !729, expr: !DIExpression())
!729 = distinct !DIGlobalVariable(scope: null, file: !3, line: 36, type: !730, isLocal: true, isDefinition: true)
!730 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 408, elements: !731)
!731 = !{!732}
!732 = !DISubrange(count: 51)
!733 = !DIGlobalVariableExpression(var: !734, expr: !DIExpression())
!734 = distinct !DIGlobalVariable(name: "ready", linkageName: "_ZN12_GLOBAL__N_15readyE", scope: !2, file: !3, line: 16, type: !396, isLocal: true, isDefinition: true)
!735 = !DIGlobalVariableExpression(var: !736, expr: !DIExpression())
!736 = distinct !DIGlobalVariable(name: "frame_cleanup_count", linkageName: "_ZN12_GLOBAL__N_119frame_cleanup_countE", scope: !2, file: !3, line: 19, type: !540, isLocal: true, isDefinition: true)
!737 = !DIGlobalVariableExpression(var: !738, expr: !DIExpression())
!738 = distinct !DIGlobalVariable(scope: null, file: !3, line: 69, type: !739, isLocal: true, isDefinition: true)
!739 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 432, elements: !740)
!740 = !{!741}
!741 = !DISubrange(count: 54)
!742 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "clang version 22.1.8", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !743, retainedTypes: !753, globals: !773, imports: !781, splitDebugInlining: false, nameTableKind: None)
!743 = !{!446, !744, !749}
!744 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !745, line: 168, baseType: !31, size: 32, elements: !746)
!745 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/pthread.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "2b60be7fa73249bcf83f3192783962e1")
!746 = !{!747, !748}
!747 = !DIEnumerator(name: "PTHREAD_CANCEL_ENABLE", value: 0, isUnsigned: true)
!748 = !DIEnumerator(name: "PTHREAD_CANCEL_DISABLE", value: 1, isUnsigned: true)
!749 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !745, line: 175, baseType: !31, size: 32, elements: !750)
!750 = !{!751, !752}
!751 = !DIEnumerator(name: "PTHREAD_CANCEL_DEFERRED", value: 0, isUnsigned: true)
!752 = !DIEnumerator(name: "PTHREAD_CANCEL_ASYNCHRONOUS", value: 1, isUnsigned: true)
!753 = !{!342, !754, !88, !755, !756, !31, !759, !766, !4, !226, !222, !407, !403, !400, !396, !552, !549, !546, !543, !540, !75, !69, !146, !156, !165}
!754 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!755 = !DIDerivedType(tag: DW_TAG_typedef, name: "intptr_t", file: !286, line: 76, baseType: !108)
!756 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !757, line: 18, baseType: !758)
!757 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/lib64/clang/22/include/__stddef_size_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "2c44e821a2b1951cde2eb0fb2e656867")
!758 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!759 = !DIDerivedType(tag: DW_TAG_typedef, name: "__ptr_type", scope: !760, file: !408, line: 80, baseType: !765)
!760 = distinct !DISubprogram(name: "__cxx_atomic_load<int>", linkageName: "_ZNSt3__117__cxx_atomic_loadB9nqe220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE", scope: !6, file: !408, line: 79, type: !761, scopeLine: 79, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, templateParams: !563, retainedNodes: !246)
!761 = !DISubroutineType(types: !762)
!762 = !{!88, !763, !446}
!763 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !764, size: 64)
!764 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !552)
!765 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !555, size: 64)
!766 = !DIDerivedType(tag: DW_TAG_typedef, name: "__ptr_type", scope: !767, file: !408, line: 80, baseType: !772)
!767 = distinct !DISubprogram(name: "__cxx_atomic_load<bool>", linkageName: "_ZNSt3__117__cxx_atomic_loadB9nqe220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE", scope: !6, file: !408, line: 79, type: !768, scopeLine: 79, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, templateParams: !419, retainedNodes: !246)
!768 = !DISubroutineType(types: !769)
!769 = !{!127, !770, !446}
!770 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !771, size: 64)
!771 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !407)
!772 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !411, size: 64)
!773 = !{!0, !347, !774, !355, !362, !777, !367, !372, !779, !374, !379, !384, !389, !340, !394, !353, !538, !728, !360, !733, !735, !737}
!774 = !DIGlobalVariableExpression(var: !775, expr: !DIExpression(DW_OP_constu, 0, DW_OP_stack_value))
!775 = distinct !DIGlobalVariable(name: "memory_order_relaxed", scope: !6, file: !447, line: 43, type: !776, isLocal: true, isDefinition: true)
!776 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !446)
!777 = !DIGlobalVariableExpression(var: !778, expr: !DIExpression(DW_OP_constu, 2, DW_OP_stack_value))
!778 = distinct !DIGlobalVariable(name: "memory_order_acquire", scope: !6, file: !447, line: 45, type: !776, isLocal: true, isDefinition: true)
!779 = !DIGlobalVariableExpression(var: !780, expr: !DIExpression(DW_OP_constu, 3, DW_OP_stack_value))
!780 = distinct !DIGlobalVariable(name: "memory_order_release", scope: !6, file: !447, line: 46, type: !776, isLocal: true, isDefinition: true)
!781 = !{!782, !788, !791, !794, !797, !802, !806, !809, !812, !816, !819, !822, !825, !828, !831, !834, !837, !839, !841, !843, !845, !847, !849, !851, !853, !854, !856, !857, !860, !870, !874, !882, !886, !888, !890, !894, !898, !902, !904, !908, !912, !916, !920, !924, !926, !928, !930, !932, !936, !940, !944, !947, !952, !956, !959, !966, !971, !976, !981, !986, !992, !998, !1002, !1004, !1009, !1014, !1022, !1024, !1026, !1028, !1034, !1036, !1037, !1038, !1039, !1043, !1045, !1050, !1052, !1054, !1058, !1060, !1062, !1064, !1066, !1068, !1070, !1072, !1077, !1081, !1083, !1085, !1086, !1091, !1093, !1095, !1097, !1099, !1101, !1103, !1105, !1107, !1109, !1111, !1113, !1115, !1117, !1119, !1121, !1123, !1127, !1129, !1131, !1133, !1137, !1139, !1143, !1145, !1147, !1149, !1151, !1155, !1157, !1161, !1165, !1167, !1169, !1173, !1175, !1179, !1181, !1183, !1187, !1189, !1191, !1193, !1197, !1199, !1201, !1205, !1207, !1209, !1211, !1213, !1215, !1217, !1219, !1223, !1227, !1229, !1231, !1233, !1235, !1237, !1239, !1241, !1243, !1245, !1247, !1249, !1251, !1253, !1255, !1257, !1259, !1261, !1263, !1265, !1269, !1271, !1273, !1275, !1279, !1281, !1285, !1287, !1289, !1291, !1293, !1297, !1299, !1303, !1305, !1307, !1309, !1311, !1315, !1317, !1319, !1323, !1325, !1327, !1329, !1337, !1342, !1348, !1354, !1356, !1360, !1364, !1368, !1374, !1378, !1382, !1386, !1390, !1394, !1398, !1402, !1406, !1410, !1414, !1418, !1422, !1426, !1431, !1435, !1437, !1441, !1443, !1451, !1455, !1459, !1463, !1467, !1472, !1476, !1478, !1482, !1489, !1493, !1497, !1504, !1506, !1508, !1510, !1527, !1531, !1533, !1535, !1537, !1539, !1541, !1543, !1545, !1547, !1549, !1551, !1553, !1555, !1557, !1561, !1566, !1569, !1573, !1575, !1577, !1579, !1581, !1583, !1585, !1587, !1589, !1591, !1593, !1595, !1599, !1603, !1607, !1609, !1613, !1617, !1619, !1620, !1621, !1626, !1634, !1636, !1640, !1646, !1650, !1654, !1656, !1660, !1664, !1668, !1672, !1676, !1680, !1682, !1684, !1688, !1694, !1698, !1702, !1706, !1710, !1714, !1718, !1722, !1726, !1728, !1730, !1734, !1736, !1740, !1744, !1748, !1752, !1754, !1756, !1760, !1764, !1768, !1770, !1774, !1776, !1778, !1782, !1786, !1790, !1794, !1796, !1802, !1808, !1812, !1816, !1822, !1828, !1832, !1836, !1840, !1844, !1846, !1848, !1850, !1856, !1861, !1863, !1868, !1872, !1876, !1878, !1882, !1886, !1890, !1894, !1896, !1900, !1904, !1908, !1910, !1914, !1918, !1922, !1924, !1926, !1928, !1932, !1936, !1942, !1946, !1952, !1956, !1960, !1962, !1964, !1966, !1970, !1974, !1978, !1980, !1982, !1986, !1990, !1992, !1996, !2000, !2002, !2004, !2006}
!782 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !783, file: !787, line: 158)
!783 = !DIDerivedType(tag: DW_TAG_typedef, name: "int8_t", file: !784, line: 24, baseType: !785)
!784 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/stdint-intn.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "649b383a60bfa3eb90e85840b2b0be20")
!785 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int8_t", file: !288, line: 37, baseType: !786)
!786 = !DIBasicType(name: "signed char", size: 8, encoding: DW_ATE_signed_char)
!787 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstdint", directory: "/home/toolchain/development")
!788 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !789, file: !787, line: 159)
!789 = !DIDerivedType(tag: DW_TAG_typedef, name: "int16_t", file: !784, line: 25, baseType: !790)
!790 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int16_t", file: !288, line: 39, baseType: !94)
!791 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !792, file: !787, line: 160)
!792 = !DIDerivedType(tag: DW_TAG_typedef, name: "int32_t", file: !784, line: 26, baseType: !793)
!793 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int32_t", file: !288, line: 41, baseType: !88)
!794 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !795, file: !787, line: 161)
!795 = !DIDerivedType(tag: DW_TAG_typedef, name: "int64_t", file: !784, line: 27, baseType: !796)
!796 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int64_t", file: !288, line: 44, baseType: !108)
!797 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !798, file: !787, line: 163)
!798 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !799, line: 24, baseType: !800)
!799 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/stdint-uintn.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "256fcabbefa27ca8cf5e6d37525e6e16")
!800 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint8_t", file: !288, line: 38, baseType: !801)
!801 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!802 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !803, file: !787, line: 164)
!803 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint16_t", file: !799, line: 25, baseType: !804)
!804 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint16_t", file: !288, line: 40, baseType: !805)
!805 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!806 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !807, file: !787, line: 165)
!807 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint32_t", file: !799, line: 26, baseType: !808)
!808 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint32_t", file: !288, line: 42, baseType: !31)
!809 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !810, file: !787, line: 166)
!810 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint64_t", file: !799, line: 27, baseType: !811)
!811 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint64_t", file: !288, line: 45, baseType: !758)
!812 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !813, file: !787, line: 168)
!813 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least8_t", file: !814, line: 25, baseType: !815)
!814 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/stdint-least.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "a866be81c480920b0293bd5f6336a0a3")
!815 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least8_t", file: !288, line: 52, baseType: !785)
!816 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !817, file: !787, line: 169)
!817 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least16_t", file: !814, line: 26, baseType: !818)
!818 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least16_t", file: !288, line: 54, baseType: !790)
!819 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !820, file: !787, line: 170)
!820 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least32_t", file: !814, line: 27, baseType: !821)
!821 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least32_t", file: !288, line: 56, baseType: !793)
!822 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !823, file: !787, line: 171)
!823 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least64_t", file: !814, line: 28, baseType: !824)
!824 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least64_t", file: !288, line: 58, baseType: !796)
!825 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !826, file: !787, line: 173)
!826 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least8_t", file: !814, line: 31, baseType: !827)
!827 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least8_t", file: !288, line: 53, baseType: !800)
!828 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !829, file: !787, line: 174)
!829 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least16_t", file: !814, line: 32, baseType: !830)
!830 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least16_t", file: !288, line: 55, baseType: !804)
!831 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !832, file: !787, line: 175)
!832 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least32_t", file: !814, line: 33, baseType: !833)
!833 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least32_t", file: !288, line: 57, baseType: !808)
!834 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !835, file: !787, line: 176)
!835 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least64_t", file: !814, line: 34, baseType: !836)
!836 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least64_t", file: !288, line: 59, baseType: !811)
!837 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !838, file: !787, line: 178)
!838 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast8_t", file: !286, line: 47, baseType: !786)
!839 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !840, file: !787, line: 179)
!840 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast16_t", file: !286, line: 49, baseType: !108)
!841 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !842, file: !787, line: 180)
!842 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast32_t", file: !286, line: 50, baseType: !108)
!843 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !844, file: !787, line: 181)
!844 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast64_t", file: !286, line: 51, baseType: !108)
!845 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !846, file: !787, line: 183)
!846 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast8_t", file: !286, line: 60, baseType: !801)
!847 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !848, file: !787, line: 184)
!848 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast16_t", file: !286, line: 62, baseType: !758)
!849 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !850, file: !787, line: 185)
!850 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast32_t", file: !286, line: 63, baseType: !758)
!851 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !852, file: !787, line: 186)
!852 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast64_t", file: !286, line: 64, baseType: !758)
!853 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !755, file: !787, line: 188)
!854 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !855, file: !787, line: 189)
!855 = !DIDerivedType(tag: DW_TAG_typedef, name: "uintptr_t", file: !286, line: 79, baseType: !758)
!856 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !285, file: !787, line: 191)
!857 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !858, file: !787, line: 192)
!858 = !DIDerivedType(tag: DW_TAG_typedef, name: "uintmax_t", file: !286, line: 91, baseType: !859)
!859 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uintmax_t", file: !288, line: 73, baseType: !758)
!860 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !861, file: !869, line: 82)
!861 = !DISubprogram(name: "memcpy", scope: !862, file: !862, line: 43, type: !863, flags: DIFlagPrototyped, spFlags: 0)
!862 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/string.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "b395f57719fc1ba9602e6cb95fab8598")
!863 = !DISubroutineType(types: !864)
!864 = !{!754, !865, !866, !756}
!865 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !754)
!866 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !867)
!867 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !868, size: 64)
!868 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!869 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstring", directory: "/home/toolchain/development")
!870 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !871, file: !869, line: 83)
!871 = !DISubprogram(name: "memmove", scope: !862, file: !862, line: 47, type: !872, flags: DIFlagPrototyped, spFlags: 0)
!872 = !DISubroutineType(types: !873)
!873 = !{!754, !754, !867, !756}
!874 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !875, file: !869, line: 84)
!875 = !DISubprogram(name: "strcpy", scope: !862, file: !862, line: 141, type: !876, flags: DIFlagPrototyped, spFlags: 0)
!876 = !DISubroutineType(types: !877)
!877 = !{!878, !879, !880}
!878 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !44, size: 64)
!879 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !878)
!880 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !881)
!881 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !350, size: 64)
!882 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !883, file: !869, line: 85)
!883 = !DISubprogram(name: "strncpy", scope: !862, file: !862, line: 144, type: !884, flags: DIFlagPrototyped, spFlags: 0)
!884 = !DISubroutineType(types: !885)
!885 = !{!878, !879, !880, !756}
!886 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !887, file: !869, line: 86)
!887 = !DISubprogram(name: "strcat", scope: !862, file: !862, line: 149, type: !876, flags: DIFlagPrototyped, spFlags: 0)
!888 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !889, file: !869, line: 87)
!889 = !DISubprogram(name: "strncat", scope: !862, file: !862, line: 152, type: !884, flags: DIFlagPrototyped, spFlags: 0)
!890 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !891, file: !869, line: 88)
!891 = !DISubprogram(name: "memcmp", scope: !862, file: !862, line: 64, type: !892, flags: DIFlagPrototyped, spFlags: 0)
!892 = !DISubroutineType(types: !893)
!893 = !{!88, !867, !867, !756}
!894 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !895, file: !869, line: 89)
!895 = !DISubprogram(name: "strcmp", scope: !862, file: !862, line: 156, type: !896, flags: DIFlagPrototyped, spFlags: 0)
!896 = !DISubroutineType(types: !897)
!897 = !{!88, !881, !881}
!898 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !899, file: !869, line: 90)
!899 = !DISubprogram(name: "strncmp", scope: !862, file: !862, line: 159, type: !900, flags: DIFlagPrototyped, spFlags: 0)
!900 = !DISubroutineType(types: !901)
!901 = !{!88, !881, !881, !756}
!902 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !903, file: !869, line: 91)
!903 = !DISubprogram(name: "strcoll", scope: !862, file: !862, line: 163, type: !896, flags: DIFlagPrototyped, spFlags: 0)
!904 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !905, file: !869, line: 92)
!905 = !DISubprogram(name: "strxfrm", scope: !862, file: !862, line: 166, type: !906, flags: DIFlagPrototyped, spFlags: 0)
!906 = !DISubroutineType(types: !907)
!907 = !{!756, !879, !880, !756}
!908 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !909, file: !869, line: 93)
!909 = !DISubprogram(name: "memchr", scope: !862, file: !862, line: 89, type: !910, flags: DIFlagPrototyped, spFlags: 0)
!910 = !DISubroutineType(types: !911)
!911 = !{!867, !867, !88, !756}
!912 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !913, file: !869, line: 94)
!913 = !DISubprogram(name: "strchr", scope: !862, file: !862, line: 228, type: !914, flags: DIFlagPrototyped, spFlags: 0)
!914 = !DISubroutineType(types: !915)
!915 = !{!881, !881, !88}
!916 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !917, file: !869, line: 95)
!917 = !DISubprogram(name: "strcspn", scope: !862, file: !862, line: 293, type: !918, flags: DIFlagPrototyped, spFlags: 0)
!918 = !DISubroutineType(types: !919)
!919 = !{!756, !881, !881}
!920 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !921, file: !869, line: 96)
!921 = !DISubprogram(name: "strpbrk", scope: !862, file: !862, line: 305, type: !922, flags: DIFlagPrototyped, spFlags: 0)
!922 = !DISubroutineType(types: !923)
!923 = !{!881, !881, !881}
!924 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !925, file: !869, line: 97)
!925 = !DISubprogram(name: "strrchr", scope: !862, file: !862, line: 255, type: !914, flags: DIFlagPrototyped, spFlags: 0)
!926 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !927, file: !869, line: 98)
!927 = !DISubprogram(name: "strspn", scope: !862, file: !862, line: 297, type: !918, flags: DIFlagPrototyped, spFlags: 0)
!928 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !929, file: !869, line: 99)
!929 = !DISubprogram(name: "strstr", scope: !862, file: !862, line: 332, type: !922, flags: DIFlagPrototyped, spFlags: 0)
!930 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !931, file: !869, line: 100)
!931 = !DISubprogram(name: "strtok", scope: !862, file: !862, line: 356, type: !876, flags: DIFlagPrototyped, spFlags: 0)
!932 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !933, file: !869, line: 101)
!933 = !DISubprogram(name: "memset", scope: !862, file: !862, line: 61, type: !934, flags: DIFlagPrototyped, spFlags: 0)
!934 = !DISubroutineType(types: !935)
!935 = !{!754, !754, !88, !756}
!936 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !937, file: !869, line: 102)
!937 = !DISubprogram(name: "strerror", scope: !862, file: !862, line: 419, type: !938, flags: DIFlagPrototyped, spFlags: 0)
!938 = !DISubroutineType(types: !939)
!939 = !{!878, !88}
!940 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !941, file: !869, line: 103)
!941 = !DISubprogram(name: "strlen", scope: !862, file: !862, line: 407, type: !942, flags: DIFlagPrototyped, spFlags: 0)
!942 = !DISubroutineType(types: !943)
!943 = !{!756, !881}
!944 = !DIImportedEntity(tag: DW_TAG_imported_module, scope: !218, entity: !945, file: !227, line: 549)
!945 = !DINamespace(name: "chrono_literals", scope: !946, exportSymbols: true)
!946 = !DINamespace(name: "literals", scope: !6, exportSymbols: true)
!947 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !948, file: !951, line: 68)
!948 = !DIDerivedType(tag: DW_TAG_typedef, name: "clock_t", file: !949, line: 7, baseType: !950)
!949 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/clock_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "1aade99fd778d1551600c7ca1410b9f1")
!950 = !DIDerivedType(tag: DW_TAG_typedef, name: "__clock_t", file: !288, line: 156, baseType: !108)
!951 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/ctime", directory: "/home/toolchain/development")
!952 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !953, file: !951, line: 69)
!953 = !DIDerivedType(tag: DW_TAG_typedef, name: "time_t", file: !954, line: 10, baseType: !955)
!954 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/time_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "34d0816fa9aad5db7f457b5364ebec5f")
!955 = !DIDerivedType(tag: DW_TAG_typedef, name: "__time_t", file: !288, line: 160, baseType: !108)
!956 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !957, file: !951, line: 70)
!957 = !DICompositeType(tag: DW_TAG_structure_type, name: "tm", file: !958, line: 7, size: 448, flags: DIFlagFwdDecl, identifier: "_ZTS2tm")
!958 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/struct_tm.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "9e5545b565ef031c4cd0faf90b69386f")
!959 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !960, file: !951, line: 72)
!960 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "timespec", file: !961, line: 11, size: 128, flags: DIFlagTypePassByValue, elements: !962, identifier: "_ZTS8timespec")
!961 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/struct_timespec.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "9378e9ebbd658baccf881d3300eb1828")
!962 = !{!963, !964}
!963 = !DIDerivedType(tag: DW_TAG_member, name: "tv_sec", scope: !960, file: !961, line: 16, baseType: !955, size: 64)
!964 = !DIDerivedType(tag: DW_TAG_member, name: "tv_nsec", scope: !960, file: !961, line: 21, baseType: !965, size: 64, offset: 64)
!965 = !DIDerivedType(tag: DW_TAG_typedef, name: "__syscall_slong_t", file: !288, line: 197, baseType: !108)
!966 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !967, file: !951, line: 74)
!967 = !DISubprogram(name: "clock", scope: !968, file: !968, line: 72, type: !969, flags: DIFlagPrototyped, spFlags: 0)
!968 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/time.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "fc01762aedf7abce7804961f8acfd6e7")
!969 = !DISubroutineType(types: !970)
!970 = !{!948}
!971 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !972, file: !951, line: 75)
!972 = !DISubprogram(name: "difftime", scope: !968, file: !968, line: 79, type: !973, flags: DIFlagPrototyped, spFlags: 0)
!973 = !DISubroutineType(types: !974)
!974 = !{!975, !953, !953}
!975 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!976 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !977, file: !951, line: 76)
!977 = !DISubprogram(name: "mktime", scope: !968, file: !968, line: 82, type: !978, flags: DIFlagPrototyped, spFlags: 0)
!978 = !DISubroutineType(types: !979)
!979 = !{!953, !980}
!980 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !957, size: 64)
!981 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !982, file: !951, line: 77)
!982 = !DISubprogram(name: "time", scope: !968, file: !968, line: 76, type: !983, flags: DIFlagPrototyped, spFlags: 0)
!983 = !DISubroutineType(types: !984)
!984 = !{!953, !985}
!985 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !953, size: 64)
!986 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !987, file: !951, line: 78)
!987 = !DISubprogram(name: "asctime", scope: !968, file: !968, line: 179, type: !988, flags: DIFlagPrototyped, spFlags: 0)
!988 = !DISubroutineType(types: !989)
!989 = !{!878, !990}
!990 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !991, size: 64)
!991 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !957)
!992 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !993, file: !951, line: 79)
!993 = !DISubprogram(name: "ctime", scope: !968, file: !968, line: 183, type: !994, flags: DIFlagPrototyped, spFlags: 0)
!994 = !DISubroutineType(types: !995)
!995 = !{!878, !996}
!996 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !997, size: 64)
!997 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !953)
!998 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !999, file: !951, line: 80)
!999 = !DISubprogram(name: "gmtime", scope: !968, file: !968, line: 132, type: !1000, flags: DIFlagPrototyped, spFlags: 0)
!1000 = !DISubroutineType(types: !1001)
!1001 = !{!980, !996}
!1002 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1003, file: !951, line: 81)
!1003 = !DISubprogram(name: "localtime", scope: !968, file: !968, line: 136, type: !1000, flags: DIFlagPrototyped, spFlags: 0)
!1004 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1005, file: !951, line: 82)
!1005 = !DISubprogram(name: "strftime", scope: !968, file: !968, line: 99, type: !1006, flags: DIFlagPrototyped, spFlags: 0)
!1006 = !DISubroutineType(types: !1007)
!1007 = !{!756, !879, !756, !880, !1008}
!1008 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !990)
!1009 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1010, file: !951, line: 84)
!1010 = !DISubprogram(name: "timespec_get", scope: !968, file: !968, line: 383, type: !1011, flags: DIFlagPrototyped, spFlags: 0)
!1011 = !DISubroutineType(types: !1012)
!1012 = !{!88, !1013, !88}
!1013 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !960, size: 64)
!1014 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !742, entity: !1015, file: !1021, line: 420)
!1015 = !DISubprogram(name: "isfinite", linkageName: "_ZNSt3__16__math8isfiniteB9nqe220108Ee", scope: !1017, file: !1016, line: 66, type: !1018, flags: DIFlagPrototyped, spFlags: 0)
!1016 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__math/traits.h", directory: "/home/toolchain/development")
!1017 = !DINamespace(name: "__math", scope: !6)
!1018 = !DISubroutineType(types: !1019)
!1019 = !{!127, !1020}
!1020 = !DIBasicType(name: "long double", size: 128, encoding: DW_ATE_float)
!1021 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/math.h", directory: "/home/toolchain/development")
!1022 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !742, entity: !1023, file: !1021, line: 423)
!1023 = !DISubprogram(name: "isinf", linkageName: "_ZNSt3__16__math5isinfB9nqe220108Ee", scope: !1017, file: !1016, line: 90, type: !1018, flags: DIFlagPrototyped, spFlags: 0)
!1024 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !742, entity: !1025, file: !1021, line: 427)
!1025 = !DISubprogram(name: "isnan", linkageName: "_ZNSt3__16__math5isnanB9nqe220108Ee", scope: !1017, file: !1016, line: 114, type: !1018, flags: DIFlagPrototyped, spFlags: 0)
!1026 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !742, entity: !1027, file: !1021, line: 428)
!1027 = !DISubprogram(name: "isnormal", linkageName: "_ZNSt3__16__math8isnormalB9nqe220108Ee", scope: !1017, file: !1016, line: 133, type: !1018, flags: DIFlagPrototyped, spFlags: 0)
!1028 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !742, entity: !1029, file: !1021, line: 495)
!1029 = !DISubprogram(name: "modf", linkageName: "_ZNSt3__16__math4modfB9nqe220108EePe", scope: !1017, file: !1030, line: 55, type: !1031, flags: DIFlagPrototyped, spFlags: 0)
!1030 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__math/modulo.h", directory: "/home/toolchain/development")
!1031 = !DISubroutineType(types: !1032)
!1032 = !{!1020, !1020, !1033}
!1033 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1020, size: 64)
!1034 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1015, file: !1035, line: 352)
!1035 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cmath", directory: "/home/toolchain/development")
!1036 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1023, file: !1035, line: 353)
!1037 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1025, file: !1035, line: 354)
!1038 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1027, file: !1035, line: 355)
!1039 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1040, file: !1035, line: 364)
!1040 = !DIDerivedType(tag: DW_TAG_typedef, name: "float_t", file: !1041, line: 163, baseType: !1042)
!1041 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/math.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "0977e8e2fb84ef94cb89ae31455a7c7d")
!1042 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!1043 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1044, file: !1035, line: 365)
!1044 = !DIDerivedType(tag: DW_TAG_typedef, name: "double_t", file: !1041, line: 164, baseType: !975)
!1045 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1046, file: !1035, line: 370)
!1046 = !DISubprogram(name: "acosf", scope: !1047, file: !1047, line: 53, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1047 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/mathcalls.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "805f19fbe3cd3730c6c76acca0f9374c")
!1048 = !DISubroutineType(types: !1049)
!1049 = !{!1042, !1042}
!1050 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1051, file: !1035, line: 372)
!1051 = !DISubprogram(name: "asinf", scope: !1047, file: !1047, line: 55, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1052 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1053, file: !1035, line: 374)
!1053 = !DISubprogram(name: "atanf", scope: !1047, file: !1047, line: 57, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1054 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1055, file: !1035, line: 376)
!1055 = !DISubprogram(name: "atan2f", scope: !1047, file: !1047, line: 59, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1056 = !DISubroutineType(types: !1057)
!1057 = !{!1042, !1042, !1042}
!1058 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1059, file: !1035, line: 378)
!1059 = !DISubprogram(name: "ceilf", scope: !1047, file: !1047, line: 174, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1060 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1061, file: !1035, line: 380)
!1061 = !DISubprogram(name: "cosf", scope: !1047, file: !1047, line: 62, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1062 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1063, file: !1035, line: 382)
!1063 = !DISubprogram(name: "coshf", scope: !1047, file: !1047, line: 71, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1064 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1065, file: !1035, line: 385)
!1065 = !DISubprogram(name: "expf", scope: !1047, file: !1047, line: 95, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1066 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1067, file: !1035, line: 388)
!1067 = !DISubprogram(name: "fabsf", scope: !1047, file: !1047, line: 177, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1068 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1069, file: !1035, line: 390)
!1069 = !DISubprogram(name: "floorf", scope: !1047, file: !1047, line: 180, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1070 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1071, file: !1035, line: 393)
!1071 = !DISubprogram(name: "fmodf", scope: !1047, file: !1047, line: 183, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1072 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1073, file: !1035, line: 396)
!1073 = !DISubprogram(name: "frexpf", scope: !1047, file: !1047, line: 98, type: !1074, flags: DIFlagPrototyped, spFlags: 0)
!1074 = !DISubroutineType(types: !1075)
!1075 = !{!1042, !1042, !1076}
!1076 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !88, size: 64)
!1077 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1078, file: !1035, line: 398)
!1078 = !DISubprogram(name: "ldexpf", scope: !1047, file: !1047, line: 101, type: !1079, flags: DIFlagPrototyped, spFlags: 0)
!1079 = !DISubroutineType(types: !1080)
!1080 = !{!1042, !1042, !88}
!1081 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1082, file: !1035, line: 401)
!1082 = !DISubprogram(name: "logf", scope: !1047, file: !1047, line: 104, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1083 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1084, file: !1035, line: 404)
!1084 = !DISubprogram(name: "log10f", scope: !1047, file: !1047, line: 107, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1085 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1029, file: !1035, line: 405)
!1086 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1087, file: !1035, line: 406)
!1087 = !DISubprogram(name: "modff", scope: !1047, file: !1047, line: 110, type: !1088, flags: DIFlagPrototyped, spFlags: 0)
!1088 = !DISubroutineType(types: !1089)
!1089 = !{!1042, !1042, !1090}
!1090 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1042, size: 64)
!1091 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1092, file: !1035, line: 409)
!1092 = !DISubprogram(name: "powf", scope: !1047, file: !1047, line: 155, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1093 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1094, file: !1035, line: 412)
!1094 = !DISubprogram(name: "sinf", scope: !1047, file: !1047, line: 64, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1095 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1096, file: !1035, line: 414)
!1096 = !DISubprogram(name: "sinhf", scope: !1047, file: !1047, line: 73, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1097 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1098, file: !1035, line: 417)
!1098 = !DISubprogram(name: "sqrtf", scope: !1047, file: !1047, line: 158, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1099 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1100, file: !1035, line: 419)
!1100 = !DISubprogram(name: "tanf", scope: !1047, file: !1047, line: 66, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1101 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1102, file: !1035, line: 422)
!1102 = !DISubprogram(name: "tanhf", scope: !1047, file: !1047, line: 75, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1103 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1104, file: !1035, line: 425)
!1104 = !DISubprogram(name: "acoshf", scope: !1047, file: !1047, line: 85, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1105 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1106, file: !1035, line: 427)
!1106 = !DISubprogram(name: "asinhf", scope: !1047, file: !1047, line: 87, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1107 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1108, file: !1035, line: 429)
!1108 = !DISubprogram(name: "atanhf", scope: !1047, file: !1047, line: 89, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1109 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1110, file: !1035, line: 431)
!1110 = !DISubprogram(name: "cbrtf", scope: !1047, file: !1047, line: 167, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1111 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1112, file: !1035, line: 434)
!1112 = !DISubprogram(name: "copysignf", scope: !1047, file: !1047, line: 213, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1113 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1114, file: !1035, line: 437)
!1114 = !DISubprogram(name: "erff", scope: !1047, file: !1047, line: 246, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1115 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1116, file: !1035, line: 439)
!1116 = !DISubprogram(name: "erfcf", scope: !1047, file: !1047, line: 247, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1117 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1118, file: !1035, line: 441)
!1118 = !DISubprogram(name: "exp2f", scope: !1047, file: !1047, line: 145, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1119 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1120, file: !1035, line: 443)
!1120 = !DISubprogram(name: "expm1f", scope: !1047, file: !1047, line: 134, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1121 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1122, file: !1035, line: 445)
!1122 = !DISubprogram(name: "fdimf", scope: !1047, file: !1047, line: 344, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1123 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1124, file: !1035, line: 446)
!1124 = !DISubprogram(name: "fmaf", scope: !1047, file: !1047, line: 355, type: !1125, flags: DIFlagPrototyped, spFlags: 0)
!1125 = !DISubroutineType(types: !1126)
!1126 = !{!1042, !1042, !1042, !1042}
!1127 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1128, file: !1035, line: 449)
!1128 = !DISubprogram(name: "fmaxf", scope: !1047, file: !1047, line: 348, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1129 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1130, file: !1035, line: 451)
!1130 = !DISubprogram(name: "fminf", scope: !1047, file: !1047, line: 351, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1131 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1132, file: !1035, line: 453)
!1132 = !DISubprogram(name: "hypotf", scope: !1047, file: !1047, line: 162, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1133 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1134, file: !1035, line: 455)
!1134 = !DISubprogram(name: "ilogbf", scope: !1047, file: !1047, line: 298, type: !1135, flags: DIFlagPrototyped, spFlags: 0)
!1135 = !DISubroutineType(types: !1136)
!1136 = !{!88, !1042}
!1137 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1138, file: !1035, line: 457)
!1138 = !DISubprogram(name: "lgammaf", scope: !1047, file: !1047, line: 248, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1139 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1140, file: !1035, line: 459)
!1140 = !DISubprogram(name: "llrintf", scope: !1047, file: !1047, line: 334, type: !1141, flags: DIFlagPrototyped, spFlags: 0)
!1141 = !DISubroutineType(types: !1142)
!1142 = !{!48, !1042}
!1143 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1144, file: !1035, line: 461)
!1144 = !DISubprogram(name: "llroundf", scope: !1047, file: !1047, line: 340, type: !1141, flags: DIFlagPrototyped, spFlags: 0)
!1145 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1146, file: !1035, line: 463)
!1146 = !DISubprogram(name: "log1pf", scope: !1047, file: !1047, line: 137, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1147 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1148, file: !1035, line: 465)
!1148 = !DISubprogram(name: "log2f", scope: !1047, file: !1047, line: 148, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1149 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1150, file: !1035, line: 467)
!1150 = !DISubprogram(name: "logbf", scope: !1047, file: !1047, line: 140, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1151 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1152, file: !1035, line: 469)
!1152 = !DISubprogram(name: "lrintf", scope: !1047, file: !1047, line: 332, type: !1153, flags: DIFlagPrototyped, spFlags: 0)
!1153 = !DISubroutineType(types: !1154)
!1154 = !{!108, !1042}
!1155 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1156, file: !1035, line: 471)
!1156 = !DISubprogram(name: "lroundf", scope: !1047, file: !1047, line: 338, type: !1153, flags: DIFlagPrototyped, spFlags: 0)
!1157 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1158, file: !1035, line: 473)
!1158 = !DISubprogram(name: "nan", scope: !1047, file: !1047, line: 218, type: !1159, flags: DIFlagPrototyped, spFlags: 0)
!1159 = !DISubroutineType(types: !1160)
!1160 = !{!975, !881}
!1161 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1162, file: !1035, line: 474)
!1162 = !DISubprogram(name: "nanf", scope: !1047, file: !1047, line: 218, type: !1163, flags: DIFlagPrototyped, spFlags: 0)
!1163 = !DISubroutineType(types: !1164)
!1164 = !{!1042, !881}
!1165 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1166, file: !1035, line: 477)
!1166 = !DISubprogram(name: "nearbyintf", scope: !1047, file: !1047, line: 312, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1167 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1168, file: !1035, line: 479)
!1168 = !DISubprogram(name: "nextafterf", scope: !1047, file: !1047, line: 277, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1169 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1170, file: !1035, line: 481)
!1170 = !DISubprogram(name: "nexttowardf", scope: !1047, file: !1047, line: 279, type: !1171, flags: DIFlagPrototyped, spFlags: 0)
!1171 = !DISubroutineType(types: !1172)
!1172 = !{!1042, !1042, !1020}
!1173 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1174, file: !1035, line: 483)
!1174 = !DISubprogram(name: "remainderf", scope: !1047, file: !1047, line: 290, type: !1056, flags: DIFlagPrototyped, spFlags: 0)
!1175 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1176, file: !1035, line: 485)
!1176 = !DISubprogram(name: "remquof", scope: !1047, file: !1047, line: 325, type: !1177, flags: DIFlagPrototyped, spFlags: 0)
!1177 = !DISubroutineType(types: !1178)
!1178 = !{!1042, !1042, !1042, !1076}
!1179 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1180, file: !1035, line: 487)
!1180 = !DISubprogram(name: "rintf", scope: !1047, file: !1047, line: 274, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1181 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1182, file: !1035, line: 489)
!1182 = !DISubprogram(name: "roundf", scope: !1047, file: !1047, line: 316, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1183 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1184, file: !1035, line: 491)
!1184 = !DISubprogram(name: "scalblnf", scope: !1047, file: !1047, line: 308, type: !1185, flags: DIFlagPrototyped, spFlags: 0)
!1185 = !DISubroutineType(types: !1186)
!1186 = !{!1042, !1042, !108}
!1187 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1188, file: !1035, line: 493)
!1188 = !DISubprogram(name: "scalbnf", scope: !1047, file: !1047, line: 294, type: !1079, flags: DIFlagPrototyped, spFlags: 0)
!1189 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1190, file: !1035, line: 495)
!1190 = !DISubprogram(name: "tgammaf", scope: !1047, file: !1047, line: 253, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1191 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1192, file: !1035, line: 497)
!1192 = !DISubprogram(name: "truncf", scope: !1047, file: !1047, line: 320, type: !1048, flags: DIFlagPrototyped, spFlags: 0)
!1193 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1194, file: !1035, line: 499)
!1194 = !DISubprogram(name: "acosl", scope: !1047, file: !1047, line: 53, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1195 = !DISubroutineType(types: !1196)
!1196 = !{!1020, !1020}
!1197 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1198, file: !1035, line: 500)
!1198 = !DISubprogram(name: "asinl", scope: !1047, file: !1047, line: 55, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1199 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1200, file: !1035, line: 501)
!1200 = !DISubprogram(name: "atanl", scope: !1047, file: !1047, line: 57, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1201 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1202, file: !1035, line: 502)
!1202 = !DISubprogram(name: "atan2l", scope: !1047, file: !1047, line: 59, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1203 = !DISubroutineType(types: !1204)
!1204 = !{!1020, !1020, !1020}
!1205 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1206, file: !1035, line: 503)
!1206 = !DISubprogram(name: "ceill", scope: !1047, file: !1047, line: 174, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1207 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1208, file: !1035, line: 504)
!1208 = !DISubprogram(name: "cosl", scope: !1047, file: !1047, line: 62, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1209 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1210, file: !1035, line: 505)
!1210 = !DISubprogram(name: "coshl", scope: !1047, file: !1047, line: 71, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1211 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1212, file: !1035, line: 506)
!1212 = !DISubprogram(name: "expl", scope: !1047, file: !1047, line: 95, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1213 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1214, file: !1035, line: 507)
!1214 = !DISubprogram(name: "fabsl", scope: !1047, file: !1047, line: 177, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1215 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1216, file: !1035, line: 508)
!1216 = !DISubprogram(name: "floorl", scope: !1047, file: !1047, line: 180, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1217 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1218, file: !1035, line: 509)
!1218 = !DISubprogram(name: "fmodl", scope: !1047, file: !1047, line: 183, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1219 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1220, file: !1035, line: 510)
!1220 = !DISubprogram(name: "frexpl", scope: !1047, file: !1047, line: 98, type: !1221, flags: DIFlagPrototyped, spFlags: 0)
!1221 = !DISubroutineType(types: !1222)
!1222 = !{!1020, !1020, !1076}
!1223 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1224, file: !1035, line: 511)
!1224 = !DISubprogram(name: "ldexpl", scope: !1047, file: !1047, line: 101, type: !1225, flags: DIFlagPrototyped, spFlags: 0)
!1225 = !DISubroutineType(types: !1226)
!1226 = !{!1020, !1020, !88}
!1227 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1228, file: !1035, line: 512)
!1228 = !DISubprogram(name: "logl", scope: !1047, file: !1047, line: 104, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1229 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1230, file: !1035, line: 513)
!1230 = !DISubprogram(name: "log10l", scope: !1047, file: !1047, line: 107, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1231 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1232, file: !1035, line: 514)
!1232 = !DISubprogram(name: "modfl", scope: !1047, file: !1047, line: 110, type: !1031, flags: DIFlagPrototyped, spFlags: 0)
!1233 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1234, file: !1035, line: 515)
!1234 = !DISubprogram(name: "powl", scope: !1047, file: !1047, line: 155, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1235 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1236, file: !1035, line: 516)
!1236 = !DISubprogram(name: "sinl", scope: !1047, file: !1047, line: 64, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1237 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1238, file: !1035, line: 517)
!1238 = !DISubprogram(name: "sinhl", scope: !1047, file: !1047, line: 73, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1239 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1240, file: !1035, line: 518)
!1240 = !DISubprogram(name: "sqrtl", scope: !1047, file: !1047, line: 158, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1241 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1242, file: !1035, line: 519)
!1242 = !DISubprogram(name: "tanl", scope: !1047, file: !1047, line: 66, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1243 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1244, file: !1035, line: 521)
!1244 = !DISubprogram(name: "tanhl", scope: !1047, file: !1047, line: 75, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1245 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1246, file: !1035, line: 522)
!1246 = !DISubprogram(name: "acoshl", scope: !1047, file: !1047, line: 85, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1247 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1248, file: !1035, line: 523)
!1248 = !DISubprogram(name: "asinhl", scope: !1047, file: !1047, line: 87, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1249 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1250, file: !1035, line: 524)
!1250 = !DISubprogram(name: "atanhl", scope: !1047, file: !1047, line: 89, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1251 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1252, file: !1035, line: 525)
!1252 = !DISubprogram(name: "cbrtl", scope: !1047, file: !1047, line: 167, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1253 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1254, file: !1035, line: 527)
!1254 = !DISubprogram(name: "copysignl", scope: !1047, file: !1047, line: 213, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1255 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1256, file: !1035, line: 529)
!1256 = !DISubprogram(name: "erfl", scope: !1047, file: !1047, line: 246, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1257 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1258, file: !1035, line: 530)
!1258 = !DISubprogram(name: "erfcl", scope: !1047, file: !1047, line: 247, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1259 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1260, file: !1035, line: 531)
!1260 = !DISubprogram(name: "exp2l", scope: !1047, file: !1047, line: 145, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1261 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1262, file: !1035, line: 532)
!1262 = !DISubprogram(name: "expm1l", scope: !1047, file: !1047, line: 134, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1263 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1264, file: !1035, line: 533)
!1264 = !DISubprogram(name: "fdiml", scope: !1047, file: !1047, line: 344, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1265 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1266, file: !1035, line: 534)
!1266 = !DISubprogram(name: "fmal", scope: !1047, file: !1047, line: 355, type: !1267, flags: DIFlagPrototyped, spFlags: 0)
!1267 = !DISubroutineType(types: !1268)
!1268 = !{!1020, !1020, !1020, !1020}
!1269 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1270, file: !1035, line: 535)
!1270 = !DISubprogram(name: "fmaxl", scope: !1047, file: !1047, line: 348, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1271 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1272, file: !1035, line: 536)
!1272 = !DISubprogram(name: "fminl", scope: !1047, file: !1047, line: 351, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1273 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1274, file: !1035, line: 537)
!1274 = !DISubprogram(name: "hypotl", scope: !1047, file: !1047, line: 162, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1275 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1276, file: !1035, line: 538)
!1276 = !DISubprogram(name: "ilogbl", scope: !1047, file: !1047, line: 298, type: !1277, flags: DIFlagPrototyped, spFlags: 0)
!1277 = !DISubroutineType(types: !1278)
!1278 = !{!88, !1020}
!1279 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1280, file: !1035, line: 539)
!1280 = !DISubprogram(name: "lgammal", scope: !1047, file: !1047, line: 248, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1281 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1282, file: !1035, line: 540)
!1282 = !DISubprogram(name: "llrintl", scope: !1047, file: !1047, line: 334, type: !1283, flags: DIFlagPrototyped, spFlags: 0)
!1283 = !DISubroutineType(types: !1284)
!1284 = !{!48, !1020}
!1285 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1286, file: !1035, line: 541)
!1286 = !DISubprogram(name: "llroundl", scope: !1047, file: !1047, line: 340, type: !1283, flags: DIFlagPrototyped, spFlags: 0)
!1287 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1288, file: !1035, line: 542)
!1288 = !DISubprogram(name: "log1pl", scope: !1047, file: !1047, line: 137, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1289 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1290, file: !1035, line: 543)
!1290 = !DISubprogram(name: "log2l", scope: !1047, file: !1047, line: 148, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1291 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1292, file: !1035, line: 544)
!1292 = !DISubprogram(name: "logbl", scope: !1047, file: !1047, line: 140, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1293 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1294, file: !1035, line: 545)
!1294 = !DISubprogram(name: "lrintl", scope: !1047, file: !1047, line: 332, type: !1295, flags: DIFlagPrototyped, spFlags: 0)
!1295 = !DISubroutineType(types: !1296)
!1296 = !{!108, !1020}
!1297 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1298, file: !1035, line: 546)
!1298 = !DISubprogram(name: "lroundl", scope: !1047, file: !1047, line: 338, type: !1295, flags: DIFlagPrototyped, spFlags: 0)
!1299 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1300, file: !1035, line: 547)
!1300 = !DISubprogram(name: "nanl", scope: !1047, file: !1047, line: 218, type: !1301, flags: DIFlagPrototyped, spFlags: 0)
!1301 = !DISubroutineType(types: !1302)
!1302 = !{!1020, !881}
!1303 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1304, file: !1035, line: 548)
!1304 = !DISubprogram(name: "nearbyintl", scope: !1047, file: !1047, line: 312, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1305 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1306, file: !1035, line: 549)
!1306 = !DISubprogram(name: "nextafterl", scope: !1047, file: !1047, line: 277, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1307 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1308, file: !1035, line: 550)
!1308 = !DISubprogram(name: "nexttowardl", scope: !1047, file: !1047, line: 279, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1309 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1310, file: !1035, line: 551)
!1310 = !DISubprogram(name: "remainderl", scope: !1047, file: !1047, line: 290, type: !1203, flags: DIFlagPrototyped, spFlags: 0)
!1311 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1312, file: !1035, line: 552)
!1312 = !DISubprogram(name: "remquol", scope: !1047, file: !1047, line: 325, type: !1313, flags: DIFlagPrototyped, spFlags: 0)
!1313 = !DISubroutineType(types: !1314)
!1314 = !{!1020, !1020, !1020, !1076}
!1315 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1316, file: !1035, line: 553)
!1316 = !DISubprogram(name: "rintl", scope: !1047, file: !1047, line: 274, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1317 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1318, file: !1035, line: 554)
!1318 = !DISubprogram(name: "roundl", scope: !1047, file: !1047, line: 316, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1319 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1320, file: !1035, line: 555)
!1320 = !DISubprogram(name: "scalblnl", scope: !1047, file: !1047, line: 308, type: !1321, flags: DIFlagPrototyped, spFlags: 0)
!1321 = !DISubroutineType(types: !1322)
!1322 = !{!1020, !1020, !108}
!1323 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1324, file: !1035, line: 556)
!1324 = !DISubprogram(name: "scalbnl", scope: !1047, file: !1047, line: 294, type: !1225, flags: DIFlagPrototyped, spFlags: 0)
!1325 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1326, file: !1035, line: 557)
!1326 = !DISubprogram(name: "tgammal", scope: !1047, file: !1047, line: 253, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1327 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1328, file: !1035, line: 558)
!1328 = !DISubprogram(name: "truncl", scope: !1047, file: !1047, line: 320, type: !1195, flags: DIFlagPrototyped, spFlags: 0)
!1329 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1330, file: !1336, line: 22)
!1330 = !DIDerivedType(tag: DW_TAG_typedef, name: "max_align_t", file: !1331, line: 24, baseType: !1332)
!1331 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/lib64/clang/22/include/__stddef_max_align_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "3c0a2f19d136d39aa835c737c7105def")
!1332 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1331, line: 19, size: 256, flags: DIFlagTypePassByValue, elements: !1333, identifier: "_ZTS11max_align_t")
!1333 = !{!1334, !1335}
!1334 = !DIDerivedType(tag: DW_TAG_member, name: "__clang_max_align_nonce1", scope: !1332, file: !1331, line: 20, baseType: !48, size: 64, align: 64)
!1335 = !DIDerivedType(tag: DW_TAG_member, name: "__clang_max_align_nonce2", scope: !1332, file: !1331, line: 22, baseType: !1020, size: 128, align: 128, offset: 128)
!1336 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__cstddef/max_align_t.h", directory: "/home/toolchain/development")
!1337 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1338, file: !1341, line: 106)
!1338 = !DIDerivedType(tag: DW_TAG_typedef, name: "div_t", file: !1339, line: 63, baseType: !1340)
!1339 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/stdlib.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "a032969b0aa823d3e66e5bfb69d943da")
!1340 = !DICompositeType(tag: DW_TAG_structure_type, file: !1339, line: 59, size: 64, flags: DIFlagFwdDecl, identifier: "_ZTS5div_t")
!1341 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstdlib", directory: "/home/toolchain/development")
!1342 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1343, file: !1341, line: 107)
!1343 = !DIDerivedType(tag: DW_TAG_typedef, name: "ldiv_t", file: !1339, line: 71, baseType: !1344)
!1344 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1339, line: 67, size: 128, flags: DIFlagTypePassByValue, elements: !1345, identifier: "_ZTS6ldiv_t")
!1345 = !{!1346, !1347}
!1346 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !1344, file: !1339, line: 69, baseType: !108, size: 64)
!1347 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !1344, file: !1339, line: 70, baseType: !108, size: 64, offset: 64)
!1348 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1349, file: !1341, line: 108)
!1349 = !DIDerivedType(tag: DW_TAG_typedef, name: "lldiv_t", file: !1339, line: 81, baseType: !1350)
!1350 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1339, line: 77, size: 128, flags: DIFlagTypePassByValue, elements: !1351, identifier: "_ZTS7lldiv_t")
!1351 = !{!1352, !1353}
!1352 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !1350, file: !1339, line: 79, baseType: !48, size: 64)
!1353 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !1350, file: !1339, line: 80, baseType: !48, size: 64, offset: 64)
!1354 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1355, file: !1341, line: 109)
!1355 = !DISubprogram(name: "atof", scope: !1339, file: !1339, line: 102, type: !1159, flags: DIFlagPrototyped, spFlags: 0)
!1356 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1357, file: !1341, line: 110)
!1357 = !DISubprogram(name: "atoi", scope: !1339, file: !1339, line: 105, type: !1358, flags: DIFlagPrototyped, spFlags: 0)
!1358 = !DISubroutineType(types: !1359)
!1359 = !{!88, !881}
!1360 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1361, file: !1341, line: 111)
!1361 = !DISubprogram(name: "atol", scope: !1339, file: !1339, line: 108, type: !1362, flags: DIFlagPrototyped, spFlags: 0)
!1362 = !DISubroutineType(types: !1363)
!1363 = !{!108, !881}
!1364 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1365, file: !1341, line: 112)
!1365 = !DISubprogram(name: "atoll", scope: !1339, file: !1339, line: 113, type: !1366, flags: DIFlagPrototyped, spFlags: 0)
!1366 = !DISubroutineType(types: !1367)
!1367 = !{!48, !881}
!1368 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1369, file: !1341, line: 113)
!1369 = !DISubprogram(name: "strtod", scope: !1339, file: !1339, line: 118, type: !1370, flags: DIFlagPrototyped, spFlags: 0)
!1370 = !DISubroutineType(types: !1371)
!1371 = !{!975, !880, !1372}
!1372 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1373)
!1373 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !878, size: 64)
!1374 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1375, file: !1341, line: 114)
!1375 = !DISubprogram(name: "strtof", scope: !1339, file: !1339, line: 124, type: !1376, flags: DIFlagPrototyped, spFlags: 0)
!1376 = !DISubroutineType(types: !1377)
!1377 = !{!1042, !880, !1372}
!1378 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1379, file: !1341, line: 115)
!1379 = !DISubprogram(name: "strtold", scope: !1339, file: !1339, line: 127, type: !1380, flags: DIFlagPrototyped, spFlags: 0)
!1380 = !DISubroutineType(types: !1381)
!1381 = !{!1020, !880, !1372}
!1382 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1383, file: !1341, line: 116)
!1383 = !DISubprogram(name: "strtol", linkageName: "__isoc23_strtol", scope: !1339, file: !1339, line: 215, type: !1384, flags: DIFlagPrototyped, spFlags: 0)
!1384 = !DISubroutineType(types: !1385)
!1385 = !{!108, !880, !1372, !88}
!1386 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1387, file: !1341, line: 117)
!1387 = !DISubprogram(name: "strtoll", linkageName: "__isoc23_strtoll", scope: !1339, file: !1339, line: 238, type: !1388, flags: DIFlagPrototyped, spFlags: 0)
!1388 = !DISubroutineType(types: !1389)
!1389 = !{!48, !880, !1372, !88}
!1390 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1391, file: !1341, line: 118)
!1391 = !DISubprogram(name: "strtoul", linkageName: "__isoc23_strtoul", scope: !1339, file: !1339, line: 219, type: !1392, flags: DIFlagPrototyped, spFlags: 0)
!1392 = !DISubroutineType(types: !1393)
!1393 = !{!758, !880, !1372, !88}
!1394 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1395, file: !1341, line: 119)
!1395 = !DISubprogram(name: "strtoull", linkageName: "__isoc23_strtoull", scope: !1339, file: !1339, line: 243, type: !1396, flags: DIFlagPrototyped, spFlags: 0)
!1396 = !DISubroutineType(types: !1397)
!1397 = !{!26, !880, !1372, !88}
!1398 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1399, file: !1341, line: 120)
!1399 = !DISubprogram(name: "rand", scope: !1339, file: !1339, line: 573, type: !1400, flags: DIFlagPrototyped, spFlags: 0)
!1400 = !DISubroutineType(types: !1401)
!1401 = !{!88}
!1402 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1403, file: !1341, line: 121)
!1403 = !DISubprogram(name: "srand", scope: !1339, file: !1339, line: 575, type: !1404, flags: DIFlagPrototyped, spFlags: 0)
!1404 = !DISubroutineType(types: !1405)
!1405 = !{null, !31}
!1406 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1407, file: !1341, line: 122)
!1407 = !DISubprogram(name: "calloc", scope: !1339, file: !1339, line: 675, type: !1408, flags: DIFlagPrototyped, spFlags: 0)
!1408 = !DISubroutineType(types: !1409)
!1409 = !{!754, !756, !756}
!1410 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1411, file: !1341, line: 123)
!1411 = !DISubprogram(name: "free", scope: !1339, file: !1339, line: 687, type: !1412, flags: DIFlagPrototyped, spFlags: 0)
!1412 = !DISubroutineType(types: !1413)
!1413 = !{null, !754}
!1414 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1415, file: !1341, line: 124)
!1415 = !DISubprogram(name: "malloc", scope: !1339, file: !1339, line: 672, type: !1416, flags: DIFlagPrototyped, spFlags: 0)
!1416 = !DISubroutineType(types: !1417)
!1417 = !{!754, !756}
!1418 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1419, file: !1341, line: 125)
!1419 = !DISubprogram(name: "realloc", scope: !1339, file: !1339, line: 683, type: !1420, flags: DIFlagPrototyped, spFlags: 0)
!1420 = !DISubroutineType(types: !1421)
!1421 = !{!754, !754, !756}
!1422 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1423, file: !1341, line: 126)
!1423 = !DISubprogram(name: "abort", scope: !1339, file: !1339, line: 730, type: !1424, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1424 = !DISubroutineType(types: !1425)
!1425 = !{null}
!1426 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1427, file: !1341, line: 127)
!1427 = !DISubprogram(name: "atexit", scope: !1339, file: !1339, line: 734, type: !1428, flags: DIFlagPrototyped, spFlags: 0)
!1428 = !DISubroutineType(types: !1429)
!1429 = !{!88, !1430}
!1430 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1424, size: 64)
!1431 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1432, file: !1341, line: 128)
!1432 = !DISubprogram(name: "exit", scope: !1339, file: !1339, line: 756, type: !1433, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1433 = !DISubroutineType(types: !1434)
!1434 = !{null, !88}
!1435 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1436, file: !1341, line: 129)
!1436 = !DISubprogram(name: "_Exit", scope: !1339, file: !1339, line: 768, type: !1433, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1437 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1438, file: !1341, line: 130)
!1438 = !DISubprogram(name: "getenv", scope: !1339, file: !1339, line: 773, type: !1439, flags: DIFlagPrototyped, spFlags: 0)
!1439 = !DISubroutineType(types: !1440)
!1440 = !{!878, !881}
!1441 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1442, file: !1341, line: 131)
!1442 = !DISubprogram(name: "system", scope: !1339, file: !1339, line: 923, type: !1358, flags: DIFlagPrototyped, spFlags: 0)
!1443 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1444, file: !1341, line: 132)
!1444 = !DISubprogram(name: "bsearch", scope: !1339, file: !1339, line: 960, type: !1445, flags: DIFlagPrototyped, spFlags: 0)
!1445 = !DISubroutineType(types: !1446)
!1446 = !{!754, !867, !867, !756, !756, !1447}
!1447 = !DIDerivedType(tag: DW_TAG_typedef, name: "__compar_fn_t", file: !1339, line: 948, baseType: !1448)
!1448 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1449, size: 64)
!1449 = !DISubroutineType(types: !1450)
!1450 = !{!88, !867, !867}
!1451 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1452, file: !1341, line: 133)
!1452 = !DISubprogram(name: "qsort", scope: !1339, file: !1339, line: 970, type: !1453, flags: DIFlagPrototyped, spFlags: 0)
!1453 = !DISubroutineType(types: !1454)
!1454 = !{null, !754, !756, !756, !1447}
!1455 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1456, file: !1341, line: 134)
!1456 = !DISubprogram(name: "abs", scope: !1339, file: !1339, line: 980, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1457 = !DISubroutineType(types: !1458)
!1458 = !{!88, !88}
!1459 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1460, file: !1341, line: 135)
!1460 = !DISubprogram(name: "labs", scope: !1339, file: !1339, line: 981, type: !1461, flags: DIFlagPrototyped, spFlags: 0)
!1461 = !DISubroutineType(types: !1462)
!1462 = !{!108, !108}
!1463 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1464, file: !1341, line: 136)
!1464 = !DISubprogram(name: "llabs", scope: !1339, file: !1339, line: 984, type: !1465, flags: DIFlagPrototyped, spFlags: 0)
!1465 = !DISubroutineType(types: !1466)
!1466 = !{!48, !48}
!1467 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1468, file: !1341, line: 137)
!1468 = !DISubprogram(name: "div", linkageName: "_Z3divB9nqe220108xx", scope: !1469, file: !1469, line: 128, type: !1470, flags: DIFlagPrototyped, spFlags: 0)
!1469 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/stdlib.h", directory: "/home/toolchain/development")
!1470 = !DISubroutineType(types: !1471)
!1471 = !{!1349, !48, !48}
!1472 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1473, file: !1341, line: 138)
!1473 = !DISubprogram(name: "ldiv", scope: !1339, file: !1339, line: 994, type: !1474, flags: DIFlagPrototyped, spFlags: 0)
!1474 = !DISubroutineType(types: !1475)
!1475 = !{!1343, !108, !108}
!1476 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1477, file: !1341, line: 139)
!1477 = !DISubprogram(name: "lldiv", scope: !1339, file: !1339, line: 998, type: !1470, flags: DIFlagPrototyped, spFlags: 0)
!1478 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1479, file: !1341, line: 140)
!1479 = !DISubprogram(name: "mblen", scope: !1339, file: !1339, line: 1062, type: !1480, flags: DIFlagPrototyped, spFlags: 0)
!1480 = !DISubroutineType(types: !1481)
!1481 = !{!88, !881, !756}
!1482 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1483, file: !1341, line: 142)
!1483 = !DISubprogram(name: "mbtowc", scope: !1339, file: !1339, line: 1065, type: !1484, flags: DIFlagPrototyped, spFlags: 0)
!1484 = !DISubroutineType(types: !1485)
!1485 = !{!88, !1486, !880, !756}
!1486 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1487)
!1487 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1488, size: 64)
!1488 = !DIBasicType(name: "wchar_t", size: 32, encoding: DW_ATE_signed)
!1489 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1490, file: !1341, line: 143)
!1490 = !DISubprogram(name: "wctomb", scope: !1339, file: !1339, line: 1069, type: !1491, flags: DIFlagPrototyped, spFlags: 0)
!1491 = !DISubroutineType(types: !1492)
!1492 = !{!88, !878, !1488}
!1493 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1494, file: !1341, line: 144)
!1494 = !DISubprogram(name: "mbstowcs", scope: !1339, file: !1339, line: 1073, type: !1495, flags: DIFlagPrototyped, spFlags: 0)
!1495 = !DISubroutineType(types: !1496)
!1496 = !{!756, !1486, !880, !756}
!1497 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1498, file: !1341, line: 145)
!1498 = !DISubprogram(name: "wcstombs", scope: !1339, file: !1339, line: 1077, type: !1499, flags: DIFlagPrototyped, spFlags: 0)
!1499 = !DISubroutineType(types: !1500)
!1500 = !{!756, !879, !1501, !756}
!1501 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1502)
!1502 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1503, size: 64)
!1503 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !1488)
!1504 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1505, file: !1341, line: 148)
!1505 = !DISubprogram(name: "at_quick_exit", scope: !1339, file: !1339, line: 739, type: !1428, flags: DIFlagPrototyped, spFlags: 0)
!1506 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1507, file: !1341, line: 149)
!1507 = !DISubprogram(name: "quick_exit", scope: !1339, file: !1339, line: 762, type: !1433, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1508 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1509, file: !1341, line: 152)
!1509 = !DISubprogram(name: "aligned_alloc", scope: !1339, file: !1339, line: 724, type: !1408, flags: DIFlagPrototyped, spFlags: 0)
!1510 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1511, file: !1526, line: 25)
!1511 = !DIDerivedType(tag: DW_TAG_typedef, name: "mbstate_t", file: !1512, line: 6, baseType: !1513)
!1512 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/mbstate_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "ba8742313715e20e434cf6ccb2db98e3")
!1513 = !DIDerivedType(tag: DW_TAG_typedef, name: "__mbstate_t", file: !1514, line: 21, baseType: !1515)
!1514 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/__mbstate_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "82911a3e689448e3691ded3e0b471a55")
!1515 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1514, line: 13, size: 64, flags: DIFlagTypePassByValue, elements: !1516, identifier: "_ZTS11__mbstate_t")
!1516 = !{!1517, !1518}
!1517 = !DIDerivedType(tag: DW_TAG_member, name: "__count", scope: !1515, file: !1514, line: 15, baseType: !88, size: 32)
!1518 = !DIDerivedType(tag: DW_TAG_member, name: "__value", scope: !1515, file: !1514, line: 20, baseType: !1519, size: 32, offset: 32)
!1519 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !1515, file: !1514, line: 16, size: 32, flags: DIFlagTypePassByValue, elements: !1520, identifier: "_ZTSN11__mbstate_tUt_E")
!1520 = !{!1521, !1522}
!1521 = !DIDerivedType(tag: DW_TAG_member, name: "__wch", scope: !1519, file: !1514, line: 18, baseType: !31, size: 32)
!1522 = !DIDerivedType(tag: DW_TAG_member, name: "__wchb", scope: !1519, file: !1514, line: 19, baseType: !1523, size: 32)
!1523 = !DICompositeType(tag: DW_TAG_array_type, baseType: !44, size: 32, elements: !1524)
!1524 = !{!1525}
!1525 = !DISubrange(count: 4)
!1526 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__std_mbstate_t.h", directory: "/home/toolchain/development")
!1527 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1528, file: !1530, line: 113)
!1528 = !DISubprogram(name: "isalnum", scope: !1529, file: !1529, line: 108, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1529 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/ctype.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "43fd45dcf96e8fb7d8f14700096497c7")
!1530 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cctype", directory: "/home/toolchain/development")
!1531 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1532, file: !1530, line: 114)
!1532 = !DISubprogram(name: "isalpha", scope: !1529, file: !1529, line: 109, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1533 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1534, file: !1530, line: 115)
!1534 = !DISubprogram(name: "isblank", scope: !1529, file: !1529, line: 130, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1535 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1536, file: !1530, line: 116)
!1536 = !DISubprogram(name: "iscntrl", scope: !1529, file: !1529, line: 110, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1537 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1538, file: !1530, line: 117)
!1538 = !DISubprogram(name: "isdigit", scope: !1529, file: !1529, line: 111, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1539 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1540, file: !1530, line: 118)
!1540 = !DISubprogram(name: "isgraph", scope: !1529, file: !1529, line: 113, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1541 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1542, file: !1530, line: 119)
!1542 = !DISubprogram(name: "islower", scope: !1529, file: !1529, line: 112, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1543 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1544, file: !1530, line: 120)
!1544 = !DISubprogram(name: "isprint", scope: !1529, file: !1529, line: 114, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1545 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1546, file: !1530, line: 121)
!1546 = !DISubprogram(name: "ispunct", scope: !1529, file: !1529, line: 115, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1547 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1548, file: !1530, line: 122)
!1548 = !DISubprogram(name: "isspace", scope: !1529, file: !1529, line: 116, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1549 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1550, file: !1530, line: 123)
!1550 = !DISubprogram(name: "isupper", scope: !1529, file: !1529, line: 117, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1551 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1552, file: !1530, line: 124)
!1552 = !DISubprogram(name: "isxdigit", scope: !1529, file: !1529, line: 118, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1553 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1554, file: !1530, line: 125)
!1554 = !DISubprogram(name: "tolower", scope: !1529, file: !1529, line: 122, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1555 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1556, file: !1530, line: 126)
!1556 = !DISubprogram(name: "toupper", scope: !1529, file: !1529, line: 125, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!1557 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1558, file: !1560, line: 75)
!1558 = !DIDerivedType(tag: DW_TAG_typedef, name: "wint_t", file: !1559, line: 20, baseType: !31)
!1559 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/wint_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "aa31b53ef28dc23152ceb41e2763ded3")
!1560 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cwctype", directory: "/home/toolchain/development")
!1561 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1562, file: !1560, line: 76)
!1562 = !DIDerivedType(tag: DW_TAG_typedef, name: "wctrans_t", file: !1563, line: 48, baseType: !1564)
!1563 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/wctype.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "eff95da6508e8f67a3c7b77d9d8ab229")
!1564 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1565, size: 64)
!1565 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !793)
!1566 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1567, file: !1560, line: 77)
!1567 = !DIDerivedType(tag: DW_TAG_typedef, name: "wctype_t", file: !1568, line: 38, baseType: !758)
!1568 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/wctype-wchar.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "7f19501745f9a1fbbace8f0f185de59a")
!1569 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1570, file: !1560, line: 78)
!1570 = !DISubprogram(name: "iswalnum", scope: !1568, file: !1568, line: 95, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1571 = !DISubroutineType(types: !1572)
!1572 = !{!88, !1558}
!1573 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1574, file: !1560, line: 79)
!1574 = !DISubprogram(name: "iswalpha", scope: !1568, file: !1568, line: 101, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1575 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1576, file: !1560, line: 80)
!1576 = !DISubprogram(name: "iswblank", scope: !1568, file: !1568, line: 146, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1577 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1578, file: !1560, line: 81)
!1578 = !DISubprogram(name: "iswcntrl", scope: !1568, file: !1568, line: 104, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1579 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1580, file: !1560, line: 82)
!1580 = !DISubprogram(name: "iswdigit", scope: !1568, file: !1568, line: 108, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1581 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1582, file: !1560, line: 83)
!1582 = !DISubprogram(name: "iswgraph", scope: !1568, file: !1568, line: 112, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1583 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1584, file: !1560, line: 84)
!1584 = !DISubprogram(name: "iswlower", scope: !1568, file: !1568, line: 117, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1585 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1586, file: !1560, line: 85)
!1586 = !DISubprogram(name: "iswprint", scope: !1568, file: !1568, line: 120, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1587 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1588, file: !1560, line: 86)
!1588 = !DISubprogram(name: "iswpunct", scope: !1568, file: !1568, line: 125, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1589 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1590, file: !1560, line: 87)
!1590 = !DISubprogram(name: "iswspace", scope: !1568, file: !1568, line: 130, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1591 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1592, file: !1560, line: 88)
!1592 = !DISubprogram(name: "iswupper", scope: !1568, file: !1568, line: 135, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1593 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1594, file: !1560, line: 89)
!1594 = !DISubprogram(name: "iswxdigit", scope: !1568, file: !1568, line: 140, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1595 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1596, file: !1560, line: 90)
!1596 = !DISubprogram(name: "iswctype", scope: !1568, file: !1568, line: 159, type: !1597, flags: DIFlagPrototyped, spFlags: 0)
!1597 = !DISubroutineType(types: !1598)
!1598 = !{!88, !1558, !1567}
!1599 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1600, file: !1560, line: 91)
!1600 = !DISubprogram(name: "wctype", scope: !1568, file: !1568, line: 155, type: !1601, flags: DIFlagPrototyped, spFlags: 0)
!1601 = !DISubroutineType(types: !1602)
!1602 = !{!1567, !881}
!1603 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1604, file: !1560, line: 92)
!1604 = !DISubprogram(name: "towlower", scope: !1568, file: !1568, line: 166, type: !1605, flags: DIFlagPrototyped, spFlags: 0)
!1605 = !DISubroutineType(types: !1606)
!1606 = !{!1558, !1558}
!1607 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1608, file: !1560, line: 93)
!1608 = !DISubprogram(name: "towupper", scope: !1568, file: !1568, line: 169, type: !1605, flags: DIFlagPrototyped, spFlags: 0)
!1609 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1610, file: !1560, line: 94)
!1610 = !DISubprogram(name: "towctrans", scope: !1563, file: !1563, line: 55, type: !1611, flags: DIFlagPrototyped, spFlags: 0)
!1611 = !DISubroutineType(types: !1612)
!1612 = !{!1558, !1558, !1562}
!1613 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1614, file: !1560, line: 95)
!1614 = !DISubprogram(name: "wctrans", scope: !1563, file: !1563, line: 52, type: !1615, flags: DIFlagPrototyped, spFlags: 0)
!1615 = !DISubroutineType(types: !1616)
!1616 = !{!1562, !881}
!1617 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1511, file: !1618, line: 134)
!1618 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cwchar", directory: "/home/toolchain/development")
!1619 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !957, file: !1618, line: 135)
!1620 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1558, file: !1618, line: 136)
!1621 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1622, file: !1618, line: 137)
!1622 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !1623, line: 7, baseType: !1624)
!1623 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/FILE.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "571f9fb6223c42439075fdde11a0de5d")
!1624 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !1625, line: 4, flags: DIFlagFwdDecl | DIFlagNonTrivial, identifier: "_ZTS8_IO_FILE")
!1625 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/__FILE.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "72a8fe90981f484acae7c6f3dfc5c2b7")
!1626 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1627, file: !1618, line: 138)
!1627 = !DISubprogram(name: "fwprintf", scope: !1628, file: !1628, line: 732, type: !1629, flags: DIFlagPrototyped, spFlags: 0)
!1628 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/wchar.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "91346206ecdf1575ff4fea6ab909018f")
!1629 = !DISubroutineType(types: !1630)
!1630 = !{!88, !1631, !1501, null}
!1631 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1632)
!1632 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1633, size: 64)
!1633 = !DIDerivedType(tag: DW_TAG_typedef, name: "__FILE", file: !1625, line: 5, baseType: !1624)
!1634 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1635, file: !1618, line: 139)
!1635 = !DISubprogram(name: "fwscanf", linkageName: "__isoc23_fwscanf", scope: !1628, file: !1628, line: 795, type: !1629, flags: DIFlagPrototyped, spFlags: 0)
!1636 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1637, file: !1618, line: 140)
!1637 = !DISubprogram(name: "swprintf", scope: !1628, file: !1628, line: 742, type: !1638, flags: DIFlagPrototyped, spFlags: 0)
!1638 = !DISubroutineType(types: !1639)
!1639 = !{!88, !1486, !756, !1501, null}
!1640 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1641, file: !1618, line: 141)
!1641 = !DISubprogram(name: "vfwprintf", scope: !1628, file: !1628, line: 750, type: !1642, flags: DIFlagPrototyped, spFlags: 0)
!1642 = !DISubroutineType(types: !1643)
!1643 = !{!88, !1631, !1501, !1644}
!1644 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1645, size: 64)
!1645 = !DICompositeType(tag: DW_TAG_structure_type, name: "__va_list_tag", file: !3, size: 192, flags: DIFlagFwdDecl, identifier: "_ZTS13__va_list_tag")
!1646 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1647, file: !1618, line: 142)
!1647 = !DISubprogram(name: "vswprintf", scope: !1628, file: !1628, line: 763, type: !1648, flags: DIFlagPrototyped, spFlags: 0)
!1648 = !DISubroutineType(types: !1649)
!1649 = !{!88, !1486, !756, !1501, !1644}
!1650 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1651, file: !1618, line: 143)
!1651 = !DISubprogram(name: "swscanf", linkageName: "__isoc23_swscanf", scope: !1628, file: !1628, line: 802, type: !1652, flags: DIFlagPrototyped, spFlags: 0)
!1652 = !DISubroutineType(types: !1653)
!1653 = !{!88, !1501, !1501, null}
!1654 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1655, file: !1618, line: 144)
!1655 = !DISubprogram(name: "vfwscanf", linkageName: "__isoc23_vfwscanf", scope: !1628, file: !1628, line: 875, type: !1642, flags: DIFlagPrototyped, spFlags: 0)
!1656 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1657, file: !1618, line: 145)
!1657 = !DISubprogram(name: "vswscanf", linkageName: "__isoc23_vswscanf", scope: !1628, file: !1628, line: 882, type: !1658, flags: DIFlagPrototyped, spFlags: 0)
!1658 = !DISubroutineType(types: !1659)
!1659 = !{!88, !1501, !1501, !1644}
!1660 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1661, file: !1618, line: 146)
!1661 = !DISubprogram(name: "fgetwc", scope: !1628, file: !1628, line: 935, type: !1662, flags: DIFlagPrototyped, spFlags: 0)
!1662 = !DISubroutineType(types: !1663)
!1663 = !{!1558, !1632}
!1664 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1665, file: !1618, line: 147)
!1665 = !DISubprogram(name: "fgetws", scope: !1628, file: !1628, line: 964, type: !1666, flags: DIFlagPrototyped, spFlags: 0)
!1666 = !DISubroutineType(types: !1667)
!1667 = !{!1487, !1486, !88, !1631}
!1668 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1669, file: !1618, line: 148)
!1669 = !DISubprogram(name: "fputwc", scope: !1628, file: !1628, line: 949, type: !1670, flags: DIFlagPrototyped, spFlags: 0)
!1670 = !DISubroutineType(types: !1671)
!1671 = !{!1558, !1488, !1632}
!1672 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1673, file: !1618, line: 149)
!1673 = !DISubprogram(name: "fputws", scope: !1628, file: !1628, line: 971, type: !1674, flags: DIFlagPrototyped, spFlags: 0)
!1674 = !DISubroutineType(types: !1675)
!1675 = !{!88, !1501, !1631}
!1676 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1677, file: !1618, line: 150)
!1677 = !DISubprogram(name: "fwide", scope: !1628, file: !1628, line: 725, type: !1678, flags: DIFlagPrototyped, spFlags: 0)
!1678 = !DISubroutineType(types: !1679)
!1679 = !{!88, !1632, !88}
!1680 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1681, file: !1618, line: 151)
!1681 = !DISubprogram(name: "getwc", scope: !1628, file: !1628, line: 936, type: !1662, flags: DIFlagPrototyped, spFlags: 0)
!1682 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1683, file: !1618, line: 152)
!1683 = !DISubprogram(name: "putwc", scope: !1628, file: !1628, line: 950, type: !1670, flags: DIFlagPrototyped, spFlags: 0)
!1684 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1685, file: !1618, line: 153)
!1685 = !DISubprogram(name: "ungetwc", scope: !1628, file: !1628, line: 979, type: !1686, flags: DIFlagPrototyped, spFlags: 0)
!1686 = !DISubroutineType(types: !1687)
!1687 = !{!1558, !1558, !1632}
!1688 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1689, file: !1618, line: 154)
!1689 = !DISubprogram(name: "wcstod", scope: !1628, file: !1628, line: 402, type: !1690, flags: DIFlagPrototyped, spFlags: 0)
!1690 = !DISubroutineType(types: !1691)
!1691 = !{!975, !1501, !1692}
!1692 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1693)
!1693 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1487, size: 64)
!1694 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1695, file: !1618, line: 155)
!1695 = !DISubprogram(name: "wcstof", scope: !1628, file: !1628, line: 407, type: !1696, flags: DIFlagPrototyped, spFlags: 0)
!1696 = !DISubroutineType(types: !1697)
!1697 = !{!1042, !1501, !1692}
!1698 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1699, file: !1618, line: 156)
!1699 = !DISubprogram(name: "wcstold", scope: !1628, file: !1628, line: 409, type: !1700, flags: DIFlagPrototyped, spFlags: 0)
!1700 = !DISubroutineType(types: !1701)
!1701 = !{!1020, !1501, !1692}
!1702 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1703, file: !1618, line: 157)
!1703 = !DISubprogram(name: "wcstol", linkageName: "__isoc23_wcstol", scope: !1628, file: !1628, line: 500, type: !1704, flags: DIFlagPrototyped, spFlags: 0)
!1704 = !DISubroutineType(types: !1705)
!1705 = !{!108, !1501, !1692, !88}
!1706 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1707, file: !1618, line: 158)
!1707 = !DISubprogram(name: "wcstoll", linkageName: "__isoc23_wcstoll", scope: !1628, file: !1628, line: 508, type: !1708, flags: DIFlagPrototyped, spFlags: 0)
!1708 = !DISubroutineType(types: !1709)
!1709 = !{!48, !1501, !1692, !88}
!1710 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1711, file: !1618, line: 159)
!1711 = !DISubprogram(name: "wcstoul", linkageName: "__isoc23_wcstoul", scope: !1628, file: !1628, line: 503, type: !1712, flags: DIFlagPrototyped, spFlags: 0)
!1712 = !DISubroutineType(types: !1713)
!1713 = !{!758, !1501, !1692, !88}
!1714 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1715, file: !1618, line: 160)
!1715 = !DISubprogram(name: "wcstoull", linkageName: "__isoc23_wcstoull", scope: !1628, file: !1628, line: 513, type: !1716, flags: DIFlagPrototyped, spFlags: 0)
!1716 = !DISubroutineType(types: !1717)
!1717 = !{!26, !1501, !1692, !88}
!1718 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1719, file: !1618, line: 161)
!1719 = !DISubprogram(name: "wcscpy", scope: !1628, file: !1628, line: 98, type: !1720, flags: DIFlagPrototyped, spFlags: 0)
!1720 = !DISubroutineType(types: !1721)
!1721 = !{!1487, !1486, !1501}
!1722 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1723, file: !1618, line: 162)
!1723 = !DISubprogram(name: "wcsncpy", scope: !1628, file: !1628, line: 103, type: !1724, flags: DIFlagPrototyped, spFlags: 0)
!1724 = !DISubroutineType(types: !1725)
!1725 = !{!1487, !1486, !1501, !756}
!1726 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1727, file: !1618, line: 163)
!1727 = !DISubprogram(name: "wcscat", scope: !1628, file: !1628, line: 121, type: !1720, flags: DIFlagPrototyped, spFlags: 0)
!1728 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1729, file: !1618, line: 164)
!1729 = !DISubprogram(name: "wcsncat", scope: !1628, file: !1628, line: 125, type: !1724, flags: DIFlagPrototyped, spFlags: 0)
!1730 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1731, file: !1618, line: 165)
!1731 = !DISubprogram(name: "wcscmp", scope: !1628, file: !1628, line: 130, type: !1732, flags: DIFlagPrototyped, spFlags: 0)
!1732 = !DISubroutineType(types: !1733)
!1733 = !{!88, !1502, !1502}
!1734 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1735, file: !1618, line: 166)
!1735 = !DISubprogram(name: "wcscoll", scope: !1628, file: !1628, line: 155, type: !1732, flags: DIFlagPrototyped, spFlags: 0)
!1736 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1737, file: !1618, line: 167)
!1737 = !DISubprogram(name: "wcsncmp", scope: !1628, file: !1628, line: 133, type: !1738, flags: DIFlagPrototyped, spFlags: 0)
!1738 = !DISubroutineType(types: !1739)
!1739 = !{!88, !1502, !1502, !756}
!1740 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1741, file: !1618, line: 168)
!1741 = !DISubprogram(name: "wcsxfrm", scope: !1628, file: !1628, line: 159, type: !1742, flags: DIFlagPrototyped, spFlags: 0)
!1742 = !DISubroutineType(types: !1743)
!1743 = !{!756, !1486, !1501, !756}
!1744 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1745, file: !1618, line: 169)
!1745 = !DISubprogram(name: "wcschr", scope: !1628, file: !1628, line: 186, type: !1746, flags: DIFlagPrototyped, spFlags: 0)
!1746 = !DISubroutineType(types: !1747)
!1747 = !{!1502, !1502, !1488}
!1748 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1749, file: !1618, line: 170)
!1749 = !DISubprogram(name: "wcspbrk", scope: !1628, file: !1628, line: 222, type: !1750, flags: DIFlagPrototyped, spFlags: 0)
!1750 = !DISubroutineType(types: !1751)
!1751 = !{!1502, !1502, !1502}
!1752 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1753, file: !1618, line: 171)
!1753 = !DISubprogram(name: "wcsrchr", scope: !1628, file: !1628, line: 196, type: !1746, flags: DIFlagPrototyped, spFlags: 0)
!1754 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1755, file: !1618, line: 172)
!1755 = !DISubprogram(name: "wcsstr", scope: !1628, file: !1628, line: 233, type: !1750, flags: DIFlagPrototyped, spFlags: 0)
!1756 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1757, file: !1618, line: 173)
!1757 = !DISubprogram(name: "wmemchr", scope: !1628, file: !1628, line: 274, type: !1758, flags: DIFlagPrototyped, spFlags: 0)
!1758 = !DISubroutineType(types: !1759)
!1759 = !{!1502, !1502, !1488, !756}
!1760 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1761, file: !1618, line: 174)
!1761 = !DISubprogram(name: "wcscspn", scope: !1628, file: !1628, line: 212, type: !1762, flags: DIFlagPrototyped, spFlags: 0)
!1762 = !DISubroutineType(types: !1763)
!1763 = !{!756, !1502, !1502}
!1764 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1765, file: !1618, line: 175)
!1765 = !DISubprogram(name: "wcslen", scope: !1628, file: !1628, line: 247, type: !1766, flags: DIFlagPrototyped, spFlags: 0)
!1766 = !DISubroutineType(types: !1767)
!1767 = !{!756, !1502}
!1768 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1769, file: !1618, line: 176)
!1769 = !DISubprogram(name: "wcsspn", scope: !1628, file: !1628, line: 216, type: !1762, flags: DIFlagPrototyped, spFlags: 0)
!1770 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1771, file: !1618, line: 177)
!1771 = !DISubprogram(name: "wcstok", scope: !1628, file: !1628, line: 242, type: !1772, flags: DIFlagPrototyped, spFlags: 0)
!1772 = !DISubroutineType(types: !1773)
!1773 = !{!1487, !1486, !1501, !1692}
!1774 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1775, file: !1618, line: 178)
!1775 = !DISubprogram(name: "wmemcmp", scope: !1628, file: !1628, line: 283, type: !1738, flags: DIFlagPrototyped, spFlags: 0)
!1776 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1777, file: !1618, line: 179)
!1777 = !DISubprogram(name: "wmemcpy", scope: !1628, file: !1628, line: 287, type: !1724, flags: DIFlagPrototyped, spFlags: 0)
!1778 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1779, file: !1618, line: 180)
!1779 = !DISubprogram(name: "wmemmove", scope: !1628, file: !1628, line: 292, type: !1780, flags: DIFlagPrototyped, spFlags: 0)
!1780 = !DISubroutineType(types: !1781)
!1781 = !{!1487, !1487, !1502, !756}
!1782 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1783, file: !1618, line: 181)
!1783 = !DISubprogram(name: "wmemset", scope: !1628, file: !1628, line: 296, type: !1784, flags: DIFlagPrototyped, spFlags: 0)
!1784 = !DISubroutineType(types: !1785)
!1785 = !{!1487, !1487, !1488, !756}
!1786 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1787, file: !1618, line: 182)
!1787 = !DISubprogram(name: "wcsftime", scope: !1628, file: !1628, line: 1043, type: !1788, flags: DIFlagPrototyped, spFlags: 0)
!1788 = !DISubroutineType(types: !1789)
!1789 = !{!756, !1486, !756, !1501, !1008}
!1790 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1791, file: !1618, line: 183)
!1791 = !DISubprogram(name: "btowc", scope: !1628, file: !1628, line: 309, type: !1792, flags: DIFlagPrototyped, spFlags: 0)
!1792 = !DISubroutineType(types: !1793)
!1793 = !{!1558, !88}
!1794 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1795, file: !1618, line: 184)
!1795 = !DISubprogram(name: "wctob", scope: !1628, file: !1628, line: 313, type: !1571, flags: DIFlagPrototyped, spFlags: 0)
!1796 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1797, file: !1618, line: 185)
!1797 = !DISubprogram(name: "mbsinit", scope: !1628, file: !1628, line: 317, type: !1798, flags: DIFlagPrototyped, spFlags: 0)
!1798 = !DISubroutineType(types: !1799)
!1799 = !{!88, !1800}
!1800 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1801, size: 64)
!1801 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !1511)
!1802 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1803, file: !1618, line: 186)
!1803 = !DISubprogram(name: "mbrlen", scope: !1628, file: !1628, line: 332, type: !1804, flags: DIFlagPrototyped, spFlags: 0)
!1804 = !DISubroutineType(types: !1805)
!1805 = !{!756, !880, !756, !1806}
!1806 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1807)
!1807 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1511, size: 64)
!1808 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1809, file: !1618, line: 187)
!1809 = !DISubprogram(name: "mbrtowc", scope: !1628, file: !1628, line: 321, type: !1810, flags: DIFlagPrototyped, spFlags: 0)
!1810 = !DISubroutineType(types: !1811)
!1811 = !{!756, !1486, !880, !756, !1806}
!1812 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1813, file: !1618, line: 188)
!1813 = !DISubprogram(name: "wcrtomb", scope: !1628, file: !1628, line: 326, type: !1814, flags: DIFlagPrototyped, spFlags: 0)
!1814 = !DISubroutineType(types: !1815)
!1815 = !{!756, !879, !1488, !1806}
!1816 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1817, file: !1618, line: 189)
!1817 = !DISubprogram(name: "mbsrtowcs", scope: !1628, file: !1628, line: 362, type: !1818, flags: DIFlagPrototyped, spFlags: 0)
!1818 = !DISubroutineType(types: !1819)
!1819 = !{!756, !1486, !1820, !756, !1806}
!1820 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1821)
!1821 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !881, size: 64)
!1822 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1823, file: !1618, line: 190)
!1823 = !DISubprogram(name: "wcsrtombs", scope: !1628, file: !1628, line: 368, type: !1824, flags: DIFlagPrototyped, spFlags: 0)
!1824 = !DISubroutineType(types: !1825)
!1825 = !{!756, !879, !1826, !756, !1806}
!1826 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1827)
!1827 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1502, size: 64)
!1828 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1829, file: !1618, line: 192)
!1829 = !DISubprogram(name: "getwchar", scope: !1628, file: !1628, line: 942, type: !1830, flags: DIFlagPrototyped, spFlags: 0)
!1830 = !DISubroutineType(types: !1831)
!1831 = !{!1558}
!1832 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1833, file: !1618, line: 193)
!1833 = !DISubprogram(name: "vwscanf", linkageName: "__isoc23_vwscanf", scope: !1628, file: !1628, line: 879, type: !1834, flags: DIFlagPrototyped, spFlags: 0)
!1834 = !DISubroutineType(types: !1835)
!1835 = !{!88, !1501, !1644}
!1836 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1837, file: !1618, line: 194)
!1837 = !DISubprogram(name: "wscanf", linkageName: "__isoc23_wscanf", scope: !1628, file: !1628, line: 799, type: !1838, flags: DIFlagPrototyped, spFlags: 0)
!1838 = !DISubroutineType(types: !1839)
!1839 = !{!88, !1501, null}
!1840 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1841, file: !1618, line: 196)
!1841 = !DISubprogram(name: "putwchar", scope: !1628, file: !1628, line: 956, type: !1842, flags: DIFlagPrototyped, spFlags: 0)
!1842 = !DISubroutineType(types: !1843)
!1843 = !{!1558, !1488}
!1844 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1845, file: !1618, line: 197)
!1845 = !DISubprogram(name: "vwprintf", scope: !1628, file: !1628, line: 758, type: !1834, flags: DIFlagPrototyped, spFlags: 0)
!1846 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1847, file: !1618, line: 198)
!1847 = !DISubprogram(name: "wprintf", scope: !1628, file: !1628, line: 739, type: !1838, flags: DIFlagPrototyped, spFlags: 0)
!1848 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1622, file: !1849, line: 120)
!1849 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstdio", directory: "/home/toolchain/development")
!1850 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1851, file: !1849, line: 121)
!1851 = !DIDerivedType(tag: DW_TAG_typedef, name: "fpos_t", file: !1852, line: 85, baseType: !1853)
!1852 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/stdio.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "bf878b5a7be9bd3141cebb72b92597e8")
!1853 = !DIDerivedType(tag: DW_TAG_typedef, name: "__fpos_t", file: !1854, line: 14, baseType: !1855)
!1854 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/__fpos_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "32de8bdaf3551a6c0a9394f9af4389ce")
!1855 = !DICompositeType(tag: DW_TAG_structure_type, name: "_G_fpos_t", file: !1854, line: 10, size: 128, flags: DIFlagFwdDecl, identifier: "_ZTS9_G_fpos_t")
!1856 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1857, file: !1849, line: 123)
!1857 = !DISubprogram(name: "fclose", scope: !1852, file: !1852, line: 184, type: !1858, flags: DIFlagPrototyped, spFlags: 0)
!1858 = !DISubroutineType(types: !1859)
!1859 = !{!88, !1860}
!1860 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1622, size: 64)
!1861 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1862, file: !1849, line: 124)
!1862 = !DISubprogram(name: "fflush", scope: !1852, file: !1852, line: 236, type: !1858, flags: DIFlagPrototyped, spFlags: 0)
!1863 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1864, file: !1849, line: 125)
!1864 = !DISubprogram(name: "setbuf", scope: !1852, file: !1852, line: 334, type: !1865, flags: DIFlagPrototyped, spFlags: 0)
!1865 = !DISubroutineType(types: !1866)
!1866 = !{null, !1867, !879}
!1867 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1860)
!1868 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1869, file: !1849, line: 126)
!1869 = !DISubprogram(name: "setvbuf", scope: !1852, file: !1852, line: 339, type: !1870, flags: DIFlagPrototyped, spFlags: 0)
!1870 = !DISubroutineType(types: !1871)
!1871 = !{!88, !1867, !879, !88, !756}
!1872 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1873, file: !1849, line: 127)
!1873 = !DISubprogram(name: "fprintf", scope: !1852, file: !1852, line: 357, type: !1874, flags: DIFlagPrototyped, spFlags: 0)
!1874 = !DISubroutineType(types: !1875)
!1875 = !{!88, !1867, !880, null}
!1876 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1877, file: !1849, line: 128)
!1877 = !DISubprogram(name: "fscanf", linkageName: "__isoc23_fscanf", scope: !1852, file: !1852, line: 442, type: !1874, flags: DIFlagPrototyped, spFlags: 0)
!1878 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1879, file: !1849, line: 129)
!1879 = !DISubprogram(name: "snprintf", scope: !1852, file: !1852, line: 385, type: !1880, flags: DIFlagPrototyped, spFlags: 0)
!1880 = !DISubroutineType(types: !1881)
!1881 = !{!88, !879, !756, !880, null}
!1882 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1883, file: !1849, line: 130)
!1883 = !DISubprogram(name: "sprintf", scope: !1852, file: !1852, line: 365, type: !1884, flags: DIFlagPrototyped, spFlags: 0)
!1884 = !DISubroutineType(types: !1885)
!1885 = !{!88, !879, !880, null}
!1886 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1887, file: !1849, line: 131)
!1887 = !DISubprogram(name: "sscanf", linkageName: "__isoc23_sscanf", scope: !1852, file: !1852, line: 447, type: !1888, flags: DIFlagPrototyped, spFlags: 0)
!1888 = !DISubroutineType(types: !1889)
!1889 = !{!88, !880, !880, null}
!1890 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1891, file: !1849, line: 132)
!1891 = !DISubprogram(name: "vfprintf", scope: !1852, file: !1852, line: 372, type: !1892, flags: DIFlagPrototyped, spFlags: 0)
!1892 = !DISubroutineType(types: !1893)
!1893 = !{!88, !1867, !880, !1644}
!1894 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1895, file: !1849, line: 133)
!1895 = !DISubprogram(name: "vfscanf", linkageName: "__isoc23_vfscanf", scope: !1852, file: !1852, line: 511, type: !1892, flags: DIFlagPrototyped, spFlags: 0)
!1896 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1897, file: !1849, line: 134)
!1897 = !DISubprogram(name: "vsscanf", linkageName: "__isoc23_vsscanf", scope: !1852, file: !1852, line: 519, type: !1898, flags: DIFlagPrototyped, spFlags: 0)
!1898 = !DISubroutineType(types: !1899)
!1899 = !{!88, !880, !880, !1644}
!1900 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1901, file: !1849, line: 135)
!1901 = !DISubprogram(name: "vsnprintf", scope: !1852, file: !1852, line: 389, type: !1902, flags: DIFlagPrototyped, spFlags: 0)
!1902 = !DISubroutineType(types: !1903)
!1903 = !{!88, !879, !756, !880, !1644}
!1904 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1905, file: !1849, line: 136)
!1905 = !DISubprogram(name: "vsprintf", scope: !1852, file: !1852, line: 380, type: !1906, flags: DIFlagPrototyped, spFlags: 0)
!1906 = !DISubroutineType(types: !1907)
!1907 = !{!88, !879, !880, !1644}
!1908 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1909, file: !1849, line: 137)
!1909 = !DISubprogram(name: "fgetc", scope: !1852, file: !1852, line: 575, type: !1858, flags: DIFlagPrototyped, spFlags: 0)
!1910 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1911, file: !1849, line: 138)
!1911 = !DISubprogram(name: "fgets", scope: !1852, file: !1852, line: 654, type: !1912, flags: DIFlagPrototyped, spFlags: 0)
!1912 = !DISubroutineType(types: !1913)
!1913 = !{!878, !879, !88, !1867}
!1914 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1915, file: !1849, line: 139)
!1915 = !DISubprogram(name: "fputc", scope: !1852, file: !1852, line: 611, type: !1916, flags: DIFlagPrototyped, spFlags: 0)
!1916 = !DISubroutineType(types: !1917)
!1917 = !{!88, !88, !1860}
!1918 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1919, file: !1849, line: 140)
!1919 = !DISubprogram(name: "fputs", scope: !1852, file: !1852, line: 707, type: !1920, flags: DIFlagPrototyped, spFlags: 0)
!1920 = !DISubroutineType(types: !1921)
!1921 = !{!88, !880, !1867}
!1922 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1923, file: !1849, line: 141)
!1923 = !DISubprogram(name: "getc", scope: !1852, file: !1852, line: 576, type: !1858, flags: DIFlagPrototyped, spFlags: 0)
!1924 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1925, file: !1849, line: 142)
!1925 = !DISubprogram(name: "putc", scope: !1852, file: !1852, line: 612, type: !1916, flags: DIFlagPrototyped, spFlags: 0)
!1926 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1927, file: !1849, line: 143)
!1927 = !DISubprogram(name: "ungetc", scope: !1852, file: !1852, line: 721, type: !1916, flags: DIFlagPrototyped, spFlags: 0)
!1928 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1929, file: !1849, line: 144)
!1929 = !DISubprogram(name: "fread", scope: !1852, file: !1852, line: 728, type: !1930, flags: DIFlagPrototyped, spFlags: 0)
!1930 = !DISubroutineType(types: !1931)
!1931 = !{!756, !865, !756, !756, !1867}
!1932 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1933, file: !1849, line: 145)
!1933 = !DISubprogram(name: "fwrite", scope: !1852, file: !1852, line: 735, type: !1934, flags: DIFlagPrototyped, spFlags: 0)
!1934 = !DISubroutineType(types: !1935)
!1935 = !{!756, !866, !756, !756, !1867}
!1936 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1937, file: !1849, line: 146)
!1937 = !DISubprogram(name: "fgetpos", scope: !1852, file: !1852, line: 819, type: !1938, flags: DIFlagPrototyped, spFlags: 0)
!1938 = !DISubroutineType(types: !1939)
!1939 = !{!88, !1867, !1940}
!1940 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1941)
!1941 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1851, size: 64)
!1942 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1943, file: !1849, line: 147)
!1943 = !DISubprogram(name: "fseek", scope: !1852, file: !1852, line: 769, type: !1944, flags: DIFlagPrototyped, spFlags: 0)
!1944 = !DISubroutineType(types: !1945)
!1945 = !{!88, !1860, !108, !88}
!1946 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1947, file: !1849, line: 148)
!1947 = !DISubprogram(name: "fsetpos", scope: !1852, file: !1852, line: 825, type: !1948, flags: DIFlagPrototyped, spFlags: 0)
!1948 = !DISubroutineType(types: !1949)
!1949 = !{!88, !1860, !1950}
!1950 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1951, size: 64)
!1951 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !1851)
!1952 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1953, file: !1849, line: 149)
!1953 = !DISubprogram(name: "ftell", scope: !1852, file: !1852, line: 775, type: !1954, flags: DIFlagPrototyped, spFlags: 0)
!1954 = !DISubroutineType(types: !1955)
!1955 = !{!108, !1860}
!1956 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1957, file: !1849, line: 150)
!1957 = !DISubprogram(name: "rewind", scope: !1852, file: !1852, line: 780, type: !1958, flags: DIFlagPrototyped, spFlags: 0)
!1958 = !DISubroutineType(types: !1959)
!1959 = !{null, !1860}
!1960 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1961, file: !1849, line: 151)
!1961 = !DISubprogram(name: "clearerr", scope: !1852, file: !1852, line: 850, type: !1958, flags: DIFlagPrototyped, spFlags: 0)
!1962 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1963, file: !1849, line: 152)
!1963 = !DISubprogram(name: "feof", scope: !1852, file: !1852, line: 852, type: !1858, flags: DIFlagPrototyped, spFlags: 0)
!1964 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1965, file: !1849, line: 153)
!1965 = !DISubprogram(name: "ferror", scope: !1852, file: !1852, line: 854, type: !1858, flags: DIFlagPrototyped, spFlags: 0)
!1966 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1967, file: !1849, line: 154)
!1967 = !DISubprogram(name: "perror", scope: !1852, file: !1852, line: 868, type: !1968, flags: DIFlagPrototyped, spFlags: 0)
!1968 = !DISubroutineType(types: !1969)
!1969 = !{null, !881}
!1970 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1971, file: !1849, line: 156)
!1971 = !DISubprogram(name: "fopen", scope: !1852, file: !1852, line: 264, type: !1972, flags: DIFlagPrototyped, spFlags: 0)
!1972 = !DISubroutineType(types: !1973)
!1973 = !{!1860, !880, !880}
!1974 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1975, file: !1849, line: 157)
!1975 = !DISubprogram(name: "freopen", scope: !1852, file: !1852, line: 271, type: !1976, flags: DIFlagPrototyped, spFlags: 0)
!1976 = !DISubroutineType(types: !1977)
!1977 = !{!1860, !880, !880, !1867}
!1978 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1979, file: !1849, line: 158)
!1979 = !DISubprogram(name: "remove", scope: !1852, file: !1852, line: 158, type: !1358, flags: DIFlagPrototyped, spFlags: 0)
!1980 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1981, file: !1849, line: 159)
!1981 = !DISubprogram(name: "rename", scope: !1852, file: !1852, line: 160, type: !896, flags: DIFlagPrototyped, spFlags: 0)
!1982 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1983, file: !1849, line: 160)
!1983 = !DISubprogram(name: "tmpfile", scope: !1852, file: !1852, line: 194, type: !1984, flags: DIFlagPrototyped, spFlags: 0)
!1984 = !DISubroutineType(types: !1985)
!1985 = !{!1860}
!1986 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1987, file: !1849, line: 161)
!1987 = !DISubprogram(name: "tmpnam", scope: !1852, file: !1852, line: 211, type: !1988, flags: DIFlagPrototyped, spFlags: 0)
!1988 = !DISubroutineType(types: !1989)
!1989 = !{!878, !878}
!1990 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1991, file: !1849, line: 163)
!1991 = !DISubprogram(name: "getchar", scope: !1852, file: !1852, line: 582, type: !1400, flags: DIFlagPrototyped, spFlags: 0)
!1992 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1993, file: !1849, line: 167)
!1993 = !DISubprogram(name: "scanf", linkageName: "__isoc23_scanf", scope: !1852, file: !1852, line: 445, type: !1994, flags: DIFlagPrototyped, spFlags: 0)
!1994 = !DISubroutineType(types: !1995)
!1995 = !{!88, !880, null}
!1996 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1997, file: !1849, line: 168)
!1997 = !DISubprogram(name: "vscanf", linkageName: "__isoc23_vscanf", scope: !1852, file: !1852, line: 516, type: !1998, flags: DIFlagPrototyped, spFlags: 0)
!1998 = !DISubroutineType(types: !1999)
!1999 = !{!88, !880, !1644}
!2000 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2001, file: !1849, line: 170)
!2001 = !DISubprogram(name: "printf", scope: !1852, file: !1852, line: 363, type: !1994, flags: DIFlagPrototyped, spFlags: 0)
!2002 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2003, file: !1849, line: 171)
!2003 = !DISubprogram(name: "putchar", scope: !1852, file: !1852, line: 618, type: !1457, flags: DIFlagPrototyped, spFlags: 0)
!2004 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2005, file: !1849, line: 172)
!2005 = !DISubprogram(name: "puts", scope: !1852, file: !1852, line: 714, type: !1358, flags: DIFlagPrototyped, spFlags: 0)
!2006 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2007, file: !1849, line: 173)
!2007 = !DISubprogram(name: "vprintf", scope: !1852, file: !1852, line: 378, type: !1998, flags: DIFlagPrototyped, spFlags: 0)
!2008 = !{i32 7, !"Dwarf Version", i32 5}
!2009 = !{i32 2, !"Debug Info Version", i32 3}
!2010 = !{i32 1, !"wchar_size", i32 4}
!2011 = !{i32 8, !"PIC Level", i32 2}
!2012 = !{i32 7, !"uwtable", i32 2}
!2013 = !{i32 7, !"frame-pointer", i32 2}
!2014 = !{!"clang version 22.1.8"}
!2015 = distinct !DISubprogram(name: "__cxx_global_var_init", scope: !3, file: !3, type: !1424, flags: DIFlagArtificial, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !742)
!2016 = !DILocation(line: 0, scope: !2015)
!2017 = distinct !DISubprogram(name: "pthread_mutex_unlock", scope: !3, file: !3, line: 77, type: !344, scopeLine: 77, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, retainedNodes: !246)
!2018 = !DILocalVariable(name: "mutex", arg: 1, scope: !2017, file: !3, line: 77, type: !346)
!2019 = !DILocation(line: 77, column: 54, scope: !2017)
!2020 = !DILocation(line: 78, column: 7, scope: !2021)
!2021 = distinct !DILexicalBlock(scope: !2017, file: !3, line: 78, column: 7)
!2022 = !DILocation(line: 78, column: 19, scope: !2021)
!2023 = !DILocation(line: 79, column: 46, scope: !2021)
!2024 = !DILocation(line: 79, column: 17, scope: !2021)
!2025 = !DILocation(line: 79, column: 5, scope: !2021)
!2026 = !DILocation(line: 80, column: 20, scope: !2027)
!2027 = distinct !DILexicalBlock(scope: !2017, file: !3, line: 80, column: 7)
!2028 = !DILocation(line: 80, column: 52, scope: !2027)
!2029 = !DILocation(line: 80, column: 55, scope: !2027)
!2030 = !DILocation(line: 80, column: 64, scope: !2027)
!2031 = !DILocation(line: 80, column: 61, scope: !2027)
!2032 = !DILocation(line: 81, column: 25, scope: !2027)
!2033 = !DILocation(line: 81, column: 5, scope: !2027)
!2034 = !DILocation(line: 82, column: 7, scope: !2035)
!2035 = distinct !DILexicalBlock(scope: !2017, file: !3, line: 82, column: 7)
!2036 = !DILocation(line: 82, column: 19, scope: !2035)
!2037 = !DILocation(line: 83, column: 5, scope: !2035)
!2038 = !DILocation(line: 84, column: 10, scope: !2017)
!2039 = !DILocation(line: 84, column: 22, scope: !2017)
!2040 = !DILocation(line: 84, column: 3, scope: !2017)
!2041 = distinct !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqe220108ENS_12memory_orderE", scope: !400, file: !397, line: 70, type: !463, scopeLine: 71, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, declaration: !462, retainedNodes: !246)
!2042 = !DILocalVariable(name: "this", arg: 1, scope: !2041, type: !2043, flags: DIFlagArtificial | DIFlagObjectPointer)
!2043 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !441, size: 64)
!2044 = !DILocation(line: 0, scope: !2041)
!2045 = !DILocalVariable(name: "__m", arg: 2, scope: !2041, file: !397, line: 70, type: !446)
!2046 = !DILocation(line: 70, column: 65, scope: !2041)
!2047 = !DILocation(line: 72, column: 50, scope: !2041)
!2048 = !DILocation(line: 72, column: 57, scope: !2041)
!2049 = !DILocation(line: 72, column: 12, scope: !2041)
!2050 = !DILocation(line: 72, column: 5, scope: !2041)
!2051 = distinct !DISubprogram(name: "fetch_add", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqe220108EiNS_12memory_orderE", scope: !543, file: !397, line: 156, type: !666, scopeLine: 156, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, declaration: !665, retainedNodes: !246)
!2052 = !DILocalVariable(name: "this", arg: 1, scope: !2051, type: !2053, flags: DIFlagArtificial | DIFlagObjectPointer)
!2053 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !543, size: 64)
!2054 = !DILocation(line: 0, scope: !2051)
!2055 = !DILocalVariable(name: "__op", arg: 2, scope: !2051, file: !397, line: 156, type: !88)
!2056 = !DILocation(line: 156, column: 43, scope: !2051)
!2057 = !DILocalVariable(name: "__m", arg: 3, scope: !2051, file: !397, line: 156, type: !446)
!2058 = !DILocation(line: 156, column: 62, scope: !2051)
!2059 = !DILocation(line: 157, column: 61, scope: !2051)
!2060 = !DILocation(line: 157, column: 68, scope: !2051)
!2061 = !DILocation(line: 157, column: 74, scope: !2051)
!2062 = !DILocation(line: 157, column: 12, scope: !2051)
!2063 = !DILocation(line: 157, column: 5, scope: !2051)
!2064 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 87, type: !1400, scopeLine: 87, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, retainedNodes: !246)
!2065 = !DILocation(line: 88, column: 16, scope: !2064)
!2066 = !DILocation(line: 88, column: 3, scope: !2064)
!2067 = !DILocation(line: 89, column: 3, scope: !2064)
!2068 = !DILocation(line: 90, column: 44, scope: !2064)
!2069 = !DILocation(line: 90, column: 15, scope: !2064)
!2070 = !DILocation(line: 91, column: 7, scope: !2071)
!2071 = distinct !DILexicalBlock(scope: !2064, file: !3, line: 91, column: 7)
!2072 = !DILocation(line: 91, column: 19, scope: !2071)
!2073 = !DILocation(line: 92, column: 5, scope: !2074)
!2074 = distinct !DILexicalBlock(scope: !2071, file: !3, line: 91, column: 31)
!2075 = !DILocation(line: 93, column: 5, scope: !2074)
!2076 = !DILocation(line: 95, column: 32, scope: !2064)
!2077 = !DILocation(line: 95, column: 17, scope: !2064)
!2078 = !DILocalVariable(name: "thread", scope: !2064, file: !3, line: 97, type: !2079)
!2079 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !13, line: 27, baseType: !758)
!2080 = !DILocation(line: 97, column: 13, scope: !2064)
!2081 = !DILocalVariable(name: "create_rc", scope: !2064, file: !3, line: 98, type: !88)
!2082 = !DILocation(line: 98, column: 7, scope: !2064)
!2083 = !DILocation(line: 98, column: 19, scope: !2064)
!2084 = !DILocation(line: 99, column: 39, scope: !2064)
!2085 = !DILocation(line: 99, column: 3, scope: !2064)
!2086 = !DILocation(line: 100, column: 7, scope: !2087)
!2087 = distinct !DILexicalBlock(scope: !2064, file: !3, line: 100, column: 7)
!2088 = !DILocation(line: 100, column: 17, scope: !2087)
!2089 = !DILocation(line: 101, column: 5, scope: !2087)
!2090 = !DILocation(line: 102, column: 3, scope: !2064)
!2091 = !DILocation(line: 102, column: 17, scope: !2064)
!2092 = !DILocation(line: 102, column: 10, scope: !2064)
!2093 = !DILocation(line: 103, column: 5, scope: !2064)
!2094 = distinct !{!2094, !2090, !2095, !2096}
!2095 = !DILocation(line: 103, column: 16, scope: !2064)
!2096 = !{!"llvm.loop.mustprogress"}
!2097 = !DILocation(line: 105, column: 3, scope: !2064)
!2098 = !DILocation(line: 107, column: 16, scope: !2064)
!2099 = !DILocation(line: 108, column: 16, scope: !2064)
!2100 = !DILocation(line: 109, column: 3, scope: !2064)
!2101 = !DILocation(line: 110, column: 16, scope: !2064)
!2102 = !DILocalVariable(name: "cancel_rc", scope: !2064, file: !3, line: 111, type: !88)
!2103 = !DILocation(line: 111, column: 7, scope: !2064)
!2104 = !DILocation(line: 111, column: 34, scope: !2064)
!2105 = !DILocation(line: 111, column: 19, scope: !2064)
!2106 = !DILocalVariable(name: "result", scope: !2064, file: !3, line: 112, type: !754)
!2107 = !DILocation(line: 112, column: 9, scope: !2064)
!2108 = !DILocalVariable(name: "join_rc", scope: !2064, file: !3, line: 113, type: !88)
!2109 = !DILocation(line: 113, column: 7, scope: !2064)
!2110 = !DILocation(line: 113, column: 30, scope: !2064)
!2111 = !DILocation(line: 113, column: 17, scope: !2064)
!2112 = !DILocation(line: 114, column: 16, scope: !2064)
!2113 = !DILocalVariable(name: "unlocks", scope: !2064, file: !3, line: 115, type: !88)
!2114 = !DILocation(line: 115, column: 7, scope: !2064)
!2115 = !DILocation(line: 115, column: 37, scope: !2064)
!2116 = !DILocalVariable(name: "cleanups", scope: !2064, file: !3, line: 116, type: !88)
!2117 = !DILocation(line: 116, column: 7, scope: !2064)
!2118 = !DILocation(line: 116, column: 38, scope: !2064)
!2119 = !DILocation(line: 118, column: 15, scope: !2064)
!2120 = !DILocation(line: 119, column: 15, scope: !2064)
!2121 = !DILocation(line: 120, column: 15, scope: !2064)
!2122 = !DILocation(line: 120, column: 22, scope: !2064)
!2123 = !DILocation(line: 121, column: 15, scope: !2064)
!2124 = !DILocation(line: 122, column: 15, scope: !2064)
!2125 = !DILocation(line: 117, column: 3, scope: !2064)
!2126 = !DILocalVariable(name: "try_rc", scope: !2064, file: !3, line: 124, type: !88)
!2127 = !DILocation(line: 124, column: 7, scope: !2064)
!2128 = !DILocation(line: 124, column: 38, scope: !2064)
!2129 = !DILocation(line: 124, column: 16, scope: !2064)
!2130 = !DILocation(line: 125, column: 54, scope: !2064)
!2131 = !DILocation(line: 125, column: 3, scope: !2064)
!2132 = !DILocation(line: 126, column: 7, scope: !2133)
!2133 = distinct !DILexicalBlock(scope: !2064, file: !3, line: 126, column: 7)
!2134 = !DILocation(line: 126, column: 14, scope: !2133)
!2135 = !DILocation(line: 127, column: 5, scope: !2133)
!2136 = !DILocation(line: 127, column: 17, scope: !2133)
!2137 = !DILocalVariable(name: "contender_thread", scope: !2064, file: !3, line: 129, type: !2079)
!2138 = !DILocation(line: 129, column: 13, scope: !2064)
!2139 = !DILocalVariable(name: "contender_create_rc", scope: !2064, file: !3, line: 130, type: !88)
!2140 = !DILocation(line: 130, column: 7, scope: !2064)
!2141 = !DILocation(line: 130, column: 29, scope: !2064)
!2142 = !DILocalVariable(name: "contender_result", scope: !2064, file: !3, line: 131, type: !754)
!2143 = !DILocation(line: 131, column: 9, scope: !2064)
!2144 = !DILocalVariable(name: "contender_join_rc", scope: !2064, file: !3, line: 132, type: !88)
!2145 = !DILocation(line: 132, column: 7, scope: !2064)
!2146 = !DILocation(line: 132, column: 27, scope: !2064)
!2147 = !DILocation(line: 132, column: 47, scope: !2064)
!2148 = !DILocation(line: 132, column: 67, scope: !2064)
!2149 = !DILocation(line: 132, column: 54, scope: !2064)
!2150 = !DILocalVariable(name: "contender_rc", scope: !2064, file: !3, line: 133, type: !88)
!2151 = !DILocation(line: 133, column: 7, scope: !2064)
!2152 = !DILocation(line: 133, column: 66, scope: !2064)
!2153 = !DILocation(line: 133, column: 39, scope: !2064)
!2154 = !DILocation(line: 135, column: 15, scope: !2064)
!2155 = !DILocation(line: 136, column: 15, scope: !2064)
!2156 = !DILocation(line: 137, column: 15, scope: !2064)
!2157 = !DILocation(line: 134, column: 3, scope: !2064)
!2158 = !DILocalVariable(name: "joined_canceled", scope: !2064, file: !3, line: 139, type: !127)
!2159 = !DILocation(line: 139, column: 8, scope: !2064)
!2160 = !DILocation(line: 139, column: 26, scope: !2064)
!2161 = !DILocation(line: 139, column: 36, scope: !2064)
!2162 = !DILocation(line: 139, column: 41, scope: !2064)
!2163 = !DILocation(line: 139, column: 44, scope: !2064)
!2164 = !DILocation(line: 139, column: 52, scope: !2064)
!2165 = !DILocation(line: 139, column: 57, scope: !2064)
!2166 = !DILocation(line: 139, column: 60, scope: !2064)
!2167 = !DILocation(line: 139, column: 67, scope: !2064)
!2168 = !DILocation(line: 0, scope: !2064)
!2169 = !DILocalVariable(name: "clean", scope: !2064, file: !3, line: 145, type: !127)
!2170 = !DILocation(line: 145, column: 8, scope: !2064)
!2171 = !DILocation(line: 145, column: 16, scope: !2064)
!2172 = !DILocation(line: 145, column: 25, scope: !2064)
!2173 = !DILocation(line: 145, column: 30, scope: !2064)
!2174 = !DILocation(line: 145, column: 33, scope: !2064)
!2175 = !DILocation(line: 145, column: 41, scope: !2064)
!2176 = !DILocation(line: 145, column: 46, scope: !2064)
!2177 = !DILocation(line: 145, column: 49, scope: !2064)
!2178 = !DILocation(line: 145, column: 56, scope: !2064)
!2179 = !DILocation(line: 145, column: 61, scope: !2064)
!2180 = !DILocation(line: 145, column: 64, scope: !2064)
!2181 = !DILocation(line: 145, column: 77, scope: !2064)
!2182 = !DILocalVariable(name: "retained_lock", scope: !2064, file: !3, line: 146, type: !127)
!2183 = !DILocation(line: 146, column: 8, scope: !2064)
!2184 = !DILocation(line: 146, column: 24, scope: !2064)
!2185 = !DILocation(line: 146, column: 33, scope: !2064)
!2186 = !DILocation(line: 146, column: 38, scope: !2064)
!2187 = !DILocation(line: 146, column: 41, scope: !2064)
!2188 = !DILocation(line: 146, column: 49, scope: !2064)
!2189 = !DILocation(line: 146, column: 54, scope: !2064)
!2190 = !DILocation(line: 146, column: 57, scope: !2064)
!2191 = !DILocation(line: 146, column: 64, scope: !2064)
!2192 = !DILocation(line: 146, column: 73, scope: !2064)
!2193 = !DILocation(line: 146, column: 76, scope: !2064)
!2194 = !DILocation(line: 146, column: 89, scope: !2064)
!2195 = !DILocalVariable(name: "expected", scope: !2064, file: !3, line: 147, type: !127)
!2196 = !DILocation(line: 147, column: 8, scope: !2064)
!2197 = !DILocation(line: 147, column: 19, scope: !2064)
!2198 = !DILocation(line: 147, column: 35, scope: !2064)
!2199 = !DILocation(line: 147, column: 39, scope: !2064)
!2200 = !DILocation(line: 147, column: 45, scope: !2064)
!2201 = !DILocation(line: 147, column: 48, scope: !2064)
!2202 = !DILocation(line: 149, column: 45, scope: !2064)
!2203 = !DILocation(line: 149, column: 3, scope: !2064)
!2204 = !DILocation(line: 150, column: 10, scope: !2064)
!2205 = !DILocation(line: 150, column: 3, scope: !2064)
!2206 = !DILocation(line: 151, column: 1, scope: !2064)
!2207 = distinct !DISubprogram(name: "terminate_marker", linkageName: "_ZN12_GLOBAL__N_116terminate_markerEv", scope: !2, file: !3, line: 32, type: !1424, scopeLine: 32, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !742, retainedNodes: !246)
!2208 = !DILocalVariable(name: "buffer", scope: !2207, file: !3, line: 33, type: !2209)
!2209 = !DICompositeType(tag: DW_TAG_array_type, baseType: !44, size: 1024, elements: !2210)
!2210 = !{!2211}
!2211 = !DISubrange(count: 128)
!2212 = !DILocation(line: 33, column: 8, scope: !2207)
!2213 = !DILocalVariable(name: "length", scope: !2207, file: !3, line: 34, type: !88)
!2214 = !DILocation(line: 34, column: 7, scope: !2207)
!2215 = !DILocation(line: 34, column: 30, scope: !2207)
!2216 = !DILocation(line: 37, column: 50, scope: !2207)
!2217 = !DILocation(line: 38, column: 50, scope: !2207)
!2218 = !DILocation(line: 34, column: 16, scope: !2207)
!2219 = !DILocation(line: 39, column: 7, scope: !2220)
!2220 = distinct !DILexicalBlock(scope: !2207, file: !3, line: 39, column: 7)
!2221 = !DILocation(line: 39, column: 14, scope: !2220)
!2222 = !DILocation(line: 40, column: 33, scope: !2220)
!2223 = !DILocation(line: 40, column: 61, scope: !2220)
!2224 = !DILocation(line: 40, column: 12, scope: !2220)
!2225 = !DILocation(line: 40, column: 11, scope: !2220)
!2226 = !DILocation(line: 40, column: 5, scope: !2220)
!2227 = !DILocation(line: 41, column: 3, scope: !2207)
!2228 = distinct !DISubprogram(name: "native_handle", linkageName: "_ZNSt3__15mutex13native_handleB9nqe220108Ev", scope: !75, file: !76, line: 44, type: !130, scopeLine: 44, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, declaration: !129, retainedNodes: !246)
!2229 = !DILocalVariable(name: "this", arg: 1, scope: !2228, type: !2230, flags: DIFlagArtificial | DIFlagObjectPointer)
!2230 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !75, size: 64)
!2231 = !DILocation(line: 0, scope: !2228)
!2232 = !DILocation(line: 44, column: 88, scope: !2228)
!2233 = !DILocation(line: 44, column: 80, scope: !2228)
!2234 = distinct !DISubprogram(name: "worker", linkageName: "_ZN12_GLOBAL__N_16workerEPv", scope: !2, file: !3, line: 44, type: !2235, scopeLine: 44, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !742, retainedNodes: !246)
!2235 = !DISubroutineType(types: !2236)
!2236 = !{!754, !754}
!2237 = !DILocalVariable(arg: 1, scope: !2234, file: !3, line: 44, type: !754)
!2238 = !DILocation(line: 44, column: 19, scope: !2234)
!2239 = !DILocation(line: 45, column: 9, scope: !2234)
!2240 = !DILocation(line: 46, column: 9, scope: !2234)
!2241 = !DILocalVariable(name: "lock", scope: !2234, file: !3, line: 47, type: !69)
!2242 = !DILocation(line: 47, column: 32, scope: !2234)
!2243 = !DILocalVariable(name: "cleanup", scope: !2234, file: !3, line: 48, type: !2244)
!2244 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "FrameCleanup", scope: !2, file: !3, line: 24, size: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !2245)
!2245 = !{!2246}
!2246 = !DISubprogram(name: "~FrameCleanup", linkageName: "_ZN12_GLOBAL__N_112FrameCleanupD4Ev", scope: !2244, file: !3, line: 25, type: !2247, scopeLine: 25, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit)
!2247 = !DISubroutineType(types: !2248)
!2248 = !{null, !2249}
!2249 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !2244, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!2250 = !DILocation(line: 48, column: 16, scope: !2234)
!2251 = !DILocation(line: 49, column: 9, scope: !2234)
!2252 = !DILocation(line: 53, column: 13, scope: !2234)
!2253 = !DILocation(line: 58, column: 1, scope: !2234)
!2254 = distinct !DISubprogram(name: "store", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqe220108EbNS_12memory_orderE", scope: !400, file: !397, line: 62, type: !456, scopeLine: 63, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, declaration: !455, retainedNodes: !246)
!2255 = !DILocalVariable(name: "this", arg: 1, scope: !2254, type: !2256, flags: DIFlagArtificial | DIFlagObjectPointer)
!2256 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !400, size: 64)
!2257 = !DILocation(line: 0, scope: !2254)
!2258 = !DILocalVariable(name: "__d", arg: 2, scope: !2254, file: !397, line: 62, type: !127)
!2259 = !DILocation(line: 62, column: 40, scope: !2254)
!2260 = !DILocalVariable(name: "__m", arg: 3, scope: !2254, file: !397, line: 62, type: !446)
!2261 = !DILocation(line: 62, column: 58, scope: !2254)
!2262 = !DILocation(line: 64, column: 44, scope: !2254)
!2263 = !DILocation(line: 64, column: 51, scope: !2254)
!2264 = !DILocation(line: 64, column: 56, scope: !2254)
!2265 = !DILocation(line: 64, column: 5, scope: !2254)
!2266 = !DILocation(line: 65, column: 3, scope: !2254)
!2267 = distinct !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqe220108ENS_12memory_orderE", scope: !546, file: !397, line: 70, type: !598, scopeLine: 71, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, declaration: !597, retainedNodes: !246)
!2268 = !DILocalVariable(name: "this", arg: 1, scope: !2267, type: !2269, flags: DIFlagArtificial | DIFlagObjectPointer)
!2269 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !585, size: 64)
!2270 = !DILocation(line: 0, scope: !2267)
!2271 = !DILocalVariable(name: "__m", arg: 2, scope: !2267, file: !397, line: 70, type: !446)
!2272 = !DILocation(line: 70, column: 65, scope: !2267)
!2273 = !DILocation(line: 72, column: 50, scope: !2267)
!2274 = !DILocation(line: 72, column: 57, scope: !2267)
!2275 = !DILocation(line: 72, column: 12, scope: !2267)
!2276 = !DILocation(line: 72, column: 5, scope: !2267)
!2277 = distinct !DISubprogram(name: "contender", linkageName: "_ZN12_GLOBAL__N_19contenderEPv", scope: !2, file: !3, line: 60, type: !2235, scopeLine: 60, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !742, retainedNodes: !246)
!2278 = !DILocalVariable(arg: 1, scope: !2277, file: !3, line: 60, type: !754)
!2279 = !DILocation(line: 60, column: 22, scope: !2277)
!2280 = !DILocalVariable(name: "deadline", scope: !2277, file: !3, line: 61, type: !960)
!2281 = !DILocation(line: 61, column: 12, scope: !2277)
!2282 = !DILocation(line: 62, column: 3, scope: !2277)
!2283 = !DILocation(line: 63, column: 12, scope: !2277)
!2284 = !DILocation(line: 63, column: 20, scope: !2277)
!2285 = !DILocation(line: 64, column: 16, scope: !2286)
!2286 = distinct !DILexicalBlock(scope: !2277, file: !3, line: 64, column: 7)
!2287 = !DILocation(line: 64, column: 24, scope: !2286)
!2288 = !DILocation(line: 65, column: 14, scope: !2289)
!2289 = distinct !DILexicalBlock(scope: !2286, file: !3, line: 64, column: 39)
!2290 = !DILocation(line: 65, column: 21, scope: !2289)
!2291 = !DILocation(line: 66, column: 14, scope: !2289)
!2292 = !DILocation(line: 66, column: 22, scope: !2289)
!2293 = !DILocation(line: 67, column: 3, scope: !2289)
!2294 = !DILocalVariable(name: "rc", scope: !2277, file: !3, line: 68, type: !88)
!2295 = !DILocation(line: 68, column: 7, scope: !2277)
!2296 = !DILocation(line: 68, column: 36, scope: !2277)
!2297 = !DILocation(line: 68, column: 12, scope: !2277)
!2298 = !DILocation(line: 69, column: 73, scope: !2277)
!2299 = !DILocation(line: 69, column: 3, scope: !2277)
!2300 = !DILocation(line: 70, column: 7, scope: !2301)
!2301 = distinct !DILexicalBlock(scope: !2277, file: !3, line: 70, column: 7)
!2302 = !DILocation(line: 70, column: 10, scope: !2301)
!2303 = !DILocation(line: 71, column: 5, scope: !2301)
!2304 = !DILocation(line: 71, column: 17, scope: !2301)
!2305 = !DILocation(line: 72, column: 56, scope: !2277)
!2306 = !DILocation(line: 72, column: 10, scope: !2277)
!2307 = !DILocation(line: 72, column: 3, scope: !2277)
!2308 = distinct !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC2B9nqe220108ERS1_", scope: !69, file: !70, line: 42, type: !140, scopeLine: 43, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, declaration: !139, retainedNodes: !246)
!2309 = !DILocalVariable(name: "this", arg: 1, scope: !2308, type: !2310, flags: DIFlagArtificial | DIFlagObjectPointer)
!2310 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !69, size: 64)
!2311 = !DILocation(line: 0, scope: !2308)
!2312 = !DILocalVariable(name: "__m", arg: 2, scope: !2308, file: !70, line: 42, type: !142)
!2313 = !DILocation(line: 42, column: 76, scope: !2308)
!2314 = !DILocation(line: 43, column: 9, scope: !2308)
!2315 = !DILocation(line: 43, column: 29, scope: !2308)
!2316 = !{i64 8}
!2317 = !DILocation(line: 43, column: 36, scope: !2308)
!2318 = !DILocation(line: 44, column: 5, scope: !2319)
!2319 = distinct !DILexicalBlock(scope: !2308, file: !70, line: 43, column: 50)
!2320 = !DILocation(line: 44, column: 11, scope: !2319)
!2321 = !DILocation(line: 45, column: 3, scope: !2308)
!2322 = distinct !DISubprogram(name: "~FrameCleanup", linkageName: "_ZN12_GLOBAL__N_112FrameCleanupD2Ev", scope: !2244, file: !3, line: 25, type: !2247, scopeLine: 25, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !742, declaration: !2246, retainedNodes: !246)
!2323 = !DILocalVariable(name: "this", arg: 1, scope: !2322, type: !2324, flags: DIFlagArtificial | DIFlagObjectPointer)
!2324 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !2244, size: 64)
!2325 = !DILocation(line: 0, scope: !2322)
!2326 = !DILocation(line: 26, column: 25, scope: !2327)
!2327 = distinct !DILexicalBlock(scope: !2322, file: !3, line: 25, column: 19)
!2328 = !DILocalVariable(name: "marker", scope: !2327, file: !3, line: 27, type: !349)
!2329 = !DILocation(line: 27, column: 20, scope: !2327)
!2330 = !DILocation(line: 28, column: 33, scope: !2327)
!2331 = !DILocation(line: 28, column: 12, scope: !2327)
!2332 = !DILocation(line: 28, column: 11, scope: !2327)
!2333 = !DILocation(line: 29, column: 3, scope: !2322)
!2334 = distinct !DISubprogram(name: "~unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEED2B9nqe220108Ev", scope: !69, file: !70, line: 65, type: !136, scopeLine: 65, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, declaration: !171, retainedNodes: !246)
!2335 = !DILocalVariable(name: "this", arg: 1, scope: !2334, type: !2310, flags: DIFlagArtificial | DIFlagObjectPointer)
!2336 = !DILocation(line: 0, scope: !2334)
!2337 = !DILocation(line: 66, column: 9, scope: !2338)
!2338 = distinct !DILexicalBlock(scope: !2339, file: !70, line: 66, column: 9)
!2339 = distinct !DILexicalBlock(scope: !2334, file: !70, line: 65, column: 40)
!2340 = !DILocation(line: 67, column: 7, scope: !2338)
!2341 = !DILocation(line: 67, column: 13, scope: !2338)
!2342 = !DILocation(line: 68, column: 3, scope: !2334)
!2343 = distinct !DISubprogram(name: "__cxx_atomic_store<bool>", linkageName: "_ZNSt3__118__cxx_atomic_storeB9nqe220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE", scope: !6, file: !408, line: 67, type: !2344, scopeLine: 67, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, templateParams: !419, retainedNodes: !246)
!2344 = !DISubroutineType(types: !2345)
!2345 = !{null, !2346, !127, !446}
!2346 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !407, size: 64)
!2347 = !DILocalVariable(name: "__a", arg: 1, scope: !2343, file: !408, line: 67, type: !2346)
!2348 = !DILocation(line: 67, column: 49, scope: !2343)
!2349 = !DILocalVariable(name: "__val", arg: 2, scope: !2343, file: !408, line: 67, type: !127)
!2350 = !DILocation(line: 67, column: 58, scope: !2343)
!2351 = !DILocalVariable(name: "__order", arg: 3, scope: !2343, file: !408, line: 67, type: !446)
!2352 = !DILocation(line: 67, column: 78, scope: !2343)
!2353 = !DILocation(line: 68, column: 37, scope: !2343)
!2354 = !DILocation(line: 68, column: 42, scope: !2343)
!2355 = !DILocation(line: 68, column: 102, scope: !2343)
!2356 = !DILocation(line: 68, column: 54, scope: !2343)
!2357 = !DILocation(line: 68, column: 3, scope: !2343)
!2358 = !DILocation(line: 69, column: 1, scope: !2343)
!2359 = distinct !DISubprogram(name: "__cxx_atomic_fetch_add<int>", linkageName: "_ZNSt3__122__cxx_atomic_fetch_addB9nqe220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE", scope: !6, file: !408, line: 180, type: !2360, scopeLine: 180, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !742, templateParams: !563, retainedNodes: !246)
!2360 = !DISubroutineType(types: !2361)
!2361 = !{!88, !2362, !88, !446}
!2362 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !552, size: 64)
!2363 = !DILocalVariable(name: "__a", arg: 1, scope: !2359, file: !408, line: 180, type: !2362)
!2364 = !DILocation(line: 180, column: 53, scope: !2359)
!2365 = !DILocalVariable(name: "__delta", arg: 2, scope: !2359, file: !408, line: 180, type: !88)
!2366 = !DILocation(line: 180, column: 62, scope: !2359)
!2367 = !DILocalVariable(name: "__order", arg: 3, scope: !2359, file: !408, line: 180, type: !446)
!2368 = !DILocation(line: 180, column: 84, scope: !2359)
!2369 = !DILocation(line: 182, column: 22, scope: !2359)
!2370 = !DILocation(line: 182, column: 27, scope: !2359)
!2371 = !DILocation(line: 182, column: 89, scope: !2359)
!2372 = !DILocation(line: 182, column: 39, scope: !2359)
!2373 = !DILocation(line: 181, column: 10, scope: !2359)
!2374 = !DILocation(line: 181, column: 3, scope: !2359)
!2375 = !DILocalVariable(name: "__a", arg: 1, scope: !760, file: !408, line: 79, type: !763)
!2376 = !DILocation(line: 79, column: 80, scope: !760)
!2377 = !DILocalVariable(name: "__order", arg: 2, scope: !760, file: !408, line: 79, type: !446)
!2378 = !DILocation(line: 79, column: 98, scope: !760)
!2379 = !DILocation(line: 82, column: 45, scope: !760)
!2380 = !DILocation(line: 82, column: 50, scope: !760)
!2381 = !DILocation(line: 82, column: 104, scope: !760)
!2382 = !DILocation(line: 81, column: 10, scope: !760)
!2383 = !DILocation(line: 81, column: 3, scope: !760)
!2384 = !DILocalVariable(name: "__a", arg: 1, scope: !767, file: !408, line: 79, type: !770)
!2385 = !DILocation(line: 79, column: 80, scope: !767)
!2386 = !DILocalVariable(name: "__order", arg: 2, scope: !767, file: !408, line: 79, type: !446)
!2387 = !DILocation(line: 79, column: 98, scope: !767)
!2388 = !DILocation(line: 82, column: 45, scope: !767)
!2389 = !DILocation(line: 82, column: 50, scope: !767)
!2390 = !DILocation(line: 82, column: 104, scope: !767)
!2391 = !DILocation(line: 81, column: 10, scope: !767)
!2392 = !DILocation(line: 81, column: 3, scope: !767)
!2393 = distinct !DISubprogram(linkageName: "_GLOBAL__sub_I_item1_oldcaller_cancel.cpp", scope: !3, file: !3, type: !2394, flags: DIFlagArtificial, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !742)
!2394 = !DISubroutineType(types: !246)
!2395 = !DILocation(line: 0, scope: !2393)
