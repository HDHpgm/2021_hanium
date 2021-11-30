package com.robobo.rbb_springboot.repository;

import com.robobo.rbb_springboot.model.Memo;
import org.springframework.data.jpa.repository.JpaRepository;

import java.time.LocalDateTime;
import java.util.List;

public interface MemoRepository extends JpaRepository<Memo, Long> {
    List<Memo> findAllByModifiedAtBetweenOrderByModifiedAtDesc(LocalDateTime start, LocalDateTime end);
    Memo findTop1ByOrderByIdDesc();
    long countByModifiedAtBetween(LocalDateTime start, LocalDateTime end);
}