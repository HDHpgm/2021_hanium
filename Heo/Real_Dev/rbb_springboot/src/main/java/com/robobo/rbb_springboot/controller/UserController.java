package com.robobo.rbb_springboot.controller;

import com.robobo.rbb_springboot.dto.SignUpRequestDto;
import com.robobo.rbb_springboot.service.UserService;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.ResponseBody;



@Controller
public class UserController {

    private final UserService userService;

    @Autowired
    public UserController(UserService userService) {
        this.userService = userService;
    }

    //로그인 페이지
    @GetMapping("/user/login")
    public String loginPage() {
        return "login";
    }

    //회원가입 페이지
    @GetMapping("/user/register")
    public String registerPage() {
        return "register";
    }

    //회원가입 요청 처리
    @PostMapping("/user/register")
    @ResponseBody
    public String registerProc(@RequestBody SignUpRequestDto requestDto) {
        userService.registerUser(requestDto);
        return requestDto.getUsername();
    }

    @GetMapping("/user/forbidden")
    public String forbidden() {
        return "forbidden";
    }
}
