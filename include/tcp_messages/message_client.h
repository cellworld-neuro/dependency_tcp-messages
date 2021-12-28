#pragma once
#include <deque>
#include <easy_tcp.h>
#include <tcp_messages/message.h>

namespace tcp_messages {
    struct Message_client: easy_tcp::Client {
        virtual bool route(const Message &);
        virtual void unrouted_message(const Message &);
        virtual void failed_route(const Message &);
        void received_data(const std::string &) override;
        virtual void failed_message(const std::string &);
        bool contains(const std::string &header);
        bool send_message(const Message &);
        Message get_message (const std::string &header);
        Message get_last_message (const std::string &header);
        std::deque<Message> messages;
    };
}