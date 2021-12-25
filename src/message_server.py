import socket
from threading import Thread
from .message_connection import Message_connection
from .message_router import Message_router
from .util import check_type, Message

class Message_server:

    def __init__(self, ip="0.0.0.0"):
        self.router = Message_router()
        self.connections = []
        self.thread = None
        self.threads = []
        self.server = socket.socket()
        self.ip = ip
        self.running = False
        self.thread = Thread(target=self.__proc__)

    def start(self, port):
        self.server.bind((self.ip, port))
        self.server.listen()
        self.server.settimeout(0.001)
        self.thread.start()
        while not self.running:
            pass


    def stop(self):
        self.running = False
        for c, t in zip(self.connections, self.threads):
            c.close()
            t.join()
        self.thread.join()
        self.server.close()

    def __proc__(self):
        self.running = True
        while self.running:
            try:
                client, address = self.server.accept()
                if client:
                    c = Message_connection(client)
                    self.connections.append(c)
                    t = Thread(target=self.__client_proc__, args=[c])
                    t.start()
                    self.threads.append(t)
            except socket.timeout:
                pass# no pending connecttions
            except Exception as e:
                print("Server: socked closed unexpectedly")
                self.running = False

    def __client_proc__(self, connection):
        check_type(connection, Message_connection, "incorrect type for connection")
        while connection.state == Message_connection.State.Open:
            message = connection.receive()
            if message:
                responses = self.router.route(message)
                if responses:
                    for response in responses:
                        if isinstance(response, Message):
                            connection.send(response)
                        elif isinstance(response, bool):
                            response_message = Message(message.header + "_result", "ok" if response else "fail")
                            connection.send(response_message)
                        else:
                            if response:
                                response_message = Message(message.header+"_result", str(response))
                                connection.send(response_message)