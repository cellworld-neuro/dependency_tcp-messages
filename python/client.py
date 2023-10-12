from json_cpp import *
from time import sleep
from src import MessageClient, Message

client = MessageClient()
client.connect("127.0.0.1", 6500)
sleep(2)
print("method1")
print(client.send_request("method1", v=10))
sleep(2)
print("method2")
print(client.send_request("method2", v=30))
sleep(2)
print("method3")
print(client.send_request("method3", v1=130, v2=230))
sleep(2)
print("method4")
print(client.send_message("method4", v=130))
sleep(2)
print("method5")
print(client.send_request("method5", i=130, f=1.4, s="test"))
sleep(2)
print("stopping")
print(client.send_message(Message("stop")))

sleep(3)
