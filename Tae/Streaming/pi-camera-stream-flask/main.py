# main.py
# 필요한 패키지들을 임포트
from flask import Flask, render_template, Response, request
from camera import VideoCamera
import time
import threading
import os

pi_camera = VideoCamera(flip=False) # flip=True 일경우 화면 상하반전

# Flask 인스턴스를 생성한 것이다. python에서 __name__은 모듈의 이름을 뜻한다. 모듈의 이름이란 게
# 애매한 의미로 느껴지는 데, 간단하게 app이라는 파이썬 파일을 만들었다고 생각하자.
app = Flask(__name__)

# @ : 장식자(decorator)
# URL 연결에 활용
# route 메소드 : flask 서버로 '/URL' 요청이 들어오면 어떤 함수를 호출할 것인지 조정
@app.route('/')
def index():
    return render_template('index.html') #index.html을 불러오기

def gen(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

@app.route('/video_feed')
# '/video_feed' : video_feed 함수와 연결
def video_feed():
    # 특정 메시지 유형과 함께 생성된 응답 반환
    # 화면을 작게 보여줌
    return Response(gen(pi_camera),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    # flask 앱 시작
    app.run(host='0.0.0.0', debug=False)