# 236 个“不需要改”包的分组说明

本文件把共同判据与逐包差异分开。所有判定均沿用 R100；运行时精确符号边覆盖 armv7l，镜像归属覆盖已保存的六份标准清单。

| 分组 | 数量 | 共同依据 |
|---|---:|---|
| 不含 C++ 载体 | 181 | 三架构四项检查（实际构建日志、C++ 运行时依赖、静态归档全局 C++ 符号、C++ 头文件载体）结果均为否；没有可施加标准库选择的 C++ 构建或交付边界。 |
| C++ 只出现在构建或非运行时载体 | 29 | 有构建期 C++ 动作或头文件载体，但包级运行时信号和精确运行时图不构成由本包固定实现的跨包 C++ ABI 边界。进入镜像与否逐包注明。 |
| 独立工具或仅在 Base 拓扑中运行 | 17 | 产物直接链接 libstdc++，但 armv7l 完整精确拓扑测得通向 Unified 的运行时边为 0；Base 内部也没有 C++ 语义边，或仅有纯 C 接口。 |
| 跨包边界只有 C 接口 | 4 | 产物自身可以用 C++ 实现并链接 libstdc++，但实际 consumer/provider 符号交集只有 C 接口；C++ 对象布局、异常和析构规则不跨边界。 |
| 含 C++ 静态归档但无消费边 | 2 | 静态归档确有 C++ 符号，但精确拓扑中的 Unified 与 Base 内部运行时消费边均为 0，当前没有被迁移侧吸收到同一进程。 |
| 头文件由使用方编译 | 2 | C++ 载体是头文件而非本包固定实现的运行时；使用方会用自己的编译器和标准库实例化。 |
| 已是 libc++ 运行时 | 1 | 该包就是 libc++/libc++abi 提供方，不是仍消费 libstdc++、需要切换的对象。 |

## 不含 C++ 载体（181 包）

三架构四项检查（实际构建日志、C++ 运行时依赖、静态归档全局 C++ 符号、C++ 头文件载体）结果均为否；没有可施加标准库选择的 C++ 构建或交付边界。

