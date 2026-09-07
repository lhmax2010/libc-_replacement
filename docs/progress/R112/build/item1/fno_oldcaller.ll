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

$_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqn220108ENS_12memory_orderE = comdat any

$_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqn220108EiNS_12memory_orderE = comdat any

$_ZNSt3__15mutex13native_handleB9nqn220108Ev = comdat any

$_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE = comdat any

$_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE = comdat any

$_ZNSt3__111unique_lockINS_5mutexEEC2B9nqn220108ERS1_ = comdat any

$_ZNSt3__111unique_lockINS_5mutexEED2B9nqn220108Ev = comdat any

$_ZNSt3__118__cxx_atomic_storeB9nqn220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE = comdat any

$_ZNSt3__122__cxx_atomic_fetch_addB9nqn220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE = comdat any

$_ZNSt3__117__cxx_atomic_loadB9nqn220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE = comdat any

$_ZNSt3__117__cxx_atomic_loadB9nqn220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE = comdat any

@_ZN12_GLOBAL__N_19target_cvE = internal global %"class.std::__1::condition_variable" zeroinitializer, align 8, !dbg !0
@__dso_handle = external hidden global i8
@_ZN12_GLOBAL__N_111real_unlockE = internal global ptr null, align 8, !dbg !340
@.str = private unnamed_addr constant [21 x i8] c"pthread_mutex_unlock\00", align 1, !dbg !347
@_ZN12_GLOBAL__N_113target_nativeE = internal global ptr null, align 8, !dbg !353
@stdout = external global ptr, align 8
@.str.1 = private unnamed_addr constant [19 x i8] c"event=dlsym_failed\00", align 1, !dbg !355
@_ZN12_GLOBAL__N_112target_mutexE = internal global %"class.std::__1::mutex" zeroinitializer, align 8, !dbg !360
@.str.2 = private unnamed_addr constant [20 x i8] c"event=create rc=%d\0A\00", align 1, !dbg !362
@.str.3 = private unnamed_addr constant [21 x i8] c"event=wait_confirmed\00", align 1, !dbg !367
@.str.4 = private unnamed_addr constant [84 x i8] c"event=joined cancel_rc=%d join_rc=%d canceled=%d frame_cleanup=%d target_unlock=%d\0A\00", align 1, !dbg !369
@.str.5 = private unnamed_addr constant [35 x i8] c"event=trylock rc=%d errno_busy=%d\0A\00", align 1, !dbg !374
@.str.6 = private unnamed_addr constant [62 x i8] c"event=contender_join create_rc=%d join_rc=%d contender_rc=%d\0A\00", align 1, !dbg !379
@.str.7 = private unnamed_addr constant [26 x i8] c"event=assert expected=%d\0A\00", align 1, !dbg !384
@_ZN12_GLOBAL__N_112track_unlockE = internal global { { i8 } } zeroinitializer, align 1, !dbg !389
@_ZN12_GLOBAL__N_119target_unlock_countE = internal global { { i32 } } zeroinitializer, align 4, !dbg !533
@.str.10 = private unnamed_addr constant [51 x i8] c"event=terminate frame_cleanup=%d target_unlock=%d\0A\00", align 1, !dbg !723
@.str.11 = private unnamed_addr constant [19 x i8] c"event=worker_ready\00", align 1, !dbg !728
@.str.12 = private unnamed_addr constant [24 x i8] c"event=unexpected_return\00", align 1, !dbg !730
@"__const.~FrameCleanup.marker" = private unnamed_addr constant [21 x i8] c"event=frame_cleanup\0A\00", align 16
@_ZN12_GLOBAL__N_15readyE = internal global { { i8 } } zeroinitializer, align 1, !dbg !735
@_ZN12_GLOBAL__N_119frame_cleanup_countE = internal global { { i32 } } zeroinitializer, align 4, !dbg !737
@.str.15 = private unnamed_addr constant [54 x i8] c"event=contender rc=%d errno_busy=%d errno_timeout=%d\0A\00", align 1, !dbg !739
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 65535, ptr @_GLOBAL__sub_I_item1_oldcaller_cancel.cpp, ptr null }]

; Function Attrs: noinline nounwind uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" !dbg !2017 {
  %1 = call i32 @__cxa_atexit(ptr @_ZNSt3__118condition_variableD1Ev, ptr @_ZN12_GLOBAL__N_19target_cvE, ptr @__dso_handle) #2, !dbg !2018
  ret void, !dbg !2018
}

; Function Attrs: nounwind
declare void @_ZNSt3__118condition_variableD1Ev(ptr noundef nonnull align 8 dereferenceable(48)) unnamed_addr #1

; Function Attrs: nounwind
declare i32 @__cxa_atexit(ptr, ptr, ptr) #2

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define i32 @pthread_mutex_unlock(ptr noundef nonnull %0) #3 !dbg !2019 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2020, !DIExpression(), !2021)
  %3 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2022
  %4 = icmp eq ptr %3, null, !dbg !2024
  br i1 %4, label %5, label %7, !dbg !2024

5:                                                ; preds = %1
  %6 = call ptr @dlsym(ptr noundef inttoptr (i64 -1 to ptr), ptr noundef @.str) #2, !dbg !2025
  store ptr %6, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2026
  br label %7, !dbg !2027

7:                                                ; preds = %5, %1
  %8 = call noundef zeroext i1 @_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_112track_unlockE, i32 noundef 0) #2, !dbg !2028
  br i1 %8, label %9, label %15, !dbg !2030

9:                                                ; preds = %7
  %10 = load ptr, ptr %2, align 8, !dbg !2031
  %11 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2032
  %12 = icmp eq ptr %10, %11, !dbg !2033
  br i1 %12, label %13, label %15, !dbg !2030

13:                                               ; preds = %9
  %14 = call noundef i32 @_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqn220108EiNS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119target_unlock_countE, i32 noundef 1, i32 noundef 0) #2, !dbg !2034
  br label %15, !dbg !2035

15:                                               ; preds = %13, %9, %7
  %16 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2036
  %17 = icmp eq ptr %16, null, !dbg !2038
  br i1 %17, label %18, label %19, !dbg !2038

18:                                               ; preds = %15
  call void @_Exit(i32 noundef 87) #9, !dbg !2039
  unreachable, !dbg !2039

19:                                               ; preds = %15
  %20 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2040
  %21 = load ptr, ptr %2, align 8, !dbg !2041
  %22 = call noundef i32 %20(ptr noundef %21), !dbg !2040
  ret i32 %22, !dbg !2042
}

; Function Attrs: nounwind
declare ptr @dlsym(ptr noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef zeroext i1 @_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) %0, i32 noundef %1) #3 comdat align 2 !dbg !2043 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2044, !DIExpression(), !2046)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2047, !DIExpression(), !2048)
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base", ptr %5, i32 0, i32 0, !dbg !2049
  %7 = load i32, ptr %4, align 4, !dbg !2050
  %8 = call noundef zeroext i1 @_ZNSt3__117__cxx_atomic_loadB9nqn220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %6, i32 noundef %7) #2, !dbg !2051
  ret i1 %8, !dbg !2052
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqn220108EiNS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) %0, i32 noundef %1, i32 noundef %2) #3 comdat align 2 !dbg !2053 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2054, !DIExpression(), !2056)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !2057, !DIExpression(), !2058)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2059, !DIExpression(), !2060)
  %7 = load ptr, ptr %4, align 8
  %8 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base.2", ptr %7, i32 0, i32 0, !dbg !2061
  %9 = load i32, ptr %5, align 4, !dbg !2062
  %10 = load i32, ptr %6, align 4, !dbg !2063
  %11 = call noundef i32 @_ZNSt3__122__cxx_atomic_fetch_addB9nqn220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE(ptr noundef %8, i32 noundef %9, i32 noundef %10) #2, !dbg !2064
  ret i32 %11, !dbg !2065
}

; Function Attrs: noreturn nounwind
declare void @_Exit(i32 noundef) #4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #5 !dbg !2066 {
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
  %19 = load ptr, ptr @stdout, align 8, !dbg !2067
  %20 = call i32 @setvbuf(ptr noundef %19, ptr noundef null, i32 noundef 2, i64 noundef 0) #2, !dbg !2068
  %21 = call noundef ptr @_ZSt13set_terminatePFvvE(ptr noundef @_ZN12_GLOBAL__N_116terminate_markerEv) #2, !dbg !2069
  %22 = call ptr @dlsym(ptr noundef inttoptr (i64 -1 to ptr), ptr noundef @.str) #2, !dbg !2070
  store ptr %22, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2071
  %23 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2072
  %24 = icmp eq ptr %23, null, !dbg !2074
  br i1 %24, label %25, label %27, !dbg !2074

25:                                               ; preds = %0
  %26 = call i32 @puts(ptr noundef @.str.1), !dbg !2075
  store i32 68, ptr %1, align 4, !dbg !2077
  br label %146, !dbg !2077

27:                                               ; preds = %0
  %28 = call noundef ptr @_ZNSt3__15mutex13native_handleB9nqn220108Ev(ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE), !dbg !2078
  store ptr %28, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2079
    #dbg_declare(ptr %2, !2080, !DIExpression(), !2082)
    #dbg_declare(ptr %3, !2083, !DIExpression(), !2084)
  %29 = call i32 @pthread_create(ptr noundef %2, ptr noundef null, ptr noundef @_ZN12_GLOBAL__N_16workerEPv, ptr noundef null) #2, !dbg !2085
  store i32 %29, ptr %3, align 4, !dbg !2084
  %30 = load i32, ptr %3, align 4, !dbg !2086
  %31 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %30), !dbg !2087
  %32 = load i32, ptr %3, align 4, !dbg !2088
  %33 = icmp ne i32 %32, 0, !dbg !2090
  br i1 %33, label %34, label %35, !dbg !2090

34:                                               ; preds = %27
  store i32 65, ptr %1, align 4, !dbg !2091
  br label %146, !dbg !2091

35:                                               ; preds = %27
  br label %36, !dbg !2092

36:                                               ; preds = %39, %35
  %37 = call noundef zeroext i1 @_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_15readyE, i32 noundef 2) #2, !dbg !2093
  %38 = xor i1 %37, true, !dbg !2094
  br i1 %38, label %39, label %41, !dbg !2092

39:                                               ; preds = %36
  %40 = call i32 @usleep(i32 noundef 1000), !dbg !2095
  br label %36, !dbg !2092, !llvm.loop !2096

41:                                               ; preds = %36
  call void @_ZNSt3__15mutex4lockEv(ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE), !dbg !2099
  call void @_ZNSt3__15mutex6unlockEv(ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE) #2, !dbg !2100
  %42 = call i32 @puts(ptr noundef @.str.3), !dbg !2101
  call void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_112track_unlockE, i1 noundef zeroext true, i32 noundef 3) #2, !dbg !2102
    #dbg_declare(ptr %4, !2103, !DIExpression(), !2104)
  %43 = load i64, ptr %2, align 8, !dbg !2105
  %44 = call i32 @pthread_cancel(i64 noundef %43), !dbg !2106
  store i32 %44, ptr %4, align 4, !dbg !2104
    #dbg_declare(ptr %5, !2107, !DIExpression(), !2108)
  store ptr null, ptr %5, align 8, !dbg !2108
    #dbg_declare(ptr %6, !2109, !DIExpression(), !2110)
  %45 = load i64, ptr %2, align 8, !dbg !2111
  %46 = call i32 @pthread_join(i64 noundef %45, ptr noundef %5), !dbg !2112
  store i32 %46, ptr %6, align 4, !dbg !2110
  call void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_112track_unlockE, i1 noundef zeroext false, i32 noundef 3) #2, !dbg !2113
    #dbg_declare(ptr %7, !2114, !DIExpression(), !2115)
  %47 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119target_unlock_countE, i32 noundef 0) #2, !dbg !2116
  store i32 %47, ptr %7, align 4, !dbg !2115
    #dbg_declare(ptr %8, !2117, !DIExpression(), !2118)
  %48 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119frame_cleanup_countE, i32 noundef 0) #2, !dbg !2119
  store i32 %48, ptr %8, align 4, !dbg !2118
  %49 = load i32, ptr %4, align 4, !dbg !2120
  %50 = load i32, ptr %6, align 4, !dbg !2121
  %51 = load ptr, ptr %5, align 8, !dbg !2122
  %52 = icmp eq ptr %51, inttoptr (i64 -1 to ptr), !dbg !2123
  %53 = zext i1 %52 to i64, !dbg !2122
  %54 = select i1 %52, i32 1, i32 0, !dbg !2122
  %55 = load i32, ptr %8, align 4, !dbg !2124
  %56 = load i32, ptr %7, align 4, !dbg !2125
  %57 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %49, i32 noundef %50, i32 noundef %54, i32 noundef %55, i32 noundef %56), !dbg !2126
    #dbg_declare(ptr %9, !2127, !DIExpression(), !2128)
  %58 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2129
  %59 = call i32 @pthread_mutex_trylock(ptr noundef %58) #2, !dbg !2130
  store i32 %59, ptr %9, align 4, !dbg !2128
  %60 = load i32, ptr %9, align 4, !dbg !2131
  %61 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %60, i32 noundef 16), !dbg !2132
  %62 = load i32, ptr %9, align 4, !dbg !2133
  %63 = icmp eq i32 %62, 0, !dbg !2135
  br i1 %63, label %64, label %68, !dbg !2135

64:                                               ; preds = %41
  %65 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2136
  %66 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2137
  %67 = call noundef i32 %65(ptr noundef %66), !dbg !2136
  br label %68, !dbg !2136

68:                                               ; preds = %64, %41
    #dbg_declare(ptr %10, !2138, !DIExpression(), !2139)
    #dbg_declare(ptr %11, !2140, !DIExpression(), !2141)
  %69 = call i32 @pthread_create(ptr noundef %10, ptr noundef null, ptr noundef @_ZN12_GLOBAL__N_19contenderEPv, ptr noundef null) #2, !dbg !2142
  store i32 %69, ptr %11, align 4, !dbg !2141
    #dbg_declare(ptr %12, !2143, !DIExpression(), !2144)
  store ptr null, ptr %12, align 8, !dbg !2144
    #dbg_declare(ptr %13, !2145, !DIExpression(), !2146)
  %70 = load i32, ptr %11, align 4, !dbg !2147
  %71 = icmp eq i32 %70, 0, !dbg !2148
  br i1 %71, label %72, label %75, !dbg !2147

72:                                               ; preds = %68
  %73 = load i64, ptr %10, align 8, !dbg !2149
  %74 = call i32 @pthread_join(i64 noundef %73, ptr noundef %12), !dbg !2150
  br label %76, !dbg !2147

75:                                               ; preds = %68
  br label %76, !dbg !2147

76:                                               ; preds = %75, %72
  %77 = phi i32 [ %74, %72 ], [ -1, %75 ], !dbg !2147
  store i32 %77, ptr %13, align 4, !dbg !2146
    #dbg_declare(ptr %14, !2151, !DIExpression(), !2152)
  %78 = load ptr, ptr %12, align 8, !dbg !2153
  %79 = ptrtoint ptr %78 to i64, !dbg !2154
  %80 = trunc i64 %79 to i32, !dbg !2154
  store i32 %80, ptr %14, align 4, !dbg !2152
  %81 = load i32, ptr %11, align 4, !dbg !2155
  %82 = load i32, ptr %13, align 4, !dbg !2156
  %83 = load i32, ptr %14, align 4, !dbg !2157
  %84 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, i32 noundef %81, i32 noundef %82, i32 noundef %83), !dbg !2158
    #dbg_declare(ptr %15, !2159, !DIExpression(), !2160)
  %85 = load i32, ptr %4, align 4, !dbg !2161
  %86 = icmp eq i32 %85, 0, !dbg !2162
  br i1 %86, label %87, label %93, !dbg !2163

87:                                               ; preds = %76
  %88 = load i32, ptr %6, align 4, !dbg !2164
  %89 = icmp eq i32 %88, 0, !dbg !2165
  br i1 %89, label %90, label %93, !dbg !2166

90:                                               ; preds = %87
  %91 = load ptr, ptr %5, align 8, !dbg !2167
  %92 = icmp eq ptr %91, inttoptr (i64 -1 to ptr), !dbg !2168
  br label %93

93:                                               ; preds = %90, %87, %76
  %94 = phi i1 [ false, %87 ], [ false, %76 ], [ %92, %90 ], !dbg !2169
  %95 = zext i1 %94 to i8, !dbg !2160
  store i8 %95, ptr %15, align 1, !dbg !2160
    #dbg_declare(ptr %16, !2170, !DIExpression(), !2171)
  %96 = load i32, ptr %8, align 4, !dbg !2172
  %97 = icmp eq i32 %96, 1, !dbg !2173
  br i1 %97, label %98, label %107, !dbg !2174

98:                                               ; preds = %93
  %99 = load i32, ptr %7, align 4, !dbg !2175
  %100 = icmp eq i32 %99, 1, !dbg !2176
  br i1 %100, label %101, label %107, !dbg !2177

101:                                              ; preds = %98
  %102 = load i32, ptr %9, align 4, !dbg !2178
  %103 = icmp eq i32 %102, 0, !dbg !2179
  br i1 %103, label %104, label %107, !dbg !2180

104:                                              ; preds = %101
  %105 = load i32, ptr %14, align 4, !dbg !2181
  %106 = icmp eq i32 %105, 0, !dbg !2182
  br label %107

107:                                              ; preds = %104, %101, %98, %93
  %108 = phi i1 [ false, %101 ], [ false, %98 ], [ false, %93 ], [ %106, %104 ], !dbg !2169
  %109 = zext i1 %108 to i8, !dbg !2171
  store i8 %109, ptr %16, align 1, !dbg !2171
    #dbg_declare(ptr %17, !2183, !DIExpression(), !2184)
  %110 = load i32, ptr %8, align 4, !dbg !2185
  %111 = icmp eq i32 %110, 0, !dbg !2186
  br i1 %111, label %112, label %121, !dbg !2187

112:                                              ; preds = %107
  %113 = load i32, ptr %7, align 4, !dbg !2188
  %114 = icmp eq i32 %113, 0, !dbg !2189
  br i1 %114, label %115, label %121, !dbg !2190

115:                                              ; preds = %112
  %116 = load i32, ptr %9, align 4, !dbg !2191
  %117 = icmp eq i32 %116, 16, !dbg !2192
  br i1 %117, label %118, label %121, !dbg !2193

118:                                              ; preds = %115
  %119 = load i32, ptr %14, align 4, !dbg !2194
  %120 = icmp eq i32 %119, 110, !dbg !2195
  br label %121

121:                                              ; preds = %118, %115, %112, %107
  %122 = phi i1 [ false, %115 ], [ false, %112 ], [ false, %107 ], [ %120, %118 ], !dbg !2169
  %123 = zext i1 %122 to i8, !dbg !2184
  store i8 %123, ptr %17, align 1, !dbg !2184
    #dbg_declare(ptr %18, !2196, !DIExpression(), !2197)
  %124 = load i8, ptr %15, align 1, !dbg !2198
  %125 = trunc i8 %124 to i1, !dbg !2198
  br i1 %125, label %126, label %134, !dbg !2199

126:                                              ; preds = %121
  %127 = load i8, ptr %16, align 1, !dbg !2200
  %128 = trunc i8 %127 to i1, !dbg !2200
  br i1 %128, label %132, label %129, !dbg !2201

129:                                              ; preds = %126
  %130 = load i8, ptr %17, align 1, !dbg !2202
  %131 = trunc i8 %130 to i1, !dbg !2202
  br label %132, !dbg !2201

132:                                              ; preds = %129, %126
  %133 = phi i1 [ true, %126 ], [ %131, %129 ]
  br label %134

134:                                              ; preds = %132, %121
  %135 = phi i1 [ false, %121 ], [ %133, %132 ], !dbg !2169
  %136 = zext i1 %135 to i8, !dbg !2197
  store i8 %136, ptr %18, align 1, !dbg !2197
  %137 = load i8, ptr %18, align 1, !dbg !2203
  %138 = trunc i8 %137 to i1, !dbg !2203
  %139 = zext i1 %138 to i64, !dbg !2203
  %140 = select i1 %138, i32 1, i32 0, !dbg !2203
  %141 = call i32 (ptr, ...) @printf(ptr noundef @.str.7, i32 noundef %140), !dbg !2204
  %142 = load i8, ptr %18, align 1, !dbg !2205
  %143 = trunc i8 %142 to i1, !dbg !2205
  %144 = zext i1 %143 to i64, !dbg !2205
  %145 = select i1 %143, i32 0, i32 66, !dbg !2205
  store i32 %145, ptr %1, align 4, !dbg !2206
  br label %146, !dbg !2206

146:                                              ; preds = %134, %34, %25
  %147 = load i32, ptr %1, align 4, !dbg !2207
  ret i32 %147, !dbg !2207
}

; Function Attrs: nounwind
declare i32 @setvbuf(ptr noundef, ptr noundef, i32 noundef, i64 noundef) #1

; Function Attrs: nounwind
declare noundef ptr @_ZSt13set_terminatePFvvE(ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define internal void @_ZN12_GLOBAL__N_116terminate_markerEv() #3 !dbg !2208 {
  %1 = alloca [128 x i8], align 16
  %2 = alloca i32, align 4
    #dbg_declare(ptr %1, !2209, !DIExpression(), !2213)
    #dbg_declare(ptr %2, !2214, !DIExpression(), !2215)
  %3 = getelementptr inbounds [128 x i8], ptr %1, i64 0, i64 0, !dbg !2216
  %4 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119frame_cleanup_countE, i32 noundef 0) #2, !dbg !2217
  %5 = call noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119target_unlock_countE, i32 noundef 0) #2, !dbg !2218
  %6 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef %3, i64 noundef 128, ptr noundef @.str.10, i32 noundef %4, i32 noundef %5) #2, !dbg !2219
  store i32 %6, ptr %2, align 4, !dbg !2215
  %7 = load i32, ptr %2, align 4, !dbg !2220
  %8 = icmp sgt i32 %7, 0, !dbg !2222
  br i1 %8, label %9, label %16, !dbg !2222

9:                                                ; preds = %0
  %10 = getelementptr inbounds [128 x i8], ptr %1, i64 0, i64 0, !dbg !2223
  %11 = load i32, ptr %2, align 4, !dbg !2224
  %12 = sext i32 %11 to i64, !dbg !2224
  %13 = call i64 @write(i32 noundef 1, ptr noundef %10, i64 noundef %12), !dbg !2225
  %14 = icmp ne i64 %13, 0, !dbg !2225
  %15 = xor i1 %14, true, !dbg !2226
  br label %16, !dbg !2227

16:                                               ; preds = %9, %0
  call void @_Exit(i32 noundef 86) #9, !dbg !2228
  unreachable, !dbg !2228
}

declare i32 @puts(ptr noundef) #6

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef ptr @_ZNSt3__15mutex13native_handleB9nqn220108Ev(ptr noundef nonnull align 8 dereferenceable(40) %0) #3 comdat align 2 !dbg !2229 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2230, !DIExpression(), !2232)
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %"class.std::__1::mutex", ptr %3, i32 0, i32 0, !dbg !2233
  ret ptr %4, !dbg !2234
}

; Function Attrs: nounwind
declare i32 @pthread_create(ptr noundef, ptr noundef, ptr noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define internal noundef ptr @_ZN12_GLOBAL__N_16workerEPv(ptr noundef %0) #3 !dbg !2235 {
  %2 = alloca ptr, align 8
  %3 = alloca %"class.std::__1::unique_lock", align 8
  %4 = alloca %"struct.(anonymous namespace)::FrameCleanup", align 1
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2238, !DIExpression(), !2239)
  %5 = call i32 @pthread_setcancelstate(i32 noundef 0, ptr noundef null), !dbg !2240
  %6 = call i32 @pthread_setcanceltype(i32 noundef 0, ptr noundef null), !dbg !2241
    #dbg_declare(ptr %3, !2242, !DIExpression(), !2243)
  call void @_ZNSt3__111unique_lockINS_5mutexEEC2B9nqn220108ERS1_(ptr noundef nonnull align 8 dereferenceable(9) %3, ptr noundef nonnull align 8 dereferenceable(40) @_ZN12_GLOBAL__N_112target_mutexE), !dbg !2243
    #dbg_declare(ptr %4, !2244, !DIExpression(), !2251)
  call void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) @_ZN12_GLOBAL__N_15readyE, i1 noundef zeroext true, i32 noundef 3) #2, !dbg !2252
  %7 = call i32 @puts(ptr noundef @.str.11), !dbg !2253
  call void @_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE(ptr noundef nonnull align 8 dereferenceable(48) @_ZN12_GLOBAL__N_19target_cvE, ptr noundef nonnull align 8 dereferenceable(9) %3) #2, !dbg !2254
  %8 = call i32 @puts(ptr noundef @.str.12), !dbg !2255
  call void @_ZN12_GLOBAL__N_112FrameCleanupD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %4) #2, !dbg !2256
  call void @_ZNSt3__111unique_lockINS_5mutexEED2B9nqn220108Ev(ptr noundef nonnull align 8 dereferenceable(9) %3) #2, !dbg !2256
  ret ptr inttoptr (i64 85 to ptr), !dbg !2256
}

declare i32 @printf(ptr noundef, ...) #6

declare i32 @usleep(i32 noundef) #6

declare void @_ZNSt3__15mutex4lockEv(ptr noundef nonnull align 8 dereferenceable(40)) #6

; Function Attrs: nounwind
declare void @_ZNSt3__15mutex6unlockEv(ptr noundef nonnull align 8 dereferenceable(40)) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden void @_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE(ptr noundef nonnull align 1 dereferenceable(1) %0, i1 noundef zeroext %1, i32 noundef %2) #3 comdat align 2 !dbg !2257 {
  %4 = alloca ptr, align 8
  %5 = alloca i8, align 1
  %6 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2258, !DIExpression(), !2260)
  %7 = zext i1 %1 to i8
  store i8 %7, ptr %5, align 1
    #dbg_declare(ptr %5, !2261, !DIExpression(), !2262)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2263, !DIExpression(), !2264)
  %8 = load ptr, ptr %4, align 8
  %9 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base", ptr %8, i32 0, i32 0, !dbg !2265
  %10 = load i8, ptr %5, align 1, !dbg !2266
  %11 = trunc i8 %10 to i1, !dbg !2266
  %12 = load i32, ptr %6, align 4, !dbg !2267
  call void @_ZNSt3__118__cxx_atomic_storeB9nqn220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE(ptr noundef %9, i1 noundef zeroext %11, i32 noundef %12) #2, !dbg !2268
  ret void, !dbg !2269
}

declare i32 @pthread_cancel(i64 noundef) #6

declare i32 @pthread_join(i64 noundef, ptr noundef) #6

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) %0, i32 noundef %1) #3 comdat align 2 !dbg !2270 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2271, !DIExpression(), !2273)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2274, !DIExpression(), !2275)
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %"struct.std::__1::__atomic_base.2", ptr %5, i32 0, i32 0, !dbg !2276
  %7 = load i32, ptr %4, align 4, !dbg !2277
  %8 = call noundef i32 @_ZNSt3__117__cxx_atomic_loadB9nqn220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %6, i32 noundef %7) #2, !dbg !2278
  ret i32 %8, !dbg !2279
}

; Function Attrs: nounwind
declare i32 @pthread_mutex_trylock(ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define internal noundef ptr @_ZN12_GLOBAL__N_19contenderEPv(ptr noundef %0) #3 !dbg !2280 {
  %2 = alloca ptr, align 8
  %3 = alloca %struct.timespec, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2281, !DIExpression(), !2282)
    #dbg_declare(ptr %3, !2283, !DIExpression(), !2284)
  call void @llvm.memset.p0.i64(ptr align 8 %3, i8 0, i64 16, i1 false), !dbg !2284
  %5 = call i32 @clock_gettime(i32 noundef 0, ptr noundef %3) #2, !dbg !2285
  %6 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 1, !dbg !2286
  %7 = load i64, ptr %6, align 8, !dbg !2287
  %8 = add nsw i64 %7, 300000000, !dbg !2287
  store i64 %8, ptr %6, align 8, !dbg !2287
  %9 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 1, !dbg !2288
  %10 = load i64, ptr %9, align 8, !dbg !2288
  %11 = icmp sge i64 %10, 1000000000, !dbg !2290
  br i1 %11, label %12, label %19, !dbg !2290

12:                                               ; preds = %1
  %13 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 0, !dbg !2291
  %14 = load i64, ptr %13, align 8, !dbg !2293
  %15 = add nsw i64 %14, 1, !dbg !2293
  store i64 %15, ptr %13, align 8, !dbg !2293
  %16 = getelementptr inbounds nuw %struct.timespec, ptr %3, i32 0, i32 1, !dbg !2294
  %17 = load i64, ptr %16, align 8, !dbg !2295
  %18 = sub nsw i64 %17, 1000000000, !dbg !2295
  store i64 %18, ptr %16, align 8, !dbg !2295
  br label %19, !dbg !2296

