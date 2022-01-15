#include <tcp_messages/message_list.h>

using namespace std;

namespace tcp_messages {

    void Message_list::queue(const Message &message) {
        push_back(message);
        if (pending_responses.contains(message.id)){
            auto &event = pending_responses.at(message.id);
            pending_responses.erase(message.id);
            event.trigger(message);
        }
    }

    bool Message_list::add_message_event(const std::string &request_id, Message_event &event) {
        if (pending_responses.contains(request_id)){
            return false;
        }
        pending_responses.insert(std::pair<std::string,Message_event &>(request_id, event));
        return true;
    }
}