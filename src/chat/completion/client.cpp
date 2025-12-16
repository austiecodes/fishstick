#include "client.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

// JSON library (nlohmann/json) - single header file
// In Go, you'd use encoding/json package
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// HTTP library (cpp-httplib) - single header file
// In Go, you'd use net/http package
// Note: For HTTPS support, you need OpenSSL. For now, we'll use HTTP or
// you can install OpenSSL and uncomment the line below:
// #define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

namespace chat::completion {

// Constructor implementation
ChatCompletionClient::ChatCompletionClient(std::string api_key, std::string base_url)
    : api_key_(std::move(api_key))  // std::move is similar to Go's ownership transfer
    , base_url_(std::move(base_url)) {
    // Initialization done in member initializer list (C++ best practice)
}

// Chat method implementation
std::optional<ChatCompletionResponse> ChatCompletionClient::chat(const ChatCompletionRequest& request) {
    try {
        // 1. Build JSON request body
        std::string json_body = build_request_body(request);
        
        // 2. Parse base URL (extract host and path)
        // In Go: url.Parse(baseURL)
        std::string protocol, host, path;
        size_t protocol_end = base_url_.find("://");
        if (protocol_end == std::string::npos) {
            std::cerr << "Invalid base URL: " << base_url_ << std::endl;
            return std::nullopt;
        }
        
        protocol = base_url_.substr(0, protocol_end);
        size_t host_start = protocol_end + 3;
        size_t path_start = base_url_.find('/', host_start);
        
        if (path_start == std::string::npos) {
            host = base_url_.substr(host_start);
            path = "/chat/completions";
        } else {
            host = base_url_.substr(host_start, path_start - host_start);
            std::string base_path = base_url_.substr(path_start);
            // Ensure path ends with /chat/completions
            if (base_path.back() == '/') {
                path = base_path + "chat/completions";
            } else {
                path = base_path + "/chat/completions";
            }
        }
        
        // For HTTPS, we need OpenSSL. For now, use HTTP if protocol is https
        // In production, you should enable OpenSSL support
        if (protocol == "https") {
            std::cerr << "Warning: HTTPS requires OpenSSL. Please enable CPPHTTPLIB_OPENSSL_SUPPORT" << std::endl;
            // For now, we'll try anyway (may fail without OpenSSL)
        }
        
        // 3. Make HTTP POST request
        // In Go: http.Post(url, "application/json", body)
        httplib::Client client(host);
        client.set_read_timeout(30);  // 30 seconds timeout
        client.set_write_timeout(30);
        
        // Set headers
        httplib::Headers headers = {
            {"Authorization", "Bearer " + api_key_},
            {"Content-Type", "application/json"}
        };
        
        // Make POST request
        auto response = client.Post(path.c_str(), headers, json_body, "application/json");
        
        if (!response) {
            std::cerr << "HTTP request failed" << std::endl;
            return std::nullopt;
        }
        
        if (response->status != 200) {
            std::cerr << "HTTP error: " << response->status << std::endl;
            std::cerr << "Response: " << response->body << std::endl;
            return std::nullopt;
        }
        
        // 4. Parse JSON response
        return parse_response(response->body);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in chat: " << e.what() << std::endl;
        return std::nullopt;
    }
}

// Helper method to build JSON request body
// In Go, you'd use json.Marshal(request)
std::string ChatCompletionClient::build_request_body(const ChatCompletionRequest& request) {
    json j;
    
    // Required fields
    j["model"] = request.model;
    j["stream"] = request.stream;
    
    // Messages array
    json messages_array = json::array();
    for (const auto& msg : request.messages) {
        json msg_obj;
        msg_obj["role"] = msg.role;
        msg_obj["content"] = msg.content;
        messages_array.push_back(msg_obj);
    }
    j["messages"] = messages_array;
    
    // Optional fields (only include if they have values)
    if (request.temperature.has_value()) {
        j["temperature"] = request.temperature.value();
    }
    if (request.top_p.has_value()) {
        j["top_p"] = request.top_p.value();
    }
    if (request.n.has_value()) {
        j["n"] = request.n.value();
    }
    if (request.max_completion_tokens.has_value()) {
        j["max_completion_tokens"] = request.max_completion_tokens.value();
    }
    
    // Convert to string
    // In Go: json.Marshal returns []byte, then string(bytes)
    return j.dump();
}

// Helper method to parse JSON response
// In Go, you'd use json.Unmarshal(responseBody, &response)
std::optional<ChatCompletionResponse> ChatCompletionClient::parse_response(const std::string& response_body) {
    try {
        json j = json::parse(response_body);
        
        ChatCompletionResponse response;
        
        // Parse messages (from choices array)
        if (j.contains("choices") && j["choices"].is_array()) {
            for (const auto& choice : j["choices"]) {
                if (choice.contains("message")) {
                    Message msg;
                    msg.role = choice["message"]["role"].get<std::string>();
                    msg.content = choice["message"]["content"].get<std::string>();
                    response.messages.push_back(msg);
                }
            }
        }
        
        // Parse token counts (from usage object)
        if (j.contains("usage")) {
            const auto& usage = j["usage"];
            if (usage.contains("completion_tokens")) {
                response.completion_tokens = usage["completion_tokens"].get<float>();
            }
            if (usage.contains("prompt_tokens")) {
                response.prompt_tokens = usage["prompt_tokens"].get<float>();
            }
            if (usage.contains("total_tokens")) {
                response.total_tokens = usage["total_tokens"].get<float>();
            }
        }
        
        return response;
        
    } catch (const json::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

}  // namespace chat::completion
