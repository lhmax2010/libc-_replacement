# QuickBuild 1165447 失败日志关键摘录

以下仅摘录每个可得架构的首个直接失败证据；完整日志位于被 `.gitignore` 排除的 `logs/quickbuild/build_1165447/`，未提交。

## `lapack`

- 来源：`0006_standard-aarch64_lapack_failed_log.txt`；SHA256 `56c71f99488dcc13702ffa12f1fa0fbf1cf0b59e51ef30fba22aa1679a0d5464`
- 第 250 行：

```text
[   28s]     gfortran: error: unrecognized command-line option '-stdlib=libc++'
```

- 来源：`0026_standard-x86_64_lapack_failed_log.txt`；SHA256 `60d77a1fab08c822578a393a1565eb16c02b74bde8aa2b02ce8f097152471ec8`
- 第 246 行：

```text
[   20s]     gfortran: error: unrecognized command-line option '-stdlib=libc++'
```

## `openblas`

- 来源：`0012_standard-aarch64_openblas_failed_log.txt`；SHA256 `a11221e15f2a246d6a10d9409d9f9859702e011054c30c74afa7a444cf38f6b7`
- 第 14962 行：

```text
[  205s] aarch64-tizen-linux-gnu-gcc: error: unrecognized command-line option '-stdlib=libc++'
```

- 来源：`0032_standard-x86_64_openblas_failed_log.txt`；SHA256 `5fbc0bd2c1325682830aea593a83aebce11ceca71e091f2dca5798fd8916dc03`
- 第 22206 行：

```text
[  576s] x86_64-tizen-linux-gnu-gcc: error: unrecognized command-line option '-stdlib=libc++'
```

## `onnxruntime`

- 来源：`0011_standard-aarch64_onnxruntime_failed_log.txt`；SHA256 `75a70e15cfbd2e1e2dbc21547d2f5537be9f5a2661ca430552196db3497817f0`
- 第 9370 行：

```text
[   38s]     aarch64-tizen-linux-gnu-gcc: error: unrecognized command-line option '-stdlib=libc++'
```

- 来源：`0031_standard-x86_64_onnxruntime_failed_log.txt`；SHA256 `3a56dfecd4f45e1c5ec2d0ba4226982fb9bc571056d8e59b2cac09a807ec9df0`
- 第 9365 行：

```text
[   29s]     x86_64-tizen-linux-gnu-gcc: error: unrecognized command-line option '-stdlib=libc++'
```

## `yaca`

- 来源：`0021_standard-aarch64_yaca_failed_log.txt`；SHA256 `dfd9e29692816b3d40caee35747f3198e4c5714b434e7708a1be0296c0bae157`
- 第 317 行：

```text
[   42s]     aarch64-tizen-linux-gnu-gcc: error: unrecognized command-line option '-stdlib=libc++'
```

- 来源：`0041_standard-x86_64_yaca_failed_log.txt`；SHA256 `5f1b0bb01a42b3c7898c86598814cc49fc9b9467d6d0de2ca7884d4bc470c969`
- 第 318 行：

```text
[   28s]     x86_64-tizen-linux-gnu-gcc: error: unrecognized command-line option '-stdlib=libc++'
```

## `tidl`

- 来源：`0018_standard-aarch64_tidl_failed_log.txt`；SHA256 `537dfdfe1e5eab337409e75c987ffede6bfb603f2e86248ea3574388198ca9c5`
- 第 506 行：

```text
[   53s] /usr/bin/ld: cannot find -lc++: No such file or directory
```

- 来源：`0038_standard-x86_64_tidl_failed_log.txt`；SHA256 `2b0f68339e7269a9833459414fcdb2335ecdf264f502326b62f49f6287e4ab9c`
- 第 511 行：

```text
[   82s] /usr/bin/ld: cannot find -lc++: No such file or directory
```

## `noise-suppression`

- 来源：`0010_standard-aarch64_noise-suppression_failed_log.txt`；SHA256 `4877c8e00baf56d0216ed4fcde3df097da0b0073a18ebe30f8ee34e35970a3dd`
- 第 236 行：

```text
[   15s] /home/abuild/rpmbuild/BUILD/noise-suppression-1.2/src/NoiseSuppression.cpp:154:2: error: no member named 'copy' in namespace 'std'; did you mean 'bcopy'?
```

- 来源：`0030_standard-x86_64_noise-suppression_failed_log.txt`；SHA256 `d53aee2aece7fabd61c0a1e32b5dbfb56a703b3e40a0f467e5096b25cb41aa2c`
- 第 233 行：

```text
[   14s] /home/abuild/rpmbuild/BUILD/noise-suppression-1.2/src/NoiseSuppression.cpp:154:2: error: no member named 'copy' in namespace 'std'; did you mean 'bcopy'?
```

## `tensorflow2`

