package com.robobo.rbb_springboot.controller;

import com.robobo.rbb_springboot.dto.MemoRequestDto;
import com.robobo.rbb_springboot.model.Memo;
import com.robobo.rbb_springboot.repository.MemoRepository;

import com.robobo.rbb_springboot.security.auth.PrincipalDetails;
import com.robobo.rbb_springboot.service.MemoService;
import lombok.RequiredArgsConstructor;
import org.springframework.data.domain.Sort;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.List;

@Controller
@RequiredArgsConstructor
public class MemoController {

    private final MemoRepository memoRepository;
    private final MemoService memoService;

    @GetMapping("/memo")
    public String memoPage(Model model, @AuthenticationPrincipal PrincipalDetails principalDetails) {
        if (principalDetails == null) {
            model.addAttribute("login",false);
        }
        else {

            model.addAttribute("login",true);
            model.addAttribute("username", principalDetails.getUsername());
            System.out.println(principalDetails.getUser());

        }
        return "timeline";
    }

    @GetMapping("api/memos/getUserName")
    public @ResponseBody String getUserName(@AuthenticationPrincipal PrincipalDetails principalDetails) {
        return principalDetails.getUsername();
    }

    @GetMapping("api/memos/getUserEmail")
    public @ResponseBody String getUserEmail(@AuthenticationPrincipal PrincipalDetails principalDetails) {
        return principalDetails.getUserEmail();
    }

    @PostMapping("/api/memos")
    public @ResponseBody Memo createMemo(@RequestBody MemoRequestDto requestDto) {
        Memo memo = new Memo(requestDto);
        return memoRepository.save(memo);
    }

    @GetMapping("/api/memos")
    public @ResponseBody List<Memo> readMemo() {
        return memoRepository.findAll(Sort.by(Sort.Direction.DESC, "modifiedAt"));
    }

    //최근 글
    @GetMapping("/api/lastmemo")
    public @ResponseBody Memo readLastMemo() {
        return memoRepository.findTop1ByOrderByIdDesc();
    }

    // 24시간 내 작업내역 등록 갯수
    @GetMapping("/api/numberoflastmemo")
    public @ResponseBody long numberOfLastMemo() {
        LocalDateTime startDatetime = LocalDateTime.of(LocalDate.now().minusDays(1), LocalTime.of(0,0,0));//어제
        LocalDateTime endDatetime = LocalDateTime.of(LocalDate.now(), LocalTime.of(23,59,59));
        return memoRepository.countByModifiedAtBetween(startDatetime, endDatetime);
    }

    @PutMapping("/api/memos/{id}")
    public @ResponseBody Long updateMemo(@PathVariable Long id, @RequestBody MemoRequestDto requestDto) {
        return memoService.update(id, requestDto);
    }

    @DeleteMapping("/api/memos/{id}")
    public @ResponseBody Long deleteMemo(@PathVariable Long id) {
        memoRepository.deleteById(id);
        return id;
    }

}
