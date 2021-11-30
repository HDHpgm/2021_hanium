package com.robobo.rbb_springboot.model;

import lombok.Builder;
import lombok.Getter;
import lombok.NoArgsConstructor;

import javax.persistence.*;


@Getter // get 함수를 일괄적으로 만들어줍니다.
@NoArgsConstructor // 기본 생성자를 만들어줍니다.
@Entity // DB 테이블 역할을 합니다.
public class User extends Timestamped {

    @Builder // 빌더 패턴 사용
    public User(String username, String password, String email, String tel, UserRole role, String provider, String providerId) {
        this.username = username;
        this.password = password;
        this.email = email;
        this.tel = tel;
        this.role = role;
        this.provider = provider;
        this.providerId = providerId;
    }


    // ID가 자동으로 생성 및 증가합니다.
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Id
    private Long id;

    // 반드시 값을 가지도록 합니다.
    @Column(nullable = false)
    private String username;

    @Column(nullable = false)
    private String password;

    @Column(nullable = false, unique = true)
    private String email;

    @Column(nullable = false)
    private String tel;

    @Column(nullable = false)
    @Enumerated(value = EnumType.STRING)
    private UserRole role;

    @Column
    private String provider;

    @Column
    private String providerId;

}
