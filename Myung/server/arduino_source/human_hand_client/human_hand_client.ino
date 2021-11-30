String ch = "";

int check_name = 0;
String check_finger_data="";

String finger_1 = "";
int finger_1_name = 1;

String finger_2 = "";
int finger_2_name = 2;

String finger_3 = "";
int finger_3_name = 3;

void setup() {
  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  if(Serial.available()){
    ch = Serial.readStringUntil('\n');
   
    
    check_name = ch.substring(0,1).toInt();
    if(check_name == finger_1_name){
      check_finger_data = ch.substring(1,4);
      finger_1 = check_finger_data;
    }

    check_name = ch.substring(4,5).toInt();
    if(check_name == finger_2_name){
      check_finger_data = ch.substring(5,8);
      finger_2 = check_finger_data;
    }

    check_name = ch.substring(8,9).toInt();
    if(check_name == finger_3_name){
      check_finger_data = ch.substring(9,12);
      finger_3 = check_finger_data;
    }

    Serial.print("엄지 : ");
    Serial.print(finger_1);
    Serial.print(" 검지 : ");
    Serial.print(finger_2);
    Serial.print(" 약지 : ");
    Serial.println(finger_3);
    
    Serial.flush();
    
  }

  

}
