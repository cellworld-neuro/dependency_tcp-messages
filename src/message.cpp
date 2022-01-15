#include <tcp_messages//message.h>

using namespace std;

namespace tcp_messages {

    std::string get_response_header(const std::string &request_header) {
        return request_header + "_response";
    }

    string random_hex(int size){
        std::stringstream stream;
        for (auto i = 0; i<size; i++)
            stream << std::hex << (rand() % 16);
        return string(stream.str());
    }

    Message::Message(const std::string &header) :
    header(header){
        set_id();
    }

    void Message::set_id() {
        id = random_hex(8) + "-" + random_hex(4) + "-" + random_hex(4) + "-" + random_hex(4) + "-" + random_hex(12);
    }

    void Message::set_id(const string &new_id) {
        id = new_id;
    }

    Message::Message() {
        set_id();
    }

    void Manifest::add_route(const std::string &route, const std::string &input_type) {
        auto &new_route = emplace_back();
        new_route.input_type = input_type;
        new_route.route = route;
    }
}