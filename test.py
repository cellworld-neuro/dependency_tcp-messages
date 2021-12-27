from src import *
from json_cpp import JsonObject, JsonList
from random import randint, random, choice
import string

server = Server()
server.router.add_route("int_value", lambda m: randint(0, 100))
server.router.add_route("float_value", lambda m: random())
server.router.add_route("string_value", lambda m: ''.join(choice(string.ascii_lowercase) for i in range(10)))
server.router.add_route("JsonObject_value", lambda m: JsonObject(v0=randint(0, 100), v1=random(), v2=JsonObject(v3="string value")))
server.router.add_route("JsonList_value", lambda m: JsonList(iterable=["1", 2, JsonObject(a=2), 4]))
server.router.add_route("stop", lambda m: server.stop())
server.start(port=6500) #service starts listening on port 6500
print("server is running")
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
