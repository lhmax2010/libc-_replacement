# libstdc++ 与 libc++ 之间的接口兼容性

- 日期：2026-09-09
- 适用场景：平台迁移到 libc++ 之后，与仍使用 libstdc++ 的组件（第三方应用、
  预编译库）之间的接口

---

## 一、结论概要

**兼容性不取决于"用哪套标准库"，取决于"接口跨边界传什么"。**

| 接口形态 | 能否支持 | 依据 |
|---|---|---|
| 纯 C 接口 | **可以** | 已实测 |
| C++ 接口，只传内置类型与 POD | **可以** | 已实测 |
| C++ 接口，传标准库对象 | **不可以** | 已实测，静默出错 |
| C++ 接口，跨边界抛接异常 | **不可以** | 已实测，类型信息丢失 |
| C++ 接口，跨边界传递所有权 | **不可以** | 分配器不同 |

下面逐项用代码说明。

---

## 二、可以支持的接口

### 2.1 纯 C 接口

```c
// 头文件
#ifdef __cplusplus
extern "C" {
#endif

typedef struct engine_s* engine_handle;

engine_handle engine_create(void);
int           engine_get_width(engine_handle h);
void          engine_set_position(engine_handle h, int x, int y);
int           engine_read(engine_handle h, char* buf, size_t buflen);
void          engine_destroy(engine_handle h);

#ifdef __cplusplus
}
#endif
```

**为什么安全**：跨边界的只有 `int`、`char*`、`size_t` 和一个不透明指针。
这些的布局由平台 C ABI 规定，与标准库无关。

**已实测**：真实的 ICU 库（libstdc++ 构建）通过其 C 接口被 libc++ 构建的
主程序调用，双方行为完全正常。

### 2.2 C++ 接口，但只传内置类型与 POD

```cpp
// 这些跨边界是安全的
class Engine {
public:
    int    width() const;                    // 返回 int
    void   setPosition(int x, int y);        // 参数是 int
    bool   isReady() const;                  // 返回 bool
    double scale() const;                    // 返回 double
};

struct Rect { int x, y, w, h; };             // POD，布局由 C ABI 定
Rect   getBounds();                          // 按值返回 POD，安全
```

**为什么安全**：虽然这是 C++ 类，但跨边界传递的数据全是内置类型或 POD。
这类类型的布局两套库一致（由平台 ABI 而非标准库决定）。

**注意**：类本身的存在不是问题，**跨边界的数据形态才是**。

### 2.3 两条必须遵守的附加条件

即使接口形态正确，下面两条不满足仍会出问题。

**条件一：异常不能逸出边界**

```cpp
// 不安全 —— 参数和返回值都是 int，但异常会穿过去
int engine_compute(engine_handle h) {
    auto result = internal_cpp_function();   // 这里可能抛
    return result;
}

// 安全 —— 挡住异常，转成错误码
int engine_compute(engine_handle h, int* out) {
    try {
        *out = internal_cpp_function();
        return 0;
    } catch (const std::exception&) {
        return -1;
    } catch (...) {
        return -2;
    }
}
```

**为什么**：异常跨越两套标准库的边界时，接收方无法按类型捕获
（类型信息在两套 RTTI 中是独立的），只能落到 `catch (...)`；
若接收方的调用链上有标注为不抛异常的函数，**进程直接终止**。

**条件二：分配与释放必须在同一侧**

```cpp
// 不安全 —— 一侧分配，另一侧释放
char* engine_get_name(engine_handle h);   // 内部 new[] 出来的
// 应用侧：free(name);  ← 用的是另一套 operator delete

// 安全 —— 提供配对的释放函数
char* engine_get_name(engine_handle h);
void  engine_free_string(char* s);        // 由提供方释放

// 或者由调用方提供缓冲区
int   engine_get_name(engine_handle h, char* buf, size_t buflen);
```

**为什么**：两套标准库各有自己的 `operator new/delete` 实现与内部状态，
跨边界释放会破坏堆结构。

---

## 三、不能支持的接口

### 3.1 传递标准库对象

```cpp
// 不能跨边界
std::string  engine_get_name();                    // 返回 string
void         engine_set_items(std::vector<int>);   // 参数是 vector
std::map<int, std::string> engine_get_config();    // 返回 map
```

**为什么**：同一个类型在两套库中的内存布局不同。我们比对了 33 个常用类型，
**其中 10 个布局不同**：

| 类型 | libstdc++ | libc++ |
|---|---:|---:|
| `std::string` | 32 字节 | 24 字节 |
| `std::deque<int>` | 80 字节 | 48 字节 |
| `std::map<int,int>` | 48 字节 | 24 字节 |
| `std::set<int>` | 48 字节 | 24 字节 |
| `std::unordered_map<int,int>` | 56 字节 | 40 字节 |
| `std::unordered_set<int>` | 56 字节 | 40 字节 |
| `std::function<void()>` | 32 字节 | 48 字节 |
| `std::any` | 16 字节 | 32 字节 |
| `std::future<int>` | 16 字节 | 8 字节 |
| `std::promise<int>` | 24 字节 | 8 字节 |

