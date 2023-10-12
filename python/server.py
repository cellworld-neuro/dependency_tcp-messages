from src import MessageServer, MessageServiceServer
from json_cpp import JsonObject, JsonList
from random import randint, random, choice
import string


class MyService:

    def __init__(self):
        self.value = 0
        self.list = JsonList(int)

    def method1(self, v: int):
        print("method1")
        self.value += v
        return self.value

    def method2(self, v: int):
        print("method2")
        self.value -= v
        return self.value

    def method3(self, v1: str, v2: str):
        print("method3")
        return v1 + v2

    def method4(self, v: int):
        print("method4")
        self.value += v
        self.list.append(v)

    def method5(self, i: int, f: float, s: string):
        print("method4 %i, %f, %s" % (i, f, s))
        return "method4 %i, %f, %s" % (i, f, s)

    def method6(self):
        print("hello")
        return self.list

# server = MessageServer()
# server.router.add_route("int_value", lambda m: randint(0, 100))
# server.router.add_route("float_value", lambda m: random())
# server.router.add_route("string_value", lambda m: ''.join(choice(string.ascii_lowercase) for i in range(10000)))
# server.router.add_route("JsonObject_value", lambda m: JsonObject(v0=randint(0, 100), v1=random(), v2=JsonObject(v3="string value")))
# server.router.add_route("JsonList_value", lambda m: JsonList(iterable=["1", 2, JsonObject(a=2), 4]))
# server.router.add_route("parse_sum", lambda a=0, b=0: a + b)
# server.router.add_route("stop", server.stop)

server = MessageServiceServer(MyService, True)
print(dir(server))
server.start(port=6500) #service starts listening on port 6500
print("server is running")
server.join()


# from src import MessageServer, MessageClient, Message
# from json_cpp import JsonObject, JsonList
# from random import randint, random, choice
# import string
#
# server = MessageServer()
# server.router.add_route("int_value", lambda m: randint(0, 100), int)
# server.router.add_route("float_value", lambda m: random())
# server.router.add_route("string_value", lambda m: ''.join(choice(string.ascii_lowercase) for i in range(10)))
# server.router.add_route("JsonObject_value", lambda m: JsonObject(v0=randint(0, 100), v1=random(), v2=JsonObject(v3="string value")))
# server.router.add_route("JsonList_value", lambda m: JsonList(iterable=["1", 2, JsonObject(a=2), 4]))
# server.router.add_route("stop", lambda m: server.stop())
# server.start(port=5500) #service starts listening on port 6500
# print("server is running")
#
#
#
# client = MessageClient()
# client.router.add_route("int_value_response", lambda v: print("value:", v, "type:", type(v).__name__), int)
# client.router.add_route("float_value_response", lambda v: print("value:", v, "type:", type(v).__name__), float)
# client.router.add_route("string_value_response", lambda v: print("value:", v, "type:", type(v).__name__), str)
# client.router.add_route("JsonObject_value_response", lambda v: print("value:", v, "type:", type(v).__name__), JsonObject)
# client.router.add_route("JsonList_value_response", lambda v: print("value:", v, "type:", type(v).__name__), JsonList)
# client.connect("127.0.0.1", 5500)
# client.connection.send(Message("int_value", 1))
# client.connection.send(Message("float_value"))
# client.connection.send(Message("string_value"))
# client.connection.send(Message("JsonObject_value"))
# client.connection.send(Message("JsonList_value"))
# while client.router.routing_count < 5:
#     pass
#
# print(client.send_request(Message("!manifest")))
# client.connection.send(Message("stop"))
# while client.connection:
#     pass
#
#
