#include <tcp_messages/message_client.h>
#include <stdexcept>
#include <chrono>

using namespace std;

namespace tcp_messages{

    bool Message_client::send_message(const Message &message) {
        return send_data(message.to_json());
    }

    Message Message_client::send_request(const Message &request, int time_out) {
        Message_event event;
        messages.add_message_event(request.id, event);
        if (!send_message(request)) throw "failed to send the request to server.";
        return event.wait(time_out);
    }


    void Message_client::received_data(const std::string &data) {
        try {
            auto message = json_cpp::Json_create<Message>(data);
            if (!route(message))
                unrouted_message(message);
        } catch (...) {
            failed_message(data);
        }
    }

    void Message_client::failed_message(const std::string &d) {
        cerr << "Client: failed parsing data '" << d << "' to message" << endl;
    }

    bool Message_client::contains(const std::string &header) {
        if (messages.empty()) return false;
        for (auto &message:messages) if (message.header == header) return true;
        return false;
    }

    Message Message_client::get_message(const string &header) {
        if (messages.empty()) throw std::runtime_error("message not found: " + header);
        auto header_match = [header](const Message& m)->size_t{
            return m.header == header;
        };
        auto it = find_if(messages.begin(), messages.end(), header_match);
        if ( it == messages.end()) throw std::runtime_error("message not found: " + header);
        Message res_message = *it;
        messages.erase(it);
        return res_message;
    }

    Message Message_client::get_last_message(const string &header) {
        if (!contains(header)) throw std::runtime_error("message not found: " + header);
        Message message;
        while  (contains(header)) message = get_message(header);
        return message;
    }

    void Message_client::unrouted_message(const Message &message) {
        messages.queue(message);
    }

    bool Message_client::route(const Message &) {
        return false;
    }

    void Message_client::failed_route(const Message &) {

    }

    Manifest Message_client::get_manifest() {
        return send_request(Message("!manifest")).get_body<Manifest>();
    }

    void Message_client::send_async_request(const Message &request, void (*call_back)(Response_type)) {
        Message_event event(call_back);
        messages.add_message_event(request.id, event);
        if (!send_message(request)) throw "failed to send the request to server.";
    }
}