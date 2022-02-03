#include <tcp_messages/message_server.h>
#include <iostream>


using namespace std;
namespace tcp_messages {

    void Message_service::failed_route(const Message &m) {
        cerr << "Server: failed to route message " << m << endl;
    }

    void Message_service::failed_message(const string &d) {
        cerr << "Server: failed parsing data '" << d << "' to message" << endl;
    }


    void Message_service::unrouted_message(const Message &message) {
        messages.queue(message);
    }

    void Message_service::on_incoming_data(const string &data) {
        Message message;
        try {
            data >> message;
            if (message.parts > 1) {
                if (!_partials.contains(message.id)) {
                    _partials[message.id] = Message_parts();
                }
                _partials[message.id].push_back(message);
                if( _partials[message.id].is_ready()) {
                    message = _partials[message.id].join();
                    _partials.erase(message.id);
                } else {
                    return ;
                }
            }
            if (!route(message)) unrouted_message(message);
        } catch (...) {
            failed_message(data);
        }
    }

    bool Message_service::send_message(const Message &message) {
        Message_parts parts (message);
        for (auto &part:parts) {
            if (!send_data(part.to_json())) return false;
        }
        return true;
    }

    void Message_service::broadcast(const Message &message) {
        auto message_server=(Message_server<Message_service> *) this->_server;
        message_server->broadcast(message);
    }

    void Message_service::broadcast_subscribed(const Message &message) {
        auto message_server=(Message_server<Message_service> *) this->_server;
        message_server->broadcast_subscribed(message);
    }

    bool Message_service::subscribe() {
        _subscribed = true;
        return false;
    }
}