# 第三方库管理说明

## 统一管理方式

本项目使用统一的第三方库目录：`~/codes/cpp/third_party/`

这种方式类似于 Go 的 `go mod` 或 Python 的虚拟环境，所有 C++ 项目共享同一份第三方库。

## 设置依赖

运行以下脚本来自动克隆所有依赖：

```bash
./scripts/setup_third_party.sh
```

这个脚本会：
1. 在 `~/codes/cpp/third_party/` 创建目录
2. 使用 `git clone` 克隆以下库：
   - `nlohmann/json` (v3.11.3) - JSON 序列化库
   - `cpp-httplib` (v0.15.3) - HTTP 客户端库

## 库的位置

```
~/codes/cpp/third_party/
├── json/              # nlohmann/json
│   └── include/
│       └── nlohmann/
│           └── json.hpp
└── cpp-httplib/       # cpp-httplib
    └── httplib.h
```

## 在 CMakeLists.txt 中使用

CMakeLists.txt 已经配置好，会自动查找 `~/codes/cpp/third_party/` 目录。

## 每次新增/更换第三方库时的流程（你问的重点）

结论：**是的**，一般你需要做两件事：

- **在 `CMakeLists.txt` 里把头文件路径加进来**（例如 `include_directories(...)` 或更推荐的 `target_include_directories(my_app ...)`）
- **重新运行一次 CMake 配置**，让它把新的 include 路径写进 `build/compile_commands.json`，这样 clangd/VSCode 才能用“真实编译参数”做跳转/补全

### 为什么要重新跑 `cmake ..`？

因为 `build/compile_commands.json` 是 CMake 在“配置阶段”根据 `CMakeLists.txt` 生成的编译数据库：

- 你改了 include 路径 / 宏定义 / 编译选项
- 只有重新配置（`cmake ..`），CMake 才会重新生成 compile commands
- IDE 读取 `compile_commands.json` 后，才知道每个 `.cpp` 的 `-I...`、`-D...` 等参数，从而正确解析 `#include <>` 并做代码跳转

### 常用命令（推荐）

在项目根目录：

```bash
cmake -S . -B build
cmake --build build
```

如果你只是想更新 `compile_commands.json` 给 clangd 用，通常跑第一行就够了。

### 头文件库 vs 需要链接的库

- **header-only（纯头文件）库**（比如 `nlohmann/json`、`cpp-httplib` 默认用法）：
  - 通常只需要 `include_directories(...)` → 体现在 `compile_commands.json` 里的 `-I...`
  - 一般不需要额外 `target_link_libraries(...)`

- **需要编译/链接的库**（比如 `libcurl`、OpenSSL、sqlite、boost 的部分组件）：
  - 除了 include 路径，你还需要在 CMake 里加链接配置（`target_link_libraries(...)`、`find_package(...)` 等）
  - 这类库在最终链接步骤会出现 `-L...`、`-l...`（或 macOS 的 `-framework ...`）

如果需要自定义路径，可以在 CMake 时指定：

```bash
cmake -DTHIRD_PARTY_ROOT=/path/to/your/third_party ..
```

## 更新依赖

如果需要更新到最新版本，可以：

1. 手动进入目录更新：
```bash
cd ~/codes/cpp/third_party/json
git pull origin v3.11.3

cd ~/codes/cpp/third_party/cpp-httplib
git pull origin v0.15.3
```

2. 或者删除后重新运行 `setup_third_party.sh`

## 优势

-  所有 C++ 项目共享同一份库（节省空间）
-  使用 git 管理，可以轻松切换版本
-  不污染项目目录
-  类似 Go 的 `go mod` 管理方式
