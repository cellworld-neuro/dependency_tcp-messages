#include <tcp_messages//message.h>

namespace tcp_messages {

    std::string get_response_header(const std::string &request_header) {
        return request_header + "_response";
    }

    Message::Message(const std::string &header) :
    header(header){

    }

}