| 包 | 仓库用途摘要 | 镜像与工具差异 |
|---|---|---|
| `acl` | Commands for Manipulating POSIX Access Control Lists | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `alsa-lib` | The Advanced Linux Sound Architecture (ALSA) library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `alsa-utils` | Advanced Linux Sound Architecture (ALSA) utilities | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `app-rootstrap-checker` | Package for app rootstrap checker | 否：六个精确镜像清单无该 VCS；是：rootstrap 完整性检查器 |
| `attr` | Commands for Manipulating Extended Attributes | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `autoconf` | A GNU Tool for Automatically Configuring Source Code | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `autoconf-archive` | Collection of reusable Autoconf macros | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `autoconf213` | A GNU Tool for Automatically Configuring Source Code | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `bash` | The GNU Bourne Again shell | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `bc` | GNU Command Line Calculator | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `boost-jam` | An Enhanced Make Replacement | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `build` | A Script to Build SUSE Linux RPMs | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `build-compare` | Build Result Compare Script | 否：六个精确镜像清单无该 VCS；是：构建结果比较工具 |
| `bzip2` | A Program for Compressing Files | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `chrpath` | Modify rpath of compiled programs | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `coreutils` | The GNU core utilities: a set of tools commonly used in shell scripts | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `cpio` | A GNU archiving program | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `curl` | A utility for getting files from remote servers (FTP, HTTP, and others) | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `dash` | Small and fast POSIX-compliant shell | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `default-ac-domains` | The definition of default ac domains | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `docbook-xsl-stylesheets` | XSL Stylesheets for DocBook 4 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `docbook_4` | DocBook DTD Version 4.x | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `dos2unix` | Text converters to and from DOS/MAC to UNIX | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `e2fsprogs` | Utilities for the Second Extended File System | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `ed` | Standard UNIX Line Editor | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `expect` | A Tool for Automating Interactive Programs | 否：六个精确镜像清单无该 VCS；是：交互程序自动化/测试工具 |
| `fdupes` | Identifying or deleting duplicate files | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `filesystem` | The basic directory layout for a Linux system | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `findutils` | The GNU versions of find utilities (find and xargs) | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `fribidi` | Free Implementation of BiDi Algorithm | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `gawk` | GNU awk | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `gcc-contrib` | GCC related scripts | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `gdbm` | GNU Database Routines | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `giflib` | Library for manipulating GIF format image files | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `grep` | Print lines matching a pattern | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `gtk-doc` | GTK+ DocBook Documentation Generator | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `gzip` | The GNU data compression program | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `hal-rootstrap-checker` | Package for hal rootstrap checker | 否：六个精确镜像清单无该 VCS；是：HAL rootstrap 完整性检查器 |
| `help2man` | Create Simple Man Pages from --help Output | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `hostname` | Utility to set/show the host name or domain name | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `iniparser` | Stand-alone ini file parsing library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `intltool` | Internationalization Tool Collection | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `iproute2` | collection of utilities for controlling TCP/IP networking and traffic control in Linux | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `iptables` | Tools for managing Linux kernel packet filtering capabilities | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `iso_ent` | Character Entity Sets for ISO 8879:1986 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `itstool` | Tool to translate XML documents using PO files | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `kmod` | Utilities to load modules into the kernel | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `less` | A text file browser similar to more, but better | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libarchive` | A library for handling streaming archive formats | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libbpf` | Libbpf library | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libcap` | Library for Capabilities (linux-privs) Support | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libdrm` | Userspace interface to kernel DRM services | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libelf0` | An ELF Object File Access Library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libev` | High-performance event loop/event model with lots of features | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libevent` | Library Providing an Event Handling API | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libgcrypt` | The GNU Crypto Library | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libgpg-error` | Library That Defines Common Error Values for All GnuPG Components | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libjpeg-turbo` | A MMX/SSE2 accelerated library for manipulating JPEG image files | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libmd` | Message digest functions from BSD systems | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libnl2` | Library for netlink sockets | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libnl3` | Library for netlink sockets | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libogg` | Ogg Bitstream Library | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libpciaccess` | PCI access library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libpsl` | C library for the Publix Suffix List | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libtirpc` | Transport Independent RPC Library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libvorbis` | The Vorbis General Audio Compression Codec | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libwebp` | WebP image codec libraries | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libxcrypt` | Extended crypt library for descrypt, md5crypt, bcrypt, and others | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libxml2` | A Library to Manipulate XML Files | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libxslt` | XSL Transformation Library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `libzio` | A Library for Accessing Compressed Text Files | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `linux-glibc-devel` | Linux headers for userspace development | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `m4` | GNU m4 macro processor | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `make` | GNU make | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `meson` | Python-based build system | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `mm-common` | Common build files of the C++ bindings | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `mtdev` | Multitouch Protocol Translation Library | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `net-tools` | Basic networking tools | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `nspr` | Netscape Portable Runtime | 是：tizen-headed-aarch64;tizen-headed-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `nss` | Network Security Services | 是：tizen-headed-aarch64;tizen-headed-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `openssl1.1` | Secure Sockets Layer and cryptography libraries and tools | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `openssl3` | Secure Sockets Layer and cryptography libraries and tools | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `opus` | An audio codec for use in low-delay speech and audio communication | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `pam` | A Security Tool that Provides Authentication for Applications | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `patch` | The GNU patch command, for modifying/upgrading files | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `pcre2` | Perl-compatible regular expression library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl` | The Perl interpreter | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-Devel-Symdump` | Dump symbol names or the symbol table | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-HTML-Parser` | Perl module for parsing HTML | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-HTML-Tagset` | HTML::Tagset - data tables useful in parsing HTML | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-Module-Build` | Module::Build - Build and install Perl modules | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-PerlIO-gzip` | Perl extension to provide a PerlIO layer to gzip/gunzip | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-Pod-Coverage` | Checks if the documentation of a module is comprehensive | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-Software-License` | Software::License - packages that provide templated software licenses | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-Test-Pod` | check for POD errors in files | 否：六个精确镜像清单无该 VCS；是：测试模块 |
| `perl-Test-Pod-Coverage` | Test::Pod::Coverage Perl module | 否：六个精确镜像清单无该 VCS；是：测试/覆盖率模块 |
| `perl-TimeDate` | A Perl module for time and date manipulation | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-URI` | A Perl module implementing URI parsing and manipulation | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-XML-Parser` | A perl module for parsing XML documents | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-gettext` | Message handling functions | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-inc-latest` | inc::latest - use modules bundled in inc/ if they are newer than installed ones | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-json` | JSON (JavaScript Object Notation) encoder/decoder | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `perl-libwww-perl` | A Perl interface to the World-Wide Web | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `popt` | A C library for parsing command line parameters | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `procps-ng` | System and process monitoring utilities | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；是：进程监控与诊断工具 |
| `python-MarkupSafe` | Implements a XML/HTML/XHTML Markup safe string for Python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-appdirs` | A small Python module for determining platform-specific dirs | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-argparse` | Python command-line parsing library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-funcsigs` | Python function signatures from PEP362 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-libxml2` | python2 Bindings for libxml2 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-linecache2` | Backports of the linecache module | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-lxml` | Powerful and Pythonic XML processing library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-magic` | Python module to use libmagic | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-mako` | A super-fast Python templating language | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-markdown` | Markdown implementation in Python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-mock` | A Python Mocking and Patching Library for Testing | 否：六个精确镜像清单无该 VCS；是：测试 mock 库 |
| `python-nose` | Nose extends unittest to make testing easier | 否：六个精确镜像清单无该 VCS；是：测试框架 |
| `python-packaging` | Core utilities for Python packages | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-pbr` | Python Build Reasonableness | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-ply` | Python Lex & Yacc | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-pygments` | A syntax highlighting engine written in Python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-pyparsing` | Grammar Parser Library for Python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-setuptools` | Easily download, build, install, upgrade, and uninstall Python packages | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-six` | Python 2 and 3 compatibility utilities | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-traceback2` | Backports of the traceback module | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python-unittest2` | The new features in unittest for Python 2.7 backported to Python 2.3+ | 否：六个精确镜像清单无该 VCS；是：测试框架 |
| `python3-MarkupSafe` | Implements a XML/HTML/XHTML Markup safe string for Python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-appdirs` | A small Python module for determining platform-specific dirs | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-build` | Simple PEP517 package builder | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-calver` | Setuptools extension for CalVer package versions | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-cython` | The Cython compiler for writing C extensions for the python3 language | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-editables` | Editable installations | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-flit_core` | Distribution-building parts of Flit | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-hatchling` | Build backend used by Hatch | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-libxml2` | python3 Bindings for libxml2 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-lxml` | Powerful and Pythonic XML processing library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-magic` | Python3 module to use libmagic | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-mako` | A super-fast Python templating language | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-markdown` | Markdown implementation in Python3 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-nose` | Nose extends unittest to make testing easier | 否：六个精确镜像清单无该 VCS；是：测试框架 |
| `python3-packaging` | Core utilities for Python3 packages | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-pathspec` | Utility library for gitignore style pattern matching of file paths | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-pbr` | Python Build Reasonableness | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-pip` | A Python package management system | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-pluggy` | plugin and hook calling mechanisms for python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-ply` | Python Lex & Yacc | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-pygments` | A syntax highlighting engine written in Python3 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-pyparsing` | Grammar Parser Library for Python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-pyproject_hooks` | Wrappers to call pyproject.toml-based build backend hooks | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-setuptools` | Easily download, build, install, upgrade, and uninstall Python packages | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-setuptools_scm` | setuptools_scm handles managing your Python package versions | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-six` | Python 2 and 3 compatibility utilities | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-trove_classifiers` | Canonical source for classifiers on PyPI | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-typing_extensions` | Backported and Experimental Type Hints for Python 3.8+ | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `python3-wheel` | A built-package format for Python | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `qemu-linux-user` | Machine emulator and virtualizer | 否：六个精确镜像清单无该 VCS；是：用户态模拟器 |
| `readline` | The Readline Library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `sed` | A Stream-Oriented Non-Interactive Text Editor | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `setup` | A set of system configuration and setup files | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `sgml-skel` | Helper Scripts for the SGML System | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `shadow-utils` | Utilities for managing accounts and shadow password files | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `smack` | Selection of tools for developers working with Smack | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `tar` | GNU implementation of tar ((t)ape (ar)chiver) | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `tcl` | The Tcl Programming Language | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `texinfo` | Tools Needed to Create Documentation from Texinfo Sources | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `tzdata` | Timezone Descriptions | 是：tizen-headed-aarch64;tizen-headed-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `unzip` | A utility for unpacking zip files | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `userspace-rcu` | RCU (read-copy-update) implementation in user-space | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `uthash` | A hash table for C structures | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `util-macros` | X.Org build utilities | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `vim` | Vi IMproved | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `which` | Displays where a particular program in your path is located | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `xdgmime` | Querying information about file type | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `xmlcharent` | XML Character Entities | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `xmlsec1` | Library providing support for "XML Signature" and "XML Encryption" standards | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `xmlstarlet` | Command Line XML Toolkit | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `xxhash` | Non-cryptographic hash algorithm | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `xz` | A Program for Compressing Files | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `yasm` | Yasm Modular Assembler | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `zip` | File compression program | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |
| `zlib` | Data Compression Library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准 |

