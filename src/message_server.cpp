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
        Message_part message_part;
        Message message;
//        try {
            data >> message_part;
            if (message_part.parts > 1) {
                if (!_partials.contains(message_part.id)) {
                    _partials[message_part.id] = Message_parts();
                }
                _partials[message_part.id].push_back(message_part);
                if( _partials[message_part.id].is_ready()) {
                    message = _partials[message_part.id].join();
                    _partials.erase(message_part.id);
                } else {
                    // message not complete yet
                    return ;
                }
            } else {
                message = message_part.to_message();
            }
            if (!route(message)) unrouted_message(message);
//        } catch (...) {
//            failed_message(data);
//        }
    }

    bool Message_service::send_message(const Message &message) {
        //_sending_message.lock();
        Message_parts parts (message);
        for (auto &part:parts) {
            if (!send_data(part.to_json())) {
                //_sending_message.unlock();
                return false;
            }
        }
        //_sending_message.unlock();
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

    bool Message_service::_subscribe() {
        _subscribed = true;
        return true;
    }

    bool Message_service::_unsubscribe() {
        _subscribed = false;
        return true;
    }
}