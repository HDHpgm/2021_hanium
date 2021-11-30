package com.sparta.week01.service;

import com.sparta.week01.domain.Person;
import com.sparta.week01.domain.PersonRepository;
import com.sparta.week01.domain.PersonRequestDto;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import javax.transaction.Transactional;

@RequiredArgsConstructor
@Service
public class PersonService {
    private final PersonRepository personRepository;

    @Transactional
    public Long update(Long id, PersonRequestDto requestDto){
        Person person = personRepository.findById(id).orElseThrow(
                () -> new IllegalArgumentException("id가 존재하지 않습니다."));
        person.update(requestDto);
        return person.getId();
    }
}
