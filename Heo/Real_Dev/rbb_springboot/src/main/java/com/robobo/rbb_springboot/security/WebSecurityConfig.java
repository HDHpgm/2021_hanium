package com.robobo.rbb_springboot.security;


import com.robobo.rbb_springboot.security.oauth.PrincipalOauth2UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.method.configuration.EnableGlobalMethodSecurity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;


@EnableWebSecurity
@Configuration
@EnableGlobalMethodSecurity(securedEnabled = true)
public class WebSecurityConfig extends WebSecurityConfigurerAdapter {

    private final PrincipalOauth2UserService principalOauth2UserService;

    public WebSecurityConfig(PrincipalOauth2UserService principalOauth2UserService) {
        this.principalOauth2UserService = principalOauth2UserService;
    }


    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http.csrf().disable();
        http.headers().frameOptions().disable();

        http
                .authorizeRequests()
                // static 자원들을 login 없이 허용
//                    .antMatchers("/","/css/**","/js/**", "/scss/**",
//                            "/vendor/**","/img/**","/h2-console/**","/user/register").permitAll()
                    .antMatchers("/admin").access("hasRole('ROLE_ADMIN')")
//                    .antMatchers("/memo").access("hasRole('ROLE_USER') or hasRole('ROLE_ADMIN')")
                        .anyRequest().permitAll()
                        .and()
                    .formLogin()
                        .loginPage("/user/login") // 인증 안돼있으면 로그인페이지로 이동
                        //시큐리티가 로그인프로세스 낚아챔 컨트롤러에 안만들어도된다.
                        .loginProcessingUrl("/user/loginProcess")

                        //성공 시 "/" 이동
                        .defaultSuccessUrl("/")
                        .permitAll() // 모든 사용자에게 허용
                        .and()
                    .logout()
                        .logoutSuccessUrl("/")
                        .permitAll()
                        .and()
                    .exceptionHandling()
                        .accessDeniedPage("/user/forbidden") // 접근권한 없을 시 403 페이지 이동
                        .and()

                    .oauth2Login()
                        .loginPage("/user/login")
                        /* 구글 로그인 완료된 뒤 후처리 필요 (Tip. 로그인완료 후에 코드X , 엑세스토큰+사용자 프로필정보를 한방에 받음)
                         *  1.코드받기(인증)  2.엑세스토큰(사용자 정보 접근 권한)
                         *  3. 사용자 프로필 정보 가져오기
                         *  4. 정보를 토대로 회원가입 자동진행 or 이메일,전화번호,이름,아이디 외에 필요하다면(ex.쇼핑몰 이라면) -> (집주소, 등급 등) 추가 회원가입
                         */
                        .userInfoEndpoint()
                        .userService(principalOauth2UserService);
    }
}