## C++ 只出现在构建或非运行时载体（29 包）

有构建期 C++ 动作或头文件载体，但包级运行时信号和精确运行时图不构成由本包固定实现的跨包 C++ ABI 边界。进入镜像与否逐包注明。

| 包 | 仓库用途摘要 | 镜像与工具差异 |
|---|---|---|
| `automake` | A Program for Automatically Generating GNU-Style Makefile.in Files | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`automake` 的仓库摘要将其用途定义为“A Program for Automatically Generating GNU-Style Makefile.in Files”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `binutils-riscv64` | GNU Binutils | 否：六个精确镜像清单无该 VCS；是：交叉架构二进制检查与诊断工具；`binutils-riscv64` 的仓库摘要将其用途定义为“GNU Binutils”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `c-ares` | Library for asynchronous name resolves | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`c-ares` 的仓库摘要将其用途定义为“Library for asynchronous name resolves”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `dejagnu` | Framework for testing other programs. | 否：六个精确镜像清单无该 VCS；是：测试框架；`dejagnu` 的仓库摘要将其用途定义为“Framework for testing other programs.”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `diffutils` | GNU diff Utilities | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`diffutils` 的仓库摘要将其用途定义为“GNU diff Utilities”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `expat` | XML Parser Toolkit | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`expat` 的仓库摘要将其用途定义为“XML Parser Toolkit”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `file` | A Tool to Determine File Types | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`file` 的仓库摘要将其用途定义为“A Tool to Determine File Types”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `flex` | Fast Lexical Analyzer Generator | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`flex` 的仓库摘要将其用途定义为“Fast Lexical Analyzer Generator”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `keyutils` | Linux Key Management Utilities | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`keyutils` 的仓库摘要将其用途定义为“Linux Key Management Utilities”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `krb5` | MIT Kerberos5 Implementation--Libraries | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`krb5` 的仓库摘要将其用途定义为“MIT Kerberos5 Implementation--Libraries”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `libdbuspolicy-header` | Header-only package of libdbuspolicy | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`libdbuspolicy-header` 的仓库摘要将其用途定义为“Header-only package of libdbuspolicy”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `libffi` | A portable foreign function interface library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`libffi` 的仓库摘要将其用途定义为“A portable foreign function interface library”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `libsndfile` | C library for reading and writing sound files | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`libsndfile` 的仓库摘要将其用途定义为“C library for reading and writing sound files”。实际构建日志中有 C++ 编译动作，已发布包还包含 3 个架构份次的 C++ 头文件载体；这些头文件由使用方自己的编译器和标准库编译。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `libtool` | A Tool to Build Shared Libraries | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`libtool` 的仓库摘要将其用途定义为“A Tool to Build Shared Libraries”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `libunistring` | GNU Unicode string library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`libunistring` 的仓库摘要将其用途定义为“GNU Unicode string library”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `libunwind` | Unwind Library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；是：产出 libunwind-tests 测试子包；`libunwind` 的仓库摘要将其用途定义为“Unwind Library”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `lttng-ust` | LTTng Userspace Tracer library | 否：六个精确镜像清单无该 VCS；是：用户态跟踪设施；`lttng-ust` 的仓库摘要将其用途定义为“LTTng Userspace Tracer library”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `lua` | Small Embeddable Language with Simple Procedural Syntax | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`lua` 的仓库摘要将其用途定义为“Small Embeddable Language with Simple Procedural Syntax”。已发布包还包含 3 个架构份次的 C++ 头文件载体；这些头文件由使用方自己的编译器和标准库编译。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `pkg-config` | A library management system | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`pkg-config` 的仓库摘要将其用途定义为“A library management system”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `python` | Python Interpreter | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`python` 的仓库摘要将其用途定义为“Python Interpreter”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它在 Base 内部测得的跨包调用也只经过 C 接口。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `python3` | Python 3 Interpreter | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`python3` 的仓库摘要将其用途定义为“Python 3 Interpreter”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它在 Base 内部测得的跨包调用也只经过 C 接口。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `python3-base` | Python 3 Interpreter and Stdlib Core | 否：六个精确镜像清单无该 VCS；是：产出 python3-testsuite 子包；`python3-base` 的仓库摘要将其用途定义为“Python 3 Interpreter and Stdlib Core”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `qemu-accel` | Native binaries for speeding up cross compile | 否：六个精确镜像清单无该 VCS；是：交叉构建加速/模拟工具；`qemu-accel` 的仓库摘要将其用途定义为“Native binaries for speeding up cross compile”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本不在已核对的六份标准设备镜像清单中。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `rpm` | The Package Manager | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`rpm` 的仓库摘要将其用途定义为“The Package Manager”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `sqlite` | Embeddable SQL Database Engine | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`sqlite` 的仓库摘要将其用途定义为“Embeddable SQL Database Engine”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `update-alternatives` | Maintain symbolic links determining default commands | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`update-alternatives` 的仓库摘要将其用途定义为“Maintain symbolic links determining default commands”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `util-linux` | A collection of basic system utilities | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`util-linux` 的仓库摘要将其用途定义为“A collection of basic system utilities”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它在 Base 内部测得的跨包调用也只经过 C 接口。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `v4l-utils` | Utilities for video4linux and DVB devices | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`v4l-utils` 的仓库摘要将其用途定义为“Utilities for video4linux and DVB devices”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |
| `zstd` | Zstandard compression tools | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`zstd` 的仓库摘要将其用途定义为“Zstandard compression tools”。实际构建日志中有 C++ 编译动作，但已发布包没有 C++ 头文件载体。包级检查同时给出：直接 C++ 运行时依赖为“NO”、含全局 C++ 符号的静态归档数量为 0；armv7l 精确运行时图中通向 Unified 的 C++ 边为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。所以这里的 C++ 只存在于构建过程或随使用方编译的非运行时载体，不形成由本包固定标准库实现的跨包 C++ ABI 边界，无需切换本包。 |

