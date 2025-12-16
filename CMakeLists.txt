# -----------------------------------------------------------------------------
# 1. CMake 版本要求
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)

# -----------------------------------------------------------------------------
# 2. 项目信息
# -----------------------------------------------------------------------------
project(MyChatCompletionProject CXX)

# -----------------------------------------------------------------------------
# 3. 编译器和标准设置
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# 4. 启用 compile_commands.json 生成 (用于 Clangd / Cpptools)
# -----------------------------------------------------------------------------
# 【非常重要】这行指令告诉 CMake 生成编译数据库文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# 5. 第三方库配置（统一管理在项目外部）
# -----------------------------------------------------------------------------
# 在 Go 中，你会用 go mod 管理依赖
# 在 C++ 中，我们使用统一的 third_party 目录（项目外部）
# 
# 运行 scripts/setup_third_party.sh 来克隆依赖库
# 
# 第三方库位置：~/codes/cpp/third_party/
#   - json:        nlohmann/json (JSON 序列化)
#   - cpp-httplib: cpp-httplib (HTTP 客户端)
#   - CLI11:       CLI11 (命令行参数解析，支持子命令)

# 定义统一的第三方库根目录
# 可以通过环境变量覆盖，例如：cmake -DTHIRD_PARTY_ROOT=/path/to/third_party
if(NOT DEFINED THIRD_PARTY_ROOT)
    set(THIRD_PARTY_ROOT "$ENV{HOME}/codes/cpp/third_party")
endif()

# 检查第三方库是否存在
if(NOT EXISTS "${THIRD_PARTY_ROOT}/json/include/nlohmann/json.hpp")
    message(WARNING "nlohmann/json not found at ${THIRD_PARTY_ROOT}/json")
    message(WARNING "Please run: ./scripts/setup_third_party.sh")
endif()

if(NOT EXISTS "${THIRD_PARTY_ROOT}/cpp-httplib/httplib.h")
    message(WARNING "cpp-httplib not found at ${THIRD_PARTY_ROOT}/cpp-httplib")
    message(WARNING "Please run: ./scripts/setup_third_party.sh")
endif()

if(NOT EXISTS "${THIRD_PARTY_ROOT}/CLI11/include/CLI/CLI.hpp")
    message(WARNING "CLI11 not found at ${THIRD_PARTY_ROOT}/CLI11")
    message(WARNING "Please run: ./scripts/setup_third_party.sh")
endif()

# -----------------------------------------------------------------------------
# 6. 定义头文件搜索路径
# -----------------------------------------------------------------------------
include_directories(src)
# nlohmann/json 的头文件在 include/ 目录下
include_directories(${THIRD_PARTY_ROOT}/json/include)
# cpp-httplib 的头文件直接在根目录
include_directories(${THIRD_PARTY_ROOT}/cpp-httplib)
# CLI11 的头文件在 include/ 目录下
include_directories(${THIRD_PARTY_ROOT}/CLI11/include)

# -----------------------------------------------------------------------------
# 6. 收集源文件
# -----------------------------------------------------------------------------
set(PROJECT_SOURCES
    src/main.cpp
    src/chat/completion/client.cpp
)

# -----------------------------------------------------------------------------
# 7. 构建可执行文件
# -----------------------------------------------------------------------------
add_executable(my_app ${PROJECT_SOURCES})

# -----------------------------------------------------------------------------
# 8. CLI11（命令行参数解析，header-only）
# -----------------------------------------------------------------------------
# CLI11 是 header-only 库，不需要链接，只需 include_directories 即可