#include <tcp_messages/message_client.h>
#include <stdexcept>
#include <chrono>

using namespace std;

namespace tcp_messages{

    bool Message_client::send_message(const Message &message) {
        Message_parts parts (message);
        for (auto &part:parts) {
            if (!send_data(part.to_json())) return false;
        }
        return true;
    }

    Message Message_client::send_request(const Message &request, int time_out) {
        Message_event event;
        _pending_responses.insert(std::pair<std::string,Message_event &>(request.id, event));
        if (!send_message(request)) throw std::runtime_error("failed to send the request to server.");
        if (time_out < 0) time_out = _request_time_out;
        return event.wait(time_out);
    }

    void Message_client::received_data(const std::string &data) {
        try {
            Message message;
            auto message_part = json_cpp::Json_create<Message_part>(data);

            if (message_part.parts > 1) {
                if (!_partials.contains(message_part.id)) {
                    _partials[message_part.id] = Message_parts();
                }
                _partials[message_part.id].push_back(message_part);
                if( _partials[message_part.id].is_ready()) {
                    message = _partials[message_part.id].join();
                    _partials.erase(message_part.id);
                } else {
                    return ;
                }
            } else {
                message = message_part.to_message();
            }

            if (_pending_responses.contains(message.id)){
                auto &event = _pending_responses.at(message.id);
                _pending_responses.erase(message.id);
                event.trigger(message);
            } else {
                if (!route(message))
                    unrouted_message(message);
            }
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
        thread ([&request, &call_back, this] (){
            auto response = send_request(request, 0);
            call_back(response);
        }).detach();
    }

    bool Message_client::subscribe() {
        return send_request(Message("!subscribe")).get_body<bool>();
    }

    bool Message_client::unsubscribe() {
        return send_request(Message("!unsubscribe")).get_body<bool>();
    }

    void Message_client::set_request_time_out(int time_out) {
        _request_time_out = time_out;
    }
}