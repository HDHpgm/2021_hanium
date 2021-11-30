package hello.hellospring.service;

import hello.hellospring.domain.Member;
import static org.assertj.core.api.Assertions.*;

import hello.hellospring.repository.MemoryMemberRepository;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class MemberServiceTest {
    MemberService memberService;
    MemoryMemberRepository memoryMemberRepository;

    @BeforeEach
    public void beforeEach(){ //DI
        memoryMemberRepository = new MemoryMemberRepository();
        memberService = new MemberService(memoryMemberRepository);
    }
    @AfterEach
    public void afterEach(){
        memoryMemberRepository.clearStore();
    }

    @Test
    void join() {
        // given
        Member member = new Member();
        member.setName("hello");
        // when
        Long saveId = memberService.join(member); // join 리턴값이 id임

        // then
        Member findMember = memberService.findOne(saveId).get(); //저장소에서 savdId의 member 객체 가져오기
        assertThat(member.getName()).isEqualTo(findMember.getName()); // 비교
    }
    @Test
    public void 중복_회원_예외(){
        //given
        Member member1 = new Member();
        member1.setName("spring");
        Member member2 = new Member();
        member2.setName("spring");


        //when
        memberService.join(member1);
        IllegalStateException e = assertThrows(IllegalStateException.class, () -> memberService.join(member2));
        assertThat(e.getMessage()).isEqualTo("이미 존재하는 회원입니다.");
//        try {
//            memberService.join(member2);
//            fail();
//        }catch (IllegalStateException e){
//            assertThat(e.getMessage()).isEqualTo("이미 존재하는 회원입니다.");
//        }


        //then
    }

    @Test
    void findMembers() {

    }

    @Test
    void findOne() {

    }
}