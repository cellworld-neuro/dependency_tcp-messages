#include <tcp_messages/message_event.h>

namespace tcp_messages{
    void Message_event::trigger(const tcp_messages::Message &message) {
        if (call_back) call_back (message);
        std::unique_lock<std::mutex> lock(mtx);
        event.notify_all();
    }

    Message_event::Message_event(void (*call_back)(const Message &)):
    call_back(call_back){
    }

    Message &Message_event::wait(int time_out) {
        std::unique_lock<std::mutex> lock(mtx);
        if (time_out) {
            if (event.wait_for(lock, std::chrono::duration<double, std::milli>(time_out)) == std::cv_status::timeout) {
                throw std::runtime_error("timed out");
            } else {
                return message;
            }
        } else {
            event.wait(lock);
            return message;
        }
    }
}

