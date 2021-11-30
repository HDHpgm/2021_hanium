import time
import json
import serial
from multiprocessing import Process, Queue

def test_p(ard):
	s1 = "m"
	s2 = "myun"
	s3 = "doha"
	ard_client = ard

	while 1:
		ard_client.write(s1.encode())
		time.sleep(1)
		print(s1)

def conn_ard():
	port = "/dev/ttyACM1"
	ser = serial.Serial(port, 9600)
	ser.flushInput()
	return ser


if __name__ == '__main__':

	ard_client = conn_ard()
	
	print("message from aws")
	procs = []

	proc1 = Process(target=test_p, args=(ard_client,))
	procs.append(proc1)
	proc1.start()

