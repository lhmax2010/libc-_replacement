# vector 完整数值附录

每段为该格第 1 次；全部五次在对应 JSON 与 raw 中。MAP 地址仅从此比对附录剔除，原始输出完整保留。

## armv7l / gnu / cxx23_append_range_extension

编译退出码：1；[原始命令](../raw/vector_armv7l_cxx23_append_range_extension_gnu_build.command.txt)

## armv7l / cxx / cxx23_append_range_extension

编译退出码：0；[原始命令](../raw/vector_armv7l_cxx23_append_range_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=202302 POINTER_BYTES=4
size=3 values=2,5,9

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / cxx23_from_range_extension

编译退出码：1；[原始命令](../raw/vector_armv7l_cxx23_from_range_extension_gnu_build.command.txt)

## armv7l / cxx / cxx23_from_range_extension

编译退出码：0；[原始命令](../raw/vector_armv7l_cxx23_from_range_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=202302 POINTER_BYTES=4
size=3 values=2,5,9

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / endpoints

编译退出码：0；[原始命令](../raw/vector_armv7l_endpoints_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
size=3 capacity=7
endpoint0_offsets=0, matches=1
endpoint1_offsets=4, matches=1
endpoint2_offsets=8, matches=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / endpoints

编译退出码：0；[原始命令](../raw/vector_armv7l_endpoints_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
size=3 capacity=7
endpoint0_offsets=0, matches=1
endpoint1_offsets=4, matches=1
endpoint2_offsets=8, matches=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / allocation

编译退出码：0；[原始命令](../raw/vector_armv7l_allocation_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_alloc unchanged=1 capacity=3 size=3

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / allocation

编译退出码：0；[原始命令](../raw/vector_armv7l_allocation_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_alloc unchanged=1 capacity=3 size=3

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / at

编译退出码：0；[原始命令](../raw/vector_armv7l_at_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=out_of_range what=vector::_M_range_check: __n (which is 2) >= this->size() (which is 2)
size=2 values=5,9

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / at

编译退出码：0；[原始命令](../raw/vector_armv7l_at_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=out_of_range what=vector
size=2 values=5,9

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / bool

编译退出码：0；[原始命令](../raw/vector_armv7l_bool_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
reference_is_bool_ref=0 reference_bytes=8 iterator_bytes=8 bits=000
bool_size=4 capacity=32
bool_size=33 capacity=64
bool_size=65 capacity=128
bool_size=129 capacity=256
bool_shrink_size=3 capacity=32

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / bool

编译退出码：0；[原始命令](../raw/vector_armv7l_bool_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
reference_is_bool_ref=0 reference_bytes=8 iterator_bytes=8 bits=000
bool_size=4 capacity=32
bool_size=33 capacity=64
bool_size=65 capacity=128
bool_size=129 capacity=256
bool_shrink_size=3 capacity=32

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / bool_data_extension

编译退出码：1；[原始命令](../raw/vector_armv7l_bool_data_extension_gnu_build.command.txt)

## armv7l / cxx / bool_data_extension

编译退出码：1；[原始命令](../raw/vector_armv7l_bool_data_extension_cxx_build.command.txt)

## armv7l / gnu / construction

编译退出码：0；[原始命令](../raw/vector_armv7l_construction_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
empty_size=0 moved_from_size=0 values=3,5,8, move_assigned_size=3
nested_sizes=1,3 nested_byte2=99

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / construction

编译退出码：0；[原始命令](../raw/vector_armv7l_construction_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
empty_size=0 moved_from_size=0 values=3,5,8, move_assigned_size=3
nested_sizes=1,3 nested_byte2=99

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / gnu_extension

编译退出码：0；[原始命令](../raw/vector_armv7l_gnu_extension_gnu_build.command.txt)

```text
size=2 values=2,5

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / gnu_extension

编译退出码：1；[原始命令](../raw/vector_armv7l_gnu_extension_cxx_build.command.txt)

## armv7l / gnu / growth

编译退出码：0；[原始命令](../raw/vector_armv7l_growth_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
size=0 capacity=0
size=1 capacity=1
size=2 capacity=2
size=3 capacity=4
size=5 capacity=8
size=9 capacity=16
size=17 capacity=32
size=33 capacity=64
size=65 capacity=128
size=129 capacity=256
reserve=0 size=3 capacity=3
reserve=1 size=3 capacity=3
reserve=7 size=3 capacity=7
reserve=16 size=3 capacity=16
reserve=23 size=3 capacity=23
reserve=100 size=3 capacity=100
shrink before=100 after=3 size=3 value=9

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / growth

编译退出码：0；[原始命令](../raw/vector_armv7l_growth_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
size=0 capacity=0
size=1 capacity=1
size=2 capacity=2
size=3 capacity=4
size=5 capacity=8
size=9 capacity=16
size=17 capacity=32
size=33 capacity=64
size=65 capacity=128
size=129 capacity=256
reserve=0 size=3 capacity=3
reserve=1 size=3 capacity=3
reserve=7 size=3 capacity=7
reserve=16 size=3 capacity=16
reserve=23 size=3 capacity=23
reserve=100 size=3 capacity=100
shrink before=100 after=3 size=3 value=9

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / iteration

编译退出码：0；[原始命令](../raw/vector_armv7l_iteration_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
no_realloc_first=1 address_same=1
erase_return=3 values=1,3,4,
realloc_address_same=0 contiguous_stride=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / iteration

编译退出码：0；[原始命令](../raw/vector_armv7l_iteration_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
no_realloc_first=1 address_same=1
erase_return=3 values=1,3,4,
realloc_address_same=0 contiguous_stride=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / layout

编译退出码：0；[原始命令](../raw/vector_armv7l_layout_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
vector_int sizeof=12 alignof=4 size=3 capacity=3
vector_bool sizeof=20 alignof=4 size=3 capacity=32 proxy_size=8
endpoint0_offsets=0,
endpoint1_offsets=4,8,
endpoint2_offsets=4,8,

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / layout

编译退出码：0；[原始命令](../raw/vector_armv7l_layout_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
vector_int sizeof=12 alignof=4 size=3 capacity=3
vector_bool sizeof=12 alignof=4 size=3 capacity=32 proxy_size=8
endpoint0_offsets=0,
endpoint1_offsets=4,8,
endpoint2_offsets=4,8,

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / libcxx_extension

编译退出码：1；[原始命令](../raw/vector_armv7l_libcxx_extension_gnu_build.command.txt)

## armv7l / cxx / libcxx_extension

编译退出码：0；[原始命令](../raw/vector_armv7l_libcxx_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
invariants=1 size=2

```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / cxx23_append_range_extension

编译退出码：1；[原始命令](../raw/vector_x86_64_cxx23_append_range_extension_gnu_build.command.txt)

## x86_64 / cxx / cxx23_append_range_extension

编译退出码：0；[原始命令](../raw/vector_x86_64_cxx23_append_range_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=202302 POINTER_BYTES=8
size=3 values=2,5,9
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / cxx23_from_range_extension

编译退出码：1；[原始命令](../raw/vector_x86_64_cxx23_from_range_extension_gnu_build.command.txt)

## x86_64 / cxx / cxx23_from_range_extension

编译退出码：0；[原始命令](../raw/vector_x86_64_cxx23_from_range_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=202302 POINTER_BYTES=8
size=3 values=2,5,9
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / endpoints

编译退出码：0；[原始命令](../raw/vector_x86_64_endpoints_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
size=3 capacity=7
endpoint0_offsets=0, matches=1
endpoint1_offsets=8, matches=1
endpoint2_offsets=16, matches=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / endpoints

编译退出码：0；[原始命令](../raw/vector_x86_64_endpoints_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
size=3 capacity=7
endpoint0_offsets=0, matches=1
endpoint1_offsets=8, matches=1
endpoint2_offsets=16, matches=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / allocation

编译退出码：0；[原始命令](../raw/vector_x86_64_allocation_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_alloc unchanged=1 capacity=3 size=3
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / allocation

编译退出码：0；[原始命令](../raw/vector_x86_64_allocation_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_alloc unchanged=1 capacity=3 size=3
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / at

编译退出码：0；[原始命令](../raw/vector_x86_64_at_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=out_of_range what=vector::_M_range_check: __n (which is 2) >= this->size() (which is 2)
size=2 values=5,9
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / at

编译退出码：0；[原始命令](../raw/vector_x86_64_at_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=out_of_range what=vector
size=2 values=5,9
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / bool

编译退出码：0；[原始命令](../raw/vector_x86_64_bool_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
reference_is_bool_ref=0 reference_bytes=16 iterator_bytes=16 bits=000
bool_size=4 capacity=64
bool_size=65 capacity=128
bool_size=129 capacity=256
bool_shrink_size=3 capacity=64
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / bool

编译退出码：0；[原始命令](../raw/vector_x86_64_bool_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
reference_is_bool_ref=0 reference_bytes=16 iterator_bytes=16 bits=000
bool_size=4 capacity=64
bool_size=65 capacity=128
bool_size=129 capacity=256
bool_shrink_size=3 capacity=64
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / bool_data_extension

编译退出码：1；[原始命令](../raw/vector_x86_64_bool_data_extension_gnu_build.command.txt)

## x86_64 / cxx / bool_data_extension

编译退出码：1；[原始命令](../raw/vector_x86_64_bool_data_extension_cxx_build.command.txt)

## x86_64 / gnu / construction

编译退出码：0；[原始命令](../raw/vector_x86_64_construction_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
empty_size=0 moved_from_size=0 values=3,5,8, move_assigned_size=3
nested_sizes=1,3 nested_byte2=99
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / construction

编译退出码：0；[原始命令](../raw/vector_x86_64_construction_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
empty_size=0 moved_from_size=0 values=3,5,8, move_assigned_size=3
nested_sizes=1,3 nested_byte2=99
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / gnu_extension

编译退出码：0；[原始命令](../raw/vector_x86_64_gnu_extension_gnu_build.command.txt)

```text
size=2 values=2,5
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / gnu_extension

编译退出码：1；[原始命令](../raw/vector_x86_64_gnu_extension_cxx_build.command.txt)

## x86_64 / gnu / growth

编译退出码：0；[原始命令](../raw/vector_x86_64_growth_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
size=0 capacity=0
size=1 capacity=1
size=2 capacity=2
size=3 capacity=4
size=5 capacity=8
size=9 capacity=16
size=17 capacity=32
size=33 capacity=64
size=65 capacity=128
size=129 capacity=256
reserve=0 size=3 capacity=3
reserve=1 size=3 capacity=3
reserve=7 size=3 capacity=7
reserve=16 size=3 capacity=16
reserve=23 size=3 capacity=23
reserve=100 size=3 capacity=100
shrink before=100 after=3 size=3 value=9
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / growth

编译退出码：0；[原始命令](../raw/vector_x86_64_growth_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
size=0 capacity=0
size=1 capacity=1
size=2 capacity=2
size=3 capacity=4
size=5 capacity=8
size=9 capacity=16
size=17 capacity=32
size=33 capacity=64
size=65 capacity=128
size=129 capacity=256
reserve=0 size=3 capacity=3
reserve=1 size=3 capacity=3
reserve=7 size=3 capacity=7
reserve=16 size=3 capacity=16
reserve=23 size=3 capacity=23
reserve=100 size=3 capacity=100
shrink before=100 after=3 size=3 value=9
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / iteration

编译退出码：0；[原始命令](../raw/vector_x86_64_iteration_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
no_realloc_first=1 address_same=1
erase_return=3 values=1,3,4,
realloc_address_same=0 contiguous_stride=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / iteration

编译退出码：0；[原始命令](../raw/vector_x86_64_iteration_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
no_realloc_first=1 address_same=1
erase_return=3 values=1,3,4,
realloc_address_same=0 contiguous_stride=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / layout

编译退出码：0；[原始命令](../raw/vector_x86_64_layout_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
vector_int sizeof=24 alignof=8 size=3 capacity=3
vector_bool sizeof=40 alignof=8 size=3 capacity=64 proxy_size=16
endpoint0_offsets=0,
endpoint1_offsets=8,16,
endpoint2_offsets=8,16,
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / layout

编译退出码：0；[原始命令](../raw/vector_x86_64_layout_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
vector_int sizeof=24 alignof=8 size=3 capacity=3
vector_bool sizeof=24 alignof=8 size=3 capacity=64 proxy_size=16
endpoint0_offsets=0,
endpoint1_offsets=8,16,
endpoint2_offsets=8,16,
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / libcxx_extension

编译退出码：1；[原始命令](../raw/vector_x86_64_libcxx_extension_gnu_build.command.txt)

## x86_64 / cxx / libcxx_extension

编译退出码：0；[原始命令](../raw/vector_x86_64_libcxx_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
invariants=1 size=2
```

重复次数：5；数值重复一致：True。
