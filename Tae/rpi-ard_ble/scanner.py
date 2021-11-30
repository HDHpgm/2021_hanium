from bluepy import btle
import time
import tkinter as tk

#Have to match with Peripheral
MAC = "24:62:AB:FE:0B:12"    #연결할 디바이스의 맥 주소
SERVICE_UUID = "19b10000-e8f2-537e-4f6c-d104768a1214"
CHARACTERISTIC_UUID = "19b10001-e8f2-537e-4f6c-d104768a1214"

nano = None

print("Hello")

print("Connect to:" + MAC)
dev = btle.Peripheral(MAC)
print("\n--- dev ----------------------------")
print(type(dev))
print(dev)

print("\n--- dev.services -------------------")
for svc in dev.services:
    print(str(svc))
    
print("\n------------------------------------")
print("Get Serice By UUID: " + SERVICE_UUID)
service_uuid = btle.UUID(SERVICE_UUID)
service = dev.getServiceByUUID(service_uuid)

print(service)
print("\n--- service.getCharacteristics() ---")
print(type(service.getCharacteristics()))
print(service.getCharacteristics())

#------------------------------------------------
characteristics = dev.getCharacteristics()     
print("\n--- dev.getCharacteristics() -------")
print(type(characteristics))
print(characteristics)
    
for char in characteristics:
    print("----------")
    print(type(char))
    print(char)
    print(char.uuid)
    #라즈베리파이와 ble 디바이스의 CHARACTERISTIC_UUID가 일치하는지 확인
    if(char.uuid == CHARACTERISTIC_UUID ):
        print("=== !CHARACTERISTIC_UUID matched! ==")
        nano = char
        print(char)
        print(dir(char))
        print(char.read())
        
bytes_ON = b'\x01'  #1
bytes_OFF = b'\x00' #0

if nano != None:
    print("\n nano found")
    print(nano)
    while True:
     nano.write(bytes_ON, True) # 1 이라는 값을 peripheral ble 디바이스 보냄
else:
    print("\n nano NOT found!")
#=============================================
dev.disconnect() #연결 해제
print("\n--- bye ---\n")
