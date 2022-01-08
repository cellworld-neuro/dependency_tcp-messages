#pragma once
#include <easy_tcp.h>
#include <json_cpp.h>
#include <tcp_messages/message.h>
#include <regex>

#define Add_route(HEADER, DESTINATION, ...) {                             \
    if (std::regex_match (message.header,std::regex(HEADER))) {           \
        routed = true;                                                    \
        try {                                                             \
                __VA_OPT__(auto request = message.get_body<__VA_ARGS__>();)\
                DESTINATION(__VA_OPT__(request));                         \
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
        response_message.header = get_response_header(request_header);
        if constexpr (std::is_same_v<bool, T> || std::is_same_v<const bool, T>) {
            if (response) {
                response_message.body = "success";
            } else {
                response_message.body = "fail";
            }
        } else {
            response_message.set_body(response);
        }
        return response_message;
    }
}

#define Add_route_with_response(HEADER, DESTINATION, ...) {               \
    if (std::regex_match (message.header,std::regex(HEADER))) {           \
        routed = true;                                                    \
        try {                                                             \
                __VA_OPT__(auto request = message.get_body<__VA_ARGS__>();)\
                auto response = DESTINATION(__VA_OPT__(request)); \
                send_message(tcp_messages::get_response(message.header, response));         \
        } catch (...) {                                                   \
            failed_route(message);                                        \
        }                                                                 \
    }                                                                     \
}


#define Routes(ADD_ROUTES) bool route(const tcp_messages::Message &message) override { bool routed = false; ADD_ROUTES; return routed; }
