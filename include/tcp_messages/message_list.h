#pragma once
#include <tcp_messages/message.h>
#include <tcp_messages/message_event.h>
#include <map>
#include <deque>
namespace tcp_messages{
    struct Message_list : std::deque<Message> {
        void queue(const Message &);
        bool add_message_event(const std::string &, Message_event &);
        std::map<std::string, Message_event&> pending_responses;
    };
}