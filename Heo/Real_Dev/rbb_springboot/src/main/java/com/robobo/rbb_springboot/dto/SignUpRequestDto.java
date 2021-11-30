package com.robobo.rbb_springboot.dto;

import lombok.Getter;
import lombok.Setter;

@Setter
@Getter
public class SignUpRequestDto {
    private String username;
    private String email;
    private String password;
    private String tel;
    private boolean admin = false;
    private String adminToken = "";
}
