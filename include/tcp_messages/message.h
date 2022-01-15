#pragma once
#include <json_cpp.h>

namespace tcp_messages {

    struct Route_description : json_cpp::Json_object {
        Json_object_members({
                                Add_member(route);
                                Add_member(input_type);
                            });
        std::string route;
        std::string input_type;
    };

    struct Manifest : json_cpp::Json_vector<Route_description> {
        void add_route (const std::string &route, const std::string &input_type="");
    };

    std::string get_response_header(const std::string &request_header);

    struct Message : json_cpp::Json_object {
        Message (const std::string &header);
        Message ();
        template <class T>
        Message (const std::string &header, const T &body){
            this->header = header;
            set_body(body);
            set_id();
        }
        template <class T>
        T get_body() const{
            if constexpr (std::is_same_v<T, std::string>) {
                return body;
            }  else if constexpr (std::is_base_of<json_cpp::Json_base ,T>::value) {
                return json_cpp::Json_create<T>(this->body);
            } else {
                T x;
                json_cpp::Json_object_wrapper wx(x);
                body >> wx;
                return x;
            }
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
            } else {
                this->body = std::to_string(body);
            }
        }
        Json_object_members({
            Add_member(header);
            Add_member(body);
            Add_optional_member(id);
        });
        void set_id();
        void set_id(const std::string &);
        std::string header;
        std::string body;
        std::string id;
    };
}