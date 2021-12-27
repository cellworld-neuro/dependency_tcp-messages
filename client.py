from src import Client, Message
from json_cpp import JsonObject, JsonList

client = Client()
client.connect("127.0.0.1", 6500)
client.connection.send(Message("int_value"))
while not client.messages.contains("int_value_response"):
    pass
print(client.messages.get_message("int_value_response").get_body(int))
client.connection.send(Message("float_value"))
while not client.messages.contains("float_value_response"):
    pass
print(client.messages.get_message("float_value_response").get_body(float))
client.connection.send(Message("string_value"))
while not client.messages.contains("string_value_response"):
    pass
print(client.messages.get_message("string_value_response").get_body(str))
client.connection.send(Message("JsonObject_value"))
while not client.messages.contains("JsonObject_value_response"):
    pass
print(client.messages.get_message("JsonObject_value_response").get_body(JsonObject))
client.connection.send(Message("JsonList_value"))
while not client.messages.contains("JsonList_value_response"):
    pass
print(client.messages.get_message("JsonList_value_response").get_body(JsonList))
client.connection.send(Message("stop"))
while client:
    pass
