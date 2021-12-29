import socket
from threading import Thread
from .connection import Connection
from .router import Router
from .message import Message
from .message_list import MessageList

class Server:

    def __init__(self, ip="0.0.0.0"):
        self.failed_messages = None
        self.messages = MessageList()
        self.router = Router()
        self.router.unrouted_message = self.__unrouted__
        self.connections = []
        self.thread = None
        self.server = socket.socket()
        self.ip = ip
        self.running = False
        self.thread = Thread(target=self.__proc__)
        self.thread.daemon = True

    def __unrouted__(self, message: Message):
        self.messages.append(message)

    def start(self, port):
        self.server.bind((self.ip, port))
        self.server.listen()
        self.server.settimeout(0.001)
        self.thread.start()
        while not self.running:
            pass

    def stop(self):
        if self.running:
            self.running = False
            for c in self.connections:
                c.close()
            self.thread.join()
            self.server.close()

    def __proc__(self):
        self.running = True
        while self.running:
            try:
                client, address = self.server.accept()
                if client:
                    client_connection = Connection(client, self.failed_messages)
                    self.connections.append(client_connection)
                    self.router.attend(client_connection)

            except socket.timeout:
                pass# no pending connecttions
            except Exception as e:
                print("Server: socked closed unexpectedly")
                self.running = False

    def join(self):
        if self.running:
            self.thread.join()

    def __del__(self):
        self.stop()

    def __bool__(self):
        if self.running:
            return True
        else:
            try:
                self.thread.join()
            finally:
                return False
