import socket
import time

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostname(), 27015))

while True:
    s.send(bytes("Sending...", "utf-8"))
    time.sleep(0.10)