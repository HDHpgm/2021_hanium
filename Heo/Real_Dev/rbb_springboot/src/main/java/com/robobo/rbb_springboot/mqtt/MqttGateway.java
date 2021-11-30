package com.robobo.rbb_springboot.mqtt;


import org.springframework.integration.annotation.MessagingGateway;

@MessagingGateway(defaultRequestChannel = "mqttOutboundChannel")
public interface MqttGateway {

	void senToMqtt(String data);
}
