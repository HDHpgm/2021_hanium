import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

def helloworld(self, params, packet):
	print("received message from aws")
	print("topic: " + packet.topic)
	print("payload: ", (packet.payload))

myMQTTClient = AWSIoTMQTTClient("myung")
myMQTTClient.configureEndpoint("awts3jg7w6w7y-ats.iot.us-east-1.amazonaws.com", 8883)

myMQTTClient.configureCredentials("/home/pi/ROBOBO/Myung/MQTT_TEST/root-ca.pem", "/home/pi/ROBOBO/Myung/MQTT_TEST/private.pem.key", "/home/pi/ROBOBO/Myung/MQTT_TEST/certificate.pem.crt")

myMQTTClient.configureOfflinePublishQueueing(-1)
myMQTTClient.configureDrainingFrequency(2)
myMQTTClient.configureConnectDisconnectTimeout(10)
myMQTTClient.configureMQTTOperationTimeout(5)

print("starting....")
myMQTTClient.connect()


#while True:
#	time.sleep(3)



print("message from aws")

while True:
	myMQTTClient.publish(
		topic = "home/helloworld",
		QoS = 1,
		payload = "myung hello"
	)

	myMQTTClient.subscribe("home/helloworld", 1, helloworld)
	time.sleep(2)