**而且编译与链接都不会报错。** 已构造可复现实例：

```cpp
// 提供方（libstdc++ 构建）
std::deque<int>* make_data() {
    auto* d = new std::deque<int>{1, 2, 3, 4};
    return d;                        // 报告长度 4
}

// 消费方（libc++ 构建）
auto* d = make_data();
printf("%zu\n", d->size());          // 读到 106884723786536
```

**结果**：链接成功、加载成功、**进程正常退出，不崩溃**，但数据是错的。

**为什么链接器发现不了**：C++ 的名字修饰**不把返回类型编进符号名**。
两侧的 `make_data` 都修饰成 `_Z9make_datav`，符号完全匹配。

### 3.2 跨边界抛接异常

```cpp
// 不能跨边界
class EngineError : public std::runtime_error { ... };

void engine_process();               // 内部抛 EngineError

// 应用侧
try {
    engine_process();
} catch (const EngineError& e) {     // 接不住 —— 类型信息对不上
    ...
} catch (...) {                      // 只能落到这里
    ...
}
```

**已实测**：跨边界的类型化异常无法按类型捕获，只能被 `catch (...)` 接住。

### 3.3 传递所有权

```cpp
// 不能跨边界
std::unique_ptr<Widget> engine_create_widget();     // 谁来释放？
std::shared_ptr<Config> engine_get_config();        // 控制块在哪一侧？
```

**为什么**：智能指针的释放要调用构造它的那一侧的 `operator delete`；
`shared_ptr` 的引用计数控制块也在原侧，跨边界后两侧各自计数。

### 3.4 传递迭代器

```cpp
// 不能跨边界
std::vector<int>::iterator engine_begin();
```

**为什么**：迭代器指向容器的内部结构，而两侧对那个结构的理解不同。

---

## 四、为什么不做转换层

一种设想是：在边界上把 libstdc++ 的对象转成 libc++ 的。技术上可以做，
但结果会退化成 C 接口，且留下无法覆盖的缺口。

**转换函数放在哪一侧？**

一个编译单元只能链接一套标准库，因此转换只能两侧各做一半，
中间用不含 C++ 类型的形式传递：

```
libstdc++ 侧:  std::string  →  { const char* data; size_t len; }
                                        ↓ 跨边界（纯 C）
libc++ 侧:     { const char* data; size_t len; }  →  std::string
```

**中间那层就是 C 接口。**

**而且有三类包不进去**：

- **异常** —— 异常在飞的时候没有转换的时机；
- **迭代器** —— 指向内部结构，转过去就是野指针；
- **所有权** —— 转换后谁负责释放仍然没有答案。

**结论**：转换层的极限形态就是 C 接口，但多了每次调用的复制开销、
每个类型一对转换函数的维护成本，而且掩盖了问题——用起来像 C++，
实际性能特征完全不同。

**业界参照**：Chromium 的嵌入框架 CEF 同时提供 C++ 与 C 两套 API，
**做 C API 的理由正是 C++ ABI 跨编译器不兼容**。ICU 也是同样的设计。

---

## 五、判断一个接口能否支持的方法

**判据：这个接口用纯 C 能不能表达完整？**

| 能 | 不能 |
|---|---|
| 内置类型、POD、不透明指针 | 类、模板、标准库容器 |
| 错误码 | 异常 |
| 调用方提供缓冲区 | 返回动态分配的对象 |
| C 函数指针回调 | `std::function` 回调 |

**`extern "C"` 不是判据** —— 可以给一个返回 `std::string` 的函数加
`extern "C"`，符号名是干净了，但对象照样错。

---

## 六、需要业务部门提供的信息

要逐项评估第三方应用的兼容性，我们需要：

1. **第三方应用与平台之间的接口清单** —— 哪些是 C 接口、哪些传 C++ 类型；
2. **应用是否自带标准库**，还是依赖平台提供；
3. 若有传 C++ 类型的接口，**优先级排序** —— 我们据此评估改成 C 接口的
   工作量。

**平台侧我们会做的**：核查现有 C 接口的实现是否满足第二节的两条附加条件
（异常是否挡住、分配释放是否同侧）——这一条只有我们能查。

---

## 七、本文结论的证据范围

- 布局比对：33 个类型，x86_64；
- 跨边界实测：真实组件（ICU、Boost），x86_64 与 armv7l；
- 异常跨边界：已实测，两种架构；
- **未覆盖**：aarch64 的运行时验证；第三方应用的实际接口（材料不可得）。
