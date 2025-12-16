#include <iostream>
#include <string>
#include <CLI/CLI.hpp>

int test_chat_completion() {
    return 0;
}

int main(int argc, char* argv[]) {
    // 创建主程序（类似 Go 的 rootCmd）
    CLI::App app{"fstk - CLI tool for chat completion"};
    app.require_subcommand(1); // 至少需要一个子命令

    // 全局选项（可选）
    bool verbose = false;
    app.add_flag("-v,--verbose", verbose, "Print extra debug info");

    // 子命令 1: set
    // 类似 Go cobra 的 app.AddCommand(&setCmd)
    auto* set_cmd = app.add_subcommand("set", "Set configuration values");
    std::string chat_model;
    set_cmd->add_option("--chatModel", chat_model, "Chat model to use (e.g., gpt-4o)")
        ->required(); // 该参数必须提供

    // 子命令 2: get（示例）
    auto* get_cmd = app.add_subcommand("get", "Get configuration values");
    std::string config_key;
    get_cmd->add_option("key", config_key, "Configuration key to retrieve");

    // 解析命令行参数
    // 在 Go 中是 rootCmd.Execute()
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // 根据子命令执行不同逻辑
    if (set_cmd->parsed()) {
        if (verbose) {
            std::cout << "[verbose] Setting chatModel\n";
        }
        std::cout << "Configuration updated:\n";
        std::cout << "  chatModel = " << chat_model << '\n';
    } else if (get_cmd->parsed()) {
        std::cout << "Getting config for key: " << config_key << '\n';
        // 实际实现：读取配置文件
    }

    return test_chat_completion();
}
