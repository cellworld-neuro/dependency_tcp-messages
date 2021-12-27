from src import Client, Message
from json_cpp import JsonObject, JsonList

client = Client()
client.router.add_route("int_value_response", lambda v: print("value:", v, "type:", type(v).__name__), int)
client.router.add_route("float_value_response", lambda v: print("value:", v, "type:", type(v).__name__), float)
client.router.add_route("string_value_response", lambda v: print("value:", v, "type:", type(v).__name__), str)
client.router.add_route("JsonObject_value_response", lambda v: print("value:", v, "type:", type(v).__name__), JsonObject)
client.router.add_route("JsonList_value_response", lambda v: print("value:", v, "type:", type(v).__name__), JsonList)
client.connect("127.0.0.1", 6500)
client.connection.send(Message("int_value"))
client.connection.send(Message("float_value"))
client.connection.send(Message("string_value"))
client.connection.send(Message("JsonObject_value"))
client.connection.send(Message("JsonList_value"))
while client.router.routing_count < 5:
    pass
client.connection.send(Message("stop"))
while client.connection:
    pass
