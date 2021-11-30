
function hideall() {
    $("#tab").hide();
    $("#tab1").hide();
    $("#tab2").hide();
    $("#tab3").hide();
    $("#tab4").hide();
}

$(document).ready(function () {
    getLastMessages();
    numberOfLastMemo();
    $("#icon1").mouseover(function () {
        hideall();
        $("#tab1").css("display", "block");
    });

    $("#icon2").mouseover(function () {
        hideall();
        $("#tab2").css("display", "block");
    });

    $("#icon3").mouseover(function () {
        hideall();
        $("#tab3").css("display", "block");
    });

    hideall();
    $("#tab").show();

});

function getLastMessages() {
    // 1. 기존 메모 내용을 지웁니다.
    $('#cards-box').empty();
    // 2. 메모 목록을 불러와서 HTML로 붙입니다.
    $.ajax({
        type: 'GET',
        url: '/api/lastmemo',
        success: function (response) {
            let memo = response;
            let id = memo.id;
            let username = memo.username;
            let contents = memo.contents;
            let modifiedAt = memo.modifiedAt;
            let email = memo.userEmail;
            modifiedAt = modifiedAt.substring(0, 10) + " / " + modifiedAt.substring(11, 19);
            addHTML(id, username, contents, modifiedAt, email);
        }
    })
}

// 메모 하나를 HTML로 만들어서 body 태그 내 원하는 곳에 붙입니다.
function addHTML(id, username, contents, modifiedAt, email) {
    // 1. HTML 태그를 만듭니다.
    let temp_html = `<div class="card-timeline" style="width: 500px">
                                <!-- date/username 영역 -->
                                <div class="metadata">
                                    <div class="date_timeline">
                                        ${modifiedAt}
                                    </div>
                                    <div id="${id}-username" class="username">
                                        ${username}
                                    </div>
                                    <div id="${id}-useremail" class="username">
                                        ${email}
                                    </div>
                                </div>
                                <!-- contents 조회/수정 영역-->
                                <div class="contents">
                                    <div id="${id}-contents" class="text">
                                        ${contents}
                                    </div>
                                </div>
                                <!-- 안내 영역 -->
                                <div class="footer">
                                </div>
                            </div>
                            <p>전체 글 조회, 수정, 삭제는 메뉴의 타임라인 서비스를 통해 이용하세요!</p>`;
    // 2. #cards-box 에 HTML을 붙인다.
    $('#cards-box').append(temp_html);
}

// 오늘 하루 최신 작업내역 등록 갯수
function numberOfLastMemo() {
    $.ajax({
        type: 'GET',
        url: '/api/numberoflastmemo',
        success: function (response) {
            console.log(response);
            $('#timeline-num').text(response);
        }
    })
}