#pragma once
#include <easy_tcp.h>
#include <tcp_messages/message.h>
#include <tcp_messages/message_list.h>
#include <tcp_messages/message_event.h>
#include <map>

namespace tcp_messages {
    struct Message_client: easy_tcp::Client {
        virtual bool route(const Message &);
        virtual void unrouted_message(const Message &);
        virtual void failed_route(const Message &);
        void received_data(const std::string &) override;
        virtual void failed_message(const std::string &);
        bool subscribe();
        bool unsubscribe();
        bool contains(const std::string &header);
        bool send_message(const Message &);
        Message send_request(const Message &request, int time_out = -1);
        void send_async_request(const Message &request, void (*call_back)(Response_type));
        Manifest get_manifest();
        Message get_message (const std::string &header);
        Message get_last_message (const std::string &header);
        void set_request_time_out(int);
        Message_list messages;
        std::map<std::string,Message_event &> _pending_responses;
        std::map<std::string, Message_parts> _partials;
    private:
        int _request_time_out = 500;
    };
}