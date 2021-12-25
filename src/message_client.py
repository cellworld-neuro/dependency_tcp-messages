import socket
from threading import Thread
from .util import Message, check_type, Message_list
from .message_connection import Message_connection
from .message_router import Message_router


class Message_client:
    def __init__(self):
        self.unrouted_messages = None
        self.failed_messages = None
        self.running = False
        self.registered = False
        self.router = Message_router()
        self.router.unrouted_message = self.__unrouted__
        self.ip = ""
        self.port = 0
        self.client_thread = None
        self.messages = Message_list()
        self.connection = None

    def __unrouted__(self, message):
        self.messages.append(message)

    def connect(self, ip, port):
        check_type(ip, str, "incorrect type for string")
        check_type(port, int, "incorrect type for port")
        self.ip = ip
        self.port = port
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.ip, self.port))
        self.connection = Message_connection(s, self.failed_messages)
        self.client_thread = Thread(target=self.__proc__)
        self.client_thread.start()
        while not self.running:
            pass

    def disconnect(self):
        self.running = False
        self.client_thread.join(4)

    def __proc__(self):
        self.running = True
        while self.running and self.connection.state == Message_connection.State.Open:
            message = self.connection.receive()
            if message:
                responses = self.router.route(message)
                if responses:
                    for response in responses:
                        if not response:
                            continue
                        if isinstance(response, Message):
                            self.connection.send(response)
                        elif isinstance(response, bool):
                            response_message = Message(message.header + "_result", "ok" if response else "fail")
                            self.connection.send(response_message)
                        else:
                            response_message = Message(message.header + "_result", str(response))
                            self.connection.send(response_message)
