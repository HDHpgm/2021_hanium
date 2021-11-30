package hello.hellospring.controller;

import hello.hellospring.HelloSpringApplication;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class HelloController {
    @GetMapping("hello")
    public String hello(Model model){
        model.addAttribute("data","hello!!!!");
        return "hello";
    }

    @GetMapping("hello-mvc")
    public String helloMvc(@RequestParam("name") String name, Model model){
        model.addAttribute("name", name);
        return "hello-template";
    }

    @GetMapping("hello-string")
    @ResponseBody
    public String helloString(@RequestParam("name") String name) {
        return "hello " + name; // "hello spring"   뷰가 없는 api 방식 / 데이터 그대로 내려줌
    }

    @GetMapping("hello-api") //JSON 형식으로 나옴
    @ResponseBody // 응답에 데이터 그대로 넘김(객체) -> HttpMessageConverter = 디폴트 jsonConverter / 단순 문자형이면 StringConverter
    public Hello helloApi(@RequestParam("name")String name){
        Hello hello = new Hello();
        hello.setName(name);
        return hello; // 객체 반환
    }
    static class Hello{ // 자바 빈 표준 방식, 프로퍼티 접근 방식
        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        private String name;

    }

}
