# HM-10 명령어

- 블루투스 연결(페어링) 기능과 관련된 명령어 모음

①장치 스캔 시작(AT+ROLE1 와 AT+IMME1 명령어 실행 후)

- AT+DISC?

⇒OK+DISC[Para]

[Para]: S, E, Address string

S: Start discovery

E: End discovery

최대 6개의 장치 주소가 검색된다.



Example)

Send: AT+DISC?

Recv: OK+DISCS

Recv: OK+DISC:123456789012 (discovered device address information)

 
(만약 AT+SHOW1 명령어가 실행되었다면 , 이름에 대한 정보가 추가로 송신된다.)

Recv: OK+NAME: xxx

After send Name value, will send two extra “\r\n” value ASCII byte

Recv: OK+DISC:234567890123

Recv: OK+NAME: xxx

After send Name value, will send two extra “\r\n” value ASCII byte

…..(Max results is 6, use array 0~5)

Recv: OK+DISCE



→ 명령어 실행 후 발견된 장치와 연결하는 방법 : AT+CONN0, AT+CONN1……AT+CONN5 명령어 실행

 



②스캔 후 발견한 장비와 연결 (AT+DISC? 실행후 사용가능)

​- AT+CONN[Para1]

⇒OK+CONN[Para2]

​[Para1]: 0~5
[Para2]: E, F, 0~5
E: Link error
F: Link failed
0~5: Try to connect





③주소를 통하여 장치 연결시도(Central 역할에서 사용)

- AT+CON[Address]

⇒OK+CONN[Para] 
[Address] : like "0017EA090909"

[Para]: A, E, F

A: Connecting

E: Connect error

F: Connect Fail 
OK+CONNA ========= Accept request, connecting

OK+CONNE ========= Connect error

OK+CONN ========= Connected, if AT+NOTI1 is setup

OK+CONNF ========= Connect Failed, After 10 seconds

​

​

④연결후 연결된 장치의 주소 저장방법 확인/설정​ 
- AT+SAVE?

⇒OK+Get:[Para] 
- AT+SAVE[Para]

​⇒OK+Set:[Para] 
[Para]: 0~1

0 : Save when connected

1 : Don’t Save

Default: 0​

 





⑤마지막으로 연결했던 장치와 재연결(Central 역할에서 사용)

​- AT+CONNL

⇒OK+CONN[Para] 
[Para]: L, E, F, N

L: Connecting

E: Connect error

F: Connect Fail

N: No Address

(이미 다른 장치와 연결되었거나 연결해지된 경우 약 10초동안 OK+CONNN 응답)




​

⑥마지막으로 연결했던 장치주소 확인

- AT+RADD?

⇒ OK+RADD : [MAC Address]



 
⑦Mac address로 연결 시도(Central 역할에서 사용, AT+IMM1 와 AT+ROLE1 명령어 실행 후)

- AT+CO[Para1][para2][para3]

⇒OK+CONN[para1][para2][para3] 
[para1]: N, 1

N: Normal address

1: Dual module address

[para2]: MAC address

Like: 0017EA090909

[para3]: A , E, F

A: Connecting

E: Connect error

F: Connect Fail
If remote device has already connected to other device or shut down, “OK+CONNF” will received after about 10 Seconds.

 

​ 
 ⑧Notify 모드 확인/설정 
- AT+NOTP?

⇒OK+Get:[para]

- AT+NOTP[para]

⇒OK+Set:[para] 
[para]:0, 1

0: Without address

1: With address

Default: 0

AT+NOTP1 실행시, 모듈이 원격 장치와 연결되었을 때, UART통신으로 “OK+CONN:001122334455”와 같은 응답을 전송한다.

여기서 “001122334455” is Central 역할장치의 Mac address이다.

​

​ 

⑨모듈 연결 원격 장치 Timeout 값 확인/설정(Central 역할에서 사용)

- AT+TCON?

⇒OK+TCON:[para]

- AT+TCON[para]

⇒OK+Set:[para]

[para]: 000000~009999

Unit: ms
모듈이 마지막으로 연결된 주소 정보를 가지고 있으면, 전원 인가 후 이 주소로 자동으로 연결을 시도한다. 설정한 시간 값을 넘었을 때, 모듈은 장치 스캔을 시작한다. '000000' 값은 연결을 시도하는것을 유지하도록 한다.