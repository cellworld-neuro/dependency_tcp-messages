#include <tcp_messages/message_server.h>
#include <iostream>


using namespace std;
namespace tcp_messages{

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
        bool msg_converted = false;
        Message message;
        try {
            data >> message;
            msg_converted = true;
        } catch (...) {
            failed_message(data);
        }
        if (msg_converted) {
            if (!route(message)) unrouted_message(message);
        }
    }

    bool Message_service::send_message(const Message &message) {
        return send_data(message.to_json());
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