## 独立工具或仅在 Base 拓扑中运行（17 包）

产物直接链接 libstdc++，但 armv7l 完整精确拓扑测得通向 Unified 的运行时边为 0；Base 内部也没有 C++ 语义边，或仅有纯 C 接口。

| 包 | 仓库用途摘要 | 镜像与工具差异 |
|---|---|---|
| `binutils` | GNU Binutils | 否：六个精确镜像清单无该 VCS；是：含 readelf/objdump 等二进制检查与诊断工具；`binutils` 的仓库摘要将其用途定义为“GNU Binutils”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。仓元数据还将它标识为测试、诊断或构建工具，按已测拓扑独立运行。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `binutils-aarch64` | GNU Binutils | 否：六个精确镜像清单无该 VCS；是：交叉架构二进制检查与诊断工具；`binutils-aarch64` 的仓库摘要将其用途定义为“GNU Binutils”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。仓元数据还将它标识为测试、诊断或构建工具，按已测拓扑独立运行。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `binutils-armv7hl` | GNU Binutils | 否：六个精确镜像清单无该 VCS；是：交叉架构二进制检查与诊断工具；`binutils-armv7hl` 的仓库摘要将其用途定义为“GNU Binutils”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。仓元数据还将它标识为测试、诊断或构建工具，按已测拓扑独立运行。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `binutils-armv7l` | GNU Binutils | 否：六个精确镜像清单无该 VCS；是：交叉架构二进制检查与诊断工具；`binutils-armv7l` 的仓库摘要将其用途定义为“GNU Binutils”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。仓元数据还将它标识为测试、诊断或构建工具，按已测拓扑独立运行。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `ccache` | A Fast C/C++ Compiler Cache | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`ccache` 的仓库摘要将其用途定义为“A Fast C/C++ Compiler Cache”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `cmake` | Cross-platform make system | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`cmake` 的仓库摘要将其用途定义为“Cross-platform make system”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部唯一相关调用只共享 C 接口符号，不传递 C++ 对象。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `doxygen` | Automated C, C++, and Java Documentation Generator | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`doxygen` 的仓库摘要将其用途定义为“Automated C, C++, and Java Documentation Generator”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `elfutils` | Higher-level library to access ELF | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；是：ELF 检查与诊断工具集；`elfutils` 的仓库摘要将其用途定义为“Higher-level library to access ELF”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。仓元数据还将它标识为测试、诊断或构建工具，按已测拓扑独立运行。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `gdb` | A GNU source-level debugger for C, C++, Java and other languages | 否：六个精确镜像清单无该 VCS；是：调试器；`gdb` 的仓库摘要将其用途定义为“A GNU source-level debugger for C, C++, Java and other languages”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部唯一相关调用只共享 C 接口符号，不传递 C++ 对象。它的源码版本不在已核对的六份标准设备镜像清单中。仓元数据还将它标识为测试、诊断或构建工具，按已测拓扑独立运行。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `gettext` | GNU libraries and utilities for producing multi-lingual messages | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`gettext` 的仓库摘要将其用途定义为“GNU libraries and utilities for producing multi-lingual messages”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `gn` | A meta-build system that generates build files for Ninja | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`gn` 的仓库摘要将其用途定义为“A meta-build system that generates build files for Ninja”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `groff` | A document formatting system | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`groff` 的仓库摘要将其用途定义为“A document formatting system”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `ninja` | Ninja build system | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`ninja` 的仓库摘要将其用途定义为“Ninja build system”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `openfst` | Weighted finite-state transducer library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`openfst` 的仓库摘要将其用途定义为“Weighted finite-state transducer library”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `patchelf` | A utility for patching ELF binaries | 是：tizen-headed-aarch64;tizen-headed-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`patchelf` 的仓库摘要将其用途定义为“A utility for patching ELF binaries”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `pbzip2` | Parallel implementation of bzip2 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`pbzip2` 的仓库摘要将其用途定义为“Parallel implementation of bzip2”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |
| `tinyxml2` | A simple, small, C++ XML parser | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`tinyxml2` 的仓库摘要将其用途定义为“A simple, small, C++ XML parser”。它产出的 ELF 直接依赖 libstdc++，但 armv7l 的完整精确运行时图测得：通向 Unified 迁移集合的运行时边为 0；Base 内部跨包 C++ 语义边也为 0。它的源码版本不在已核对的六份标准设备镜像清单中。因此它虽然自身使用 libstdc++，却不构成已识别的混合标准库 C++ 对象边界；切换它不能关闭额外风险边，判定无需改。 |

