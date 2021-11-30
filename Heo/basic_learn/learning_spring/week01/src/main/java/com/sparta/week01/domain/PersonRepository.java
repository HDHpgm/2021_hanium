package com.sparta.week01.domain;

import org.springframework.data.jpa.repository.JpaRepository;

public interface PersonRepository extends JpaRepository<Person, Long> {
}
// jpaRepository 를 상속받아 JPA 를 통해서 Person 테이블에 대해 id로 crud 가능하게 해줌
