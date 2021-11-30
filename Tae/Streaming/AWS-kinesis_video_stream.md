# 라즈베리파이 - 실시간 웹 스트리밍(kinesis video stream)

amazon kinesis video stream 다운로드

좀 더 자세한 설명은 https://github.com/awslabs/amazon-kinesis-video-streams-producer-sdk-cpp 에 들어가게 되면 상세하게 알려준다.

    git clone --recursive https://github.com/awslabs/amazon-kinesis-video-streams-producer-sdk-cpp.git
    
우선 git clone을 통해 git 파일을 받아준다.

git을 받으면 이러한 폴더가 생성된것을 볼 수 있다.

    sudo mkdir -p amazon-kinesis-video-streams-producer-sdk-cpp/build
    cd amazon-kinesis-video-streams-producer-sdk-cpp/build
    
이전으로 돌아와 해당 명령어를 통해서 build  폴더를 만들고 build 위치로 이동하면 된다.

해당 위치에서 

    cmake ..

해당 명령어를 실행해주면 된다.

성공적으로 설치가 된다. 그뒤에는 라이브러리를 설치해주면 된다. 필요 라이브러리는 다음과 같다.


    sudo apt-get install libssl-dev libcurl4-openssl-dev liblog4cplus-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev gstreamer1.0-plugins-base-apps gstreamer1.0-plugins-bad gstreamer1.0-plugins-good gstreamer1.0-plugins-ugly gstreamer1.0-tools

라이브러리를 설치가 완료 되었다면

상위 폴더로 올라와서

위와 같이 make 커맨드를 이용해주면 기본적으로 필요한 예제 코드나 빌드 파일들을 설치하게 된다.

예제 실행하기에 앞서서 환경 변수를 설정해준다. 설정하지 않으면 오류가 발생한다.

    export GST_PLUGIN_PATH=`pwd`/build
    export LD_LIBRARY_PATH=`pwd`/open-source/local/lib

gst-inspect-1.0 kvssink 해당 명령어를 실행하게 되면 아래와 같은 정보가 나오면 설치가 무사히 끝난것이다.(환경변수가 완료된 상태여야 해당 명령어를 사용 할 수 있다.

![10](/uploads/96288084cdf9bf78cf800687735b1d5d/10.png)

빌드가 실패했거나 GST_PLUGIN_PATH제대로 설정되지 않은 경우 다음 과 같은 출력이 표시된다.

    No such element or plugin 'kvssink'
    
성공적으로 완료가 되었다면

기본적으로 명령어로 실행이 가능하다. aws에서 비디오 스트리밍을 설정을 해두었다면

![11](/uploads/14f4b7b007ae52cf25dbcad46b2fc320/11.png)

이렇게 명령어를 입력하게 되면

![1](/uploads/c8030b83a78c315b11a87d525064e4eb/1.png)

위와 같이 실행 화면이 나타나게 된다.

![4](/uploads/bdf1ea82e052cfcd78d98eb7ab927d1f/4.png)

아마존 서비스에 들어가게 되면 스트림이 생성되는것을 볼수 있다. 이를 통해서 명령어를 통해서 스트림 내용을 생성하는 것도 가능하다라는 것을 알 수 있다.

![2](/uploads/d3879fa7a0f8afaf7edb8c06502b88d1/2.png)

이렇게 재생되는 화면이 보인다면 정상적으로 된것이다.

스트림을 웹상에서 볼 수도 있는데 아래 사이트로 들어가서 필요한 부분에만 값을 넣어주면 확인할 수 있다.

https://aws-samples.github.io/amazon-kinesis-video-streams-media-viewer/


