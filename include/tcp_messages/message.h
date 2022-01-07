#pragma once
#include <json_cpp.h>

namespace tcp_messages {
    struct Message : json_cpp::Json_object {
        Message () = default;
        Message (const std::string &header);
        template <class T>
        Message (const std::string &header, const T &body){
            this->header = header;
            set_body(body);
        }
        template <class T>
        T get_body() const{
            if constexpr (std::is_same_v<T, std::string>) {
                return body;
            }
            T x;
            json_cpp::Json_object_wrapper wx(x);
            body >> wx;
            return x;
        }
        template <class T>
        void set_body(const T body){
            if constexpr (std::is_same_v<T, std::string>) {
                this->body = body;
            } else if constexpr (std::is_same_v<T, char *>) {
                this->body = std::string(body);
            } else if constexpr (std::is_same_v<T, const char *>) {
                this->body = std::string(body);
            } else if constexpr (std::is_base_of<json_cpp::Json_base ,T>::value) {
                this->body = body.to_json();
            }
        }
        Json_object_members({
            Add_member(header);
            Add_member(body);
        });
        std::string header;
        std::string body;
    };
}