package com.robobo.rbb_springboot.dto;

import lombok.Getter;

@Getter
public class MemoRequestDto {
    private String username;
    private String contents;
    private String userEmail;
}
