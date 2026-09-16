# string 完整数值附录

每段为该格第 1 次；全部五次在对应 JSON 与 raw 中。MAP 地址仅从此比对附录剔除，原始输出完整保留。

## armv7l / gnu / libcxx_extension

编译退出码：1；[原始命令](../raw/string_armv7l_libcxx_extension_gnu_build.command.txt)

## armv7l / cxx / libcxx_extension

编译退出码：0；[原始命令](../raw/string_armv7l_libcxx_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
invariants=1 size=9

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / allocation

编译退出码：0；[原始命令](../raw/string_armv7l_allocation_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_alloc size=100 capacity=100 unchanged=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / allocation

编译退出码：0；[原始命令](../raw/string_armv7l_allocation_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=bad_alloc size=100 capacity=103 unchanged=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / at

编译退出码：0；[原始命令](../raw/string_armv7l_at_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=out_of_range what=basic_string::at: __n (which is 3) >= this->size() (which is 3)
after size=3 hex=616263
valid_at_1=98

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / at

编译退出码：0；[原始命令](../raw/string_armv7l_at_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
exception=out_of_range what=basic_string
after size=3 hex=616263
valid_at_1=98

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / c_api

编译退出码：0；[原始命令](../raw/string_armv7l_c_api_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
embedded_nul size=3 hex=61007a
size=3 strlen=1 trailing_nul=1 npos=4294967295 mutable_data=1
empty_nonnull=1 empty_nul=1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / c_api

编译退出码：0；[原始命令](../raw/string_armv7l_c_api_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
embedded_nul size=3 hex=61007a
size=3 strlen=1 trailing_nul=1 npos=4294967295 mutable_data=1
empty_nonnull=1 empty_nul=1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / compare_hash

编译退出码：0；[原始命令](../raw/string_armv7l_compare_hash_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
compare_az=-25 compare_za=25 compare_length=-99 compare_equal=0
hash size=0 value=3990065800
hash size=1 value=2167009006
hash size=3 value=3350977461
hash size=3 value=590206204
map_order=a:1,z:2,

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / compare_hash

编译退出码：0；[原始命令](../raw/string_armv7l_compare_hash_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
compare_az=-25 compare_za=25 compare_length=-1 compare_equal=0
hash size=0 value=0
hash size=1 value=2456313694
hash size=3 value=324500635
hash size=3 value=1759948281
map_order=a:1,z:2,

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / construction

编译退出码：0；[原始命令](../raw/string_armv7l_construction_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
empty size=0 hex=
cstr size=1 hex=61
counted size=4 hex=61006263
range size=5 hex=72616e6765
copy_then_moved size=4 hex=61006263
moved_from size=0 hex=
moveassigned size=5 hex=72616e6765
long_move_size=200 source_size=0

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / construction

编译退出码：0；[原始命令](../raw/string_armv7l_construction_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
empty size=0 hex=
cstr size=1 hex=61
counted size=4 hex=61006263
range size=5 hex=72616e6765
copy_then_moved size=4 hex=61006263
moved_from size=0 hex=
moveassigned size=5 hex=72616e6765
long_move_size=200 source_size=0

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / gnu_extension

编译退出码：0；[原始命令](../raw/string_armv7l_gnu_extension_gnu_build.command.txt)

```text
size=9 text=extension

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / gnu_extension

编译退出码：1；[原始命令](../raw/string_armv7l_gnu_extension_cxx_build.command.txt)

## armv7l / gnu / growth

编译退出码：0；[原始命令](../raw/string_armv7l_growth_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
push size=0 capacity=15
push size=16 capacity=30
push size=31 capacity=60
push size=61 capacity=120
push size=121 capacity=240
push size=241 capacity=480
reserve request=0 size=3 capacity=15
reserve request=1 size=3 capacity=15
reserve request=15 size=3 capacity=15
reserve request=16 size=3 capacity=30
reserve request=22 size=3 capacity=30
reserve request=23 size=3 capacity=30
reserve request=30 size=3 capacity=30
reserve request=31 size=3 capacity=31
reserve request=32 size=3 capacity=32
reserve request=63 size=3 capacity=63
reserve request=64 size=3 capacity=64
reserve request=100 size=3 capacity=100
reserve request=129 size=3 capacity=129
shrink before=200 after=15

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / growth

编译退出码：0；[原始命令](../raw/string_armv7l_growth_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
push size=0 capacity=10
push size=11 capacity=23
push size=24 capacity=47
push size=48 capacity=95
push size=96 capacity=191
push size=192 capacity=383
reserve request=0 size=3 capacity=10
reserve request=1 size=3 capacity=10
reserve request=15 size=3 capacity=15
reserve request=16 size=3 capacity=23
reserve request=22 size=3 capacity=23
reserve request=23 size=3 capacity=23
reserve request=30 size=3 capacity=31
reserve request=31 size=3 capacity=31
reserve request=32 size=3 capacity=39
reserve request=63 size=3 capacity=63
reserve request=64 size=3 capacity=71
reserve request=100 size=3 capacity=103
reserve request=129 size=3 capacity=135
shrink before=207 after=10

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / iteration

编译退出码：0；[原始命令](../raw/string_armv7l_iteration_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
within_capacity_address_same=1 capacity=100 value=abcde
beyond_capacity_address_same=0 capacity=200 value=abcde
iteration=abcde

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / iteration

编译退出码：0；[原始命令](../raw/string_armv7l_iteration_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
within_capacity_address_same=1 capacity=103 value=abcde
beyond_capacity_address_same=0 capacity=207 value=abcde
iteration=abcde

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / layout

编译退出码：0；[原始命令](../raw/string_armv7l_layout_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
sizeof=24 alignof=4
n=0 capacity=15 allocations=0 inline=1 data_offset=8
n=1 capacity=15 allocations=0 inline=1 data_offset=8
n=2 capacity=15 allocations=0 inline=1 data_offset=8
n=3 capacity=15 allocations=0 inline=1 data_offset=8
n=4 capacity=15 allocations=0 inline=1 data_offset=8
n=5 capacity=15 allocations=0 inline=1 data_offset=8
n=6 capacity=15 allocations=0 inline=1 data_offset=8
n=7 capacity=15 allocations=0 inline=1 data_offset=8
n=8 capacity=15 allocations=0 inline=1 data_offset=8
n=9 capacity=15 allocations=0 inline=1 data_offset=8
n=10 capacity=15 allocations=0 inline=1 data_offset=8
n=11 capacity=15 allocations=0 inline=1 data_offset=8
n=12 capacity=15 allocations=0 inline=1 data_offset=8
n=13 capacity=15 allocations=0 inline=1 data_offset=8
n=14 capacity=15 allocations=0 inline=1 data_offset=8
n=15 capacity=15 allocations=0 inline=1 data_offset=8
n=16 capacity=16 allocations=1 inline=0 data_offset=-1
n=17 capacity=17 allocations=1 inline=0 data_offset=-1
n=18 capacity=18 allocations=1 inline=0 data_offset=-1
n=19 capacity=19 allocations=1 inline=0 data_offset=-1
n=20 capacity=20 allocations=1 inline=0 data_offset=-1
n=21 capacity=21 allocations=1 inline=0 data_offset=-1
n=22 capacity=22 allocations=1 inline=0 data_offset=-1
n=23 capacity=23 allocations=1 inline=0 data_offset=-1
n=24 capacity=24 allocations=1 inline=0 data_offset=-1
n=25 capacity=25 allocations=1 inline=0 data_offset=-1
n=26 capacity=26 allocations=1 inline=0 data_offset=-1
n=27 capacity=27 allocations=1 inline=0 data_offset=-1
n=28 capacity=28 allocations=1 inline=0 data_offset=-1
n=29 capacity=29 allocations=1 inline=0 data_offset=-1
n=30 capacity=30 allocations=1 inline=0 data_offset=-1
n=31 capacity=31 allocations=1 inline=0 data_offset=-1
n=32 capacity=32 allocations=1 inline=0 data_offset=-1
n=33 capacity=33 allocations=1 inline=0 data_offset=-1
n=34 capacity=34 allocations=1 inline=0 data_offset=-1
n=35 capacity=35 allocations=1 inline=0 data_offset=-1
n=36 capacity=36 allocations=1 inline=0 data_offset=-1
n=37 capacity=37 allocations=1 inline=0 data_offset=-1
n=38 capacity=38 allocations=1 inline=0 data_offset=-1
n=39 capacity=39 allocations=1 inline=0 data_offset=-1
n=40 capacity=40 allocations=1 inline=0 data_offset=-1

```

重复次数：5；数值重复一致：True。

## armv7l / cxx / layout

编译退出码：0；[原始命令](../raw/string_armv7l_layout_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=4
sizeof=12 alignof=4
n=0 capacity=10 allocations=0 inline=1 data_offset=1
n=1 capacity=10 allocations=0 inline=1 data_offset=1
n=2 capacity=10 allocations=0 inline=1 data_offset=1
n=3 capacity=10 allocations=0 inline=1 data_offset=1
n=4 capacity=10 allocations=0 inline=1 data_offset=1
n=5 capacity=10 allocations=0 inline=1 data_offset=1
n=6 capacity=10 allocations=0 inline=1 data_offset=1
n=7 capacity=10 allocations=0 inline=1 data_offset=1
n=8 capacity=10 allocations=0 inline=1 data_offset=1
n=9 capacity=10 allocations=0 inline=1 data_offset=1
n=10 capacity=10 allocations=0 inline=1 data_offset=1
n=11 capacity=15 allocations=1 inline=0 data_offset=-1
n=12 capacity=15 allocations=1 inline=0 data_offset=-1
n=13 capacity=15 allocations=1 inline=0 data_offset=-1
n=14 capacity=15 allocations=1 inline=0 data_offset=-1
n=15 capacity=15 allocations=1 inline=0 data_offset=-1
n=16 capacity=23 allocations=1 inline=0 data_offset=-1
n=17 capacity=23 allocations=1 inline=0 data_offset=-1
n=18 capacity=23 allocations=1 inline=0 data_offset=-1
n=19 capacity=23 allocations=1 inline=0 data_offset=-1
n=20 capacity=23 allocations=1 inline=0 data_offset=-1
n=21 capacity=23 allocations=1 inline=0 data_offset=-1
n=22 capacity=23 allocations=1 inline=0 data_offset=-1
n=23 capacity=23 allocations=1 inline=0 data_offset=-1
n=24 capacity=31 allocations=1 inline=0 data_offset=-1
n=25 capacity=31 allocations=1 inline=0 data_offset=-1
n=26 capacity=31 allocations=1 inline=0 data_offset=-1
n=27 capacity=31 allocations=1 inline=0 data_offset=-1
n=28 capacity=31 allocations=1 inline=0 data_offset=-1
n=29 capacity=31 allocations=1 inline=0 data_offset=-1
n=30 capacity=31 allocations=1 inline=0 data_offset=-1
n=31 capacity=31 allocations=1 inline=0 data_offset=-1
n=32 capacity=39 allocations=1 inline=0 data_offset=-1
n=33 capacity=39 allocations=1 inline=0 data_offset=-1
n=34 capacity=39 allocations=1 inline=0 data_offset=-1
n=35 capacity=39 allocations=1 inline=0 data_offset=-1
n=36 capacity=39 allocations=1 inline=0 data_offset=-1
n=37 capacity=39 allocations=1 inline=0 data_offset=-1
n=38 capacity=39 allocations=1 inline=0 data_offset=-1
n=39 capacity=39 allocations=1 inline=0 data_offset=-1
n=40 capacity=47 allocations=1 inline=0 data_offset=-1

```

重复次数：5；数值重复一致：True。

## armv7l / gnu / member_extension

编译退出码：1；[原始命令](../raw/string_armv7l_member_extension_gnu_build.command.txt)

## armv7l / cxx / member_extension

编译退出码：1；[原始命令](../raw/string_armv7l_member_extension_cxx_build.command.txt)

## x86_64 / gnu / libcxx_extension

编译退出码：1；[原始命令](../raw/string_x86_64_libcxx_extension_gnu_build.command.txt)

## x86_64 / cxx / libcxx_extension

编译退出码：0；[原始命令](../raw/string_x86_64_libcxx_extension_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
invariants=1 size=9
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / allocation

编译退出码：0；[原始命令](../raw/string_x86_64_allocation_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_alloc size=100 capacity=100 unchanged=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / allocation

编译退出码：0；[原始命令](../raw/string_x86_64_allocation_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=bad_alloc size=100 capacity=103 unchanged=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / at

编译退出码：0；[原始命令](../raw/string_x86_64_at_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=out_of_range what=basic_string::at: __n (which is 3) >= this->size() (which is 3)
after size=3 hex=616263
valid_at_1=98
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / at

编译退出码：0；[原始命令](../raw/string_x86_64_at_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
exception=out_of_range what=basic_string
after size=3 hex=616263
valid_at_1=98
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / c_api

编译退出码：0；[原始命令](../raw/string_x86_64_c_api_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
embedded_nul size=3 hex=61007a
size=3 strlen=1 trailing_nul=1 npos=18446744073709551615 mutable_data=1
empty_nonnull=1 empty_nul=1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / c_api

编译退出码：0；[原始命令](../raw/string_x86_64_c_api_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
embedded_nul size=3 hex=61007a
size=3 strlen=1 trailing_nul=1 npos=18446744073709551615 mutable_data=1
empty_nonnull=1 empty_nul=1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / compare_hash

编译退出码：0；[原始命令](../raw/string_x86_64_compare_hash_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
compare_az=-25 compare_za=25 compare_length=-99 compare_equal=0
hash size=0 value=6142509188972423790
hash size=1 value=4993892634952068459
hash size=3 value=3663726644998027833
hash size=3 value=12492577553301056559
map_order=a:1,z:2,
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / compare_hash

编译退出码：0；[原始命令](../raw/string_x86_64_compare_hash_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
compare_az=-25 compare_za=25 compare_length=-1 compare_equal=0
hash size=0 value=11160318154034397263
hash size=1 value=2603192927274642682
hash size=3 value=4220206313085259313
hash size=3 value=174674430866144721
map_order=a:1,z:2,
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / construction

编译退出码：0；[原始命令](../raw/string_x86_64_construction_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
empty size=0 hex=
cstr size=1 hex=61
counted size=4 hex=61006263
range size=5 hex=72616e6765
copy_then_moved size=4 hex=61006263
moved_from size=0 hex=
moveassigned size=5 hex=72616e6765
long_move_size=200 source_size=0
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / construction

编译退出码：0；[原始命令](../raw/string_x86_64_construction_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
empty size=0 hex=
cstr size=1 hex=61
counted size=4 hex=61006263
range size=5 hex=72616e6765
copy_then_moved size=4 hex=61006263
moved_from size=0 hex=
moveassigned size=5 hex=72616e6765
long_move_size=200 source_size=0
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / gnu_extension

编译退出码：0；[原始命令](../raw/string_x86_64_gnu_extension_gnu_build.command.txt)

```text
size=9 text=extension
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / gnu_extension

编译退出码：1；[原始命令](../raw/string_x86_64_gnu_extension_cxx_build.command.txt)

## x86_64 / gnu / growth

编译退出码：0；[原始命令](../raw/string_x86_64_growth_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
push size=0 capacity=15
push size=16 capacity=30
push size=31 capacity=60
push size=61 capacity=120
push size=121 capacity=240
push size=241 capacity=480
reserve request=0 size=3 capacity=15
reserve request=1 size=3 capacity=15
reserve request=15 size=3 capacity=15
reserve request=16 size=3 capacity=30
reserve request=22 size=3 capacity=30
reserve request=23 size=3 capacity=30
reserve request=30 size=3 capacity=30
reserve request=31 size=3 capacity=31
reserve request=32 size=3 capacity=32
reserve request=63 size=3 capacity=63
reserve request=64 size=3 capacity=64
reserve request=100 size=3 capacity=100
reserve request=129 size=3 capacity=129
shrink before=200 after=15
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / growth

编译退出码：0；[原始命令](../raw/string_x86_64_growth_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
push size=0 capacity=22
push size=23 capacity=47
push size=48 capacity=95
push size=96 capacity=191
push size=192 capacity=383
reserve request=0 size=3 capacity=22
reserve request=1 size=3 capacity=22
reserve request=15 size=3 capacity=22
reserve request=16 size=3 capacity=22
reserve request=22 size=3 capacity=22
reserve request=23 size=3 capacity=25
reserve request=30 size=3 capacity=31
reserve request=31 size=3 capacity=31
reserve request=32 size=3 capacity=39
reserve request=63 size=3 capacity=63
reserve request=64 size=3 capacity=71
reserve request=100 size=3 capacity=103
reserve request=129 size=3 capacity=135
shrink before=207 after=22
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / iteration

编译退出码：0；[原始命令](../raw/string_x86_64_iteration_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
within_capacity_address_same=1 capacity=100 value=abcde
beyond_capacity_address_same=0 capacity=200 value=abcde
iteration=abcde
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / iteration

编译退出码：0；[原始命令](../raw/string_x86_64_iteration_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
within_capacity_address_same=1 capacity=103 value=abcde
beyond_capacity_address_same=0 capacity=207 value=abcde
iteration=abcde
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / layout

编译退出码：0；[原始命令](../raw/string_x86_64_layout_gnu_build.command.txt)

```text
LIB=libstdc++ VERSION=14 DATE=20240801 CXX11_ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
sizeof=32 alignof=8
n=0 capacity=15 allocations=0 inline=1 data_offset=16
n=1 capacity=15 allocations=0 inline=1 data_offset=16
n=2 capacity=15 allocations=0 inline=1 data_offset=16
n=3 capacity=15 allocations=0 inline=1 data_offset=16
n=4 capacity=15 allocations=0 inline=1 data_offset=16
n=5 capacity=15 allocations=0 inline=1 data_offset=16
n=6 capacity=15 allocations=0 inline=1 data_offset=16
n=7 capacity=15 allocations=0 inline=1 data_offset=16
n=8 capacity=15 allocations=0 inline=1 data_offset=16
n=9 capacity=15 allocations=0 inline=1 data_offset=16
n=10 capacity=15 allocations=0 inline=1 data_offset=16
n=11 capacity=15 allocations=0 inline=1 data_offset=16
n=12 capacity=15 allocations=0 inline=1 data_offset=16
n=13 capacity=15 allocations=0 inline=1 data_offset=16
n=14 capacity=15 allocations=0 inline=1 data_offset=16
n=15 capacity=15 allocations=0 inline=1 data_offset=16
n=16 capacity=16 allocations=1 inline=0 data_offset=-1
n=17 capacity=17 allocations=1 inline=0 data_offset=-1
n=18 capacity=18 allocations=1 inline=0 data_offset=-1
n=19 capacity=19 allocations=1 inline=0 data_offset=-1
n=20 capacity=20 allocations=1 inline=0 data_offset=-1
n=21 capacity=21 allocations=1 inline=0 data_offset=-1
n=22 capacity=22 allocations=1 inline=0 data_offset=-1
n=23 capacity=23 allocations=1 inline=0 data_offset=-1
n=24 capacity=24 allocations=1 inline=0 data_offset=-1
n=25 capacity=25 allocations=1 inline=0 data_offset=-1
n=26 capacity=26 allocations=1 inline=0 data_offset=-1
n=27 capacity=27 allocations=1 inline=0 data_offset=-1
n=28 capacity=28 allocations=1 inline=0 data_offset=-1
n=29 capacity=29 allocations=1 inline=0 data_offset=-1
n=30 capacity=30 allocations=1 inline=0 data_offset=-1
n=31 capacity=31 allocations=1 inline=0 data_offset=-1
n=32 capacity=32 allocations=1 inline=0 data_offset=-1
n=33 capacity=33 allocations=1 inline=0 data_offset=-1
n=34 capacity=34 allocations=1 inline=0 data_offset=-1
n=35 capacity=35 allocations=1 inline=0 data_offset=-1
n=36 capacity=36 allocations=1 inline=0 data_offset=-1
n=37 capacity=37 allocations=1 inline=0 data_offset=-1
n=38 capacity=38 allocations=1 inline=0 data_offset=-1
n=39 capacity=39 allocations=1 inline=0 data_offset=-1
n=40 capacity=40 allocations=1 inline=0 data_offset=-1
```

重复次数：5；数值重复一致：True。

## x86_64 / cxx / layout

编译退出码：0；[原始命令](../raw/string_x86_64_layout_cxx_build.command.txt)

```text
LIB=libc++ VERSION=220108 ABI=1
CPLUSPLUS=201703 POINTER_BYTES=8
sizeof=24 alignof=8
n=0 capacity=22 allocations=0 inline=1 data_offset=1
n=1 capacity=22 allocations=0 inline=1 data_offset=1
n=2 capacity=22 allocations=0 inline=1 data_offset=1
n=3 capacity=22 allocations=0 inline=1 data_offset=1
n=4 capacity=22 allocations=0 inline=1 data_offset=1
n=5 capacity=22 allocations=0 inline=1 data_offset=1
n=6 capacity=22 allocations=0 inline=1 data_offset=1
n=7 capacity=22 allocations=0 inline=1 data_offset=1
n=8 capacity=22 allocations=0 inline=1 data_offset=1
n=9 capacity=22 allocations=0 inline=1 data_offset=1
n=10 capacity=22 allocations=0 inline=1 data_offset=1
n=11 capacity=22 allocations=0 inline=1 data_offset=1
n=12 capacity=22 allocations=0 inline=1 data_offset=1
n=13 capacity=22 allocations=0 inline=1 data_offset=1
n=14 capacity=22 allocations=0 inline=1 data_offset=1
n=15 capacity=22 allocations=0 inline=1 data_offset=1
n=16 capacity=22 allocations=0 inline=1 data_offset=1
n=17 capacity=22 allocations=0 inline=1 data_offset=1
n=18 capacity=22 allocations=0 inline=1 data_offset=1
n=19 capacity=22 allocations=0 inline=1 data_offset=1
n=20 capacity=22 allocations=0 inline=1 data_offset=1
n=21 capacity=22 allocations=0 inline=1 data_offset=1
n=22 capacity=22 allocations=0 inline=1 data_offset=1
n=23 capacity=25 allocations=1 inline=0 data_offset=-1
n=24 capacity=31 allocations=1 inline=0 data_offset=-1
n=25 capacity=31 allocations=1 inline=0 data_offset=-1
n=26 capacity=31 allocations=1 inline=0 data_offset=-1
n=27 capacity=31 allocations=1 inline=0 data_offset=-1
n=28 capacity=31 allocations=1 inline=0 data_offset=-1
n=29 capacity=31 allocations=1 inline=0 data_offset=-1
n=30 capacity=31 allocations=1 inline=0 data_offset=-1
n=31 capacity=31 allocations=1 inline=0 data_offset=-1
n=32 capacity=39 allocations=1 inline=0 data_offset=-1
n=33 capacity=39 allocations=1 inline=0 data_offset=-1
n=34 capacity=39 allocations=1 inline=0 data_offset=-1
n=35 capacity=39 allocations=1 inline=0 data_offset=-1
n=36 capacity=39 allocations=1 inline=0 data_offset=-1
n=37 capacity=39 allocations=1 inline=0 data_offset=-1
n=38 capacity=39 allocations=1 inline=0 data_offset=-1
n=39 capacity=39 allocations=1 inline=0 data_offset=-1
n=40 capacity=47 allocations=1 inline=0 data_offset=-1
```

重复次数：5；数值重复一致：True。

## x86_64 / gnu / member_extension

编译退出码：1；[原始命令](../raw/string_x86_64_member_extension_gnu_build.command.txt)

## x86_64 / cxx / member_extension

编译退出码：1；[原始命令](../raw/string_x86_64_member_extension_cxx_build.command.txt)
