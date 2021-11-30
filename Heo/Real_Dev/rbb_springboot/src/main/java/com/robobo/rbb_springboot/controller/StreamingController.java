package com.robobo.rbb_springboot.controller;

import com.robobo.rbb_springboot.security.auth.PrincipalDetails;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class StreamingController {
    @GetMapping("/streaming")
    public String streaming(Model model, @AuthenticationPrincipal PrincipalDetails principalDetails) {
        if (principalDetails == null) {
            model.addAttribute("login",false);
        }
        else {

            model.addAttribute("login",true);
            model.addAttribute("username", principalDetails.getUsername());
            System.out.println(principalDetails.getUser());

        }
        return "streaming";
    }
}
