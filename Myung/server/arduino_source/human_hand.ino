int flexpin_num1 = A5; //엄지
int flexpin_num2 = A4; //검지
int flexpin_num3 = A3; //중지
int flexpin_num4 = A2; //약지
int flexpin_num5 = A1; //새끼

void setup() {
  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  int flexVal0, flexVal1, flexVal2, flexVal3, flexVal4;
  float before_flex[5];
  int flex[5];
  int flex_int;

  flexVal0 = analogRead(flexpin_num1); //엄지
  flexVal1 = analogRead(flexpin_num2); //검지
  flexVal2 = analogRead(flexpin_num3); //중지
  flexVal3 = analogRead(flexpin_num4); //약지
  flexVal4 = analogRead(flexpin_num5); //새끼

  before_flex[0] = flexVal0 / 10;
  before_flex[1] = flexVal1 / 10;
  before_flex[2] = flexVal2 / 10;
  before_flex[3] = flexVal3 / 10;
  before_flex[4] = flexVal4 / 10;

  for(int i=0; i<5; i++){
    flex_int = int(before_flex[i]);
    if((before_flex[i] - flex_int)>0.5){
      flex[i] = int(before_flex[i]) + 1;
    }
    else{
      flex[i] = int(before_flex[i]);
    }
  }

  Serial.print("1");
  Serial.print("40");
  Serial.print(" ");
  Serial.print("2");
  Serial.print(flex[1]);
  Serial.print(" ");
  Serial.print("3");
  Serial.print(flex[2]);
  Serial.print(" ");
  Serial.print("4");
  Serial.print(flex[3]);
  Serial.print(" ");
  Serial.print("5");
  Serial.println(flex[4]);

  delay(450);

}