- 来源：`0017_standard-aarch64_tensorflow2_failed_log.txt`；SHA256 `ab117d79ab343f59490296349154914d194c63d0ef47fac2a6afc6ff18c8de7c`
- 第 2172 行：

```text
[  214s] code_generator.cc:(.text+0xf0): undefined reference to `std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace(unsigned long, unsigned long, char const*, unsigned long)'
```

- 来源：`0037_standard-x86_64_tensorflow2_failed_log.txt`；SHA256 `bfde236e946accfe7eb3e4e06ba8055dbaf48df5064d92115a86646fed4abd09`
- 第 2180 行：

```text
[  202s] code_generator.cc:(.text+0x11c): undefined reference to `std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace(unsigned long, unsigned long, char const*, unsigned long)'
```

## `openusd`

- 来源：`0013_standard-aarch64_openusd_failed_log.txt`；SHA256 `7bdbc4911b2fbdfc755f820bff75db1def5c17cd94f69828c2783460e4a69af4`
- 第 1180 行：

```text
[  112s] /home/abuild/rpmbuild/BUILD/openusd-24.03/pxr/base/gf/half.h:45:12: error: unknown type name 'size_t'; did you mean 'std::size_t'?
```

- 来源：`0033_standard-x86_64_openusd_failed_log.txt`；SHA256 `6779b1b8e89d8f68e0155133052adb794d1bd21321879196611d5117e9670743`
- 第 1170 行：

```text
[  132s] /home/abuild/rpmbuild/BUILD/openusd-24.03/pxr/base/gf/half.h:45:12: error: unknown type name 'size_t'; did you mean 'std::size_t'?
```

## `protobuf`

- 来源：`0015_standard-aarch64_protobuf_failed_log.txt`；SHA256 `d5157bf88f6aed2210a188b0f5632aae326d730e3bd2217e5f961f8a09e5ed2d`
- 第 708 行：

```text
[  210s] /usr/include/absl/strings/internal/str_split_internal.h:139:(.text._ZN4absl12lts_2025081416strings_internal13SplitIteratorINS1_8SplitterINS0_6ByCharENS0_9SkipEmptyENSt3__117basic_string_viewIcNS6_11char_traitsIcEEEEEEEppEv[_ZN4absl12lts_2025081416strings_internal13SplitIteratorINS1_8SplitterINS0_6ByCharENS0_9SkipEmptyENSt3__117basic_string_viewIcNS6_11char_traitsIcEEEEEEEppEv]+0x3c): undefined reference to `absl::lts_20250814::ByChar::Find(std::__1::basic_string_view<char, std::__1::char_traits<char> >, unsigned long) const'
```

- 来源：`0035_standard-x86_64_protobuf_failed_log.txt`；SHA256 `c83ecf44c4a4c7c67b4ab6a4f14802b18b56b9de0e6a97689cc9f4c812206bee`
- 第 763 行：

```text
[  200s] /usr/include/absl/strings/internal/str_split_internal.h:139:(.text._ZN4absl12lts_2025081416strings_internal13SplitIteratorINS1_8SplitterINS0_6ByCharENS0_9SkipEmptyENSt3__117basic_string_viewIcNS6_11char_traitsIcEEEEEEEppEv[_ZN4absl12lts_2025081416strings_internal13SplitIteratorINS1_8SplitterINS0_6ByCharENS0_9SkipEmptyENSt3__117basic_string_viewIcNS6_11char_traitsIcEEEEEEEppEv]+0x30): undefined reference to `absl::lts_20250814::ByChar::Find(std::__1::basic_string_view<char, std::__1::char_traits<char> >, unsigned long) const'
```

## `lottie-player`

- 来源：`0009_standard-aarch64_lottie-player_failed_log.txt`；SHA256 `0d159e877d81dca3da05ff58e196960d588de05e30cdb7ddc66576d4ec114430`
- 第 303 行：

```text
[   27s] ../format:1:1: error: unknown type name 'git'
```

- 来源：`0029_standard-x86_64_lottie-player_failed_log.txt`；SHA256 `a905e254292f33078b0c4609e1117cf2974b7fa5b6a47b14411ff8df7dee2cfe`
- 第 294 行：

```text
[   32s] ../format:1:1: error: unknown type name 'git'
```

## `libcynara-commons`

- 来源：`0007_standard-aarch64_libcynara-commons_failed_log.txt`；SHA256 `e004ccca26805ebc77a85e5aad8683baf3c153c7421d1c9ff99441dc20ac2424`
- 第 1038 行：

```text
[  145s] /home/abuild/rpmbuild/BUILD/cynara-0.26.0/test/cyad/commandline_options.cpp:40:21: error: ignoring return value of function declared with 'nodiscard' attribute [-Werror,-Wunused-result]
```

- 来源：`0027_standard-x86_64_libcynara-commons_failed_log.txt`；SHA256 `c1478baeaf59f8cf619cc464d2449857454541f62de729acad50e98e5c349d80`
- 第 1021 行：