## 跨包边界只有 C 接口（4 包）

产物自身可以用 C++ 实现并链接 libstdc++，但实际 consumer/provider 符号交集只有 C 接口；C++ 对象布局、异常和析构规则不跨边界。

| 包 | 仓库用途摘要 | 镜像与工具差异 |
|---|---|---|
| `db4` | Berkeley DB Database Library Version 4.8 | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`db4` 的仓库摘要将其用途定义为“Berkeley DB Database Library Version 4.8”。它的部分 ELF 自身直接依赖 libstdc++，但 armv7l 的实际依赖和符号交集显示，Unified 侧调用方：`man-db`；跨包共享的都是 C 接口符号，其余被分析边没有 C++ 符号交集。Base 内部相关调用同样只共享 C 接口。它的源码版本不在已核对的六份标准设备镜像清单中。C 接口只传递 C 类型，不依赖任一 C++ 标准库的对象布局、异常或析构规则，因此两侧标准库不同不会改变这条已测边界，判定无需改。 |
| `ncurses` | Terminal control library | 是：tizen-boot-aarch64-rpi;tizen-boot-armv7l-rpi;tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`ncurses` 的仓库摘要将其用途定义为“Terminal control library”。它的部分 ELF 自身直接依赖 libstdc++，但 armv7l 的实际依赖和符号交集显示，Unified 侧调用方：`aspell`、`parted`、`pass`、`psmisc`；跨包共享的都是 C 接口符号，其余被分析边没有 C++ 符号交集。Base 内部相关调用同样只共享 C 接口。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。C 接口只传递 C 类型，不依赖任一 C++ 标准库的对象布局、异常或析构规则，因此两侧标准库不同不会改变这条已测边界，判定无需改。 |
| `nghttp2` | This is an experimental implementation of Hypertext Transfer Protocol version 2. | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`nghttp2` 的仓库摘要将其用途定义为“This is an experimental implementation of Hypertext Transfer Protocol version 2.”。它的部分 ELF 自身直接依赖 libstdc++，但 armv7l 的实际依赖和符号交集显示，Unified 侧调用方：`lwnode`；跨包共享的都是 C 接口符号，其余被分析边没有 C++ 符号交集。Base 内部相关调用同样只共享 C 接口。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。C 接口只传递 C 类型，不依赖任一 C++ 标准库的对象布局、异常或析构规则，因此两侧标准库不同不会改变这条已测边界，判定无需改。 |
| `tiff` | Tools for Converting from and to the Tiff Format | 是：tizen-headed-aarch64;tizen-headed-armv7l;tizen-headless-aarch64;tizen-headless-armv7l；否：仓库元数据的主要用途不是测试/诊断/基准；`tiff` 的仓库摘要将其用途定义为“Tools for Converting from and to the Tiff Format”。它的部分 ELF 自身直接依赖 libstdc++，但 armv7l 的实际依赖和符号交集显示，Unified 侧调用方：`efl`、`lcms2`、`opencv`；跨包共享的都是 C 接口符号，其余被分析边没有 C++ 符号交集。它的源码版本精确匹配至少一份已核对的标准设备镜像清单。C 接口只传递 C 类型，不依赖任一 C++ 标准库的对象布局、异常或析构规则，因此两侧标准库不同不会改变这条已测边界，判定无需改。 |

