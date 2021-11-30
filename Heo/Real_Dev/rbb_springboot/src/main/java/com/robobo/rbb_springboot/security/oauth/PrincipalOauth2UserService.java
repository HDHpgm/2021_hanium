package com.robobo.rbb_springboot.security.oauth;

import com.nimbusds.openid.connect.sdk.claims.UserInfo;
import com.robobo.rbb_springboot.model.User;
import com.robobo.rbb_springboot.model.UserRole;
import com.robobo.rbb_springboot.repository.UserRepository;
import com.robobo.rbb_springboot.security.auth.PrincipalDetails;
import com.robobo.rbb_springboot.security.oauth.provider.FacebookUserInfo;
import com.robobo.rbb_springboot.security.oauth.provider.GoogleUserInfo;
import com.robobo.rbb_springboot.security.oauth.provider.OAuth2UserInfo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Lazy;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.oauth2.client.userinfo.DefaultOAuth2UserService;
import org.springframework.security.oauth2.client.userinfo.OAuth2UserRequest;
import org.springframework.security.oauth2.core.OAuth2AuthenticationException;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.stereotype.Service;


@Service
public class PrincipalOauth2UserService extends DefaultOAuth2UserService {


    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;


    public PrincipalOauth2UserService(UserRepository userRepository, PasswordEncoder passwordEncoder) {
        this.userRepository = userRepository;
        this.passwordEncoder = passwordEncoder;
    }


    // google 에서 받은 userRequest 데이터에 대한 후처리 함수
    @Override
    public OAuth2User loadUser(OAuth2UserRequest userRequest) throws OAuth2AuthenticationException {

        System.out.println("getClientRegistration:" + userRequest.getClientRegistration());
        System.out.println("getAccessToken:" + userRequest.getAccessToken().getTokenValue());

        OAuth2User oAuth2User = super.loadUser(userRequest);
        // 구글로그인 -> 로그인완료 -> code를 리턴(OAuth-client 라이브러리) -> Access Token 요청
        // userRequest 정보 -> loadUser 함수 호출 -> 구글로부터 회원 프로필 받아준다.
        System.out.println("getAttribute:" + oAuth2User.getAttributes());



        //회원가입을 강제로(자동으로) 진행
        OAuth2UserInfo oAuth2UserInfo = null;
        if (userRequest.getClientRegistration().getRegistrationId().equals("google")){
            System.out.println("구글 로그인 요청");
            oAuth2UserInfo = new GoogleUserInfo(oAuth2User.getAttributes());
        }
        else if (userRequest.getClientRegistration().getRegistrationId().equals("facebook")) {
            System.out.println("페이스북 로그인 요청");
            oAuth2UserInfo = new FacebookUserInfo(oAuth2User.getAttributes());
        }
        else {
            System.out.println("구글과 페이스북만 지원합니다.");
        }

        String provider = oAuth2UserInfo.getProvider(); // google
        String providerId = oAuth2UserInfo.getProviderId(); // 1023231~~~
        String username = oAuth2UserInfo.getName();
        String password = passwordEncoder.encode("비밀번호 어차피 여기선 필요없음");

        String email = oAuth2UserInfo.getEmail();
        UserRole role = UserRole.ROLE_USER;

        User user = userRepository.findByEmail(email).orElse(null);

        if (user == null) {
            user = User.builder()
                    .username(username)
                    .password(password)
                    .email(email)
                    .tel(provider) // 소셜 로그인 시에는 번호 없이 provider 를 넣어주기로 함
                    .role(role)
                    .provider(provider)
                    .providerId(providerId)
                    .build();
            userRepository.save(user);
        }

        // PrincipalDetails -> OAuth 생성자 -> 생성해서 Authentication 객체에 전달
        return new PrincipalDetails(user, oAuth2User.getAttributes());
    }
}
