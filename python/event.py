from threading import Event, Thread
import time


class c:
    @staticmethod
    def f(event: Event):
        print ("wait!")
        time.sleep(5)
        print ("done!")
        event.set()


e=Event()
print("starting the thread")
Thread(target=c.f, args=(e,)).start()
print("waiting")
e.wait()
print("it's over")


import uuid

print (uuid.uuid1())