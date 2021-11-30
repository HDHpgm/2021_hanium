package hello.hellospring.service;

import hello.hellospring.domain.Member;
import hello.hellospring.repository.MemberRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;

//@Service
public class MemberService { // 실제 비지니스 로직
    private final MemberRepository memberRepository; //저장소 객체 불러오기

    //@Autowired
    public MemberService(MemberRepository memberRepository) { //DI
        this.memberRepository = memberRepository;
    }


    /**
     * 회원가입
     */
    public Long join(Member member){
        //같은 이름이 있는 중복회원x
        validateDuplicateMember(member);

        memberRepository.save(member);
        return member.getId();
    }

    private void validateDuplicateMember(Member member) {
        memberRepository.findByName(member.getName())
                .ifPresent(m -> { //옵셔널이기 때문에 가능 / 널이 아닌 값이 오면 동작
                    throw new IllegalStateException("이미 존재하는 회원입니다.");
                });
    }

    /**
     *
     * 전체 회원 조회
     */
    public List<Member> findMembers(){
        return memberRepository.findAll();
    }

    public Optional<Member> findOne(Long memberId){
        return memberRepository.findById(memberId);
    }
}
