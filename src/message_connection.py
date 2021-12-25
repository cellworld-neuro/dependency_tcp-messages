from enum import Enum
from .util import check_type, Message, Message_list, Json_get
import socket


class Message_connection:
    class State:
        Open = 1
        Close = 4

    def __init__(self, s, failed_message=None):
        check_type(s, socket.socket, "incorrect type for s")
        self.socket = s
        self.socket.settimeout(0.001)
        self.failed_message = failed_message
        self.pending_messages = Message_list()
        self.state = None
        self.peek()

    def close(self):
        self.socket.close()
        self.state = Message_connection.State.Close

    def send(self, message):
        check_type(message, Message, "incorrect type for message")
        message_str = str(message)
        message_bytes = message_str.encode()
        message_bytes += b'\x00'
        self.socket.send(message_bytes)

    def peek(self):
        try:
            data = self.socket.recv(1, socket.MSG_PEEK)
        except socket.timeout:
            self.state = Message_connection.State.Open
            return False
        except:
            self.state = Message_connection.State.Close
            return False
        self.state = Message_connection.State.Open
        return True

    def receive(self):
        if not self.state == Message_connection.State.Open: #if the connection is not open
            return
        if self.pending_messages:
            return self.pending_messages.dequeue() #if there are pending messages retrun the oldest
        if not self.peek():
            return None #if there are no messages
        data = bytes()
        try:
            data = self.socket.recv(8192)
        except socket.timeout as e:
            pass
        except Exception as e:
            self.state = Message_connection.State.Close #if connection was closed from the other side
        else:
            if data:
                messages_str = data.decode().split('\x00')
                for message_str in messages_str:
                    if message_str:
                        try:
                            message = Json_get(message_str, Message)
                            self.pending_messages.queue(message)
                        except:
                            if self.failed_message:
                                self.failed_message(message_str)
        return self.pending_messages.dequeue()


