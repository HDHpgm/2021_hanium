package com.sparta.week01;

import com.sparta.week01.domain.Person;
import com.sparta.week01.domain.PersonRepository;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

import java.util.List;

@EnableJpaAuditing // 시간데이터 자동으로 넣어주기위해서 사용
@SpringBootApplication
public class Week01Application {

	public static void main(String[] args) {
		SpringApplication.run(Week01Application.class, args);
	}

}
