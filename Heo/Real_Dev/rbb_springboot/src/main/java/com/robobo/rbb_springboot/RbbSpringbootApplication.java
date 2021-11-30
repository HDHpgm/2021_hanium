package com.robobo.rbb_springboot;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;
import org.springframework.context.annotation.Bean;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;

@SpringBootApplication(scanBasePackages = {"com.robobo.rbb_springboot"})
@EnableJpaAuditing
@ServletComponentScan
public class RbbSpringbootApplication {
    // 패스워드 암호화 하기위해 Bean 등록 (DI 사용)
    @Bean
    public PasswordEncoder encodePassword() {
        return new BCryptPasswordEncoder();
    }

    public static void main(String[] args) {
        SpringApplication.run(RbbSpringbootApplication.class, args);
    }

}
