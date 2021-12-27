class HttpRequest:
    def __init__(self):
        self.verb = ""
        self.url = ""
        self.version = ""
        self.headers = []
        self.body = ""

class HttpResponse:
    def __init__(self):
        self.headers = []
        self.body = ""
        self.code = 0

class HttpRequestProcessor:
    def __init__(self, content:str):
        self.content = content
        self.request = HttpRequest()
        lines = content.replace('\r', '').split('\n')
        self.request.verb = lines[0].split(' ')[0]
        self.request.url = lines[0].split(' ')[1]
        self.request.version = lines[0].split(' ')[2]
        for header_str in lines[1:]:
            if header_str:
                self.request.headers.append(header_str)


