import socket
from .message import Message
from .message_list import MessageList
from .connection import MessageConnection
from .router import MessageRouter


class MessageClient:
    def __init__(self):
        self.failed_messages = None
        self.running = False
        self.registered = False
        self.router = MessageRouter()
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
        self.connection = MessageConnection(s, self.failed_messages)
        self.router.attend(self.connection)

    def disconnect(self):
        self.running = False
