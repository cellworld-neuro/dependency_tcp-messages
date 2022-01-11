#include <tcp_messages//message.h>

namespace tcp_messages {

    std::string get_response_header(const std::string &request_header) {
        return request_header + "_response";
    }

    Message::Message(const std::string &header) :
    header(header){

    }

    void Manifest::add_route(const std::string &route, const std::string &input_type) {
        auto &new_route = emplace_back();
        new_route.input_type = input_type;
        new_route.route = route;
    }
}