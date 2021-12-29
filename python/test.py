from src import *
from json_cpp import JsonObject, JsonList
from random import randint, random, choice
import string

server = Server()
server.start(port=6500) #service starts listening on port 6500
print("server is running")
while server:
    m = server.messages.dequeue()
    if m:
        if m.header == "int_value":
            m.reply(Message("int_value_response", randint(0, 100)))
        elif m.header == "float_value":
            m.reply(Message("float_value_response", random()))
        elif m.header == "string_value":
            m.reply(Message("string_value_response", ''.join(choice(string.ascii_lowercase) for i in range(10))))
        elif m.header == "JsonObject_value":
            m.reply(Message("JsonObject_value_response", JsonObject(v0=randint(0, 100), v1=random(), v2=JsonObject(v3="string value"))))
        elif m.header == "JsonList_value":
            m.reply(Message("JsonList_value_response", JsonList(iterable=["1", 2, JsonObject(a=2), 4])))
        elif m.header == "stop":
            server.stop()
server.join()


# s = Server()
# # s.router.add_route("greeting", lambda m: "thanks!")
#
#
# def failed(m):
#     h = HttpRequestProcessor(m)
#     print(h)
#
#
# s.failed_messages = failed
#
#
# try:
#     s.start(6500)
# except ConnectionError:
#     print("Failed to start the server")
#     exit(1)
#
# while True:
#     pass
#
# # print("server started")
# # c = Client()
# # c.connect("127.0.0.1", 6500)
# # print("client connected")
# # c.connection.send(Message("greeting", "Merry Christmas"))
# # while not c.messages:
# #     pass
# #
# # m = c.messages.get_message("greeting_response")
# # print(m)
# # m.reply(Message("seems_it_worked", "ok"))
# #
# # while not s.messages:
# #     pass
# # print(s.messages)
