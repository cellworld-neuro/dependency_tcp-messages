#include <catch.h>
#include <thread>
#include <chrono>
#include <tcp_messages.h>

using namespace tcp_messages;
using namespace std;

TEST_CASE("get_response") {
    cout << get_response("h1", true) << endl;
    cout << get_response("h1", 1) << endl;
    cout << get_response("h1", "HELLO!") << endl;
    cout << get_response("h1", Message("hola","1")) << endl;
    cout << get_response("h1", true) << endl;
}
//
//struct test_data :json_cpp::Json_object{
//    Json_object_members(
//        Add_member(d1);
//            Add_member(d2)
//    )
//    std::string d1;
//    std::string d2;
//};


struct Test_service : Message_service {
    Routes(
            Add_route_with_response("H1", h1, int);
            Add_route_with_response("H2", h2, int);
            Add_route_with_response("H3", h3, int);
            Add_route_with_response("H4", h4, int);
            Add_route("stop", stop);
            )
    int h1(int i) { return i+1; }
    string h2(int i) { return to_string(i+1); }
    bool h3(int i) { return i % 2; }
    Message h4(int i) { return Message("H4_response",i+1); }
};

using namespace std::chrono_literals;
using namespace std::this_thread;
TEST_CASE("test_service") {
    Message_server<Test_service> s;
    cout << "starting server" << endl;
    s.start(6500);
    thread t([](){
        cout << "message wait start" << endl;
        sleep_for(5s);
        cout << "message wait ends" << endl;
        Message_client c;
        c.connect("127.0.0.1", 6500);
        cout << c.send_request(Message("H1",100)) << endl;
        cout << c.send_request(Message("H2",100)) << endl;
        cout << c.send_request(Message("H3",100)) << endl;
        cout << c.send_request(Message("H4",100)) << endl;
        cout << c.send_request(Message("!manifest",100)).get_body<Manifest>() << endl;
        c.send_message(Message("stop"));
    });
    s.join();
    cout << "stopping server" << endl;
    t.join();
}


TEST_CASE("test_message") {
    cout << Message("hello","hola!") << endl;
}