package com.robobo.rbb_springboot.controller;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import com.robobo.rbb_springboot.mqtt.MqttGateway;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;


@RestController
public class MqttController {
	@SuppressWarnings("SpringJavaInjectionPointsAutowiringInspection")
	@Autowired
	MqttGateway mqtGateway;


	@PostMapping("/sendPowerOn1")
	public String publish1(@RequestBody String mqttMessage){

		try {
	//		System.out.println(mqttMessage);
	//		JsonObject convertObject = new Gson().fromJson(mqttMessage, JsonObject.class);
			mqtGateway.senToMqtt("1");
			return "전원 켜짐";
		}catch(Exception ex) {
			System.out.println("실패 "+ex);
			return "실패라빠빠..";
		}
	}

	@PostMapping("/sendPowerOn2")
	public String publish2(@RequestBody String mqttMessage){

		try {
			//		System.out.println(mqttMessage);
			//		JsonObject convertObject = new Gson().fromJson(mqttMessage, JsonObject.class);
			mqtGateway.senToMqtt("2");
			return "전원 켜짐";
		}catch(Exception ex) {
			System.out.println("실패 "+ex);
			return "실패라빠빠..";
		}
	}

}
