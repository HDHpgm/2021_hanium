function registerUser() {
    console.log($('#admin-check').is(':checked'))
    $.ajax({
        type: 'POST',
        url: '/user/register',
        contentType: "application/json",
        data: JSON.stringify({
            "username": $('#username').val(),
            "email": $('#email').val(),
            "password": $('#password').val(),
            "tel": $('#tel').val(),
            "admin": $('#admin-check').is(':checked'),
            "adminToken": $('#admin-token').val()
        }),
        success: function (response) {
            alert(response + " 님 사용자 등록이 완료됐습니다.");
            window.location.href='/user/login';
        },
        error: function (request, error) {
            var text = JSON.parse(request.responseText);
            alert(text.message);
            console.log("code:"+request.status+"\n"+ text);
        }
    })
}

