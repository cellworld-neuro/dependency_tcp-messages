from .util import check_type
from json_cpp import JsonObject, JsonList


class Message(JsonObject):

    def __init__(self, header="", body=""):
        self.header = header
        self.body = str(body)

    def get_body(self, body_type=None):
        if body_type:
            check_type(body_type, type, "wrong type for body_body")
            if issubclass(body_type, JsonObject) or issubclass(body_type, JsonList):
                return body_type.parse(self.body)
        else:
            return JsonObject.load(self.body)

    def set_body(self, v):
        self.body = str(v)