## 含 C++ 静态归档但无消费边（2 包）

静态归档确有 C++ 符号，但精确拓扑中的 Unified 与 Base 内部运行时消费边均为 0，当前没有被迁移侧吸收到同一进程。

| 包 | 仓库用途摘要 | 镜像与工具差异 |
|---|---|---|
| `golang` | The Go Programming Language | 否：六个精确镜像清单无该 VCS；是：产出 golang-tests 测试子包；`golang` 的仓库摘要将其用途定义为“The Go Programming Language”。包中确有 2 个架构份次的静态归档含可解码全局 C++ 符号，但 armv7l 精确依赖图中，Unified 与 Base 内部对这些产物的运行时消费边均为 0。它的源码版本不在已核对的六份标准设备镜像清单中。在当前已测拓扑里，这些 C++ 代码没有被迁移侧吸收到同一进程，也没有跨包传递 C++ 语义；切换该包不能关闭额外边界，判定无需改。 |
| `icecream` | For Distributed Compile in the Network | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`icecream` 的仓库摘要将其用途定义为“For Distributed Compile in the Network”。包中确有 3 个架构份次的静态归档含可解码全局 C++ 符号，但 armv7l 精确依赖图中，Unified 与 Base 内部对这些产物的运行时消费边均为 0。它的源码版本不在已核对的六份标准设备镜像清单中。在当前已测拓扑里，这些 C++ 代码没有被迁移侧吸收到同一进程，也没有跨包传递 C++ 语义；切换该包不能关闭额外边界，判定无需改。 |

