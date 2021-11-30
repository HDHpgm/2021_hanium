package com.sparta.week01.domain;

import lombok.Getter;
import lombok.RequiredArgsConstructor;
import lombok.Setter;


@Setter
@Getter
@RequiredArgsConstructor //final 붙은 필드 자동으로 생성자 만들어줌
public class PersonRequestDto {
    private final String name;

    private final String job;

    private final int age;

    private final String address;
}
