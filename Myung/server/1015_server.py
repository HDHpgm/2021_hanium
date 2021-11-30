import time
import json
import serial
from bluetooth import *
from multiprocessing import Process, Queue
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

from bluepy import btle
from signal import signal, SIGPIPE, SIG_DFL
import tkinter as tk

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CHARACTERISTIC_UUID = [0,0,0,0,0]
nano = [0,0,0,0,0]

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

def send_message():

	MAC = "D2:0E:20:DA:1B:31"
	SERVICE_UUID = "19b10000-e8f2-537e-4f6c-d104768a1214" #블루투스 연결을 위한 번호
	CHARACTERISTIC_UUID[0] = "19b10001-e8f2-537e-4f6c-d104768a1111" #블루투스 연결하는 길을 위한 번호
	CHARACTERISTIC_UUID[1] = "19b10001-e8f2-537e-4f6c-d104768a2222" #블루투스 연결하는 길을 위한 번호
	CHARACTERISTIC_UUID[2] = "19b10001-e8f2-537e-4f6c-d104768a3333" #블루투스 연결하는 길을 위한 번호
	CHARACTERISTIC_UUID[3] = "19b10001-e8f2-537e-4f6c-d104768a4444" #블루투스 연결하는 길을 위한 번호
	CHARACTERISTIC_UUID[4] = "19b10001-e8f2-537e-4f6c-d104768a5555" #블루투스 연결하는 길을 위한 번호

	print("아두이노 연결시도중")
	dev = btle.Peripheral(MAC) #맥주소에 해당하는 블루투스 연동

	service_uuid = btle.UUID(SERVICE_UUID)
	service = dev.getServiceByUUID(service_uuid)
	characteristics = dev.getCharacteristics()


	i=0
	for char in characteristics:
		print("Connect Checking ...")
		if(char.uuid == CHARACTERISTIC_UUID[i]):
			print("=== UUID matched! ==")
			print(char)
			nano[i] = char #블루투스가 알맞게 연동됐는지 확인 후 nano 변수에 저장
			i=i+1
