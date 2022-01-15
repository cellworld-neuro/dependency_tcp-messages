#pragma once
#include <tcp_messages/message.h>
#include <condition_variable>

#define Response_type const tcp_messages::Message &
#define Response_handler(handler_name) void handler_name (Response_type response)

namespace tcp_messages {
    struct Message_event {
        Message_event() = default;
        void trigger(Response_type);
        Message &wait(int time_out=500);

        std::condition_variable event;
        Message message;
        std::mutex mtx;
    };
}