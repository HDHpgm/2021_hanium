import time
import json
import serial
from bluetooth import *
from multiprocessing import Process, Queue
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

def conn_ard():
	port = "/dev/ttyACM0"
	ser = serial.Serial(port, 9600)
	ser.flushInput()
	return ser

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# aws와 연결을 위한 함수 선언

def conn_aws(client_name):
	if client_name == "write_client":
		myMQTTClient = AWSIoTMQTTClient("myung_write")
	if client_name == "read_client":
		myMQTTClient = AWSIoTMQTTClient("myung_read")
	#멀티프로세스 별 접속 닉네임 설정

	myMQTTClient.configureEndpoint("awts3jg7w6w7y-ats.iot.us-east-1.amazonaws.com", 8883)
	#aws 목적지 주소 설정 

	myMQTTClient.configureCredentials("/home/pi/ROBOBO/Myung/MQTT_TEST/root-ca.pem", "/home/pi/ROBOBO/Myung/MQTT_TEST/private.pem.key", "/home/pi/ROBOBO/Myung/MQTT_TEST/certificate.pem.crt") 
	#라즈베리파이 접속용 인증서

	myMQTTClient.configureOfflinePublishQueueing(-1)
	myMQTTClient.configureDrainingFrequency(2)
	myMQTTClient.configureConnectDisconnectTimeout(10)
	myMQTTClient.configureMQTTOperationTimeout(5)
	#접속대기시간 등 설정

	print("starting aws ", client_name)
	myMQTTClient.connect()
	#어떤 프로세스(읽기용 프로세스 / 쓰기용 프로세스)로 접속했는지 알려주고 연결

	return myMQTTClient
	#프로세스 정보 반환


#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# 라즈베리파이 -> aws로 메시지 보내는 함수 선언 / 멀티프로세스 write_client라고 이름정의

def send_message(client_ard):
	test_num = "hello"
	myMQTTClient = conn_aws("write_client")
	arduino = client_ard

	json_message = '''{
		"finger_1":0,
		"name_1":"엄지",
		"finger_2":0,
		"name_2":"검지",
		"finger_3":0,
		"name_3":"중지",
		"finger_4":0,
		"name_4":"약지",
		"finger_5":0,
		"name_5":"새끼"
	}'''
	#aws 로 보내는 말을 json 형태로 저장
	
	while True:
		ard_data = arduino.readline()
		
		ard_data_str = ard_data.decode()
		sensor_num1 = ard_data_str[0:1]
		sensor_talk1 = ard_data_str[1:4] #엄지 데이터

		sensor_num2 = ard_data_str[5:6]
		sensor_talk2 = ard_data_str[6:9] #검지 데이터

		sensor_num3 = ard_data_str[10:11]
		sensor_talk3 = ard_data_str[11:14] #중지 데이터

		sensor_num4 = ard_data_str[15:16]
		sensor_talk4 = ard_data_str[16:19] #약지 데이터

		sensor_num5 = ard_data_str[20:21]
		sensor_talk5 = ard_data_str[21:24] #새끼 데이터


		python_message = json.loads(json_message)
		python_message['finger_1'] = sensor_talk1
		python_message['finger_2'] = sensor_talk2
		python_message['finger_3'] = sensor_talk3
		python_message['finger_4'] = sensor_talk4
		python_message['finger_5'] = sensor_talk5

		python_message['name_1'] = sensor_num1
		python_message['name_2'] = sensor_num2
		python_message['name_3'] = sensor_num3
		python_message['name_4'] = sensor_num4
		python_message['name_5'] = sensor_num5
		#파이썬에서 json 형식의 메시지를 바꾸기위해 json 형식을 python 형식으로 파싱하고 수정

		myMQTTClient.publish(
			topic = "server/write",		
			QoS = 1,
			payload = json_message
		) 
		#publish 메소드를 이용해 aws로 전송
		#topic : 내가 지정한 채팅방
		#QoS : 메세지 송수신 체크 방식 (1은 메시지를 보내고, 받았는지 체크까지 하고 다음메시지를 보낸다.)
		#paylod : 내가 보낼 메시지 (json 형식으로 보내야 aws 내에서 활용 가능)


		json_message = json.dumps(python_message)
		#위에서 다시 publish를 해야하기 때문에 최신화된 메시지를 json형식으로 파싱
		
		

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# aws -> 라즈베리파이로 메시지 읽어오기

def call_subscribe():
	myMQTTClient = conn_aws("read_client")
	while True:
		myMQTTClient.subscribe("server/read", 1, recv_message) #read
	#subscribe 메소드를 이용해 메시지를 읽어올 (채팅방 지정, QoS 설정, 이용할 함수) 지정


def recv_message(self, topic, packet):
	come_message = json.loads(packet.payload)
	print("엄지", " : ", come_message['finger_1'])
	print("검지", " : ", come_message['finger_2'])
	print("중지", " : ", come_message['finger_3'])
	print("약지", " : ", come_message['finger_4'])
	print("새끼", " : ", come_message['finger_5'])
	print("--------------------------------------------------------------------")
	#위의 subscribe 메소드에 의해 실행될 함수
	#topic에는 지정된 채팅방 이름이 저장됌
	#packet에는 aws 오는 메시지가 json 형식으로 저장됌
	#json.loads()를 이용해 json 메시지를 python 형식으로 파싱

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# 소스코드 시작부분



if __name__ == '__main__':

	client_ard = conn_ard()
	print("connect arduino")

	procs = []

	proc1 = Process(target=send_message, args=(client_ard, ))
	procs.append(proc1)
	proc1.start()

	proc2 = Process(target=call_subscribe)
	procs.append(proc2)
	proc2.start()

	
	
	#aws와 통신할 함수를 멀티프로세스로 구동

	#for result in th:
	#	reasult.join()






