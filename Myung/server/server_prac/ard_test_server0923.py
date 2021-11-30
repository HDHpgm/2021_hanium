from bluepy import btle
import time
import tkinter as tk
from signal import signal, SIGPIPE, SIG_DFL

MAC = "D2:0E:20:DA:1B:31"
SERVICE_UUID = "19b10000-e8f2-537e-4f6c-d104768a1214" #블루투스 연결을 위한 번호
CHARACTERISTIC_UUID = "19b10001-e8f2-537e-4f6c-d104768a1214" #블루투스 연결하는 길을 위한 번호

nano = None

read_data = " "

print("hello")

print("conn to :" + MAC)
dev = btle.Peripheral(MAC) #맥주소에 해당하는 블루투스 연동
print("dev : ", dev)


print("Get Service by UUID: " + SERVICE_UUID)
service_uuid = btle.UUID(SERVICE_UUID)
service = dev.getServiceByUUID(service_uuid)
characteristics = dev.getCharacteristics()

for char in characteristics:
	print("----------")
	if(char.uuid == CHARACTERISTIC_UUID):
		print("=== UUID matched! ==")
		print(char)
		nano = char #블루투스가 알맞게 연동됐는지 확인 후 nano 변수에 저장


bmyung = b'\x00'
bytes_ON = b'\x04'
bytes_OFF = b'\x00'

if nano != None:
	print("\n nano found")
	print("nano : ", nano)
	while True:
		try:
			aa = nano.read()
			aa = aa.decode('utf-8')
			print(aa)
			#nano.write(bytes_ON, True) #블루투스를 통해 전송
			#bb = aa.decode('utf-8')

		except IOError as e:
			if e.errno == errno.EPIPE:
				signal(SIGPIPE, SIG_DFL)
			pass
				

		except Exception as e:
			print('에러 발생 : ', e)
			time.sleep(1)
			dev.disconnect()
			dev = btle.Peripheral(MAC)
			service_uuid = btle.UUID(SERVICE_UUID)
			service = dev.getServiceByUUID(service_uuid)
			characteristics = dev.getCharacteristics()
			for char in characteristics:
				print("----------")
				if(char.uuid == CHARACTERISTIC_UUID):
					print("=== UUID RE !~! matched! ==")
					print(char)
					nano = char #블루투스가 알맞게 연동됐는지 확인 후 nano 변수에 저장
			pass

		
else:
	print("\n nano NOT found!")

dev.disconnect()
print("\n ---bye---\n")
