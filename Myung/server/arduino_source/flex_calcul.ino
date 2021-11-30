
int flex[] = {A1,A2,A3,A4,A5};
float flex_val[] = {1,2,3,4,5};
float lock_val[] = {0,0,0,0,0};
int new_val[] = {0,0,0,0,0};

float sensitivity = 0.25;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lock_val[0] = analogRead(flex[0]);
  lock_val[1] = analogRead(flex[1]);
  lock_val[2] = analogRead(flex[2]);
  lock_val[3] = analogRead(flex[3]);
  lock_val[4] = analogRead(flex[4]);
}

void loop() {
  for(int i=0; i<sizeof(flex_val)/sizeof(flex_val[0]); i++){
    flex_val[i] = analogRead(flex[i]);
    new_val[i] = int(lock_val[i] * (1 - sensitivity) + flex_val[i] * sensitivity);
    //로우패스 필터 (고정값을 계속 0.9배하고 새로 감지한값의 10%만 변수에 더해서 나중에는 10%의 값만 이용해서 정확도를 높이는 방법)
  }

  new_val[0] = map(new_val[0],16,35,0,100);
  new_val[1] = map(new_val[1],22,40,0,100);
  new_val[2] = map(new_val[2],26,48,0,100);
  new_val[3] = map(new_val[3],92,110,0,100);
  new_val[4] = map(new_val[4],87,110,0,100);

  for(int j=0; j<5; j++){
    Serial.print(new_val[j]);
    Serial.print(" ");
  }
  Serial.println("");
  delay(200);



}