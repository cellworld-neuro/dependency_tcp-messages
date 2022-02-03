#include <catch.h>
#include <tcp_messages.h>
#include <fstream>


using namespace tcp_messages;
using namespace std;

TEST_CASE("long_message") {
    Message message;
    message.header = "test_header";
    std::ifstream t("json");
    std::stringstream buffer;
    buffer << t.rdbuf();
    message.body = buffer.str();

    auto parts = Message_parts(message);
    int s = 0;
    for (auto part:parts) {
        s += part.body.size();
        cout << part.seq << " of " << part.parts << ": size = " << part.to_json().size() << endl;

    }
    cout <<  "total message: " <<  message.body.size() << endl;
    CHECK(s == message.body.size());
    auto joined = parts.join();
    CHECK(message.to_json() == joined.to_json());
}