# function 完整数值附录

每段为该格第 1 次；全部五次在对应 JSON 与 raw 中。MAP 地址仅从此比对附录剔除，原始输出完整保留。

## armv7l / gnu / gnu_extension

编译退出码：0；[原始命令](../raw/function_armv7l_gnu_extension_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
value=37 allocations=0

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / gnu_extension

编译退出码：1；[原始命令](../raw/function_armv7l_gnu_extension_cxx_build.command.txt)

## armv7l / gnu / allocation

编译退出码：0；[原始命令](../raw/function_armv7l_allocation_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_alloc old_value=23

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / allocation

编译退出码：0；[原始命令](../raw/function_armv7l_allocation_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_alloc old_value=23

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / allocator_extension

编译退出码：1；[原始命令](../raw/function_armv7l_allocator_extension_gnu_build.command.txt)

## armv7l / cxx / allocator_extension

编译退出码：1；[原始命令](../raw/function_armv7l_allocator_extension_cxx_build.command.txt)

## armv7l / gnu / construction

编译退出码：0；[原始命令](../raw/function_armv7l_construction_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
values=12,12,12 moved_from_empty=1
nested_value=13
reference_values=1,2 count=2

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / construction

编译退出码：0；[原始命令](../raw/function_armv7l_construction_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
values=12,12,12 moved_from_empty=0
nested_value=13
reference_values=1,2 count=2

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / empty

编译退出码：0；[原始命令](../raw/function_armv7l_empty_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_function_call what=bad_function_call
empty=1 caught=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / empty

编译退出码：0；[原始命令](../raw/function_armv7l_empty_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_function_call what=std::bad_function_call
empty=1 caught=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / layout

编译退出码：0；[原始命令](../raw/function_armv7l_layout_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
function_int sizeof=16 alignof=4 function_void_size=16 function_void_align=4
target_pointer_present=1 value=42 empty=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / layout

编译退出码：0；[原始命令](../raw/function_armv7l_layout_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
function_int sizeof=24 alignof=8 function_void_size=24 function_void_align=8
target_pointer_present=1 value=42 empty=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / sbo

编译退出码：0；[原始命令](../raw/function_armv7l_sbo_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
target=trivial1 size=1 align=1 allocations=0 value=71
target=trivial1 frees=0
target=trivial4 size=4 align=1 allocations=0 value=71
target=trivial4 frees=0
target=trivial8 size=8 align=1 allocations=0 value=71
target=trivial8 frees=0
target=trivial9 size=9 align=1 allocations=1 value=71
target=trivial9 frees=1
target=trivial12 size=12 align=1 allocations=1 value=71
target=trivial12 frees=1
target=trivial16 size=16 align=1 allocations=1 value=71
target=trivial16 frees=1
target=trivial17 size=17 align=1 allocations=1 value=71
target=trivial17 frees=1
target=trivial24 size=24 align=1 allocations=1 value=71
target=trivial24 frees=1
target=trivial32 size=32 align=1 allocations=1 value=71
target=trivial32 frees=1
target=align8 size=8 align=8 allocations=1 value=71
target=align8 frees=1
target=align16 size=16 align=16 allocations=1 value=71
target=align16 frees=1
target=align32 size=32 align=32 allocations=1 value=71
target=align32 frees=1
target=nontrivial_nothrow_copy size=4 align=4 allocations=1 value=73
target=nontrivial_nothrow_copy frees=1
target=nontrivial_throwing_copy size=4 align=4 allocations=1 value=73
target=nontrivial_throwing_copy frees=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / sbo

编译退出码：0；[原始命令](../raw/function_armv7l_sbo_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
target=trivial1 size=1 align=1 allocations=0 value=71
target=trivial1 frees=0
target=trivial4 size=4 align=1 allocations=0 value=71
target=trivial4 frees=0
target=trivial8 size=8 align=1 allocations=0 value=71
target=trivial8 frees=0
target=trivial9 size=9 align=1 allocations=0 value=71
target=trivial9 frees=0
target=trivial12 size=12 align=1 allocations=0 value=71
target=trivial12 frees=0
target=trivial16 size=16 align=1 allocations=1 value=71
target=trivial16 frees=1
target=trivial17 size=17 align=1 allocations=1 value=71
target=trivial17 frees=1
target=trivial24 size=24 align=1 allocations=1 value=71
target=trivial24 frees=1
target=trivial32 size=32 align=1 allocations=1 value=71
target=trivial32 frees=1
target=align8 size=8 align=8 allocations=0 value=71
target=align8 frees=0
target=align16 size=16 align=16 allocations=1 value=71
target=align16 frees=1
target=align32 size=32 align=32 allocations=1 value=71
target=align32 frees=1
target=nontrivial_nothrow_copy size=4 align=4 allocations=0 value=73
target=nontrivial_nothrow_copy frees=0
target=nontrivial_throwing_copy size=4 align=4 allocations=1 value=73
target=nontrivial_throwing_copy frees=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / target_exception

编译退出码：0；[原始命令](../raw/function_armv7l_target_exception_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=invalid_argument what=target-input
calls=1 nonempty=1 caught=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / target_exception

编译退出码：0；[原始命令](../raw/function_armv7l_target_exception_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=invalid_argument what=target-input
calls=1 nonempty=1 caught=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / threshold

编译退出码：0；[原始命令](../raw/function_armv7l_threshold_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
callable_size=1 align=1 allocations=0 value=1
callable_size=2 align=1 allocations=0 value=2
callable_size=3 align=1 allocations=0 value=3
callable_size=4 align=1 allocations=0 value=4
callable_size=5 align=1 allocations=0 value=5
callable_size=6 align=1 allocations=0 value=6
callable_size=7 align=1 allocations=0 value=7
callable_size=8 align=1 allocations=0 value=8
callable_size=9 align=1 allocations=1 value=9
callable_size=10 align=1 allocations=1 value=10
callable_size=11 align=1 allocations=1 value=11
callable_size=12 align=1 allocations=1 value=12
callable_size=13 align=1 allocations=1 value=13
callable_size=14 align=1 allocations=1 value=14
callable_size=15 align=1 allocations=1 value=15
callable_size=16 align=1 allocations=1 value=16
callable_size=17 align=1 allocations=1 value=17
callable_size=18 align=1 allocations=1 value=18
callable_size=19 align=1 allocations=1 value=19
callable_size=20 align=1 allocations=1 value=20
callable_size=21 align=1 allocations=1 value=21
callable_size=22 align=1 allocations=1 value=22
callable_size=23 align=1 allocations=1 value=23
callable_size=24 align=1 allocations=1 value=24
callable_size=25 align=1 allocations=1 value=25
callable_size=26 align=1 allocations=1 value=26
callable_size=27 align=1 allocations=1 value=27
callable_size=28 align=1 allocations=1 value=28
callable_size=29 align=1 allocations=1 value=29
callable_size=30 align=1 allocations=1 value=30
callable_size=31 align=1 allocations=1 value=31
callable_size=32 align=1 allocations=1 value=32
callable_size=33 align=1 allocations=1 value=33
callable_size=34 align=1 allocations=1 value=34
callable_size=35 align=1 allocations=1 value=35
callable_size=36 align=1 allocations=1 value=36
callable_size=37 align=1 allocations=1 value=37
callable_size=38 align=1 allocations=1 value=38
callable_size=39 align=1 allocations=1 value=39
callable_size=40 align=1 allocations=1 value=40

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / threshold

编译退出码：0；[原始命令](../raw/function_armv7l_threshold_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
callable_size=1 align=1 allocations=0 value=1
callable_size=2 align=1 allocations=0 value=2
callable_size=3 align=1 allocations=0 value=3
callable_size=4 align=1 allocations=0 value=4
callable_size=5 align=1 allocations=0 value=5
callable_size=6 align=1 allocations=0 value=6
callable_size=7 align=1 allocations=0 value=7
callable_size=8 align=1 allocations=0 value=8
callable_size=9 align=1 allocations=0 value=9
callable_size=10 align=1 allocations=0 value=10
callable_size=11 align=1 allocations=0 value=11
callable_size=12 align=1 allocations=0 value=12
callable_size=13 align=1 allocations=1 value=13
callable_size=14 align=1 allocations=1 value=14
callable_size=15 align=1 allocations=1 value=15
callable_size=16 align=1 allocations=1 value=16
callable_size=17 align=1 allocations=1 value=17
callable_size=18 align=1 allocations=1 value=18
callable_size=19 align=1 allocations=1 value=19
callable_size=20 align=1 allocations=1 value=20
callable_size=21 align=1 allocations=1 value=21
callable_size=22 align=1 allocations=1 value=22
callable_size=23 align=1 allocations=1 value=23
callable_size=24 align=1 allocations=1 value=24
callable_size=25 align=1 allocations=1 value=25
callable_size=26 align=1 allocations=1 value=26
callable_size=27 align=1 allocations=1 value=27
callable_size=28 align=1 allocations=1 value=28
callable_size=29 align=1 allocations=1 value=29
callable_size=30 align=1 allocations=1 value=30
callable_size=31 align=1 allocations=1 value=31
callable_size=32 align=1 allocations=1 value=32
callable_size=33 align=1 allocations=1 value=33
callable_size=34 align=1 allocations=1 value=34
callable_size=35 align=1 allocations=1 value=35
callable_size=36 align=1 allocations=1 value=36
callable_size=37 align=1 allocations=1 value=37
callable_size=38 align=1 allocations=1 value=38
callable_size=39 align=1 allocations=1 value=39
callable_size=40 align=1 allocations=1 value=40

```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / gnu_extension

编译退出码：0；[原始命令](../raw/function_x86_64_gnu_extension_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
value=37 allocations=0
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / gnu_extension

编译退出码：1；[原始命令](../raw/function_x86_64_gnu_extension_cxx_build.command.txt)

## x86_64 / gnu / allocation

编译退出码：0；[原始命令](../raw/function_x86_64_allocation_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_alloc old_value=23
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / allocation

编译退出码：0；[原始命令](../raw/function_x86_64_allocation_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_alloc old_value=23
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / allocator_extension

编译退出码：1；[原始命令](../raw/function_x86_64_allocator_extension_gnu_build.command.txt)

## x86_64 / cxx / allocator_extension

编译退出码：1；[原始命令](../raw/function_x86_64_allocator_extension_cxx_build.command.txt)

## x86_64 / gnu / construction

编译退出码：0；[原始命令](../raw/function_x86_64_construction_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
values=12,12,12 moved_from_empty=1
nested_value=13
reference_values=1,2 count=2
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / construction

编译退出码：0；[原始命令](../raw/function_x86_64_construction_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
values=12,12,12 moved_from_empty=0
nested_value=13
reference_values=1,2 count=2
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / empty

编译退出码：0；[原始命令](../raw/function_x86_64_empty_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_function_call what=bad_function_call
empty=1 caught=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / empty

编译退出码：0；[原始命令](../raw/function_x86_64_empty_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_function_call what=std::bad_function_call
empty=1 caught=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / layout

编译退出码：0；[原始命令](../raw/function_x86_64_layout_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
function_int sizeof=32 alignof=8 function_void_size=32 function_void_align=8
target_pointer_present=1 value=42 empty=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / layout

编译退出码：0；[原始命令](../raw/function_x86_64_layout_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
function_int sizeof=48 alignof=16 function_void_size=48 function_void_align=16
target_pointer_present=1 value=42 empty=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / sbo

编译退出码：0；[原始命令](../raw/function_x86_64_sbo_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
target=trivial1 size=1 align=1 allocations=0 value=71
target=trivial1 frees=0
target=trivial4 size=4 align=1 allocations=0 value=71
target=trivial4 frees=0
target=trivial8 size=8 align=1 allocations=0 value=71
target=trivial8 frees=0
target=trivial9 size=9 align=1 allocations=0 value=71
target=trivial9 frees=0
target=trivial12 size=12 align=1 allocations=0 value=71
target=trivial12 frees=0
target=trivial16 size=16 align=1 allocations=0 value=71
target=trivial16 frees=0
target=trivial17 size=17 align=1 allocations=1 value=71
target=trivial17 frees=1
target=trivial24 size=24 align=1 allocations=1 value=71
target=trivial24 frees=1
target=trivial32 size=32 align=1 allocations=1 value=71
target=trivial32 frees=1
target=align8 size=8 align=8 allocations=0 value=71
target=align8 frees=0
target=align16 size=16 align=16 allocations=1 value=71
target=align16 frees=1
target=align32 size=32 align=32 allocations=1 value=71
target=align32 frees=1
target=nontrivial_nothrow_copy size=4 align=4 allocations=1 value=73
target=nontrivial_nothrow_copy frees=1
target=nontrivial_throwing_copy size=4 align=4 allocations=1 value=73
target=nontrivial_throwing_copy frees=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / sbo

编译退出码：0；[原始命令](../raw/function_x86_64_sbo_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
target=trivial1 size=1 align=1 allocations=0 value=71
target=trivial1 frees=0
target=trivial4 size=4 align=1 allocations=0 value=71
target=trivial4 frees=0
target=trivial8 size=8 align=1 allocations=0 value=71
target=trivial8 frees=0
target=trivial9 size=9 align=1 allocations=0 value=71
target=trivial9 frees=0
target=trivial12 size=12 align=1 allocations=0 value=71
target=trivial12 frees=0
target=trivial16 size=16 align=1 allocations=0 value=71
target=trivial16 frees=0
target=trivial17 size=17 align=1 allocations=0 value=71
target=trivial17 frees=0
target=trivial24 size=24 align=1 allocations=0 value=71
target=trivial24 frees=0
target=trivial32 size=32 align=1 allocations=1 value=71
target=trivial32 frees=1
target=align8 size=8 align=8 allocations=0 value=71
target=align8 frees=0
target=align16 size=16 align=16 allocations=0 value=71
target=align16 frees=0
target=align32 size=32 align=32 allocations=1 value=71
target=align32 frees=1
target=nontrivial_nothrow_copy size=4 align=4 allocations=0 value=73
target=nontrivial_nothrow_copy frees=0
target=nontrivial_throwing_copy size=4 align=4 allocations=1 value=73
target=nontrivial_throwing_copy frees=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / target_exception

编译退出码：0；[原始命令](../raw/function_x86_64_target_exception_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=invalid_argument what=target-input
calls=1 nonempty=1 caught=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / target_exception

编译退出码：0；[原始命令](../raw/function_x86_64_target_exception_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=invalid_argument what=target-input
calls=1 nonempty=1 caught=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / threshold

编译退出码：0；[原始命令](../raw/function_x86_64_threshold_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
callable_size=1 align=1 allocations=0 value=1
callable_size=2 align=1 allocations=0 value=2
callable_size=3 align=1 allocations=0 value=3
callable_size=4 align=1 allocations=0 value=4
callable_size=5 align=1 allocations=0 value=5
callable_size=6 align=1 allocations=0 value=6
callable_size=7 align=1 allocations=0 value=7
callable_size=8 align=1 allocations=0 value=8
callable_size=9 align=1 allocations=0 value=9
callable_size=10 align=1 allocations=0 value=10
callable_size=11 align=1 allocations=0 value=11
callable_size=12 align=1 allocations=0 value=12
callable_size=13 align=1 allocations=0 value=13
callable_size=14 align=1 allocations=0 value=14
callable_size=15 align=1 allocations=0 value=15
callable_size=16 align=1 allocations=0 value=16
callable_size=17 align=1 allocations=1 value=17
callable_size=18 align=1 allocations=1 value=18
callable_size=19 align=1 allocations=1 value=19
callable_size=20 align=1 allocations=1 value=20
callable_size=21 align=1 allocations=1 value=21
callable_size=22 align=1 allocations=1 value=22
callable_size=23 align=1 allocations=1 value=23
callable_size=24 align=1 allocations=1 value=24
callable_size=25 align=1 allocations=1 value=25
callable_size=26 align=1 allocations=1 value=26
callable_size=27 align=1 allocations=1 value=27
callable_size=28 align=1 allocations=1 value=28
callable_size=29 align=1 allocations=1 value=29
callable_size=30 align=1 allocations=1 value=30
callable_size=31 align=1 allocations=1 value=31
callable_size=32 align=1 allocations=1 value=32
callable_size=33 align=1 allocations=1 value=33
callable_size=34 align=1 allocations=1 value=34
callable_size=35 align=1 allocations=1 value=35
callable_size=36 align=1 allocations=1 value=36
callable_size=37 align=1 allocations=1 value=37
callable_size=38 align=1 allocations=1 value=38
callable_size=39 align=1 allocations=1 value=39
callable_size=40 align=1 allocations=1 value=40
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / threshold

编译退出码：0；[原始命令](../raw/function_x86_64_threshold_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
callable_size=1 align=1 allocations=0 value=1
callable_size=2 align=1 allocations=0 value=2
callable_size=3 align=1 allocations=0 value=3
callable_size=4 align=1 allocations=0 value=4
callable_size=5 align=1 allocations=0 value=5
callable_size=6 align=1 allocations=0 value=6
callable_size=7 align=1 allocations=0 value=7
callable_size=8 align=1 allocations=0 value=8
callable_size=9 align=1 allocations=0 value=9
callable_size=10 align=1 allocations=0 value=10
callable_size=11 align=1 allocations=0 value=11
callable_size=12 align=1 allocations=0 value=12
callable_size=13 align=1 allocations=0 value=13
callable_size=14 align=1 allocations=0 value=14
callable_size=15 align=1 allocations=0 value=15
callable_size=16 align=1 allocations=0 value=16
callable_size=17 align=1 allocations=0 value=17
callable_size=18 align=1 allocations=0 value=18
callable_size=19 align=1 allocations=0 value=19
callable_size=20 align=1 allocations=0 value=20
callable_size=21 align=1 allocations=0 value=21
callable_size=22 align=1 allocations=0 value=22
callable_size=23 align=1 allocations=0 value=23
callable_size=24 align=1 allocations=0 value=24
callable_size=25 align=1 allocations=1 value=25
callable_size=26 align=1 allocations=1 value=26
callable_size=27 align=1 allocations=1 value=27
callable_size=28 align=1 allocations=1 value=28
callable_size=29 align=1 allocations=1 value=29
callable_size=30 align=1 allocations=1 value=30
callable_size=31 align=1 allocations=1 value=31
callable_size=32 align=1 allocations=1 value=32
callable_size=33 align=1 allocations=1 value=33
callable_size=34 align=1 allocations=1 value=34
callable_size=35 align=1 allocations=1 value=35
callable_size=36 align=1 allocations=1 value=36
callable_size=37 align=1 allocations=1 value=37
callable_size=38 align=1 allocations=1 value=38
callable_size=39 align=1 allocations=1 value=39
callable_size=40 align=1 allocations=1 value=40
```

重复次数：5；数值重复一致：True。
