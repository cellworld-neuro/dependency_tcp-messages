#include <catch.h>
#include <thread>
#include <chrono>
#include <tcp_messages.h>
#include <iomanip>

using namespace tcp_messages;
using namespace std;


TEST_CASE("get_response") {
    auto m = Message("h1");
    cout << "request: " << m << endl;
    cout << get_response(m, true) << endl;
    cout << get_response(m, 1) << endl;
    cout << get_response(m, "HELLO!") << endl;
    cout << get_response(m, Message("hola","1")) << endl;
    cout << get_response(m, true) << endl;
}


struct Test_service : Message_service {
    Routes(
            Add_route_with_response("H1", h1, int);
            Add_route_with_response("H2", h2, int);
            Add_route_with_response("H3", h3, int);
            Add_route_with_response("H4", h4, int);
            Add_route("stop", stop);
            )
    int h1(int i) {
        return i+1;
    }
    string h2(int i) {
        return to_string(i+1);
    }
    bool h3(int i) {
        return i % 2;
    }
    Message h4(int i) {
        return Message("H4_response",i+1);
    }
};

using namespace std::chrono_literals;
using namespace std::this_thread;

Response_handler(async_handler_0){
    cout << "async request: " << response << endl;
}

TEST_CASE("test_service") {
    Message_server<Test_service> s;
    cout << "starting server" << endl;
    s.start(6500);
    thread t([](){
        cout << "message wait start" << endl;
        sleep_for(1s);
        cout << "message wait ends" << endl;
        Message_client c;
        c.connect("127.0.0.1", 6500);
        auto m = Message("H1",100);
        cout << "request :" << m << endl;
        cout << "response :" << c.send_request(m,0) << endl;
        m = Message("H2",100);
        cout << "request :" << m << endl;
        cout << "response :" << c.send_request(Message("H2",100),0) << endl;
        m = Message("H3",100);
        cout << "request :" << m << endl;
        cout << "response :" << c.send_request(Message("H3",100),0) << endl;
        m = Message("H4",100);
        cout << "request :" << m << endl;
        cout << "response :" << c.send_request(Message("H4",100),0) << endl;
        m = Message("H4",100);
        //c.send_async_request(m, async_handler_0);
        sleep_for(1s);
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


Response_handler(handler1){
    cout << "event_handler: " << response << endl;
}


TEST_CASE("message_event") {
    Message_event event(handler1);
    thread t([&event](){
        sleep_for(1s);
        Message message( "test_header", "test_body");
        event.trigger(message);
    });
    cout<< "wait start" << endl;
    auto m = event.wait(2000);
    cout << "sync:" << m << endl;
    cout<< "wait end" << endl;
    if (t.joinable()) t.join();
    sleep_for(1s);
}

Response_handler(test){
    cout << "async worked: " << response << endl;
}

TEST_CASE("message_list_event_async") {
    Message_event event(test);
    Message request( "test_header", "test_body");
    Message_list list;
    list.add_message_event(request.id, event);
    list.queue(request);
}

TEST_CASE("message_list_event_sync") {
    Message_event event;
    Message request( "test_header", "test_body");
    Message_list list;
    list.add_message_event(request.id, event);
    thread t([&list, &request](){
        sleep_for(1s);
        list.queue(request);
    });
    cout << "waiting..." << endl;
    auto m = event.wait(2000);
    if (t.joinable()) t.join();
    cout << "sync worked: " << m << endl;
}