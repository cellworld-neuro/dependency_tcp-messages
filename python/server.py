from src import MessageServer
from json_cpp import JsonObject, JsonList
from random import randint, random, choice
import string


def new_connection(p = None):
    print ("New connection")


server = MessageServer()
server.router.add_route("int_value", lambda m: randint(0, 100))
server.router.add_route("float_value", lambda m: random())
server.router.add_route("string_value", lambda m: ''.join(choice(string.ascii_lowercase) for i in range(10000)))
server.router.add_route("JsonObject_value", lambda m: JsonObject(v0=randint(0, 100), v1=random(), v2=JsonObject(v3="string value")))
server.router.add_route("JsonList_value", lambda m: JsonList(iterable=["1", 2, JsonObject(a=2), 4]))
server.router.add_route("stop", lambda m: server.stop())
server.start(port=6500) #service starts listening on port 6500
server.on_new_connection = new_connection
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
