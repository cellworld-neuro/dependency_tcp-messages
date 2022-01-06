#pragma once
#include <easy_tcp.h>
#include <json_cpp.h>
#include <tcp_messages/message.h>
#include <regex>

#define Add_route(HEADER, DESTINATION, ...) {                             \
    if (std::regex_match (message.header,std::regex(HEADER))) {           \
        routed = true;                                                    \
        try {                                                             \
                DESTINATION(__VA_OPT__(message.get_body<__VA_ARGS__>())); \
        } catch (...) {                                                   \
            failed_route(message);                                        \
        }                                                                 \
    }                                                                     \
}

namespace tcp_messages {
    template<class T>
    Message get_response(const std::string &request_header, T response) {
        if constexpr (std::is_same_v<T, Message>) {
            return response;
        }
        Message response_message;
        response_message.header = request_header + "_response";
        if constexpr (std::is_base_of<json_cpp::Json_base, T>::value) {
            response_message.body = response.to_json();
        } else if constexpr (std::is_same_v<char *, T> || std::is_same_v<const char *, T>) {
            response_message.body = response;
        } else if constexpr (std::is_same_v<bool, T> || std::is_same_v<const bool, T>) {
            if (response) {
                response_message.body = "success";
            } else {
                response_message.body = "fail";
            }
        } else {
            response_message.body = std::to_string(response);
        }
        return response_message;
    }
}

#define Add_route_with_response(HEADER, DESTINATION, ...) {               \
    if (std::regex_match (message.header,std::regex(HEADER))) {           \
        routed = true;                                                    \
        try {                                                             \
                auto response = DESTINATION(__VA_OPT__(message.get_body<__VA_ARGS__>())); \
                send_message(tcp_messages::get_response(message.header, response));         \
        } catch (...) {                                                   \
            failed_route(message);                                        \
        }                                                                 \
    }                                                                     \
}


#define Routes(ADD_ROUTES) bool route(const tcp_messages::Message &message) override { bool routed = false; ADD_ROUTES; return routed; }
