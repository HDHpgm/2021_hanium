package hello.hellospring;

import hello.hellospring.repository.MemberRepository;
import hello.hellospring.repository.MemoryMemberRepository;
import hello.hellospring.service.MemberService;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
//직접 자바 빈으로 등록하는 방법
@Configuration
public class SpringConfig {

    @Bean
    public MemberService memberService(){
        return new MemberService(memberRepository());  //아래 memberRepository 가져와서 넣음
    }


    @Bean
    public MemberRepository memberRepository() {
        return new MemoryMemberRepository();
    }
}