```text
[  184s] /home/abuild/rpmbuild/BUILD/cynara-0.26.0/test/cyad/commandline_options.cpp:40:21: error: ignoring return value of function declared with 'nodiscard' attribute [-Werror,-Wunused-result]
```

## `rive-tizen`

- 来源：`0016_standard-aarch64_rive-tizen_failed_log.txt`；SHA256 `dbf0f03519998a4763b4a0805062fe7fdb9075e2a914b5f7d8058dd0ad86632e`
- 第 1182 行：

```text
[  148s] DSLType.cpp:(.text.unlikely._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC2EPKcmRKS3_.part.0+0x10): undefined reference to `std::__throw_logic_error(char const*)'
```

- 来源：`0036_standard-x86_64_rive-tizen_failed_log.txt`；SHA256 `cd12d5986b10574a6324d62e93ed926642bf1985c7501863296b6265a1beb13e`
- 第 4328 行：

```text
[  192s] SkImageFilterCache.cpp:(.text.unlikely._ZN12_GLOBAL__N_19CacheImpl3setERK21SkImageFilterCacheKeyPK13SkImageFilterRKN4skif12FilterResultE+0x8): undefined reference to `std::__throw_length_error(char const*)'
```

## `glog`

- 来源：`0003_standard-aarch64_glog_failed_log.txt`；SHA256 `2b4fc2e530c51254858f94e031f930325d0a7400404e40191c7c3e1642634f2b`
- 第 658 行：

```text
[  146s] src/logging_unittest.cc:575:3: error: use of undeclared identifier 'auto_ptr'
```

- 来源：`0024_standard-x86_64_glog_failed_log.txt`；SHA256 `fa0d1303ae2b529b0806f50a66fe037616577d8b1de7373927badbf66ea1a493`
- 第 656 行：

```text
[   39s] src/logging_unittest.cc:575:3: error: use of undeclared identifier 'auto_ptr'
```

## `c-mock`

- 来源：`0001_standard-aarch64_c-mock_failed_log.txt`；SHA256 `493a202afe21d858c32416fd2f4834b1f038e34b8f8776680e0044e84899a303`
- 第 258 行：

```text
[   32s] cmock-function-class-mockers-old-style_test.cc:(.text.startup+0x178): undefined reference to `testing::internal::MakeAndRegisterTestInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char const*, char const*, char const*, testing::internal::CodeLocation, void const*, void (*)(), void (*)(), testing::internal::TestFactoryBase*)'
```

- 来源：`0022_standard-x86_64_c-mock_failed_log.txt`；SHA256 `9b1bee2da35d7a56dcc4401d0d90ffe7007bdae2ecfdfcf366d3484ed1db3c0d`
- 第 246 行：

```text
[   35s] cmock-function-class-mockers-old-style_test.cc:(.text.startup+0x176): undefined reference to `testing::internal::MakeAndRegisterTestInfo(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, char const*, char const*, char const*, testing::internal::CodeLocation, void const*, void (*)(), void (*)(), testing::internal::TestFactoryBase*)'
```

## `docker-launcher`

- 来源：`0002_standard-aarch64_docker-launcher_failed_log.txt`；SHA256 `faad808e2bb17ccfd72a766c7043ee6a0cb8df53ed06c8388d2dd7eb7ffc697e`
- 第 646 行：

```text
[   70s] /home/abuild/rpmbuild/BUILD/docker-launcher-1.0.1/src/Controller/DockerController.cpp:275:(.text+0x23cc): undefined reference to `Json::Value::operator[](std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)'
```

## `hdf5`

- 来源：`0005_standard-aarch64_hdf5_failed_log.txt`；SHA256 `07bc1a42cf111805610f4e0a9d591157fb1c8c152e068050f175f64ef604d163`
- 第 358 行：

```text
[   50s] configure: error: Fortran compiler requires either intrinsic functions SIZEOF or STORAGE_SIZE
```

- 来源：`0025_standard-x86_64_hdf5_failed_log.txt`；SHA256 `8ccf81277765c7ad46477c7131c78278f0a6b87e9f6f8e3edf56c494fa3f0496`
- 第 349 行：

```text
[   23s] configure: error: Fortran compiler requires either intrinsic functions SIZEOF or STORAGE_SIZE
```

## `hailo-rt`

- 来源：`0004_standard-aarch64_hailo-rt_failed_log.txt`；SHA256 `1692b05ba91a5bf670370707b5924448374e049e33272d26eacd5241267f9d00`
- 第 1289 行：

```text
[  122s] /home/abuild/rpmbuild/BUILD/hailo-rt-0.1/hailort/external/spdlog-src/include/spdlog/fmt/bundled/core.h:378:25: error: implicit instantiation of undefined template 'std::char_traits<fmt::internal::char8_type>'
```
