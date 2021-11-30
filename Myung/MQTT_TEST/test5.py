import time
import json
import serial
from bluetooth import *
from multiprocessing import Process, Queue
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
def conn_ard():
	port = "/dev/ttyUSB0"
	ser = serial.Serial(port, 9600)
	ser.flushInput()
	return ser

def test_p(ard):
	arduino = ard
	while 1:
		ard_data = arduino.readline()
	
		ard_data_str = ard_data.decode()
		print(ard_data_str)



if __name__ == '__main__':

	client_ard = conn_ard()
	print("connect arduino")

	procs = []

	proc1 = Process(target=test_p, args=(client_ard,))
	procs.append(proc1)
	proc1.start()

	#proc2 = Process(target=call_subscribe)
	#procs.append(proc2)
	#proc2.start()

	
	
	#aws와 통신할 함수를 멀티프로세스로 구동

	#for result in th:
	#	reasult.join()