19:                                               ; preds = %12, %1
    #dbg_declare(ptr %4, !2297, !DIExpression(), !2298)
  %20 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2299
  %21 = call i32 @pthread_mutex_timedlock(ptr noundef %20, ptr noundef %3) #2, !dbg !2300
  store i32 %21, ptr %4, align 4, !dbg !2298
  %22 = load i32, ptr %4, align 4, !dbg !2301
  %23 = call i32 (ptr, ...) @printf(ptr noundef @.str.15, i32 noundef %22, i32 noundef 16, i32 noundef 110), !dbg !2302
  %24 = load i32, ptr %4, align 4, !dbg !2303
  %25 = icmp eq i32 %24, 0, !dbg !2305
  br i1 %25, label %26, label %30, !dbg !2305

26:                                               ; preds = %19
  %27 = load ptr, ptr @_ZN12_GLOBAL__N_111real_unlockE, align 8, !dbg !2306
  %28 = load ptr, ptr @_ZN12_GLOBAL__N_113target_nativeE, align 8, !dbg !2307
  %29 = call noundef i32 %27(ptr noundef %28), !dbg !2306
  br label %30, !dbg !2306

30:                                               ; preds = %26, %19
  %31 = load i32, ptr %4, align 4, !dbg !2308
  %32 = sext i32 %31 to i64, !dbg !2308
  %33 = inttoptr i64 %32 to ptr, !dbg !2309
  ret ptr %33, !dbg !2310
}

; Function Attrs: nounwind
declare i32 @snprintf(ptr noundef, i64 noundef, ptr noundef, ...) #1

declare i64 @write(i32 noundef, ptr noundef, i64 noundef) #6

declare i32 @pthread_setcancelstate(i32 noundef, ptr noundef) #6

declare i32 @pthread_setcanceltype(i32 noundef, ptr noundef) #6

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden void @_ZNSt3__111unique_lockINS_5mutexEEC2B9nqn220108ERS1_(ptr noundef nonnull align 8 dereferenceable(9) %0, ptr noundef nonnull align 8 dereferenceable(40) %1) unnamed_addr #3 comdat align 2 !dbg !2311 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2312, !DIExpression(), !2314)
  store ptr %1, ptr %4, align 8
    #dbg_declare(ptr %4, !2315, !DIExpression(), !2316)
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %5, i32 0, i32 0, !dbg !2317
  %7 = load ptr, ptr %4, align 8, !dbg !2318, !nonnull !246, !align !2319
  store ptr %7, ptr %6, align 8, !dbg !2317
  %8 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %5, i32 0, i32 1, !dbg !2320
  store i8 1, ptr %8, align 8, !dbg !2320
  %9 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %5, i32 0, i32 0, !dbg !2321
  %10 = load ptr, ptr %9, align 8, !dbg !2321
  call void @_ZNSt3__15mutex4lockEv(ptr noundef nonnull align 8 dereferenceable(40) %10), !dbg !2323
  ret void, !dbg !2324
}

; Function Attrs: nounwind
declare void @_ZNSt3__118condition_variable4waitERNS_11unique_lockINS_5mutexEEE(ptr noundef nonnull align 8 dereferenceable(48), ptr noundef nonnull align 8 dereferenceable(9)) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define internal void @_ZN12_GLOBAL__N_112FrameCleanupD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %0) unnamed_addr #3 align 2 !dbg !2325 {
  %2 = alloca ptr, align 8
  %3 = alloca [21 x i8], align 16
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2326, !DIExpression(), !2328)
  %4 = load ptr, ptr %2, align 8
  %5 = call noundef i32 @_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqn220108EiNS_12memory_orderE(ptr noundef nonnull align 4 dereferenceable(4) @_ZN12_GLOBAL__N_119frame_cleanup_countE, i32 noundef 1, i32 noundef 0) #2, !dbg !2329
    #dbg_declare(ptr %3, !2331, !DIExpression(), !2332)
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %3, ptr align 16 @"__const.~FrameCleanup.marker", i64 21, i1 false), !dbg !2332
  %6 = getelementptr inbounds [21 x i8], ptr %3, i64 0, i64 0, !dbg !2333
  %7 = call i64 @write(i32 noundef 1, ptr noundef %6, i64 noundef 20), !dbg !2334
  %8 = icmp ne i64 %7, 0, !dbg !2334
  %9 = xor i1 %8, true, !dbg !2335
  ret void, !dbg !2336
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden void @_ZNSt3__111unique_lockINS_5mutexEED2B9nqn220108Ev(ptr noundef nonnull align 8 dereferenceable(9) %0) unnamed_addr #3 comdat align 2 !dbg !2337 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !2338, !DIExpression(), !2339)
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %3, i32 0, i32 1, !dbg !2340
  %5 = load i8, ptr %4, align 8, !dbg !2340
  %6 = trunc i8 %5 to i1, !dbg !2340
  br i1 %6, label %7, label %10, !dbg !2340

7:                                                ; preds = %1
  %8 = getelementptr inbounds nuw %"class.std::__1::unique_lock", ptr %3, i32 0, i32 0, !dbg !2343
  %9 = load ptr, ptr %8, align 8, !dbg !2343
  call void @_ZNSt3__15mutex6unlockEv(ptr noundef nonnull align 8 dereferenceable(40) %9) #2, !dbg !2344
  br label %10, !dbg !2343

10:                                               ; preds = %7, %1
  ret void, !dbg !2345
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #7

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #8

; Function Attrs: nounwind
declare i32 @clock_gettime(i32 noundef, ptr noundef) #1

; Function Attrs: nounwind
declare i32 @pthread_mutex_timedlock(ptr noundef, ptr noundef) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden void @_ZNSt3__118__cxx_atomic_storeB9nqn220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE(ptr noundef %0, i1 noundef zeroext %1, i32 noundef %2) #3 comdat !dbg !2346 {
  %4 = alloca ptr, align 8
  %5 = alloca i8, align 1
  %6 = alloca i32, align 4
  %7 = alloca i8, align 1
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2350, !DIExpression(), !2351)
  %8 = zext i1 %1 to i8
  store i8 %8, ptr %5, align 1
    #dbg_declare(ptr %5, !2352, !DIExpression(), !2353)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2354, !DIExpression(), !2355)
  %9 = load ptr, ptr %4, align 8, !dbg !2356
  %10 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl", ptr %9, i32 0, i32 0, !dbg !2357
  %11 = load i32, ptr %6, align 4, !dbg !2358
  %12 = load i8, ptr %5, align 1, !dbg !2359
  %13 = trunc i8 %12 to i1, !dbg !2359
  %14 = zext i1 %13 to i8, !dbg !2360
  store i8 %14, ptr %7, align 1, !dbg !2360
  switch i32 %11, label %15 [
    i32 3, label %17
    i32 5, label %19
  ], !dbg !2360

15:                                               ; preds = %3
  %16 = load i8, ptr %7, align 1, !dbg !2360
  store atomic i8 %16, ptr %10 monotonic, align 1, !dbg !2360
  br label %21, !dbg !2360

17:                                               ; preds = %3
  %18 = load i8, ptr %7, align 1, !dbg !2360
  store atomic i8 %18, ptr %10 release, align 1, !dbg !2360
  br label %21, !dbg !2360

19:                                               ; preds = %3
  %20 = load i8, ptr %7, align 1, !dbg !2360
  store atomic i8 %20, ptr %10 seq_cst, align 1, !dbg !2360
  br label %21, !dbg !2360

21:                                               ; preds = %19, %17, %15
  ret void, !dbg !2361
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNSt3__122__cxx_atomic_fetch_addB9nqn220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE(ptr noundef %0, i32 noundef %1, i32 noundef %2) #3 comdat !dbg !2362 {
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !2366, !DIExpression(), !2367)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !2368, !DIExpression(), !2369)
  store i32 %2, ptr %6, align 4
    #dbg_declare(ptr %6, !2370, !DIExpression(), !2371)
  %9 = load ptr, ptr %4, align 8, !dbg !2372
  %10 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl.4", ptr %9, i32 0, i32 0, !dbg !2373
  %11 = load i32, ptr %6, align 4, !dbg !2374
  %12 = load i32, ptr %5, align 4, !dbg !2375
  store i32 %12, ptr %7, align 4, !dbg !2376
  switch i32 %11, label %13 [
    i32 1, label %16
    i32 2, label %16
    i32 3, label %19
    i32 4, label %22
    i32 5, label %25
  ], !dbg !2376

13:                                               ; preds = %3
  %14 = load i32, ptr %7, align 4, !dbg !2376
  %15 = atomicrmw add ptr %10, i32 %14 monotonic, align 4, !dbg !2376
  store i32 %15, ptr %8, align 4, !dbg !2376
  br label %28, !dbg !2376

16:                                               ; preds = %3, %3
  %17 = load i32, ptr %7, align 4, !dbg !2376
  %18 = atomicrmw add ptr %10, i32 %17 acquire, align 4, !dbg !2376
  store i32 %18, ptr %8, align 4, !dbg !2376
  br label %28, !dbg !2376

19:                                               ; preds = %3
  %20 = load i32, ptr %7, align 4, !dbg !2376
  %21 = atomicrmw add ptr %10, i32 %20 release, align 4, !dbg !2376
  store i32 %21, ptr %8, align 4, !dbg !2376
  br label %28, !dbg !2376

22:                                               ; preds = %3
  %23 = load i32, ptr %7, align 4, !dbg !2376
  %24 = atomicrmw add ptr %10, i32 %23 acq_rel, align 4, !dbg !2376
  store i32 %24, ptr %8, align 4, !dbg !2376
  br label %28, !dbg !2376

25:                                               ; preds = %3
  %26 = load i32, ptr %7, align 4, !dbg !2376
  %27 = atomicrmw add ptr %10, i32 %26 seq_cst, align 4, !dbg !2376
  store i32 %27, ptr %8, align 4, !dbg !2376
  br label %28, !dbg !2376

28:                                               ; preds = %25, %22, %19, %16, %13
  %29 = load i32, ptr %8, align 4, !dbg !2376
  ret i32 %29, !dbg !2377
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef i32 @_ZNSt3__117__cxx_atomic_loadB9nqn220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %0, i32 noundef %1) #3 comdat !dbg !762 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2378, !DIExpression(), !2379)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2380, !DIExpression(), !2381)
  %6 = load ptr, ptr %3, align 8, !dbg !2382
  %7 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl.4", ptr %6, i32 0, i32 0, !dbg !2383
  %8 = load i32, ptr %4, align 4, !dbg !2384
  switch i32 %8, label %9 [
    i32 1, label %11
    i32 2, label %11
    i32 5, label %13
  ], !dbg !2385

9:                                                ; preds = %2
  %10 = load atomic i32, ptr %7 monotonic, align 4, !dbg !2385
  store i32 %10, ptr %5, align 4, !dbg !2385
  br label %15, !dbg !2385

11:                                               ; preds = %2, %2
  %12 = load atomic i32, ptr %7 acquire, align 4, !dbg !2385
  store i32 %12, ptr %5, align 4, !dbg !2385
  br label %15, !dbg !2385

13:                                               ; preds = %2
  %14 = load atomic i32, ptr %7 seq_cst, align 4, !dbg !2385
  store i32 %14, ptr %5, align 4, !dbg !2385
  br label %15, !dbg !2385

15:                                               ; preds = %13, %11, %9
  %16 = load i32, ptr %5, align 4, !dbg !2385
  ret i32 %16, !dbg !2386
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr hidden noundef zeroext i1 @_ZNSt3__117__cxx_atomic_loadB9nqn220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE(ptr noundef %0, i32 noundef %1) #3 comdat !dbg !769 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  %5 = alloca i8, align 1
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !2387, !DIExpression(), !2388)
  store i32 %1, ptr %4, align 4
    #dbg_declare(ptr %4, !2389, !DIExpression(), !2390)
  %6 = load ptr, ptr %3, align 8, !dbg !2391
  %7 = getelementptr inbounds nuw %"struct.std::__1::__cxx_atomic_base_impl", ptr %6, i32 0, i32 0, !dbg !2392
  %8 = load i32, ptr %4, align 4, !dbg !2393
  switch i32 %8, label %9 [
    i32 1, label %11
    i32 2, label %11
    i32 5, label %13
  ], !dbg !2394

9:                                                ; preds = %2
  %10 = load atomic i8, ptr %7 monotonic, align 1, !dbg !2394
  store i8 %10, ptr %5, align 1, !dbg !2394
  br label %15, !dbg !2394

11:                                               ; preds = %2, %2
  %12 = load atomic i8, ptr %7 acquire, align 1, !dbg !2394
  store i8 %12, ptr %5, align 1, !dbg !2394
  br label %15, !dbg !2394

13:                                               ; preds = %2
  %14 = load atomic i8, ptr %7 seq_cst, align 1, !dbg !2394
  store i8 %14, ptr %5, align 1, !dbg !2394
  br label %15, !dbg !2394

15:                                               ; preds = %13, %11, %9
  %16 = load i8, ptr %5, align 1, !dbg !2394
  %17 = trunc i8 %16 to i1, !dbg !2394
  ret i1 %17, !dbg !2395
}

; Function Attrs: noinline nounwind uwtable
define internal void @_GLOBAL__sub_I_item1_oldcaller_cancel.cpp() #0 section ".text.startup" !dbg !2396 {
  call void @__cxx_global_var_init(), !dbg !2398
  ret void
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }
attributes #3 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress noinline norecurse nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #8 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #9 = { noreturn nounwind }

