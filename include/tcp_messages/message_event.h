#pragma once
#include <tcp_messages/message.h>
#include <condition_variable>

#define Message_event_handler(handler_name, message) void handler_name (const Message &message)

namespace tcp_messages {
    struct Message_event {
        Message_event() = default;
        explicit Message_event(void (*call_back)(const Message &));
        void trigger(const Message &);
        Message &wait(int time_out=500);

        void (*call_back)(const Message &) = nullptr;
        std::condition_variable event;
        Message message;
        std::mutex mtx;
    };
}