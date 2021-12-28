#include <tcp_messages//message.h>

namespace tcp_messages {

    void Message::set_body(const json_cpp::Json_base &new_content) {
        body << new_content;
    }

    Message::Message(const std::string &header, const json_cpp::Json_base &body) :
    header(header),
    body(body.to_json()){

    }

    Message::Message(const std::string &header, const std::string &body) :
    header(header),
    body(body){

    }
}