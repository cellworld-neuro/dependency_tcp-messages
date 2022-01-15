#pragma once
#include <easy_tcp.h>
#include <json_cpp.h>
#include <tcp_messages/message.h>
#include <tcp_messages/message_list.h>

namespace tcp_messages {
    struct Message_service : easy_tcp::Service {
        virtual bool route(const Message &) = 0;
        virtual void failed_route(const Message &);
        virtual void failed_message(const std::string &);
        virtual void unrouted_message(const Message &);
        void on_incoming_data(const std::string &) override;
        bool send_message(const Message &);
        Message_list messages;
    };

    template<class T>
    struct Message_server : easy_tcp::Server<T>{
        static_assert(std::is_base_of<Message_service, T>::value, "must derive from Message_service");
    };
}