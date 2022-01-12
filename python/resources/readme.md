#tcp-messages
##A very opinionated communication framework for python

Tcp-messages makes many design decisions to ease the development of applications that require interacting with others. 
It allows the communication of complex data structures between programs running in the same network. 
Tcp-messages implements a server-clients architecture, where messages can be passed between nodes transparently.
Messages are data structures with two main components:
    - Header: String used to route the message to the corresponding handler
    - Body : Containing the payload of the message (int, float, bool, string, JsonObject or JsonList)  

Tcp-messages uses json-cpp to manage data structure serialization. For more information: https://github.com/germanespinosa/json-cpp  

###Installing tcp-messages
```
pip3 install tcp-messages
```


###Creating your first message service:
In this example two routes are defined for headers "greeting" and "stop".
- The "greeting" route: 
    Replies with a new message with a body composed by the received body plus the string " to you too". 
    The header of the response is automatically created based on the received message by adding the "_response" suffix
- The "stop" route: stops the server and send no response

####server.py:
```
from tcp_messages import MessageServer
server = MessageServer()
server.router.add_route("greeting", lambda message: message.body + " to you too!")
server.router.add_route("stop", lambda message: server.stop())
server.start(port=6500) #service starts listening on port 6500
print("server is running")
server.join()
```
The client first connects to the server and sends a first message with the "greeting" header.
Then it checks until it receives a response from the server and prints the body of the response.
Finally, it sends a message with the header "stop" to stop the server and waits until the server closes the connection. 
####client.py:
```
from tcp_messages import MessageClient, Message
client = MessageClient()
client.connect("127.0.0.1", 6500)
client.connection.send(Message("greeting", "Merry Christmas"))
while not client.messages:
    pass
print(client.messages.dequeue().body)
client.connection.send(Message("stop"))
while client.connection:
    pass
print("server stopped")
```

## Message routing 
Routing is a powerful tool to attend specific messages. 
When a connection receives a message with a header that matches a predefined route, the route handler is called.
A route handler is any callable object (function, lambda, etc.). If the handler returns a value, the framework will send a response to the sender.
- Value response: If the returned value is int, float or string, that value will become the body of the response and the header will be the same as the original message plus the "_response" suffix.
- Result response: if the response value is a boolean, the body will be "success" or "fail" depending on the value.
- JsonObject or JsonList: if the response is a JsonObject (https://github.com/germanespinosa/json-cpp) the value will be serialized and will be recovered in the other end.
- Message response: you can return a custom message with your own header and body

Message routing is possible in both, server and client. 
example:
####server.py
```
from tcp_messages import MessageServer
from json_cpp import JsonObject, JsonList
from random import randint, random, choice
import string

server = MessageServer()
server.router.add_route("int_value", lambda m: randint(0, 100))
server.router.add_route("float_value", lambda m: random())
server.router.add_route("string_value", lambda m: ''.join(choice(string.ascii_lowercase) for i in range(10)))
server.router.add_route("JsonObject_value", lambda m: JsonObject(v0=randint(0, 100), v1=random(), v2=JsonObject(v3="string value")))
server.router.add_route("JsonList_value", lambda m: JsonList(iterable=["1", 2, JsonObject(a=2), 4]))
server.router.add_route("stop", lambda m: server.stop())
server.start(port=6500) #service starts listening on port 6500
print("server is running")
server.join()
```
####client.py
```
from tcp_messages import MessageClient, Message
from json_cpp import JsonObject, JsonList

client = MessageClient()
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
```

###Message queue
Another way to communicate process is by using message queues. Unrouted messages get queued in the "messages" structure to be consumed later.

Using the previous