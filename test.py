from src import *
from time import sleep

s = Server()

s.router.add_route("greeting", lambda m: "thanks!")

try:
    s.start(6500)
except ConnectionError:
    print("Failed to start the server")
    exit(1)
print("server started")
c = Client()
c.connect("127.0.0.1", 6500)
print("client connected")
c.connection.send(Message("greeting", "Merry Christmas"))
while not c.messages:
    pass

print(c.messages)