!llvm.dbg.cu = !{!744}
!llvm.module.flags = !{!2010, !2011, !2012, !2013, !2014, !2015}
!llvm.ident = !{!2016}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "target_cv", linkageName: "_ZN12_GLOBAL__N_19target_cvE", scope: !2, file: !3, line: 15, type: !4, isLocal: true, isDefinition: true)
!2 = !DINamespace(scope: null)
!3 = !DIFile(filename: "docs/progress/R112/code/item1_oldcaller_cancel.cpp", directory: "/home/toolchain/development/libcxx_runtime_validation", checksumkind: CSK_MD5, checksum: "d9eb924ef8b75221260b6aa86ef8583b")
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
!49 = !DISubprogram(name: "condition_variable", linkageName: "_ZNSt3__118condition_variableC4B9nqn220108Ev", scope: !4, file: !5, line: 93, type: !50, scopeLine: 93, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
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
!109 = !DISubprogram(name: "mutex", linkageName: "_ZNSt3__15mutexC4B9nqn220108Ev", scope: !75, file: !76, line: 28, type: !110, scopeLine: 28, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
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
!122 = !DISubprogram(name: "~mutex", linkageName: "_ZNSt3__15mutexD4B9nqn220108Ev", scope: !75, file: !76, line: 34, type: !110, scopeLine: 34, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!123 = !DISubprogram(name: "lock", linkageName: "_ZNSt3__15mutex4lockEv", scope: !75, file: !76, line: 39, type: !110, scopeLine: 39, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!124 = !DISubprogram(name: "try_lock", linkageName: "_ZNSt3__15mutex8try_lockEv", scope: !75, file: !76, line: 40, type: !125, scopeLine: 40, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!125 = !DISubroutineType(types: !126)
!126 = !{!127, !112}
!127 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!128 = !DISubprogram(name: "unlock", linkageName: "_ZNSt3__15mutex6unlockEv", scope: !75, file: !76, line: 41, type: !110, scopeLine: 41, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!129 = !DISubprogram(name: "native_handle", linkageName: "_ZNSt3__15mutex13native_handleB9nqn220108Ev", scope: !75, file: !76, line: 44, type: !130, scopeLine: 44, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!130 = !DISubroutineType(types: !131)
!131 = !{!132, !112}
!132 = !DIDerivedType(tag: DW_TAG_typedef, name: "native_handle_type", scope: !75, file: !76, line: 43, baseType: !133, flags: DIFlagPublic)
!133 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !79, size: 64)
!134 = !DIDerivedType(tag: DW_TAG_member, name: "__owns_", scope: !69, file: !70, line: 38, baseType: !127, size: 8, offset: 64)
!135 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqn220108Ev", scope: !69, file: !70, line: 41, type: !136, scopeLine: 41, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!136 = !DISubroutineType(types: !137)
!137 = !{null, !138}
!138 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !69, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!139 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqn220108ERS1_", scope: !69, file: !70, line: 42, type: !140, scopeLine: 42, flags: DIFlagPublic | DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!140 = !DISubroutineType(types: !141)
!141 = !{null, !138, !142}
!142 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !74, size: 64)
!143 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqn220108ERS1_NS_12defer_lock_tE", scope: !69, file: !70, line: 47, type: !144, scopeLine: 47, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!144 = !DISubroutineType(types: !145)
!145 = !{null, !138, !142, !146}
!146 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "defer_lock_t", scope: !6, file: !147, line: 20, size: 8, flags: DIFlagTypePassByValue, elements: !148, identifier: "_ZTSNSt3__112defer_lock_tE")
!147 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__mutex/tag_types.h", directory: "/home/toolchain/development")
!148 = !{!149}
!149 = !DISubprogram(name: "defer_lock_t", linkageName: "_ZNSt3__112defer_lock_tC4Ev", scope: !146, file: !147, line: 21, type: !150, scopeLine: 21, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!150 = !DISubroutineType(types: !151)
!151 = !{null, !152}
!152 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !146, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!153 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqn220108ERS1_NS_13try_to_lock_tE", scope: !69, file: !70, line: 51, type: !154, scopeLine: 51, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!154 = !DISubroutineType(types: !155)
!155 = !{null, !138, !142, !156}
!156 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "try_to_lock_t", scope: !6, file: !147, line: 24, size: 8, flags: DIFlagTypePassByValue, elements: !157, identifier: "_ZTSNSt3__113try_to_lock_tE")
!157 = !{!158}
!158 = !DISubprogram(name: "try_to_lock_t", linkageName: "_ZNSt3__113try_to_lock_tC4Ev", scope: !156, file: !147, line: 25, type: !159, scopeLine: 25, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!159 = !DISubroutineType(types: !160)
!160 = !{null, !161}
!161 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !156, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!162 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqn220108ERS1_NS_12adopt_lock_tE", scope: !69, file: !70, line: 54, type: !163, scopeLine: 54, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!163 = !DISubroutineType(types: !164)
!164 = !{null, !138, !142, !165}
!165 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "adopt_lock_t", scope: !6, file: !147, line: 28, size: 8, flags: DIFlagTypePassByValue, elements: !166, identifier: "_ZTSNSt3__112adopt_lock_tE")
!166 = !{!167}
!167 = !DISubprogram(name: "adopt_lock_t", linkageName: "_ZNSt3__112adopt_lock_tC4Ev", scope: !165, file: !147, line: 29, type: !168, scopeLine: 29, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!168 = !DISubroutineType(types: !169)
!169 = !{null, !170}
!170 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !165, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!171 = !DISubprogram(name: "~unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEED4B9nqn220108Ev", scope: !69, file: !70, line: 65, type: !136, scopeLine: 65, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!172 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4ERKS2_", scope: !69, file: !70, line: 70, type: !173, scopeLine: 70, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!173 = !DISubroutineType(types: !174)
!174 = !{null, !138, !175}
!175 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !176, size: 64)
!176 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !69)
!177 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEaSERKS2_", scope: !69, file: !70, line: 71, type: !178, scopeLine: 71, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDeleted)
!178 = !DISubroutineType(types: !179)
!179 = !{!68, !138, !175}
!180 = !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC4B9nqn220108EOS2_", scope: !69, file: !70, line: 73, type: !181, scopeLine: 73, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!181 = !DISubroutineType(types: !182)
!182 = !{null, !138, !183}
!183 = !DIDerivedType(tag: DW_TAG_rvalue_reference_type, baseType: !69, size: 64)
!184 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEaSB9nqn220108EOS2_", scope: !69, file: !70, line: 80, type: !185, scopeLine: 80, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!185 = !DISubroutineType(types: !186)
!186 = !{!68, !138, !183}
!187 = !DISubprogram(name: "lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE4lockB9nqn220108Ev", scope: !69, file: !70, line: 86, type: !136, scopeLine: 86, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!188 = !DISubprogram(name: "try_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE8try_lockB9nqn220108Ev", scope: !69, file: !70, line: 87, type: !189, scopeLine: 87, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!189 = !DISubroutineType(types: !190)
!190 = !{!127, !138}
!191 = !DISubprogram(name: "unlock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE6unlockB9nqn220108Ev", scope: !69, file: !70, line: 95, type: !136, scopeLine: 95, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!192 = !DISubprogram(name: "swap", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE4swapB9nqn220108ERS2_", scope: !69, file: !70, line: 97, type: !193, scopeLine: 97, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!193 = !DISubroutineType(types: !194)
!194 = !{null, !138, !68}
!195 = !DISubprogram(name: "release", linkageName: "_ZNSt3__111unique_lockINS_5mutexEE7releaseB9nqn220108Ev", scope: !69, file: !70, line: 102, type: !196, scopeLine: 102, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!196 = !DISubroutineType(types: !197)
!197 = !{!73, !138}
!198 = !DISubprogram(name: "owns_lock", linkageName: "_ZNKSt3__111unique_lockINS_5mutexEE9owns_lockB9nqn220108Ev", scope: !69, file: !70, line: 109, type: !199, scopeLine: 109, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!199 = !DISubroutineType(types: !200)
!200 = !{!127, !201}
!201 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !176, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!202 = !DISubprogram(name: "operator bool", linkageName: "_ZNKSt3__111unique_lockINS_5mutexEEcvbB9nqn220108Ev", scope: !69, file: !70, line: 110, type: !199, scopeLine: 110, flags: DIFlagPublic | DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!203 = !DISubprogram(name: "mutex", linkageName: "_ZNKSt3__111unique_lockINS_5mutexEE5mutexB9nqn220108Ev", scope: !69, file: !70, line: 111, type: !204, scopeLine: 111, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!204 = !DISubroutineType(types: !205)
!205 = !{!73, !201}
!206 = !{!207}
!207 = !DITemplateTypeParameter(name: "_Mutex", type: !75)
!208 = !DISubprogram(name: "native_handle", linkageName: "_ZNSt3__118condition_variable13native_handleB9nqn220108Ev", scope: !4, file: !5, line: 173, type: !209, scopeLine: 173, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
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
!219 = !DISubprogram(name: "__do_timed_wait", linkageName: "_ZNSt3__118condition_variable15__do_timed_waitB9nqn220108ERNS_11unique_lockINS_5mutexEEENS_6chrono10time_pointINS5_12steady_clockENS5_8durationIxNS_5ratioILl1ELl1000000000EEEEEEE", scope: !4, file: !5, line: 180, type: !220, scopeLine: 180, flags: DIFlagPrototyped, spFlags: 0)
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
!235 = !DISubprogram(name: "count", linkageName: "_ZNKSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE5countB9nqn220108Ev", scope: !226, file: !227, line: 230, type: !236, scopeLine: 230, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!236 = !DISubroutineType(types: !237)
!237 = !{!230, !238}
!238 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !239, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!239 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !226)
!240 = !DISubprogram(name: "operator+", linkageName: "_ZNKSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEpsB9nqn220108Ev", scope: !226, file: !227, line: 234, type: !241, scopeLine: 234, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!241 = !DISubroutineType(types: !242)
!242 = !{!243, !238}
!243 = !DIDerivedType(tag: DW_TAG_typedef, name: "type", scope: !245, file: !244, line: 22, baseType: !226)
!244 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__type_traits/type_identity.h", directory: "/home/toolchain/development")
!245 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__type_identity<std::__1::chrono::duration<long long, std::__1::ratio<1L, 1000000000L> > >", scope: !6, file: !244, line: 21, size: 8, flags: DIFlagTypePassByValue, elements: !246, templateParams: !247, identifier: "_ZTSNSt3__115__type_identityINS_6chrono8durationIxNS_5ratioILl1ELl1000000000EEEEEEE")
!246 = !{}
!247 = !{!248}
!248 = !DITemplateTypeParameter(name: "_Tp", type: !226)
!249 = !DISubprogram(name: "operator-", linkageName: "_ZNKSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEngB9nqn220108Ev", scope: !226, file: !227, line: 237, type: !241, scopeLine: 237, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!250 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEppB9nqn220108Ev", scope: !226, file: !227, line: 240, type: !251, scopeLine: 240, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!251 = !DISubroutineType(types: !252)
!252 = !{!253, !234}
!253 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !226, size: 64)
!254 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEppB9nqn220108Ei", scope: !226, file: !227, line: 244, type: !255, scopeLine: 244, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!255 = !DISubroutineType(types: !256)
!256 = !{!226, !234, !88}
!257 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmmB9nqn220108Ev", scope: !226, file: !227, line: 245, type: !251, scopeLine: 245, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!258 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmmB9nqn220108Ei", scope: !226, file: !227, line: 249, type: !255, scopeLine: 249, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!259 = !DISubprogram(name: "operator+=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEpLB9nqn220108ERKS4_", scope: !226, file: !227, line: 251, type: !260, scopeLine: 251, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!260 = !DISubroutineType(types: !261)
!261 = !{!253, !234, !262}
!262 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !239, size: 64)
!263 = !DISubprogram(name: "operator-=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmIB9nqn220108ERKS4_", scope: !226, file: !227, line: 255, type: !260, scopeLine: 255, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!264 = !DISubprogram(name: "operator*=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEmLB9nqn220108ERKx", scope: !226, file: !227, line: 260, type: !265, scopeLine: 260, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!265 = !DISubroutineType(types: !266)
!266 = !{!253, !234, !267}
!267 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !268, size: 64)
!268 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !230)
!269 = !DISubprogram(name: "operator/=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEEdVB9nqn220108ERKx", scope: !226, file: !227, line: 264, type: !265, scopeLine: 264, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!270 = !DISubprogram(name: "operator%=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEErMB9nqn220108ERKx", scope: !226, file: !227, line: 268, type: !265, scopeLine: 268, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!271 = !DISubprogram(name: "operator%=", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEErMB9nqn220108ERKS4_", scope: !226, file: !227, line: 272, type: !260, scopeLine: 272, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!272 = !DISubprogram(name: "zero", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE4zeroB9nqn220108Ev", scope: !226, file: !227, line: 279, type: !273, scopeLine: 279, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!273 = !DISubroutineType(types: !274)
!274 = !{!226}
!275 = !DISubprogram(name: "min", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE3minB9nqn220108Ev", scope: !226, file: !227, line: 282, type: !273, scopeLine: 282, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!276 = !DISubprogram(name: "max", linkageName: "_ZNSt3__16chrono8durationIxNS_5ratioILl1ELl1000000000EEEE3maxB9nqn220108Ev", scope: !226, file: !227, line: 285, type: !273, scopeLine: 285, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
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
!297 = !DISubprogram(name: "time_point", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEC4B9nqn220108Ev", scope: !222, file: !217, line: 49, type: !298, scopeLine: 49, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!298 = !DISubroutineType(types: !299)
!299 = !{null, !300}
!300 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !222, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!301 = !DISubprogram(name: "time_point", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEC4B9nqn220108ERKS6_", scope: !222, file: !217, line: 50, type: !302, scopeLine: 50, flags: DIFlagPublic | DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!302 = !DISubroutineType(types: !303)
!303 = !{null, !300, !304}
!304 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !305, size: 64)
!305 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !225)
!306 = !DISubprogram(name: "time_since_epoch", linkageName: "_ZNKSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEE16time_since_epochB9nqn220108Ev", scope: !222, file: !217, line: 59, type: !307, scopeLine: 59, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!307 = !DISubroutineType(types: !308)
!308 = !{!225, !309}
!309 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !310, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!310 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !222)
!311 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEppB9nqn220108Ev", scope: !222, file: !217, line: 66, type: !312, scopeLine: 66, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!312 = !DISubroutineType(types: !313)
!313 = !{!314, !300}
!314 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !222, size: 64)
!315 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEppB9nqn220108Ei", scope: !222, file: !217, line: 70, type: !316, scopeLine: 70, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!316 = !DISubroutineType(types: !317)
!317 = !{!222, !300, !88}
!318 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEmmB9nqn220108Ev", scope: !222, file: !217, line: 71, type: !312, scopeLine: 71, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!319 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEmmB9nqn220108Ei", scope: !222, file: !217, line: 75, type: !316, scopeLine: 75, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!320 = !DISubprogram(name: "operator+=", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEpLB9nqn220108ERKS6_", scope: !222, file: !217, line: 78, type: !321, scopeLine: 78, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!321 = !DISubroutineType(types: !322)
!322 = !{!314, !300, !304}
!323 = !DISubprogram(name: "operator-=", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEEmIB9nqn220108ERKS6_", scope: !222, file: !217, line: 82, type: !321, scopeLine: 82, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!324 = !DISubprogram(name: "min", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEE3minB9nqn220108Ev", scope: !222, file: !217, line: 89, type: !325, scopeLine: 89, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
!325 = !DISubroutineType(types: !326)
!326 = !{!222}
!327 = !DISubprogram(name: "max", linkageName: "_ZNSt3__16chrono10time_pointINS0_12steady_clockENS0_8durationIxNS_5ratioILl1ELl1000000000EEEEEE3maxB9nqn220108Ev", scope: !222, file: !217, line: 92, type: !325, scopeLine: 92, flags: DIFlagPublic | DIFlagPrototyped | DIFlagStaticMember, spFlags: 0)
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
!348 = distinct !DIGlobalVariable(scope: null, file: !3, line: 75, type: !349, isLocal: true, isDefinition: true)
!349 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 168, elements: !351)
!350 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !44)
!351 = !{!352}
!352 = !DISubrange(count: 21)
!353 = !DIGlobalVariableExpression(var: !354, expr: !DIExpression())
!354 = distinct !DIGlobalVariable(name: "target_native", linkageName: "_ZN12_GLOBAL__N_113target_nativeE", scope: !2, file: !3, line: 20, type: !346, isLocal: true, isDefinition: true)
!355 = !DIGlobalVariableExpression(var: !356, expr: !DIExpression())
!356 = distinct !DIGlobalVariable(scope: null, file: !3, line: 88, type: !357, isLocal: true, isDefinition: true)
!357 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 152, elements: !358)
!358 = !{!359}
!359 = !DISubrange(count: 19)
!360 = !DIGlobalVariableExpression(var: !361, expr: !DIExpression())
!361 = distinct !DIGlobalVariable(name: "target_mutex", linkageName: "_ZN12_GLOBAL__N_112target_mutexE", scope: !2, file: !3, line: 14, type: !75, isLocal: true, isDefinition: true)
!362 = !DIGlobalVariableExpression(var: !363, expr: !DIExpression())
!363 = distinct !DIGlobalVariable(scope: null, file: !3, line: 95, type: !364, isLocal: true, isDefinition: true)
!364 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 160, elements: !365)
!365 = !{!366}
!366 = !DISubrange(count: 20)
!367 = !DIGlobalVariableExpression(var: !368, expr: !DIExpression())
!368 = distinct !DIGlobalVariable(scope: null, file: !3, line: 103, type: !349, isLocal: true, isDefinition: true)
!369 = !DIGlobalVariableExpression(var: !370, expr: !DIExpression())
!370 = distinct !DIGlobalVariable(scope: null, file: !3, line: 111, type: !371, isLocal: true, isDefinition: true)
!371 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 672, elements: !372)
!372 = !{!373}
!373 = !DISubrange(count: 84)
!374 = !DIGlobalVariableExpression(var: !375, expr: !DIExpression())
!375 = distinct !DIGlobalVariable(scope: null, file: !3, line: 119, type: !376, isLocal: true, isDefinition: true)
!376 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 280, elements: !377)
!377 = !{!378}
!378 = !DISubrange(count: 35)
!379 = !DIGlobalVariableExpression(var: !380, expr: !DIExpression())
!380 = distinct !DIGlobalVariable(scope: null, file: !3, line: 128, type: !381, isLocal: true, isDefinition: true)
!381 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 496, elements: !382)
!382 = !{!383}
!383 = !DISubrange(count: 62)
!384 = !DIGlobalVariableExpression(var: !385, expr: !DIExpression())
!385 = distinct !DIGlobalVariable(scope: null, file: !3, line: 143, type: !386, isLocal: true, isDefinition: true)
!386 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 208, elements: !387)
!387 = !{!388}
!388 = !DISubrange(count: 26)
!389 = !DIGlobalVariableExpression(var: !390, expr: !DIExpression())
!390 = distinct !DIGlobalVariable(name: "track_unlock", linkageName: "_ZN12_GLOBAL__N_112track_unlockE", scope: !2, file: !3, line: 17, type: !391, isLocal: true, isDefinition: true)
!391 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "atomic<bool>", scope: !6, file: !392, line: 242, size: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !393, templateParams: !414, identifier: "_ZTSNSt3__16atomicIbEE")
!392 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/atomic.h", directory: "/home/toolchain/development")
!393 = !{!394, !509, !513, !516, !521, !524, !530}
!394 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !391, baseType: !395, extraData: i32 0)
!395 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__atomic_base<bool, false>", scope: !6, file: !392, line: 42, size: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !396, templateParams: !507, identifier: "_ZTSNSt3__113__atomic_baseIbLb0EEE")
!396 = !{!397, !425, !426, !432, !437, !450, !454, !457, !460, !461, !462, !465, !468, !472, !475, !476, !477, !480, !483, !484, !485, !488, !491, !494, !497, !498, !499, !500, !503}
!397 = !DIDerivedType(tag: DW_TAG_member, name: "__a_", scope: !395, file: !392, line: 44, baseType: !398, size: 8)
!398 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_impl<bool, std::__1::__cxx_atomic_base_impl<bool> >", scope: !6, file: !399, line: 114, size: 8, flags: DIFlagTypePassByValue, elements: !400, templateParams: !423, identifier: "_ZTSNSt3__117__cxx_atomic_implIbNS_22__cxx_atomic_base_implIbEEEE")
!399 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/support.h", directory: "/home/toolchain/development")
!400 = !{!401, !416, !420}
!401 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !398, baseType: !402, extraData: i32 0)
!402 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_base_impl<bool>", scope: !6, file: !403, line: 29, size: 8, flags: DIFlagTypePassByValue, elements: !404, templateParams: !414, identifier: "_ZTSNSt3__122__cxx_atomic_base_implIbEE")
!403 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/support/c11.h", directory: "/home/toolchain/development")
!404 = !{!405, !407, !411}
!405 = !DIDerivedType(tag: DW_TAG_member, name: "__a_value", scope: !402, file: !403, line: 38, baseType: !406, size: 8)
!406 = !DIDerivedType(tag: DW_TAG_atomic_type, baseType: !127)
!407 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIbEC4B9nqn220108Ev", scope: !402, file: !403, line: 32, type: !408, scopeLine: 32, flags: DIFlagPrototyped, spFlags: 0)
!408 = !DISubroutineType(types: !409)
!409 = !{null, !410}
!410 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !402, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!411 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIbEC4Eb", scope: !402, file: !403, line: 37, type: !412, scopeLine: 37, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!412 = !DISubroutineType(types: !413)
!413 = !{null, !410, !127}
!414 = !{!415}
!415 = !DITemplateTypeParameter(name: "_Tp", type: !127)
!416 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIbNS_22__cxx_atomic_base_implIbEEEC4B9nqn220108Ev", scope: !398, file: !399, line: 115, type: !417, scopeLine: 115, flags: DIFlagPrototyped, spFlags: 0)
!417 = !DISubroutineType(types: !418)
!418 = !{null, !419}
!419 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !398, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!420 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIbNS_22__cxx_atomic_base_implIbEEEC4B9nqn220108Eb", scope: !398, file: !399, line: 116, type: !421, scopeLine: 116, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!421 = !DISubroutineType(types: !422)
!422 = !{null, !419, !127}
!423 = !{!415, !424}
!424 = !DITemplateTypeParameter(name: "_Base", type: !402, defaulted: true)
!425 = !DIDerivedType(tag: DW_TAG_variable, name: "is_always_lock_free", scope: !395, file: !392, line: 49, baseType: !334, flags: DIFlagStaticMember)
!426 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EE12is_lock_freeB9nqn220108Ev", scope: !395, file: !392, line: 52, type: !427, scopeLine: 52, flags: DIFlagPrototyped, spFlags: 0)
!427 = !DISubroutineType(types: !428)
!428 = !{!127, !429}
!429 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !430, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!430 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !431)
!431 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !395)
!432 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE12is_lock_freeB9nqn220108Ev", scope: !395, file: !392, line: 55, type: !433, scopeLine: 55, flags: DIFlagPrototyped, spFlags: 0)
!433 = !DISubroutineType(types: !434)
!434 = !{!127, !435}
!435 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !436, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!436 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !395)
!437 = !DISubprogram(name: "store", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE", scope: !395, file: !392, line: 58, type: !438, scopeLine: 58, flags: DIFlagPrototyped, spFlags: 0)
!438 = !DISubroutineType(types: !439)
!439 = !{null, !440, !127, !441}
!440 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !431, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!441 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "memory_order", scope: !6, file: !442, line: 31, baseType: !31, size: 32, flags: DIFlagEnumClass, elements: !443, identifier: "_ZTSNSt3__112memory_orderE")
!442 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__atomic/memory_order.h", directory: "/home/toolchain/development")
!443 = !{!444, !445, !446, !447, !448, !449}
!444 = !DIEnumerator(name: "relaxed", value: 0, isUnsigned: true)
!445 = !DIEnumerator(name: "consume", value: 1, isUnsigned: true)
!446 = !DIEnumerator(name: "acquire", value: 2, isUnsigned: true)
!447 = !DIEnumerator(name: "release", value: 3, isUnsigned: true)
!448 = !DIEnumerator(name: "acq_rel", value: 4, isUnsigned: true)
!449 = !DIEnumerator(name: "seq_cst", value: 5, isUnsigned: true)
!450 = !DISubprogram(name: "store", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE", scope: !395, file: !392, line: 62, type: !451, scopeLine: 62, flags: DIFlagPrototyped, spFlags: 0)
!451 = !DISubroutineType(types: !452)
!452 = !{null, !453, !127, !441}
!453 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !395, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!454 = !DISubprogram(name: "load", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EE4loadB9nqn220108ENS_12memory_orderE", scope: !395, file: !392, line: 66, type: !455, scopeLine: 66, flags: DIFlagPrototyped, spFlags: 0)
!455 = !DISubroutineType(types: !456)
!456 = !{!127, !429, !441}
!457 = !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqn220108ENS_12memory_orderE", scope: !395, file: !392, line: 70, type: !458, scopeLine: 70, flags: DIFlagPrototyped, spFlags: 0)
!458 = !DISubroutineType(types: !459)
!459 = !{!127, !435, !441}
!460 = !DISubprogram(name: "operator bool", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EEcvbB9nqn220108Ev", scope: !395, file: !392, line: 74, type: !427, scopeLine: 74, flags: DIFlagPrototyped, spFlags: 0)
!461 = !DISubprogram(name: "operator bool", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EEcvbB9nqn220108Ev", scope: !395, file: !392, line: 75, type: !433, scopeLine: 75, flags: DIFlagPrototyped, spFlags: 0)
!462 = !DISubprogram(name: "exchange", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE8exchangeB9nqn220108EbNS_12memory_orderE", scope: !395, file: !392, line: 76, type: !463, scopeLine: 76, flags: DIFlagPrototyped, spFlags: 0)
!463 = !DISubroutineType(types: !464)
!464 = !{!127, !440, !127, !441}
!465 = !DISubprogram(name: "exchange", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE8exchangeB9nqn220108EbNS_12memory_orderE", scope: !395, file: !392, line: 79, type: !466, scopeLine: 79, flags: DIFlagPrototyped, spFlags: 0)
!466 = !DISubroutineType(types: !467)
!467 = !{!127, !453, !127, !441}
!468 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqn220108ERbbNS_12memory_orderES3_", scope: !395, file: !392, line: 83, type: !469, scopeLine: 83, flags: DIFlagPrototyped, spFlags: 0)
!469 = !DISubroutineType(types: !470)
!470 = !{!127, !440, !471, !127, !441, !441}
!471 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !127, size: 64)
!472 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqn220108ERbbNS_12memory_orderES3_", scope: !395, file: !392, line: 87, type: !473, scopeLine: 87, flags: DIFlagPrototyped, spFlags: 0)
!473 = !DISubroutineType(types: !474)
!474 = !{!127, !453, !471, !127, !441, !441}
!475 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqn220108ERbbNS_12memory_orderES3_", scope: !395, file: !392, line: 92, type: !469, scopeLine: 92, flags: DIFlagPrototyped, spFlags: 0)
!476 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqn220108ERbbNS_12memory_orderES3_", scope: !395, file: !392, line: 96, type: !473, scopeLine: 96, flags: DIFlagPrototyped, spFlags: 0)
!477 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqn220108ERbbNS_12memory_orderE", scope: !395, file: !392, line: 101, type: !478, scopeLine: 101, flags: DIFlagPrototyped, spFlags: 0)
!478 = !DISubroutineType(types: !479)
!479 = !{!127, !440, !471, !127, !441}
!480 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE21compare_exchange_weakB9nqn220108ERbbNS_12memory_orderE", scope: !395, file: !392, line: 105, type: !481, scopeLine: 105, flags: DIFlagPrototyped, spFlags: 0)
!481 = !DISubroutineType(types: !482)
!482 = !{!127, !453, !471, !127, !441}
!483 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqn220108ERbbNS_12memory_orderE", scope: !395, file: !392, line: 109, type: !478, scopeLine: 109, flags: DIFlagPrototyped, spFlags: 0)
!484 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE23compare_exchange_strongB9nqn220108ERbbNS_12memory_orderE", scope: !395, file: !392, line: 113, type: !481, scopeLine: 113, flags: DIFlagPrototyped, spFlags: 0)
!485 = !DISubprogram(name: "wait", linkageName: "_ZNVKSt3__113__atomic_baseIbLb0EE4waitB9nqn220108EbNS_12memory_orderE", scope: !395, file: !392, line: 118, type: !486, scopeLine: 118, flags: DIFlagPrototyped, spFlags: 0)
!486 = !DISubroutineType(types: !487)
!487 = !{null, !429, !127, !441}
!488 = !DISubprogram(name: "wait", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE4waitB9nqn220108EbNS_12memory_orderE", scope: !395, file: !392, line: 121, type: !489, scopeLine: 121, flags: DIFlagPrototyped, spFlags: 0)
!489 = !DISubroutineType(types: !490)
!490 = !{null, !435, !127, !441}
!491 = !DISubprogram(name: "notify_one", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE10notify_oneB9nqn220108Ev", scope: !395, file: !392, line: 124, type: !492, scopeLine: 124, flags: DIFlagPrototyped, spFlags: 0)
!492 = !DISubroutineType(types: !493)
!493 = !{null, !440}
!494 = !DISubprogram(name: "notify_one", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE10notify_oneB9nqn220108Ev", scope: !395, file: !392, line: 125, type: !495, scopeLine: 125, flags: DIFlagPrototyped, spFlags: 0)
!495 = !DISubroutineType(types: !496)
!496 = !{null, !453}
!497 = !DISubprogram(name: "notify_all", linkageName: "_ZNVSt3__113__atomic_baseIbLb0EE10notify_allB9nqn220108Ev", scope: !395, file: !392, line: 126, type: !492, scopeLine: 126, flags: DIFlagPrototyped, spFlags: 0)
!498 = !DISubprogram(name: "notify_all", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE10notify_allB9nqn220108Ev", scope: !395, file: !392, line: 127, type: !495, scopeLine: 127, flags: DIFlagPrototyped, spFlags: 0)
!499 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIbLb0EEC4B9nqn220108Ev", scope: !395, file: !392, line: 131, type: !495, scopeLine: 131, flags: DIFlagPrototyped, spFlags: 0)
!500 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIbLb0EEC4B9nqn220108Eb", scope: !395, file: !392, line: 136, type: !501, scopeLine: 136, flags: DIFlagPrototyped, spFlags: 0)
!501 = !DISubroutineType(types: !502)
!502 = !{null, !453, !127}
!503 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIbLb0EEC4ERKS1_", scope: !395, file: !392, line: 138, type: !504, scopeLine: 138, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!504 = !DISubroutineType(types: !505)
!505 = !{null, !453, !506}
!506 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !436, size: 64)
!507 = !{!415, !508}
!508 = !DITemplateValueParameter(type: !127, defaulted: true, value: i1 false)
!509 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIbEC4B9nqn220108Ev", scope: !391, file: !392, line: 246, type: !510, scopeLine: 246, flags: DIFlagPrototyped, spFlags: 0)
!510 = !DISubroutineType(types: !511)
!511 = !{null, !512}
!512 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !391, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!513 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIbEC4B9nqn220108Eb", scope: !391, file: !392, line: 251, type: !514, scopeLine: 251, flags: DIFlagPrototyped, spFlags: 0)
!514 = !DISubroutineType(types: !515)
!515 = !{null, !512, !127}
!516 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIbEaSB9nqn220108Eb", scope: !391, file: !392, line: 253, type: !517, scopeLine: 253, flags: DIFlagPrototyped, spFlags: 0)
!517 = !DISubroutineType(types: !518)
!518 = !{!127, !519, !127}
!519 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !520, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!520 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !391)
!521 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIbEaSB9nqn220108Eb", scope: !391, file: !392, line: 257, type: !522, scopeLine: 257, flags: DIFlagPrototyped, spFlags: 0)
!522 = !DISubroutineType(types: !523)
!523 = !{!127, !512, !127}
!524 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIbEaSERKS1_", scope: !391, file: !392, line: 262, type: !525, scopeLine: 262, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!525 = !DISubroutineType(types: !526)
!526 = !{!527, !512, !528}
!527 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !391, size: 64)
!528 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !529, size: 64)
!529 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !391)
!530 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIbEaSERKS1_", scope: !391, file: !392, line: 263, type: !531, scopeLine: 263, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!531 = !DISubroutineType(types: !532)
!532 = !{!527, !519, !528}
!533 = !DIGlobalVariableExpression(var: !534, expr: !DIExpression())
!534 = distinct !DIGlobalVariable(name: "target_unlock_count", linkageName: "_ZN12_GLOBAL__N_119target_unlock_countE", scope: !2, file: !3, line: 18, type: !535, isLocal: true, isDefinition: true)
!535 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "atomic<int>", scope: !6, file: !392, line: 242, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !536, templateParams: !558, identifier: "_ZTSNSt3__16atomicIiEE")
!536 = !{!537, !699, !703, !706, !711, !714, !720}
!537 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !535, baseType: !538, extraData: i32 0)
!538 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__atomic_base<int, true>", scope: !6, file: !392, line: 144, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !539, templateParams: !697, identifier: "_ZTSNSt3__113__atomic_baseIiLb1EEE")
!539 = !{!540, !648, !652, !655, !660, !663, !664, !665, !666, !667, !668, !669, !670, !671, !674, !677, !678, !679, !682, !685, !686, !687, !688, !689, !690, !691, !692, !693, !694, !695, !696}
!540 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !538, baseType: !541, extraData: i32 0)
!541 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__atomic_base<int, false>", scope: !6, file: !392, line: 42, size: 32, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !542, templateParams: !646, identifier: "_ZTSNSt3__113__atomic_baseIiLb0EEE")
!542 = !{!543, !569, !570, !576, !581, !585, !589, !592, !595, !598, !601, !604, !607, !611, !614, !615, !616, !619, !622, !623, !624, !627, !630, !633, !636, !637, !638, !639, !642}
!543 = !DIDerivedType(tag: DW_TAG_member, name: "__a_", scope: !541, file: !392, line: 44, baseType: !544, size: 32)
!544 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_impl<int, std::__1::__cxx_atomic_base_impl<int> >", scope: !6, file: !399, line: 114, size: 32, flags: DIFlagTypePassByValue, elements: !545, templateParams: !567, identifier: "_ZTSNSt3__117__cxx_atomic_implIiNS_22__cxx_atomic_base_implIiEEEE")
!545 = !{!546, !560, !564}
!546 = !DIDerivedType(tag: DW_TAG_inheritance, scope: !544, baseType: !547, extraData: i32 0)
!547 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__cxx_atomic_base_impl<int>", scope: !6, file: !403, line: 29, size: 32, flags: DIFlagTypePassByValue, elements: !548, templateParams: !558, identifier: "_ZTSNSt3__122__cxx_atomic_base_implIiEE")
!548 = !{!549, !551, !555}
!549 = !DIDerivedType(tag: DW_TAG_member, name: "__a_value", scope: !547, file: !403, line: 38, baseType: !550, size: 32)
!550 = !DIDerivedType(tag: DW_TAG_atomic_type, baseType: !88)
!551 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIiEC4B9nqn220108Ev", scope: !547, file: !403, line: 32, type: !552, scopeLine: 32, flags: DIFlagPrototyped, spFlags: 0)
!552 = !DISubroutineType(types: !553)
!553 = !{null, !554}
!554 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !547, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!555 = !DISubprogram(name: "__cxx_atomic_base_impl", linkageName: "_ZNSt3__122__cxx_atomic_base_implIiEC4Ei", scope: !547, file: !403, line: 37, type: !556, scopeLine: 37, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!556 = !DISubroutineType(types: !557)
!557 = !{null, !554, !88}
!558 = !{!559}
!559 = !DITemplateTypeParameter(name: "_Tp", type: !88)
!560 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIiNS_22__cxx_atomic_base_implIiEEEC4B9nqn220108Ev", scope: !544, file: !399, line: 115, type: !561, scopeLine: 115, flags: DIFlagPrototyped, spFlags: 0)
!561 = !DISubroutineType(types: !562)
!562 = !{null, !563}
!563 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !544, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!564 = !DISubprogram(name: "__cxx_atomic_impl", linkageName: "_ZNSt3__117__cxx_atomic_implIiNS_22__cxx_atomic_base_implIiEEEC4B9nqn220108Ei", scope: !544, file: !399, line: 116, type: !565, scopeLine: 116, flags: DIFlagExplicit | DIFlagPrototyped, spFlags: 0)
!565 = !DISubroutineType(types: !566)
!566 = !{null, !563, !88}
!567 = !{!559, !568}
!568 = !DITemplateTypeParameter(name: "_Base", type: !547, defaulted: true)
!569 = !DIDerivedType(tag: DW_TAG_variable, name: "is_always_lock_free", scope: !541, file: !392, line: 49, baseType: !334, flags: DIFlagStaticMember)
!570 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EE12is_lock_freeB9nqn220108Ev", scope: !541, file: !392, line: 52, type: !571, scopeLine: 52, flags: DIFlagPrototyped, spFlags: 0)
!571 = !DISubroutineType(types: !572)
!572 = !{!127, !573}
!573 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !574, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!574 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !575)
!575 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !541)
!576 = !DISubprogram(name: "is_lock_free", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE12is_lock_freeB9nqn220108Ev", scope: !541, file: !392, line: 55, type: !577, scopeLine: 55, flags: DIFlagPrototyped, spFlags: 0)
!577 = !DISubroutineType(types: !578)
!578 = !{!127, !579}
!579 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !580, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!580 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !541)
!581 = !DISubprogram(name: "store", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE5storeB9nqn220108EiNS_12memory_orderE", scope: !541, file: !392, line: 58, type: !582, scopeLine: 58, flags: DIFlagPrototyped, spFlags: 0)
!582 = !DISubroutineType(types: !583)
!583 = !{null, !584, !88, !441}
!584 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !575, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!585 = !DISubprogram(name: "store", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE5storeB9nqn220108EiNS_12memory_orderE", scope: !541, file: !392, line: 62, type: !586, scopeLine: 62, flags: DIFlagPrototyped, spFlags: 0)
!586 = !DISubroutineType(types: !587)
!587 = !{null, !588, !88, !441}
!588 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !541, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!589 = !DISubprogram(name: "load", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE", scope: !541, file: !392, line: 66, type: !590, scopeLine: 66, flags: DIFlagPrototyped, spFlags: 0)
!590 = !DISubroutineType(types: !591)
!591 = !{!88, !573, !441}
!592 = !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE", scope: !541, file: !392, line: 70, type: !593, scopeLine: 70, flags: DIFlagPrototyped, spFlags: 0)
!593 = !DISubroutineType(types: !594)
!594 = !{!88, !579, !441}
!595 = !DISubprogram(name: "operator int", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EEcviB9nqn220108Ev", scope: !541, file: !392, line: 74, type: !596, scopeLine: 74, flags: DIFlagPrototyped, spFlags: 0)
!596 = !DISubroutineType(types: !597)
!597 = !{!88, !573}
!598 = !DISubprogram(name: "operator int", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EEcviB9nqn220108Ev", scope: !541, file: !392, line: 75, type: !599, scopeLine: 75, flags: DIFlagPrototyped, spFlags: 0)
!599 = !DISubroutineType(types: !600)
!600 = !{!88, !579}
!601 = !DISubprogram(name: "exchange", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE8exchangeB9nqn220108EiNS_12memory_orderE", scope: !541, file: !392, line: 76, type: !602, scopeLine: 76, flags: DIFlagPrototyped, spFlags: 0)
!602 = !DISubroutineType(types: !603)
!603 = !{!88, !584, !88, !441}
!604 = !DISubprogram(name: "exchange", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE8exchangeB9nqn220108EiNS_12memory_orderE", scope: !541, file: !392, line: 79, type: !605, scopeLine: 79, flags: DIFlagPrototyped, spFlags: 0)
!605 = !DISubroutineType(types: !606)
!606 = !{!88, !588, !88, !441}
!607 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqn220108ERiiNS_12memory_orderES3_", scope: !541, file: !392, line: 83, type: !608, scopeLine: 83, flags: DIFlagPrototyped, spFlags: 0)
!608 = !DISubroutineType(types: !609)
!609 = !{!127, !584, !610, !88, !441, !441}
!610 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !88, size: 64)
!611 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqn220108ERiiNS_12memory_orderES3_", scope: !541, file: !392, line: 87, type: !612, scopeLine: 87, flags: DIFlagPrototyped, spFlags: 0)
!612 = !DISubroutineType(types: !613)
!613 = !{!127, !588, !610, !88, !441, !441}
!614 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqn220108ERiiNS_12memory_orderES3_", scope: !541, file: !392, line: 92, type: !608, scopeLine: 92, flags: DIFlagPrototyped, spFlags: 0)
!615 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqn220108ERiiNS_12memory_orderES3_", scope: !541, file: !392, line: 96, type: !612, scopeLine: 96, flags: DIFlagPrototyped, spFlags: 0)
!616 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqn220108ERiiNS_12memory_orderE", scope: !541, file: !392, line: 101, type: !617, scopeLine: 101, flags: DIFlagPrototyped, spFlags: 0)
!617 = !DISubroutineType(types: !618)
!618 = !{!127, !584, !610, !88, !441}
!619 = !DISubprogram(name: "compare_exchange_weak", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE21compare_exchange_weakB9nqn220108ERiiNS_12memory_orderE", scope: !541, file: !392, line: 105, type: !620, scopeLine: 105, flags: DIFlagPrototyped, spFlags: 0)
!620 = !DISubroutineType(types: !621)
!621 = !{!127, !588, !610, !88, !441}
!622 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqn220108ERiiNS_12memory_orderE", scope: !541, file: !392, line: 109, type: !617, scopeLine: 109, flags: DIFlagPrototyped, spFlags: 0)
!623 = !DISubprogram(name: "compare_exchange_strong", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE23compare_exchange_strongB9nqn220108ERiiNS_12memory_orderE", scope: !541, file: !392, line: 113, type: !620, scopeLine: 113, flags: DIFlagPrototyped, spFlags: 0)
!624 = !DISubprogram(name: "wait", linkageName: "_ZNVKSt3__113__atomic_baseIiLb0EE4waitB9nqn220108EiNS_12memory_orderE", scope: !541, file: !392, line: 118, type: !625, scopeLine: 118, flags: DIFlagPrototyped, spFlags: 0)
!625 = !DISubroutineType(types: !626)
!626 = !{null, !573, !88, !441}
!627 = !DISubprogram(name: "wait", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE4waitB9nqn220108EiNS_12memory_orderE", scope: !541, file: !392, line: 121, type: !628, scopeLine: 121, flags: DIFlagPrototyped, spFlags: 0)
!628 = !DISubroutineType(types: !629)
!629 = !{null, !579, !88, !441}
!630 = !DISubprogram(name: "notify_one", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE10notify_oneB9nqn220108Ev", scope: !541, file: !392, line: 124, type: !631, scopeLine: 124, flags: DIFlagPrototyped, spFlags: 0)
!631 = !DISubroutineType(types: !632)
!632 = !{null, !584}
!633 = !DISubprogram(name: "notify_one", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE10notify_oneB9nqn220108Ev", scope: !541, file: !392, line: 125, type: !634, scopeLine: 125, flags: DIFlagPrototyped, spFlags: 0)
!634 = !DISubroutineType(types: !635)
!635 = !{null, !588}
!636 = !DISubprogram(name: "notify_all", linkageName: "_ZNVSt3__113__atomic_baseIiLb0EE10notify_allB9nqn220108Ev", scope: !541, file: !392, line: 126, type: !631, scopeLine: 126, flags: DIFlagPrototyped, spFlags: 0)
!637 = !DISubprogram(name: "notify_all", linkageName: "_ZNSt3__113__atomic_baseIiLb0EE10notify_allB9nqn220108Ev", scope: !541, file: !392, line: 127, type: !634, scopeLine: 127, flags: DIFlagPrototyped, spFlags: 0)
!638 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb0EEC4B9nqn220108Ev", scope: !541, file: !392, line: 131, type: !634, scopeLine: 131, flags: DIFlagPrototyped, spFlags: 0)
!639 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb0EEC4B9nqn220108Ei", scope: !541, file: !392, line: 136, type: !640, scopeLine: 136, flags: DIFlagPrototyped, spFlags: 0)
!640 = !DISubroutineType(types: !641)
!641 = !{null, !588, !88}
!642 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb0EEC4ERKS1_", scope: !541, file: !392, line: 138, type: !643, scopeLine: 138, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!643 = !DISubroutineType(types: !644)
!644 = !{null, !588, !645}
!645 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !580, size: 64)
!646 = !{!559, !647}
!647 = !DITemplateValueParameter(type: !127, value: i1 false)
!648 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEC4B9nqn220108Ev", scope: !538, file: !392, line: 149, type: !649, scopeLine: 149, flags: DIFlagPrototyped, spFlags: 0)
!649 = !DISubroutineType(types: !650)
!650 = !{null, !651}
!651 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !538, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!652 = !DISubprogram(name: "__atomic_base", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEC4B9nqn220108Ei", scope: !538, file: !392, line: 151, type: !653, scopeLine: 151, flags: DIFlagPrototyped, spFlags: 0)
!653 = !DISubroutineType(types: !654)
!654 = !{null, !651, !88}
!655 = !DISubprogram(name: "fetch_add", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_addB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 153, type: !656, scopeLine: 153, flags: DIFlagPrototyped, spFlags: 0)
!656 = !DISubroutineType(types: !657)
!657 = !{!88, !658, !88, !441}
!658 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !659, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!659 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !538)
!660 = !DISubprogram(name: "fetch_add", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 156, type: !661, scopeLine: 156, flags: DIFlagPrototyped, spFlags: 0)
!661 = !DISubroutineType(types: !662)
!662 = !{!88, !651, !88, !441}
!663 = !DISubprogram(name: "fetch_sub", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_subB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 159, type: !656, scopeLine: 159, flags: DIFlagPrototyped, spFlags: 0)
!664 = !DISubprogram(name: "fetch_sub", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_subB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 162, type: !661, scopeLine: 162, flags: DIFlagPrototyped, spFlags: 0)
!665 = !DISubprogram(name: "fetch_and", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_andB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 165, type: !656, scopeLine: 165, flags: DIFlagPrototyped, spFlags: 0)
!666 = !DISubprogram(name: "fetch_and", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_andB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 168, type: !661, scopeLine: 168, flags: DIFlagPrototyped, spFlags: 0)
!667 = !DISubprogram(name: "fetch_or", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE8fetch_orB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 171, type: !656, scopeLine: 171, flags: DIFlagPrototyped, spFlags: 0)
!668 = !DISubprogram(name: "fetch_or", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE8fetch_orB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 174, type: !661, scopeLine: 174, flags: DIFlagPrototyped, spFlags: 0)
!669 = !DISubprogram(name: "fetch_xor", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EE9fetch_xorB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 177, type: !656, scopeLine: 177, flags: DIFlagPrototyped, spFlags: 0)
!670 = !DISubprogram(name: "fetch_xor", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_xorB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 180, type: !661, scopeLine: 180, flags: DIFlagPrototyped, spFlags: 0)
!671 = !DISubprogram(name: "operator++", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEppB9nqn220108Ei", scope: !538, file: !392, line: 184, type: !672, scopeLine: 184, flags: DIFlagPrototyped, spFlags: 0)
!672 = !DISubroutineType(types: !673)
!673 = !{!88, !658, !88}
!674 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEppB9nqn220108Ei", scope: !538, file: !392, line: 185, type: !675, scopeLine: 185, flags: DIFlagPrototyped, spFlags: 0)
!675 = !DISubroutineType(types: !676)
!676 = !{!88, !651, !88}
!677 = !DISubprogram(name: "operator--", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEmmB9nqn220108Ei", scope: !538, file: !392, line: 186, type: !672, scopeLine: 186, flags: DIFlagPrototyped, spFlags: 0)
!678 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEmmB9nqn220108Ei", scope: !538, file: !392, line: 187, type: !675, scopeLine: 187, flags: DIFlagPrototyped, spFlags: 0)
!679 = !DISubprogram(name: "operator++", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEppB9nqn220108Ev", scope: !538, file: !392, line: 188, type: !680, scopeLine: 188, flags: DIFlagPrototyped, spFlags: 0)
!680 = !DISubroutineType(types: !681)
!681 = !{!88, !658}
!682 = !DISubprogram(name: "operator++", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEppB9nqn220108Ev", scope: !538, file: !392, line: 189, type: !683, scopeLine: 189, flags: DIFlagPrototyped, spFlags: 0)
!683 = !DISubroutineType(types: !684)
!684 = !{!88, !651}
!685 = !DISubprogram(name: "operator--", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEmmB9nqn220108Ev", scope: !538, file: !392, line: 190, type: !680, scopeLine: 190, flags: DIFlagPrototyped, spFlags: 0)
!686 = !DISubprogram(name: "operator--", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEmmB9nqn220108Ev", scope: !538, file: !392, line: 191, type: !683, scopeLine: 191, flags: DIFlagPrototyped, spFlags: 0)
!687 = !DISubprogram(name: "operator+=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEpLB9nqn220108Ei", scope: !538, file: !392, line: 192, type: !672, scopeLine: 192, flags: DIFlagPrototyped, spFlags: 0)
!688 = !DISubprogram(name: "operator+=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEpLB9nqn220108Ei", scope: !538, file: !392, line: 193, type: !675, scopeLine: 193, flags: DIFlagPrototyped, spFlags: 0)
!689 = !DISubprogram(name: "operator-=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEmIB9nqn220108Ei", scope: !538, file: !392, line: 194, type: !672, scopeLine: 194, flags: DIFlagPrototyped, spFlags: 0)
!690 = !DISubprogram(name: "operator-=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEmIB9nqn220108Ei", scope: !538, file: !392, line: 195, type: !675, scopeLine: 195, flags: DIFlagPrototyped, spFlags: 0)
!691 = !DISubprogram(name: "operator&=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEaNB9nqn220108Ei", scope: !538, file: !392, line: 196, type: !672, scopeLine: 196, flags: DIFlagPrototyped, spFlags: 0)
!692 = !DISubprogram(name: "operator&=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEaNB9nqn220108Ei", scope: !538, file: !392, line: 197, type: !675, scopeLine: 197, flags: DIFlagPrototyped, spFlags: 0)
!693 = !DISubprogram(name: "operator|=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEoRB9nqn220108Ei", scope: !538, file: !392, line: 198, type: !672, scopeLine: 198, flags: DIFlagPrototyped, spFlags: 0)
!694 = !DISubprogram(name: "operator|=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEoRB9nqn220108Ei", scope: !538, file: !392, line: 199, type: !675, scopeLine: 199, flags: DIFlagPrototyped, spFlags: 0)
!695 = !DISubprogram(name: "operator^=", linkageName: "_ZNVSt3__113__atomic_baseIiLb1EEeOB9nqn220108Ei", scope: !538, file: !392, line: 200, type: !672, scopeLine: 200, flags: DIFlagPrototyped, spFlags: 0)
!696 = !DISubprogram(name: "operator^=", linkageName: "_ZNSt3__113__atomic_baseIiLb1EEeOB9nqn220108Ei", scope: !538, file: !392, line: 201, type: !675, scopeLine: 201, flags: DIFlagPrototyped, spFlags: 0)
!697 = !{!559, !698}
!698 = !DITemplateValueParameter(type: !127, defaulted: true, value: i1 true)
!699 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIiEC4B9nqn220108Ev", scope: !535, file: !392, line: 246, type: !700, scopeLine: 246, flags: DIFlagPrototyped, spFlags: 0)
!700 = !DISubroutineType(types: !701)
!701 = !{null, !702}
!702 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !535, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!703 = !DISubprogram(name: "atomic", linkageName: "_ZNSt3__16atomicIiEC4B9nqn220108Ei", scope: !535, file: !392, line: 251, type: !704, scopeLine: 251, flags: DIFlagPrototyped, spFlags: 0)
!704 = !DISubroutineType(types: !705)
!705 = !{null, !702, !88}
!706 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIiEaSB9nqn220108Ei", scope: !535, file: !392, line: 253, type: !707, scopeLine: 253, flags: DIFlagPrototyped, spFlags: 0)
!707 = !DISubroutineType(types: !708)
!708 = !{!88, !709, !88}
!709 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !710, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!710 = !DIDerivedType(tag: DW_TAG_volatile_type, baseType: !535)
!711 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIiEaSB9nqn220108Ei", scope: !535, file: !392, line: 257, type: !712, scopeLine: 257, flags: DIFlagPrototyped, spFlags: 0)
!712 = !DISubroutineType(types: !713)
!713 = !{!88, !702, !88}
!714 = !DISubprogram(name: "operator=", linkageName: "_ZNSt3__16atomicIiEaSERKS1_", scope: !535, file: !392, line: 262, type: !715, scopeLine: 262, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!715 = !DISubroutineType(types: !716)
!716 = !{!717, !702, !718}
!717 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !535, size: 64)
!718 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !719, size: 64)
!719 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !535)
!720 = !DISubprogram(name: "operator=", linkageName: "_ZNVSt3__16atomicIiEaSERKS1_", scope: !535, file: !392, line: 263, type: !721, scopeLine: 263, flags: DIFlagPrototyped, spFlags: DISPFlagDeleted)
!721 = !DISubroutineType(types: !722)
!722 = !{!717, !709, !718}
!723 = !DIGlobalVariableExpression(var: !724, expr: !DIExpression())
!724 = distinct !DIGlobalVariable(scope: null, file: !3, line: 36, type: !725, isLocal: true, isDefinition: true)
!725 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 408, elements: !726)
!726 = !{!727}
!727 = !DISubrange(count: 51)
!728 = !DIGlobalVariableExpression(var: !729, expr: !DIExpression())
!729 = distinct !DIGlobalVariable(scope: null, file: !3, line: 50, type: !357, isLocal: true, isDefinition: true)
!730 = !DIGlobalVariableExpression(var: !731, expr: !DIExpression())
!731 = distinct !DIGlobalVariable(scope: null, file: !3, line: 52, type: !732, isLocal: true, isDefinition: true)
!732 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 192, elements: !733)
!733 = !{!734}
!734 = !DISubrange(count: 24)
!735 = !DIGlobalVariableExpression(var: !736, expr: !DIExpression())
!736 = distinct !DIGlobalVariable(name: "ready", linkageName: "_ZN12_GLOBAL__N_15readyE", scope: !2, file: !3, line: 16, type: !391, isLocal: true, isDefinition: true)
!737 = !DIGlobalVariableExpression(var: !738, expr: !DIExpression())
!738 = distinct !DIGlobalVariable(name: "frame_cleanup_count", linkageName: "_ZN12_GLOBAL__N_119frame_cleanup_countE", scope: !2, file: !3, line: 19, type: !535, isLocal: true, isDefinition: true)
!739 = !DIGlobalVariableExpression(var: !740, expr: !DIExpression())
!740 = distinct !DIGlobalVariable(scope: null, file: !3, line: 65, type: !741, isLocal: true, isDefinition: true)
!741 = !DICompositeType(tag: DW_TAG_array_type, baseType: !350, size: 432, elements: !742)
!742 = !{!743}
!743 = !DISubrange(count: 54)
!744 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "clang version 22.1.8", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !745, retainedTypes: !755, globals: !775, imports: !783, splitDebugInlining: false, nameTableKind: None)
!745 = !{!441, !746, !751}
!746 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !747, line: 168, baseType: !31, size: 32, elements: !748)
!747 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/pthread.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "2b60be7fa73249bcf83f3192783962e1")
!748 = !{!749, !750}
!749 = !DIEnumerator(name: "PTHREAD_CANCEL_ENABLE", value: 0, isUnsigned: true)
!750 = !DIEnumerator(name: "PTHREAD_CANCEL_DISABLE", value: 1, isUnsigned: true)
!751 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !747, line: 175, baseType: !31, size: 32, elements: !752)
!752 = !{!753, !754}
!753 = !DIEnumerator(name: "PTHREAD_CANCEL_DEFERRED", value: 0, isUnsigned: true)
!754 = !DIEnumerator(name: "PTHREAD_CANCEL_ASYNCHRONOUS", value: 1, isUnsigned: true)
!755 = !{!342, !756, !88, !757, !758, !31, !761, !768, !4, !226, !222, !402, !398, !395, !391, !547, !544, !541, !538, !535, !75, !69, !146, !156, !165}
!756 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!757 = !DIDerivedType(tag: DW_TAG_typedef, name: "intptr_t", file: !286, line: 76, baseType: !108)
!758 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !759, line: 18, baseType: !760)
!759 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/lib64/clang/22/include/__stddef_size_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "2c44e821a2b1951cde2eb0fb2e656867")
!760 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!761 = !DIDerivedType(tag: DW_TAG_typedef, name: "__ptr_type", scope: !762, file: !403, line: 80, baseType: !767)
!762 = distinct !DISubprogram(name: "__cxx_atomic_load<int>", linkageName: "_ZNSt3__117__cxx_atomic_loadB9nqn220108IiEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE", scope: !6, file: !403, line: 79, type: !763, scopeLine: 79, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, templateParams: !558, retainedNodes: !246)
!763 = !DISubroutineType(types: !764)
!764 = !{!88, !765, !441}
!765 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !766, size: 64)
!766 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !547)
!767 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !550, size: 64)
!768 = !DIDerivedType(tag: DW_TAG_typedef, name: "__ptr_type", scope: !769, file: !403, line: 80, baseType: !774)
!769 = distinct !DISubprogram(name: "__cxx_atomic_load<bool>", linkageName: "_ZNSt3__117__cxx_atomic_loadB9nqn220108IbEET_PKNS_22__cxx_atomic_base_implIS1_EENS_12memory_orderE", scope: !6, file: !403, line: 79, type: !770, scopeLine: 79, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, templateParams: !414, retainedNodes: !246)
!770 = !DISubroutineType(types: !771)
!771 = !{!127, !772, !441}
!772 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !773, size: 64)
!773 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !402)
!774 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !406, size: 64)
!775 = !{!0, !347, !776, !355, !362, !779, !367, !781, !369, !374, !379, !384, !340, !389, !353, !533, !723, !360, !728, !730, !735, !737, !739}
!776 = !DIGlobalVariableExpression(var: !777, expr: !DIExpression(DW_OP_constu, 0, DW_OP_stack_value))
!777 = distinct !DIGlobalVariable(name: "memory_order_relaxed", scope: !6, file: !442, line: 43, type: !778, isLocal: true, isDefinition: true)
!778 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !441)
!779 = !DIGlobalVariableExpression(var: !780, expr: !DIExpression(DW_OP_constu, 2, DW_OP_stack_value))
!780 = distinct !DIGlobalVariable(name: "memory_order_acquire", scope: !6, file: !442, line: 45, type: !778, isLocal: true, isDefinition: true)
!781 = !DIGlobalVariableExpression(var: !782, expr: !DIExpression(DW_OP_constu, 3, DW_OP_stack_value))
!782 = distinct !DIGlobalVariable(name: "memory_order_release", scope: !6, file: !442, line: 46, type: !778, isLocal: true, isDefinition: true)
!783 = !{!784, !790, !793, !796, !799, !804, !808, !811, !814, !818, !821, !824, !827, !830, !833, !836, !839, !841, !843, !845, !847, !849, !851, !853, !855, !856, !858, !859, !862, !872, !876, !884, !888, !890, !892, !896, !900, !904, !906, !910, !914, !918, !922, !926, !928, !930, !932, !934, !938, !942, !946, !949, !954, !958, !961, !968, !973, !978, !983, !988, !994, !1000, !1004, !1006, !1011, !1016, !1024, !1026, !1028, !1030, !1036, !1038, !1039, !1040, !1041, !1045, !1047, !1052, !1054, !1056, !1060, !1062, !1064, !1066, !1068, !1070, !1072, !1074, !1079, !1083, !1085, !1087, !1088, !1093, !1095, !1097, !1099, !1101, !1103, !1105, !1107, !1109, !1111, !1113, !1115, !1117, !1119, !1121, !1123, !1125, !1129, !1131, !1133, !1135, !1139, !1141, !1145, !1147, !1149, !1151, !1153, !1157, !1159, !1163, !1167, !1169, !1171, !1175, !1177, !1181, !1183, !1185, !1189, !1191, !1193, !1195, !1199, !1201, !1203, !1207, !1209, !1211, !1213, !1215, !1217, !1219, !1221, !1225, !1229, !1231, !1233, !1235, !1237, !1239, !1241, !1243, !1245, !1247, !1249, !1251, !1253, !1255, !1257, !1259, !1261, !1263, !1265, !1267, !1271, !1273, !1275, !1277, !1281, !1283, !1287, !1289, !1291, !1293, !1295, !1299, !1301, !1305, !1307, !1309, !1311, !1313, !1317, !1319, !1321, !1325, !1327, !1329, !1331, !1339, !1344, !1350, !1356, !1358, !1362, !1366, !1370, !1376, !1380, !1384, !1388, !1392, !1396, !1400, !1404, !1408, !1412, !1416, !1420, !1424, !1428, !1433, !1437, !1439, !1443, !1445, !1453, !1457, !1461, !1465, !1469, !1474, !1478, !1480, !1484, !1491, !1495, !1499, !1506, !1508, !1510, !1512, !1529, !1533, !1535, !1537, !1539, !1541, !1543, !1545, !1547, !1549, !1551, !1553, !1555, !1557, !1559, !1563, !1568, !1571, !1575, !1577, !1579, !1581, !1583, !1585, !1587, !1589, !1591, !1593, !1595, !1597, !1601, !1605, !1609, !1611, !1615, !1619, !1621, !1622, !1623, !1628, !1636, !1638, !1642, !1648, !1652, !1656, !1658, !1662, !1666, !1670, !1674, !1678, !1682, !1684, !1686, !1690, !1696, !1700, !1704, !1708, !1712, !1716, !1720, !1724, !1728, !1730, !1732, !1736, !1738, !1742, !1746, !1750, !1754, !1756, !1758, !1762, !1766, !1770, !1772, !1776, !1778, !1780, !1784, !1788, !1792, !1796, !1798, !1804, !1810, !1814, !1818, !1824, !1830, !1834, !1838, !1842, !1846, !1848, !1850, !1852, !1858, !1863, !1865, !1870, !1874, !1878, !1880, !1884, !1888, !1892, !1896, !1898, !1902, !1906, !1910, !1912, !1916, !1920, !1924, !1926, !1928, !1930, !1934, !1938, !1944, !1948, !1954, !1958, !1962, !1964, !1966, !1968, !1972, !1976, !1980, !1982, !1984, !1988, !1992, !1994, !1998, !2002, !2004, !2006, !2008}
!784 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !785, file: !789, line: 158)
!785 = !DIDerivedType(tag: DW_TAG_typedef, name: "int8_t", file: !786, line: 24, baseType: !787)
!786 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/stdint-intn.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "649b383a60bfa3eb90e85840b2b0be20")
!787 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int8_t", file: !288, line: 37, baseType: !788)
!788 = !DIBasicType(name: "signed char", size: 8, encoding: DW_ATE_signed_char)
!789 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstdint", directory: "/home/toolchain/development")
!790 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !791, file: !789, line: 159)
!791 = !DIDerivedType(tag: DW_TAG_typedef, name: "int16_t", file: !786, line: 25, baseType: !792)
!792 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int16_t", file: !288, line: 39, baseType: !94)
!793 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !794, file: !789, line: 160)
!794 = !DIDerivedType(tag: DW_TAG_typedef, name: "int32_t", file: !786, line: 26, baseType: !795)
!795 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int32_t", file: !288, line: 41, baseType: !88)
!796 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !797, file: !789, line: 161)
!797 = !DIDerivedType(tag: DW_TAG_typedef, name: "int64_t", file: !786, line: 27, baseType: !798)
!798 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int64_t", file: !288, line: 44, baseType: !108)
!799 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !800, file: !789, line: 163)
!800 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !801, line: 24, baseType: !802)
!801 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/stdint-uintn.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "256fcabbefa27ca8cf5e6d37525e6e16")
!802 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint8_t", file: !288, line: 38, baseType: !803)
!803 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!804 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !805, file: !789, line: 164)
!805 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint16_t", file: !801, line: 25, baseType: !806)
!806 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint16_t", file: !288, line: 40, baseType: !807)
!807 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!808 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !809, file: !789, line: 165)
!809 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint32_t", file: !801, line: 26, baseType: !810)
!810 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint32_t", file: !288, line: 42, baseType: !31)
!811 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !812, file: !789, line: 166)
!812 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint64_t", file: !801, line: 27, baseType: !813)
!813 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint64_t", file: !288, line: 45, baseType: !760)
!814 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !815, file: !789, line: 168)
!815 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least8_t", file: !816, line: 25, baseType: !817)
!816 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/stdint-least.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "a866be81c480920b0293bd5f6336a0a3")
!817 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least8_t", file: !288, line: 52, baseType: !787)
!818 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !819, file: !789, line: 169)
!819 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least16_t", file: !816, line: 26, baseType: !820)
!820 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least16_t", file: !288, line: 54, baseType: !792)
!821 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !822, file: !789, line: 170)
!822 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least32_t", file: !816, line: 27, baseType: !823)
!823 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least32_t", file: !288, line: 56, baseType: !795)
!824 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !825, file: !789, line: 171)
!825 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_least64_t", file: !816, line: 28, baseType: !826)
!826 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int_least64_t", file: !288, line: 58, baseType: !798)
!827 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !828, file: !789, line: 173)
!828 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least8_t", file: !816, line: 31, baseType: !829)
!829 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least8_t", file: !288, line: 53, baseType: !802)
!830 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !831, file: !789, line: 174)
!831 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least16_t", file: !816, line: 32, baseType: !832)
!832 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least16_t", file: !288, line: 55, baseType: !806)
!833 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !834, file: !789, line: 175)
!834 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least32_t", file: !816, line: 33, baseType: !835)
!835 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least32_t", file: !288, line: 57, baseType: !810)
!836 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !837, file: !789, line: 176)
!837 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_least64_t", file: !816, line: 34, baseType: !838)
!838 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uint_least64_t", file: !288, line: 59, baseType: !813)
!839 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !840, file: !789, line: 178)
!840 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast8_t", file: !286, line: 47, baseType: !788)
!841 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !842, file: !789, line: 179)
!842 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast16_t", file: !286, line: 49, baseType: !108)
!843 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !844, file: !789, line: 180)
!844 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast32_t", file: !286, line: 50, baseType: !108)
!845 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !846, file: !789, line: 181)
!846 = !DIDerivedType(tag: DW_TAG_typedef, name: "int_fast64_t", file: !286, line: 51, baseType: !108)
!847 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !848, file: !789, line: 183)
!848 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast8_t", file: !286, line: 60, baseType: !803)
!849 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !850, file: !789, line: 184)
!850 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast16_t", file: !286, line: 62, baseType: !760)
!851 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !852, file: !789, line: 185)
!852 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast32_t", file: !286, line: 63, baseType: !760)
!853 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !854, file: !789, line: 186)
!854 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint_fast64_t", file: !286, line: 64, baseType: !760)
!855 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !757, file: !789, line: 188)
!856 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !857, file: !789, line: 189)
!857 = !DIDerivedType(tag: DW_TAG_typedef, name: "uintptr_t", file: !286, line: 79, baseType: !760)
!858 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !285, file: !789, line: 191)
!859 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !860, file: !789, line: 192)
!860 = !DIDerivedType(tag: DW_TAG_typedef, name: "uintmax_t", file: !286, line: 91, baseType: !861)
!861 = !DIDerivedType(tag: DW_TAG_typedef, name: "__uintmax_t", file: !288, line: 73, baseType: !760)
!862 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !863, file: !871, line: 82)
!863 = !DISubprogram(name: "memcpy", scope: !864, file: !864, line: 43, type: !865, flags: DIFlagPrototyped, spFlags: 0)
!864 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/string.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "b395f57719fc1ba9602e6cb95fab8598")
!865 = !DISubroutineType(types: !866)
!866 = !{!756, !867, !868, !758}
!867 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !756)
!868 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !869)
!869 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !870, size: 64)
!870 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!871 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstring", directory: "/home/toolchain/development")
!872 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !873, file: !871, line: 83)
!873 = !DISubprogram(name: "memmove", scope: !864, file: !864, line: 47, type: !874, flags: DIFlagPrototyped, spFlags: 0)
!874 = !DISubroutineType(types: !875)
!875 = !{!756, !756, !869, !758}
!876 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !877, file: !871, line: 84)
!877 = !DISubprogram(name: "strcpy", scope: !864, file: !864, line: 141, type: !878, flags: DIFlagPrototyped, spFlags: 0)
!878 = !DISubroutineType(types: !879)
!879 = !{!880, !881, !882}
!880 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !44, size: 64)
!881 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !880)
!882 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !883)
!883 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !350, size: 64)
!884 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !885, file: !871, line: 85)
!885 = !DISubprogram(name: "strncpy", scope: !864, file: !864, line: 144, type: !886, flags: DIFlagPrototyped, spFlags: 0)
!886 = !DISubroutineType(types: !887)
!887 = !{!880, !881, !882, !758}
!888 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !889, file: !871, line: 86)
!889 = !DISubprogram(name: "strcat", scope: !864, file: !864, line: 149, type: !878, flags: DIFlagPrototyped, spFlags: 0)
!890 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !891, file: !871, line: 87)
!891 = !DISubprogram(name: "strncat", scope: !864, file: !864, line: 152, type: !886, flags: DIFlagPrototyped, spFlags: 0)
!892 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !893, file: !871, line: 88)
!893 = !DISubprogram(name: "memcmp", scope: !864, file: !864, line: 64, type: !894, flags: DIFlagPrototyped, spFlags: 0)
!894 = !DISubroutineType(types: !895)
!895 = !{!88, !869, !869, !758}
!896 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !897, file: !871, line: 89)
!897 = !DISubprogram(name: "strcmp", scope: !864, file: !864, line: 156, type: !898, flags: DIFlagPrototyped, spFlags: 0)
!898 = !DISubroutineType(types: !899)
!899 = !{!88, !883, !883}
!900 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !901, file: !871, line: 90)
!901 = !DISubprogram(name: "strncmp", scope: !864, file: !864, line: 159, type: !902, flags: DIFlagPrototyped, spFlags: 0)
!902 = !DISubroutineType(types: !903)
!903 = !{!88, !883, !883, !758}
!904 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !905, file: !871, line: 91)
!905 = !DISubprogram(name: "strcoll", scope: !864, file: !864, line: 163, type: !898, flags: DIFlagPrototyped, spFlags: 0)
!906 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !907, file: !871, line: 92)
!907 = !DISubprogram(name: "strxfrm", scope: !864, file: !864, line: 166, type: !908, flags: DIFlagPrototyped, spFlags: 0)
!908 = !DISubroutineType(types: !909)
!909 = !{!758, !881, !882, !758}
!910 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !911, file: !871, line: 93)
!911 = !DISubprogram(name: "memchr", scope: !864, file: !864, line: 89, type: !912, flags: DIFlagPrototyped, spFlags: 0)
!912 = !DISubroutineType(types: !913)
!913 = !{!869, !869, !88, !758}
!914 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !915, file: !871, line: 94)
!915 = !DISubprogram(name: "strchr", scope: !864, file: !864, line: 228, type: !916, flags: DIFlagPrototyped, spFlags: 0)
!916 = !DISubroutineType(types: !917)
!917 = !{!883, !883, !88}
!918 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !919, file: !871, line: 95)
!919 = !DISubprogram(name: "strcspn", scope: !864, file: !864, line: 293, type: !920, flags: DIFlagPrototyped, spFlags: 0)
!920 = !DISubroutineType(types: !921)
!921 = !{!758, !883, !883}
!922 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !923, file: !871, line: 96)
!923 = !DISubprogram(name: "strpbrk", scope: !864, file: !864, line: 305, type: !924, flags: DIFlagPrototyped, spFlags: 0)
!924 = !DISubroutineType(types: !925)
!925 = !{!883, !883, !883}
!926 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !927, file: !871, line: 97)
!927 = !DISubprogram(name: "strrchr", scope: !864, file: !864, line: 255, type: !916, flags: DIFlagPrototyped, spFlags: 0)
!928 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !929, file: !871, line: 98)
!929 = !DISubprogram(name: "strspn", scope: !864, file: !864, line: 297, type: !920, flags: DIFlagPrototyped, spFlags: 0)
!930 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !931, file: !871, line: 99)
!931 = !DISubprogram(name: "strstr", scope: !864, file: !864, line: 332, type: !924, flags: DIFlagPrototyped, spFlags: 0)
!932 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !933, file: !871, line: 100)
!933 = !DISubprogram(name: "strtok", scope: !864, file: !864, line: 356, type: !878, flags: DIFlagPrototyped, spFlags: 0)
!934 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !935, file: !871, line: 101)
!935 = !DISubprogram(name: "memset", scope: !864, file: !864, line: 61, type: !936, flags: DIFlagPrototyped, spFlags: 0)
!936 = !DISubroutineType(types: !937)
!937 = !{!756, !756, !88, !758}
!938 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !939, file: !871, line: 102)
!939 = !DISubprogram(name: "strerror", scope: !864, file: !864, line: 419, type: !940, flags: DIFlagPrototyped, spFlags: 0)
!940 = !DISubroutineType(types: !941)
!941 = !{!880, !88}
!942 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !943, file: !871, line: 103)
!943 = !DISubprogram(name: "strlen", scope: !864, file: !864, line: 407, type: !944, flags: DIFlagPrototyped, spFlags: 0)
!944 = !DISubroutineType(types: !945)
!945 = !{!758, !883}
!946 = !DIImportedEntity(tag: DW_TAG_imported_module, scope: !218, entity: !947, file: !227, line: 549)
!947 = !DINamespace(name: "chrono_literals", scope: !948, exportSymbols: true)
!948 = !DINamespace(name: "literals", scope: !6, exportSymbols: true)
!949 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !950, file: !953, line: 68)
!950 = !DIDerivedType(tag: DW_TAG_typedef, name: "clock_t", file: !951, line: 7, baseType: !952)
!951 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/clock_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "1aade99fd778d1551600c7ca1410b9f1")
!952 = !DIDerivedType(tag: DW_TAG_typedef, name: "__clock_t", file: !288, line: 156, baseType: !108)
!953 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/ctime", directory: "/home/toolchain/development")
!954 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !955, file: !953, line: 69)
!955 = !DIDerivedType(tag: DW_TAG_typedef, name: "time_t", file: !956, line: 10, baseType: !957)
!956 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/time_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "34d0816fa9aad5db7f457b5364ebec5f")
!957 = !DIDerivedType(tag: DW_TAG_typedef, name: "__time_t", file: !288, line: 160, baseType: !108)
!958 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !959, file: !953, line: 70)
!959 = !DICompositeType(tag: DW_TAG_structure_type, name: "tm", file: !960, line: 7, size: 448, flags: DIFlagFwdDecl, identifier: "_ZTS2tm")
!960 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/struct_tm.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "9e5545b565ef031c4cd0faf90b69386f")
!961 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !962, file: !953, line: 72)
!962 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "timespec", file: !963, line: 11, size: 128, flags: DIFlagTypePassByValue, elements: !964, identifier: "_ZTS8timespec")
!963 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/struct_timespec.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "9378e9ebbd658baccf881d3300eb1828")
!964 = !{!965, !966}
!965 = !DIDerivedType(tag: DW_TAG_member, name: "tv_sec", scope: !962, file: !963, line: 16, baseType: !957, size: 64)
!966 = !DIDerivedType(tag: DW_TAG_member, name: "tv_nsec", scope: !962, file: !963, line: 21, baseType: !967, size: 64, offset: 64)
!967 = !DIDerivedType(tag: DW_TAG_typedef, name: "__syscall_slong_t", file: !288, line: 197, baseType: !108)
!968 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !969, file: !953, line: 74)
!969 = !DISubprogram(name: "clock", scope: !970, file: !970, line: 72, type: !971, flags: DIFlagPrototyped, spFlags: 0)
!970 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/time.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "fc01762aedf7abce7804961f8acfd6e7")
!971 = !DISubroutineType(types: !972)
!972 = !{!950}
!973 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !974, file: !953, line: 75)
!974 = !DISubprogram(name: "difftime", scope: !970, file: !970, line: 79, type: !975, flags: DIFlagPrototyped, spFlags: 0)
!975 = !DISubroutineType(types: !976)
!976 = !{!977, !955, !955}
!977 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!978 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !979, file: !953, line: 76)
!979 = !DISubprogram(name: "mktime", scope: !970, file: !970, line: 82, type: !980, flags: DIFlagPrototyped, spFlags: 0)
!980 = !DISubroutineType(types: !981)
!981 = !{!955, !982}
!982 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !959, size: 64)
!983 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !984, file: !953, line: 77)
!984 = !DISubprogram(name: "time", scope: !970, file: !970, line: 76, type: !985, flags: DIFlagPrototyped, spFlags: 0)
!985 = !DISubroutineType(types: !986)
!986 = !{!955, !987}
!987 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !955, size: 64)
!988 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !989, file: !953, line: 78)
!989 = !DISubprogram(name: "asctime", scope: !970, file: !970, line: 179, type: !990, flags: DIFlagPrototyped, spFlags: 0)
!990 = !DISubroutineType(types: !991)
!991 = !{!880, !992}
!992 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !993, size: 64)
!993 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !959)
!994 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !995, file: !953, line: 79)
!995 = !DISubprogram(name: "ctime", scope: !970, file: !970, line: 183, type: !996, flags: DIFlagPrototyped, spFlags: 0)
!996 = !DISubroutineType(types: !997)
!997 = !{!880, !998}
!998 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !999, size: 64)
!999 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !955)
!1000 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1001, file: !953, line: 80)
!1001 = !DISubprogram(name: "gmtime", scope: !970, file: !970, line: 132, type: !1002, flags: DIFlagPrototyped, spFlags: 0)
!1002 = !DISubroutineType(types: !1003)
!1003 = !{!982, !998}
!1004 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1005, file: !953, line: 81)
!1005 = !DISubprogram(name: "localtime", scope: !970, file: !970, line: 136, type: !1002, flags: DIFlagPrototyped, spFlags: 0)
!1006 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1007, file: !953, line: 82)
!1007 = !DISubprogram(name: "strftime", scope: !970, file: !970, line: 99, type: !1008, flags: DIFlagPrototyped, spFlags: 0)
!1008 = !DISubroutineType(types: !1009)
!1009 = !{!758, !881, !758, !882, !1010}
!1010 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !992)
!1011 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1012, file: !953, line: 84)
!1012 = !DISubprogram(name: "timespec_get", scope: !970, file: !970, line: 383, type: !1013, flags: DIFlagPrototyped, spFlags: 0)
!1013 = !DISubroutineType(types: !1014)
!1014 = !{!88, !1015, !88}
!1015 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !962, size: 64)
!1016 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !744, entity: !1017, file: !1023, line: 420)
!1017 = !DISubprogram(name: "isfinite", linkageName: "_ZNSt3__16__math8isfiniteB9nqn220108Ee", scope: !1019, file: !1018, line: 66, type: !1020, flags: DIFlagPrototyped, spFlags: 0)
!1018 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__math/traits.h", directory: "/home/toolchain/development")
!1019 = !DINamespace(name: "__math", scope: !6)
!1020 = !DISubroutineType(types: !1021)
!1021 = !{!127, !1022}
!1022 = !DIBasicType(name: "long double", size: 128, encoding: DW_ATE_float)
!1023 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/math.h", directory: "/home/toolchain/development")
!1024 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !744, entity: !1025, file: !1023, line: 423)
!1025 = !DISubprogram(name: "isinf", linkageName: "_ZNSt3__16__math5isinfB9nqn220108Ee", scope: !1019, file: !1018, line: 90, type: !1020, flags: DIFlagPrototyped, spFlags: 0)
!1026 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !744, entity: !1027, file: !1023, line: 427)
!1027 = !DISubprogram(name: "isnan", linkageName: "_ZNSt3__16__math5isnanB9nqn220108Ee", scope: !1019, file: !1018, line: 114, type: !1020, flags: DIFlagPrototyped, spFlags: 0)
!1028 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !744, entity: !1029, file: !1023, line: 428)
!1029 = !DISubprogram(name: "isnormal", linkageName: "_ZNSt3__16__math8isnormalB9nqn220108Ee", scope: !1019, file: !1018, line: 133, type: !1020, flags: DIFlagPrototyped, spFlags: 0)
!1030 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !744, entity: !1031, file: !1023, line: 495)
!1031 = !DISubprogram(name: "modf", linkageName: "_ZNSt3__16__math4modfB9nqn220108EePe", scope: !1019, file: !1032, line: 55, type: !1033, flags: DIFlagPrototyped, spFlags: 0)
!1032 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__math/modulo.h", directory: "/home/toolchain/development")
!1033 = !DISubroutineType(types: !1034)
!1034 = !{!1022, !1022, !1035}
!1035 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1022, size: 64)
!1036 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1017, file: !1037, line: 352)
!1037 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cmath", directory: "/home/toolchain/development")
!1038 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1025, file: !1037, line: 353)
!1039 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1027, file: !1037, line: 354)
!1040 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1029, file: !1037, line: 355)
!1041 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1042, file: !1037, line: 364)
!1042 = !DIDerivedType(tag: DW_TAG_typedef, name: "float_t", file: !1043, line: 163, baseType: !1044)
!1043 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/math.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "0977e8e2fb84ef94cb89ae31455a7c7d")
!1044 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!1045 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1046, file: !1037, line: 365)
!1046 = !DIDerivedType(tag: DW_TAG_typedef, name: "double_t", file: !1043, line: 164, baseType: !977)
!1047 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1048, file: !1037, line: 370)
!1048 = !DISubprogram(name: "acosf", scope: !1049, file: !1049, line: 53, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1049 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/mathcalls.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "805f19fbe3cd3730c6c76acca0f9374c")
!1050 = !DISubroutineType(types: !1051)
!1051 = !{!1044, !1044}
!1052 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1053, file: !1037, line: 372)
!1053 = !DISubprogram(name: "asinf", scope: !1049, file: !1049, line: 55, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1054 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1055, file: !1037, line: 374)
!1055 = !DISubprogram(name: "atanf", scope: !1049, file: !1049, line: 57, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1056 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1057, file: !1037, line: 376)
!1057 = !DISubprogram(name: "atan2f", scope: !1049, file: !1049, line: 59, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1058 = !DISubroutineType(types: !1059)
!1059 = !{!1044, !1044, !1044}
!1060 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1061, file: !1037, line: 378)
!1061 = !DISubprogram(name: "ceilf", scope: !1049, file: !1049, line: 174, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1062 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1063, file: !1037, line: 380)
!1063 = !DISubprogram(name: "cosf", scope: !1049, file: !1049, line: 62, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1064 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1065, file: !1037, line: 382)
!1065 = !DISubprogram(name: "coshf", scope: !1049, file: !1049, line: 71, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1066 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1067, file: !1037, line: 385)
!1067 = !DISubprogram(name: "expf", scope: !1049, file: !1049, line: 95, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1068 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1069, file: !1037, line: 388)
!1069 = !DISubprogram(name: "fabsf", scope: !1049, file: !1049, line: 177, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1070 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1071, file: !1037, line: 390)
!1071 = !DISubprogram(name: "floorf", scope: !1049, file: !1049, line: 180, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1072 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1073, file: !1037, line: 393)
!1073 = !DISubprogram(name: "fmodf", scope: !1049, file: !1049, line: 183, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1074 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1075, file: !1037, line: 396)
!1075 = !DISubprogram(name: "frexpf", scope: !1049, file: !1049, line: 98, type: !1076, flags: DIFlagPrototyped, spFlags: 0)
!1076 = !DISubroutineType(types: !1077)
!1077 = !{!1044, !1044, !1078}
!1078 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !88, size: 64)
!1079 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1080, file: !1037, line: 398)
!1080 = !DISubprogram(name: "ldexpf", scope: !1049, file: !1049, line: 101, type: !1081, flags: DIFlagPrototyped, spFlags: 0)
!1081 = !DISubroutineType(types: !1082)
!1082 = !{!1044, !1044, !88}
!1083 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1084, file: !1037, line: 401)
!1084 = !DISubprogram(name: "logf", scope: !1049, file: !1049, line: 104, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1085 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1086, file: !1037, line: 404)
!1086 = !DISubprogram(name: "log10f", scope: !1049, file: !1049, line: 107, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1087 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1031, file: !1037, line: 405)
!1088 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1089, file: !1037, line: 406)
!1089 = !DISubprogram(name: "modff", scope: !1049, file: !1049, line: 110, type: !1090, flags: DIFlagPrototyped, spFlags: 0)
!1090 = !DISubroutineType(types: !1091)
!1091 = !{!1044, !1044, !1092}
!1092 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1044, size: 64)
!1093 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1094, file: !1037, line: 409)
!1094 = !DISubprogram(name: "powf", scope: !1049, file: !1049, line: 155, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1095 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1096, file: !1037, line: 412)
!1096 = !DISubprogram(name: "sinf", scope: !1049, file: !1049, line: 64, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1097 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1098, file: !1037, line: 414)
!1098 = !DISubprogram(name: "sinhf", scope: !1049, file: !1049, line: 73, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1099 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1100, file: !1037, line: 417)
!1100 = !DISubprogram(name: "sqrtf", scope: !1049, file: !1049, line: 158, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1101 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1102, file: !1037, line: 419)
!1102 = !DISubprogram(name: "tanf", scope: !1049, file: !1049, line: 66, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1103 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1104, file: !1037, line: 422)
!1104 = !DISubprogram(name: "tanhf", scope: !1049, file: !1049, line: 75, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1105 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1106, file: !1037, line: 425)
!1106 = !DISubprogram(name: "acoshf", scope: !1049, file: !1049, line: 85, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1107 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1108, file: !1037, line: 427)
!1108 = !DISubprogram(name: "asinhf", scope: !1049, file: !1049, line: 87, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1109 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1110, file: !1037, line: 429)
!1110 = !DISubprogram(name: "atanhf", scope: !1049, file: !1049, line: 89, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1111 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1112, file: !1037, line: 431)
!1112 = !DISubprogram(name: "cbrtf", scope: !1049, file: !1049, line: 167, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1113 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1114, file: !1037, line: 434)
!1114 = !DISubprogram(name: "copysignf", scope: !1049, file: !1049, line: 213, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1115 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1116, file: !1037, line: 437)
!1116 = !DISubprogram(name: "erff", scope: !1049, file: !1049, line: 246, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1117 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1118, file: !1037, line: 439)
!1118 = !DISubprogram(name: "erfcf", scope: !1049, file: !1049, line: 247, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1119 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1120, file: !1037, line: 441)
!1120 = !DISubprogram(name: "exp2f", scope: !1049, file: !1049, line: 145, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1121 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1122, file: !1037, line: 443)
!1122 = !DISubprogram(name: "expm1f", scope: !1049, file: !1049, line: 134, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1123 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1124, file: !1037, line: 445)
!1124 = !DISubprogram(name: "fdimf", scope: !1049, file: !1049, line: 344, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1125 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1126, file: !1037, line: 446)
!1126 = !DISubprogram(name: "fmaf", scope: !1049, file: !1049, line: 355, type: !1127, flags: DIFlagPrototyped, spFlags: 0)
!1127 = !DISubroutineType(types: !1128)
!1128 = !{!1044, !1044, !1044, !1044}
!1129 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1130, file: !1037, line: 449)
!1130 = !DISubprogram(name: "fmaxf", scope: !1049, file: !1049, line: 348, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1131 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1132, file: !1037, line: 451)
!1132 = !DISubprogram(name: "fminf", scope: !1049, file: !1049, line: 351, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1133 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1134, file: !1037, line: 453)
!1134 = !DISubprogram(name: "hypotf", scope: !1049, file: !1049, line: 162, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1135 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1136, file: !1037, line: 455)
!1136 = !DISubprogram(name: "ilogbf", scope: !1049, file: !1049, line: 298, type: !1137, flags: DIFlagPrototyped, spFlags: 0)
!1137 = !DISubroutineType(types: !1138)
!1138 = !{!88, !1044}
!1139 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1140, file: !1037, line: 457)
!1140 = !DISubprogram(name: "lgammaf", scope: !1049, file: !1049, line: 248, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1141 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1142, file: !1037, line: 459)
!1142 = !DISubprogram(name: "llrintf", scope: !1049, file: !1049, line: 334, type: !1143, flags: DIFlagPrototyped, spFlags: 0)
!1143 = !DISubroutineType(types: !1144)
!1144 = !{!48, !1044}
!1145 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1146, file: !1037, line: 461)
!1146 = !DISubprogram(name: "llroundf", scope: !1049, file: !1049, line: 340, type: !1143, flags: DIFlagPrototyped, spFlags: 0)
!1147 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1148, file: !1037, line: 463)
!1148 = !DISubprogram(name: "log1pf", scope: !1049, file: !1049, line: 137, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1149 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1150, file: !1037, line: 465)
!1150 = !DISubprogram(name: "log2f", scope: !1049, file: !1049, line: 148, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1151 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1152, file: !1037, line: 467)
!1152 = !DISubprogram(name: "logbf", scope: !1049, file: !1049, line: 140, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1153 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1154, file: !1037, line: 469)
!1154 = !DISubprogram(name: "lrintf", scope: !1049, file: !1049, line: 332, type: !1155, flags: DIFlagPrototyped, spFlags: 0)
!1155 = !DISubroutineType(types: !1156)
!1156 = !{!108, !1044}
!1157 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1158, file: !1037, line: 471)
!1158 = !DISubprogram(name: "lroundf", scope: !1049, file: !1049, line: 338, type: !1155, flags: DIFlagPrototyped, spFlags: 0)
!1159 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1160, file: !1037, line: 473)
!1160 = !DISubprogram(name: "nan", scope: !1049, file: !1049, line: 218, type: !1161, flags: DIFlagPrototyped, spFlags: 0)
!1161 = !DISubroutineType(types: !1162)
!1162 = !{!977, !883}
!1163 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1164, file: !1037, line: 474)
!1164 = !DISubprogram(name: "nanf", scope: !1049, file: !1049, line: 218, type: !1165, flags: DIFlagPrototyped, spFlags: 0)
!1165 = !DISubroutineType(types: !1166)
!1166 = !{!1044, !883}
!1167 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1168, file: !1037, line: 477)
!1168 = !DISubprogram(name: "nearbyintf", scope: !1049, file: !1049, line: 312, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1169 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1170, file: !1037, line: 479)
!1170 = !DISubprogram(name: "nextafterf", scope: !1049, file: !1049, line: 277, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1171 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1172, file: !1037, line: 481)
!1172 = !DISubprogram(name: "nexttowardf", scope: !1049, file: !1049, line: 279, type: !1173, flags: DIFlagPrototyped, spFlags: 0)
!1173 = !DISubroutineType(types: !1174)
!1174 = !{!1044, !1044, !1022}
!1175 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1176, file: !1037, line: 483)
!1176 = !DISubprogram(name: "remainderf", scope: !1049, file: !1049, line: 290, type: !1058, flags: DIFlagPrototyped, spFlags: 0)
!1177 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1178, file: !1037, line: 485)
!1178 = !DISubprogram(name: "remquof", scope: !1049, file: !1049, line: 325, type: !1179, flags: DIFlagPrototyped, spFlags: 0)
!1179 = !DISubroutineType(types: !1180)
!1180 = !{!1044, !1044, !1044, !1078}
!1181 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1182, file: !1037, line: 487)
!1182 = !DISubprogram(name: "rintf", scope: !1049, file: !1049, line: 274, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1183 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1184, file: !1037, line: 489)
!1184 = !DISubprogram(name: "roundf", scope: !1049, file: !1049, line: 316, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1185 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1186, file: !1037, line: 491)
!1186 = !DISubprogram(name: "scalblnf", scope: !1049, file: !1049, line: 308, type: !1187, flags: DIFlagPrototyped, spFlags: 0)
!1187 = !DISubroutineType(types: !1188)
!1188 = !{!1044, !1044, !108}
!1189 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1190, file: !1037, line: 493)
!1190 = !DISubprogram(name: "scalbnf", scope: !1049, file: !1049, line: 294, type: !1081, flags: DIFlagPrototyped, spFlags: 0)
!1191 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1192, file: !1037, line: 495)
!1192 = !DISubprogram(name: "tgammaf", scope: !1049, file: !1049, line: 253, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1193 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1194, file: !1037, line: 497)
!1194 = !DISubprogram(name: "truncf", scope: !1049, file: !1049, line: 320, type: !1050, flags: DIFlagPrototyped, spFlags: 0)
!1195 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1196, file: !1037, line: 499)
!1196 = !DISubprogram(name: "acosl", scope: !1049, file: !1049, line: 53, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1197 = !DISubroutineType(types: !1198)
!1198 = !{!1022, !1022}
!1199 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1200, file: !1037, line: 500)
!1200 = !DISubprogram(name: "asinl", scope: !1049, file: !1049, line: 55, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1201 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1202, file: !1037, line: 501)
!1202 = !DISubprogram(name: "atanl", scope: !1049, file: !1049, line: 57, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1203 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1204, file: !1037, line: 502)
!1204 = !DISubprogram(name: "atan2l", scope: !1049, file: !1049, line: 59, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1205 = !DISubroutineType(types: !1206)
!1206 = !{!1022, !1022, !1022}
!1207 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1208, file: !1037, line: 503)
!1208 = !DISubprogram(name: "ceill", scope: !1049, file: !1049, line: 174, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1209 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1210, file: !1037, line: 504)
!1210 = !DISubprogram(name: "cosl", scope: !1049, file: !1049, line: 62, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1211 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1212, file: !1037, line: 505)
!1212 = !DISubprogram(name: "coshl", scope: !1049, file: !1049, line: 71, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1213 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1214, file: !1037, line: 506)
!1214 = !DISubprogram(name: "expl", scope: !1049, file: !1049, line: 95, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1215 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1216, file: !1037, line: 507)
!1216 = !DISubprogram(name: "fabsl", scope: !1049, file: !1049, line: 177, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1217 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1218, file: !1037, line: 508)
!1218 = !DISubprogram(name: "floorl", scope: !1049, file: !1049, line: 180, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1219 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1220, file: !1037, line: 509)
!1220 = !DISubprogram(name: "fmodl", scope: !1049, file: !1049, line: 183, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1221 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1222, file: !1037, line: 510)
!1222 = !DISubprogram(name: "frexpl", scope: !1049, file: !1049, line: 98, type: !1223, flags: DIFlagPrototyped, spFlags: 0)
!1223 = !DISubroutineType(types: !1224)
!1224 = !{!1022, !1022, !1078}
!1225 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1226, file: !1037, line: 511)
!1226 = !DISubprogram(name: "ldexpl", scope: !1049, file: !1049, line: 101, type: !1227, flags: DIFlagPrototyped, spFlags: 0)
!1227 = !DISubroutineType(types: !1228)
!1228 = !{!1022, !1022, !88}
!1229 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1230, file: !1037, line: 512)
!1230 = !DISubprogram(name: "logl", scope: !1049, file: !1049, line: 104, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1231 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1232, file: !1037, line: 513)
!1232 = !DISubprogram(name: "log10l", scope: !1049, file: !1049, line: 107, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1233 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1234, file: !1037, line: 514)
!1234 = !DISubprogram(name: "modfl", scope: !1049, file: !1049, line: 110, type: !1033, flags: DIFlagPrototyped, spFlags: 0)
!1235 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1236, file: !1037, line: 515)
!1236 = !DISubprogram(name: "powl", scope: !1049, file: !1049, line: 155, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1237 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1238, file: !1037, line: 516)
!1238 = !DISubprogram(name: "sinl", scope: !1049, file: !1049, line: 64, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1239 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1240, file: !1037, line: 517)
!1240 = !DISubprogram(name: "sinhl", scope: !1049, file: !1049, line: 73, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1241 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1242, file: !1037, line: 518)
!1242 = !DISubprogram(name: "sqrtl", scope: !1049, file: !1049, line: 158, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1243 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1244, file: !1037, line: 519)
!1244 = !DISubprogram(name: "tanl", scope: !1049, file: !1049, line: 66, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1245 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1246, file: !1037, line: 521)
!1246 = !DISubprogram(name: "tanhl", scope: !1049, file: !1049, line: 75, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1247 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1248, file: !1037, line: 522)
!1248 = !DISubprogram(name: "acoshl", scope: !1049, file: !1049, line: 85, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1249 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1250, file: !1037, line: 523)
!1250 = !DISubprogram(name: "asinhl", scope: !1049, file: !1049, line: 87, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1251 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1252, file: !1037, line: 524)
!1252 = !DISubprogram(name: "atanhl", scope: !1049, file: !1049, line: 89, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1253 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1254, file: !1037, line: 525)
!1254 = !DISubprogram(name: "cbrtl", scope: !1049, file: !1049, line: 167, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1255 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1256, file: !1037, line: 527)
!1256 = !DISubprogram(name: "copysignl", scope: !1049, file: !1049, line: 213, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1257 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1258, file: !1037, line: 529)
!1258 = !DISubprogram(name: "erfl", scope: !1049, file: !1049, line: 246, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1259 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1260, file: !1037, line: 530)
!1260 = !DISubprogram(name: "erfcl", scope: !1049, file: !1049, line: 247, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1261 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1262, file: !1037, line: 531)
!1262 = !DISubprogram(name: "exp2l", scope: !1049, file: !1049, line: 145, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1263 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1264, file: !1037, line: 532)
!1264 = !DISubprogram(name: "expm1l", scope: !1049, file: !1049, line: 134, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1265 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1266, file: !1037, line: 533)
!1266 = !DISubprogram(name: "fdiml", scope: !1049, file: !1049, line: 344, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1267 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1268, file: !1037, line: 534)
!1268 = !DISubprogram(name: "fmal", scope: !1049, file: !1049, line: 355, type: !1269, flags: DIFlagPrototyped, spFlags: 0)
!1269 = !DISubroutineType(types: !1270)
!1270 = !{!1022, !1022, !1022, !1022}
!1271 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1272, file: !1037, line: 535)
!1272 = !DISubprogram(name: "fmaxl", scope: !1049, file: !1049, line: 348, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1273 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1274, file: !1037, line: 536)
!1274 = !DISubprogram(name: "fminl", scope: !1049, file: !1049, line: 351, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1275 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1276, file: !1037, line: 537)
!1276 = !DISubprogram(name: "hypotl", scope: !1049, file: !1049, line: 162, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1277 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1278, file: !1037, line: 538)
!1278 = !DISubprogram(name: "ilogbl", scope: !1049, file: !1049, line: 298, type: !1279, flags: DIFlagPrototyped, spFlags: 0)
!1279 = !DISubroutineType(types: !1280)
!1280 = !{!88, !1022}
!1281 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1282, file: !1037, line: 539)
!1282 = !DISubprogram(name: "lgammal", scope: !1049, file: !1049, line: 248, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1283 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1284, file: !1037, line: 540)
!1284 = !DISubprogram(name: "llrintl", scope: !1049, file: !1049, line: 334, type: !1285, flags: DIFlagPrototyped, spFlags: 0)
!1285 = !DISubroutineType(types: !1286)
!1286 = !{!48, !1022}
!1287 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1288, file: !1037, line: 541)
!1288 = !DISubprogram(name: "llroundl", scope: !1049, file: !1049, line: 340, type: !1285, flags: DIFlagPrototyped, spFlags: 0)
!1289 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1290, file: !1037, line: 542)
!1290 = !DISubprogram(name: "log1pl", scope: !1049, file: !1049, line: 137, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1291 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1292, file: !1037, line: 543)
!1292 = !DISubprogram(name: "log2l", scope: !1049, file: !1049, line: 148, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1293 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1294, file: !1037, line: 544)
!1294 = !DISubprogram(name: "logbl", scope: !1049, file: !1049, line: 140, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1295 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1296, file: !1037, line: 545)
!1296 = !DISubprogram(name: "lrintl", scope: !1049, file: !1049, line: 332, type: !1297, flags: DIFlagPrototyped, spFlags: 0)
!1297 = !DISubroutineType(types: !1298)
!1298 = !{!108, !1022}
!1299 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1300, file: !1037, line: 546)
!1300 = !DISubprogram(name: "lroundl", scope: !1049, file: !1049, line: 338, type: !1297, flags: DIFlagPrototyped, spFlags: 0)
!1301 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1302, file: !1037, line: 547)
!1302 = !DISubprogram(name: "nanl", scope: !1049, file: !1049, line: 218, type: !1303, flags: DIFlagPrototyped, spFlags: 0)
!1303 = !DISubroutineType(types: !1304)
!1304 = !{!1022, !883}
!1305 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1306, file: !1037, line: 548)
!1306 = !DISubprogram(name: "nearbyintl", scope: !1049, file: !1049, line: 312, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1307 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1308, file: !1037, line: 549)
!1308 = !DISubprogram(name: "nextafterl", scope: !1049, file: !1049, line: 277, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1309 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1310, file: !1037, line: 550)
!1310 = !DISubprogram(name: "nexttowardl", scope: !1049, file: !1049, line: 279, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1311 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1312, file: !1037, line: 551)
!1312 = !DISubprogram(name: "remainderl", scope: !1049, file: !1049, line: 290, type: !1205, flags: DIFlagPrototyped, spFlags: 0)
!1313 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1314, file: !1037, line: 552)
!1314 = !DISubprogram(name: "remquol", scope: !1049, file: !1049, line: 325, type: !1315, flags: DIFlagPrototyped, spFlags: 0)
!1315 = !DISubroutineType(types: !1316)
!1316 = !{!1022, !1022, !1022, !1078}
!1317 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1318, file: !1037, line: 553)
!1318 = !DISubprogram(name: "rintl", scope: !1049, file: !1049, line: 274, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1319 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1320, file: !1037, line: 554)
!1320 = !DISubprogram(name: "roundl", scope: !1049, file: !1049, line: 316, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1321 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1322, file: !1037, line: 555)
!1322 = !DISubprogram(name: "scalblnl", scope: !1049, file: !1049, line: 308, type: !1323, flags: DIFlagPrototyped, spFlags: 0)
!1323 = !DISubroutineType(types: !1324)
!1324 = !{!1022, !1022, !108}
!1325 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1326, file: !1037, line: 556)
!1326 = !DISubprogram(name: "scalbnl", scope: !1049, file: !1049, line: 294, type: !1227, flags: DIFlagPrototyped, spFlags: 0)
!1327 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1328, file: !1037, line: 557)
!1328 = !DISubprogram(name: "tgammal", scope: !1049, file: !1049, line: 253, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1329 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1330, file: !1037, line: 558)
!1330 = !DISubprogram(name: "truncl", scope: !1049, file: !1049, line: 320, type: !1197, flags: DIFlagPrototyped, spFlags: 0)
!1331 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1332, file: !1338, line: 22)
!1332 = !DIDerivedType(tag: DW_TAG_typedef, name: "max_align_t", file: !1333, line: 24, baseType: !1334)
!1333 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/lib64/clang/22/include/__stddef_max_align_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "3c0a2f19d136d39aa835c737c7105def")
!1334 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1333, line: 19, size: 256, flags: DIFlagTypePassByValue, elements: !1335, identifier: "_ZTS11max_align_t")
!1335 = !{!1336, !1337}
!1336 = !DIDerivedType(tag: DW_TAG_member, name: "__clang_max_align_nonce1", scope: !1334, file: !1333, line: 20, baseType: !48, size: 64, align: 64)
!1337 = !DIDerivedType(tag: DW_TAG_member, name: "__clang_max_align_nonce2", scope: !1334, file: !1333, line: 22, baseType: !1022, size: 128, align: 128, offset: 128)
!1338 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__cstddef/max_align_t.h", directory: "/home/toolchain/development")
!1339 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1340, file: !1343, line: 106)
!1340 = !DIDerivedType(tag: DW_TAG_typedef, name: "div_t", file: !1341, line: 63, baseType: !1342)
!1341 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/stdlib.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "a032969b0aa823d3e66e5bfb69d943da")
!1342 = !DICompositeType(tag: DW_TAG_structure_type, file: !1341, line: 59, size: 64, flags: DIFlagFwdDecl, identifier: "_ZTS5div_t")
!1343 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstdlib", directory: "/home/toolchain/development")
!1344 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1345, file: !1343, line: 107)
!1345 = !DIDerivedType(tag: DW_TAG_typedef, name: "ldiv_t", file: !1341, line: 71, baseType: !1346)
!1346 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1341, line: 67, size: 128, flags: DIFlagTypePassByValue, elements: !1347, identifier: "_ZTS6ldiv_t")
!1347 = !{!1348, !1349}
!1348 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !1346, file: !1341, line: 69, baseType: !108, size: 64)
!1349 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !1346, file: !1341, line: 70, baseType: !108, size: 64, offset: 64)
!1350 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1351, file: !1343, line: 108)
!1351 = !DIDerivedType(tag: DW_TAG_typedef, name: "lldiv_t", file: !1341, line: 81, baseType: !1352)
!1352 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1341, line: 77, size: 128, flags: DIFlagTypePassByValue, elements: !1353, identifier: "_ZTS7lldiv_t")
!1353 = !{!1354, !1355}
!1354 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !1352, file: !1341, line: 79, baseType: !48, size: 64)
!1355 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !1352, file: !1341, line: 80, baseType: !48, size: 64, offset: 64)
!1356 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1357, file: !1343, line: 109)
!1357 = !DISubprogram(name: "atof", scope: !1341, file: !1341, line: 102, type: !1161, flags: DIFlagPrototyped, spFlags: 0)
!1358 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1359, file: !1343, line: 110)
!1359 = !DISubprogram(name: "atoi", scope: !1341, file: !1341, line: 105, type: !1360, flags: DIFlagPrototyped, spFlags: 0)
!1360 = !DISubroutineType(types: !1361)
!1361 = !{!88, !883}
!1362 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1363, file: !1343, line: 111)
!1363 = !DISubprogram(name: "atol", scope: !1341, file: !1341, line: 108, type: !1364, flags: DIFlagPrototyped, spFlags: 0)
!1364 = !DISubroutineType(types: !1365)
!1365 = !{!108, !883}
!1366 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1367, file: !1343, line: 112)
!1367 = !DISubprogram(name: "atoll", scope: !1341, file: !1341, line: 113, type: !1368, flags: DIFlagPrototyped, spFlags: 0)
!1368 = !DISubroutineType(types: !1369)
!1369 = !{!48, !883}
!1370 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1371, file: !1343, line: 113)
!1371 = !DISubprogram(name: "strtod", scope: !1341, file: !1341, line: 118, type: !1372, flags: DIFlagPrototyped, spFlags: 0)
!1372 = !DISubroutineType(types: !1373)
!1373 = !{!977, !882, !1374}
!1374 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1375)
!1375 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !880, size: 64)
!1376 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1377, file: !1343, line: 114)
!1377 = !DISubprogram(name: "strtof", scope: !1341, file: !1341, line: 124, type: !1378, flags: DIFlagPrototyped, spFlags: 0)
!1378 = !DISubroutineType(types: !1379)
!1379 = !{!1044, !882, !1374}
!1380 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1381, file: !1343, line: 115)
!1381 = !DISubprogram(name: "strtold", scope: !1341, file: !1341, line: 127, type: !1382, flags: DIFlagPrototyped, spFlags: 0)
!1382 = !DISubroutineType(types: !1383)
!1383 = !{!1022, !882, !1374}
!1384 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1385, file: !1343, line: 116)
!1385 = !DISubprogram(name: "strtol", linkageName: "__isoc23_strtol", scope: !1341, file: !1341, line: 215, type: !1386, flags: DIFlagPrototyped, spFlags: 0)
!1386 = !DISubroutineType(types: !1387)
!1387 = !{!108, !882, !1374, !88}
!1388 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1389, file: !1343, line: 117)
!1389 = !DISubprogram(name: "strtoll", linkageName: "__isoc23_strtoll", scope: !1341, file: !1341, line: 238, type: !1390, flags: DIFlagPrototyped, spFlags: 0)
!1390 = !DISubroutineType(types: !1391)
!1391 = !{!48, !882, !1374, !88}
!1392 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1393, file: !1343, line: 118)
!1393 = !DISubprogram(name: "strtoul", linkageName: "__isoc23_strtoul", scope: !1341, file: !1341, line: 219, type: !1394, flags: DIFlagPrototyped, spFlags: 0)
!1394 = !DISubroutineType(types: !1395)
!1395 = !{!760, !882, !1374, !88}
!1396 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1397, file: !1343, line: 119)
!1397 = !DISubprogram(name: "strtoull", linkageName: "__isoc23_strtoull", scope: !1341, file: !1341, line: 243, type: !1398, flags: DIFlagPrototyped, spFlags: 0)
!1398 = !DISubroutineType(types: !1399)
!1399 = !{!26, !882, !1374, !88}
!1400 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1401, file: !1343, line: 120)
!1401 = !DISubprogram(name: "rand", scope: !1341, file: !1341, line: 573, type: !1402, flags: DIFlagPrototyped, spFlags: 0)
!1402 = !DISubroutineType(types: !1403)
!1403 = !{!88}
!1404 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1405, file: !1343, line: 121)
!1405 = !DISubprogram(name: "srand", scope: !1341, file: !1341, line: 575, type: !1406, flags: DIFlagPrototyped, spFlags: 0)
!1406 = !DISubroutineType(types: !1407)
!1407 = !{null, !31}
!1408 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1409, file: !1343, line: 122)
!1409 = !DISubprogram(name: "calloc", scope: !1341, file: !1341, line: 675, type: !1410, flags: DIFlagPrototyped, spFlags: 0)
!1410 = !DISubroutineType(types: !1411)
!1411 = !{!756, !758, !758}
!1412 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1413, file: !1343, line: 123)
!1413 = !DISubprogram(name: "free", scope: !1341, file: !1341, line: 687, type: !1414, flags: DIFlagPrototyped, spFlags: 0)
!1414 = !DISubroutineType(types: !1415)
!1415 = !{null, !756}
!1416 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1417, file: !1343, line: 124)
!1417 = !DISubprogram(name: "malloc", scope: !1341, file: !1341, line: 672, type: !1418, flags: DIFlagPrototyped, spFlags: 0)
!1418 = !DISubroutineType(types: !1419)
!1419 = !{!756, !758}
!1420 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1421, file: !1343, line: 125)
!1421 = !DISubprogram(name: "realloc", scope: !1341, file: !1341, line: 683, type: !1422, flags: DIFlagPrototyped, spFlags: 0)
!1422 = !DISubroutineType(types: !1423)
!1423 = !{!756, !756, !758}
!1424 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1425, file: !1343, line: 126)
!1425 = !DISubprogram(name: "abort", scope: !1341, file: !1341, line: 730, type: !1426, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1426 = !DISubroutineType(types: !1427)
!1427 = !{null}
!1428 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1429, file: !1343, line: 127)
!1429 = !DISubprogram(name: "atexit", scope: !1341, file: !1341, line: 734, type: !1430, flags: DIFlagPrototyped, spFlags: 0)
!1430 = !DISubroutineType(types: !1431)
!1431 = !{!88, !1432}
!1432 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1426, size: 64)
!1433 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1434, file: !1343, line: 128)
!1434 = !DISubprogram(name: "exit", scope: !1341, file: !1341, line: 756, type: !1435, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1435 = !DISubroutineType(types: !1436)
!1436 = !{null, !88}
!1437 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1438, file: !1343, line: 129)
!1438 = !DISubprogram(name: "_Exit", scope: !1341, file: !1341, line: 768, type: !1435, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1439 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1440, file: !1343, line: 130)
!1440 = !DISubprogram(name: "getenv", scope: !1341, file: !1341, line: 773, type: !1441, flags: DIFlagPrototyped, spFlags: 0)
!1441 = !DISubroutineType(types: !1442)
!1442 = !{!880, !883}
!1443 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1444, file: !1343, line: 131)
!1444 = !DISubprogram(name: "system", scope: !1341, file: !1341, line: 923, type: !1360, flags: DIFlagPrototyped, spFlags: 0)
!1445 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1446, file: !1343, line: 132)
!1446 = !DISubprogram(name: "bsearch", scope: !1341, file: !1341, line: 960, type: !1447, flags: DIFlagPrototyped, spFlags: 0)
!1447 = !DISubroutineType(types: !1448)
!1448 = !{!756, !869, !869, !758, !758, !1449}
!1449 = !DIDerivedType(tag: DW_TAG_typedef, name: "__compar_fn_t", file: !1341, line: 948, baseType: !1450)
!1450 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1451, size: 64)
!1451 = !DISubroutineType(types: !1452)
!1452 = !{!88, !869, !869}
!1453 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1454, file: !1343, line: 133)
!1454 = !DISubprogram(name: "qsort", scope: !1341, file: !1341, line: 970, type: !1455, flags: DIFlagPrototyped, spFlags: 0)
!1455 = !DISubroutineType(types: !1456)
!1456 = !{null, !756, !758, !758, !1449}
!1457 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1458, file: !1343, line: 134)
!1458 = !DISubprogram(name: "abs", scope: !1341, file: !1341, line: 980, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1459 = !DISubroutineType(types: !1460)
!1460 = !{!88, !88}
!1461 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1462, file: !1343, line: 135)
!1462 = !DISubprogram(name: "labs", scope: !1341, file: !1341, line: 981, type: !1463, flags: DIFlagPrototyped, spFlags: 0)
!1463 = !DISubroutineType(types: !1464)
!1464 = !{!108, !108}
!1465 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1466, file: !1343, line: 136)
!1466 = !DISubprogram(name: "llabs", scope: !1341, file: !1341, line: 984, type: !1467, flags: DIFlagPrototyped, spFlags: 0)
!1467 = !DISubroutineType(types: !1468)
!1468 = !{!48, !48}
!1469 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1470, file: !1343, line: 137)
!1470 = !DISubprogram(name: "div", linkageName: "_Z3divB9nqn220108xx", scope: !1471, file: !1471, line: 128, type: !1472, flags: DIFlagPrototyped, spFlags: 0)
!1471 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/stdlib.h", directory: "/home/toolchain/development")
!1472 = !DISubroutineType(types: !1473)
!1473 = !{!1351, !48, !48}
!1474 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1475, file: !1343, line: 138)
!1475 = !DISubprogram(name: "ldiv", scope: !1341, file: !1341, line: 994, type: !1476, flags: DIFlagPrototyped, spFlags: 0)
!1476 = !DISubroutineType(types: !1477)
!1477 = !{!1345, !108, !108}
!1478 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1479, file: !1343, line: 139)
!1479 = !DISubprogram(name: "lldiv", scope: !1341, file: !1341, line: 998, type: !1472, flags: DIFlagPrototyped, spFlags: 0)
!1480 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1481, file: !1343, line: 140)
!1481 = !DISubprogram(name: "mblen", scope: !1341, file: !1341, line: 1062, type: !1482, flags: DIFlagPrototyped, spFlags: 0)
!1482 = !DISubroutineType(types: !1483)
!1483 = !{!88, !883, !758}
!1484 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1485, file: !1343, line: 142)
!1485 = !DISubprogram(name: "mbtowc", scope: !1341, file: !1341, line: 1065, type: !1486, flags: DIFlagPrototyped, spFlags: 0)
!1486 = !DISubroutineType(types: !1487)
!1487 = !{!88, !1488, !882, !758}
!1488 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1489)
!1489 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1490, size: 64)
!1490 = !DIBasicType(name: "wchar_t", size: 32, encoding: DW_ATE_signed)
!1491 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1492, file: !1343, line: 143)
!1492 = !DISubprogram(name: "wctomb", scope: !1341, file: !1341, line: 1069, type: !1493, flags: DIFlagPrototyped, spFlags: 0)
!1493 = !DISubroutineType(types: !1494)
!1494 = !{!88, !880, !1490}
!1495 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1496, file: !1343, line: 144)
!1496 = !DISubprogram(name: "mbstowcs", scope: !1341, file: !1341, line: 1073, type: !1497, flags: DIFlagPrototyped, spFlags: 0)
!1497 = !DISubroutineType(types: !1498)
!1498 = !{!758, !1488, !882, !758}
!1499 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1500, file: !1343, line: 145)
!1500 = !DISubprogram(name: "wcstombs", scope: !1341, file: !1341, line: 1077, type: !1501, flags: DIFlagPrototyped, spFlags: 0)
!1501 = !DISubroutineType(types: !1502)
!1502 = !{!758, !881, !1503, !758}
!1503 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1504)
!1504 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1505, size: 64)
!1505 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !1490)
!1506 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1507, file: !1343, line: 148)
!1507 = !DISubprogram(name: "at_quick_exit", scope: !1341, file: !1341, line: 739, type: !1430, flags: DIFlagPrototyped, spFlags: 0)
!1508 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1509, file: !1343, line: 149)
!1509 = !DISubprogram(name: "quick_exit", scope: !1341, file: !1341, line: 762, type: !1435, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!1510 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1511, file: !1343, line: 152)
!1511 = !DISubprogram(name: "aligned_alloc", scope: !1341, file: !1341, line: 724, type: !1410, flags: DIFlagPrototyped, spFlags: 0)
!1512 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1513, file: !1528, line: 25)
!1513 = !DIDerivedType(tag: DW_TAG_typedef, name: "mbstate_t", file: !1514, line: 6, baseType: !1515)
!1514 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/mbstate_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "ba8742313715e20e434cf6ccb2db98e3")
!1515 = !DIDerivedType(tag: DW_TAG_typedef, name: "__mbstate_t", file: !1516, line: 21, baseType: !1517)
!1516 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/__mbstate_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "82911a3e689448e3691ded3e0b471a55")
!1517 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !1516, line: 13, size: 64, flags: DIFlagTypePassByValue, elements: !1518, identifier: "_ZTS11__mbstate_t")
!1518 = !{!1519, !1520}
!1519 = !DIDerivedType(tag: DW_TAG_member, name: "__count", scope: !1517, file: !1516, line: 15, baseType: !88, size: 32)
!1520 = !DIDerivedType(tag: DW_TAG_member, name: "__value", scope: !1517, file: !1516, line: 20, baseType: !1521, size: 32, offset: 32)
!1521 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !1517, file: !1516, line: 16, size: 32, flags: DIFlagTypePassByValue, elements: !1522, identifier: "_ZTSN11__mbstate_tUt_E")
!1522 = !{!1523, !1524}
!1523 = !DIDerivedType(tag: DW_TAG_member, name: "__wch", scope: !1521, file: !1516, line: 18, baseType: !31, size: 32)
!1524 = !DIDerivedType(tag: DW_TAG_member, name: "__wchb", scope: !1521, file: !1516, line: 19, baseType: !1525, size: 32)
!1525 = !DICompositeType(tag: DW_TAG_array_type, baseType: !44, size: 32, elements: !1526)
!1526 = !{!1527}
!1527 = !DISubrange(count: 4)
!1528 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/__std_mbstate_t.h", directory: "/home/toolchain/development")
!1529 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1530, file: !1532, line: 113)
!1530 = !DISubprogram(name: "isalnum", scope: !1531, file: !1531, line: 108, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1531 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/ctype.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "43fd45dcf96e8fb7d8f14700096497c7")
!1532 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cctype", directory: "/home/toolchain/development")
!1533 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1534, file: !1532, line: 114)
!1534 = !DISubprogram(name: "isalpha", scope: !1531, file: !1531, line: 109, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1535 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1536, file: !1532, line: 115)
!1536 = !DISubprogram(name: "isblank", scope: !1531, file: !1531, line: 130, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1537 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1538, file: !1532, line: 116)
!1538 = !DISubprogram(name: "iscntrl", scope: !1531, file: !1531, line: 110, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1539 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1540, file: !1532, line: 117)
!1540 = !DISubprogram(name: "isdigit", scope: !1531, file: !1531, line: 111, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1541 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1542, file: !1532, line: 118)
!1542 = !DISubprogram(name: "isgraph", scope: !1531, file: !1531, line: 113, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1543 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1544, file: !1532, line: 119)
!1544 = !DISubprogram(name: "islower", scope: !1531, file: !1531, line: 112, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1545 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1546, file: !1532, line: 120)
!1546 = !DISubprogram(name: "isprint", scope: !1531, file: !1531, line: 114, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1547 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1548, file: !1532, line: 121)
!1548 = !DISubprogram(name: "ispunct", scope: !1531, file: !1531, line: 115, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1549 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1550, file: !1532, line: 122)
!1550 = !DISubprogram(name: "isspace", scope: !1531, file: !1531, line: 116, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1551 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1552, file: !1532, line: 123)
!1552 = !DISubprogram(name: "isupper", scope: !1531, file: !1531, line: 117, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1553 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1554, file: !1532, line: 124)
!1554 = !DISubprogram(name: "isxdigit", scope: !1531, file: !1531, line: 118, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1555 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1556, file: !1532, line: 125)
!1556 = !DISubprogram(name: "tolower", scope: !1531, file: !1531, line: 122, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1557 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1558, file: !1532, line: 126)
!1558 = !DISubprogram(name: "toupper", scope: !1531, file: !1531, line: 125, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!1559 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1560, file: !1562, line: 75)
!1560 = !DIDerivedType(tag: DW_TAG_typedef, name: "wint_t", file: !1561, line: 20, baseType: !31)
!1561 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/wint_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "aa31b53ef28dc23152ceb41e2763ded3")
!1562 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cwctype", directory: "/home/toolchain/development")
!1563 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1564, file: !1562, line: 76)
!1564 = !DIDerivedType(tag: DW_TAG_typedef, name: "wctrans_t", file: !1565, line: 48, baseType: !1566)
!1565 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/wctype.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "eff95da6508e8f67a3c7b77d9d8ab229")
!1566 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1567, size: 64)
!1567 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !795)
!1568 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1569, file: !1562, line: 77)
!1569 = !DIDerivedType(tag: DW_TAG_typedef, name: "wctype_t", file: !1570, line: 38, baseType: !760)
!1570 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/wctype-wchar.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "7f19501745f9a1fbbace8f0f185de59a")
!1571 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1572, file: !1562, line: 78)
!1572 = !DISubprogram(name: "iswalnum", scope: !1570, file: !1570, line: 95, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1573 = !DISubroutineType(types: !1574)
!1574 = !{!88, !1560}
!1575 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1576, file: !1562, line: 79)
!1576 = !DISubprogram(name: "iswalpha", scope: !1570, file: !1570, line: 101, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1577 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1578, file: !1562, line: 80)
!1578 = !DISubprogram(name: "iswblank", scope: !1570, file: !1570, line: 146, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1579 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1580, file: !1562, line: 81)
!1580 = !DISubprogram(name: "iswcntrl", scope: !1570, file: !1570, line: 104, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1581 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1582, file: !1562, line: 82)
!1582 = !DISubprogram(name: "iswdigit", scope: !1570, file: !1570, line: 108, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1583 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1584, file: !1562, line: 83)
!1584 = !DISubprogram(name: "iswgraph", scope: !1570, file: !1570, line: 112, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1585 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1586, file: !1562, line: 84)
!1586 = !DISubprogram(name: "iswlower", scope: !1570, file: !1570, line: 117, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1587 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1588, file: !1562, line: 85)
!1588 = !DISubprogram(name: "iswprint", scope: !1570, file: !1570, line: 120, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1589 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1590, file: !1562, line: 86)
!1590 = !DISubprogram(name: "iswpunct", scope: !1570, file: !1570, line: 125, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1591 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1592, file: !1562, line: 87)
!1592 = !DISubprogram(name: "iswspace", scope: !1570, file: !1570, line: 130, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1593 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1594, file: !1562, line: 88)
!1594 = !DISubprogram(name: "iswupper", scope: !1570, file: !1570, line: 135, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1595 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1596, file: !1562, line: 89)
!1596 = !DISubprogram(name: "iswxdigit", scope: !1570, file: !1570, line: 140, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1597 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1598, file: !1562, line: 90)
!1598 = !DISubprogram(name: "iswctype", scope: !1570, file: !1570, line: 159, type: !1599, flags: DIFlagPrototyped, spFlags: 0)
!1599 = !DISubroutineType(types: !1600)
!1600 = !{!88, !1560, !1569}
!1601 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1602, file: !1562, line: 91)
!1602 = !DISubprogram(name: "wctype", scope: !1570, file: !1570, line: 155, type: !1603, flags: DIFlagPrototyped, spFlags: 0)
!1603 = !DISubroutineType(types: !1604)
!1604 = !{!1569, !883}
!1605 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1606, file: !1562, line: 92)
!1606 = !DISubprogram(name: "towlower", scope: !1570, file: !1570, line: 166, type: !1607, flags: DIFlagPrototyped, spFlags: 0)
!1607 = !DISubroutineType(types: !1608)
!1608 = !{!1560, !1560}
!1609 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1610, file: !1562, line: 93)
!1610 = !DISubprogram(name: "towupper", scope: !1570, file: !1570, line: 169, type: !1607, flags: DIFlagPrototyped, spFlags: 0)
!1611 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1612, file: !1562, line: 94)
!1612 = !DISubprogram(name: "towctrans", scope: !1565, file: !1565, line: 55, type: !1613, flags: DIFlagPrototyped, spFlags: 0)
!1613 = !DISubroutineType(types: !1614)
!1614 = !{!1560, !1560, !1564}
!1615 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1616, file: !1562, line: 95)
!1616 = !DISubprogram(name: "wctrans", scope: !1565, file: !1565, line: 52, type: !1617, flags: DIFlagPrototyped, spFlags: 0)
!1617 = !DISubroutineType(types: !1618)
!1618 = !{!1564, !883}
!1619 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1513, file: !1620, line: 134)
!1620 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cwchar", directory: "/home/toolchain/development")
!1621 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !959, file: !1620, line: 135)
!1622 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1560, file: !1620, line: 136)
!1623 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1624, file: !1620, line: 137)
!1624 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !1625, line: 7, baseType: !1626)
!1625 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/FILE.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "571f9fb6223c42439075fdde11a0de5d")
!1626 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !1627, line: 4, flags: DIFlagFwdDecl | DIFlagNonTrivial, identifier: "_ZTS8_IO_FILE")
!1627 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/__FILE.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "72a8fe90981f484acae7c6f3dfc5c2b7")
!1628 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1629, file: !1620, line: 138)
!1629 = !DISubprogram(name: "fwprintf", scope: !1630, file: !1630, line: 732, type: !1631, flags: DIFlagPrototyped, spFlags: 0)
!1630 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/wchar.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "91346206ecdf1575ff4fea6ab909018f")
!1631 = !DISubroutineType(types: !1632)
!1632 = !{!88, !1633, !1503, null}
!1633 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1634)
!1634 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1635, size: 64)
!1635 = !DIDerivedType(tag: DW_TAG_typedef, name: "__FILE", file: !1627, line: 5, baseType: !1626)
!1636 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1637, file: !1620, line: 139)
!1637 = !DISubprogram(name: "fwscanf", linkageName: "__isoc23_fwscanf", scope: !1630, file: !1630, line: 795, type: !1631, flags: DIFlagPrototyped, spFlags: 0)
!1638 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1639, file: !1620, line: 140)
!1639 = !DISubprogram(name: "swprintf", scope: !1630, file: !1630, line: 742, type: !1640, flags: DIFlagPrototyped, spFlags: 0)
!1640 = !DISubroutineType(types: !1641)
!1641 = !{!88, !1488, !758, !1503, null}
!1642 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1643, file: !1620, line: 141)
!1643 = !DISubprogram(name: "vfwprintf", scope: !1630, file: !1630, line: 750, type: !1644, flags: DIFlagPrototyped, spFlags: 0)
!1644 = !DISubroutineType(types: !1645)
!1645 = !{!88, !1633, !1503, !1646}
!1646 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1647, size: 64)
!1647 = !DICompositeType(tag: DW_TAG_structure_type, name: "__va_list_tag", file: !3, size: 192, flags: DIFlagFwdDecl, identifier: "_ZTS13__va_list_tag")
!1648 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1649, file: !1620, line: 142)
!1649 = !DISubprogram(name: "vswprintf", scope: !1630, file: !1630, line: 763, type: !1650, flags: DIFlagPrototyped, spFlags: 0)
!1650 = !DISubroutineType(types: !1651)
!1651 = !{!88, !1488, !758, !1503, !1646}
!1652 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1653, file: !1620, line: 143)
!1653 = !DISubprogram(name: "swscanf", linkageName: "__isoc23_swscanf", scope: !1630, file: !1630, line: 802, type: !1654, flags: DIFlagPrototyped, spFlags: 0)
!1654 = !DISubroutineType(types: !1655)
!1655 = !{!88, !1503, !1503, null}
!1656 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1657, file: !1620, line: 144)
!1657 = !DISubprogram(name: "vfwscanf", linkageName: "__isoc23_vfwscanf", scope: !1630, file: !1630, line: 875, type: !1644, flags: DIFlagPrototyped, spFlags: 0)
!1658 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1659, file: !1620, line: 145)
!1659 = !DISubprogram(name: "vswscanf", linkageName: "__isoc23_vswscanf", scope: !1630, file: !1630, line: 882, type: !1660, flags: DIFlagPrototyped, spFlags: 0)
!1660 = !DISubroutineType(types: !1661)
!1661 = !{!88, !1503, !1503, !1646}
!1662 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1663, file: !1620, line: 146)
!1663 = !DISubprogram(name: "fgetwc", scope: !1630, file: !1630, line: 935, type: !1664, flags: DIFlagPrototyped, spFlags: 0)
!1664 = !DISubroutineType(types: !1665)
!1665 = !{!1560, !1634}
!1666 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1667, file: !1620, line: 147)
!1667 = !DISubprogram(name: "fgetws", scope: !1630, file: !1630, line: 964, type: !1668, flags: DIFlagPrototyped, spFlags: 0)
!1668 = !DISubroutineType(types: !1669)
!1669 = !{!1489, !1488, !88, !1633}
!1670 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1671, file: !1620, line: 148)
!1671 = !DISubprogram(name: "fputwc", scope: !1630, file: !1630, line: 949, type: !1672, flags: DIFlagPrototyped, spFlags: 0)
!1672 = !DISubroutineType(types: !1673)
!1673 = !{!1560, !1490, !1634}
!1674 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1675, file: !1620, line: 149)
!1675 = !DISubprogram(name: "fputws", scope: !1630, file: !1630, line: 971, type: !1676, flags: DIFlagPrototyped, spFlags: 0)
!1676 = !DISubroutineType(types: !1677)
!1677 = !{!88, !1503, !1633}
!1678 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1679, file: !1620, line: 150)
!1679 = !DISubprogram(name: "fwide", scope: !1630, file: !1630, line: 725, type: !1680, flags: DIFlagPrototyped, spFlags: 0)
!1680 = !DISubroutineType(types: !1681)
!1681 = !{!88, !1634, !88}
!1682 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1683, file: !1620, line: 151)
!1683 = !DISubprogram(name: "getwc", scope: !1630, file: !1630, line: 936, type: !1664, flags: DIFlagPrototyped, spFlags: 0)
!1684 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1685, file: !1620, line: 152)
!1685 = !DISubprogram(name: "putwc", scope: !1630, file: !1630, line: 950, type: !1672, flags: DIFlagPrototyped, spFlags: 0)
!1686 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1687, file: !1620, line: 153)
!1687 = !DISubprogram(name: "ungetwc", scope: !1630, file: !1630, line: 979, type: !1688, flags: DIFlagPrototyped, spFlags: 0)
!1688 = !DISubroutineType(types: !1689)
!1689 = !{!1560, !1560, !1634}
!1690 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1691, file: !1620, line: 154)
!1691 = !DISubprogram(name: "wcstod", scope: !1630, file: !1630, line: 402, type: !1692, flags: DIFlagPrototyped, spFlags: 0)
!1692 = !DISubroutineType(types: !1693)
!1693 = !{!977, !1503, !1694}
!1694 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1695)
!1695 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1489, size: 64)
!1696 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1697, file: !1620, line: 155)
!1697 = !DISubprogram(name: "wcstof", scope: !1630, file: !1630, line: 407, type: !1698, flags: DIFlagPrototyped, spFlags: 0)
!1698 = !DISubroutineType(types: !1699)
!1699 = !{!1044, !1503, !1694}
!1700 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1701, file: !1620, line: 156)
!1701 = !DISubprogram(name: "wcstold", scope: !1630, file: !1630, line: 409, type: !1702, flags: DIFlagPrototyped, spFlags: 0)
!1702 = !DISubroutineType(types: !1703)
!1703 = !{!1022, !1503, !1694}
!1704 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1705, file: !1620, line: 157)
!1705 = !DISubprogram(name: "wcstol", linkageName: "__isoc23_wcstol", scope: !1630, file: !1630, line: 500, type: !1706, flags: DIFlagPrototyped, spFlags: 0)
!1706 = !DISubroutineType(types: !1707)
!1707 = !{!108, !1503, !1694, !88}
!1708 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1709, file: !1620, line: 158)
!1709 = !DISubprogram(name: "wcstoll", linkageName: "__isoc23_wcstoll", scope: !1630, file: !1630, line: 508, type: !1710, flags: DIFlagPrototyped, spFlags: 0)
!1710 = !DISubroutineType(types: !1711)
!1711 = !{!48, !1503, !1694, !88}
!1712 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1713, file: !1620, line: 159)
!1713 = !DISubprogram(name: "wcstoul", linkageName: "__isoc23_wcstoul", scope: !1630, file: !1630, line: 503, type: !1714, flags: DIFlagPrototyped, spFlags: 0)
!1714 = !DISubroutineType(types: !1715)
!1715 = !{!760, !1503, !1694, !88}
!1716 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1717, file: !1620, line: 160)
!1717 = !DISubprogram(name: "wcstoull", linkageName: "__isoc23_wcstoull", scope: !1630, file: !1630, line: 513, type: !1718, flags: DIFlagPrototyped, spFlags: 0)
!1718 = !DISubroutineType(types: !1719)
!1719 = !{!26, !1503, !1694, !88}
!1720 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1721, file: !1620, line: 161)
!1721 = !DISubprogram(name: "wcscpy", scope: !1630, file: !1630, line: 98, type: !1722, flags: DIFlagPrototyped, spFlags: 0)
!1722 = !DISubroutineType(types: !1723)
!1723 = !{!1489, !1488, !1503}
!1724 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1725, file: !1620, line: 162)
!1725 = !DISubprogram(name: "wcsncpy", scope: !1630, file: !1630, line: 103, type: !1726, flags: DIFlagPrototyped, spFlags: 0)
!1726 = !DISubroutineType(types: !1727)
!1727 = !{!1489, !1488, !1503, !758}
!1728 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1729, file: !1620, line: 163)
!1729 = !DISubprogram(name: "wcscat", scope: !1630, file: !1630, line: 121, type: !1722, flags: DIFlagPrototyped, spFlags: 0)
!1730 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1731, file: !1620, line: 164)
!1731 = !DISubprogram(name: "wcsncat", scope: !1630, file: !1630, line: 125, type: !1726, flags: DIFlagPrototyped, spFlags: 0)
!1732 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1733, file: !1620, line: 165)
!1733 = !DISubprogram(name: "wcscmp", scope: !1630, file: !1630, line: 130, type: !1734, flags: DIFlagPrototyped, spFlags: 0)
!1734 = !DISubroutineType(types: !1735)
!1735 = !{!88, !1504, !1504}
!1736 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1737, file: !1620, line: 166)
!1737 = !DISubprogram(name: "wcscoll", scope: !1630, file: !1630, line: 155, type: !1734, flags: DIFlagPrototyped, spFlags: 0)
!1738 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1739, file: !1620, line: 167)
!1739 = !DISubprogram(name: "wcsncmp", scope: !1630, file: !1630, line: 133, type: !1740, flags: DIFlagPrototyped, spFlags: 0)
!1740 = !DISubroutineType(types: !1741)
!1741 = !{!88, !1504, !1504, !758}
!1742 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1743, file: !1620, line: 168)
!1743 = !DISubprogram(name: "wcsxfrm", scope: !1630, file: !1630, line: 159, type: !1744, flags: DIFlagPrototyped, spFlags: 0)
!1744 = !DISubroutineType(types: !1745)
!1745 = !{!758, !1488, !1503, !758}
!1746 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1747, file: !1620, line: 169)
!1747 = !DISubprogram(name: "wcschr", scope: !1630, file: !1630, line: 186, type: !1748, flags: DIFlagPrototyped, spFlags: 0)
!1748 = !DISubroutineType(types: !1749)
!1749 = !{!1504, !1504, !1490}
!1750 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1751, file: !1620, line: 170)
!1751 = !DISubprogram(name: "wcspbrk", scope: !1630, file: !1630, line: 222, type: !1752, flags: DIFlagPrototyped, spFlags: 0)
!1752 = !DISubroutineType(types: !1753)
!1753 = !{!1504, !1504, !1504}
!1754 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1755, file: !1620, line: 171)
!1755 = !DISubprogram(name: "wcsrchr", scope: !1630, file: !1630, line: 196, type: !1748, flags: DIFlagPrototyped, spFlags: 0)
!1756 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1757, file: !1620, line: 172)
!1757 = !DISubprogram(name: "wcsstr", scope: !1630, file: !1630, line: 233, type: !1752, flags: DIFlagPrototyped, spFlags: 0)
!1758 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1759, file: !1620, line: 173)
!1759 = !DISubprogram(name: "wmemchr", scope: !1630, file: !1630, line: 274, type: !1760, flags: DIFlagPrototyped, spFlags: 0)
!1760 = !DISubroutineType(types: !1761)
!1761 = !{!1504, !1504, !1490, !758}
!1762 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1763, file: !1620, line: 174)
!1763 = !DISubprogram(name: "wcscspn", scope: !1630, file: !1630, line: 212, type: !1764, flags: DIFlagPrototyped, spFlags: 0)
!1764 = !DISubroutineType(types: !1765)
!1765 = !{!758, !1504, !1504}
!1766 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1767, file: !1620, line: 175)
!1767 = !DISubprogram(name: "wcslen", scope: !1630, file: !1630, line: 247, type: !1768, flags: DIFlagPrototyped, spFlags: 0)
!1768 = !DISubroutineType(types: !1769)
!1769 = !{!758, !1504}
!1770 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1771, file: !1620, line: 176)
!1771 = !DISubprogram(name: "wcsspn", scope: !1630, file: !1630, line: 216, type: !1764, flags: DIFlagPrototyped, spFlags: 0)
!1772 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1773, file: !1620, line: 177)
!1773 = !DISubprogram(name: "wcstok", scope: !1630, file: !1630, line: 242, type: !1774, flags: DIFlagPrototyped, spFlags: 0)
!1774 = !DISubroutineType(types: !1775)
!1775 = !{!1489, !1488, !1503, !1694}
!1776 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1777, file: !1620, line: 178)
!1777 = !DISubprogram(name: "wmemcmp", scope: !1630, file: !1630, line: 283, type: !1740, flags: DIFlagPrototyped, spFlags: 0)
!1778 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1779, file: !1620, line: 179)
!1779 = !DISubprogram(name: "wmemcpy", scope: !1630, file: !1630, line: 287, type: !1726, flags: DIFlagPrototyped, spFlags: 0)
!1780 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1781, file: !1620, line: 180)
!1781 = !DISubprogram(name: "wmemmove", scope: !1630, file: !1630, line: 292, type: !1782, flags: DIFlagPrototyped, spFlags: 0)
!1782 = !DISubroutineType(types: !1783)
!1783 = !{!1489, !1489, !1504, !758}
!1784 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1785, file: !1620, line: 181)
!1785 = !DISubprogram(name: "wmemset", scope: !1630, file: !1630, line: 296, type: !1786, flags: DIFlagPrototyped, spFlags: 0)
!1786 = !DISubroutineType(types: !1787)
!1787 = !{!1489, !1489, !1490, !758}
!1788 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1789, file: !1620, line: 182)
!1789 = !DISubprogram(name: "wcsftime", scope: !1630, file: !1630, line: 1043, type: !1790, flags: DIFlagPrototyped, spFlags: 0)
!1790 = !DISubroutineType(types: !1791)
!1791 = !{!758, !1488, !758, !1503, !1010}
!1792 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1793, file: !1620, line: 183)
!1793 = !DISubprogram(name: "btowc", scope: !1630, file: !1630, line: 309, type: !1794, flags: DIFlagPrototyped, spFlags: 0)
!1794 = !DISubroutineType(types: !1795)
!1795 = !{!1560, !88}
!1796 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1797, file: !1620, line: 184)
!1797 = !DISubprogram(name: "wctob", scope: !1630, file: !1630, line: 313, type: !1573, flags: DIFlagPrototyped, spFlags: 0)
!1798 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1799, file: !1620, line: 185)
!1799 = !DISubprogram(name: "mbsinit", scope: !1630, file: !1630, line: 317, type: !1800, flags: DIFlagPrototyped, spFlags: 0)
!1800 = !DISubroutineType(types: !1801)
!1801 = !{!88, !1802}
!1802 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1803, size: 64)
!1803 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !1513)
!1804 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1805, file: !1620, line: 186)
!1805 = !DISubprogram(name: "mbrlen", scope: !1630, file: !1630, line: 332, type: !1806, flags: DIFlagPrototyped, spFlags: 0)
!1806 = !DISubroutineType(types: !1807)
!1807 = !{!758, !882, !758, !1808}
!1808 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1809)
!1809 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1513, size: 64)
!1810 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1811, file: !1620, line: 187)
!1811 = !DISubprogram(name: "mbrtowc", scope: !1630, file: !1630, line: 321, type: !1812, flags: DIFlagPrototyped, spFlags: 0)
!1812 = !DISubroutineType(types: !1813)
!1813 = !{!758, !1488, !882, !758, !1808}
!1814 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1815, file: !1620, line: 188)
!1815 = !DISubprogram(name: "wcrtomb", scope: !1630, file: !1630, line: 326, type: !1816, flags: DIFlagPrototyped, spFlags: 0)
!1816 = !DISubroutineType(types: !1817)
!1817 = !{!758, !881, !1490, !1808}
!1818 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1819, file: !1620, line: 189)
!1819 = !DISubprogram(name: "mbsrtowcs", scope: !1630, file: !1630, line: 362, type: !1820, flags: DIFlagPrototyped, spFlags: 0)
!1820 = !DISubroutineType(types: !1821)
!1821 = !{!758, !1488, !1822, !758, !1808}
!1822 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1823)
!1823 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !883, size: 64)
!1824 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1825, file: !1620, line: 190)
!1825 = !DISubprogram(name: "wcsrtombs", scope: !1630, file: !1630, line: 368, type: !1826, flags: DIFlagPrototyped, spFlags: 0)
!1826 = !DISubroutineType(types: !1827)
!1827 = !{!758, !881, !1828, !758, !1808}
!1828 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1829)
!1829 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1504, size: 64)
!1830 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1831, file: !1620, line: 192)
!1831 = !DISubprogram(name: "getwchar", scope: !1630, file: !1630, line: 942, type: !1832, flags: DIFlagPrototyped, spFlags: 0)
!1832 = !DISubroutineType(types: !1833)
!1833 = !{!1560}
!1834 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1835, file: !1620, line: 193)
!1835 = !DISubprogram(name: "vwscanf", linkageName: "__isoc23_vwscanf", scope: !1630, file: !1630, line: 879, type: !1836, flags: DIFlagPrototyped, spFlags: 0)
!1836 = !DISubroutineType(types: !1837)
!1837 = !{!88, !1503, !1646}
!1838 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1839, file: !1620, line: 194)
!1839 = !DISubprogram(name: "wscanf", linkageName: "__isoc23_wscanf", scope: !1630, file: !1630, line: 799, type: !1840, flags: DIFlagPrototyped, spFlags: 0)
!1840 = !DISubroutineType(types: !1841)
!1841 = !{!88, !1503, null}
!1842 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1843, file: !1620, line: 196)
!1843 = !DISubprogram(name: "putwchar", scope: !1630, file: !1630, line: 956, type: !1844, flags: DIFlagPrototyped, spFlags: 0)
!1844 = !DISubroutineType(types: !1845)
!1845 = !{!1560, !1490}
!1846 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1847, file: !1620, line: 197)
!1847 = !DISubprogram(name: "vwprintf", scope: !1630, file: !1630, line: 758, type: !1836, flags: DIFlagPrototyped, spFlags: 0)
!1848 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1849, file: !1620, line: 198)
!1849 = !DISubprogram(name: "wprintf", scope: !1630, file: !1630, line: 739, type: !1840, flags: DIFlagPrototyped, spFlags: 0)
!1850 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1624, file: !1851, line: 120)
!1851 = !DIFile(filename: "libc++_replacement/tmp/R68/build-baseline-x86_64/libcxx/test-suite-install/include/c++/v1/cstdio", directory: "/home/toolchain/development")
!1852 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1853, file: !1851, line: 121)
!1853 = !DIDerivedType(tag: DW_TAG_typedef, name: "fpos_t", file: !1854, line: 85, baseType: !1855)
!1854 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/stdio.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "bf878b5a7be9bd3141cebb72b92597e8")
!1855 = !DIDerivedType(tag: DW_TAG_typedef, name: "__fpos_t", file: !1856, line: 14, baseType: !1857)
!1856 = !DIFile(filename: "libc++_replacement/tmp/GBS-ROOT/R32-candidates-libcxx-x86_64-20260813-02/local/BUILD-ROOTS/scratch.x86_64.0/usr/include/bits/types/__fpos_t.h", directory: "/home/toolchain/development", checksumkind: CSK_MD5, checksum: "32de8bdaf3551a6c0a9394f9af4389ce")
!1857 = !DICompositeType(tag: DW_TAG_structure_type, name: "_G_fpos_t", file: !1856, line: 10, size: 128, flags: DIFlagFwdDecl, identifier: "_ZTS9_G_fpos_t")
!1858 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1859, file: !1851, line: 123)
!1859 = !DISubprogram(name: "fclose", scope: !1854, file: !1854, line: 184, type: !1860, flags: DIFlagPrototyped, spFlags: 0)
!1860 = !DISubroutineType(types: !1861)
!1861 = !{!88, !1862}
!1862 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1624, size: 64)
!1863 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1864, file: !1851, line: 124)
!1864 = !DISubprogram(name: "fflush", scope: !1854, file: !1854, line: 236, type: !1860, flags: DIFlagPrototyped, spFlags: 0)
!1865 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1866, file: !1851, line: 125)
!1866 = !DISubprogram(name: "setbuf", scope: !1854, file: !1854, line: 334, type: !1867, flags: DIFlagPrototyped, spFlags: 0)
!1867 = !DISubroutineType(types: !1868)
!1868 = !{null, !1869, !881}
!1869 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1862)
!1870 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1871, file: !1851, line: 126)
!1871 = !DISubprogram(name: "setvbuf", scope: !1854, file: !1854, line: 339, type: !1872, flags: DIFlagPrototyped, spFlags: 0)
!1872 = !DISubroutineType(types: !1873)
!1873 = !{!88, !1869, !881, !88, !758}
!1874 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1875, file: !1851, line: 127)
!1875 = !DISubprogram(name: "fprintf", scope: !1854, file: !1854, line: 357, type: !1876, flags: DIFlagPrototyped, spFlags: 0)
!1876 = !DISubroutineType(types: !1877)
!1877 = !{!88, !1869, !882, null}
!1878 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1879, file: !1851, line: 128)
!1879 = !DISubprogram(name: "fscanf", linkageName: "__isoc23_fscanf", scope: !1854, file: !1854, line: 442, type: !1876, flags: DIFlagPrototyped, spFlags: 0)
!1880 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1881, file: !1851, line: 129)
!1881 = !DISubprogram(name: "snprintf", scope: !1854, file: !1854, line: 385, type: !1882, flags: DIFlagPrototyped, spFlags: 0)
!1882 = !DISubroutineType(types: !1883)
!1883 = !{!88, !881, !758, !882, null}
!1884 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1885, file: !1851, line: 130)
!1885 = !DISubprogram(name: "sprintf", scope: !1854, file: !1854, line: 365, type: !1886, flags: DIFlagPrototyped, spFlags: 0)
!1886 = !DISubroutineType(types: !1887)
!1887 = !{!88, !881, !882, null}
!1888 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1889, file: !1851, line: 131)
!1889 = !DISubprogram(name: "sscanf", linkageName: "__isoc23_sscanf", scope: !1854, file: !1854, line: 447, type: !1890, flags: DIFlagPrototyped, spFlags: 0)
!1890 = !DISubroutineType(types: !1891)
!1891 = !{!88, !882, !882, null}
!1892 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1893, file: !1851, line: 132)
!1893 = !DISubprogram(name: "vfprintf", scope: !1854, file: !1854, line: 372, type: !1894, flags: DIFlagPrototyped, spFlags: 0)
!1894 = !DISubroutineType(types: !1895)
!1895 = !{!88, !1869, !882, !1646}
!1896 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1897, file: !1851, line: 133)
!1897 = !DISubprogram(name: "vfscanf", linkageName: "__isoc23_vfscanf", scope: !1854, file: !1854, line: 511, type: !1894, flags: DIFlagPrototyped, spFlags: 0)
!1898 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1899, file: !1851, line: 134)
!1899 = !DISubprogram(name: "vsscanf", linkageName: "__isoc23_vsscanf", scope: !1854, file: !1854, line: 519, type: !1900, flags: DIFlagPrototyped, spFlags: 0)
!1900 = !DISubroutineType(types: !1901)
!1901 = !{!88, !882, !882, !1646}
!1902 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1903, file: !1851, line: 135)
!1903 = !DISubprogram(name: "vsnprintf", scope: !1854, file: !1854, line: 389, type: !1904, flags: DIFlagPrototyped, spFlags: 0)
!1904 = !DISubroutineType(types: !1905)
!1905 = !{!88, !881, !758, !882, !1646}
!1906 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1907, file: !1851, line: 136)
!1907 = !DISubprogram(name: "vsprintf", scope: !1854, file: !1854, line: 380, type: !1908, flags: DIFlagPrototyped, spFlags: 0)
!1908 = !DISubroutineType(types: !1909)
!1909 = !{!88, !881, !882, !1646}
!1910 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1911, file: !1851, line: 137)
!1911 = !DISubprogram(name: "fgetc", scope: !1854, file: !1854, line: 575, type: !1860, flags: DIFlagPrototyped, spFlags: 0)
!1912 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1913, file: !1851, line: 138)
!1913 = !DISubprogram(name: "fgets", scope: !1854, file: !1854, line: 654, type: !1914, flags: DIFlagPrototyped, spFlags: 0)
!1914 = !DISubroutineType(types: !1915)
!1915 = !{!880, !881, !88, !1869}
!1916 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1917, file: !1851, line: 139)
!1917 = !DISubprogram(name: "fputc", scope: !1854, file: !1854, line: 611, type: !1918, flags: DIFlagPrototyped, spFlags: 0)
!1918 = !DISubroutineType(types: !1919)
!1919 = !{!88, !88, !1862}
!1920 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1921, file: !1851, line: 140)
!1921 = !DISubprogram(name: "fputs", scope: !1854, file: !1854, line: 707, type: !1922, flags: DIFlagPrototyped, spFlags: 0)
!1922 = !DISubroutineType(types: !1923)
!1923 = !{!88, !882, !1869}
!1924 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1925, file: !1851, line: 141)
!1925 = !DISubprogram(name: "getc", scope: !1854, file: !1854, line: 576, type: !1860, flags: DIFlagPrototyped, spFlags: 0)
!1926 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1927, file: !1851, line: 142)
!1927 = !DISubprogram(name: "putc", scope: !1854, file: !1854, line: 612, type: !1918, flags: DIFlagPrototyped, spFlags: 0)
!1928 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1929, file: !1851, line: 143)
!1929 = !DISubprogram(name: "ungetc", scope: !1854, file: !1854, line: 721, type: !1918, flags: DIFlagPrototyped, spFlags: 0)
!1930 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1931, file: !1851, line: 144)
!1931 = !DISubprogram(name: "fread", scope: !1854, file: !1854, line: 728, type: !1932, flags: DIFlagPrototyped, spFlags: 0)
!1932 = !DISubroutineType(types: !1933)
!1933 = !{!758, !867, !758, !758, !1869}
!1934 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1935, file: !1851, line: 145)
!1935 = !DISubprogram(name: "fwrite", scope: !1854, file: !1854, line: 735, type: !1936, flags: DIFlagPrototyped, spFlags: 0)
!1936 = !DISubroutineType(types: !1937)
!1937 = !{!758, !868, !758, !758, !1869}
!1938 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1939, file: !1851, line: 146)
!1939 = !DISubprogram(name: "fgetpos", scope: !1854, file: !1854, line: 819, type: !1940, flags: DIFlagPrototyped, spFlags: 0)
!1940 = !DISubroutineType(types: !1941)
!1941 = !{!88, !1869, !1942}
!1942 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !1943)
!1943 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1853, size: 64)
!1944 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1945, file: !1851, line: 147)
!1945 = !DISubprogram(name: "fseek", scope: !1854, file: !1854, line: 769, type: !1946, flags: DIFlagPrototyped, spFlags: 0)
!1946 = !DISubroutineType(types: !1947)
!1947 = !{!88, !1862, !108, !88}
!1948 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1949, file: !1851, line: 148)
!1949 = !DISubprogram(name: "fsetpos", scope: !1854, file: !1854, line: 825, type: !1950, flags: DIFlagPrototyped, spFlags: 0)
!1950 = !DISubroutineType(types: !1951)
!1951 = !{!88, !1862, !1952}
!1952 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1953, size: 64)
!1953 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !1853)
!1954 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1955, file: !1851, line: 149)
!1955 = !DISubprogram(name: "ftell", scope: !1854, file: !1854, line: 775, type: !1956, flags: DIFlagPrototyped, spFlags: 0)
!1956 = !DISubroutineType(types: !1957)
!1957 = !{!108, !1862}
!1958 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1959, file: !1851, line: 150)
!1959 = !DISubprogram(name: "rewind", scope: !1854, file: !1854, line: 780, type: !1960, flags: DIFlagPrototyped, spFlags: 0)
!1960 = !DISubroutineType(types: !1961)
!1961 = !{null, !1862}
!1962 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1963, file: !1851, line: 151)
!1963 = !DISubprogram(name: "clearerr", scope: !1854, file: !1854, line: 850, type: !1960, flags: DIFlagPrototyped, spFlags: 0)
!1964 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1965, file: !1851, line: 152)
!1965 = !DISubprogram(name: "feof", scope: !1854, file: !1854, line: 852, type: !1860, flags: DIFlagPrototyped, spFlags: 0)
!1966 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1967, file: !1851, line: 153)
!1967 = !DISubprogram(name: "ferror", scope: !1854, file: !1854, line: 854, type: !1860, flags: DIFlagPrototyped, spFlags: 0)
!1968 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1969, file: !1851, line: 154)
!1969 = !DISubprogram(name: "perror", scope: !1854, file: !1854, line: 868, type: !1970, flags: DIFlagPrototyped, spFlags: 0)
!1970 = !DISubroutineType(types: !1971)
!1971 = !{null, !883}
!1972 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1973, file: !1851, line: 156)
!1973 = !DISubprogram(name: "fopen", scope: !1854, file: !1854, line: 264, type: !1974, flags: DIFlagPrototyped, spFlags: 0)
!1974 = !DISubroutineType(types: !1975)
!1975 = !{!1862, !882, !882}
!1976 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1977, file: !1851, line: 157)
!1977 = !DISubprogram(name: "freopen", scope: !1854, file: !1854, line: 271, type: !1978, flags: DIFlagPrototyped, spFlags: 0)
!1978 = !DISubroutineType(types: !1979)
!1979 = !{!1862, !882, !882, !1869}
!1980 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1981, file: !1851, line: 158)
!1981 = !DISubprogram(name: "remove", scope: !1854, file: !1854, line: 158, type: !1360, flags: DIFlagPrototyped, spFlags: 0)
!1982 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1983, file: !1851, line: 159)
!1983 = !DISubprogram(name: "rename", scope: !1854, file: !1854, line: 160, type: !898, flags: DIFlagPrototyped, spFlags: 0)
!1984 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1985, file: !1851, line: 160)
!1985 = !DISubprogram(name: "tmpfile", scope: !1854, file: !1854, line: 194, type: !1986, flags: DIFlagPrototyped, spFlags: 0)
!1986 = !DISubroutineType(types: !1987)
!1987 = !{!1862}
!1988 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1989, file: !1851, line: 161)
!1989 = !DISubprogram(name: "tmpnam", scope: !1854, file: !1854, line: 211, type: !1990, flags: DIFlagPrototyped, spFlags: 0)
!1990 = !DISubroutineType(types: !1991)
!1991 = !{!880, !880}
!1992 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1993, file: !1851, line: 163)
!1993 = !DISubprogram(name: "getchar", scope: !1854, file: !1854, line: 582, type: !1402, flags: DIFlagPrototyped, spFlags: 0)
!1994 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1995, file: !1851, line: 167)
!1995 = !DISubprogram(name: "scanf", linkageName: "__isoc23_scanf", scope: !1854, file: !1854, line: 445, type: !1996, flags: DIFlagPrototyped, spFlags: 0)
!1996 = !DISubroutineType(types: !1997)
!1997 = !{!88, !882, null}
!1998 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !1999, file: !1851, line: 168)
!1999 = !DISubprogram(name: "vscanf", linkageName: "__isoc23_vscanf", scope: !1854, file: !1854, line: 516, type: !2000, flags: DIFlagPrototyped, spFlags: 0)
!2000 = !DISubroutineType(types: !2001)
!2001 = !{!88, !882, !1646}
!2002 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2003, file: !1851, line: 170)
!2003 = !DISubprogram(name: "printf", scope: !1854, file: !1854, line: 363, type: !1996, flags: DIFlagPrototyped, spFlags: 0)
!2004 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2005, file: !1851, line: 171)
!2005 = !DISubprogram(name: "putchar", scope: !1854, file: !1854, line: 618, type: !1459, flags: DIFlagPrototyped, spFlags: 0)
!2006 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2007, file: !1851, line: 172)
!2007 = !DISubprogram(name: "puts", scope: !1854, file: !1854, line: 714, type: !1360, flags: DIFlagPrototyped, spFlags: 0)
!2008 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !6, entity: !2009, file: !1851, line: 173)
!2009 = !DISubprogram(name: "vprintf", scope: !1854, file: !1854, line: 378, type: !2000, flags: DIFlagPrototyped, spFlags: 0)
!2010 = !{i32 7, !"Dwarf Version", i32 5}
!2011 = !{i32 2, !"Debug Info Version", i32 3}
!2012 = !{i32 1, !"wchar_size", i32 4}
!2013 = !{i32 8, !"PIC Level", i32 2}
!2014 = !{i32 7, !"uwtable", i32 2}
!2015 = !{i32 7, !"frame-pointer", i32 2}
!2016 = !{!"clang version 22.1.8"}
!2017 = distinct !DISubprogram(name: "__cxx_global_var_init", scope: !3, file: !3, type: !1426, flags: DIFlagArtificial, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !744)
!2018 = !DILocation(line: 0, scope: !2017)
!2019 = distinct !DISubprogram(name: "pthread_mutex_unlock", scope: !3, file: !3, line: 73, type: !344, scopeLine: 73, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, retainedNodes: !246)
!2020 = !DILocalVariable(name: "mutex", arg: 1, scope: !2019, file: !3, line: 73, type: !346)
!2021 = !DILocation(line: 73, column: 54, scope: !2019)
!2022 = !DILocation(line: 74, column: 7, scope: !2023)
!2023 = distinct !DILexicalBlock(scope: !2019, file: !3, line: 74, column: 7)
!2024 = !DILocation(line: 74, column: 19, scope: !2023)
!2025 = !DILocation(line: 75, column: 46, scope: !2023)
!2026 = !DILocation(line: 75, column: 17, scope: !2023)
!2027 = !DILocation(line: 75, column: 5, scope: !2023)
!2028 = !DILocation(line: 76, column: 20, scope: !2029)
!2029 = distinct !DILexicalBlock(scope: !2019, file: !3, line: 76, column: 7)
!2030 = !DILocation(line: 76, column: 52, scope: !2029)
!2031 = !DILocation(line: 76, column: 55, scope: !2029)
!2032 = !DILocation(line: 76, column: 64, scope: !2029)
!2033 = !DILocation(line: 76, column: 61, scope: !2029)
!2034 = !DILocation(line: 77, column: 25, scope: !2029)
!2035 = !DILocation(line: 77, column: 5, scope: !2029)
!2036 = !DILocation(line: 78, column: 7, scope: !2037)
!2037 = distinct !DILexicalBlock(scope: !2019, file: !3, line: 78, column: 7)
!2038 = !DILocation(line: 78, column: 19, scope: !2037)
!2039 = !DILocation(line: 79, column: 5, scope: !2037)
!2040 = !DILocation(line: 80, column: 10, scope: !2019)
!2041 = !DILocation(line: 80, column: 22, scope: !2019)
!2042 = !DILocation(line: 80, column: 3, scope: !2019)
!2043 = distinct !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIbLb0EE4loadB9nqn220108ENS_12memory_orderE", scope: !395, file: !392, line: 70, type: !458, scopeLine: 71, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, declaration: !457, retainedNodes: !246)
!2044 = !DILocalVariable(name: "this", arg: 1, scope: !2043, type: !2045, flags: DIFlagArtificial | DIFlagObjectPointer)
!2045 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !436, size: 64)
!2046 = !DILocation(line: 0, scope: !2043)
!2047 = !DILocalVariable(name: "__m", arg: 2, scope: !2043, file: !392, line: 70, type: !441)
!2048 = !DILocation(line: 70, column: 65, scope: !2043)
!2049 = !DILocation(line: 72, column: 50, scope: !2043)
!2050 = !DILocation(line: 72, column: 57, scope: !2043)
!2051 = !DILocation(line: 72, column: 12, scope: !2043)
!2052 = !DILocation(line: 72, column: 5, scope: !2043)
!2053 = distinct !DISubprogram(name: "fetch_add", linkageName: "_ZNSt3__113__atomic_baseIiLb1EE9fetch_addB9nqn220108EiNS_12memory_orderE", scope: !538, file: !392, line: 156, type: !661, scopeLine: 156, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, declaration: !660, retainedNodes: !246)
!2054 = !DILocalVariable(name: "this", arg: 1, scope: !2053, type: !2055, flags: DIFlagArtificial | DIFlagObjectPointer)
!2055 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !538, size: 64)
!2056 = !DILocation(line: 0, scope: !2053)
!2057 = !DILocalVariable(name: "__op", arg: 2, scope: !2053, file: !392, line: 156, type: !88)
!2058 = !DILocation(line: 156, column: 43, scope: !2053)
!2059 = !DILocalVariable(name: "__m", arg: 3, scope: !2053, file: !392, line: 156, type: !441)
!2060 = !DILocation(line: 156, column: 62, scope: !2053)
!2061 = !DILocation(line: 157, column: 61, scope: !2053)
!2062 = !DILocation(line: 157, column: 68, scope: !2053)
!2063 = !DILocation(line: 157, column: 74, scope: !2053)
!2064 = !DILocation(line: 157, column: 12, scope: !2053)
!2065 = !DILocation(line: 157, column: 5, scope: !2053)
!2066 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 83, type: !1402, scopeLine: 83, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, retainedNodes: !246)
!2067 = !DILocation(line: 84, column: 16, scope: !2066)
!2068 = !DILocation(line: 84, column: 3, scope: !2066)
!2069 = !DILocation(line: 85, column: 3, scope: !2066)
!2070 = !DILocation(line: 86, column: 44, scope: !2066)
!2071 = !DILocation(line: 86, column: 15, scope: !2066)
!2072 = !DILocation(line: 87, column: 7, scope: !2073)
!2073 = distinct !DILexicalBlock(scope: !2066, file: !3, line: 87, column: 7)
!2074 = !DILocation(line: 87, column: 19, scope: !2073)
!2075 = !DILocation(line: 88, column: 5, scope: !2076)
!2076 = distinct !DILexicalBlock(scope: !2073, file: !3, line: 87, column: 31)
!2077 = !DILocation(line: 89, column: 5, scope: !2076)
!2078 = !DILocation(line: 91, column: 32, scope: !2066)
!2079 = !DILocation(line: 91, column: 17, scope: !2066)
!2080 = !DILocalVariable(name: "thread", scope: !2066, file: !3, line: 93, type: !2081)
!2081 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !13, line: 27, baseType: !760)
!2082 = !DILocation(line: 93, column: 13, scope: !2066)
!2083 = !DILocalVariable(name: "create_rc", scope: !2066, file: !3, line: 94, type: !88)
!2084 = !DILocation(line: 94, column: 7, scope: !2066)
!2085 = !DILocation(line: 94, column: 19, scope: !2066)
!2086 = !DILocation(line: 95, column: 39, scope: !2066)
!2087 = !DILocation(line: 95, column: 3, scope: !2066)
!2088 = !DILocation(line: 96, column: 7, scope: !2089)
!2089 = distinct !DILexicalBlock(scope: !2066, file: !3, line: 96, column: 7)
!2090 = !DILocation(line: 96, column: 17, scope: !2089)
!2091 = !DILocation(line: 97, column: 5, scope: !2089)
!2092 = !DILocation(line: 98, column: 3, scope: !2066)
!2093 = !DILocation(line: 98, column: 17, scope: !2066)
!2094 = !DILocation(line: 98, column: 10, scope: !2066)
!2095 = !DILocation(line: 99, column: 5, scope: !2066)
!2096 = distinct !{!2096, !2092, !2097, !2098}
!2097 = !DILocation(line: 99, column: 16, scope: !2066)
!2098 = !{!"llvm.loop.mustprogress"}
!2099 = !DILocation(line: 101, column: 16, scope: !2066)
!2100 = !DILocation(line: 102, column: 16, scope: !2066)
!2101 = !DILocation(line: 103, column: 3, scope: !2066)
!2102 = !DILocation(line: 104, column: 16, scope: !2066)
!2103 = !DILocalVariable(name: "cancel_rc", scope: !2066, file: !3, line: 105, type: !88)
!2104 = !DILocation(line: 105, column: 7, scope: !2066)
!2105 = !DILocation(line: 105, column: 34, scope: !2066)
!2106 = !DILocation(line: 105, column: 19, scope: !2066)
!2107 = !DILocalVariable(name: "result", scope: !2066, file: !3, line: 106, type: !756)
!2108 = !DILocation(line: 106, column: 9, scope: !2066)
!2109 = !DILocalVariable(name: "join_rc", scope: !2066, file: !3, line: 107, type: !88)
!2110 = !DILocation(line: 107, column: 7, scope: !2066)
!2111 = !DILocation(line: 107, column: 30, scope: !2066)
!2112 = !DILocation(line: 107, column: 17, scope: !2066)
!2113 = !DILocation(line: 108, column: 16, scope: !2066)
!2114 = !DILocalVariable(name: "unlocks", scope: !2066, file: !3, line: 109, type: !88)
!2115 = !DILocation(line: 109, column: 7, scope: !2066)
!2116 = !DILocation(line: 109, column: 37, scope: !2066)
!2117 = !DILocalVariable(name: "cleanups", scope: !2066, file: !3, line: 110, type: !88)
!2118 = !DILocation(line: 110, column: 7, scope: !2066)
!2119 = !DILocation(line: 110, column: 38, scope: !2066)
!2120 = !DILocation(line: 112, column: 15, scope: !2066)
!2121 = !DILocation(line: 113, column: 15, scope: !2066)
!2122 = !DILocation(line: 114, column: 15, scope: !2066)
!2123 = !DILocation(line: 114, column: 22, scope: !2066)
!2124 = !DILocation(line: 115, column: 15, scope: !2066)
!2125 = !DILocation(line: 116, column: 15, scope: !2066)
!2126 = !DILocation(line: 111, column: 3, scope: !2066)
!2127 = !DILocalVariable(name: "try_rc", scope: !2066, file: !3, line: 118, type: !88)
!2128 = !DILocation(line: 118, column: 7, scope: !2066)
!2129 = !DILocation(line: 118, column: 38, scope: !2066)
!2130 = !DILocation(line: 118, column: 16, scope: !2066)
!2131 = !DILocation(line: 119, column: 54, scope: !2066)
!2132 = !DILocation(line: 119, column: 3, scope: !2066)
!2133 = !DILocation(line: 120, column: 7, scope: !2134)
!2134 = distinct !DILexicalBlock(scope: !2066, file: !3, line: 120, column: 7)
!2135 = !DILocation(line: 120, column: 14, scope: !2134)
!2136 = !DILocation(line: 121, column: 5, scope: !2134)
!2137 = !DILocation(line: 121, column: 17, scope: !2134)
!2138 = !DILocalVariable(name: "contender_thread", scope: !2066, file: !3, line: 123, type: !2081)
!2139 = !DILocation(line: 123, column: 13, scope: !2066)
!2140 = !DILocalVariable(name: "contender_create_rc", scope: !2066, file: !3, line: 124, type: !88)
!2141 = !DILocation(line: 124, column: 7, scope: !2066)
!2142 = !DILocation(line: 124, column: 29, scope: !2066)
!2143 = !DILocalVariable(name: "contender_result", scope: !2066, file: !3, line: 125, type: !756)
!2144 = !DILocation(line: 125, column: 9, scope: !2066)
!2145 = !DILocalVariable(name: "contender_join_rc", scope: !2066, file: !3, line: 126, type: !88)
!2146 = !DILocation(line: 126, column: 7, scope: !2066)
!2147 = !DILocation(line: 126, column: 27, scope: !2066)
!2148 = !DILocation(line: 126, column: 47, scope: !2066)
!2149 = !DILocation(line: 126, column: 67, scope: !2066)
!2150 = !DILocation(line: 126, column: 54, scope: !2066)
!2151 = !DILocalVariable(name: "contender_rc", scope: !2066, file: !3, line: 127, type: !88)
!2152 = !DILocation(line: 127, column: 7, scope: !2066)
!2153 = !DILocation(line: 127, column: 66, scope: !2066)
!2154 = !DILocation(line: 127, column: 39, scope: !2066)
!2155 = !DILocation(line: 129, column: 15, scope: !2066)
!2156 = !DILocation(line: 130, column: 15, scope: !2066)
!2157 = !DILocation(line: 131, column: 15, scope: !2066)
!2158 = !DILocation(line: 128, column: 3, scope: !2066)
!2159 = !DILocalVariable(name: "joined_canceled", scope: !2066, file: !3, line: 133, type: !127)
!2160 = !DILocation(line: 133, column: 8, scope: !2066)
!2161 = !DILocation(line: 133, column: 26, scope: !2066)
!2162 = !DILocation(line: 133, column: 36, scope: !2066)
!2163 = !DILocation(line: 133, column: 41, scope: !2066)
!2164 = !DILocation(line: 133, column: 44, scope: !2066)
!2165 = !DILocation(line: 133, column: 52, scope: !2066)
!2166 = !DILocation(line: 133, column: 57, scope: !2066)
!2167 = !DILocation(line: 133, column: 60, scope: !2066)
!2168 = !DILocation(line: 133, column: 67, scope: !2066)
!2169 = !DILocation(line: 0, scope: !2066)
!2170 = !DILocalVariable(name: "clean", scope: !2066, file: !3, line: 139, type: !127)
!2171 = !DILocation(line: 139, column: 8, scope: !2066)
!2172 = !DILocation(line: 139, column: 16, scope: !2066)
!2173 = !DILocation(line: 139, column: 25, scope: !2066)
!2174 = !DILocation(line: 139, column: 30, scope: !2066)
!2175 = !DILocation(line: 139, column: 33, scope: !2066)
!2176 = !DILocation(line: 139, column: 41, scope: !2066)
!2177 = !DILocation(line: 139, column: 46, scope: !2066)
!2178 = !DILocation(line: 139, column: 49, scope: !2066)
!2179 = !DILocation(line: 139, column: 56, scope: !2066)
!2180 = !DILocation(line: 139, column: 61, scope: !2066)
!2181 = !DILocation(line: 139, column: 64, scope: !2066)
!2182 = !DILocation(line: 139, column: 77, scope: !2066)
!2183 = !DILocalVariable(name: "retained_lock", scope: !2066, file: !3, line: 140, type: !127)
!2184 = !DILocation(line: 140, column: 8, scope: !2066)
!2185 = !DILocation(line: 140, column: 24, scope: !2066)
!2186 = !DILocation(line: 140, column: 33, scope: !2066)
!2187 = !DILocation(line: 140, column: 38, scope: !2066)
!2188 = !DILocation(line: 140, column: 41, scope: !2066)
!2189 = !DILocation(line: 140, column: 49, scope: !2066)
!2190 = !DILocation(line: 140, column: 54, scope: !2066)
!2191 = !DILocation(line: 140, column: 57, scope: !2066)
!2192 = !DILocation(line: 140, column: 64, scope: !2066)
!2193 = !DILocation(line: 140, column: 73, scope: !2066)
!2194 = !DILocation(line: 140, column: 76, scope: !2066)
!2195 = !DILocation(line: 140, column: 89, scope: !2066)
!2196 = !DILocalVariable(name: "expected", scope: !2066, file: !3, line: 141, type: !127)
!2197 = !DILocation(line: 141, column: 8, scope: !2066)
!2198 = !DILocation(line: 141, column: 19, scope: !2066)
!2199 = !DILocation(line: 141, column: 35, scope: !2066)
!2200 = !DILocation(line: 141, column: 39, scope: !2066)
!2201 = !DILocation(line: 141, column: 45, scope: !2066)
!2202 = !DILocation(line: 141, column: 48, scope: !2066)
!2203 = !DILocation(line: 143, column: 45, scope: !2066)
!2204 = !DILocation(line: 143, column: 3, scope: !2066)
!2205 = !DILocation(line: 144, column: 10, scope: !2066)
!2206 = !DILocation(line: 144, column: 3, scope: !2066)
!2207 = !DILocation(line: 145, column: 1, scope: !2066)
!2208 = distinct !DISubprogram(name: "terminate_marker", linkageName: "_ZN12_GLOBAL__N_116terminate_markerEv", scope: !2, file: !3, line: 32, type: !1426, scopeLine: 32, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !744, retainedNodes: !246)
!2209 = !DILocalVariable(name: "buffer", scope: !2208, file: !3, line: 33, type: !2210)
!2210 = !DICompositeType(tag: DW_TAG_array_type, baseType: !44, size: 1024, elements: !2211)
!2211 = !{!2212}
!2212 = !DISubrange(count: 128)
!2213 = !DILocation(line: 33, column: 8, scope: !2208)
!2214 = !DILocalVariable(name: "length", scope: !2208, file: !3, line: 34, type: !88)
!2215 = !DILocation(line: 34, column: 7, scope: !2208)
!2216 = !DILocation(line: 34, column: 30, scope: !2208)
!2217 = !DILocation(line: 37, column: 50, scope: !2208)
!2218 = !DILocation(line: 38, column: 50, scope: !2208)
!2219 = !DILocation(line: 34, column: 16, scope: !2208)
!2220 = !DILocation(line: 39, column: 7, scope: !2221)
!2221 = distinct !DILexicalBlock(scope: !2208, file: !3, line: 39, column: 7)
!2222 = !DILocation(line: 39, column: 14, scope: !2221)
!2223 = !DILocation(line: 40, column: 33, scope: !2221)
!2224 = !DILocation(line: 40, column: 61, scope: !2221)
!2225 = !DILocation(line: 40, column: 12, scope: !2221)
!2226 = !DILocation(line: 40, column: 11, scope: !2221)
!2227 = !DILocation(line: 40, column: 5, scope: !2221)
!2228 = !DILocation(line: 41, column: 3, scope: !2208)
!2229 = distinct !DISubprogram(name: "native_handle", linkageName: "_ZNSt3__15mutex13native_handleB9nqn220108Ev", scope: !75, file: !76, line: 44, type: !130, scopeLine: 44, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, declaration: !129, retainedNodes: !246)
!2230 = !DILocalVariable(name: "this", arg: 1, scope: !2229, type: !2231, flags: DIFlagArtificial | DIFlagObjectPointer)
!2231 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !75, size: 64)
!2232 = !DILocation(line: 0, scope: !2229)
!2233 = !DILocation(line: 44, column: 88, scope: !2229)
!2234 = !DILocation(line: 44, column: 80, scope: !2229)
!2235 = distinct !DISubprogram(name: "worker", linkageName: "_ZN12_GLOBAL__N_16workerEPv", scope: !2, file: !3, line: 44, type: !2236, scopeLine: 44, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !744, retainedNodes: !246)
!2236 = !DISubroutineType(types: !2237)
!2237 = !{!756, !756}
!2238 = !DILocalVariable(arg: 1, scope: !2235, file: !3, line: 44, type: !756)
!2239 = !DILocation(line: 44, column: 19, scope: !2235)
!2240 = !DILocation(line: 45, column: 9, scope: !2235)
!2241 = !DILocation(line: 46, column: 9, scope: !2235)
!2242 = !DILocalVariable(name: "lock", scope: !2235, file: !3, line: 47, type: !69)
!2243 = !DILocation(line: 47, column: 32, scope: !2235)
!2244 = !DILocalVariable(name: "cleanup", scope: !2235, file: !3, line: 48, type: !2245)
!2245 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "FrameCleanup", scope: !2, file: !3, line: 24, size: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !2246)
!2246 = !{!2247}
!2247 = !DISubprogram(name: "~FrameCleanup", linkageName: "_ZN12_GLOBAL__N_112FrameCleanupD4Ev", scope: !2245, file: !3, line: 25, type: !2248, scopeLine: 25, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit)
!2248 = !DISubroutineType(types: !2249)
!2249 = !{null, !2250}
!2250 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !2245, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!2251 = !DILocation(line: 48, column: 16, scope: !2235)
!2252 = !DILocation(line: 49, column: 9, scope: !2235)
!2253 = !DILocation(line: 50, column: 3, scope: !2235)
!2254 = !DILocation(line: 51, column: 13, scope: !2235)
!2255 = !DILocation(line: 52, column: 3, scope: !2235)
!2256 = !DILocation(line: 54, column: 1, scope: !2235)
!2257 = distinct !DISubprogram(name: "store", linkageName: "_ZNSt3__113__atomic_baseIbLb0EE5storeB9nqn220108EbNS_12memory_orderE", scope: !395, file: !392, line: 62, type: !451, scopeLine: 63, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, declaration: !450, retainedNodes: !246)
!2258 = !DILocalVariable(name: "this", arg: 1, scope: !2257, type: !2259, flags: DIFlagArtificial | DIFlagObjectPointer)
!2259 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !395, size: 64)
!2260 = !DILocation(line: 0, scope: !2257)
!2261 = !DILocalVariable(name: "__d", arg: 2, scope: !2257, file: !392, line: 62, type: !127)
!2262 = !DILocation(line: 62, column: 40, scope: !2257)
!2263 = !DILocalVariable(name: "__m", arg: 3, scope: !2257, file: !392, line: 62, type: !441)
!2264 = !DILocation(line: 62, column: 58, scope: !2257)
!2265 = !DILocation(line: 64, column: 44, scope: !2257)
!2266 = !DILocation(line: 64, column: 51, scope: !2257)
!2267 = !DILocation(line: 64, column: 56, scope: !2257)
!2268 = !DILocation(line: 64, column: 5, scope: !2257)
!2269 = !DILocation(line: 65, column: 3, scope: !2257)
!2270 = distinct !DISubprogram(name: "load", linkageName: "_ZNKSt3__113__atomic_baseIiLb0EE4loadB9nqn220108ENS_12memory_orderE", scope: !541, file: !392, line: 70, type: !593, scopeLine: 71, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, declaration: !592, retainedNodes: !246)
!2271 = !DILocalVariable(name: "this", arg: 1, scope: !2270, type: !2272, flags: DIFlagArtificial | DIFlagObjectPointer)
!2272 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !580, size: 64)
!2273 = !DILocation(line: 0, scope: !2270)
!2274 = !DILocalVariable(name: "__m", arg: 2, scope: !2270, file: !392, line: 70, type: !441)
!2275 = !DILocation(line: 70, column: 65, scope: !2270)
!2276 = !DILocation(line: 72, column: 50, scope: !2270)
!2277 = !DILocation(line: 72, column: 57, scope: !2270)
!2278 = !DILocation(line: 72, column: 12, scope: !2270)
!2279 = !DILocation(line: 72, column: 5, scope: !2270)
!2280 = distinct !DISubprogram(name: "contender", linkageName: "_ZN12_GLOBAL__N_19contenderEPv", scope: !2, file: !3, line: 56, type: !2236, scopeLine: 56, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !744, retainedNodes: !246)
!2281 = !DILocalVariable(arg: 1, scope: !2280, file: !3, line: 56, type: !756)
!2282 = !DILocation(line: 56, column: 22, scope: !2280)
!2283 = !DILocalVariable(name: "deadline", scope: !2280, file: !3, line: 57, type: !962)
!2284 = !DILocation(line: 57, column: 12, scope: !2280)
!2285 = !DILocation(line: 58, column: 3, scope: !2280)
!2286 = !DILocation(line: 59, column: 12, scope: !2280)
!2287 = !DILocation(line: 59, column: 20, scope: !2280)
!2288 = !DILocation(line: 60, column: 16, scope: !2289)
!2289 = distinct !DILexicalBlock(scope: !2280, file: !3, line: 60, column: 7)
!2290 = !DILocation(line: 60, column: 24, scope: !2289)
!2291 = !DILocation(line: 61, column: 14, scope: !2292)
!2292 = distinct !DILexicalBlock(scope: !2289, file: !3, line: 60, column: 39)
!2293 = !DILocation(line: 61, column: 21, scope: !2292)
!2294 = !DILocation(line: 62, column: 14, scope: !2292)
!2295 = !DILocation(line: 62, column: 22, scope: !2292)
!2296 = !DILocation(line: 63, column: 3, scope: !2292)
!2297 = !DILocalVariable(name: "rc", scope: !2280, file: !3, line: 64, type: !88)
!2298 = !DILocation(line: 64, column: 7, scope: !2280)
!2299 = !DILocation(line: 64, column: 36, scope: !2280)
!2300 = !DILocation(line: 64, column: 12, scope: !2280)
!2301 = !DILocation(line: 65, column: 73, scope: !2280)
!2302 = !DILocation(line: 65, column: 3, scope: !2280)
!2303 = !DILocation(line: 66, column: 7, scope: !2304)
!2304 = distinct !DILexicalBlock(scope: !2280, file: !3, line: 66, column: 7)
!2305 = !DILocation(line: 66, column: 10, scope: !2304)
!2306 = !DILocation(line: 67, column: 5, scope: !2304)
!2307 = !DILocation(line: 67, column: 17, scope: !2304)
!2308 = !DILocation(line: 68, column: 56, scope: !2280)
!2309 = !DILocation(line: 68, column: 10, scope: !2280)
!2310 = !DILocation(line: 68, column: 3, scope: !2280)
!2311 = distinct !DISubprogram(name: "unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEEC2B9nqn220108ERS1_", scope: !69, file: !70, line: 42, type: !140, scopeLine: 43, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, declaration: !139, retainedNodes: !246)
!2312 = !DILocalVariable(name: "this", arg: 1, scope: !2311, type: !2313, flags: DIFlagArtificial | DIFlagObjectPointer)
!2313 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !69, size: 64)
!2314 = !DILocation(line: 0, scope: !2311)
!2315 = !DILocalVariable(name: "__m", arg: 2, scope: !2311, file: !70, line: 42, type: !142)
!2316 = !DILocation(line: 42, column: 76, scope: !2311)
!2317 = !DILocation(line: 43, column: 9, scope: !2311)
!2318 = !DILocation(line: 43, column: 29, scope: !2311)
!2319 = !{i64 8}
!2320 = !DILocation(line: 43, column: 36, scope: !2311)
!2321 = !DILocation(line: 44, column: 5, scope: !2322)
!2322 = distinct !DILexicalBlock(scope: !2311, file: !70, line: 43, column: 50)
!2323 = !DILocation(line: 44, column: 11, scope: !2322)
!2324 = !DILocation(line: 45, column: 3, scope: !2311)
!2325 = distinct !DISubprogram(name: "~FrameCleanup", linkageName: "_ZN12_GLOBAL__N_112FrameCleanupD2Ev", scope: !2245, file: !3, line: 25, type: !2248, scopeLine: 25, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !744, declaration: !2247, retainedNodes: !246)
!2326 = !DILocalVariable(name: "this", arg: 1, scope: !2325, type: !2327, flags: DIFlagArtificial | DIFlagObjectPointer)
!2327 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !2245, size: 64)
!2328 = !DILocation(line: 0, scope: !2325)
!2329 = !DILocation(line: 26, column: 25, scope: !2330)
!2330 = distinct !DILexicalBlock(scope: !2325, file: !3, line: 25, column: 19)
!2331 = !DILocalVariable(name: "marker", scope: !2330, file: !3, line: 27, type: !349)
!2332 = !DILocation(line: 27, column: 20, scope: !2330)
!2333 = !DILocation(line: 28, column: 33, scope: !2330)
!2334 = !DILocation(line: 28, column: 12, scope: !2330)
!2335 = !DILocation(line: 28, column: 11, scope: !2330)
!2336 = !DILocation(line: 29, column: 3, scope: !2325)
!2337 = distinct !DISubprogram(name: "~unique_lock", linkageName: "_ZNSt3__111unique_lockINS_5mutexEED2B9nqn220108Ev", scope: !69, file: !70, line: 65, type: !136, scopeLine: 65, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, declaration: !171, retainedNodes: !246)
!2338 = !DILocalVariable(name: "this", arg: 1, scope: !2337, type: !2313, flags: DIFlagArtificial | DIFlagObjectPointer)
!2339 = !DILocation(line: 0, scope: !2337)
!2340 = !DILocation(line: 66, column: 9, scope: !2341)
!2341 = distinct !DILexicalBlock(scope: !2342, file: !70, line: 66, column: 9)
!2342 = distinct !DILexicalBlock(scope: !2337, file: !70, line: 65, column: 40)
!2343 = !DILocation(line: 67, column: 7, scope: !2341)
!2344 = !DILocation(line: 67, column: 13, scope: !2341)
!2345 = !DILocation(line: 68, column: 3, scope: !2337)
!2346 = distinct !DISubprogram(name: "__cxx_atomic_store<bool>", linkageName: "_ZNSt3__118__cxx_atomic_storeB9nqn220108IbEEvPNS_22__cxx_atomic_base_implIT_EES2_NS_12memory_orderE", scope: !6, file: !403, line: 67, type: !2347, scopeLine: 67, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, templateParams: !414, retainedNodes: !246)
!2347 = !DISubroutineType(types: !2348)
!2348 = !{null, !2349, !127, !441}
!2349 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !402, size: 64)
!2350 = !DILocalVariable(name: "__a", arg: 1, scope: !2346, file: !403, line: 67, type: !2349)
!2351 = !DILocation(line: 67, column: 49, scope: !2346)
!2352 = !DILocalVariable(name: "__val", arg: 2, scope: !2346, file: !403, line: 67, type: !127)
!2353 = !DILocation(line: 67, column: 58, scope: !2346)
!2354 = !DILocalVariable(name: "__order", arg: 3, scope: !2346, file: !403, line: 67, type: !441)
!2355 = !DILocation(line: 67, column: 78, scope: !2346)
!2356 = !DILocation(line: 68, column: 37, scope: !2346)
!2357 = !DILocation(line: 68, column: 42, scope: !2346)
!2358 = !DILocation(line: 68, column: 102, scope: !2346)
!2359 = !DILocation(line: 68, column: 54, scope: !2346)
!2360 = !DILocation(line: 68, column: 3, scope: !2346)
!2361 = !DILocation(line: 69, column: 1, scope: !2346)
!2362 = distinct !DISubprogram(name: "__cxx_atomic_fetch_add<int>", linkageName: "_ZNSt3__122__cxx_atomic_fetch_addB9nqn220108IiEET_PNS_22__cxx_atomic_base_implIS1_EES1_NS_12memory_orderE", scope: !6, file: !403, line: 180, type: !2363, scopeLine: 180, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !744, templateParams: !558, retainedNodes: !246)
!2363 = !DISubroutineType(types: !2364)
!2364 = !{!88, !2365, !88, !441}
!2365 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !547, size: 64)
!2366 = !DILocalVariable(name: "__a", arg: 1, scope: !2362, file: !403, line: 180, type: !2365)
!2367 = !DILocation(line: 180, column: 53, scope: !2362)
!2368 = !DILocalVariable(name: "__delta", arg: 2, scope: !2362, file: !403, line: 180, type: !88)
!2369 = !DILocation(line: 180, column: 62, scope: !2362)
!2370 = !DILocalVariable(name: "__order", arg: 3, scope: !2362, file: !403, line: 180, type: !441)
!2371 = !DILocation(line: 180, column: 84, scope: !2362)
!2372 = !DILocation(line: 182, column: 22, scope: !2362)
!2373 = !DILocation(line: 182, column: 27, scope: !2362)
!2374 = !DILocation(line: 182, column: 89, scope: !2362)
!2375 = !DILocation(line: 182, column: 39, scope: !2362)
!2376 = !DILocation(line: 181, column: 10, scope: !2362)
!2377 = !DILocation(line: 181, column: 3, scope: !2362)
!2378 = !DILocalVariable(name: "__a", arg: 1, scope: !762, file: !403, line: 79, type: !765)
!2379 = !DILocation(line: 79, column: 80, scope: !762)
!2380 = !DILocalVariable(name: "__order", arg: 2, scope: !762, file: !403, line: 79, type: !441)
!2381 = !DILocation(line: 79, column: 98, scope: !762)
!2382 = !DILocation(line: 82, column: 45, scope: !762)
!2383 = !DILocation(line: 82, column: 50, scope: !762)
!2384 = !DILocation(line: 82, column: 104, scope: !762)
!2385 = !DILocation(line: 81, column: 10, scope: !762)
!2386 = !DILocation(line: 81, column: 3, scope: !762)
!2387 = !DILocalVariable(name: "__a", arg: 1, scope: !769, file: !403, line: 79, type: !772)
!2388 = !DILocation(line: 79, column: 80, scope: !769)
!2389 = !DILocalVariable(name: "__order", arg: 2, scope: !769, file: !403, line: 79, type: !441)
!2390 = !DILocation(line: 79, column: 98, scope: !769)
!2391 = !DILocation(line: 82, column: 45, scope: !769)
!2392 = !DILocation(line: 82, column: 50, scope: !769)
!2393 = !DILocation(line: 82, column: 104, scope: !769)
!2394 = !DILocation(line: 81, column: 10, scope: !769)
!2395 = !DILocation(line: 81, column: 3, scope: !769)
!2396 = distinct !DISubprogram(linkageName: "_GLOBAL__sub_I_item1_oldcaller_cancel.cpp", scope: !3, file: !3, type: !2397, flags: DIFlagArtificial, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !744)
!2397 = !DISubroutineType(types: !246)
!2398 = !DILocation(line: 0, scope: !2396)
