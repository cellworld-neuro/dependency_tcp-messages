from .util import check_type
from json_cpp import JsonList
from .message import Message


class MessageList(JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=Message)

    def queue(self, message):
        check_type(message, Message, "wrong type for message")
        self.append(message)

    def dequeue(self):
        if len(self):
            message = self[0]
            del self[0]
            return message
        return None

    def contains(self, header):
        for message in self:
            if message.header == header:
                return True
        return False

    def get_message(self, header):
        for i in range(len(self)):
            if self[i].header == header:
                message = self[i]
                del self[i]
                return message
        return None

    def get_last_message(self, header):
        message = None
        for i in range(len(self)):
            if self[i].header == header:
                message = self[i]
                del self[i]
        return message