## 头文件由使用方编译（2 包）

C++ 载体是头文件而非本包固定实现的运行时；使用方会用自己的编译器和标准库实例化。

| 包 | 仓库用途摘要 | 镜像与工具差异 |
|---|---|---|
| `bison` | The GNU Parser Generator | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`bison` 是把语法规则生成解析器源码的构建工具（仓库摘要：“The GNU Parser Generator”）。它作为构建工具把输入语法生成源码，本身不提供 C++ 运行库。C++ 命中来自 3 个架构份次的头文件载体；已发布包的直接 C++ 运行时依赖为“NO”，含全局 C++ 符号的静态归档数量为 0，armv7l 运行时图中的 Unified/Base C++ 边也都是 0。它的源码版本不在已核对的六份标准设备镜像清单中。这些头文件最终由使用方按其自身工具链实例化，会自然继承使用方选择的 libc++，无需给本包单独增加标准库切换。 |
| `cereal` | A header-only C++11 serialization library | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`cereal` 是由使用方直接包含和实例化的 C++ 序列化头文件库（仓库摘要：“A header-only C++11 serialization library”）。C++ 命中来自 3 个架构份次的头文件载体；已发布包的直接 C++ 运行时依赖为“NO”，含全局 C++ 符号的静态归档数量为 0，armv7l 运行时图中的 Unified/Base C++ 边也都是 0。它的源码版本不在已核对的六份标准设备镜像清单中。这些头文件最终由使用方按其自身工具链实例化，会自然继承使用方选择的 libc++，无需给本包单独增加标准库切换。 |

## 已是 libc++ 运行时（1 包）

该包就是 libc++/libc++abi 提供方，不是仍消费 libstdc++、需要切换的对象。

| 包 | 仓库用途摘要 | 镜像与工具差异 |
|---|---|---|
| `libcxx-runtimes` | LLVM libc++ and libc++abi runtimes | 否：六个精确镜像清单无该 VCS；否：仓库元数据的主要用途不是测试/诊断/基准；`libcxx-runtimes` 是提供 libc++ 与 libc++abi 本身的运行时和开发文件（仓库摘要：“LLVM libc++ and libc++abi runtimes”）。它构建和安装的正是目标 libc++/libc++abi，实现本身不直接链接 libstdc++。本轮逐包切换针对的是仍消费 libstdc++ 的其他 C++ 包；对运行时提供方再添加消费者侧的 `-stdlib=libc++` 不构成迁移，因此判定无需改。 |
