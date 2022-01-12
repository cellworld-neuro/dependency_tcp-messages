import socket
from .message import Message
from .message_list import MessageList
from .connection import Connection
from .router import Router
from json_cpp import JsonList
import datetime

class MessageClient:
    def __init__(self):
        self.failed_messages = None
        self.running = False
        self.registered = False
        self.router = Router()
        self.router.unrouted_message = self.__unrouted__
        self.ip = ""
        self.port = 0
        self.messages = MessageList()
        self.connection = None

    def __unrouted__(self, message: Message):
        self.messages.append(message)

    def connect(self, ip: str, port: int):
        self.ip = ip
        self.port = port
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.ip, self.port))
        self.connection = Connection(s, self.failed_messages)
        self.router.attend(self.connection)

    def send_request(self, message: Message, time_out: int = 500) -> Message:
        self.send_message(message)
        start = datetime.datetime.now()
        while ((datetime.datetime.now()-start).total_seconds() * 1000) < time_out or time_out==0:
            if self.messages.contains(message.header + "_response"):
                return self.messages.get_message(message.header + "_response")
        raise TimeoutError("the request has timed_out")

    def get_manifest(self):
        return self.send_request(Message("!manifest")).get_body(JsonList)

    def send_message(self, message: Message):
        self.connection.send(message)

    def disconnect(self):
        self.running = False

    def __del__(self):
        self.disconnect()

    def __bool__(self):
        return self.connection is True
