from bluetooth import *

socket = BluetoothSocket(RFCOMM)
socket.connect(("64:69:4E:8B:E7:5D", 1))
print("conn")

socket.send("i am ras")

while True:
	data = socket.recv(1024)
	print("RECEIVED : %S" %data)
