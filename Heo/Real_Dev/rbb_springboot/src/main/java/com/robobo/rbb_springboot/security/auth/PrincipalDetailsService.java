package com.robobo.rbb_springboot.security.auth;

import com.robobo.rbb_springboot.model.User;
import com.robobo.rbb_springboot.repository.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;


// 시큐리티 설정에서 loginProcessingUrl()
// login 요청이 오면 자동으로 UserDetailsService 타입으로 IoC 되어있는
// loadUserByUsername 함수가 실행됨 (규칙임)

@Service
public class PrincipalDetailsService implements UserDetailsService {

    private final UserRepository userRepository;


    @Autowired
    public PrincipalDetailsService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }

    // 매개변수 받는 username 잘 맞춰줘야함
    // 시큐리티 session (내부 Authentication (내부 UserDetails))
    /**
     * Spring Security 필수 메소드 구현
     *
     * param email 이메일
     * return UserDetails
     * throws UsernameNotFoundException 유저가 없을 때 예외 발생
     */
    @Override
    public UserDetails loadUserByUsername(String email) throws UsernameNotFoundException {
        User user = userRepository.findByEmail(email).orElse(null);

        // null 값이 아니면 실행
        if(user != null) {
            return new PrincipalDetails(user);
        }
        return null;
    }
}
