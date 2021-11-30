# 라즈베리파이 - 실시간 웹 스트리밍(MJPG-Streamer)

자세한 내용은 https://github.com/jacksonliam/mjpg-streamer.git 에서 확인할 수 있습니다.

## 1. 라즈베리파이에 웹캠 또는 파이 카메라를 연결하고 사용할 수 있도록 설정합니다. 
    
## 2. mjpg-streamer 소스 코드를 다운로드 받을 디렉토리를 만들어줍니다.

    mkdir project
    cd project
    
## 3. mjpg-streamer 소스 코드를 다운로드를 합니다

    git clone https://github.com/jacksonliam/mjpg-streamer.git

## 4. mjpg-streamer 소스 코드를 컴파일하기 위해 필요한 라이브러리를 설치합니다.

    sudo apt-get install cmake python-pil python3-pil libjpeg-dev build-essential

## 5. mjpg-streamer 소스 디렉토리로 이동하여 컴파일 및 설치를 진행합니다.

    cd mjpg-streamer/mjpg-streamer-experimental/
    make CMAKE_BUILD_TYPE=Debug
    sudo make install
    cd

## 6. /dev/video0에 연결된 파이카메라로부터 캡처한 영상을 HTTP 포트 8000으로 스트리밍하도록 합니다. 

    mjpg_streamer -i "input_raspicam.so" -o "output_http.so -p 800 -w /usr/local/share/mjpg-streamer/www/"

파이카메라가 아닌 웹캠을 사용할 시 input_raspicam.so 대신에 input_uvc.so를 사용해주면 된다.

## 7. 카메라로부터 영상을 캡쳐하여 웹으로 전송되는 상황을 볼 수 있습니다.

![6](/uploads/916566c37d18ed0d071e75a5695841f5/6.png)

## 8. PC의 웹브라우저로 라즈베리파이_아이피주소:8000에 접속합니다.

![5](/uploads/912cb5665c45fac964bb1e61872f5e80/5.png)

## 9. 왼쪽 메뉴에서 Stream을 선택하면 실시간 영상을 볼 수 있습니다.

![8](/uploads/d8bb05538dc4e9e679c81bb4f39b9834/8.png)

## 10. 웹에서 말고 바로 영상을 보고 싶다면 스냅사진
라즈베리파이_아이피주소:8000/?action=stream 

![9](/uploads/94f56c987f284f46e00338ac4fe7dfa8/9.png)




