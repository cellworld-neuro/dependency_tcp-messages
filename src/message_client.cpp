#include <tcp_messages/message_client.h>
#include <stdexcept>
#include <chrono>

using namespace std;

namespace tcp_messages{

    bool Message_client::send_message(const Message &message) {
        return send_data(message.to_json());
    }

    Message Message_client::send_request(const Message &request, int time_out) {
        if (!send_message(request)) throw "failed to send the request to server.";
        auto check_point = std::chrono::high_resolution_clock::now();
        auto time_stamp = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration<double, std::milli>(time_stamp - check_point).count();
        auto response_header = get_response_header(request.header);
        while (!contains(response_header) && (ms < time_out || time_out == 0)){
            time_stamp = std::chrono::high_resolution_clock::now();
            ms = std::chrono::duration<double, std::milli>(time_stamp - check_point).count();
        }
        if (contains(response_header)) {
            return get_message(response_header);
        } else {
            throw "request timed out";
        }
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
        messages.push_back(message);
    }

    bool Message_client::route(const Message &) {
        return false;
    }

    void Message_client::failed_route(const Message &) {

    }

    Manifest Message_client::get_manifest() {
        return send_request(Message("!manifest")).get_body<Manifest>();
    }
}