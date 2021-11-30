package com.robobo.rbb_springboot.service;

import com.robobo.rbb_springboot.dto.MemoRequestDto;
import com.robobo.rbb_springboot.model.Memo;
import com.robobo.rbb_springboot.repository.MemoRepository;

import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import javax.transaction.Transactional;

@RequiredArgsConstructor
@Service
public class MemoService {

    private final MemoRepository memoRepository;

    @Transactional
    public Long update(Long id, MemoRequestDto requestDto) {
        Memo memo = memoRepository.findById(id).orElseThrow(
                () -> new IllegalArgumentException("아이디가 존재하지 않습니다.")
        );
        memo.update(requestDto);
        return memo.getId();
    }
}