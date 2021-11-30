import time
import json
import serial
from multiprocessing import Process, Queue
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient


def conn_ard():
	port = "/dev/ttyACM1"
	ser = serial.Serial(port, 9600)
	ser.flushInput()
	return ser


def conn_aws(client_name):
	
	if client_name == "write_client":
		myMQTTClient = AWSIoTMQTTClient("tae_write")
	if client_name == "read_client":
		myMQTTClient = AWSIoTMQTTClient("tae_read")

	myMQTTClient.configureEndpoint("awts3jg7w6w7y-ats.iot.us-east-1.amazonaws.com", 8883)

	myMQTTClient.configureCredentials("/home/pi/ROBOBO/Myung/CLIENT_TEST/root-ca.pem", "/home/pi/ROBOBO/Myung/CLIENT_TEST/private.pem.key", "/home/pi/ROBOBO/Myung/CLIENT_TEST/certificate.pem.crt")

	myMQTTClient.configureOfflinePublishQueueing(-1)
	myMQTTClient.configureDrainingFrequency(2)
	myMQTTClient.configureConnectDisconnectTimeout(10)
	myMQTTClient.configureMQTTOperationTimeout(5)

	print("starting aws",client_name)
	myMQTTClient.connect()

	return myMQTTClient


def send_to_ard(ard_client):
	myAWS = conn_aws("write_client")
	global myARD
	myARD = ard_client
	
	while True:
		myAWS.subscribe("server/client", 1, recv_message_to_ard)


def recv_message_to_ard(self, topic, packet):
	ard_client = myARD
	come_message = json.loads(packet.payload)

	sensor_data = come_message['name_1']+come_message['finger_1']+come_message['name_2']+come_message['finger_2']+come_message['name_3']+come_message['finger_3']+"\n"
	
	sensor_data = str(sensor_data)

	ard_client.write(sensor_data.encode())




def call_subscribe():
	myAWS = conn_aws("read_client")
	while True:
		myAWS.subscribe("server/client", 1, recv_message_test) #read

def recv_message_test(self, topic, packet):
	come_message = json.loads(packet.payload)
	print("엄지 : ", come_message['finger_1'])
	print("검지 : ", come_message['finger_2'])
	print("약지 : ", come_message['finger_3'])

	#read #라즈베리파이에 메시지가 잘오는지 검사하기 위한 것

	


#--------------------------------------------aws_iot와 연결---------------------------------------------------------------------------------------------------------------------



#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

#my_talk = str(input("aws_iot에 전송할 말 입력 : "))


if __name__ == '__main__':

	ard_client = conn_ard()
	
	print("message from aws")
	procs = []

	proc1 = Process(target=send_to_ard, args=(ard_client,))
	procs.append(proc1)
	proc1.start()

	#proc2 = Process(target=call_subscribe)
	#procs.append(proc2)
	#proc2.start()

	#for proc in procs:
	#	proc.join()





