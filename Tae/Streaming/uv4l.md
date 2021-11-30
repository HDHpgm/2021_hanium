# 라즈베리파이 - 실시간 웹 스트리밍(UV4L)

자세한 설명은 https://www.linux-projects.org/uv4l/installation/ 에 들어가면 확인할 수 있다.

## 1. 인증키 설치

    $ curl https://www.linux-projects.org/listing/uv4l_repo/lpkey.asc | sudo apt-key add -
    
## 2. /etc/apt/sources.list 파일에 다음 줄을 추가합니다 .

    deb https://www.linux-projects.org/listing/uv4l_repo/raspbian/stretch stretch main
    
## 3. 업데이트 및 업그레이드

    sudo apt-get update && sudo apt-get upgrade

## 4. uv4l 패키지 설치

4-1 Raspberry Pi 2,3,4 인 경우

    sudo apt-get install uv4l-webrtc
    
4-2. Raspberry Pi 1, Compute Module 1, Zero, Zero W 인 경우

    sudo apt-get install uv4l-webrtc-armv6

## 5. 서버 열기

    uv4l --external-driver --device-name=video0
    
![2](/uploads/12dbcdeb3b2835297cbd8e1266901452/2.png)

## 6. 웹에 접속 하기

라즈베리파이 ip주소를 입력하여 외부에서 라즈베리파이 서버로 접속 : http://라즈베리파이IP:8080/stream/webrtc

![KakaoTalk_20210721_194353304](/uploads/37b2ff96879ba9b0f7404d5edd5ac84b/KakaoTalk_20210721_194353304.jpg)