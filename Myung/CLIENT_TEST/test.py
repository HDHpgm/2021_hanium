import serial
import time

port = "/dev/ttyACM0"
ser = serial.Serial(port, 9600)
ser.flushInput()
talk = "hello"

while True:
	ser.write(talk.encode())
	print(talk.encode())
	time.sleep(3)
