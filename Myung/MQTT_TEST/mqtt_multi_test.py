import time
import json
from multiprocessing import Process, Queue
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient


def conn_aws(client_name):
	if client_name == "write_client":
		myMQTTClient = AWSIoTMQTTClient("myung_write")
	if client_name == "read_client":
		myMQTTClient = AWSIoTMQTTClient("myung_read")


	myMQTTClient.configureEndpoint("awts3jg7w6w7y-ats.iot.us-east-1.amazonaws.com", 8883)

	myMQTTClient.configureCredentials("/home/pi/ROBOBO/Myung/MQTT_TEST/root-ca.pem", "/home/pi/ROBOBO/Myung/MQTT_TEST/private.pem.key", "/home/pi/ROBOBO/Myung/MQTT_TEST/certificate.pem.crt")

	myMQTTClient.configureOfflinePublishQueueing(-1)
	myMQTTClient.configureDrainingFrequency(2)
	myMQTTClient.configureConnectDisconnectTimeout(10)
	myMQTTClient.configureMQTTOperationTimeout(5)

	print("starting ",client_name)
	myMQTTClient.connect()

	return myMQTTClient


def send_message():
	test_num = 0
	myMQTTClient = conn_aws("write_client")

	json_message = '''{
		"message":"myung",
		"num":0
	}'''
	
	while True:
		myMQTTClient.publish(
			topic = "home/helloworld",		
			QoS = 1,
			payload = json_message
		) #write

		test_num = test_num + 1

		python_message = json.loads(json_message)
		python_message['num'] = test_num
		json_message = json.dumps(python_message)

		time.sleep(1)



def call_subscribe():
	myMQTTClient = conn_aws("read_client")
	while True:
		myMQTTClient.subscribe("home/helloworld/write", 1, recv_message) #read
		time.sleep(1)

def recv_message(self, topic, packet):
	come_message = json.loads(packet.payload)
	print("message : ", come_message['message'])
	print("test_num : ", come_message['num'])
	#read


#--------------------------------------------aws_iot와 연결---------------------------------------------------------------------------------------------------------------------



#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

#my_talk = str(input("aws_iot에 전송할 말 입력 : "))



if __name__ == '__main__':
	print("message from aws")
	procs = []

	proc1 = Process(target=send_message)
	procs.append(proc1)
	proc1.start()

	proc2 = Process(target=call_subscribe)
	procs.append(proc2)
	proc2.start()

	for proc in procs:
		proc.join()





