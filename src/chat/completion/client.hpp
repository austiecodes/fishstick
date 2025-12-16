#pragma once

#include "types.hpp"
#include <string>
#include <memory>

namespace chat::completion {

constexpr std::string_view CHAT_COMPLETIONS_PATH = "/chat/completions";
constexpr std::string_view MODEL_PATH = "/models";
constexpr std::string_view DEFAULT_MODEL = "gpt-5-nano";


// ChatCompletionClient is similar to a Go struct with methods.
// Think of it as:
//   type ChatCompletionClient struct { ... }
//   func (c *Client) Chat(...) (Response, error)
class ChatCompletionClient {
public:
    // Constructor - similar to Go's NewClient() factory function
    explicit ChatCompletionClient(std::string api_key, std::string base_url = "https://api.openai.com/v1");
    
    // Destructor - C++ needs explicit cleanup (Go has GC)
    ~ChatCompletionClient() = default;
    
    // Delete copy operations (similar to Go's non-copyable types)
    ChatCompletionClient(const ChatCompletionClient&) = delete;
    ChatCompletionClient& operator=(const ChatCompletionClient&) = delete;
    
    // Allow move operations (similar to Go's ownership transfer)
    ChatCompletionClient(ChatCompletionClient&&) = default;
    ChatCompletionClient& operator=(ChatCompletionClient&&) = default;
    
    // Chat method - the main API
    // In Go, this would return (ChatCompletionResponse, error)
    // In C++, we use std::optional to represent potential failure
    // (or you could use exceptions for error handling)
    std::optional<ChatCompletionResponse> chat(const ChatCompletionRequest& request);

private:
    std::string api_key_;
    std::string base_url_;
    
    // Helper methods (private implementation details)
    std::string build_request_body(const ChatCompletionRequest& request);
    std::optional<ChatCompletionResponse> parse_response(const std::string& response_body);
};

}  // namespace chat::completion
