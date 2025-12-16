#!/bin/bash
# 设置统一的第三方库目录
# 在 Go 中，你会用 go mod 管理依赖
# 在 C++ 中，我们用 git clone 到统一的 third_party 目录

set -e

# 统一的第三方库目录（项目外部）
THIRD_PARTY_ROOT="${HOME}/codes/cpp/third_party"

echo "Setting up third-party libraries in: $THIRD_PARTY_ROOT"
echo ""

# 创建目录
mkdir -p "$THIRD_PARTY_ROOT"

# 1. nlohmann/json - JSON 序列化库
# Go 对比：encoding/json 包
if [ ! -d "$THIRD_PARTY_ROOT/json" ]; then
    echo "Cloning nlohmann/json..."
    git clone --depth 1 --branch v3.12.0 https://github.com/nlohmann/json.git "$THIRD_PARTY_ROOT/json"
    echo "✓ nlohmann/json cloned"
else
    echo "✓ nlohmann/json already exists, skipping..."
    cd "$THIRD_PARTY_ROOT/json"
    git pull || echo "  (Note: Already up to date or on specific tag)"
fi

# 2. cpp-httplib - HTTP 客户端库
# Go 对比：net/http 包
if [ ! -d "$THIRD_PARTY_ROOT/cpp-httplib" ]; then
    echo "Cloning cpp-httplib..."
    git clone --depth 1 --branch v0.28.0 https://github.com/yhirose/cpp-httplib.git "$THIRD_PARTY_ROOT/cpp-httplib"
    echo "✓ cpp-httplib cloned"
else
    echo "✓ cpp-httplib already exists, skipping..."
    cd "$THIRD_PARTY_ROOT/cpp-httplib"
    git pull || echo "  (Note: Already up to date or on specific tag)"
fi

echo ""
echo "Third-party libraries setup complete!"
echo ""
echo "Libraries location:"
echo "  - nlohmann/json:    $THIRD_PARTY_ROOT/json"
echo "  - cpp-httplib:      $THIRD_PARTY_ROOT/cpp-httplib"
echo ""
echo "Usage in CMakeLists.txt:"
echo "  set(THIRD_PARTY_ROOT \"$THIRD_PARTY_ROOT\")"
echo "  include_directories(\${THIRD_PARTY_ROOT}/json/include)"
echo "  include_directories(\${THIRD_PARTY_ROOT}/cpp-httplib)"
