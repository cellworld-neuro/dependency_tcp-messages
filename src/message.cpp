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

    Message_parts::Message_parts(const Message &message) {
        int part_count = (message.body.size() / 1024) + 1;
        for (unsigned int i = 0 ; i < part_count; i++){
            auto &part = emplace_back();
            part.id = message.id;
            part.header = message.header;
            part.parts = part_count;
            part.body = message.body.substr(i * 1024, 1024);
            part.seq = i;
        }
    }

    Message Message_parts::join() {
        if (empty()) return Message();
        Message joined =(*this)[0].to_message();
        for (unsigned int part_number=1; part_number<size(); part_number++){
            joined.body += (*this)[part_number].body;
        }
        return joined;
    }

    bool Message_parts::is_ready() {
        if (empty()) return false;
        if (size()!=(*this)[1].parts) return false;
        return true;
    }

    Message Message_part::to_message() {
        Message message;
        message.id = id;
        message.body = body;
        message.header = header;
        return message;
    }
}