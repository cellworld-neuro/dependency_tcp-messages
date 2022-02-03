#pragma once
#include <easy_tcp.h>
#include <json_cpp.h>
#include <tcp_messages/message.h>
#include <tcp_messages/message_list.h>
#include <map>

namespace tcp_messages {

    struct Message_service : easy_tcp::Service {
        virtual bool route(const Message &) = 0;
        virtual void failed_route(const Message &);
        virtual void failed_message(const std::string &);
        virtual void unrouted_message(const Message &);
        void on_incoming_data(const std::string &) override;
        bool send_message(const Message &);
        void broadcast(const Message &);
        void broadcast_subscribed(const Message &);

        Message_list messages;
        std::map<std::string, Message_parts> _partials;
        bool _subscribed;
        void *_server;
    protected:
        bool _subscribe();
        bool _unsubscribe();
    };

    template<class T>
    struct Message_server : easy_tcp::Server<T>{
        static_assert(std::is_base_of<Message_service, T>::value, "must derive from Message_service");
        void on_new_connection(T &new_connection) override {
            new_connection._subscribed = false;
            new_connection._server = (void*) this;
        }
        void broadcast(const Message &message) {
            for (auto client:this->clients){
                if (client->connection->state == easy_tcp::Connection_state::Open){
                    try{
                        client->send_message(message);
                    } catch (...)
                    {}
                }
            }
        }
        void broadcast_subscribed(const Message &message) {
            for (auto client:this->clients){
                if (client->connection->state == easy_tcp::Connection_state::Open && client->_subscribed){
                    try{
                        client->send_message(message);
                    } catch (...)
                    {}
                }
            }
        }
    };
}