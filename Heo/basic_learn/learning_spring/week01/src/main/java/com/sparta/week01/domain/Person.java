package com.sparta.week01.domain;

import lombok.Getter;
import lombok.NoArgsConstructor;

import javax.persistence.*;

@Getter
@NoArgsConstructor
@Entity
public class Person extends Timestamped{

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO) // 자동 증가 명령입니다.
    private Long id;

    @Column(nullable = false)
    private String name;

    @Column(nullable = false)
    private String job;

    @Column(nullable = false)
    private int age;

    @Column(nullable = false)
    private String address;


    public Person(PersonRequestDto requestDto){
        this.name = requestDto.getName();
        this.job = requestDto.getJob();
        this.age = requestDto.getAge();
        this.address = requestDto.getAddress();
    }

    public void update(PersonRequestDto requestDto) {
        this.name = requestDto.getName();
        this.job = requestDto.getJob();
        this.age = requestDto.getAge();
        this.address = requestDto.getAddress();
    }


}
