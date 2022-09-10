import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((socket.gethostname(), 1238))
s.listen(6)

while True:
    try:
        # print("hello")
        clientsocket, address = s.accept()
        print(f"Connection from {address} has been established.")
        clientsocket.send(bytes("Welcome to the server.", "utf-8"))
        clientsocket.close()
    except KeyboardInterrupt:
        print("Interrupted...", end=" ")
        clientsocket.close()
        print("socket closed.")
        break