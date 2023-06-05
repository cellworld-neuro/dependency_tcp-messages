#pragma once
#include <easy_tcp.h>
#include <json_cpp.h>
#include <tcp_messages/message.h>
#include <regex>

#define Add_route(HEADER, DESTINATION, ...) {                               \
    manifest.add_route(HEADER __VA_OPT__(,#__VA_ARGS__));                   \
    if (std::regex_match (message.header,std::regex(HEADER))) {             \
        routed = true;                                                      \
        try {                                                               \
                __VA_OPT__(auto request = message.get_body<__VA_ARGS__>();) \
                DESTINATION(__VA_OPT__(request));                           \
        } catch (...) {                                                     \
            failed_route(message);                                          \
        }                                                                   \
    }                                                                       \
}

namespace tcp_messages {
    template<class T>
    Message get_response(const Message &request, T response) {
        if constexpr (std::is_same_v<T, Message>) {
            response.id = request.id;
            return response;
        }
        Message response_message;
        response_message.header = get_response_header(request.header);
        response_message.id = request.id;
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

#define Add_route_with_response(HEADER, DESTINATION, ...) {                  \
    manifest.add_route(HEADER __VA_OPT__(,#__VA_ARGS__));                    \
    if (std::regex_match (message.header,std::regex(HEADER))) {              \
        routed = true;                                                       \
        try {                                                                \
                __VA_OPT__(auto request = message.get_body<__VA_ARGS__>();)  \
                auto response = DESTINATION(__VA_OPT__(request));            \
                send_message(tcp_messages::get_response(message, response)); \
        } catch (...) {                                                      \
            failed_route(message);                                           \
        }                                                                    \
    }                                                                        \
}

#define Allow_subscription() \
    Add_route_with_response("!subscribe", _subscribe);                                  \
    Add_route_with_response("!unsubscribe", _unsubscribe);                              \

#define Routes(ADD_ROUTES) bool route(const tcp_messages::Message &message) override { \
    tcp_messages::Manifest manifest;                                                   \
    bool routed = false;                                                               \
    ADD_ROUTES;                                                                        \
    Add_route_with_response("!ping", [](){return true;});              \
    Add_route_with_response("!manifest", [manifest](){return manifest;});              \
    if (!routed) {                                                                     \
        Add_route_with_response("!(.*)", [](){return false;});                         \
    }                                                                                  \
    return routed;                                                                     \
}