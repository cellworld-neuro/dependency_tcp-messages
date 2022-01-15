from time import sleep
from src import MessageClient, Message

client = MessageClient()
client.connect("127.0.0.1", 8500)
print(client.send_request(Message("int_value"), 0))

# print(client.send_request(Message("float_value")))
#
# print(client.send_request(Message("string_value")))
#
# print(client.send_request(Message("JsonObject_value")))
#
# print(client.send_request(Message("JsonList_value")))


def async_response_proc(message:Message):
    print("response", message)

client.send_async_request(Message("int_value"), async_response_proc)

print("waiting")

sleep(10)

print("stopping")
print(client.send_message(Message("stop")))