#------------------------------------------------------------------------------------------------------

	myMQTTClient = conn_aws("write_client")

	sensor_num = [0,0,0,0,0]
	sensor_talk = [0,0,0,0,0]


	fin_message1 = '''{
		"finger":0, "name":"엄지"
		
	}'''

	fin_message2 = '''{
		"finger":0, "name":"검지"
		
	}'''

	fin_message3 = '''{
		"finger":0, "name":"중지"
		
	}'''

	fin_message4 = '''{
		"finger":0, "name":"약지"
		
	}'''

	fin_message5 = '''{
		"finger":0, "name":"새끼"
		
	}'''
	#aws 로 보내는 말을 json 형태로 저장
	
	while True:
		try:
			ard_t1 = nano[0].read()
			ard_t2 = nano[1].read()
			ard_t3 = nano[2].read()
			ard_t4 = nano[3].read()
			ard_t5 = nano[4].read()

			ard_data1 = int(ard_t1)
			ard_data2 = int(ard_t2)
			ard_data3 = int(ard_t3)
			ard_data4 = int(ard_t4)
			ard_data5 = int(ard_t5)

			sensor_num[0] = 1
			sensor_talk[0] = ard_data1 #엄지 데이터

			sensor_num[1] = 2
			sensor_talk[1] = ard_data2 #검지 데이터

			sensor_num[2] = 3
			sensor_talk[2] = ard_data3 #중지 데이터

			sensor_num[3] = 4
			sensor_talk[3] = ard_data4 #약지 데이터

			sensor_num[4] = 5
			sensor_talk[4] = ard_data5 #새끼 데이터

		
			python_message1 = json.loads(fin_message1)
			python_message2 = json.loads(fin_message2)
			python_message3 = json.loads(fin_message3)
			python_message4 = json.loads(fin_message4)
			python_message5 = json.loads(fin_message5)
			
			python_message1['finger'] = sensor_talk[0]
			python_message2['finger'] = sensor_talk[1]
			python_message3['finger'] = sensor_talk[2]
			python_message4['finger'] = sensor_talk[3]
			python_message5['finger'] = sensor_talk[4]


			fin_message1 = json.dumps(python_message1)
			fin_message2 = json.dumps(python_message2)
			fin_message3 = json.dumps(python_message3)
			fin_message4 = json.dumps(python_message4)
			fin_message5 = json.dumps(python_message5)
			#위에서 다시 publish를 해야하기 때문에 최신화된 메시지를 json형식으로 파싱

			myMQTTClient.publish(
				topic = "server/write/0",		
				QoS = 1,
				payload = fin_message1
			)

			myMQTTClient.publish(
				topic = "server/write/1",		
				QoS = 1,
				payload = fin_message2
			)

			myMQTTClient.publish(
				topic = "server/write/2",		
				QoS = 1,
				payload = fin_message3
			)

			myMQTTClient.publish(
				topic = "server/write/3",		
				QoS = 1,
				payload = fin_message4
			)

			myMQTTClient.publish(
				topic = "server/write/4",		
				QoS = 1,
				payload = fin_message5
			)


			#publish 메소드를 이용해 aws로 전송
			#topic : 내가 지정한 채팅방
			#QoS : 메세지 송수신 체크 방식 (1은 메시지를 보내고, 받았는지 체크까지 하고 다음메시지를 보낸다.)
			#paylod : 내가 보낼 메시지 (json 형식으로 보내야 aws 내에서 활용 가능)




		except Exception as e:
			print('에러발생 : ', e)
			time.sleep(1)
			dev.disconnect()
			dev = btle.Peripheral(MAC)
			service_uuid = btle.UUID(SERVICE_UUID)
			service = dev.getServiceByUUID(service_uuid)
			characteristics = dev.getCharacteristics()
			i=0
			for char in characteristics:
				print("Connect Checking ...")
				if(char.uuid == CHARACTERISTIC_UUID[i]):
					print("=== UUID matched! ==")
					print(char)
					nano[i] = char #블루투스가 알맞게 연동됐는지 확인 후 nano 변수에 저장
					i=i+1
			pass
			
		
		

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# aws -> 라즈베리파이로 메시지 읽어오기

def call_subscribe():
	myMQTTClient = conn_aws("read_client")
	while True:
		myMQTTClient.subscribe("server/read", 1, recv_message) #read
	#subscribe 메소드를 이용해 메시지를 읽어올 (채팅방 지정, QoS 설정, 이용할 함수) 지정


def recv_message(self, topic, packet):
	come_message = json.loads(packet.payload)
	print("엄지", " : ", come_message['data'][0]['finger'])
	print("검지", " : ", come_message['data'][1]['finger'])
	print("중지", " : ", come_message['data'][2]['finger'])
	print("약지", " : ", come_message['data'][3]['finger'])
	print("새끼", " : ", come_message['data'][4]['finger'])
	print("--------------------------------------------------------------------")
	#위의 subscribe 메소드에 의해 실행될 함수
	#topic에는 지정된 채팅방 이름이 저장됌
	#packet에는 aws 오는 메시지가 json 형식으로 저장됌
	#json.loads()를 이용해 json 메시지를 python 형식으로 파싱

#-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# 소스코드 시작부분



if __name__ == '__main__':

	#client_ard, dev = conn_ard()
	#print("아두이노 블루투스 반환 완료")

	procs = []

	#proc1 = Process(target=send_message)
	#procs.append(proc1)
	#proc1.start()
	send_message()

	#proc2 = Process(target=call_subscribe)
	#procs.append(proc2)
	#proc2.start()

	
	
	#aws와 통신할 함수를 멀티프로세스로 구동

	#for result in th:
	#	reasult.join()






