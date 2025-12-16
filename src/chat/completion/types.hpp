#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <vector>


namespace chat::completion {

using Role = std::string;
namespace Roles {
    inline constexpr std::string_view System = "system";
    inline constexpr std::string_view Developer = "developer";
    inline constexpr std::string_view User = "user";
    inline constexpr std::string_view Assistant = "assistant";
}

struct Message {
    Role role;
    std::string content;
};

struct ChatCompletionRequest {
    std::string model;
    std::vector<Message> messages;
    bool stream;
    std::optional<float> temperature;
    std::optional<float> top_p;
    std::optional<float> n;
    std::optional<int> max_completion_tokens;
};

struct ChatCompletionResponse {
    std::vector<Message> messages;
    std::optional<float> completion_tokens;
    std::optional<float> prompt_tokens;
    std::optional<float> total_tokens;
};

}  // namespace chat::completion
